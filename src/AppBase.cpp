// Application header
#include "AppBase.h"			// Corresponding header
#include "TTS.h"			// Corresponding header
#include "TabToolbar.h"			// TabToolbar
#include "PropertyManager.h"
#include "NavigationManager.h"
#include "aciWidget.h"
#include "BodyWatcher.h"
#include "BodyWatcherVisualization.h"
#include "Settings.h"
#include "Notebook.h"
#include "Cookbook.h"

// AK header
#include <akAPI/uiAPI.h>		// The uiAPI
#include <akGui/aColorStyle.h>	// ColorStyle if needed for custom widgets
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aTabWidget.h>
#include <akWidgets/aDockWidget.h>
#include <akWidgets/aWindowManager.h>
#include <akWidgets/aWindow.h>
#include <akWidgets/aTtbPage.h>

#include <qcursor.h>

using namespace ak;

AppBase * g_instance{ nullptr };

// We create some constant values for the icons
const std::string c_appVersion = "1.0";

// #########################################################################################

AppBase * AppBase::instance() {
	if (g_instance == nullptr)  g_instance = new AppBase;
	return g_instance;
}

void AppBase::deleteInstance() {
	if (g_instance) delete g_instance;
	g_instance = nullptr;
}

bool AppBase::closeEvent(void) {
	// Check if all body watcher data was saved
	if (m_bodyWatcher) {
		if (m_bodyWatcher->hasUnsavedChanges()) {
			dialogResult result = uiAPI::promptDialog::show("You have unsaved body watcher changes. Do you want to save the changes now? All unsaved changes will be lost.",
				"Confirm", promptYesNoCancel, window());
			if (result == resultCancel) return false;
			else if (result == resultYes) {
				if (!m_bodyWatcher->saveData(false)) {
					if (uiAPI::promptDialog::show("Failed to save body watcher data. Do you want to continue? All unsaved changes will be lost",
						"Error", promptYesNo, "DialogError", "Dialog", window()) != resultYes) return false;
				}
			}
		}
	}

	// Check if all notebook data was saved
	if (m_notebook) {
		if (m_notebook->hasUnsavedChanges()) {
			dialogResult result = uiAPI::promptDialog::show("You have unsaved notebook changes. Do you want to save the changes now? All unsaved changes will be lost.",
				"Confirm", promptYesNoCancel, window());
			if (result == resultCancel) return false;
			else if (result == resultYes) {
				if (!m_notebook->saveData(false)) {
					if (uiAPI::promptDialog::show("Failed to save notebook data. Do you want to continue? All unsaved changes will be lost",
						"Error", promptYesNo, "DialogError", "Dialog", window()) != resultYes) return false;
				}
			}
		}
	}

	uiAPI::settings::setString("WindowState", uiAPI::window::saveState(m_mainWindowUID).c_str());
	return true;
}

// #########################################################################################

// Output

void AppBase::appendOutput(const QString& _text) {
	m_output->textCursor().insertText(_text);
}

void AppBase::appendOutputLine(const QString& _text) {
	m_output->textCursor().insertText(_text + "\n");
}

// #########################################################################################

// Create view

bool AppBase::initializeAci(void) {
	if (m_aci) { assert(0); return false; }

	m_aci = new aciWidget;
	m_tabView->addTab(m_aci->widget(), aciWidget::TabText());
	uiAPI::addPaintable(m_aci);
	return true;
}

bool AppBase::initializeBodyWatcher(void) {
	if (m_bodyWatcher) { assert(0); return false; }

	m_bodyWatcher = new BodyWatcher;
	m_tabView->addTab(m_bodyWatcher->widget(), BodyWatcher::TabText());
	uiAPI::addPaintable(m_bodyWatcher);

	m_bodyWatcherVisualization = new BodyWatcherVisualization(m_bodyWatcher);
	m_tabView->addTab(m_bodyWatcherVisualization->widget(), BodyWatcherVisualization::TabText());
	uiAPI::addPaintable(m_bodyWatcherVisualization);

	return true;
}

bool AppBase::initializeNotebook(void) {
	if (m_notebook) { assert(0); return false; }

	m_notebook = new Notebook;
	m_tabView->addTab(m_notebook->widget(), Notebook::TabText());
	uiAPI::addPaintable(m_notebook);

	if (!m_notebook->loadData()) {
		shutdownNotebook();
		return false;
	}

	aWindowManager * w = uiAPI::object::get<aWindowManager>(m_mainWindowUID);
	aTtbPage * page = dynamic_cast<aTtbPage *>(w->getTabToolBarSubContainer("Notebook"));
	m_notebook->setupToolbar(page);

	return true;
}

