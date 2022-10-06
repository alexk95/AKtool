#include "BodyWatcher.h"
#include "AppBase.h"
#include "TabToolbar.h"
#include "Settings.h"
#include "aciWidget.h"

#include <aci/API.h>
#include <aci/InterpreterCore.h>

#include <akAPI/uiAPI.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aComboBoxWidget.h>
#include <akWidgets/aComboButtonWidget.h>
#include <akWidgets/aDoubleSpinBoxWidget.h>
#include <akWidgets/aDatePickWidget.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aWindow.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qevent.h>
#include <qfile.h>
#include <qjsondocument.h>
#include <qjsonarray.h>

using namespace ak;

#define BODYWATCHER_DEFAULT_PERSON_NAME "<no name>"

BodyWatcherEntry::BodyWatcherEntry()
	: m_Height(0.), m_Weight(0.), m_BizepsLeft(0.), m_BizepsRight(0.),
	m_Belly(0.), m_ThighLeft(0.), m_ThighRight(0.), m_CalfLeft(0.),
	m_CalfRight(0.), m_Date(1, 1, 2000), m_PersonName(BODYWATCHER_DEFAULT_PERSON_NAME)
{}

BodyWatcherEntry::BodyWatcherEntry(double _height, double _weight, double _bizepsLeft, double _bizepsRight,
	double _belly, double _thighLeft, double _thighRight, double _calfLeft,
	double _calfRight, const aDate& _date, const QString& _personName)
	: m_Height(_height), m_Weight(_weight), m_BizepsLeft(_bizepsLeft), m_BizepsRight(_bizepsRight),
	m_Belly(_belly), m_ThighLeft(_thighLeft), m_ThighRight(_thighRight), m_CalfLeft(_calfLeft),
	m_CalfRight(_calfRight), m_Date(_date), m_PersonName(_personName)
{}

BodyWatcherEntry::BodyWatcherEntry(const BodyWatcherEntry& _other) 
	: m_Height(_other.m_Height), m_Weight(_other.m_Weight), m_BizepsLeft(_other.m_BizepsLeft), m_BizepsRight(_other.m_BizepsRight),
	m_Belly(_other.m_Belly), m_ThighLeft(_other.m_ThighLeft), m_ThighRight(_other.m_ThighRight), m_CalfLeft(_other.m_CalfLeft),
	m_CalfRight(_other.m_CalfRight), m_Date(_other.m_Date), m_PersonName(_other.m_PersonName)
{}

BodyWatcherEntry::~BodyWatcherEntry() {}

BodyWatcherEntry& BodyWatcherEntry::operator = (const BodyWatcherEntry& _other) {
	m_Height = _other.m_Height;
	m_Weight = _other.m_Weight;
	m_BizepsLeft = _other.m_BizepsLeft;
	m_BizepsRight = _other.m_BizepsRight;
	m_Belly = _other.m_Belly;
	m_ThighLeft = _other.m_ThighLeft;
	m_ThighRight = _other.m_ThighRight;
	m_CalfLeft = _other.m_CalfLeft;
	m_CalfRight = _other.m_CalfRight;
	m_Date = _other.m_Date;
	m_PersonName = _other.m_PersonName;

	return *this;
}

bool BodyWatcherEntry::setFromJsonObject(const QJsonObject& _object) {
	// Check member
	if (!_object.contains("Height")) return false;
	if (!_object.contains("Weight")) return false;
	if (!_object.contains("BizepsL")) return false;
	if (!_object.contains("BizepsR")) return false;
	if (!_object.contains("Belly")) return false;
	if (!_object.contains("ThighL")) return false;
	if (!_object.contains("ThighR")) return false;
	if (!_object.contains("CalfL")) return false;
	if (!_object.contains("CalfR")) return false;
	if (!_object.contains("DateY")) return false;
	if (!_object.contains("DateM")) return false;
	if (!_object.contains("DateD")) return false;
	if (!_object.contains("PersonName")) return false;
	
	// Check member types
	if (!_object["Height"].isDouble()) return false;
	if (!_object["Weight"].isDouble()) return false;
	if (!_object["BizepsL"].isDouble()) return false;
	if (!_object["BizepsR"].isDouble()) return false;
	if (!_object["Belly"].isDouble()) return false;
	if (!_object["ThighL"].isDouble()) return false;
	if (!_object["ThighR"].isDouble()) return false;
	if (!_object["CalfL"].isDouble()) return false;
	if (!_object["CalfR"].isDouble()) return false;
	if (!_object["DateY"].isDouble()) return false;
	if (!_object["DateM"].isDouble()) return false;
	if (!_object["DateD"].isDouble()) return false;
	if (!_object["PersonName"].isString()) return false;

	// Read values
	m_Height = _object["Height"].toDouble();
	m_Weight = _object["Weight"].toDouble();
	m_BizepsLeft = _object["BizepsL"].toDouble();
	m_BizepsRight = _object["BizepsR"].toDouble();
	m_Belly = _object["Belly"].toDouble();
	m_ThighLeft = _object["ThighL"].toDouble();
	m_ThighRight = _object["ThighR"].toDouble();
	m_CalfLeft = _object["CalfL"].toDouble();
	m_CalfRight = _object["CalfR"].toDouble();
	m_Date.setYear(_object["DateY"].toInt());
	m_Date.setMonth(_object["DateM"].toInt());
	m_Date.setDay(_object["DateD"].toInt());
	m_PersonName = _object["PersonName"].toString();

	return true;
}

void BodyWatcherEntry::writeToJsonObject(QJsonObject& _object) const {
	_object["Height"] = m_Height;
	_object["Weight"] = m_Weight;
	_object["BizepsL"] = m_BizepsLeft;
	_object["BizepsR"] = m_BizepsRight;
	_object["Belly"] = m_Belly;
	_object["ThighL"] = m_ThighLeft;
	_object["ThighR"] = m_ThighRight;
	_object["CalfL"] = m_CalfLeft;
	_object["CalfR"] = m_CalfRight;
	_object["DateY"] = m_Date.year();
	_object["DateM"] = m_Date.month();
	_object["DateD"] = m_Date.day();
	_object["PersonName"] = m_PersonName;
}

// ####################################################################################################

// ####################################################################################################

// ####################################################################################################

