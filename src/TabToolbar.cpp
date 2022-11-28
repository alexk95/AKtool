 
// Project header
#include "TabToolbar.h"
#include "AppBase.h"
#include "Settings.h"
#include "BodyWatcher.h"
#include "BodyWatcherVisualization.h"
#include "Notebook.h"
#include "Cookbook.h"
#include "CMakeConverter.h"

// uiCore header
#include <akAPI/uiAPI.h>
#include <akWidgets/aToolButtonWidget.h>

using namespace ak;

QString TabToolbar::aciPageText(void) { return "ACI"; }
QString TabToolbar::bodyWatcherPageText(void) { return "Body Watcher"; }
QString TabToolbar::notebookPageText(void) { return "Notebook"; }
QString TabToolbar::cookbookPageText(void) { return "Cookbook"; }

TabToolbar::TabToolbar(AppBase * _app)
	: m_app(_app)
{
	// Create container
	m_file.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindowUID, "File");
	m_file.gDef = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_file.page, "");

	m_aci.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindowUID, aciPageText());
	m_aci.gDef = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_aci.page, "");

	m_cmake.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindowUID, "CMake Tools");
	m_cmake.gDef = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_cmake.page, "");

	m_bodyWatcher.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindowUID, bodyWatcherPageText());
	m_bodyWatcher.gDef = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_bodyWatcher.page, "");
	m_bodyWatcher.gData = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_bodyWatcher.page, "Data");
	m_bodyWatcher.gVis = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_bodyWatcher.page, "Visualization");

	m_noteBook.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindowUID, notebookPageText());
	m_noteBook.gDef = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_noteBook.page, "");
	m_noteBook.gEdit = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_noteBook.page, "Edit");
	m_noteBook.gData = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_noteBook.page, "Data");

	m_cookbook.page = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_app->m_mainWindowUID, cookbookPageText());
	m_cookbook.gDef = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_cookbook.page, "");
	m_cookbook.gEdit = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_cookbook.page, "Edit");
	m_cookbook.gData = uiAPI::createTabToolBarSubContainer(m_app->m_uid, m_cookbook.page, "Data");

	// Create controls
	m_file.gDef_aExit = uiAPI::createToolButton(m_app->m_uid, "Exit", "ExitAppBlue", "ToolBar");
	m_file.gDef_aDockWatcher = uiAPI::createDockWatcher(m_app->m_uid, "Docks", "ToolBar", "Docks");
	m_file.gDef_aSettings = uiAPI::createToolButton(m_app->m_uid, "Settings", "Settings", "ToolBar");

	m_aci.gDef_aInit = uiAPI::createToolButton(m_app->m_uid, "Initialize", "aci", "ToolBar");
	m_aci.gDef_aReload = uiAPI::createToolButton(m_app->m_uid, "Restart", "Refresh", "ToolBar");
	m_aci.gDef_aClose = uiAPI::createToolButton(m_app->m_uid, "Close", "Cancel", "ToolBar");

	m_cmake.gCppVsProj = uiAPI::createToolButton(m_app->m_uid, "VS C++ to CMake", "VisualStudioCXX", "ToolBar");

	m_bodyWatcher.gDef_aInit = uiAPI::createToolButton(m_app->m_uid, "Initialize", "Create", "ToolBar");
	m_bodyWatcher.gDef_aConnectToAci = uiAPI::createToolButton(m_app->m_uid, "Connect to ACI", "Connected", "ToolBar");
	m_bodyWatcher.gDef_aClose = uiAPI::createToolButton(m_app->m_uid, "Close", "Cancel", "ToolBar");

	m_bodyWatcher.gData_aAdd = uiAPI::createToolButton(m_app->m_uid, "Add entry", "Add", "ToolBar");
	m_bodyWatcher.gData_aFillEntries = uiAPI::createToolButton(m_app->m_uid, "Fill entries", "CalendarMonth", "ToolBar");
	m_bodyWatcher.gData_aSave = uiAPI::createToolButton(m_app->m_uid, "Save", "Save", "ToolBar");
	
	m_bodyWatcher.gVis_aRefresh = uiAPI::createToolButton(m_app->m_uid, "Refresh", "Refresh", "ToolBar");

	m_noteBook.gDef_aInit = uiAPI::createToolButton(m_app->m_uid, "Initialize", "Create", "ToolBar");
	m_noteBook.gDef_aClose = uiAPI::createToolButton(m_app->m_uid, "Close", "Cancel", "ToolBar");

	m_noteBook.gEdit_aAddGroup = uiAPI::createToolButton(m_app->m_uid, "Add group", "NewFolder", "ToolBar");
	m_noteBook.gEdit_aAddTextItem = uiAPI::createToolButton(m_app->m_uid, "Add text", "EditText", "ToolBar");
	m_noteBook.gEdit_aAddListItem = uiAPI::createToolButton(m_app->m_uid, "Add checklist", "Checklist", "ToolBar");
	m_noteBook.gEdit_aDelete = uiAPI::createToolButton(m_app->m_uid, "Delete", "Delete", "ToolBar");

	m_noteBook.gData_aSave = uiAPI::createToolButton(m_app->m_uid, "Save", "Save", "ToolBar");

	m_cookbook.gDef_aInit = uiAPI::createToolButton(m_app->m_uid, "Initialize", "Create", "ToolBar");
	m_cookbook.gDef_aClose = uiAPI::createToolButton(m_app->m_uid, "Close", "Cancel", "ToolBar");

	m_cookbook.gEdit_aAddGroup = uiAPI::createToolButton(m_app->m_uid, "Add group", "NewFolder", "ToolBar");
	m_cookbook.gEdit_aAddItem = uiAPI::createToolButton(m_app->m_uid, "Add item", "Cake", "ToolBar");
	m_cookbook.gEdit_aDelete = uiAPI::createToolButton(m_app->m_uid, "Delete", "Delete", "ToolBar");

	m_cookbook.gData_aSave = uiAPI::createToolButton(m_app->m_uid, "Save", "Save", "ToolBar");

	// Add controls to container
	uiAPI::container::addObject(m_file.gDef, m_file.gDef_aExit);
	uiAPI::container::addObject(m_file.gDef, m_file.gDef_aDockWatcher);
	uiAPI::container::addObject(m_file.gDef, m_file.gDef_aSettings);

	uiAPI::container::addObject(m_aci.gDef, m_aci.gDef_aInit);
	uiAPI::container::addObject(m_aci.gDef, m_aci.gDef_aReload);
	uiAPI::container::addObject(m_aci.gDef, m_aci.gDef_aClose);

	uiAPI::container::addObject(m_cmake.gDef, m_cmake.gCppVsProj);

	uiAPI::container::addObject(m_bodyWatcher.gDef, m_bodyWatcher.gDef_aInit);
	uiAPI::container::addObject(m_bodyWatcher.gDef, m_bodyWatcher.gDef_aConnectToAci);
	uiAPI::container::addObject(m_bodyWatcher.gDef, m_bodyWatcher.gDef_aClose);
	uiAPI::container::addObject(m_bodyWatcher.gData, m_bodyWatcher.gData_aAdd);
	uiAPI::container::addObject(m_bodyWatcher.gData, m_bodyWatcher.gData_aFillEntries);
	uiAPI::container::addObject(m_bodyWatcher.gData, m_bodyWatcher.gData_aSave);
	uiAPI::container::addObject(m_bodyWatcher.gVis, m_bodyWatcher.gVis_aRefresh);

	uiAPI::container::addObject(m_noteBook.gDef, m_noteBook.gDef_aInit);
	uiAPI::container::addObject(m_noteBook.gDef, m_noteBook.gDef_aClose);

	uiAPI::container::addObject(m_noteBook.gEdit, m_noteBook.gEdit_aAddGroup);
	uiAPI::container::addObject(m_noteBook.gEdit, m_noteBook.gEdit_aAddTextItem);
	uiAPI::container::addObject(m_noteBook.gEdit, m_noteBook.gEdit_aAddListItem);
	uiAPI::container::addObject(m_noteBook.gEdit, m_noteBook.gEdit_aDelete);

	uiAPI::container::addObject(m_noteBook.gData, m_noteBook.gData_aSave);

	uiAPI::container::addObject(m_cookbook.gDef, m_cookbook.gDef_aInit);
	uiAPI::container::addObject(m_cookbook.gDef, m_cookbook.gDef_aClose);

	uiAPI::container::addObject(m_cookbook.gEdit, m_cookbook.gEdit_aAddGroup);
	uiAPI::container::addObject(m_cookbook.gEdit, m_cookbook.gEdit_aAddItem);
	uiAPI::container::addObject(m_cookbook.gEdit, m_cookbook.gEdit_aDelete);

	uiAPI::container::addObject(m_cookbook.gData, m_cookbook.gData_aSave);

	// Disable controls
	uiAPI::toolButton::setEnabled(m_aci.gDef_aClose, false);
	uiAPI::toolButton::setEnabled(m_aci.gDef_aReload, false);

	uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aClose, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aAdd, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aFillEntries, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aSave, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gVis_aRefresh, false);

	uiAPI::toolButton::setEnabled(m_noteBook.gDef_aClose, false);

	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddTextItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddListItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aDelete, false);

	uiAPI::toolButton::setEnabled(m_noteBook.gData_aSave, false);

	uiAPI::toolButton::setEnabled(m_cookbook.gDef_aClose, false);

	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddItem, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aDelete, false);

	uiAPI::toolButton::setEnabled(m_cookbook.gData_aSave, false);

	// Setup tooltips
	uiAPI::object::get<aToolButtonWidget>(m_cmake.gCppVsProj)->SetToolTip("Generate CMakeList.txt file from VS2017 C++ Project file");

	uiAPI::object::get<aToolButtonWidget>(m_bodyWatcher.gData_aSave)->SetToolTip("Save (Ctrl+S)");
	
	uiAPI::object::get<aToolButtonWidget>(m_noteBook.gData_aSave)->SetToolTip("Save (Ctrl+S)");

	// Setup shortcuts
	uiAPI::object::get<aToolButtonWidget>(m_bodyWatcher.gData_aSave)->setShortcut(QKeySequence("Ctrl+S"));
	uiAPI::object::get<aToolButtonWidget>(m_noteBook.gData_aSave)->setShortcut(QKeySequence("Ctrl+S"));
	
	// Show the toolBar
	uiAPI::window::setTabToolBarVisible(m_app->m_mainWindowUID, true);

	// Register callback
	uiAPI::registerUidNotifier(m_file.gDef_aExit, this);
	uiAPI::registerUidNotifier(m_file.gDef_aSettings, this);

	uiAPI::registerUidNotifier(m_aci.gDef_aInit, this);
	uiAPI::registerUidNotifier(m_aci.gDef_aReload, this);
	uiAPI::registerUidNotifier(m_aci.gDef_aClose, this);

	uiAPI::registerUidNotifier(m_cmake.gCppVsProj, this);

	uiAPI::registerUidNotifier(m_bodyWatcher.gDef_aInit, this);
	uiAPI::registerUidNotifier(m_bodyWatcher.gDef_aConnectToAci, this);
	uiAPI::registerUidNotifier(m_bodyWatcher.gDef_aClose, this);
	uiAPI::registerUidNotifier(m_bodyWatcher.gData_aAdd, this);
	uiAPI::registerUidNotifier(m_bodyWatcher.gData_aFillEntries, this);
	uiAPI::registerUidNotifier(m_bodyWatcher.gData_aSave, this);
	uiAPI::registerUidNotifier(m_bodyWatcher.gVis_aRefresh, this);

	uiAPI::registerUidNotifier(m_noteBook.gDef_aInit, this);
	uiAPI::registerUidNotifier(m_noteBook.gDef_aClose, this);

	uiAPI::registerUidNotifier(m_noteBook.gEdit_aAddGroup, this);
	uiAPI::registerUidNotifier(m_noteBook.gEdit_aAddTextItem, this);
	uiAPI::registerUidNotifier(m_noteBook.gEdit_aAddListItem, this);
	uiAPI::registerUidNotifier(m_noteBook.gEdit_aDelete, this);

	uiAPI::registerUidNotifier(m_noteBook.gData_aSave, this);

	uiAPI::registerUidNotifier(m_cookbook.gDef_aInit, this);
	uiAPI::registerUidNotifier(m_cookbook.gDef_aClose, this);

	uiAPI::registerUidNotifier(m_cookbook.gEdit_aAddGroup, this);
	uiAPI::registerUidNotifier(m_cookbook.gEdit_aAddItem, this);
	uiAPI::registerUidNotifier(m_cookbook.gEdit_aDelete, this);

	uiAPI::registerUidNotifier(m_cookbook.gData_aSave, this);
}

