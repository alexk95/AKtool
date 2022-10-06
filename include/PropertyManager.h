#pragma once

// AK header
#include <akTools/ClassHelper.h>
#include <akWidgets/aWidget.h>

// Qt header
#include <qobject.h>

namespace ak { class aPropertyGridWidget; }
namespace ak { class aPropertyGridItem; }

class PropertyManager : public QObject, public ak::aWidget {
	Q_OBJECT
public:
	enum CurrentView {
		NO_VIEW,
		BODY_WATCH,
		BODY_WATCH_VIS,
		ACI,
		COOKBOOK,
		NOTEBOOK
	};

	PropertyManager();
	virtual ~PropertyManager();

	A_PROPERTY(bool, BodyWatcherVisShowHeight);
	A_PROPERTY(bool, BodyWatcherVisShowWeight);
	A_PROPERTY(bool, BodyWatcherVisShowBizepsLeft);
	A_PROPERTY(bool, BodyWatcherVisShowBizepsRight);
	A_PROPERTY(bool, BodyWatcherVisShowBelly);
	A_PROPERTY(bool, BodyWatcherVisShowThighLeft);
	A_PROPERTY(bool, BodyWatcherVisShowThighRight);
	A_PROPERTY(bool, BodyWatcherVisShowCalfLeft);
	A_PROPERTY(bool, BodyWatcherVisShowCalfRight);

	// ##################################################################################

	// Base class functions

	virtual QWidget * widget(void) override;
	virtual void setColorStyle(ak::aColorStyle * _colorStyle) override;

	// ##################################################################################

	// Data handling

	void setCurrentView(CurrentView _view);

private slots:
	void slotItemChanged(ak::ID _itemId);

private:

	void bodyWatcherVisualizationPropertyChanged(ak::aPropertyGridItem * _item);

	ak::aPropertyGridWidget *		m_propertyGrid;

	CurrentView						m_currentView;
};