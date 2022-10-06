#pragma once

#include <aci/InterpreterObject.h>

#include <akTools/ClassHelper.h>
#include <akCore/aDate.h>
#include <akGui/aDialog.h>
#include <akWidgets/aWidget.h>

#include <qobject.h>
#include <qstring.h>
#include <qwidget.h>
#include <qjsonobject.h>

#include <vector>

// Forwad declarations

namespace ak { class aLabelWidget; }
namespace ak { class aComboButtonWidget; }
namespace ak { class aComboBoxWidget; }
namespace ak { class aDoubleSpinBoxWidget; }
namespace ak { class aDatePickWidget; }
namespace ak { class aLineEditWidget; }
namespace ak { class aPushButtonWidget; }

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

class BodyWatcherEntryWidget;

class BodyWatcherEntry {
public:
	BodyWatcherEntry();
	BodyWatcherEntry(double _height, double _weight, double _bizepsLeft, double _bizepsRight,
		double _belly, double _thighLeft, double _thighRight, double _calfLeft,
		double _calfRight, const ak::aDate& _date, const QString& _personName);
	BodyWatcherEntry(const BodyWatcherEntry& _other);
	virtual ~BodyWatcherEntry();

	BodyWatcherEntry& operator = (const BodyWatcherEntry& _other);

	bool setFromJsonObject(const QJsonObject& _object);
	void writeToJsonObject(QJsonObject& _object) const;

	A_PROPERTY(double, Height);
	A_PROPERTY(double, Weight);
	A_PROPERTY(double, BizepsLeft);
	A_PROPERTY(double, BizepsRight);
	A_PROPERTY(double, Belly);
	A_PROPERTY(double, ThighLeft);
	A_PROPERTY(double, ThighRight);
	A_PROPERTY(double, CalfLeft);
	A_PROPERTY(double, CalfRight);
	A_PROPERTY_CREF(ak::aDate, Date);
	A_PROPERTY_CREF(QString, PersonName);
};

// ####################################################################################################

// ####################################################################################################

// ####################################################################################################

class BodyWatcher : public QObject, public ak::aWidget, public aci::InterpreterObject {
	Q_OBJECT
public:
	static QString TabText(void) { return "Body Watcher"; }

	BodyWatcher();
	virtual ~BodyWatcher();

	// ###################################################################################

	// Base class functions

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

	virtual std::wstring key(void) const override { return L"bodywatcher"; }
	virtual bool handle(const std::wstring& _command, const std::vector<std::wstring>& _params) override;

	// ###################################################################################

	// Data handling

	bool isConnectedToAci(void) const { return m_isConnectedToAci; }

	void connectToAci(void);

	void aciDisconnected(void);

	void addEntry(void);

	void fillEntries(void);

	void clearData(void);

	bool loadData(void);

	bool saveData(bool _showPromt);

	bool hasUnsavedChanges(void) const { return m_hasUnsavedChanges; }

	bool hasEntry(const QString& _personName, const ak::aDate& _date);

	void deleteEntry(const QString& _personName, const ak::aDate& _date);

	void replaceEntry(size_t _index, const BodyWatcherEntry& _newEntry);

	QString currentName(void) const;

	const std::vector<BodyWatcherEntry>& currentPersonData(void) const { return m_personData; }

signals:
	void dataChanged(void);

protected:
	virtual void showCommandInfo(void) override;

private slots:
	void nameChanged(void);
	void displayData(void);

private:

	void printPrefix(void);

	void printEntry(const BodyWatcherEntry& _entry);

	std::string fillIndex(int _ix, int _len);

	bool aciShowData(const std::vector<std::wstring>& _params);

	bool aciModifyData(const std::vector<std::wstring>& _params);

	bool aciDeleteEntry(const std::vector<std::wstring>& _params);

	bool aciSaveData(void);

	double calculateNextStepValue(double _initialValue, double _currentStepValue, double _lastValue, size_t _steps);

	bool							m_isConnectedToAci;
	bool							m_isReading;
	bool							m_hasUnsavedChanges;

	std::vector<BodyWatcherEntry>	m_personData;

	QWidget *						m_centralLayoutW;
	QVBoxLayout *					m_centralLayout;

	QWidget *						m_nameLayoutW;
	QHBoxLayout *					m_nameLayout;

	QWidget *						m_dataLayoutW;
	QGridLayout *					m_dataLayout;
	
	ak::aLabelWidget *				m_nameLabel;
	ak::aComboButtonWidget *		m_nameInput;

	BodyWatcherEntryWidget *		m_entryInitial;
	BodyWatcherEntryWidget *		m_entry1;
	BodyWatcherEntryWidget *		m_entry2;
	BodyWatcherEntryWidget *		m_entry3;
	BodyWatcherEntryWidget *		m_entry4;
	BodyWatcherEntryWidget *		m_entryLatest;

