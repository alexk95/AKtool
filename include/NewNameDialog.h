#pragma once

#include <akGui/aDialog.h>


class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

namespace ak {
	class aLineEditWidget;
	class aLabelWidget;
	class aPushButtonWidget;
}

class NewNameDialog : public ak::aDialog {
	Q_OBJECT
public:
	NewNameDialog(const QString& _title, const QString& _inputLabelText, const QString& _confirmButtonText);
	virtual ~NewNameDialog();

	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

	QString inputText(void) const;

private slots:
	void slotCancel(void);
	void slotConfirm(void);

private:
	QVBoxLayout *		m_centralLayout;

	QWidget *			m_inputLayoutW;
	QGridLayout *		m_inputLayout;

	QWidget *			m_buttonLayoutW;
	QHBoxLayout *		m_buttonLayout;

	ak::aLineEditWidget *	m_input;
	ak::aLabelWidget *		m_inputL;

	ak::aPushButtonWidget *	m_buttonConfirm;
	ak::aPushButtonWidget *	m_buttonCancel;

	NewNameDialog() = delete;
};