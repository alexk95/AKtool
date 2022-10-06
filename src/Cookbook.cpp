#include "Cookbook.h"
#include "AppBase.h"
#include "TabToolbar.h"
#include "NavigationManager.h"
#include "NewNameDialog.h"
#include "Settings.h"

#include <akAPI/uiAPI.h>
#include <akCore/aJSONHelper.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aSpinBoxWidget.h>
#include <akWidgets/aTableWidget.h>
#include <akWidgets/aAnimationOverlayWidget.h>
#include <akWidgets/aWindow.h>

#include <qsplitter.h>
#include <qlayout.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qfile.h>

enum TableColumns {
	tcCount
};

using namespace ak;

QString CookbookIngredient::ingredientCountTypeToString(IngredientCountType _type) {
	switch (_type)
	{
	case CookbookIngredient::GRAMS: return "Gram";
	case CookbookIngredient::MILLILITERS: return "Milliliter";
	case CookbookIngredient::LITERS: return "Liter";
	case CookbookIngredient::COUNT: return "Count";
	default: aAssert(0, "Unknown ingredient count type"); return "";
	}
}

CookbookIngredient::IngredientCountType CookbookIngredient::stringToIngredientCountType(const QString& _type) {
	if (_type == ingredientCountTypeToString(GRAMS)) return GRAMS;
	else if (_type == ingredientCountTypeToString(MILLILITERS)) return MILLILITERS;
	else if (_type == ingredientCountTypeToString(LITERS)) return LITERS;
	else if (_type == ingredientCountTypeToString(COUNT)) return COUNT;
	else { aAssert(0, "Unknown ingredient count type"); return COUNT; }
}

CookbookIngredient::CookbookIngredient() : m_Count(0.), m_CountType(COUNT) {}

CookbookIngredient::~CookbookIngredient() {}

bool CookbookIngredient::importData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "Name", JSONHelper::String);
	A_JSON_CHECK_MEMBER(_object, "Count", JSONHelper::Double);
	A_JSON_CHECK_MEMBER(_object, "CountType", JSONHelper::String);

	m_Name = _object["Name"].toString();
	m_Count = _object["Count"].toDouble();
	m_CountType = stringToIngredientCountType(_object["CountType"].toString());

	return true;
}

void CookbookIngredient::exportData(QJsonObject& _object) {
	_object["Name"] = m_Name;
	_object["Count"] = m_Count;
	_object["CountType"] = ingredientCountTypeToString(m_CountType);
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

CookbookItem::CookbookItem(void) : m_NavigationId(-1), m_ParentGroup(nullptr) {}

CookbookItem::~CookbookItem() {}

bool CookbookItem::importData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "ItemName", JSONHelper::String);
	A_JSON_CHECK_MEMBER(_object, "DefaultPortions", JSONHelper::Double);
	A_JSON_CHECK_MEMBER(_object, "Description", JSONHelper::String);
	A_JSON_CHECK_MEMBER(_object, "Ingredients", JSONHelper::Array);

	m_ItemName = _object["ItemName"].toString();
	m_DefaultPortions = _object["DefaultPortions"].toInt();
	m_Description = _object["Description"].toString();
	
	QJsonArray ingredients = _object["Ingredients"].toArray();
	for (int i = 0; i < ingredients.count(); i++) {
		if (!ingredients.at(i).isObject()) { aAssert(0, "JSON Array entry is not an Object"); return false; }

		QJsonObject ingredientObj = ingredients[i].toObject();
		CookbookIngredient * newIngredient = new CookbookIngredient;
		m_Ingredients.push_back(newIngredient);
		if (!newIngredient->importData(ingredientObj)) return false;
	}

	return true;
}

void CookbookItem::exportData(QJsonObject& _object) {
	_object["ItemName"] = m_ItemName;
	_object["DefaultPortions"] = m_DefaultPortions;
	_object["Description"] = m_Description;

	QJsonArray ingredients;
	for (auto itm : m_Ingredients) {
		QJsonObject ingredientObj;
		itm->exportData(ingredientObj);
		ingredients.push_back(ingredientObj);
	}

	_object["Ingredients"] = ingredients;
}

void CookbookItem::displayData(void) {

}

QString CookbookItem::navigationItemPath(void) {
	if (m_ParentGroup == nullptr) {
		aAssert(0, "No parent group set");
	}
	return (m_ParentGroup->navigationItemPath() + "|" + m_ItemName);
}