bool AppBase::initializeCookbook(void) {
	if (m_cookbook) { assert(0); return false; }

	m_cookbook = new Cookbook;
	m_tabView->addTab(m_cookbook->widget(), Cookbook::TabText());
	uiAPI::addPaintable(m_cookbook);

	if (!m_cookbook->loadData()) {
		shutdownCookbook();
		return false;
	}

	return true;
}

void AppBase::reloadAci(void) {
	m_aci->restart();
}

ak::aWindow * AppBase::window(void) {
	return m_mainWindow->window();
}

void AppBase::settingChanged(void) {
	if (m_aci) m_aci->refreshAfterSettingsChanged();
	if (m_notebook) m_notebook->refreshAfterSettingsChanged();
}

// #########################################################################################

// Slots

void AppBase::shutdownAci(void) {
	if (m_aci == nullptr) {
		assert(0);
		return;
	}
	for (int i = 0; i < m_tabView->count(); i++) {
		if (m_tabView->tabText(i) == aciWidget::TabText()) {
			m_tabView->removeTab(i);
			break;
		}
	}
	uiAPI::removePaintable(m_aci);
	delete m_aci;
	m_aci = nullptr;

	m_ttb->setEnabledStateAfterAciShutdown();
}

void AppBase::shutdownBodyWatcher(void) {
	if (m_bodyWatcher == nullptr) {
		assert(0);
		return;
	}

	if (m_bodyWatcher->hasUnsavedChanges()) {
		dialogResult result = uiAPI::promptDialog::show("There are unsaved changes. Do you want to save them now? All unsaved changes will be lost.",
			"Error", promptYesNoCancel, window());

		if (result == resultCancel) return;
		if (result == resultYes) {
			if (!m_bodyWatcher->saveData(false)) {
				if (uiAPI::promptDialog::show("Failed to save body watcher data. Do you want to continue? All unsaved changes will be lost",
					"Error", promptYesNo, window()) != resultYes) return;
			}
		}
	}

	for (int i = 0; i < m_tabView->count(); i++) {
		if (m_tabView->tabText(i) == BodyWatcher::TabText()) {
			m_tabView->removeTab(i);
			break;
		}
	}
	for (int i = 0; i < m_tabView->count(); i++) {
		if (m_tabView->tabText(i) == BodyWatcherVisualization::TabText()) {
			m_tabView->removeTab(i);
			break;
		}
	}

	uiAPI::removePaintable(m_bodyWatcherVisualization);
	delete m_bodyWatcherVisualization;
	m_bodyWatcherVisualization = nullptr;

	uiAPI::removePaintable(m_bodyWatcher);
	delete m_bodyWatcher;
	m_bodyWatcher = nullptr;

	m_ttb->setEnabledStateAfterBodyWatcherShutdown();
}

void AppBase::shutdownNotebook(void) {
	if (m_notebook == nullptr) {
		assert(0);
		return;
	}

	if (m_notebook->hasUnsavedChanges()) {
		dialogResult result = uiAPI::promptDialog::show("There are unsaved changes. Do you want to save them now? All unsaved changes will be lost.",
			"Error", promptYesNoCancel, window());

		if (result == resultCancel) return;
		if (result == resultYes) {
			if (!m_notebook->saveData(false)) {
				if (uiAPI::promptDialog::show("Failed to save notebook data. Do you want to continue? All unsaved changes will be lost",
					"Error", promptYesNo, window()) != resultYes) return;
			}
		}
	}

	m_notebook->clearNavigationTree();

	for (int i = 0; i < m_tabView->count(); i++) {
		if (m_tabView->tabText(i) == Notebook::TabText()) {
			m_tabView->removeTab(i);
			break;
		}
	}

	uiAPI::removePaintable(m_notebook);
	delete m_notebook;
	m_notebook = nullptr;

	m_ttb->setEnabledStateAfterNotbookShutdown();
}

