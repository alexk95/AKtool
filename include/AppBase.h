#pragma once

// uiCore header
#include <akCore/globalDataTypes.h>		// UID type
#include <akGui/aWindowEventHandler.h>	// Window event notifications (close event)

// QT header
#include <qobject.h>

// C++ header
#include <string>

// Forward declarations
class TabToolbar;
class PropertyManager;
class NavigationManager;
class aciWidget;
class BodyWatcher;
class BodyWatcherVisualization;
class Notebook;
class Cookbook;

namespace ak { class aTextEditWidget; }
namespace ak { class aTabWidget; }
namespace ak { class aWindow; }
namespace ak { class aWindowManager; }

//! This class is managing the UI
//! We derive from aWindowEventHandler to be able to register this class as a
//! window event handler for the main window
class AppBase : public QObject, public ak::aWindowEventHandler
{
	Q_OBJECT
public:
	static AppBase * instance(void);

	static void deleteInstance(void);

	//! @brief This function will be called when the window is about to close
	//! If return false then the window will not be closed
	virtual bool closeEvent(void) override;

	// #########################################################################################

	// Output

	//! @brief Will append the provided line to the output
	void appendOutput(const QString& _text);

	void appendOutputLine(const QString& _text);

	// #########################################################################################

	// Create view

	bool initializeAci(void);
	bool initializeBodyWatcher(void);
	bool initializeNotebook(void);
	bool initializeCookbook(void);

	void reloadAci(void);

	ak::UID windowUID(void) const { return m_mainWindowUID; }
	ak::aWindow * window(void);

	TabToolbar * ttb(void) { return m_ttb; }
	PropertyManager * propertyManager(void) { return m_propertyManager; }
	NavigationManager * navigationManager(void) { return m_navigationManager; }

	aciWidget * aci(void) { return m_aci; }
	BodyWatcher * bodyWatcher(void) { return m_bodyWatcher; }
	BodyWatcherVisualization * bodyWatcherVisualization(void) { return m_bodyWatcherVisualization; }
	Notebook * notebook(void) { return m_notebook; }
	Cookbook * cookbook(void) { return m_cookbook; }

	void settingChanged(void);

public slots:
	void shutdownAci(void);
	void shutdownBodyWatcher(void);
	void shutdownNotebook(void);
	void shutdownCookbook(void);

private slots:
	void slotTabChanged(int _index);
	void slotTtbTabChanged(int _index);

private:
	friend class TabToolbar;

	ak::UID						m_uid;			//! The UID of this class, used to create controls

	TabToolbar *				m_ttb;			//! The tab toolbar

	ak::UID						m_mainWindowUID;	//! The UID of the main window
	ak::aWindowManager *		m_mainWindow;

	ak::UID						m_dockOutput;
	ak::aTextEditWidget *		m_output;

	ak::UID						m_dockProperties;
	PropertyManager *			m_propertyManager;

	ak::UID						m_dockNavigation;
	NavigationManager *			m_navigationManager;

	ak::aTabWidget *			m_tabView;

	aciWidget *					m_aci;
	BodyWatcher *				m_bodyWatcher;
	BodyWatcherVisualization *	m_bodyWatcherVisualization;
	Notebook *					m_notebook;
	Cookbook *					m_cookbook;

	AppBase();

	//! @brief Destructor
	virtual ~AppBase();

	AppBase(AppBase &) = delete;
	AppBase& operator = (AppBase&) = delete;
};