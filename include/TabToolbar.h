#pragma once

#include <akCore/globalDataTypes.h>
#include <akCore/aNotifier.h>

class AppBase;

namespace ak { class aDockWidget; }

class TabToolbar : public ak::aNotifier {
public:
	static QString aciPageText(void);
	static QString bodyWatcherPageText(void);
	static QString notebookPageText(void);
	static QString cookbookPageText(void);

	//! Constructor
	//! @param _app The Application Base that is creating this TabToolBar
	TabToolbar(AppBase * _app);

	//! Destructor
	virtual ~TabToolbar();

	//! @brief This function will be called when an event occurs
	//! The function only applies for objects that were created with the uiAPI
	//! @param _sender The UID of the sender
	//! @param _eventType The type of the event
	//! @param _info1 Additional information 1
	//! @param _info2 Additional information 2
	virtual void notify(
		ak::UID					_sender,
		ak::eventType			_eventType,
		int						_info1,
		int						_info2
	) override;

	//! @brief Will add the specified dock to the dock watcher
	//! @param _dockUid The UID of the dock
	void addDockToWatcher(ak::UID _dockUid);

	//! @brief Will add the provided dock to the dock watcher
	//! @param _dock The dock to add
	void addDockToWatcher(ak::aDockWidget * _dock);

	void setEnabledStateAfterAciShutdown(void);

	void setEnabledStateAfterBodyWatcherShutdown(void);

	void setEnabledStateAfterNotbookShutdown(void);

	void setEnabledStateAfterCookbookShutdown(void);

	void setBodyWatcherSaveButtonEnabled(bool _isEnabled);
	
	void resetNotebookStateAfterSelectionCleared(void);

	void setNotebookStateAfterGroupSelected(void);

	void setNotebookStateAfterItemSelected(void);

	void setNotebookSaveButtonEnabled(bool _isEnabled);

	void resetCookbookStateAfterSelectionCleared(void);

	void setCookbookStateAfterGroupSelected(void);

	void setCookbookStateAfterItemSelected(void);

	void setCookbookSaveButtonEnabled(bool _isEnabled);

private:
	AppBase *		m_app;			//! The Application base this toolbar belongs to

	//! This structure holds all UIDs of the Main (File) Page in the TabToolBar
	struct structFilePage {
		ak::UID		page;				//! The page UID
		ak::UID		gDef;				//! The default group UID
		ak::UID		gDef_aExit;			//! The Exit application button UID
		ak::UID		gDef_aDockWatcher;	//! The dock watcher button UID
		ak::UID		gDef_aSettings;
	};

	struct structAciPage {
		ak::UID		page;
		ak::UID		gDef;
		ak::UID		gDef_aInit;
		ak::UID		gDef_aReload;
		ak::UID		gDef_aClose;
	};

	struct structCMake {
		ak::UID		page;
		ak::UID		gDef;
		ak::UID		gCppVsProj;
	};

	struct structBodyWatcher {
		ak::UID		page;
		ak::UID		gDef;
		ak::UID		gDef_aInit;
		ak::UID		gDef_aConnectToAci;
		ak::UID		gDef_aClose;
		ak::UID		gData;
		ak::UID		gData_aAdd;
		ak::UID		gData_aFillEntries;
		ak::UID		gData_aSave;
		ak::UID		gVis;
		ak::UID		gVis_aRefresh;
	};

	struct structNotebook {
		ak::UID		page;
		ak::UID		gDef;
		ak::UID		gDef_aInit;
		ak::UID		gDef_aClose;
		ak::UID		gEdit;
		ak::UID		gEdit_aAddGroup;
		ak::UID		gEdit_aAddTextItem;
		ak::UID		gEdit_aAddListItem;
		ak::UID		gEdit_aDelete;
		ak::UID		gData;
		ak::UID		gData_aSave;
	};

	struct structCookbook {
		ak::UID		page;
		ak::UID		gDef;
		ak::UID		gDef_aInit;
		ak::UID		gDef_aClose;
		ak::UID		gEdit;
		ak::UID		gEdit_aAddGroup;
		ak::UID		gEdit_aAddItem;
		ak::UID		gEdit_aDelete;
		ak::UID		gData;
		ak::UID		gData_aSave;
	};

	structFilePage			m_file;				//! The main page
	structAciPage			m_aci;
	structCMake				m_cmake;
	structBodyWatcher		m_bodyWatcher;
	structNotebook			m_noteBook;
	structCookbook			m_cookbook;

	TabToolbar() = delete;
	TabToolbar(TabToolbar&) = delete;
	TabToolbar& operator = (TabToolbar&) = delete;
};