#pragma once

#include <akWidgets/aWidget.h>

#include <qobject.h>

namespace ak {
	class aTextEditWidget;
	class aToolButtonWidget;
	class aComboButtonWidget;
	class aSpinBoxWidget;
	class aTtbPage;
	class aTtbGroup;
	class aTtbSubGroup;

	class aTextEditorWidget : public QObject, public ak::aWidget {
		Q_OBJECT
	public:
		aTextEditorWidget();
		virtual ~aTextEditorWidget();

		// ################################################################################################

		// Base class functions

		virtual QWidget * widget(void) override;
		virtual void setColorStyle(aColorStyle * _colorStyle) override;

		// ################################################################################################

		// Visualization

		void placeWidgetsIntoToolbar(aTtbPage * _ttbPage);
		void placeFontWidgetsIntoToolbar(aTtbGroup * _ttbGroup);

		// ################################################################################################

		// Setter/Getter

		aComboButtonWidget * fontFamilyWidget(void) { return m_fontFamily; }
		aSpinBoxWidget * fontSizeWidget(void) { return m_fontSize; }

		void setText(const QString& _text);
		QString toPlainText(void) const;

		// ################################################################################################

	signals:
		void textChanged(void);

	private slots:
		void slotTextChanged(void);

	private:
		aTextEditWidget *		m_textEdit;

		aTtbPage *				m_ttbPage;

		aTtbGroup *				m_ttbFontGroup;
		aTtbSubGroup *			m_ttbFontSubgroupA;

		aComboButtonWidget *	m_fontFamily;
		aSpinBoxWidget *		m_fontSize;
	};
}