BodyWatcher::BodyWatcher()
	: aWidget(otNone), m_isReading(false), m_hasUnsavedChanges(false)
{
	// Create layouts
	m_centralLayoutW = new QWidget;
	m_centralLayout = new QVBoxLayout(m_centralLayoutW);

	m_nameLayoutW = new QWidget;
	m_nameLayout = new QHBoxLayout(m_nameLayoutW);

	m_dataLayoutW = new QWidget;
	m_dataLayout = new QGridLayout(m_dataLayoutW);

	// Create controls
	m_nameLabel = new aLabelWidget("Name: ");
	m_nameInput = new aComboButtonWidget(BODYWATCHER_DEFAULT_PERSON_NAME);
	m_nameInput->addItem(BODYWATCHER_DEFAULT_PERSON_NAME);

	m_entryInitial = new BodyWatcherEntryWidget;
	m_entryInitial->setObjectName("BodyWatcherEntry_Initial");

	m_entry1 = new BodyWatcherEntryWidget;
	m_entry1->setObjectName("BodyWatcherEntry_1");

	m_entry2 = new BodyWatcherEntryWidget;
	m_entry2->setObjectName("BodyWatcherEntry_2");

	m_entry3 = new BodyWatcherEntryWidget;
	m_entry3->setObjectName("BodyWatcherEntry_3");

	m_entry4 = new BodyWatcherEntryWidget;
	m_entry4->setObjectName("BodyWatcherEntry_4");

	m_entryLatest = new BodyWatcherEntryWidget;
	m_entryLatest->setObjectName("BodyWatcherEntry_Latest");

	// Setup layout
	m_centralLayout->addWidget(m_nameLayoutW, 0);
	m_centralLayout->addWidget(m_dataLayoutW, 1);

	m_nameLayout->addWidget(m_nameLabel, 0);
	m_nameLayout->addWidget(m_nameInput, 1);

	m_dataLayout->addWidget(m_entryInitial->widget(), 0, 0);
	m_dataLayout->addWidget(m_entry1->widget(), 0, 1);
	m_dataLayout->addWidget(m_entry2->widget(), 0, 2);
	m_dataLayout->addWidget(m_entry3->widget(), 1, 0);
	m_dataLayout->addWidget(m_entry4->widget(), 1, 1);
	m_dataLayout->addWidget(m_entryLatest->widget(), 1, 2);

	m_entryLatest->setPreviousEntry(m_entry4);
	m_entry4->setPreviousEntry(m_entry3);
	m_entry3->setPreviousEntry(m_entry2);
	m_entry2->setPreviousEntry(m_entry1);
	m_entry1->setPreviousEntry(m_entryInitial);

	if (loadData()) { displayData(); }

	connect(m_nameInput, &aComboButtonWidget::changed, this, &BodyWatcher::nameChanged);
}

BodyWatcher::~BodyWatcher() {

}

// ###################################################################################

// Base class functions

QWidget * BodyWatcher::widget(void) {
	return m_centralLayoutW;
}

void BodyWatcher::setColorStyle(ak::aColorStyle * _colorStyle) {
	m_nameInput->setColorStyle(_colorStyle);
	m_nameLabel->setColorStyle(_colorStyle);

	m_entryInitial->setColorStyle(_colorStyle);
	m_entry1->setColorStyle(_colorStyle);
	m_entry2->setColorStyle(_colorStyle);
	m_entry3->setColorStyle(_colorStyle);
	m_entry4->setColorStyle(_colorStyle);
	m_entryLatest->setColorStyle(_colorStyle);
}

bool BodyWatcher::handle(const std::wstring& _command, const std::vector<std::wstring>& _params) {
	if (_params.size() < 2) return false;
	if (_params[1] == L"data") return aciShowData(_params);
	else if (_params[1] == L"modify") return aciModifyData(_params);
	else if (_params[1] == L"delete" || _params[1] == L"remove") return aciDeleteEntry(_params);
	else if (_params[1] == L"save") {
		if (_params.size() > 2) {
			printPrefix();
			setColor(aci::Color::ORANGE);
			print("Unexpected arguments provided for \"save\"\n");
		}
		else aciSaveData();
	}
	return false;
}

// ###################################################################################

// Data handling

void BodyWatcher::connectToAci(void) {
	if (AppBase::instance()->aci()) {
		aci::InterpreterCore * i = aci::API::core();
		i->printer()->print(L"Adding bodywatcher as a script\n");
		i->addScriptObject(this);
		printPrefix();
		print("Connected\n");
	}
}

void BodyWatcher::aciDisconnected(void) {
	m_isConnectedToAci = false;
}

void BodyWatcher::addEntry(void) {
	QStringList possibleNames;
	for (auto itm : m_nameInput->items()) possibleNames.push_back(itm->text());
	BodyWatcherNewEntryDialog * dia;
	if (m_personData.empty()) dia = new BodyWatcherNewEntryDialog(BodyWatcherEntry(), possibleNames, this);
	else dia = new BodyWatcherNewEntryDialog(m_personData.back(), possibleNames, this);
	uiAPI::addPaintable(dia);
	dialogResult result = dia->showDialog();
	if (result == resultOk) {
		BodyWatcherEntry newEntry(dia->createEntry());
		// Add at good index
		size_t ix(0);
		for (; ix < m_data.size(); ix++) {
			if (newEntry.Date() <= m_data[ix].Date()) break;
		}
		m_data.insert(m_data.begin() + ix, newEntry);

		// Update current name
		m_nameInput->setText(newEntry.PersonName());
		m_hasUnsavedChanges = true;
		displayData();
		emit dataChanged();
	}
	uiAPI::removePaintable(dia);
	delete dia;
}

