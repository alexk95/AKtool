#include "aciWidget.h"
#include "Settings.h"
#include "AppBase.h"
#include "BodyWatcher.h"

#include <aci/API.h>
#include <aci/InterpreterCore.h>
#include <aci/aDir.h>
#include <aci/aFile.h>
#include <aci/ScriptLoader.h>

#include <akAPI/uiAPI.h>		// The uiAPI
#include <akGui/aColorStyle.h>	// ColorStyle if needed for custom widgets
#include <akGui/aColorStyleDefaultDark.h>
#include <akWidgets/aWindow.h>
#include <akWidgets/aWindowManager.h>
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aTabWidget.h>
#include <akWidgets/aLabelWidget.h>

#include <qwidget.h>
#include <qlayout.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qfile.h>
#include <qevent.h>
#include <qscrollbar.h>

using namespace ak;

aciWidget::aciWidget() : aWidget(otNone) {
	// Create layouts
	m_centralLayoutW = new QWidget;
	m_centralLayout = new QVBoxLayout(m_centralLayoutW);
	m_centralLayoutW->setObjectName("a_aci_centralLayoutW");

	m_inLayoutW = new QWidget;
	m_inLayout = new QHBoxLayout(m_inLayoutW);
	m_inLayout->setContentsMargins(0, 0, 0, 0);

	// Create controls
	m_out = new aTextEditWidget;
	m_out->setReadOnly(true);
	m_out->setAutoScrollToBottom(true);
	m_out->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	{
		QFont f = m_out->font();
		f.setFamily("Courier");
		f.setFixedPitch(true);
		f.setPointSize(Settings::instance()->aciPointSize());
		m_out->setFont(f);
	}
	
	m_in = new aLineEditWidget;
	m_in->setIgnoreTabulator(true);
	m_in->setObjectName("a_input");
	{
		QFont f = m_in->font();
		f.setPointSize(Settings::instance()->aciPointSize());
		m_in->setFont(f);
	}

	m_inLabel = new aLabelWidget("<:");
	m_inLabel->setObjectName("a_aci_inputLabel");
	{
		QFont f = m_inLabel->font();
		f.setPointSize(Settings::instance()->aciPointSize());
		m_inLabel->setFont(f);
	}

	// Setup layouts
	m_inLayout->addWidget(m_inLabel, 0);
	m_inLayout->addWidget(m_in, 1);

	m_centralLayout->addWidget(m_out);
	m_centralLayout->addWidget(m_inLayoutW);

	m_colorStyle = new aColorStyleDefaultDark;
	m_out->setColorStyle(m_colorStyle);
	m_in->setColorStyle(m_colorStyle);
	m_inLabel->setStyleSheet("#a_aci_inputLabel{color: #40ff40;}");

	initialize();

	QMetaObject::invokeMethod(this, &aciWidget::slotRequestInputFocus, Qt::QueuedConnection);
}

aciWidget::~aciWidget() {
	// Unload ACI
	aci::API::core()->removeScriptObject(Settings::instance()->key(), false);
	if (AppBase::instance()->bodyWatcher()) aci::API::core()->removeScriptObject(AppBase::instance()->bodyWatcher()->key(), false);
	aci::API::cleanUp();

	// Delete controls
	delete m_in;
	delete m_inLabel;
	delete m_out;
	
	// Delete layouts
	delete m_inLayout;
	delete m_inLayoutW;
	delete m_centralLayout;
	delete m_centralLayoutW;
}

void aciWidget::disableInput(void) {
	setInputEnabled(false);
}
void aciWidget::disableInputAsync(void) {
	QMetaObject::invokeMethod(this, "slotSetInputEnabled", Qt::QueuedConnection, Q_ARG(bool, false));
}
void aciWidget::enableInput(void) {
	setInputEnabled(true);
}
void aciWidget::enableInputAsync(void) {
	QMetaObject::invokeMethod(this, "slotSetInputEnabled", Qt::QueuedConnection, Q_ARG(bool, true));
}

void aciWidget::shutdown(void) {
	QMetaObject::invokeMethod(AppBase::instance(), &AppBase::shutdownAci, Qt::QueuedConnection);
}

