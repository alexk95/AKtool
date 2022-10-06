#pragma once

// aci header
#include <aci/aciSharedDataTypes.h>
#include <aci/InterpreterObject.h>

// AK header
#include <akCore/akCore.h>

// qt header
#include <qobject.h>
#include <qstring.h>

#include <string>

namespace ak { class aOptionsDialog; }
namespace ak { class aAbstractOptionsItem; }

class Settings : public QObject, public aci::InterpreterObject {
	Q_OBJECT
public:
	static Settings *  instance(void);
	static void clearInstance(void);

	// ######################################################################################################

	// Base class functions

	virtual std::wstring key(void) const override { return L"settings"; }

	virtual bool handle(const std::wstring& _command, const std::vector<std::wstring>& _params) override;

	// ######################################################################################################

	// Getter

	const std::wstring& aciScriptPath(void) const { return m_aciScriptPath; }

	const std::wstring& aciDataPath(void) const { return m_aciDataPath; }

	const QString& dataPath(void) const { return m_dataPath; }

	bool allowDoubleClickOnTabToolbar(void) const { return m_allowDoubleClickOnTabToolbar; }

	const QString& lengthUnit(void) const { return m_lengthUnit; }

	const QString& weightUnit(void) const { return m_weightUnit; }

	ak::dateFormat DateFormat(void) const { return m_dateFormat; }

	int notebookChecklistPointSize(void) const { return m_notebookChecklistPointSize; }

	// ######################################################################################################

	// Command handling

	void cmdConfiguration(void);

	void showDialog(void);

protected:
	virtual void showCommandInfo(void) override;

private slots:
	void slotSettingsItemChanged(ak::aAbstractOptionsItem * _item);

private:

	ak::aOptionsDialog *		m_dialog;
	QString						m_colorStyle;
	bool						m_allowDoubleClickOnTabToolbar;

	QString						m_dataPath;
	std::wstring				m_aciScriptPath;
	std::wstring				m_aciDataPath;

	QString						m_lengthUnit;
	QString						m_weightUnit;

	ak::dateFormat				m_dateFormat;

	int							m_notebookChecklistPointSize;

	Settings();
	~Settings();
	Settings(Settings&) = delete;
	Settings& operator = (Settings&) = delete;
};
