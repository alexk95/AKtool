#include "BodyWatcher.h"
#include "BodyWatcherVisualization.h"
#include "AppBase.h"
#include "Settings.h"
#include "PropertyManager.h"

#include <akGui/aColorStyle.h>

#include <qwtw/Plot.h>

using namespace ak;

BodyWatcherVisualization::BodyWatcherVisualization(BodyWatcher * _reference)
	: aWidget(otNone), m_reference(_reference)
{
	m_plot = new qwtw::Plot;
	m_plot->setPlotType(qwtw::AbstractPlot::Cartesian);
	
	m_plot->setGridVisible(true, false);
	m_plot->setGridColor(QColor(0, 0, 0), false);

	m_plot->setAxisTitle(qwtw::AbstractPlotAxis::xBottom, "Entry ID");
	m_plot->setAxisTitle(qwtw::AbstractPlotAxis::yLeft, "Value");
	m_plot->setAxisQuantity(qwtw::PlotDataset::axisQuantity::Magnitude);
	m_plot->setAxisAutoScale(qwtw::AbstractPlotAxis::xBottom, true);
	m_plot->setAxisAutoScale(qwtw::AbstractPlotAxis::yLeft, true);
	m_plot->setAxisLogScale(qwtw::AbstractPlotAxis::xBottom, false);
	m_plot->setAxisLogScale(qwtw::AbstractPlotAxis::yLeft, false);
	m_plot->setAxisMinValue(qwtw::AbstractPlotAxis::xBottom, 0.);

	connect(m_reference, &BodyWatcher::dataChanged, this, &BodyWatcherVisualization::slotRebuildGraph);

	slotRebuildGraph();
}

QWidget * BodyWatcherVisualization::widget(void) {
	return m_plot->widget();
}

void BodyWatcherVisualization::setColorStyle(ak::aColorStyle * _colorStyle) {
	std::string s = _colorStyle->toStyleSheet(cafBackgroundColorView | cafForegroundColorWindow, "QwtPlotCanvas {", "}").toStdString();
	if (s.length() == 0) {
		s = "QwtPlotCanvas { color: black; background-color: white; }";
	}

	m_plot->SetStyleSheets(
		_colorStyle->toStyleSheet(cafBackgroundColorView | cafForegroundColorWindow).toStdString(),
		_colorStyle->toStyleSheet(cafBackgroundColorView | cafForegroundColorWindow, "QwtPlot {", "}").toStdString(),
		s,
		_colorStyle->getViewBackgroundColor().toQColor(),
		QPen(QBrush(_colorStyle->getWindowMainForegroundColor().toQColor()), 2)
	);
	m_plot->setGridColor(_colorStyle->getControlsBorderColor().toQColor(), true);
}

