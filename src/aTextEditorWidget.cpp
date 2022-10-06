#include "aTextEditorWidget.h"

#include <akCore/aAssert.h>
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aToolButtonWidget.h>
#include <akWidgets/aComboButtonWidget.h>
#include <akWidgets/aSpinBoxWidget.h>

#include <akWidgets/aTtbPage.h>
#include <akWidgets/aTtbGroup.h>
#include <akWidgets/aTtbSubgroup.h>

ak::aTextEditorWidget::aTextEditorWidget()
	: aWidget(otNone), m_ttbPage(nullptr), m_ttbFontGroup(nullptr), m_ttbFontSubgroupA(nullptr)
{
	// Create controls
	m_textEdit = new aTextEditWidget;

	m_fontFamily = new aComboButtonWidget;
	m_fontFamily->setUid(1);

	m_fontSize = new aSpinBoxWidget;
	m_fontSize->setUid(2);

	// Connect signals
	connect(m_textEdit, &aTextEditWidget::textChanged, this, &aTextEditorWidget::slotTextChanged);
}

ak::aTextEditorWidget::~aTextEditorWidget() {

}

// ################################################################################################

// Base class functions

QWidget * ak::aTextEditorWidget::widget(void) {
	return m_textEdit; 
}

void ak::aTextEditorWidget::setColorStyle(aColorStyle * _colorStyle) {
	m_textEdit->setColorStyle(_colorStyle);
}

// ################################################################################################

// Visualization

void ak::aTextEditorWidget::placeWidgetsIntoToolbar(aTtbPage * _ttbPage) {
	if (_ttbPage == nullptr) return;
	if (m_ttbPage) {
		aAssert(0, "Clean up ttb");
		// ToDo
	}

	m_ttbPage = _ttbPage;
	
	// Create groups
	placeFontWidgetsIntoToolbar(dynamic_cast<aTtbGroup *>(m_ttbPage->createSubContainer("Font")));
}

void ak::aTextEditorWidget::placeFontWidgetsIntoToolbar(aTtbGroup * _ttbGroup) {
	if (_ttbGroup == nullptr) return;
	if (m_ttbFontGroup) {
		aAssert(0, "Clean up ttb");
		// ToDo
	}

	m_ttbFontGroup = _ttbGroup;

	// Create subgroup A
	m_ttbFontSubgroupA = dynamic_cast<aTtbSubGroup *>(m_ttbFontGroup->createSubContainer());

	m_ttbFontSubgroupA->addChild(m_fontFamily);
	m_ttbFontSubgroupA->addChild(m_fontSize);
}

// ################################################################################################

// Setter/Getter

void ak::aTextEditorWidget::setText(const QString& _text) {
	m_textEdit->setText(_text);
}

QString ak::aTextEditorWidget::toPlainText(void) const {
	return m_textEdit->toPlainText();
}

// ################################################################################################

// Slots

void ak::aTextEditorWidget::slotTextChanged(void) {

}