void BodyWatcher::fillEntries(void) {
	QStringList possibleNames;
	for (auto itm : m_nameInput->items()) possibleNames.push_back(itm->text());
	BodyWatcherNewEntryDialog * dia;
	if (m_personData.empty()) dia = new BodyWatcherNewEntryDialog(BodyWatcherEntry(), possibleNames, this);
	else dia = new BodyWatcherNewEntryDialog(m_personData.back(), possibleNames, this);
	uiAPI::addPaintable(dia);
	dialogResult result = dia->showDialog();
	if (result == resultOk) {
		BodyWatcherEntry newEntry(dia->createEntry());
		
		BodyWatcherEntry latestEntry;
		for (size_t i = m_data.size() - 1; i >= 0; i--) {
			if (m_data[i].PersonName() == newEntry.PersonName()) {
				latestEntry = m_data[i];
				break;
			}
		}
		if (latestEntry.PersonName().isEmpty()) { return; }

		std::list<aDate> datesInbetween;
		aDate stepDate(latestEntry.Date().toQDate().addDays(7));
		while (stepDate < newEntry.Date()) {
			datesInbetween.push_back(stepDate);
			stepDate = stepDate.toQDate().addDays(7);
		}

		int counter = 0;
		BodyWatcherEntry diffEntry = latestEntry;
		for (auto d : datesInbetween) {
			diffEntry.setDate(d);
			diffEntry.setBelly(calculateNextStepValue(latestEntry.Belly(), diffEntry.Belly(), newEntry.Belly(), datesInbetween.size() + 1));
			diffEntry.setBizepsLeft(calculateNextStepValue(latestEntry.BizepsLeft(), diffEntry.BizepsLeft(), newEntry.BizepsLeft(), datesInbetween.size() + 1));
			diffEntry.setBizepsRight(calculateNextStepValue(latestEntry.BizepsRight(), diffEntry.BizepsRight(), newEntry.BizepsRight(), datesInbetween.size() + 1));
			diffEntry.setCalfLeft(calculateNextStepValue(latestEntry.CalfLeft(), diffEntry.CalfLeft(), newEntry.CalfLeft(), datesInbetween.size() + 1));
			diffEntry.setCalfRight(calculateNextStepValue(latestEntry.CalfRight(), diffEntry.CalfRight(), newEntry.CalfRight(), datesInbetween.size() + 1));
			diffEntry.setHeight(calculateNextStepValue(latestEntry.Height(), diffEntry.Height(), newEntry.Height(), datesInbetween.size() + 1));
			diffEntry.setThighLeft(calculateNextStepValue(latestEntry.ThighLeft(), diffEntry.ThighLeft(), newEntry.ThighLeft(), datesInbetween.size() + 1));
			diffEntry.setThighRight(calculateNextStepValue(latestEntry.ThighRight(), diffEntry.ThighRight(), newEntry.ThighRight(), datesInbetween.size() + 1));
			diffEntry.setWeight(calculateNextStepValue(latestEntry.Weight(), diffEntry.Weight(), newEntry.Weight(), datesInbetween.size() + 1));
			m_data.push_back(BodyWatcherEntry(diffEntry));
		}
		m_data.push_back(newEntry);

		// Update current name
		m_nameInput->setText(newEntry.PersonName());
		m_hasUnsavedChanges = true;
		displayData();
		emit dataChanged();
	}
	uiAPI::removePaintable(dia);
	delete dia;
}

void BodyWatcher::clearData(void) {
	m_data.clear();
}

bool BodyWatcher::loadData(void) {
	// Open file
	QString filename = Settings::instance()->dataPath() + "/BodyWatcherData.json";

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
		uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}
	QJsonObject obj = doc.object();
	if (!obj.contains("Data")) {
		uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}
	else if (!obj.contains("CurrentName")) {
		uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}
	// Check document member types
	else if (!obj["Data"].isArray()) {
		uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	} 
	else if (!obj["CurrentName"].isString()) {
		uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}

	// Clear old data
	clearData();

	// Read data
	QJsonArray dataArray = obj["Data"].toArray();
	for (int i = 0; i < dataArray.count(); i++) {
		if (!dataArray.at(i).isObject()) {
			uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
			return false;
		}
		QJsonObject entryObject = dataArray.at(i).toObject();
		BodyWatcherEntry newEntry;
		if (!newEntry.setFromJsonObject(entryObject)) {
			if (!dataArray.at(i).isObject()) {
				uiAPI::promptDialog::show("The body watcher data file is broken", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
				return false;
			}
		}
		m_data.push_back(newEntry);
	}
	
	m_isReading = true;
	// Add person names
	m_nameInput->addItem(BODYWATCHER_DEFAULT_PERSON_NAME);
	for (auto entry : m_data) {
		bool found(false);
		for (auto possible : m_nameInput->items()) {
			if (possible->text() == entry.PersonName()) {
				found = true;
				break;
			}
		}
		if (!found) {
			m_nameInput->addItem(entry.PersonName());
		}
	}

	m_nameInput->setText(obj["CurrentName"].toString());
	m_isReading = false;

	return true;
}

bool BodyWatcher::saveData(bool _showPromt) {
	// Collect data
	QJsonObject obj;
	QJsonArray dataArray;
	for (auto entry : m_data) {
		QJsonObject entryObject;
		entry.writeToJsonObject(entryObject);
		dataArray.push_back(entryObject);
	}
	obj["Data"] = dataArray;
	obj["CurrentName"] = m_nameInput->text();

	QJsonDocument doc(obj);

	// Open file
	QString filename = Settings::instance()->dataPath() + "/BodyWatcherData.json";
	
	if (QFile::exists(filename)) {
		QFile file(filename);
		if (!file.remove()) {
			if (_showPromt) uiAPI::promptDialog::show("Failed to delete old data file", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
			return false;
		}
	}

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		if (_showPromt) uiAPI::promptDialog::show("Failed to open file for writing", "Error", promptIconLeft, "DialogError", "Dialog", AppBase::instance()->window());
		return false;
	}

	file.write(doc.toJson(QJsonDocument::Indented));
	file.close();

	m_hasUnsavedChanges = false;
	return true;
}

bool BodyWatcher::hasEntry(const QString& _personName, const aDate& _date) {
	for (auto entry : m_data) {
		if (entry.PersonName() == _personName && entry.Date() == _date) return true;
	}
	return false;
}

void BodyWatcher::deleteEntry(const QString& _personName, const aDate& _date) {
	for (size_t i = 0; i < m_data.size(); i++) {
		if (m_data[i].PersonName() == _personName && m_data[i].Date() == _date) {
			m_data.erase(m_data.begin() + i);
			m_hasUnsavedChanges = true;
			displayData();
			emit dataChanged();
			break;
		}
	}
}

void BodyWatcher::replaceEntry(size_t _index, const BodyWatcherEntry& _newEntry) {
	assert(_index >= 0 && _index < m_data.size());
	m_data.erase(m_data.begin() + _index);
	m_data.insert(m_data.begin() + _index, _newEntry);
}

QString BodyWatcher::currentName(void) const {
	return m_nameInput->text();
}

