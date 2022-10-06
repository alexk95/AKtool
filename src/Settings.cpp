// Project header
#include "Settings.h"
#include "AppBase.h"

// aci header
#include <aci/aDir.h>
#include <aci/OS.h>

// AK header
#include <akAPI/uiAPI.h>
#include <akCore/aAssert.h>
#include <akDialogs/aOptionsDialog.h>
#include <akWidgets/aWindow.h>
#include <akWidgets/aWindowManager.h>

// Qt header
#include <qdir.h>

using namespace ak;

Settings * g_instance{ nullptr };

Settings *  Settings::instance(void) {
	if (g_instance == nullptr) { g_instance = new Settings; }
	return g_instance;
}

void Settings::clearInstance(void) {
	if (g_instance) {
		delete g_instance;
		g_instance = nullptr;
	}
}

// ######################################################################################################

// Base class functions

bool Settings::handle(const std::wstring& _command, const std::vector<std::wstring>& _params) {
	if (_params.size() == 2) {
		if (_params.back() == L"cfg") {
			cmdConfiguration();
			return true;
		}
	}
	else if (_params.size() == 3) {
		if (_params[1] == L"data") {
			QDir d(QString::fromStdWString(_params.back()));
			if (!d.exists()) {
				setColor(255, 0, 0);
				print(L"The specified directory does not exist\n");
				return false;
			}
			else {
				m_aciDataPath = _params.back();
				ak::uiAPI::settings::setString("aciDataPath", QString::fromStdWString(m_aciDataPath));
				return true;
			}
		}
	}

	setColor(255, 0, 0);
	print(L"Invalid arguments for \"" + key() + L"\"\n");
	return false;
}

// ######################################################################################################

// Command handling

void Settings::cmdConfiguration(void) {
	printDelimiterLine();
	print(L"###  ");
	print(key());
	print(L" configuration  ###\n\n");

	setColor(255, 150, 50);
	print(L"\tdata path                       ");
	setColor(255, 255, 255);
	print(m_aciDataPath + L"\n");
}

