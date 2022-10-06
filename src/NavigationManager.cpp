#include "NavigationManager.h"
#include "AbstractNavigationManagerCallbackObject.h"

#include <akAPI/uiAPI.h>
#include <akCore/aAssert.h>
#include <akWidgets/aTreeWidget.h>

using namespace ak;

NavigationEntry::NavigationEntry()
	: m_RootItemName(), m_CallbackItem(nullptr), m_ItemIsSelected(false), m_RootItemId(-1) {}

NavigationEntry::NavigationEntry(const NavigationEntry& _other) 
	: m_RootItemName(_other.m_RootItemName), m_CallbackItem(_other.m_CallbackItem),
	m_ItemIsSelected(_other.m_ItemIsSelected), m_RootItemId(_other.m_RootItemId) {}

NavigationEntry& NavigationEntry::operator = (const NavigationEntry& _other) {
	m_RootItemName = _other.m_RootItemName;
	m_CallbackItem = _other.m_CallbackItem;
	m_ItemIsSelected = _other.m_ItemIsSelected;
	m_RootItemId = _other.m_RootItemId;

	return *this;
}

NavigationEntry::~NavigationEntry() {}

// #####################################################################################################################################################

// #####################################################################################################################################################

// #####################################################################################################################################################

NavigationManager::NavigationManager() 
	: aWidget(otNone), m_isInitializingCounter(0)
{
	m_navigation = new aTreeWidget;
	m_navigation->setFilterCaseSensitive(false, false);
	m_navigation->setFilterRefreshOnChange(true);
	m_navigation->setFilterVisible(true);
	m_navigation->setMultiSelectionEnabled(false);

	connect(m_navigation, &ak::aTreeWidget::selectionChanged, this, &NavigationManager::slotSelectionChanged);
}

NavigationManager::~NavigationManager() {

}

// ##################################################################################################################################

// Base class functions

QWidget * NavigationManager::widget(void) { return m_navigation->widget(); }

void NavigationManager::setColorStyle(ak::aColorStyle * _colorStyle) {
	m_navigation->setColorStyle(_colorStyle);
}

// ##################################################################################################################################

std::vector<QString> NavigationManager::getNavigationItemPath(int _itemId) const {
	return m_navigation->getItemPath(_itemId);
}

QString NavigationManager::getNavigationItemPathString(int _itemId) const {
	return m_navigation->getItemPathString(_itemId);
}

int NavigationManager::addRootItem(const QString& _text, const QString& _iconName, const QString& _iconPath, AbstractNavigationManagerCallbackObject * _callbackObject) {
	auto itemCheck = m_rootItemOwner.find(_text);
	if (itemCheck != m_rootItemOwner.end()) { aAssert(0, "Root item with the given name already exists"); return -1; }
	int itemId = m_navigation->add(-1, _text);
	m_navigation->setItemIcon(itemId, uiAPI::getIcon(_iconName, _iconPath));
	aTreeWidgetItem * item = m_navigation->item(itemId);
	item->setSelected(false);
	item->expandAllParents(false);

	NavigationEntry * newEntry = new NavigationEntry;
	newEntry->setCallbackItem(_callbackObject);
	newEntry->setRootItemName(_text);
	newEntry->setRootItemId(itemId);
	m_rootItemOwner.insert_or_assign(_text, newEntry);

	return itemId;
}

int NavigationManager::addItem(const QString& _itemPath, const QString& _iconName, const QString& _iconPath) {
	int itemId = m_navigation->add(_itemPath, '|');
	m_navigation->setItemIcon(itemId, uiAPI::getIcon(_iconName, _iconPath));
	aTreeWidgetItem * item = m_navigation->item(itemId);
	item->setSelected(false);
	if (m_isInitializingCounter == 0) item->expandAllParents(false);
	return itemId;
}

void NavigationManager::removeItem(int _navigationId) {
	for (auto itm : m_rootItemOwner) {
		if (itm.second->RootItemId() == _navigationId) {
			delete itm.second;
			m_rootItemOwner.erase(itm.first);
			break;
		}
	}
	m_navigation->deleteItem(_navigationId);
}

// ##################################################################################################################################

// Private slots

void NavigationManager::slotSelectionChanged(void) {
		auto selection = m_navigation->selectedItems();
		if (selection.size() == 0) {
			for (auto owner : m_rootItemOwner) {
				if (owner.second->ItemIsSelected()) {
					owner.second->CallbackItem()->navigationSelectionCleared();
					owner.second->setItemIsSelected(false);
				}
			}
		}
		else if (selection.size() == 1) {
			auto itemPath = m_navigation->getItemPath(selection[0]);
			auto owner = m_rootItemOwner.find(itemPath[0]);
			if (owner == m_rootItemOwner.end()) { aAssert(0, "Root item not found in map"); return; }
			for (auto o : m_rootItemOwner) {
				if (o.second->CallbackItem() != owner->second->CallbackItem()) o.second->CallbackItem()->navigationSelectionCleared();
			}
			owner->second->setItemIsSelected(true);
			owner->second->CallbackItem()->navigationItemSelected(selection[0]);
		}
		else {
			aAssert(0, "Multi selection should be disabled");
		}
}