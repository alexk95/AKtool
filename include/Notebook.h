#pragma once

#include "AbstractNavigationManagerCallbackObject.h"

#include <akTools/ClassHelper.h>
#include <akWidgets/aWidget.h>

#include <qobject.h>
#include <qstring.h>
#include <qjsonobject.h>

#include <map>

class NotebookGroup;
class Notebook;

class QWidget;
class QHBoxLayout;

namespace ak {
	class aLabelWidget;
	class aChecklistWidget;
	class aChecklistItem;
	class aAnimationOverlayWidget;
	class aTextEditorWidget;
	class aTtbPage;
}

class AbstractNotebookItem {
public:
	enum NotebookItemType {
		Text,
		Checklist
	};

	static QString itemTypeToString(NotebookItemType _type);
	static bool stringToItemType(const QString& _string, NotebookItemType& _type);

	AbstractNotebookItem(NotebookItemType _type);
	virtual ~AbstractNotebookItem();

	A_PROPERTY(NotebookItemType, ItemType);
	A_PROPERTY(int, NavigationId);
	A_PROPERTY_POINTER(NotebookGroup *, ParentGroup);
	A_PROPERTY_P(QString, ItemName);

	virtual bool importItemData(const QJsonObject& _object) = 0;
	virtual void exportItemData(QJsonObject& _object) = 0;
	virtual void displayData(void) = 0;

	QString navigationItemPath(void);
	void addToNavigationTree(void);
	void removeFromNavigationTree(void);
	Notebook * getOwner(void);

protected:
	bool importGeneralItemData(const QJsonObject& _object);
	void exportGeneralItemData(QJsonObject& _object);

private:
	AbstractNotebookItem(AbstractNotebookItem&) = delete;
	AbstractNotebookItem& operator = (AbstractNotebookItem&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class NotebookTextItem : public AbstractNotebookItem {
public:
	NotebookTextItem();
	NotebookTextItem(const QString& _text);
	virtual ~NotebookTextItem();

	// #####################################################################################

	// Base class functions

	virtual bool importItemData(const QJsonObject& _object) override;
	virtual void exportItemData(QJsonObject& _object) override;
	virtual void displayData(void) override;

	A_PROPERTY_CREF(QString, Text);
private:
	NotebookTextItem(NotebookTextItem&) = delete;
	NotebookTextItem& operator = (NotebookTextItem&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class NotebookChecklistEntry {
public:
	NotebookChecklistEntry();
	NotebookChecklistEntry(const NotebookChecklistEntry& _other);
	NotebookChecklistEntry(const QString& _text, bool _isDone, bool _isImportant);
	NotebookChecklistEntry& operator = (const NotebookChecklistEntry& _other);
	virtual ~NotebookChecklistEntry();

	bool importData(const QJsonObject& _object);
	void exportData(QJsonObject& _object);

	A_PROPERTY(QString, Text);
	A_PROPERTY_IS(Done);
	A_PROPERTY_IS(Important);
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class NotebookChecklistItem : public AbstractNotebookItem {
public:
	NotebookChecklistItem();
	virtual ~NotebookChecklistItem();

	// #####################################################################################

	// Base class functions

	virtual bool importItemData(const QJsonObject& _object) override;
	virtual void exportItemData(QJsonObject& _object) override;
	virtual void displayData(void) override;

	void refreshData(const std::vector<ak::aChecklistItem *>& _items);

	A_PROPERTY(std::vector<NotebookChecklistEntry *>, Entries);

private:
	NotebookChecklistItem(NotebookChecklistItem&) = delete;
	NotebookChecklistItem& operator = (NotebookChecklistItem&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class NotebookGroup {
public:
	NotebookGroup();
	NotebookGroup(const QString& _title);
	virtual ~NotebookGroup();

	bool importData(const QJsonObject& _object);
	void exportData(QJsonObject& _object);

	NotebookGroup * addSubgroup(const QString& _title);
	void addSubgroup(NotebookGroup * _group);
	void addItem(AbstractNotebookItem * _item);

	void removeItemFromList(AbstractNotebookItem * _item);
	void removeGroupFromList(NotebookGroup * _group);

	QString navigationItemPath(void);
	void addToNavigationTree(void);
	void removeFromNavigationTree(void);

	Notebook * determineOwner(void);

	void clearAllChilds(void);

	A_PROPERTY_CREF(QString, Title);
	A_PROPERTY(int, NavigationId);
	A_PROPERTY_CREF(std::vector<AbstractNotebookItem *>, Items);
	A_PROPERTY_CREF(std::vector<NotebookGroup *>, Subgroups);
	A_PROPERTY_POINTER(Notebook *, Owner);
	A_PROPERTY_POINTER(NotebookGroup *, ParentGroup);

private:
	void appendNavigationItemPath(QString& _itemPath);

	NotebookGroup(NotebookGroup&) = delete;
	NotebookGroup& operator = (NotebookGroup&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class Notebook : public QObject, public ak::aWidget, public AbstractNavigationManagerCallbackObject {
	Q_OBJECT
public:
	static QString TabText();

	Notebook();
	virtual ~Notebook();

	virtual QWidget * widget() override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;
	virtual void navigationItemSelected(int _itemID) override;
	virtual void navigationSelectionCleared(void) override;

	bool importData(const QJsonObject& _object);
	void exportData(QJsonObject& _object);

	bool loadData(void);
	bool saveData(bool _showPromtOnError);

	void addGroup(void);
	void addGroupToList(int _navigationId, NotebookGroup * _group);
	void addTextItem(void);
	void addListItem(void);
	void addItemToList(int _navigationId, AbstractNotebookItem * _item);
	void deleteSelected(void);

	void clearNavigationTree(void);

	void removeObjectFromList(int _navigationId);

	bool hasUnsavedChanges(void) const { return m_hasUnsavedChanges; }

	void refreshAfterSettingsChanged(void);

	void setupToolbar(ak::aTtbPage * _page);

	A_PROPERTY_POINTER(NotebookGroup *, RootGroup);
	A_PROPERTY_POINTER(ak::aTextEditorWidget *, TextWidget);
	A_PROPERTY_POINTER(ak::aChecklistWidget *, ListWidget);

private slots:
	void slotDataChanged(void);
	void slotListDataChanged(int _index);

private:

	void resetView();

	ak::aLabelWidget *						m_placeholder;
	ak::aAnimationOverlayWidget *			m_centralWidget;

	AbstractNotebookItem *					m_currentItem;
	NotebookGroup *							m_currentGroup;

	bool									m_hasUnsavedChanges;

	std::map<int, AbstractNotebookItem *>	m_items;
	std::map<int, NotebookGroup *>			m_groups;

};