void Settings::showDialog(void) {
	// Create settings
	m_dialog = new aOptionsDialog(aOptionsDialog::optionsClose);

	aOptionsGroup * general = m_dialog->addGroup("General", "General");
	aOptionsGroup * ACI = m_dialog->addGroup("ACI", "ACI");
	aOptionsGroup * notebook = m_dialog->addGroup("Notebook", "Notebook");

	aOptionsGroup * appearance = general->addGroup("Appearance", "Appearance");
	aOptionsItemComboButton * colorStyle = new aOptionsItemComboButton("ColorStyle", "Color style", { "Bright", "Dark", "Blue" }, m_colorStyle);
	aOptionsItemCheckBox * doubleClickToolBar = new aOptionsItemCheckBox("DoubleClickTTB", "Minimize toolbar on double click", m_allowDoubleClickOnTabToolbar);
	doubleClickToolBar->setExplanation("If enabled the tab toolbar will be minimized/maximized when double clicking one of the tabs");
	aOptionsItemComboButton * dateFormatItem = new aOptionsItemComboButton("DateFormat", "Date format", { toQString(dfDDMMYYYY), toQString(dfMMDDYYYY), toQString(dfYYYYDDMM) , toQString(dfYYYYMMDD) }, toQString(m_dateFormat));
	aOptionsItemComboButton * lengthUnitItem = new aOptionsItemComboButton("LengthUnit", "Length unit", { "cm", "inch" }, m_lengthUnit);
	aOptionsItemComboButton * weightUnitItem = new aOptionsItemComboButton("WeightUnit", "Weight unit", { "kg", "lbs" }, m_weightUnit);
	appearance->addItem(colorStyle);
	appearance->addItem(doubleClickToolBar);
	appearance->addItem(dateFormatItem);
	appearance->addItem(lengthUnitItem);
	appearance->addItem(weightUnitItem);

	aOptionsGroup * localData = general->addGroup("LocalData", "Local data");
	aOptionsItemDirectorySelect * dataPathItem = new aOptionsItemDirectorySelect("DataPath", "Data path", m_dataPath);
	dataPathItem->setExplanation("The root directory where the application data should be stored at");
	aOptionsItemDirectorySelect * aciDataPathItem = new aOptionsItemDirectorySelect("ACIDataPath", "ACI data path", QString::fromStdWString(m_aciDataPath));
	aciDataPathItem->setExplanation("The root directory where the ACI data should be stored at");
	aOptionsItemDirectorySelect * aciScriptPathItem = new aOptionsItemDirectorySelect("ACIScriptPath", "ACI script path", QString::fromStdWString(m_aciScriptPath));
	aciScriptPathItem->setExplanation("The directory where the ACI scripts are located at");
	localData->addItem(dataPathItem);
	localData->addItem(aciDataPathItem);
	localData->addItem(aciScriptPathItem);

	aOptionsGroup * aciAppearance = ACI->addGroup("Appearance", "Appearance");
	aOptionsItemSpinBox * aciPointSizeItem = new aOptionsItemSpinBox("TextPointSize", "Text point size", m_aciPointSize, 6, 100);
	aciPointSizeItem->setExplanation("The default text point size for the input and output");
	aciAppearance->addItem(aciPointSizeItem);

	aOptionsGroup * notebookAppearance = notebook->addGroup("Appearance", "Appearance");
	aOptionsItemSpinBox * notebookChecklistPointSizeItem = new aOptionsItemSpinBox("ChecklistPointSize", "Checklist point size", m_notebookChecklistPointSize, 6, 100);
	notebookChecklistPointSizeItem->setExplanation("The text point size of the checklist entries");
	notebookAppearance->addItem(notebookChecklistPointSizeItem);

	// Setup window
	connect(m_dialog, &aOptionsDialog::itemChanged, this, &Settings::slotSettingsItemChanged);
	aWindowManager * windowManager = uiAPI::object::get<aWindowManager>(AppBase::instance()->windowUID());
	aWindow * window(nullptr);
	if (windowManager) window = windowManager->window();
	uiAPI::addPaintable(m_dialog);
	m_dialog->resize(800, 600);
	if (window) {
		m_dialog->setWindowIcon(window->windowIcon());
		m_dialog->setWindowTitle(window->windowTitle() + " | Settings");
	}

	m_dialog->showDialog(window);

	// Clean up
	uiAPI::removePaintable(m_dialog);
	delete m_dialog;
}

// ######################################################################################################

// Protected functions

void Settings::showCommandInfo(void) {
	setColor(255, 150, 50);
	print("\tcfg                       ");
	setColor(255, 255, 255);
	print("Will show the current settings configuration\n");

	setColor(255, 150, 50);
	print("\tdata <path>               ");
	setColor(255, 255, 255);
	print("Will set the data path to the path provided\n");
}

// ######################################################################################################