TabToolbar::~TabToolbar() {

}

void TabToolbar::notify(
	ak::UID					_sender,
	ak::eventType			_eventType,
	int						_info1,
	int						_info2
) {
	try {
		if (_sender == m_file.gDef_aExit && _eventType == etClicked) uiAPI::window::close(m_app->m_mainWindowUID);
		else if (_sender == m_file.gDef_aSettings && _eventType == etClicked) Settings::instance()->showDialog();
		else if (_sender == m_aci.gDef_aInit && _eventType == etClicked) {
			if (!m_app->initializeAci()) return;
			uiAPI::toolButton::setEnabled(m_aci.gDef_aInit, false);
			uiAPI::toolButton::setEnabled(m_aci.gDef_aReload, true);
			uiAPI::toolButton::setEnabled(m_aci.gDef_aClose, true);

			if (m_app->bodyWatcher()) uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, true);
		}
		else if (_sender == m_aci.gDef_aReload && _eventType == etClicked) {
			m_app->reloadAci();
			uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, false);
			if (m_app->bodyWatcher()) {
				if (m_app->bodyWatcher()->isConnectedToAci()) uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aClose, true);
			}
		}
		else if (_sender == m_aci.gDef_aClose && _eventType == etClicked) {
			m_app->shutdownAci();
		}
		else  if (_sender == m_cmake.gCppVsProj && _eventType == etClicked) {
			CMakeConverter::runVSToCMake();
		}
		else if (_sender == m_bodyWatcher.gDef_aInit && _eventType == etClicked) {
			if (!m_app->initializeBodyWatcher()) return;
			uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aAdd, true);
			uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aFillEntries, true);
			uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aClose, true);
			uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aInit, false);
			uiAPI::toolButton::setEnabled(m_bodyWatcher.gVis_aRefresh, true);

			if (m_app->aci()) uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, true);
		}
		else if (_sender == m_bodyWatcher.gDef_aConnectToAci && _eventType == etClicked) {
			if (m_app->aci()) {
				uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, false);
				uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aClose, false);
				m_app->bodyWatcher()->connectToAci();
			}
		}
		else if (_sender == m_bodyWatcher.gDef_aClose && _eventType == etClicked) m_app->shutdownBodyWatcher();
		else if (_sender == m_bodyWatcher.gData_aAdd && _eventType == etClicked) m_app->bodyWatcher()->addEntry();
		else if (_sender == m_bodyWatcher.gData_aFillEntries && _eventType == etClicked) m_app->bodyWatcher()->fillEntries();
		else if (_sender == m_bodyWatcher.gData_aSave && _eventType == etClicked) {
			if (m_app->bodyWatcher()->saveData(true)) uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aSave, false);
		}
		else if (_sender == m_bodyWatcher.gVis_aRefresh && _eventType == etClicked) m_app->bodyWatcherVisualization()->slotRebuildGraph();
		else if (_sender == m_noteBook.gDef_aInit && _eventType == etClicked) {
			if (!m_app->initializeNotebook()) return;
			uiAPI::toolButton::setEnabled(m_noteBook.gDef_aInit, false);
			uiAPI::toolButton::setEnabled(m_noteBook.gDef_aClose, true);
		}
		else if (_sender == m_noteBook.gDef_aClose && _eventType == etClicked) {
			m_app->shutdownNotebook();
		}
		else if (_sender == m_noteBook.gEdit_aAddGroup && _eventType == etClicked) {
			m_app->m_notebook->addGroup();
		}
		else if (_sender == m_noteBook.gEdit_aAddTextItem && _eventType == etClicked) {
			m_app->m_notebook->addTextItem();
		}
		else if (_sender == m_noteBook.gEdit_aAddListItem && _eventType == etClicked) {
			m_app->m_notebook->addListItem();
		}
		else if (_sender == m_noteBook.gEdit_aDelete && _eventType == etClicked) {
			m_app->m_notebook->deleteSelected();
		}
		else if (_sender == m_noteBook.gData_aSave && _eventType == etClicked) {
			if (m_app->m_notebook->saveData(true)) uiAPI::toolButton::setEnabled(m_noteBook.gData_aSave, false);
		}
		else if (_sender == m_cookbook.gDef_aInit && _eventType == etClicked) {
			if (!m_app->initializeCookbook()) return;
			uiAPI::toolButton::setEnabled(m_cookbook.gDef_aInit, false);
			uiAPI::toolButton::setEnabled(m_cookbook.gDef_aClose, true);
		}
		else if (_sender == m_cookbook.gDef_aClose && _eventType == etClicked) {
			m_app->shutdownCookbook();
		}
		else if (_sender == m_cookbook.gEdit_aAddGroup && _eventType == etClicked) {
			m_app->m_cookbook->addGroup();
		}
		else if (_sender == m_cookbook.gEdit_aAddItem && _eventType == etClicked) {
			m_app->m_cookbook->addItem();
		}
		else if (_sender == m_cookbook.gEdit_aDelete && _eventType == etClicked) {
			m_app->m_cookbook->deleteSelected();
		}
		else if (_sender == m_cookbook.gData_aSave && _eventType == etClicked) {
			if (m_app->m_cookbook->saveData(true)) uiAPI::toolButton::setEnabled(m_cookbook.gData_aSave, false);
		}
	}
	catch (const std::exception & e) {
		assert(0);
		uiAPI::promptDialog::show(e.what(), "Error");
	}
	catch (...) {
		assert(0);
		uiAPI::promptDialog::show("Unknown error", "Error");
	}
}