void BodyWatcherVisualization::slotRebuildGraph(void) {
	m_plot->clear(true, false);

	const std::vector<BodyWatcherEntry>& data = m_reference->currentPersonData();
	size_t size = data.size();

	m_plot->setTitle(m_reference->currentName());
	
	if (size == 0) return;

	struct EntryPair {
		double *	x;
		double *	y;
	};

	EntryPair height;
	EntryPair weight;
	EntryPair bizepsL;
	EntryPair bizepsR;
	EntryPair belly;
	EntryPair thighL;
	EntryPair thighR;
	EntryPair calfL;
	EntryPair calfR;

	PropertyManager * properties = AppBase::instance()->propertyManager();

	int entriesToDisplay(0);

	// Allocate required entries
	if (properties->BodyWatcherVisShowHeight()) {
		height.x = new double[size];
		height.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowWeight()) {
		weight.x = new double[size];
		weight.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowBizepsLeft()) {
		bizepsL.x = new double[size];
		bizepsL.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowBizepsRight()) {
		bizepsR.x = new double[size];
		bizepsR.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowBelly()) {
		belly.x = new double[size];
		belly.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowThighLeft()) {
		thighL.x = new double[size];
		thighL.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowThighRight()) {
		thighR.x = new double[size];
		thighR.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowCalfLeft()) {
		calfL.x = new double[size];
		calfL.y = new double[size];
		entriesToDisplay++;
	}
	if (properties->BodyWatcherVisShowCalfRight()) {
		calfR.x = new double[size];
		calfR.y = new double[size];
		entriesToDisplay++;
	}
	
	if (entriesToDisplay == 0) return;

	double minY{ 9999999 };
	double maxY{ -9999999 };

	std::map<double, QString> labelXMap;
	dateFormat currentDateFormat = Settings::instance()->DateFormat();

	// Collect data
	for (size_t ix = 0; ix < size; ix++) {
		labelXMap.insert_or_assign((double)ix, data[ix].Date().toString("/", currentDateFormat).c_str());

		// Copy required values
		if (properties->BodyWatcherVisShowHeight()) {
			height.x[ix] = (double)ix;
			height.y[ix] = data[ix].Height();
			if (data[ix].Height() > maxY) maxY = data[ix].Height();
			if (data[ix].Height() < minY) minY = data[ix].Height();
		}
		if (properties->BodyWatcherVisShowWeight()) {
			weight.x[ix] = (double)ix;
			weight.y[ix] = data[ix].Weight();
			if (data[ix].Weight() > maxY) maxY = data[ix].Weight();
			if (data[ix].Weight() < minY) minY = data[ix].Weight();
		}
		if (properties->BodyWatcherVisShowBizepsLeft()) {
			bizepsL.x[ix] = (double)ix;
			bizepsL.y[ix] = data[ix].BizepsLeft();
			if (data[ix].BizepsLeft() > maxY) maxY = data[ix].BizepsLeft();
			if (data[ix].BizepsLeft() < minY) minY = data[ix].BizepsLeft();
		}
		if (properties->BodyWatcherVisShowBizepsRight()) {
			bizepsR.x[ix] = (double)ix;
			bizepsR.y[ix] = data[ix].BizepsRight();
			if (data[ix].BizepsRight() > maxY) maxY = data[ix].BizepsRight();
			if (data[ix].BizepsRight() < minY) minY = data[ix].BizepsRight();
		}
		if (properties->BodyWatcherVisShowBelly()) {
			belly.x[ix] = (double)ix;
			belly.y[ix] = data[ix].Belly();
			if (data[ix].Belly() > maxY) maxY = data[ix].Belly();
			if (data[ix].Belly() < minY) minY = data[ix].Belly();
		}
		if (properties->BodyWatcherVisShowThighLeft()) {
			thighL.x[ix] = (double)ix;
			thighL.y[ix] = data[ix].ThighLeft();
			if (data[ix].ThighLeft() > maxY) maxY = data[ix].ThighLeft();
			if (data[ix].ThighLeft() < minY) minY = data[ix].ThighLeft();
		}
		if (properties->BodyWatcherVisShowThighRight()) {
			thighR.x[ix] = (double)ix;
			thighR.y[ix] = data[ix].ThighRight();
			if (data[ix].ThighRight() > maxY) maxY = data[ix].ThighRight();
			if (data[ix].ThighRight() < minY) minY = data[ix].ThighRight();
		}
		if (properties->BodyWatcherVisShowCalfLeft()) {
			calfL.x[ix] = (double)ix;
			calfL.y[ix] = data[ix].CalfLeft();
			if (data[ix].CalfLeft() > maxY) maxY = data[ix].CalfLeft();
			if (data[ix].CalfLeft() < minY) minY = data[ix].CalfLeft();
		}
		if (properties->BodyWatcherVisShowCalfRight()) {
			calfR.x[ix] = (double)ix;
			calfR.y[ix] = data[ix].CalfRight();
			if (data[ix].CalfRight() > maxY) maxY = data[ix].CalfRight();
			if (data[ix].CalfRight() < minY) minY = data[ix].CalfRight();
		}
	}

	m_plot->setAxisMaxValue(qwtw::AbstractPlotAxis::xBottom, size - 1);
	m_plot->setAxisMinValue(qwtw::AbstractPlotAxis::yLeft, minY);
	m_plot->setAxisMaxValue(qwtw::AbstractPlotAxis::yLeft, maxY);

	m_plot->setAxisValueLabels(qwtw::AbstractPlotAxis::xBottom, labelXMap);

	// Add datasets
	if (properties->BodyWatcherVisShowHeight()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Height", height.x, height.y, size);
		initializeDataset(newEntry, QColor(139, 139, 139));
	}
	if (properties->BodyWatcherVisShowWeight()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Weight", weight.x, weight.y, size);
		initializeDataset(newEntry, QColor(255, 0, 0));
	}
	if (properties->BodyWatcherVisShowBizepsLeft()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Bizeps left", bizepsL.x, bizepsL.y, size);
		initializeDataset(newEntry, QColor(255, 0, 255));
	}
	if (properties->BodyWatcherVisShowBizepsRight()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Bizeps right", bizepsR.x, bizepsR.y, size);
		initializeDataset(newEntry, QColor(255, 40, 128));
	}
	if (properties->BodyWatcherVisShowBelly()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Belly", belly.x, belly.y, size);
		initializeDataset(newEntry, QColor(255, 128, 0));
	}
	if (properties->BodyWatcherVisShowThighLeft()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Thigh left", thighL.x, thighL.y, size);
		initializeDataset(newEntry, QColor(255, 255, 0));
	}
	if (properties->BodyWatcherVisShowThighRight()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Thigh right", thighR.x, thighR.y, size);
		initializeDataset(newEntry, QColor(128, 255, 40));
	}
	if (properties->BodyWatcherVisShowCalfLeft()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Calf left", calfL.x, calfL.y, size);
		initializeDataset(newEntry, QColor(0, 255, 255));
	}
	if (properties->BodyWatcherVisShowCalfRight()) {
		qwtw::PlotDataset * newEntry = m_plot->addDataset("Calf right", calfR.x, calfR.y, size);
		initializeDataset(newEntry, QColor(40, 128, 255));
	}
	
	// Replot
	m_plot->refresh();
}

void BodyWatcherVisualization::initializeDataset(qwtw::PlotDataset * _dataset, const QColor& _color, bool _isVisible) {
	_dataset->setCurveColor(_color, false);
	_dataset->setCurvePointsVisible(false, false);
	_dataset->setCurveIsVisibile(_isVisible, false);
	_dataset->repaint();

	_dataset->attach();
}
