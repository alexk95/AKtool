#pragma once

#include <akWidgets/aWidget.h>

#include <qobject.h>

class QWidget;
class BodyWatcher;
namespace qwtw { class Plot; }
namespace qwtw { class PlotDataset; }

class BodyWatcherVisualization : public QObject, public ak::aWidget {
	Q_OBJECT
public:
	static QString TabText(void) { return "Body Watcher | Visualization"; }

	BodyWatcherVisualization(BodyWatcher * _reference);

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

public slots:
	void slotRebuildGraph(void);

private:
	void initializeDataset(qwtw::PlotDataset * _dataset, const QColor& _color, bool _isVisible = true);

	BodyWatcher *		m_reference;
	qwtw::Plot *		m_plot;

};