// ###################################################################################

// Protected functions

void BodyWatcher::showCommandInfo(void) {
	printPrefix();
	print("data <options>                Will display the data currently loaded in the body watcher\n");
	printPrefix();
	print("            Options:\n");
	printPrefix();
	print("            -f <date>              From date (default: 01/01/0000)\n");
	printPrefix();
	print("            -t <date>              To date (default: 31/31/9999)\n");
	printPrefix();
	print("            -p <name>              The name of the person (default: <empty>)\n");
	
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("modify <index> <options>      Will modify the specified entry according to the specified options\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            Options:\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            date <value>           Set the date\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            person <value>         Set the person name\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            height <value>         Set the height\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            weight <value>         Set the weight\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            bizepsL <value>        Set the left bizeps\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            bizepsR <value>        Set the right bizeps\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            belly <value>          Set the belly\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            thighL <value>         Set the left thigh\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            thighR <value>         Set the right thigh\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            calfL <value>          Set the left calf\n");
	printPrefix();
	setColor(aci::Color::YELLOW);
	print("            calfR <value>          Set the right thigh\n");

	setColor(aci::Color::WHITE);
	printPrefix();
	print("delete <index>                Will delete the entry at the specified index\n");

	printPrefix();
	setColor(aci::Color::YELLOW);
	print("save                          Will save the current dataset\n");	
}

// ###################################################################################

// Slots

void BodyWatcher::displayData(void) {
	m_nameInput->clearItems();
	m_nameInput->addItem(BODYWATCHER_DEFAULT_PERSON_NAME);

	// Clear old entries
	m_entryInitial->setEntry(BodyWatcherEntry());
	m_entry1->setEntry(BodyWatcherEntry());
	m_entry2->setEntry(BodyWatcherEntry());
	m_entry3->setEntry(BodyWatcherEntry());
	m_entry4->setEntry(BodyWatcherEntry());
	m_entryLatest->setEntry(BodyWatcherEntry());

	m_personData.clear();
	for (auto entry : m_data) {
		if (entry.PersonName() == m_nameInput->text()) {
			m_personData.push_back(entry);
		}
		bool found(false);
		for (auto itm : m_nameInput->items()) {
			if (itm->text() == entry.PersonName()) { found = true; break; }
		}
		if (!found) m_nameInput->addItem(entry.PersonName());
	}

	// Gather new information
	if (m_personData.size() > 0) {
		int ix = m_personData.size() - 1;
		int targetIx = 4;
		std::vector<BodyWatcherEntryWidget *> targetVector({ m_entry1, m_entry2, m_entry3, m_entry4, m_entryLatest });
		for (; ix >= 0 && targetIx >= 0; ix--, targetIx--) {
			targetVector[targetIx]->setEntry(m_personData[ix]);
		}
		for (; targetIx >= 0; targetIx--) {
			targetVector[targetIx]->setEntry(m_personData[0]);
		}
		m_entryInitial->setEntry(m_personData[0]);
	}

	m_entryInitial->calculateValues();
	m_entry1->calculateValues();
	m_entry2->calculateValues();
	m_entry3->calculateValues();
	m_entry4->calculateValues();
	m_entryLatest->calculateValues();
}

void BodyWatcher::nameChanged(void) {
	displayData();
	emit dataChanged();
}

// ###################################################################################

// Private functions

void BodyWatcher::printPrefix(void) {
	setColor(aci::Color::WHITE);
	print("[");
	print(key());
	print("] ");
}

void BodyWatcher::printEntry(const BodyWatcherEntry& _entry) {
	QJsonObject obj;
	_entry.writeToJsonObject(obj);
	QJsonDocument doc(obj);
	print(doc.toJson(QJsonDocument::JsonFormat::Compact).toStdString());
}

std::string BodyWatcher::fillIndex(int _ix, int _len) {
	std::string ret = std::to_string(_ix);
	while (ret.length() < _len) {
		ret = "0" + ret;
	}
	return ret;
}

bool BodyWatcher::aciShowData(const std::vector<std::wstring>& _params) {
	aDate from(0, 0, 0);
	aDate to(31, 12, 9999);
	QString person;

	if ((_params.size() % 2) != 0) {
		printPrefix();
		setColor(aci::Color::RED);
		print("Invalid agument syntax\n");
		return false;
	}
	for (size_t i = 2; i < _params.size(); i+=2) {
		if (_params[i] == L"-f") {
			QString fromString = QString::fromStdWString(_params[i + 1]);
			fromString.replace("-", "/");
			fromString.replace(".", "/");
			from = aDate::parseString(fromString, "/", Settings::instance()->DateFormat());
		} else if (_params[i] == L"-t") {
			QString toString = QString::fromStdWString(_params[i + 1]);
			toString.replace("-", "/");
			toString.replace(".", "/");
			to = aDate::parseString(toString, "/", Settings::instance()->DateFormat());
		}
		else if (_params[i] == L"-p") {
			person = QString::fromStdWString(_params[i + 1]);
		}
	}

	if (m_data.size() == 0) {
		printPrefix();
		print("<empty>\n");
	}

	bool isYellow(false);
	int indexLength = QString::number(m_data.size() - 1).length();
	int ix(0);

	for (auto e : m_data) {
		if (e.Date() >= from && e.Date() <= to && (person == e.PersonName() || person.isEmpty())) {
			printPrefix();
			if (isYellow) setColor(aci::Color::YELLOW);
			else setColor(aci::Color::WHITE);
			isYellow = !isYellow;

			print("[");
			print(fillIndex(ix, indexLength));
			print("]: ");

			printEntry(e);
			print("\n");
		}
		ix++;
	}
	return true;
}