void Settings::slotSettingsItemChanged(ak::aAbstractOptionsItem * _item) {
	QString logicalName = _item->logicalName();
	if (logicalName == "General:Appearance:ColorStyle") {
		aOptionsItemComboButton * actualItem = dynamic_cast<aOptionsItemComboButton *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_colorStyle = actualItem->text();
		if (m_colorStyle == "Bright") uiAPI::setDefaultColorStyle();
		else if (m_colorStyle == "Dark") uiAPI::setDefaultDarkColorStyle();
		else if (m_colorStyle == "Blue") uiAPI::setDefaultBlueColorStyle();
		else { aAssert(0, "Unknown color style"); return; }
		uiAPI::settings::setString("ColorStyle", m_colorStyle);
	}
	else if (logicalName == "General:Appearance:DoubleClickTTB") {
		aOptionsItemCheckBox * actualItem = dynamic_cast<aOptionsItemCheckBox *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_allowDoubleClickOnTabToolbar = actualItem->isChecked();
		uiAPI::window::setTabToolbarDoubleClickEnabled(AppBase::instance()->windowUID(), m_allowDoubleClickOnTabToolbar);
		uiAPI::settings::setBool("DoubleClickOnTTB", m_allowDoubleClickOnTabToolbar);
	}
	else if (logicalName == "General:Appearance:DateFormat") {
		aOptionsItemComboButton * actualItem = dynamic_cast<aOptionsItemComboButton *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_dateFormat = toDateFormat(actualItem->text());
		uiAPI::settings::setString("DateFormat", toQString(m_dateFormat));
	}
	else if (logicalName == "General:Appearance:LengthUnit") {
		aOptionsItemComboButton * actualItem = dynamic_cast<aOptionsItemComboButton *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_lengthUnit = actualItem->text();
		uiAPI::settings::setString("LengthUnit", m_lengthUnit);
	}
	else if (logicalName == "General:Appearance:WeightUnit") {
		aOptionsItemComboButton * actualItem = dynamic_cast<aOptionsItemComboButton *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_weightUnit = actualItem->text();
		uiAPI::settings::setString("WeightUnit", m_weightUnit);
	}
	else if (logicalName == "General:LocalData:DataPath") {
		aOptionsItemDirectorySelect * actualItem = dynamic_cast<aOptionsItemDirectorySelect *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_dataPath = actualItem->selectedDirectory();
		uiAPI::settings::setString("DataPath", m_dataPath);
	}
	else if (logicalName == "General:LocalData:ACIDataPath") {
		aOptionsItemDirectorySelect * actualItem = dynamic_cast<aOptionsItemDirectorySelect *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_aciDataPath = actualItem->selectedDirectory().toStdWString();
		uiAPI::settings::setString("aciDataPath", QString::fromStdWString(m_aciDataPath));
	}
	else if (logicalName == "General:LocalData:ACIScriptPath") {
		aOptionsItemDirectorySelect * actualItem = dynamic_cast<aOptionsItemDirectorySelect *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_aciScriptPath = actualItem->selectedDirectory().toStdWString();
		uiAPI::settings::setString("aciScriptPath", QString::fromStdWString(m_aciScriptPath));
	}
	else if (logicalName == "ACI:Appearance:TextPointSize") {
		aOptionsItemSpinBox * actualItem = dynamic_cast<aOptionsItemSpinBox *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_aciPointSize = actualItem->value();
		uiAPI::settings::setInt("aciTextPointSize", m_aciPointSize);
	}
	else if (logicalName == "Notebook:Appearance:ChecklistPointSize") {
		aOptionsItemSpinBox * actualItem = dynamic_cast<aOptionsItemSpinBox *>(_item);
		if (actualItem == nullptr) { aAssert(0, "Options item cast failed"); return; }
		m_notebookChecklistPointSize = actualItem->value();
		uiAPI::settings::setInt("NotebookChecklistPointSize", m_notebookChecklistPointSize);
	}
	AppBase::instance()->settingChanged();
}

// ######################################################################################################

// Private functions

Settings::Settings() : m_dialog(nullptr) {
	m_colorStyle = ak::uiAPI::settings::getString("ColorStyle", "Bright");
	if (m_colorStyle != "Bright" && m_colorStyle != "Dark" && m_colorStyle != "Blue") m_colorStyle = "Bright";
	if (m_colorStyle == "Bright") uiAPI::setDefaultColorStyle();
	else if (m_colorStyle == "Dark") uiAPI::setDefaultDarkColorStyle();
	else if (m_colorStyle == "Blue") uiAPI::setDefaultBlueColorStyle();

	m_allowDoubleClickOnTabToolbar = uiAPI::settings::getBool("DoubleClickOnTTB", true);

	m_dataPath = uiAPI::settings::getString("DataPath", QDir::currentPath());
	m_aciDataPath = uiAPI::settings::getString("aciDataPath", QDir::currentPath()).toStdWString();
	m_aciScriptPath = uiAPI::settings::getString("aciScriptPath", QDir::currentPath()).toStdWString();

	m_dateFormat = toDateFormat(uiAPI::settings::getString("DateFormat", toQString(dfDDMMYYYY)));

	m_lengthUnit = uiAPI::settings::getString("LengthUnit", "cm");
	m_weightUnit = uiAPI::settings::getString("WeightUnit", "kg");

	m_aciPointSize = uiAPI::settings::getInt("aciTextPointSize", 8);

	m_notebookChecklistPointSize = uiAPI::settings::getInt("NotebookChecklistPointSize", 8);
}

Settings::~Settings() {}