void CookbookItem::addToNavigationTree(void) {
	m_NavigationId = AppBase::instance()->navigationManager()->addItem(navigationItemPath(), "Cake", "Tree");
}

void CookbookItem::removeFromNavigationTree(void) {
	AppBase::instance()->navigationManager()->removeItem(m_NavigationId);
}

Cookbook * CookbookItem::getOwner(void) {
	if (m_ParentGroup) return m_ParentGroup->determineOwner();
	else {
		aAssert(0, "No parent group set");
		return nullptr;
	}
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

CookbookGroup::CookbookGroup() : m_ParentGroup(nullptr), m_Owner(nullptr) {}

CookbookGroup::CookbookGroup(const QString& _title) : m_ParentGroup(nullptr), m_Owner(nullptr), m_Title(_title) {}

CookbookGroup::~CookbookGroup() {}

bool CookbookGroup::importData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "Title", JSONHelper::String);
	A_JSON_CHECK_MEMBER(_object, "Items", JSONHelper::Array);
	A_JSON_CHECK_MEMBER(_object, "Subgroups", JSONHelper::Array);

	m_Title = _object["Title"].toString();

	QJsonArray items = _object["Items"].toArray();
	for (int i = 0; i < items.count(); i++) {
		if (!items.at(i).isObject()) { aAssert(0, "JSON Array entry is not an Object"); return false; }

		QJsonObject itemObj = items.at(i).toObject();
		CookbookItem * newItem = new CookbookItem;
		newItem->setParentGroup(this);
		if (!newItem->importData(itemObj)) return false;
		addItem(newItem);

		determineOwner()->addItemToList(newItem->NavigationId(), newItem);
	}

	QJsonArray groups = _object["Subgroups"].toArray();
	for (int i = 0; i < groups.count(); i++) {
		if (!groups.at(i).isObject()) { aAssert(0, "JSON Array entry is not an Object"); return false; }

		CookbookGroup * newGroup = new CookbookGroup;
		newGroup->setParentGroup(this);
		if (!newGroup->importData(groups.at(i).toObject())) return false;
		addSubgroup(newGroup);

		determineOwner()->addGroupToList(newGroup->NavigationId(), newGroup);
	}

	return true;
}

void CookbookGroup::exportData(QJsonObject& _object) {
	_object["Title"] = m_Title;

	QJsonArray items;
	for (auto item : m_Items) {
		QJsonObject itemObj;
		item->exportData(itemObj);
		items.push_back(itemObj);
	}
	_object["Items"] = items;

	QJsonArray groups;
	for (auto group : m_Subgroups) {
		QJsonObject groupObj;
		group->exportData(groupObj);
		groups.push_back(groupObj);
	}
	_object["Subgroups"] = groups;
}

CookbookGroup * CookbookGroup::addSubgroup(const QString& _title) {
	CookbookGroup * newGroup = new CookbookGroup(_title);
	this->addSubgroup(newGroup);
	return newGroup;
}

void CookbookGroup::addSubgroup(CookbookGroup * _group) {
	m_Subgroups.push_back(_group);
	_group->setParentGroup(this);
	_group->addToNavigationTree();
}

void CookbookGroup::addItem(CookbookItem * _item) {
	m_Items.push_back(_item);
	_item->setParentGroup(this);
	_item->addToNavigationTree();
}

void CookbookGroup::removeItemFromList(CookbookItem * _item) {
	auto it = std::find(m_Items.begin(), m_Items.end(), _item);
	if (it != m_Items.end()) m_Items.erase(it);
}

void CookbookGroup::removeGroupFromList(CookbookGroup * _group) {
	auto it = std::find(m_Subgroups.begin(), m_Subgroups.end(), _group);
	if (it != m_Subgroups.end()) m_Subgroups.erase(it);
}

QString CookbookGroup::navigationItemPath(void) {
	QString path = m_Title;
	if (m_ParentGroup) {
		path = "|" + path;
		m_ParentGroup->appendNavigationItemPath(path);
	}
	return path;
}

void CookbookGroup::addToNavigationTree(void) {
	if (m_ParentGroup) m_NavigationId = AppBase::instance()->navigationManager()->addItem(navigationItemPath(), "Folder", "Tree");
	else m_NavigationId = AppBase::instance()->navigationManager()->addRootItem(m_Title, "Folder", "Tree", m_Owner);
}