void TabToolbar::addDockToWatcher(ak::UID _dockUid) {
	uiAPI::dockWatcher::addWatch(m_file.gDef_aDockWatcher, _dockUid);
}

void TabToolbar::addDockToWatcher(ak::aDockWidget * _dock) {
	uiAPI::dockWatcher::addWatch(m_file.gDef_aDockWatcher, _dock);
}

void TabToolbar::setEnabledStateAfterAciShutdown(void) {
	uiAPI::toolButton::setEnabled(m_aci.gDef_aInit, true);
	uiAPI::toolButton::setEnabled(m_aci.gDef_aReload, false);
	uiAPI::toolButton::setEnabled(m_aci.gDef_aClose, false);

	uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, false);
	if (m_app->bodyWatcher()) {
		if (m_app->bodyWatcher()->isConnectedToAci()) uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aClose, true);
	}
}

void TabToolbar::setEnabledStateAfterBodyWatcherShutdown(void) {
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aInit, true);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aConnectToAci, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gDef_aClose, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aAdd, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aFillEntries, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aSave, false);
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gVis_aRefresh, false);
}

void TabToolbar::setEnabledStateAfterNotbookShutdown(void) {
	uiAPI::toolButton::setEnabled(m_noteBook.gDef_aInit, true);
	uiAPI::toolButton::setEnabled(m_noteBook.gDef_aClose, false);
	
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddTextItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddListItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aDelete, false);

	uiAPI::toolButton::setEnabled(m_noteBook.gData_aSave, false);
}