void aciWidget::print(const std::string& _str) { slotPrintMessage(QString(_str.c_str())); }
void aciWidget::print(const std::wstring& _str) { slotPrintMessage(QString::fromWCharArray(_str.c_str())); }
void aciWidget::print(const QString& _str) { slotPrintMessage(_str); }
void aciWidget::printAsync(const std::string& _str) { QMetaObject::invokeMethod(this, "slotPrintMessage", Qt::QueuedConnection, Q_ARG(const QString&, QString(_str.c_str()))); }
void aciWidget::printAsync(const std::wstring& _str) { QMetaObject::invokeMethod(this, "slotPrintMessage", Qt::QueuedConnection, Q_ARG(const QString&, QString::fromWCharArray(_str.c_str()))); }
void aciWidget::printAsync(const QString& _str) { QMetaObject::invokeMethod(this, "slotPrintMessage", Qt::QueuedConnection, Q_ARG(const QString&, _str)); }
void aciWidget::setColor(const aci::Color& _color) { slotSetColor(QColor(_color.r(), _color.g(), _color.b(), _color.a())); }
void aciWidget::setColorAsync(const aci::Color& _color) { QMetaObject::invokeMethod(this, "slotSetColor", Qt::QueuedConnection, Q_ARG(const QColor&, QColor(_color.r(), _color.g(), _color.b(), _color.a()))); }
void aciWidget::setColor(const QColor& _color) { slotSetColor(_color); }
void aciWidget::setColorAsync(const QColor& _color) { QMetaObject::invokeMethod(this, "slotSetColor", Qt::QueuedConnection, Q_ARG(const QColor&, _color)); }

bool aciWidget::fileExists(const std::wstring& _path) {
	return QFile(QString::fromStdWString(_path)).exists();
}

bool aciWidget::deleteFile(const std::wstring& _path) {
	QFile file(QString::fromStdWString(_path));
	if (file.exists()) {
		return file.remove();
	}
	return true;
}

bool aciWidget::readFile(std::wstring& _data, const std::wstring& _path) {
	QFile file(QString::fromStdWString(_path));
	if (!file.exists()) {
		assert(0);
		return false;
	}
	if (!file.open(QIODevice::ReadOnly)) { assert(0); return false; }
	_data = QString(file.readAll()).toStdWString();
	file.close();
	return true;
}

bool aciWidget::editFile(std::list<std::wstring>& _data, std::wstring& _path) {
	return false;
}

bool aciWidget::readLinesFromFile(std::list<std::wstring>& _data, const std::wstring& _path) {
	QFile file(QString::fromStdWString(_path));
	if (!file.exists()) { assert(0); return false; }
	if (!file.open(QIODevice::ReadOnly)) { assert(0); return false; }
	while (!file.atEnd()) {
		_data.push_back(QString(file.readLine()).toStdWString());
	}

	file.close();
	return true;
}

bool aciWidget::writeFile(const std::wstring& _data, const std::wstring& _path) {
	QStringList lst = QString::fromStdWString(_path).replace("\\", "/").split("/");
	QString dir;
	for (int p{ 0 }; p < lst.count() - 1; p++) {
		if (!dir.isEmpty()) { dir.append("/"); }
		dir.append(lst[p]);
		if (!QDir(dir).exists()) {
			if (!QDir().mkdir(dir)) { assert(0); return false; }
		}
	}

	QFile file(QString::fromStdWString(_path));
	if (!file.open(QIODevice::WriteOnly)) { assert(0); return false; }
	file.write(QByteArray().append(QString::fromStdWString(_data)));
	file.close();
	return true;
}

bool aciWidget::writeLinesToFile(const std::list<std::wstring>& _data, const std::wstring& _path) {
	QStringList lst = QString::fromStdWString(_path).replace("\\", "/").split("/");
	QString dir;
	for (int p{ 0 }; p < lst.count() - 1; p++) {
		if (!dir.isEmpty()) { dir.append("/"); }
		dir.append(lst[p]);
		if (!QDir(dir).exists()) {
			if (!QDir().mkdir(dir)) { assert(0); return false; }
		}
	}

	QFile file(QString::fromStdWString(_path));
	if (!file.open(QIODevice::WriteOnly)) { assert(0); return false; }
	QTextStream stream(&file);
	for (auto l : _data) {
		stream << QString::fromStdWString(l) << "\n";
	}
	file.close();
	return true;
}

bool aciWidget::directoryExists(const std::wstring& _path) {
	return QDir(QString::fromStdWString(_path)).exists();
}