bool BodyWatcher::aciModifyData(const std::vector<std::wstring>& _params) {
	if (_params.size() < 5) {
		printPrefix();
		setColor(aci::Color::RED);
		print("Invalid argument syntax\n");
		return false;
	}
	else if (((_params.size() - 1) % 2) != 0) {
		printPrefix();
		setColor(aci::Color::RED);
		print("Invalid argument syntax\n");
		return false;
	}
	int index = QString::fromStdWString(_params[2]).toInt();
	if (index < 0 || index >= m_data.size()) {
		printPrefix();
		setColor(aci::Color::RED);
		print("Index out of range\n");
		return false;
	}
	
	bool dateModified(false);
	bool personModified(false);
	BodyWatcherEntry entry = m_data[index];
	BodyWatcherEntry original = entry;
	for (int i = 3; i < _params.size(); i += 2) {
		if (_params[i] == L"date") {
			QString date = QString::fromStdWString(_params[i + 1]);
			date.replace("-", "/");
			date.replace(".", "/");
			entry.setDate(aDate::parseString(date, "/", Settings::instance()->DateFormat()));
			dateModified = true;
		}
		else if (_params[i] == L"person") {
			entry.setPersonName(QString::fromStdWString(_params[i + 1]));
			personModified = true;
		}
		else if (_params[i] == L"height") entry.setHeight(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"weight") entry.setWeight(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"bizepsL") entry.setBizepsLeft(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"bizepsR") entry.setBizepsRight(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"belly") entry.setBelly(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"thighL") entry.setThighLeft(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"thighR") entry.setThighRight(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"calfL") entry.setCalfLeft(QString::fromStdWString(_params[i + 1]).toDouble());
		else if (_params[i] == L"calfR") entry.setCalfRight(QString::fromStdWString(_params[i + 1]).toDouble());
		else {
			printPrefix();
			setColor(aci::Color::RED);
			print(L"Unknown argument \"" + _params[i] + L"\"\n");
			return false;
		}
	}

	if (dateModified) {
		// Check duplicate
		for (auto e : m_data) {
			if (e.Date() == entry.Date() && e.PersonName() == entry.PersonName()) {
				printPrefix();
				setColor(aci::Color::RED);
				print(L"Can't modify entry: A entry for the same day and person already exists\n");
				return false;
			}
		}

		// Delete old entry
		m_data.erase(m_data.begin() + index);

		// Get new index
		size_t ix(0);
		for (auto d : m_data) {
			if (entry.Date() <= d.Date()) break;
			ix++;
		}
		m_data.insert(m_data.begin() + ix, entry);
	}
	else {
		if (personModified) {
			// Check duplicate
			for (auto e : m_data) {
				if (e.Date() == entry.Date() && e.PersonName() == entry.PersonName()) {
					printPrefix();
					setColor(aci::Color::RED);
					print(L"Can't modify entry: A entry for the same day and person already exists\n");
					return false;
				}
			}
		}
		replaceEntry(index, entry);
	}

	printPrefix();
	print("Entry modified:\n");
	printPrefix();
	print("\tFrom: ");
	printEntry(original);
	print("\n");
	printPrefix();
	print("\tTo:   ");
	printEntry(entry);

	m_hasUnsavedChanges = true;
	AppBase::instance()->ttb()->setBodyWatcherSaveButtonEnabled(true);
	displayData();
	emit dataChanged();

	return true;
}

bool BodyWatcher::aciDeleteEntry(const std::vector<std::wstring>& _params) {
	if (_params.size() != 3) {
		printPrefix();
		setColor(aci::Color::RED);
		print("Invalid argument syntax\n");
		return false;
	}
	int index = QString::fromStdWString(_params[2]).toInt();

	if (index < 0 || index >= m_data.size()) {
		printPrefix();
		setColor(aci::Color::RED);
		print("Index out of range\n");
		return false;
	}

	m_data.erase(m_data.begin() + index);
	printPrefix();
	print("Entry at index [" + std::to_string(index) + "] was removed successfully\n");

	m_hasUnsavedChanges = true;
	AppBase::instance()->ttb()->setBodyWatcherSaveButtonEnabled(true);
	displayData();
	emit dataChanged();
	return true;
}

bool BodyWatcher::aciSaveData(void) {
	if (saveData(false)) {
		printPrefix();
		print("Save successful\n");
		AppBase::instance()->ttb()->setBodyWatcherSaveButtonEnabled(false);
		return true;
	}
	else {
		printPrefix();
		setColor(aci::Color::ORANGE);
		print("Failed to save data\n");
		return false;
	}
}

double BodyWatcher::calculateNextStepValue(double _initialValue, double _currentStepValue, double _lastValue, size_t _steps) {
	if (_initialValue == _lastValue) return _lastValue;
	double totalDifference = _initialValue - _lastValue;
	double stepSize = ((totalDifference) / (double)_steps);
	double newStep = _currentStepValue - stepSize;
	return std::round(newStep * 100.) / 100.;
}

// ####################################################################################################

// ####################################################################################################

// ####################################################################################################

BodyWatcherEntryWidget::BodyWatcherEntryWidget()
	: aWidget(otNone), m_prevEntry(nullptr)
{
	buildWidget();
}

BodyWatcherEntryWidget::BodyWatcherEntryWidget(const BodyWatcherEntry& _entry) 
	: aWidget(otNone), m_entry(_entry), m_prevEntry(nullptr)
{
	buildWidget();
}

BodyWatcherEntryWidget::~BodyWatcherEntryWidget() {

}

// ###################################################################################

// Base class functions

QWidget * BodyWatcherEntryWidget::widget(void) { return m_layoutW; }

void BodyWatcherEntryWidget::setColorStyle(ak::aColorStyle * _colorStyle) {
	m_date->setColorStyle(_colorStyle);
	m_height->setColorStyle(_colorStyle);
	m_heightL->setColorStyle(_colorStyle);
	m_weight->setColorStyle(_colorStyle);
	m_weightL->setColorStyle(_colorStyle);
	m_bizepsLeft->setColorStyle(_colorStyle);
	m_bizepsLeftL->setColorStyle(_colorStyle);
	m_bizepsRight->setColorStyle(_colorStyle);
	m_bizepsRightL->setColorStyle(_colorStyle);
	m_belly->setColorStyle(_colorStyle);
	m_bellyL->setColorStyle(_colorStyle);	
	m_thighLeft->setColorStyle(_colorStyle);
	m_thighLeftL->setColorStyle(_colorStyle);
	m_thighRight->setColorStyle(_colorStyle);
	m_thighRightL->setColorStyle(_colorStyle);
	m_calfLeft->setColorStyle(_colorStyle);
	m_calfLeftL->setColorStyle(_colorStyle);
	m_calfRight->setColorStyle(_colorStyle);
	m_calfRightL->setColorStyle(_colorStyle);
	
	QString sheet("#");
	sheet.append(m_layoutW->objectName()).append("{ border: 1px solid #").append(_colorStyle->getControlsBorderColor().toHexString(true)).append(
		"; border-radius: 5px; ").append(_colorStyle->toStyleSheet(cafBackgroundColorControls | cafBorderColorControls)).append("}");
	
	m_layoutW->setStyleSheet(sheet);
}

