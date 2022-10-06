#include "Notebook.h"
#include "AppBase.h"
#include "NavigationManager.h"
#include "TabToolbar.h"
#include "NewNameDialog.h"
#include "Settings.h"

#include "aTextEditorWidget.h"
#include <akAPI/uiAPI.h>
#include <akCore/aAssert.h>
#include <akCore/aJSONHelper.h>
#include <akWidgets/aWindow.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aAnimationOverlayWidget.h>
#include <akWidgets/aChecklistWidget.h>
#include <akWidgets/aTtbPage.h>

#include <qlayout.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qfile.h>

using namespace ak;

QString AbstractNotebookItem::itemTypeToString(NotebookItemType _type) {
	switch (_type)
	{
	case AbstractNotebookItem::Text: return "Text";
	case AbstractNotebookItem::Checklist: return "Checklist";
	default:
		aAssert(0, "Unknown item type");
		return "";
	}
}

bool AbstractNotebookItem::stringToItemType(const QString& _string, NotebookItemType& _type) {
	if (_string == itemTypeToString(Text)) { _type = Text; return true; }
	if (_string == itemTypeToString(Checklist)) { _type = Checklist; return true; }
	aAssert(0, "Unknown item type");
	return false;
}

AbstractNotebookItem::AbstractNotebookItem(NotebookItemType _type)
	: m_ItemType(_type), m_NavigationId(-1), m_ParentGroup(nullptr) {}
AbstractNotebookItem::~AbstractNotebookItem() {}

QString AbstractNotebookItem::navigationItemPath(void) {
	if (m_ParentGroup == nullptr) {
		aAssert(0, "No parent group set");
	}
	return (m_ParentGroup->navigationItemPath() + "|" + m_ItemName);
}

void AbstractNotebookItem::addToNavigationTree(void) {
	QString iconName;
	switch (m_ItemType)
	{
	case AbstractNotebookItem::Text: iconName = "EditText"; break;
	case AbstractNotebookItem::Checklist: iconName = "Checklist"; break;
	default: aAssert(0, "Unknown item type"); break;
	}
	m_NavigationId = AppBase::instance()->navigationManager()->addItem(navigationItemPath(), iconName, "Tree");
}

void AbstractNotebookItem::removeFromNavigationTree(void) {
	AppBase::instance()->navigationManager()->removeItem(m_NavigationId);
}

Notebook * AbstractNotebookItem::getOwner(void) {
	if (m_ParentGroup) return m_ParentGroup->determineOwner();
	else {
		aAssert(0, "No parent group set");
		return nullptr;
	}
}

bool AbstractNotebookItem::importGeneralItemData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "ItemName", JSONHelper::String);
	m_ItemName = _object["ItemName"].toString();
	return true;
}

void AbstractNotebookItem::exportGeneralItemData(QJsonObject& _object) {
	_object["ItemName"] = m_ItemName;
	_object["Type"] = itemTypeToString(m_ItemType);
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

NotebookTextItem::NotebookTextItem() : AbstractNotebookItem(AbstractNotebookItem::Text) {}

NotebookTextItem::NotebookTextItem(const QString& _text)
	: AbstractNotebookItem(AbstractNotebookItem::Text), m_Text(_text) {}

NotebookTextItem::~NotebookTextItem() {}

// #####################################################################################

// Base class functions

bool NotebookTextItem::importItemData(const QJsonObject& _object) {
	if (!importGeneralItemData(_object)) return false;

	A_JSON_CHECK_MEMBER(_object, "Text", JSONHelper::String);
	m_Text = _object["Text"].toString();
	
	return true;
}

void NotebookTextItem::exportItemData(QJsonObject& _object) {
	exportGeneralItemData(_object);
	_object["Text"] = m_Text;
}

void NotebookTextItem::displayData(void) {
	Notebook * owner = getOwner();
	if (owner == nullptr) return;

	owner->TextWidget()->setText(m_Text);
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

NotebookChecklistEntry::NotebookChecklistEntry() : m_Done(false), m_Important(false) {}

NotebookChecklistEntry::NotebookChecklistEntry(const NotebookChecklistEntry& _other)
	: m_Done(_other.m_Done), m_Important(_other.m_Important), m_Text(_other.m_Text)
{}

NotebookChecklistEntry::NotebookChecklistEntry(const QString& _text, bool _isDone, bool _isImportant)
	: m_Done(_isDone), m_Important(_isImportant), m_Text(_text)
{}

NotebookChecklistEntry& NotebookChecklistEntry::operator = (const NotebookChecklistEntry& _other) {
	m_Done = _other.m_Done;
	m_Important = _other.m_Important;
	m_Text = _other.m_Text;

	return *this;
}

NotebookChecklistEntry::~NotebookChecklistEntry() {}

bool NotebookChecklistEntry::importData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "Text", JSONHelper::String);
	A_JSON_CHECK_MEMBER(_object, "IsDone", JSONHelper::Bool);
	A_JSON_CHECK_MEMBER(_object, "IsImportant", JSONHelper::Bool);

	m_Text = _object["Text"].toString();
	m_Done = _object["IsDone"].toBool();
	m_Important = _object["IsImportant"].toBool();

	return true;
}