void CookbookGroup::removeFromNavigationTree(void) {
	AppBase::instance()->navigationManager()->removeItem(m_NavigationId);
}

Cookbook * CookbookGroup::determineOwner(void) {
	if (m_ParentGroup) return m_ParentGroup->determineOwner();
	else if (m_Owner) return m_Owner;
	else {
		aAssert(0, "No parent group or owner set");
		return nullptr;
	}
}

void CookbookGroup::clearAllChilds(void) {
	for (auto itm : m_Items) {
		itm->removeFromNavigationTree();
		delete itm;
	}
	m_Items.clear();

	for (auto group : m_Subgroups) {
		group->clearAllChilds();
		group->removeFromNavigationTree();
		delete group;
	}
	m_Subgroups.clear();
}

void CookbookGroup::appendNavigationItemPath(QString& _itemPath) {
	_itemPath = m_Title + _itemPath;
	if (m_ParentGroup) {
		_itemPath = "|" + _itemPath;
		m_ParentGroup->appendNavigationItemPath(_itemPath);
	}
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

QString Cookbook::TabText(void) {
	return "Cookbook";
}

Cookbook::Cookbook()
	: aWidget(otNone), m_hasUnsavedChanges(false)
{
	m_centralSplitter = new QSplitter(Qt::Horizontal);

	m_leftLayoutW = new QWidget;
	m_leftLayout = new QVBoxLayout(m_leftLayoutW);

	m_portionsLayoutW = new QWidget;
	m_portionsLayout = new QHBoxLayout(m_portionsLayoutW);

	m_description = new aTextEditWidget;
	m_portionsLabel = new aLabelWidget("Portions");
	m_portions = new aSpinBoxWidget(1);
	m_portions->setMinimum(1);
	m_portions->setMaximum(99);

	m_ingredientsContainer = new aAnimationOverlayWidget;
	m_ingredients = new aTextEditWidget;
	m_ingredientsTable = new aTableWidget(0, tcCount);

	// Setup layouts
	m_centralSplitter->addWidget(m_leftLayoutW);
	m_centralSplitter->addWidget(m_description);

	m_leftLayout->addWidget(m_portionsLayoutW, 0);
	m_leftLayout->addWidget(m_ingredientsContainer, 1);

	m_portionsLayout->addStretch(1);
	m_portionsLayout->addWidget(m_portionsLabel);
	m_portionsLayout->addWidget(m_portions);

	m_ingredientsContainer->setChild(m_ingredients);

	// Setup navigation
	m_navigationRoot = new CookbookGroup("Cookbook");
	m_navigationRoot->setOwner(this);
	m_navigationRoot->addToNavigationTree();
	m_groups.insert_or_assign(m_navigationRoot->NavigationId(), m_navigationRoot);
}

Cookbook::~Cookbook() {

}

// ###################################################################################

// Base class functions

QWidget * Cookbook::widget(void) {
	return m_centralSplitter;
}

void Cookbook::setColorStyle(ak::aColorStyle * _colorStyle) {
	m_description->setColorStyle(_colorStyle);
	m_portionsLabel->setColorStyle(_colorStyle);
	m_portions->setColorStyle(_colorStyle);
	m_ingredients->setColorStyle(_colorStyle);
	m_ingredientsTable->setColorStyle(_colorStyle);
}

void Cookbook::navigationItemSelected(int _itemID) {
	resetView();

	auto item = m_items.find(_itemID);
	auto group = m_groups.find(_itemID);

	if (item != m_items.end()) {
		m_currentItem = item->second;
		m_currentItem->displayData();
		AppBase::instance()->ttb()->setCookbookStateAfterItemSelected();
	}
	else if (group != m_groups.end()) {
		m_currentGroup = group->second;
		AppBase::instance()->ttb()->setCookbookStateAfterGroupSelected();
	}
	else {
		aAssert(0, "Unknown navigation item");
		navigationSelectionCleared();
	}
}

void Cookbook::navigationSelectionCleared(void) {
	resetView();
	AppBase::instance()->ttb()->resetCookbookStateAfterSelectionCleared();
}

bool Cookbook::saveData(bool _showPromtOnError) {
	// Collect data
	QJsonObject obj;
	QJsonObject dataObj;
	m_navigationRoot->exportData(dataObj);
	obj["Data"] = dataObj;

	QJsonDocument doc(obj);

	// Delete old file
	QString filename = Settings::instance()->dataPath() + "/CookbookData.json";

	if (QFile::exists(filename)) {
		QFile f(filename);
		if (!f.remove()) {
			if (_showPromtOnError) uiAPI::promptDialog::show("Failed to delete old data file", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
			return false;
		}
	}

	// Write new file
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		if (_showPromtOnError) uiAPI::promptDialog::show("Failed to open file for writing", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}

	file.write(doc.toJson(QJsonDocument::Indented));
	file.close();

	m_hasUnsavedChanges = false;

	return true;
}

bool Cookbook::loadData(void) {
	// Clear data
	m_navigationRoot->clearAllChilds();

	m_items.clear();
	m_groups.clear();
	m_groups.insert_or_assign(m_navigationRoot->NavigationId(), m_navigationRoot);

	// Open file
	QString filename = Settings::instance()->dataPath() + "/CookbookData.json";

	if (!QFile::exists(filename)) return true;

	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		uiAPI::promptDialog::show("Failed to open file for reading", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}

	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	file.close();

	// Check document members
	if (!doc.isObject()) {
		uiAPI::promptDialog::show("The notebook data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}

	QJsonObject docObj = doc.object();
	A_JSON_CHECK_MEMBER(docObj, "Data", JSONHelper::Object);
	AppBase::instance()->navigationManager()->setInitializationMode();
	bool ret = m_navigationRoot->importData(docObj["Data"].toObject());
	AppBase::instance()->navigationManager()->removeInitializationMode();
	return ret;
}

void Cookbook::clearNavigationTree(void) {
	m_navigationRoot->clearAllChilds();
	AppBase::instance()->navigationManager()->removeItem(m_navigationRoot->NavigationId());
}

void Cookbook::addGroup(void) {
	if (m_currentGroup == nullptr) {
		aAssert(0, "No parent group selected");
		return;
	}

	NewNameDialog dia("Group Name", "Group name", "Create");
	if (dia.showDialog() == resultOk) {
		CookbookGroup * newGroup = m_currentGroup->addSubgroup(dia.inputText());
		newGroup->setParentGroup(m_currentGroup);
		newGroup->addToNavigationTree();
		m_groups.insert_or_assign(newGroup->NavigationId(), newGroup);

		slotDataChanged();
	}
}

void Cookbook::addGroupToList(int _navigationId, CookbookGroup * _group) {
	m_groups.insert_or_assign(_navigationId, _group);
}

void Cookbook::addItem(void) {
	if (m_currentGroup == nullptr) {
		aAssert(0, "No parent group selected");
		return;
	}

	NewNameDialog dia("Item Name", "Item name", "Create");
	if (dia.showDialog() == resultOk) {
		CookbookItem * newItem = new CookbookItem;
		newItem->setItemName(dia.inputText());
		m_currentGroup->addItem(newItem);
		m_items.insert_or_assign(newItem->NavigationId(), newItem);

		slotDataChanged();
	}
}

void Cookbook::addItemToList(int _navigationId, CookbookItem * _item) {
	m_items.insert_or_assign(_navigationId, _item);
}

void Cookbook::deleteSelected(void) {
	if (m_currentItem) {
		CookbookItem * itm = m_currentItem;
		itm->removeFromNavigationTree();
		m_items.erase(itm->NavigationId());
		itm->ParentGroup()->removeItemFromList(itm);
		delete itm;
		m_currentItem = nullptr;
	}
	else if (m_currentGroup) {
		CookbookGroup * group = m_currentGroup;
		group->clearAllChilds();
		if (group == m_navigationRoot) { navigationSelectionCleared(); slotDataChanged(); return; }

		group->removeFromNavigationTree();
		m_groups.erase(group->NavigationId());
		if (group->ParentGroup()) group->ParentGroup()->removeGroupFromList(group);
		delete group;
		m_currentGroup = nullptr;
	}
	navigationSelectionCleared();
	slotDataChanged();
}

void Cookbook::slotDataChanged(void) {
	if (m_currentItem) {
		aAssert(0, "Refresh item data");
	}

	m_hasUnsavedChanges = true;
	AppBase::instance()->ttb()->setCookbookSaveButtonEnabled(true);
}

void Cookbook::resetView(void) {
	m_ingredientsTable->clearRows();
	m_ingredients->clear();
	m_description->clear();

	m_currentGroup = nullptr;
	m_currentItem = nullptr;
}