// ###################################################################################

// Data handling

void BodyWatcherEntryWidget::setObjectName(const QString& _name) {
	m_layoutW->setObjectName(_name);
}

void BodyWatcherEntryWidget::setEntry(const BodyWatcherEntry& _entry) {
	m_entry = _entry;
}

void BodyWatcherEntryWidget::setPreviousEntry(BodyWatcherEntryWidget * _entry) {
	m_prevEntry = _entry;
	calculateValues();
}

// ###################################################################################

// Private functions

void BodyWatcherEntryWidget::buildWidget(void) {
	m_layoutW = new QWidget;
	m_layout = new QGridLayout(m_layoutW);

	// Create labels
	m_date = new aLabelWidget("<No data provided>");
	
	m_height = new aLabelWidget("<No data provided>");
	m_heightDiff = new aLabelWidget("<No data provided>");
	m_heightL = new aLabelWidget("Height:");

	m_weight = new aLabelWidget("<No data provided>");
	m_weightDiff = new aLabelWidget("<No data provided>");
	m_weightL = new aLabelWidget("Weight:");

	m_bizepsLeft = new aLabelWidget("<No data provided>");
	m_bizepsLeftDiff = new aLabelWidget("<No data provided>");
	m_bizepsLeftL = new aLabelWidget("Bizeps L:");

	m_bizepsRight = new aLabelWidget("<No data provided>");
	m_bizepsRightDiff = new aLabelWidget("<No data provided>");
	m_bizepsRightL = new aLabelWidget("Bizeps R:");

	m_belly = new aLabelWidget("<No data provided>");
	m_bellyDiff = new aLabelWidget("<No data provided>");
	m_bellyL = new aLabelWidget("Belly:");

	m_thighLeft = new aLabelWidget("<No data provided>");
	m_thighLeftDiff = new aLabelWidget("<No data provided>");
	m_thighLeftL = new aLabelWidget("Thigh L:");

	m_thighRight = new aLabelWidget("<No data provided>");
	m_thighRightDiff = new aLabelWidget("<No data provided>");
	m_thighRightL = new aLabelWidget("Thigh R");

	m_calfLeft = new aLabelWidget("<No data provided>");
	m_calfLeftDiff = new aLabelWidget("<No data provided>");
	m_calfLeftL = new aLabelWidget("Calf L:");

	m_calfRight = new aLabelWidget("<No data provided>");
	m_calfRightDiff = new aLabelWidget("<No data provided>");
	m_calfRightL = new aLabelWidget("Calf R:");

	// Create layout structure
	m_layout->addWidget(m_date, 0, 2);

	m_layout->addWidget(m_heightL, 1, 0);
	m_layout->addWidget(m_height, 1, 1);
	m_layout->addWidget(m_heightDiff, 1, 2);

	m_layout->addWidget(m_weightL, 2, 0);
	m_layout->addWidget(m_weight, 2, 1);
	m_layout->addWidget(m_weightDiff, 2, 2);

	m_layout->addWidget(m_bizepsLeftL, 3, 0);
	m_layout->addWidget(m_bizepsLeft, 3, 1);
	m_layout->addWidget(m_bizepsLeftDiff, 3, 2);

	m_layout->addWidget(m_bizepsRightL, 4, 0);
	m_layout->addWidget(m_bizepsRight, 4, 1);
	m_layout->addWidget(m_bizepsRightDiff, 4, 2);

	m_layout->addWidget(m_bellyL, 5, 0);
	m_layout->addWidget(m_belly, 5, 1);
	m_layout->addWidget(m_bellyDiff, 5, 2);

	m_layout->addWidget(m_thighLeftL, 6, 0);
	m_layout->addWidget(m_thighLeft, 6, 1);
	m_layout->addWidget(m_thighLeftDiff, 6, 2);

	m_layout->addWidget(m_thighRightL, 7, 0);
	m_layout->addWidget(m_thighRight, 7, 1);
	m_layout->addWidget(m_thighRightDiff, 7, 2);

	m_layout->addWidget(m_calfLeftL, 8, 0);
	m_layout->addWidget(m_calfLeft, 8, 1);
	m_layout->addWidget(m_calfLeftDiff, 8, 2);

	m_layout->addWidget(m_calfRightL, 9, 0);
	m_layout->addWidget(m_calfRight, 9, 1);
	m_layout->addWidget(m_calfRightDiff, 9, 2);

	calculateValues();
}

