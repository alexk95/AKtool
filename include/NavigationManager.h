#pragma once

#include <akCore/akCore.h>
#include <akTools/ClassHelper.h>
#include <akWidgets/aWidget.h>

#include <qobject.h>
#include <qstring.h>

#include <map>
#include <vector>

class AbstractNavigationManagerCallbackObject;
namespace ak { class aTreeWidget; }

class NavigationEntry {
public:
	NavigationEntry();
	NavigationEntry(const NavigationEntry& _other);
	NavigationEntry& operator = (const NavigationEntry& _other);
	virtual ~NavigationEntry();

	A_PROPERTY_REF(QString, RootItemName);
	A_PROPERTY(int, RootItemId);
	A_PROPERTY_POINTER(AbstractNavigationManagerCallbackObject *, CallbackItem);
	A_PROPERTY(bool, ItemIsSelected);
};

// #####################################################################################################################################################

// #####################################################################################################################################################

// #####################################################################################################################################################

class NavigationManager : public QObject, public ak::aWidget {
	Q_OBJECT
public:
	NavigationManager();
	virtual ~NavigationManager();

	// ##################################################################################################################################

	// Base class functions

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

	// ##################################################################################################################################


	std::vector<QString> getNavigationItemPath(int _itemId) const;
	QString getNavigationItemPathString(int _itemId) const;

	int addRootItem(const QString& _text, const QString& _iconName, const QString& _iconPath, AbstractNavigationManagerCallbackObject * _callbackObject);
	int addItem(const QString& _itemPath, const QString& _iconName, const QString& _iconPath);

	void removeItem(int _navigationId);

	void setInitializationMode(void) { m_isInitializingCounter++; }
	void removeInitializationMode(void) { m_isInitializingCounter--; }

private slots:
	void slotSelectionChanged(void);

	// ##################################################################################################################################

private:
	ak::aTreeWidget *						m_navigation;
	std::map<QString, NavigationEntry *>	m_rootItemOwner;
	int										m_isInitializingCounter;
};