void AppBase::shutdownCookbook(void) {
	if (m_cookbook == nullptr) {
		assert(0);
		return;
	}

	if (m_cookbook->hasUnsavedChanges()) {
		dialogResult result = uiAPI::promptDialog::show("There are unsaved changes. Do you want to save them now? All unsaved changes will be lost.",
			"Error", promptYesNoCancel, window());

		if (result == resultCancel) return;
		if (result == resultYes) {
			if (!m_cookbook->saveData(false)) {
				if (uiAPI::promptDialog::show("Failed to save cookbook data. Do you want to continue? All unsaved changes will be lost",
					"Error", promptYesNo, window()) != resultYes) return;
			}
		}
	}

	m_cookbook->clearNavigationTree();

	for (int i = 0; i < m_tabView->count(); i++) {
		if (m_tabView->tabText(i) == Cookbook::TabText()) {
			m_tabView->removeTab(i);
			break;
		}
	}

	uiAPI::removePaintable(m_cookbook);
	delete m_cookbook;
	m_cookbook = nullptr;

	m_ttb->setEnabledStateAfterCookbookShutdown();
}

void AppBase::slotInitializeTTS(void) {
	AK_TTS;
}

void AppBase::slotTabChanged(int _index) {
	if (_index == -1) m_propertyManager->setCurrentView(PropertyManager::NO_VIEW);
	else {
		QString text = m_tabView->tabText(_index);
		QString ttbText = m_mainWindow->getTabToolBarTabText(m_mainWindow->currentTabToolbarTab());
		if (text == aciWidget::TabText()) {
			m_propertyManager->setCurrentView(PropertyManager::ACI);
			if (ttbText != TabToolbar::aciPageText()) m_mainWindow->setCurrentTabToolBarTab(TabToolbar::aciPageText());
		}
		else if (text == BodyWatcher::TabText()) {
			m_propertyManager->setCurrentView(PropertyManager::BODY_WATCH);
			if (ttbText != TabToolbar::bodyWatcherPageText()) m_mainWindow->setCurrentTabToolBarTab(TabToolbar::bodyWatcherPageText());
		}
		else if (text == BodyWatcherVisualization::TabText()) {
			m_propertyManager->setCurrentView(PropertyManager::BODY_WATCH_VIS);
			if (ttbText != TabToolbar::bodyWatcherPageText()) m_mainWindow->setCurrentTabToolBarTab(TabToolbar::bodyWatcherPageText());
		}
		else if (text == Notebook::TabText()) {
			m_propertyManager->setCurrentView(PropertyManager::NOTEBOOK);
			if (ttbText != TabToolbar::notebookPageText()) m_mainWindow->setCurrentTabToolBarTab(TabToolbar::notebookPageText());
		}
		else if (text == Cookbook::TabText()) {
			m_propertyManager->setCurrentView(PropertyManager::COOKBOOK);
			if (ttbText != TabToolbar::cookbookPageText()) m_mainWindow->setCurrentTabToolBarTab(TabToolbar::cookbookPageText());
		}
		else assert(0);
	}
}

void AppBase::slotTtbTabChanged(int _index) {
	if (m_tabView->currentIndex() < 0) return;
	QString text = m_tabView->tabText(m_tabView->currentIndex());
	QString ttbText = m_mainWindow->getTabToolBarTabText(m_mainWindow->currentTabToolbarTab());

	if (ttbText == TabToolbar::aciPageText()) {
		if (text != aciWidget::TabText()) {
			for (int i = 0; i < m_tabView->count(); i++) {
				if (m_tabView->tabText(i) == aciWidget::TabText()) {
					m_tabView->setCurrentIndex(i);
					break;
				}
			}
		}
	}
	else if (ttbText == TabToolbar::bodyWatcherPageText()) {
		if (text != BodyWatcher::TabText() && text != BodyWatcherVisualization::TabText()) {
			for (int i = 0; i < m_tabView->count(); i++) {
				if (m_tabView->tabText(i) == BodyWatcher::TabText()) {
					m_tabView->setCurrentIndex(i);
					break;
				}
			}
		}
	}
	else if (ttbText == TabToolbar::notebookPageText()) {
		if (text != Notebook::TabText()) {
			for (int i = 0; i < m_tabView->count(); i++) {
				if (m_tabView->tabText(i) == Notebook::TabText()) {
					m_tabView->setCurrentIndex(i);
					break;
				}
			}
		}

	}
	else if (ttbText == TabToolbar::cookbookPageText()) {
		if (text != Cookbook::TabText()) {
			for (int i = 0; i < m_tabView->count(); i++) {
				if (m_tabView->tabText(i) == Cookbook::TabText()) {
					m_tabView->setCurrentIndex(i);
					break;
				}
			}
		}

	}
}

