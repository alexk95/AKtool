#pragma once

#include <aci/AbstractPrinter.h>
#include <aci/AbstractInterpreterNotifier.h>
#include <aci/AbstractOSHandler.h>

// uiCore header
#include <akCore/globalDataTypes.h>		// UID type
#include <akWidgets/aWidget.h>

#include <qobject.h>
#include <qstring.h>
#include <qcolor.h>

// Forward declarations
class TabToolbar;
class QWidget;
class QVBoxLayout;
class QHBoxLayout;
class QKeyEvent;

namespace ak {
	class aLineEditWidget;
	class aTextEditWidget;
	class aLabelWidget;
	class aWindowManager;
}

class aciWidget : public QObject, public ak::aWidget, public aci::AbstractPrinter, public aci::AbstractInterpreterNotifier, public aci::AbstractOSHandler {
	Q_OBJECT
public:
	static QString TabText(void) { return "ACI"; };

	aciWidget();
	virtual ~aciWidget();

	virtual void disableInput(void) override;
	virtual void disableInputAsync(void) override;
	virtual void enableInput(void) override;
	virtual void enableInputAsync(void) override;

	virtual void shutdown(void) override;

	virtual void print(const std::string& _str) override;
	virtual void print(const std::wstring& _str) override;
	void print(const QString& _str);
	virtual void printAsync(const std::string& _str) override;
	virtual void printAsync(const std::wstring& _str) override;
	void printAsync(const QString& _str);

	virtual void setColor(const aci::Color& _color) override;
	virtual void setColorAsync(const aci::Color& _color) override;
	void setColor(const QColor& _color);
	void setColorAsync(const QColor& _color);

	virtual bool fileExists(const std::wstring& _path) override;
	virtual bool deleteFile(const std::wstring& _path) override;
	virtual bool readFile(std::wstring& _data, const std::wstring& _path) override;
	virtual bool editFile(std::list<std::wstring>& _data, std::wstring& _path) override;
	virtual bool readLinesFromFile(std::list<std::wstring>& _data, const std::wstring& _path) override;
	virtual bool writeFile(const std::wstring& _data, const std::wstring& _path) override;
	virtual bool writeLinesToFile(const std::list<std::wstring>& _data, const std::wstring& _path) override;

	virtual bool directoryExists(const std::wstring& _path) override;
	virtual std::list<std::wstring> filesInDirectory(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) override;
	virtual std::list<std::wstring> subdirectories(const std::wstring& _path, bool _searchTopLevelDirectoryOnly) override;
	virtual std::wstring currentDirectory(void) override;
	virtual std::wstring scriptDataDirectory(void) override;

	virtual std::wstring getSettingsValue(const std::string& _key, const std::wstring& _defaultValue = std::wstring()) override;
	virtual void setSettingsValue(const std::string& _key, const std::wstring& _value) override;

	//! @brief Will request the next command from the Interpreter
	//! The request will be queued. When the user has finished the input the command will be send to the proivided object
	//! @param _obj The object the input will be send to
	virtual void requestNextCommand(aci::InterpreterObject * _obj) override;

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorstyle) override;

	// ###################################################################################################################################

	void loadScripts(void);

	void setInputEnabled(bool _isEnabled);

	void restart(void);

	void refreshAfterSettingsChanged(void);

	// ###################################################################################################################################

	// Slots

private slots:
	void slotRequestInputFocus(void);

	void slotHandle(void);

	void slotSetInputEnabled(bool _enabled);
	void slotPrintMessage(const QString& _message);
	void slotSetColor(const QColor& _color);

	void slotKeyDownOnInpout(QKeyEvent * _event);

	void slotKeyUpOnInput(QKeyEvent * _event);

	void slotTabPressOnInput(void);

	void slotInputChanged(const QString& _text);

private:
	void initialize(void);

	QWidget *				m_centralLayoutW;
	QVBoxLayout *			m_centralLayout;

	QWidget *				m_inLayoutW;
	QHBoxLayout *			m_inLayout;

	ak::aTextEditWidget *	m_out;
	ak::aLineEditWidget *	m_in;
	ak::aLabelWidget *		m_inLabel;

	std::vector<QString>	m_commandBuffer;
	size_t					m_commandIndex;
	QString					m_lastCommand;

	aciWidget(aciWidget&) = delete;
	aciWidget& operator = (aciWidget&) = delete;

};
