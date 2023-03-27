#pragma once

#include <QtCore/qobject.h>

#include <akWidgets/aWidget.h>

class QSplitter;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class Wishlist : public QObject, public ak::aWidget {
	Q_OBJECT
public:
	Wishlist();
	virtual ~Wishlist();

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

private:



};