void NotebookChecklistEntry::exportData(QJsonObject& _object) {
	_object["Text"] = m_Text;
	_object["IsDone"] = m_Done;
	_object["IsImportant"] = m_Important;
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

NotebookChecklistItem::NotebookChecklistItem() 
	: AbstractNotebookItem(AbstractNotebookItem::Checklist)
{

}

NotebookChecklistItem::~NotebookChecklistItem() {

}

// #####################################################################################

// Base class functions

bool NotebookChecklistItem::importItemData(const QJsonObject& _object) {
	if (!importGeneralItemData(_object)) return false;

	A_JSON_CHECK_MEMBER(_object, "Entries", JSONHelper::Array);
	m_Entries.clear();
	auto entries = _object["Entries"].toArray();
	for (int i = 0; i < entries.count(); i++) {
		if (!entries.at(i).isObject()) { aAssert(0, "JSON Array entry is not an Object"); return false; }
		NotebookChecklistEntry * newEntry = new NotebookChecklistEntry;
		if (!newEntry->importData(entries.at(i).toObject())) return false;
		m_Entries.push_back(newEntry);
	}
	return true;
}

void NotebookChecklistItem::exportItemData(QJsonObject& _object) {
	exportGeneralItemData(_object);

	QJsonArray entries;
	for (auto entry : m_Entries) {
		QJsonObject entryData;
		entry->exportData(entryData);
		entries.push_back(entryData);
	}
	_object["Entries"] = entries;
}

void NotebookChecklistItem::displayData(void) {
	Notebook * owner = getOwner();
	if (owner == nullptr) return;

	aChecklistWidget * list = owner->ListWidget();

	list->clear(false);

	for (auto entry : m_Entries) {
		list->addItem(entry->Text(), entry->isDone());
	}
	if (m_Entries.empty()) list->addItem("", false);
}

void NotebookChecklistItem::refreshData(const std::vector<ak::aChecklistItem *>& _items) {
	for (auto entry : m_Entries) {
		delete entry;
	}
	m_Entries.clear();
	m_Entries.reserve(_items.size());

	for (auto entry : _items) {
		m_Entries.push_back(new NotebookChecklistEntry(entry->text(), entry->isChecked(), false));
	}
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

NotebookGroup::NotebookGroup() : m_ParentGroup(nullptr), m_Owner(nullptr) {}

NotebookGroup::NotebookGroup(const QString& _title) : m_ParentGroup(nullptr), m_Title(_title), m_Owner(nullptr) {}

NotebookGroup::~NotebookGroup() {
	for (auto item : m_Items) delete item;
	for (auto group : m_Subgroups) delete group;
}

bool NotebookGroup::importData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "Title", JSONHelper::String);
	A_JSON_CHECK_MEMBER(_object, "Items", JSONHelper::Array);
	A_JSON_CHECK_MEMBER(_object, "Subgroups", JSONHelper::Array);

	m_Title = _object["Title"].toString();

	QJsonArray items = _object["Items"].toArray();
	for (int i = 0; i < items.count(); i++) {
		if (!items.at(i).isObject()) { aAssert(0, "JSON Array entry is not an Object"); return false; }
		
		QJsonObject itemObj = items.at(i).toObject();
		if (!itemObj.contains("Type")) { aAssert(0, "JSON Object member \"Type\" missing"); return false; }
		if (!itemObj["Type"].isString()) { aAssert(0, "JSON Object member \"Type\" is not a String"); return false; }
		
		AbstractNotebookItem::NotebookItemType type;
		if (!AbstractNotebookItem::stringToItemType(itemObj["Type"].toString(), type)) return false;
		
		AbstractNotebookItem * newItem = nullptr;
		switch (type)
		{
		case AbstractNotebookItem::Text: newItem = new NotebookTextItem; break;
		case AbstractNotebookItem::Checklist: newItem = new NotebookChecklistItem; break;
		default: aAssert(0, "Unknown item type"); break;
		}
		if (newItem == nullptr) return false;
		newItem->setParentGroup(this);
		if (!newItem->importItemData(itemObj)) return false;
		addItem(newItem);

		determineOwner()->addItemToList(newItem->NavigationId(), newItem);
	}

	QJsonArray groups = _object["Subgroups"].toArray();
	for (int i = 0; i < groups.count(); i++) {
		if (!groups.at(i).isObject()) { aAssert(0, "JSON Array entry is not an Object"); return false; }
		
		NotebookGroup * newGroup = new NotebookGroup;
		newGroup->setParentGroup(this);
		if (!newGroup->importData(groups.at(i).toObject())) return false;
		addSubgroup(newGroup);

		determineOwner()->addGroupToList(newGroup->NavigationId(), newGroup);
	}

	return true;
}