// #########################################################################################

// Private functions

AppBase::AppBase()
	: m_ttb(nullptr), m_aci(nullptr), m_bodyWatcher(nullptr), m_bodyWatcherVisualization(nullptr),
	m_notebook(nullptr), m_cookbook(nullptr)
{
	// Create own UID
	m_uid = ak::uiAPI::createUid();
	
	// Create the main window
	m_mainWindowUID = uiAPI::createWindow(m_uid);
	m_mainWindow = uiAPI::object::get<aWindowManager>(m_mainWindowUID);

	// Create the tabToolbar
	m_ttb = new TabToolbar(this);
	
	// Create output dock
	m_dockOutput = uiAPI::createDock(m_uid, "Output");
	aDockWidget * dockOutput = uiAPI::object::get<aDockWidget>(m_dockOutput);
	dockOutput->setObjectName("AKTOOL_DOCK_OUTPUT");

	ak::UID outputUID = uiAPI::createTextEdit(m_uid);
	uiAPI::dock::setCentralWidget(m_dockOutput, outputUID);

	uiAPI::window::addDock(m_mainWindowUID, m_dockOutput, ak::dockBottom);
	
	// Create property manager
	m_dockProperties = uiAPI::createDock(m_uid, "Properties");
	aDockWidget * dockProperties = uiAPI::object::get<aDockWidget>(m_dockProperties);
	dockProperties->setObjectName("AKTOOL_DOCK_PROPERTIES");

	m_propertyManager = new PropertyManager;
	dockProperties->setWidget(m_propertyManager->widget());

	uiAPI::window::addDock(m_mainWindowUID, m_dockProperties, dockRight);

	uiAPI::addPaintable(m_propertyManager);

	// Create navigation manager
	m_dockNavigation = uiAPI::createDock(m_uid, "Navigation");
	aDockWidget * dockNavigation = uiAPI::object::get<aDockWidget>(m_dockNavigation);
	dockNavigation->setObjectName("AKTOOL_DOCK_NAVIGATION");

	m_navigationManager = new NavigationManager;
	dockNavigation->setWidget(m_navigationManager->widget());

	uiAPI::window::addDock(m_mainWindowUID, m_dockNavigation, dockLeft);

	uiAPI::addPaintable(m_navigationManager);

	// Get the output widget to have direct access to its functions
	m_output = uiAPI::object::get<aTextEditWidget>(outputUID);
	m_output->setReadOnly(true);
	{
		QFont font = m_output->font();
		font.setFamily("Courier");
		font.setFixedPitch(true);
		m_output->setFont(font);
	}

	appendOutputLine("Initialize application...");
	
	// Add docks to dock watcher
	m_ttb->addDockToWatcher(m_dockOutput);
	m_ttb->addDockToWatcher(m_dockProperties);

	// Create tab view
	m_tabView = new ak::aTabWidget;
	uiAPI::addPaintable(m_tabView);
	uiAPI::window::setCentralWidget(m_mainWindowUID, m_tabView);

	// Set window icon
	uiAPI::window::setWindowIcon(m_mainWindowUID, uiAPI::getIcon("AKtool", "Window"));

	// Add this object as an window event listener to the main window
	uiAPI::window::addEventHandler(m_mainWindowUID, this);

	// Set tab toolbar double click
	uiAPI::window::setTabToolbarDoubleClickEnabled(m_mainWindowUID, Settings::instance()->allowDoubleClickOnTabToolbar());

	// Show window
	uiAPI::window::setTitle(m_mainWindowUID, "AKtool");
	uiAPI::window::showMaximized(m_mainWindowUID);

	// Restore the window settings
	uiAPI::window::restoreState(m_mainWindowUID, uiAPI::settings::getString("WindowState", "").toStdString());

	connect(m_tabView, &aTabWidget::currentChanged, this, &AppBase::slotTabChanged);
	connect(m_mainWindow, &aWindowManager::tabToolBarTabChanged, this, &AppBase::slotTtbTabChanged);

	appendOutputLine("Ready");

	QMetaObject::invokeMethod(this, &AppBase::slotInitializeTTS, Qt::QueuedConnection);
}

AppBase::~AppBase() {
	if (m_ttb != nullptr) { delete m_ttb; }
}