void TabToolbar::setEnabledStateAfterCookbookShutdown(void) {
	uiAPI::toolButton::setEnabled(m_cookbook.gDef_aInit, true);
	uiAPI::toolButton::setEnabled(m_cookbook.gDef_aClose, false);

	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddItem, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aDelete, false);

	uiAPI::toolButton::setEnabled(m_cookbook.gData_aSave, false);
}

void TabToolbar::setBodyWatcherSaveButtonEnabled(bool _isEnabled) {
	uiAPI::toolButton::setEnabled(m_bodyWatcher.gData_aSave, _isEnabled);
}

void TabToolbar::resetNotebookStateAfterSelectionCleared(void) {
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddTextItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddListItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aDelete, false);
}

void TabToolbar::setNotebookStateAfterGroupSelected(void) {
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddGroup, true);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddTextItem, true);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddListItem, true);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aDelete, true);
}

void TabToolbar::setNotebookStateAfterItemSelected(void) {
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddTextItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aAddListItem, false);
	uiAPI::toolButton::setEnabled(m_noteBook.gEdit_aDelete, true);
}

void TabToolbar::setNotebookSaveButtonEnabled(bool _isEnabled) {
	uiAPI::toolButton::setEnabled(m_noteBook.gData_aSave, _isEnabled);
}

void TabToolbar::resetCookbookStateAfterSelectionCleared(void) {
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddItem, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aDelete, false);
}

void TabToolbar::setCookbookStateAfterGroupSelected(void) {
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddGroup, true);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddItem, true);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aDelete, true);
}

void TabToolbar::setCookbookStateAfterItemSelected(void) {
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddGroup, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aAddItem, false);
	uiAPI::toolButton::setEnabled(m_cookbook.gEdit_aDelete, true);
}

void TabToolbar::setCookbookSaveButtonEnabled(bool _isEnabled) {
	uiAPI::toolButton::setEnabled(m_cookbook.gData_aSave, _isEnabled);
}