	std::vector<BodyWatcherEntry>	m_data;
};

// ####################################################################################################

// ####################################################################################################

// ####################################################################################################

class BodyWatcherEntryWidget : public ak::aWidget {
public:
	BodyWatcherEntryWidget();
	BodyWatcherEntryWidget(const BodyWatcherEntry& _entry);
	virtual ~BodyWatcherEntryWidget();

	// ###################################################################################

	// Base class functions

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

	// ###################################################################################

	// Getter/Setter

	void setObjectName(const QString& _name);

	void setEntry(const BodyWatcherEntry& _entry);
	const BodyWatcherEntry& entry(void) const { return m_entry; }

	void setPreviousEntry(BodyWatcherEntryWidget * _entry);
	BodyWatcherEntryWidget * previousEntry(void) { return m_prevEntry; }

	void calculateValues(void);

private:
	void buildWidget(void);
	
	double differenceInPercent(double _current, double _previous);

	QString nicePercent(double _perc);

	BodyWatcherEntryWidget *	m_prevEntry;

	BodyWatcherEntry			m_entry;

	QWidget *					m_layoutW;
	QGridLayout *				m_layout;

	ak::aLabelWidget *			m_date;

	ak::aLabelWidget *			m_heightL;
	ak::aLabelWidget *			m_height;
	ak::aLabelWidget *			m_heightDiff;

	ak::aLabelWidget *			m_weightL;
	ak::aLabelWidget *			m_weight;
	ak::aLabelWidget *			m_weightDiff;
	
	ak::aLabelWidget *			m_bizepsLeftL;
	ak::aLabelWidget *			m_bizepsLeft;
	ak::aLabelWidget *			m_bizepsLeftDiff;
	
	ak::aLabelWidget *			m_bizepsRightL;
	ak::aLabelWidget *			m_bizepsRight;
	ak::aLabelWidget *			m_bizepsRightDiff;
	
	ak::aLabelWidget *			m_bellyL;
	ak::aLabelWidget *			m_belly;
	ak::aLabelWidget *			m_bellyDiff;
	
	ak::aLabelWidget *			m_thighLeftL;
	ak::aLabelWidget *			m_thighLeft;
	ak::aLabelWidget *			m_thighLeftDiff;

	ak::aLabelWidget *			m_thighRightL;
	ak::aLabelWidget *			m_thighRight;
	ak::aLabelWidget *			m_thighRightDiff;

	ak::aLabelWidget *			m_calfRightL;
	ak::aLabelWidget *			m_calfRight;
	ak::aLabelWidget *			m_calfRightDiff;

	ak::aLabelWidget *			m_calfLeftL;
	ak::aLabelWidget *			m_calfLeft;
	ak::aLabelWidget *			m_calfLeftDiff;

	BodyWatcherEntryWidget(BodyWatcherEntryWidget&) = delete;
	BodyWatcherEntryWidget& operator = (BodyWatcherEntryWidget&) = delete;
};

// ####################################################################################################

// ####################################################################################################

// ####################################################################################################

class BodyWatcherNewEntryDialog : public ak::aDialog {
	Q_OBJECT
public:
	BodyWatcherNewEntryDialog(const BodyWatcherEntry& _latestEntry, const QStringList& _possibleNames, BodyWatcher * _owner);
	virtual ~BodyWatcherNewEntryDialog();

	BodyWatcherEntry createEntry(void);

	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

private slots:
	void slotAdd(void);
	void slotCancel(void);

private:
	BodyWatcher *				m_owner;

	QVBoxLayout *				m_centralLayout;

	QWidget *					m_inputLayoutW;
	QGridLayout *				m_inputLayout;

	QWidget *					m_buttonLayoutW;
	QHBoxLayout *				m_buttonLayout;

	struct InputPairDouble {
		ak::aLabelWidget *			label;
		ak::aDoubleSpinBoxWidget *	input;
	};

	InputPairDouble createDoublePair(const QString& _labelText, double _value, int _layoutRow);
	void paintDoublePair(InputPairDouble _pair);

	ak::aLabelWidget *			m_nameL;
	ak::aComboBoxWidget *		m_name;

	ak::aLabelWidget *			m_dateL;
	ak::aDatePickWidget *		m_date;

	InputPairDouble				m_height;
	InputPairDouble				m_weight;
	InputPairDouble				m_bizepsL;
	InputPairDouble				m_bizepsR;
	InputPairDouble				m_belly;
	InputPairDouble				m_thighL;
	InputPairDouble				m_thighR;
	InputPairDouble				m_calfL;
	InputPairDouble				m_calfR;

	ak::aPushButtonWidget *		m_buttonAdd;
	ak::aPushButtonWidget *		m_buttonCancel;
};