void NotebookGroup::exportData(QJsonObject& _object) {
	_object["Title"] = m_Title;

	QJsonArray items;
	for (auto item : m_Items) {
		QJsonObject itemObj;
		item->exportItemData(itemObj);
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

NotebookGroup * NotebookGroup::addSubgroup(const QString& _title) {
	NotebookGroup * newGroup = new NotebookGroup(_title);
	this->addSubgroup(newGroup);
	return newGroup;
}

void NotebookGroup::addSubgroup(NotebookGroup * _group) {
	m_Subgroups.push_back(_group);
	_group->setParentGroup(this);
	_group->addToNavigationTree();
}

void NotebookGroup::addItem(AbstractNotebookItem * _item) {
	m_Items.push_back(_item);
	_item->setParentGroup(this);
	_item->addToNavigationTree();
}

void NotebookGroup::removeItemFromList(AbstractNotebookItem * _item) {
	auto it = std::find(m_Items.begin(), m_Items.end(), _item);
	if (it != m_Items.end()) m_Items.erase(it);
}

void NotebookGroup::removeGroupFromList(NotebookGroup * _group) {
	auto it = std::find(m_Subgroups.begin(), m_Subgroups.end(), _group);
	if (it != m_Subgroups.end()) m_Subgroups.erase(it);
}

QString NotebookGroup::navigationItemPath(void) {
	QString path = m_Title;
	if (m_ParentGroup) {
		path = "|" + path;
		m_ParentGroup->appendNavigationItemPath(path);
	}
	return path;
}

void NotebookGroup::addToNavigationTree(void) {
	if (m_ParentGroup) m_NavigationId = AppBase::instance()->navigationManager()->addItem(navigationItemPath(), "Folder", "Tree");
	else m_NavigationId = AppBase::instance()->navigationManager()->addRootItem(m_Title, "Folder", "Tree", m_Owner);
}

void NotebookGroup::removeFromNavigationTree(void) {
	AppBase::instance()->navigationManager()->removeItem(m_NavigationId);
}

Notebook * NotebookGroup::determineOwner(void) {
	if (m_ParentGroup) return m_ParentGroup->determineOwner();
	else if (m_Owner) return m_Owner;
	else {
		aAssert(0, "No parent group or owner set");
		return nullptr;
	}
}

void NotebookGroup::clearAllChilds(void) {
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

// ######################################################################################

// Private functions

void NotebookGroup::appendNavigationItemPath(QString& _itemPath) {
	_itemPath = m_Title + _itemPath;
	if (m_ParentGroup) {
		_itemPath = "|" + _itemPath;
		m_ParentGroup->appendNavigationItemPath(_itemPath);
	}
}

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

QString Notebook::TabText() { return "Notebook"; }

Notebook::Notebook() : aWidget(otNone), m_currentItem(nullptr), m_currentGroup(nullptr), m_hasUnsavedChanges(false) {
	m_RootGroup = new NotebookGroup("Notebook");
	m_RootGroup->setOwner(this);
	m_RootGroup->addToNavigationTree();
	m_groups.insert_or_assign(m_RootGroup->NavigationId(), m_RootGroup);

	m_centralWidget = new aAnimationOverlayWidget;
	m_placeholder = new aLabelWidget;
	m_centralWidget->setChild(m_placeholder);

	m_TextWidget = new aTextEditorWidget;
	m_ListWidget = new aChecklistWidget;
	m_ListWidget->setCrossoutCheckedItemsEnabled(true);
	m_ListWidget->setPointSize(Settings::instance()->notebookChecklistPointSize());

	connect(m_ListWidget, &aChecklistWidget::itemChanged, this, &Notebook::slotDataChanged);
	connect(m_ListWidget, &aChecklistWidget::itemRemoved, this, &Notebook::slotDataChanged);
	connect(m_ListWidget, &aChecklistWidget::itemOrderChanged, this, &Notebook::slotDataChanged);
	connect(m_TextWidget, &aTextEditorWidget::textChanged, this, &Notebook::slotDataChanged);
}

Notebook::~Notebook() {
	delete m_RootGroup;

	delete m_TextWidget;
	delete m_ListWidget;
	delete m_placeholder;
	delete m_centralWidget;
}

QWidget * Notebook::widget() { return m_centralWidget; }

void Notebook::setColorStyle(ak::aColorStyle * _colorStyle) {
	m_TextWidget->setColorStyle(_colorStyle);
	m_ListWidget->setColorStyle(_colorStyle);
}

void Notebook::navigationItemSelected(int _itemID) {
	resetView();

	auto item = m_items.find(_itemID);
	auto group = m_groups.find(_itemID);

	if (item != m_items.end()) {
		m_currentItem = item->second;
		switch (m_currentItem->ItemType())
		{
		case AbstractNotebookItem::Checklist: m_centralWidget->setChild(m_ListWidget->widget()); break;
		case AbstractNotebookItem::Text: m_centralWidget->setChild(m_TextWidget->widget()); break;
		default: aAssert(0, "Unknown item type"); break;
		}

		m_currentItem->displayData();
		AppBase::instance()->ttb()->setNotebookStateAfterItemSelected();
	}
	else if (group != m_groups.end()) {
		m_currentGroup = group->second;
		AppBase::instance()->ttb()->setNotebookStateAfterGroupSelected();
	}
	else {
		aAssert(0, "Unknown navigation item");
		navigationSelectionCleared();
	}
}

void Notebook::navigationSelectionCleared(void) {
	resetView();
	AppBase::instance()->ttb()->resetNotebookStateAfterSelectionCleared();
}

bool Notebook::importData(const QJsonObject& _object) {
	A_JSON_CHECK_MEMBER(_object, "Data", JSONHelper::Object);
	AppBase::instance()->navigationManager()->setInitializationMode();
	bool ret = m_RootGroup->importData(_object["Data"].toObject());
	AppBase::instance()->navigationManager()->removeInitializationMode();
	return ret;
}

void Notebook::exportData(QJsonObject& _object) {
	QJsonObject dataObj;
	m_RootGroup->exportData(dataObj);
	_object["Data"] = dataObj;
}

bool Notebook::loadData(void) {
	// Clear data
	m_RootGroup->clearAllChilds();

	m_items.clear();
	m_groups.clear();
	m_groups.insert_or_assign(m_RootGroup->NavigationId(), m_RootGroup);

	// Open file
	QString filename = Settings::instance()->dataPath() + "/NotebookData.json";

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

	return importData(doc.object());
}

bool Notebook::saveData(bool _showPromtOnError) {
	// Collect data
	QJsonObject obj;
	exportData(obj);

	QJsonDocument doc(obj);

	// Delete old file
	QString filename = Settings::instance()->dataPath() + "/NotebookData.json";

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

void Notebook::addGroup(void) {
	if (m_currentGroup == nullptr) {
		aAssert(0, "No parent group selected");
		return;
	}

	NewNameDialog dia("Group Name", "Group name", "Create");
	if (dia.showDialog() == resultOk) {
		NotebookGroup * newGroup = m_currentGroup->addSubgroup(dia.inputText());
		newGroup->setParentGroup(m_currentGroup);
		newGroup->addToNavigationTree();
		m_groups.insert_or_assign(newGroup->NavigationId(), newGroup);

		slotDataChanged();
	}
}

void Notebook::addGroupToList(int _navigationId, NotebookGroup * _group) {
	m_groups.insert_or_assign(_navigationId, _group);
}

void Notebook::addTextItem(void) {
	if (m_currentGroup == nullptr) {
		aAssert(0, "No parent group selected");
		return;
	}

	NewNameDialog dia("Item Name", "Item name", "Create");
	if (dia.showDialog() == resultOk) {
		NotebookTextItem * newItem = new NotebookTextItem;
		newItem->setItemName(dia.inputText());
		m_currentGroup->addItem(newItem);
		m_items.insert_or_assign(newItem->NavigationId(), newItem);

		slotDataChanged();
	}
}

void Notebook::addListItem(void) {
	if (m_currentGroup == nullptr) {
		aAssert(0, "No parent group selected");
		return;
	}

	NewNameDialog dia("Item Name", "Item name", "Create");
	if (dia.showDialog() == resultOk) {
		NotebookChecklistItem * newItem = new NotebookChecklistItem;
		newItem->setItemName(dia.inputText());
		m_currentGroup->addItem(newItem);
		m_items.insert_or_assign(newItem->NavigationId(), newItem);

		slotDataChanged();
	}
}

void Notebook::addItemToList(int _navigationId, AbstractNotebookItem * _item) {
	m_items.insert_or_assign(_navigationId, _item);
}

void Notebook::deleteSelected(void) {
	if (m_currentItem) {
		AbstractNotebookItem * itm = m_currentItem;
		itm->removeFromNavigationTree();
		m_items.erase(itm->NavigationId());
		itm->ParentGroup()->removeItemFromList(itm);
		delete itm;
		m_currentItem = nullptr;
	}
	else if (m_currentGroup) {
		NotebookGroup * group = m_currentGroup;
		group->clearAllChilds();
		if (group == m_RootGroup) { navigationSelectionCleared(); slotDataChanged(); return; }

		group->removeFromNavigationTree();
		m_groups.erase(group->NavigationId());
		if (group->ParentGroup()) group->ParentGroup()->removeGroupFromList(group);
		delete group;
		m_currentGroup = nullptr;
	}
	navigationSelectionCleared();
	slotDataChanged();
}

void Notebook::clearNavigationTree(void) {
	m_RootGroup->clearAllChilds();
	AppBase::instance()->navigationManager()->removeItem(m_RootGroup->NavigationId());
}

void Notebook::removeObjectFromList(int _navigationId) {
	m_items.erase(_navigationId);
	m_groups.erase(_navigationId);
}

void Notebook::refreshAfterSettingsChanged(void) {
	m_ListWidget->setPointSize(Settings::instance()->notebookChecklistPointSize());
}

void Notebook::setupToolbar(ak::aTtbPage * _page) {
	m_TextWidget->placeWidgetsIntoToolbar(_page);
}

void Notebook::slotDataChanged(void) {
	if (m_currentItem) {
		switch (m_currentItem->ItemType())
		{
		case AbstractNotebookItem::Checklist:
		{
			NotebookChecklistItem * actualItem = dynamic_cast<NotebookChecklistItem *>(m_currentItem);
			if (actualItem) actualItem->refreshData(m_ListWidget->items());
			else aAssert(0, "Item cast failed");
			break;
		}
		case AbstractNotebookItem::Text:
		{
			NotebookTextItem * actualItem = dynamic_cast<NotebookTextItem *>(m_currentItem);
			if (actualItem) actualItem->setText(m_TextWidget->toPlainText());
			else aAssert(0, "Item cast failed");
			break;
		}
		default: aAssert(0, "Unknown item type"); break;
		}
	}

	m_hasUnsavedChanges = true;
	AppBase::instance()->ttb()->setNotebookSaveButtonEnabled(true);
}

void Notebook::slotListDataChanged(int _index) {
	slotDataChanged();
}

void Notebook::resetView() {
	if (m_currentItem) {
		m_centralWidget->setChild(m_placeholder);
	}
	m_currentItem = nullptr;
	m_currentGroup = nullptr;
}
