#include "NewNameDialog.h"
#include "AppBase.h"

#include <akAPI/uiAPI.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aWindow.h>

#include <qlayout.h>

using namespace ak;

NewNameDialog::NewNameDialog(const QString& _title, const QString& _inputLabelText, const QString& _confirmButtonText) {
	// Create layouts
	m_centralLayout = new QVBoxLayout(this);

	m_inputLayoutW = new QWidget;
	m_inputLayout = new QGridLayout(m_inputLayoutW);

	m_buttonLayoutW = new QWidget;
	m_buttonLayout = new QHBoxLayout(m_buttonLayoutW);

	// Create controls
	m_input = new aLineEditWidget;
	m_inputL = new aLabelWidget(_inputLabelText);
	m_inputL->setBuddy(m_input);

	m_buttonConfirm = new aPushButtonWidget(_confirmButtonText);
	m_buttonCancel = new aPushButtonWidget("Cancel");

	// Setup layouts
	m_centralLayout->addWidget(m_inputLayoutW, 1);
	m_centralLayout->addWidget(m_buttonLayoutW, 0);

	m_inputLayout->addWidget(m_inputL, 0, 0);
	m_inputLayout->addWidget(m_input, 0, 1);

	m_buttonLayout->addStretch(1);
	m_buttonLayout->addWidget(m_buttonConfirm);
	m_buttonLayout->addWidget(m_buttonCancel);

	// Setup Dialog
	setWindowTitle(_title);

	uiAPI::addPaintable(this);

	connect(m_buttonConfirm, &aPushButtonWidget::clicked, this, &NewNameDialog::slotConfirm);
	connect(m_buttonCancel, &aPushButtonWidget::clicked, this, &NewNameDialog::slotCancel);
}

NewNameDialog::~NewNameDialog() {
	uiAPI::removePaintable(this);

	delete m_buttonCancel;
	delete m_buttonConfirm;
	delete m_input;
	delete m_inputL;

	delete m_buttonLayout;
	delete m_buttonLayoutW;

	delete m_inputLayout;
	delete m_inputLayoutW;

	delete m_centralLayout;
}

void NewNameDialog::setColorStyle(ak::aColorStyle * _colorStyle) {
	aDialog::setColorStyle(_colorStyle);
	m_input->setColorStyle(_colorStyle);
	m_inputL->setColorStyle(_colorStyle);
	m_buttonCancel->setColorStyle(_colorStyle);
	m_buttonConfirm->setColorStyle(_colorStyle);
}

QString NewNameDialog::inputText(void) const {
	return m_input->text();
}

void NewNameDialog::slotCancel(void) {
	Close(resultCancel);
}

void NewNameDialog::slotConfirm(void) {
	Close(resultOk);
}