std::list<std::wstring> aciWidget::filesInDirectory(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) {
	QDir dir(QString::fromStdWString(_path));
	std::list<std::wstring> ret;
	if (dir.exists()) {
		for (auto entry : dir.entryList(QDir::Filter::Files)) {
			if (entry != "." && entry != "..") {
				ret.push_back(entry.toStdWString());
			}
		}
	}
	return ret;
}

std::list<std::wstring> aciWidget::subdirectories(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) {
	QDir dir(QString::fromStdWString(_path));
	std::list<std::wstring> ret;
	if (dir.exists()) {
		for (auto entry : dir.entryList(QDir::Filter::Dirs)) {
			if (entry != "." && entry != "..") {
				ret.push_back(entry.toStdWString());
			}
		}
	}
	return ret;
}

std::wstring aciWidget::currentDirectory(void) {
	return QDir::currentPath().toStdWString();
}

std::wstring aciWidget::scriptDataDirectory(void) {
	return Settings::instance()->aciDataPath() + L"/ScriptData";
}

std::wstring aciWidget::getSettingsValue(const std::string& _key, const std::wstring& _defaultValue) {
	return uiAPI::settings::getString(QString::fromStdString(_key), QString::fromStdWString(_defaultValue)).toStdWString();
}

void aciWidget::setSettingsValue(const std::string& _key, const std::wstring& _value) {
	uiAPI::settings::setString(QString::fromStdString(_key), QString::fromStdWString(_value));
}

void aciWidget::requestNextCommand(aci::InterpreterObject * _obj) {

}

QWidget * aciWidget::widget(void) {
	return m_centralLayoutW;
}

void aciWidget::setColorStyle(ak::aColorStyle * _colorstyle) {
	m_centralLayoutW->setStyleSheet(m_colorStyle->toStyleSheet(cafBackgroundColorControls, "#a_aci_centralLayoutW{", "}"));
}

// ###################################################################################################################################

void aciWidget::loadScripts(void) {
	aci::InterpreterCore * i = aci::API::core();
	print(L"Adding settings as a script\n");
	i->addScriptObject(Settings::instance());
#ifdef _DEBUG
	print(L"Load *.dll scripts from " + Settings::instance()->aciScriptPath() + L"/ScriptsD\n");
	aci::API::core()->scriptLoader()->loadDllsFromDirectory(Settings::instance()->aciScriptPath() + L"/ScriptsD");
#else
	print(L"Load *.dll scripts from " + Settings::instance()->aciScriptPath() + L"/Scripts\n");
	aci::API::core()->scriptLoader()->loadDllsFromDirectory(Settings::instance()->aciScriptPath() + L"/Scripts");
#endif // _DEBUG
}

void aciWidget::setInputEnabled(bool _isEnabled) {
	m_in->setEnabled(_isEnabled);
	if (_isEnabled) {
		m_inLabel->setStyleSheet("#a_aci_inputLabel{color: #40ff40;}");
		m_in->setFocus();
	}
	else {
		m_inLabel->setStyleSheet("#a_aci_inputLabel{color: #ff4040;}");
	}
}

void aciWidget::restart(void) {
	print(L"\nShutting down aci\n");

	aci::API::core()->removeScriptObject(Settings::instance()->key(), false);
	aci::API::cleanUp();

	print(L"\nRestarting aci\n");

	initialize();
}

void aciWidget::refreshAfterSettingsChanged(void) {
	{
		QFont f = m_out->font();
		f.setPointSize(Settings::instance()->aciPointSize());
		m_out->setFont(f);
	}
	{
		QFont f = m_in->font();
		f.setPointSize(Settings::instance()->aciPointSize());
		m_in->setFont(f);
	}
	{
		QFont f = m_inLabel->font();
		f.setPointSize(Settings::instance()->aciPointSize());
		m_inLabel->setFont(f);
	}
}

// ###################################################################################################################################

// Slots

void aciWidget::slotRequestInputFocus(void) {
	m_in->setFocus();
}

void aciWidget::slotHandle(void) {
	if (m_in->text().isEmpty()) { return; }
	QString cmd = m_in->text();

	if (cmd == "clear") {
		m_out->clear();
		setColor(QColor(255, 255, 255));
	}
	else {
		setColor(QColor(150, 150, 255));
		print(L"\n<: aci@" + aci::API::currentPath() + L" <: ");
		setColor(QColor(255, 255, 255));
		print(cmd + "\n");
		aci::API::core()->handle(cmd.toStdWString());
	}

	m_in->clear();
	m_inLabel->setText("<: aci@" + QString::fromStdWString(aci::API::core()->currentPath()) + " <: ");
	m_in->setFocus();
	m_commandBuffer.push_back(cmd);
	m_commandIndex = m_commandBuffer.size() + 1;
}

