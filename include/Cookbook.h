#pragma once

#include "AbstractNavigationManagerCallbackObject.h"

#include <akTools/ClassHelper.h>
#include <akWidgets/aWidget.h>

#include <qobject.h>
#include <qstring.h>
#include <qjsonobject.h>

#include <vector>

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QSplitter;

class CookbookGroup;
class Cookbook;

namespace ak {
	class aAnimationOverlayWidget;
	class aLabelWidget;
	class aSpinBoxWidget;
	class aTableWidget;
	class aTextEditWidget;
}

class CookbookIngredient {
public:
	enum IngredientCountType {
		GRAMS,
		MILLILITERS,
		LITERS,
		COUNT
	};
	static QString ingredientCountTypeToString(IngredientCountType _type);
	static IngredientCountType stringToIngredientCountType(const QString& _type);

	CookbookIngredient();
	virtual ~CookbookIngredient();

	bool importData(const QJsonObject& _object);
	void exportData(QJsonObject& _object);

	A_PROPERTY(QString, Name);
	A_PROPERTY(double, Count);
	A_PROPERTY(IngredientCountType, CountType);

private:

	CookbookIngredient(CookbookIngredient&) = delete;
	CookbookIngredient& operator = (CookbookIngredient&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class CookbookItem {
public:
	CookbookItem(void);
	virtual ~CookbookItem();

	A_PROPERTY(int, NavigationId);
	A_PROPERTY_POINTER(CookbookGroup *, ParentGroup);
	A_PROPERTY(QString, ItemName);
	A_PROPERTY(int, DefaultPortions);
	A_PROPERTY_CREF(QString, Description);
	A_PROPERTY_CREF(std::vector<CookbookIngredient *>, Ingredients);

	bool importData(const QJsonObject& _object);
	void exportData(QJsonObject& _object);
	void displayData(void);

	QString navigationItemPath(void);
	void addToNavigationTree(void);
	void removeFromNavigationTree(void);
	Cookbook * getOwner(void);

private:
	CookbookItem(CookbookItem&) = delete;
	CookbookItem& operator = (CookbookItem&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class CookbookGroup {
public:
	CookbookGroup();
	CookbookGroup(const QString& _title);
	virtual ~CookbookGroup();

	bool importData(const QJsonObject& _object);
	void exportData(QJsonObject& _object);

	CookbookGroup * addSubgroup(const QString& _title);
	void addSubgroup(CookbookGroup * _group);
	void addItem(CookbookItem * _item);

	void removeItemFromList(CookbookItem * _item);
	void removeGroupFromList(CookbookGroup * _group);

	QString navigationItemPath(void);
	void addToNavigationTree(void);
	void removeFromNavigationTree(void);

	Cookbook * determineOwner(void);

	void clearAllChilds(void);

	A_PROPERTY_CREF(QString, Title);
	A_PROPERTY(int, NavigationId);
	A_PROPERTY_CREF(std::vector<CookbookItem *>, Items);
	A_PROPERTY_CREF(std::vector<CookbookGroup *>, Subgroups);
	A_PROPERTY_POINTER(Cookbook *, Owner);
	A_PROPERTY_POINTER(CookbookGroup *, ParentGroup);

private:
	void appendNavigationItemPath(QString& _itemPath);

	CookbookGroup(CookbookGroup&) = delete;
	CookbookGroup& operator = (CookbookGroup&) = delete;
};

// ######################################################################################################################

// ######################################################################################################################

// ######################################################################################################################

class Cookbook : public QObject, public ak::aWidget, public AbstractNavigationManagerCallbackObject {
	Q_OBJECT
public:
	static QString TabText();

	Cookbook();
	virtual ~Cookbook();

	// ###################################################################################

	// Base class functions

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;
	virtual void navigationItemSelected(int _itemID) override;
	virtual void navigationSelectionCleared(void) override;

	// ###################################################################################

	bool hasUnsavedChanges(void) const { return m_hasUnsavedChanges; }
	bool saveData(bool _showPromtOnError);
	bool loadData(void);

	void clearNavigationTree(void);

	void addGroup(void);
	void addGroupToList(int _navigationId, CookbookGroup * _group);
	void addItem(void);
	void addItemToList(int _navigationId, CookbookItem * _item);
	void deleteSelected(void);

private slots:
	void slotDataChanged(void);

private:

	void resetView(void);

	QSplitter *						m_centralSplitter;

	CookbookItem *					m_currentItem;
	CookbookGroup *					m_currentGroup;

	bool							m_hasUnsavedChanges;
	std::map<int, CookbookItem *>	m_items;
	std::map<int, CookbookGroup *>	m_groups;
	
	CookbookGroup *					m_navigationRoot;

	QWidget *						m_leftLayoutW;
	QVBoxLayout *					m_leftLayout;

	QWidget *						m_portionsLayoutW;
	QHBoxLayout *					m_portionsLayout;

	ak::aTextEditWidget *			m_description;
	ak::aLabelWidget *				m_portionsLabel;
	ak::aSpinBoxWidget *			m_portions;

	ak::aAnimationOverlayWidget *	m_ingredientsContainer;
	ak::aTextEditWidget *			m_ingredients;
	ak::aTableWidget *				m_ingredientsTable;
};