void BodyWatcherEntryWidget::calculateValues(void) {
	Settings * settings = Settings::instance();

	int dayOfWeek = m_entry.Date().toQDate().dayOfWeek();
	QString dateString;
	switch (dayOfWeek)
	{
	case 1: dateString = "Mon, "; break;
	case 2: dateString = "Tue, "; break;
	case 3: dateString = "Wed, "; break;
	case 4: dateString = "Thu, "; break;
	case 5: dateString = "Fri, "; break;
	case 6: dateString = "Sat, "; break;
	case 7: dateString = "Sun, "; break;
	default:
		assert(0);
	}
	dateString.append(m_entry.Date().toQString("/", settings->DateFormat()));
	m_date->setText(dateString);

	m_height->setText(QString::number(m_entry.Height()) + " " + settings->lengthUnit());
	m_weight->setText(QString::number(m_entry.Weight()) + " " + settings->weightUnit());
	m_bizepsLeft->setText(QString::number(m_entry.BizepsLeft()) + " " + settings->lengthUnit());
	m_bizepsRight->setText(QString::number(m_entry.BizepsRight()) + " " + settings->lengthUnit());
	m_belly->setText(QString::number(m_entry.Belly()) + " " + settings->lengthUnit());
	m_thighLeft->setText(QString::number(m_entry.ThighLeft()) + " " + settings->lengthUnit());
	m_thighRight->setText(QString::number(m_entry.ThighRight()) + " " + settings->lengthUnit());
	m_calfLeft->setText(QString::number(m_entry.CalfLeft()) + " " + settings->lengthUnit());
	m_calfRight->setText(QString::number(m_entry.CalfRight()) + " " + settings->lengthUnit());

	double heightDiff(0.);
	double weightDiff(0.);
	double bizepsLDiff(0.);
	double bizepsRDiff(0.);
	double bellyDiff(0.);
	double thighLDiff(0.);
	double thighRDiff(0.);
	double calfLDiff(0.);
	double calfRDiff(0.);

	if (m_prevEntry) {
		const BodyWatcherEntry& parentEntry = m_prevEntry->entry();
		heightDiff = differenceInPercent(m_entry.Height(), parentEntry.Height());
		weightDiff = differenceInPercent(m_entry.Weight(), parentEntry.Weight());
		bizepsLDiff = differenceInPercent(m_entry.BizepsLeft(), parentEntry.BizepsLeft());
		bizepsRDiff = differenceInPercent(m_entry.BizepsRight(), parentEntry.BizepsRight());
		bellyDiff = differenceInPercent(m_entry.Belly(), parentEntry.Belly());
		thighLDiff = differenceInPercent(m_entry.ThighLeft(), parentEntry.ThighLeft());
		thighRDiff = differenceInPercent(m_entry.ThighRight(), parentEntry.ThighRight());
		calfLDiff = differenceInPercent(m_entry.CalfLeft(), parentEntry.CalfLeft());
		calfRDiff = differenceInPercent(m_entry.CalfRight(), parentEntry.CalfRight());
	}

	if (heightDiff >= 0.) {
		m_heightDiff->setText("+" + nicePercent(heightDiff) + "%");
		m_heightDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_heightDiff->setText(nicePercent(heightDiff) + "%");
		m_heightDiff->setStyleSheet("color: #FFFF0000");
	}
	if (weightDiff > 0.) {
		m_weightDiff->setText("+" + nicePercent(weightDiff) + "%");
		m_weightDiff->setStyleSheet("color: #FFFF0000");
	}
	else if (weightDiff == 0.) {
		m_weightDiff->setText("+" + nicePercent(weightDiff) + "%");
		m_weightDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_weightDiff->setText(nicePercent(weightDiff) + "%");
		m_weightDiff->setStyleSheet("color: #FF00FF00");
	}
	if (bizepsLDiff >= 0.) {
		m_bizepsLeftDiff->setText("+" + nicePercent(bizepsLDiff) + "%");
		m_bizepsLeftDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_bizepsLeftDiff->setText(nicePercent(bizepsLDiff) + "%");
		m_bizepsLeftDiff->setStyleSheet("color: #FFFF0000");
	}
	if (bizepsRDiff >= 0.) {
		m_bizepsRightDiff->setText("+" + nicePercent(bizepsRDiff) + "%");
		m_bizepsRightDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_bizepsRightDiff->setText(nicePercent(bizepsRDiff) + "%");
		m_bizepsRightDiff->setStyleSheet("color: #FFFF0000");
	}
	if (bellyDiff > 0.) {
		m_bellyDiff->setText("+" + nicePercent(bellyDiff) + "%");
		m_bellyDiff->setStyleSheet("color: #FFFF0000");
	}
	else if (bellyDiff == 0.) {
		m_bellyDiff->setText("+" + nicePercent(bellyDiff) + "%");
		m_bellyDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_bellyDiff->setText(nicePercent(bellyDiff) + "%");
		m_bellyDiff->setStyleSheet("color: #FF00FF00");
	}
	if (thighLDiff >= 0.) {
		m_thighLeftDiff->setText("+" + nicePercent(thighLDiff) + "%");
		m_thighLeftDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_thighLeftDiff->setText(nicePercent(thighLDiff) + "%");
		m_thighLeftDiff->setStyleSheet("color: #FFFF0000");
	}
	if (thighRDiff >= 0.) {
		m_thighRightDiff->setText("+" + nicePercent(thighRDiff) + "%");
		m_thighRightDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_thighRightDiff->setText(nicePercent(thighRDiff) + "%");
		m_thighRightDiff->setStyleSheet("color: #FFFF0000");
	}
	if (calfLDiff >= 0.) {
		m_calfLeftDiff->setText("+" + nicePercent(calfLDiff) + "%");
		m_calfLeftDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_calfLeftDiff->setText(nicePercent(calfLDiff) + "%");
		m_calfLeftDiff->setStyleSheet("color: #FFFF0000");
	}
	if (calfRDiff >= 0.) {
		m_calfRightDiff->setText("+" + nicePercent(calfRDiff) + "%");
		m_calfRightDiff->setStyleSheet("color: #FF00FF00");
	}
	else {
		m_calfRightDiff->setText(nicePercent(calfRDiff) + "%");
		m_calfRightDiff->setStyleSheet("color: #FFFF0000");
	}
}

double BodyWatcherEntryWidget::differenceInPercent(double _current, double _previous) {
	if (_current == 0.) return 0.;
	else if (_current > _previous) return (_current / _previous) * 100. - 100.;
	else return (100. - (_current / _previous) * 100.) * (-1.);
}

QString BodyWatcherEntryWidget::nicePercent(double _perc) {
	QString str = QString::number(_perc);
	str.replace(",", ".");
	if (str.contains(".")) {
		QStringList buff = str.split(".");
		str = buff[0];
		str.append(".");
		if (buff[1].length() == 1) return str.append(buff[1]).append("0");
		else if (buff[1].length() == 2) return str.append(buff[1]);
		else if (buff[1].length() > 2) return str.append(buff[1].at(0)).append(buff[1].at(1));
	}
	return str.append(".00");
}

// ####################################################################################################

// ####################################################################################################

// ####################################################################################################