void aciWidget::slotSetInputEnabled(bool _enabled) {
	setInputEnabled(_enabled);
}

void aciWidget::slotPrintMessage(const QString& _message) {
	QTextCursor cursor(m_out->textCursor());
	//m_out->append(_message);
	cursor.insertText(_message);
}

void aciWidget::slotSetColor(const QColor& _color) {
	m_out->moveCursor(QTextCursor::End);
	QTextCursor cursor(m_out->textCursor());

	QTextCharFormat format = cursor.charFormat();
	format.setForeground(QBrush(_color));
	cursor.setCharFormat(format);

	m_out->setTextCursor(cursor);
}

void aciWidget::slotKeyDownOnInpout(QKeyEvent * _event) {
	if (_event->key() == Qt::Key_Up) {
		if (m_commandIndex == 0) { return; }
		else {
			m_commandIndex--;
			if (m_commandIndex == 0 || m_commandBuffer.empty()) { m_in->setText(""); }
			else { m_in->setText(m_commandBuffer[m_commandIndex - 1]); }
		}

	}
	else if (_event->key() == Qt::Key_Down) {
		if (m_commandIndex > m_commandBuffer.size()) { return; }
		m_commandIndex++;
		if (m_commandIndex == 0) { m_in->setText(""); }
		else if (m_commandIndex > m_commandBuffer.size()) {
			m_commandIndex = m_commandBuffer.size() + 1;
			m_in->setText("");
		}
		else { m_in->setText(m_commandBuffer[m_commandIndex - 1]); }
	}
	else if (_event->key() == Qt::Key_Alt) {
		m_out->setEnabled(true);
	}
}

void aciWidget::slotKeyUpOnInput(QKeyEvent * _event) {
	if (_event->key() == Qt::Key_Alt) {
		m_out->setEnabled(false);
	}
}

void aciWidget::slotTabPressOnInput(void) {
	if (m_lastCommand.isEmpty()) { return; }

	// Get input
	std::vector<std::wstring> lst;
	aci::API::core()->extractClassicSyntax(lst, m_lastCommand.toStdWString());

	if (lst.size() == 0) { return; }
	aci::InterpreterObject * item = aci::API::core()->findFirstMatchingItem(lst.front());

	if (item) {
		if (lst.size() > 1) {

		}
	}
}

void aciWidget::slotInputChanged(const QString& _text) {
	m_lastCommand = _text;
}

void aciWidget::initialize(void) {
	print(L"Initializing...\n");
	print(L"UI starting at: " + QDir::currentPath().toStdWString() + L"\n");

	// Start interpreter
	print(L"Initialize interpreter\n");
	aci::API::initialize(this, this, this);
	
	print(L"Update data path\n");
	aci::API::setCurrentPath(Settings::instance()->aciDataPath());
	print(L"Data path set to:\n\t");
	print(aci::API::currentPath());
	print(L"\n");

	print(L"Load scripts\n");
	loadScripts();

	m_inLabel->setText("<: aci@" + QString::fromStdWString(aci::API::currentPath()) + " <: ");

	setColor(aci::Color(150, 150, 255));
	print("<: aci@" + QString::fromStdWString(aci::API::currentPath()) + " <: ");
	setColor(QColor(255, 255, 255));
	print(QString("Welcome\n"));

	setColor(aci::Color(150, 150, 255));
	print("<: aci@" + QString::fromStdWString(aci::API::currentPath()) + " <: ");
	setColor(QColor(255, 255, 255));
	print(QString("Use \"?\" for help\n"));

	connect(m_in, &aLineEditWidget::returnPressed, this, &aciWidget::slotHandle);
	connect(m_in, &aLineEditWidget::textChanged, this, &aciWidget::slotInputChanged);
	connect(m_in, &aLineEditWidget::keyPressed, this, &aciWidget::slotKeyDownOnInpout);
	connect(m_in, &aLineEditWidget::keyReleased, this, &aciWidget::slotKeyUpOnInput);
	connect(m_in, &aLineEditWidget::tabPressed, this, &aciWidget::slotTabPressOnInput);
	m_in->setFocus();
}