BodyWatcherNewEntryDialog::BodyWatcherNewEntryDialog(const BodyWatcherEntry& _latestEntry, const QStringList& _possibleNames, BodyWatcher * _owner)
	: m_owner(_owner)
{
	// Create layouts
	m_centralLayout = new QVBoxLayout(this);

	m_inputLayoutW = new QWidget;
	m_inputLayout = new QGridLayout(m_inputLayoutW);

	m_buttonLayoutW = new QWidget;
	m_buttonLayout = new QHBoxLayout(m_buttonLayoutW);

	// Create controls
	m_nameL = new aLabelWidget("Name:");
	m_name = new aComboBoxWidget;
	m_name->addItems(_possibleNames);
	m_inputLayout->addWidget(m_nameL, 0, 0);
	m_inputLayout->addWidget(m_name, 0, 1);
	m_name->setCurrentText(_latestEntry.PersonName());
	m_name->setEditable(true);

	m_dateL = new aLabelWidget("Date:");
	m_date = new aDatePickWidget(aDate(QDate::currentDate()), Settings::instance()->DateFormat());
	m_inputLayout->addWidget(m_dateL, 1, 0);
	m_inputLayout->addWidget(m_date, 1, 1);

	m_height = createDoublePair("Height", _latestEntry.Height(), 2);
	m_weight = createDoublePair("Weight", _latestEntry.Weight(), 3);
	m_bizepsL = createDoublePair("Bizeps left", _latestEntry.BizepsLeft(), 4);
	m_bizepsR = createDoublePair("Bizeps right", _latestEntry.BizepsRight(), 5);
	m_belly = createDoublePair("Belly", _latestEntry.Belly(), 6);
	m_thighL = createDoublePair("Thigh left", _latestEntry.ThighLeft(), 7);
	m_thighR = createDoublePair("Thigh right", _latestEntry.ThighRight(), 8);
	m_calfL = createDoublePair("Calf left", _latestEntry.CalfLeft(), 9);
	m_calfR = createDoublePair("Calf right", _latestEntry.CalfRight(), 10);

	m_inputLayout->setColumnStretch(0, 0);
	m_inputLayout->setColumnStretch(1, 1);

	m_buttonAdd = new aPushButtonWidget("Add");
	m_buttonCancel = new aPushButtonWidget("Cancel");

	// Setup layouts
	m_centralLayout->addWidget(m_inputLayoutW);
	m_centralLayout->addWidget(m_buttonLayoutW);

	m_buttonLayout->addStretch(1);
	m_buttonLayout->addWidget(m_buttonAdd);
	m_buttonLayout->addWidget(m_buttonCancel);

	hideInfoButton();
	setWindowTitle("Body watcher | Add entry");
	setWindowIcon(AppBase::instance()->window()->windowIcon());

	m_result = resultCancel;
	connect(m_buttonAdd, &aPushButtonWidget::clicked, this, &BodyWatcherNewEntryDialog::slotAdd);
	connect(m_buttonCancel, &aPushButtonWidget::clicked, this, &BodyWatcherNewEntryDialog::slotCancel);
}

BodyWatcherNewEntryDialog::~BodyWatcherNewEntryDialog() {
	delete m_nameL;
	delete m_name;
	delete m_dateL;
	delete m_date;
	delete m_height.input;
	delete m_height.label;
	delete m_weight.input;
	delete m_weight.label;
	delete m_bizepsL.input;
	delete m_bizepsL.label;
	delete m_bizepsR.input;
	delete m_bizepsR.label;
	delete m_belly.input;
	delete m_belly.label;
	delete m_thighL.input;
	delete m_thighL.label;
	delete m_thighR.input;
	delete m_thighR.label;
	delete m_calfL.input;
	delete m_calfL.label;
	delete m_calfR.input;
	delete m_calfR.label;

	delete m_buttonAdd;
	delete m_buttonCancel;

	delete m_buttonLayout;
	delete m_buttonLayoutW;
	delete m_inputLayout;
	delete m_inputLayoutW;
	
	delete m_centralLayout;
}

BodyWatcherEntry BodyWatcherNewEntryDialog::createEntry(void) {
	return BodyWatcherEntry(m_height.input->value(), m_weight.input->value(), m_bizepsL.input->value(), m_bizepsR.input->value(),
		m_belly.input->value(), m_thighL.input->value(), m_thighR.input->value(), m_calfL.input->value(), m_calfR.input->value(),
		m_date->currentDate(), m_name->currentText());
}

void BodyWatcherNewEntryDialog::setColorStyle(ak::aColorStyle * _colorStyle) {
	aDialog::setColorStyle(_colorStyle);
	m_name->setColorStyle(m_colorStyle);
	m_nameL->setColorStyle(m_colorStyle);
	m_date->setColorStyle(m_colorStyle);
	m_dateL->setColorStyle(m_colorStyle);
	m_buttonAdd->setColorStyle(m_colorStyle);
	m_buttonCancel->setColorStyle(m_colorStyle);
	paintDoublePair(m_height);
	paintDoublePair(m_weight);
	paintDoublePair(m_bizepsL);
	paintDoublePair(m_bizepsR);
	paintDoublePair(m_belly);
	paintDoublePair(m_thighL);
	paintDoublePair(m_thighR);
	paintDoublePair(m_calfL);
	paintDoublePair(m_calfR);
}

void BodyWatcherNewEntryDialog::slotAdd(void) {
	if (m_name->currentText().isEmpty()) {
		uiAPI::promptDialog::show("Please specify a name for the entry", "Missing data", promptIconLeft, "DialogInformation", "Dialog", this);
		return;
	}
	if (m_owner->hasEntry(m_name->currentText(), m_date->currentDate())) {
		dialogResult result = uiAPI::promptDialog::show("A entry for \"" + m_name->currentText() + "\" at the " + m_date->currentDate().toQString("/", Settings::instance()->DateFormat()) +
			" does already exist. Do you want to replace the existing entry?", "Confirm", promptYesNoCancel, this);

		if (result == resultNo) { Close(resultCancel); return; }
		else if (result == resultCancel) return;
		else if (result == resultYes) m_owner->deleteEntry(m_name->currentText(), m_date->currentDate());
		else return;
	}
	Close(resultOk);
}

void BodyWatcherNewEntryDialog::slotCancel(void) {
	Close(resultCancel);
}

BodyWatcherNewEntryDialog::InputPairDouble BodyWatcherNewEntryDialog::createDoublePair(const QString& _labelText, double _value, int _layoutRow) {
	InputPairDouble ret;
	ret.label = new aLabelWidget(_labelText + ":");
	ret.input = new aDoubleSpinBoxWidget;
	ret.input->setMinimum(0.);
	ret.input->setMaximum(9999.99);
	if (_value < 0.) _value = 0.;
	if (_value > 9999.99) _value = 9999.9;
	ret.input->setValue(_value);

	m_inputLayout->addWidget(ret.label, _layoutRow, 0);
	m_inputLayout->addWidget(ret.input, _layoutRow, 1);

	return ret;
}

void BodyWatcherNewEntryDialog::paintDoublePair(InputPairDouble _pair) {
	_pair.input->setColorStyle(m_colorStyle);
	_pair.label->setColorStyle(m_colorStyle);
}

