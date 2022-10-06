#include "PropertyManager.h"
#include "AppBase.h"
#include "BodyWatcherVisualization.h"

#include <akAPI/uiAPI.h>
#include <akWidgets/aPropertyGridWidget.h>

using namespace ak;

PropertyManager::PropertyManager() 
	: aWidget(otNone)
{
	m_propertyGrid = new aPropertyGridWidget;

	// Restore settings
	m_BodyWatcherVisShowHeight = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_HEIGHT", true);
	m_BodyWatcherVisShowWeight = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_WEIGHT", true);
	m_BodyWatcherVisShowBizepsLeft = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_BIZEPSL", true);
	m_BodyWatcherVisShowBizepsRight = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_BIZEPSR", true);
	m_BodyWatcherVisShowBelly = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_BELLY", true);
	m_BodyWatcherVisShowThighLeft = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_THIGHL", true);
	m_BodyWatcherVisShowThighRight = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_THIGHR", true);
	m_BodyWatcherVisShowCalfLeft = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_CALFL", true);
	m_BodyWatcherVisShowCalfRight = uiAPI::settings::getBool("PROPERTY_BODYWATCH_SHOW_CALFR", true);

	connect(m_propertyGrid, &aPropertyGridWidget::itemChanged, this, &PropertyManager::slotItemChanged);
}

PropertyManager::~PropertyManager() {
	delete m_propertyGrid;
}

// ##################################################################################

// Base class functions

QWidget * PropertyManager::widget(void) {
	return m_propertyGrid->widget();
}

void PropertyManager::setColorStyle(ak::aColorStyle * _colorStyle) {
	m_propertyGrid->setColorStyle(_colorStyle);
}

// ##################################################################################

// Data handling

void PropertyManager::setCurrentView(CurrentView _view) {
	m_propertyGrid->clear();
	m_currentView = _view;
	
	switch (m_currentView)
	{
	case PropertyManager::NO_VIEW: return;
	case PropertyManager::BODY_WATCH: return;
	case PropertyManager::BODY_WATCH_VIS:
		m_propertyGrid->addGroup("Data to display", aColor(255, 128, 128));
		m_propertyGrid->addItem("Data to display", "Height", m_BodyWatcherVisShowHeight);
		m_propertyGrid->addItem("Data to display", "Weight", m_BodyWatcherVisShowWeight);
		m_propertyGrid->addItem("Data to display", "Bizeps left", m_BodyWatcherVisShowBizepsLeft);
		m_propertyGrid->addItem("Data to display", "Bizeps right", m_BodyWatcherVisShowBizepsRight);
		m_propertyGrid->addItem("Data to display", "Belly", m_BodyWatcherVisShowBelly);
		m_propertyGrid->addItem("Data to display", "Thigh left", m_BodyWatcherVisShowThighLeft);
		m_propertyGrid->addItem("Data to display", "Thigh right", m_BodyWatcherVisShowThighRight);
		m_propertyGrid->addItem("Data to display", "Calf left", m_BodyWatcherVisShowCalfLeft);
		m_propertyGrid->addItem("Data to display", "Calf right", m_BodyWatcherVisShowCalfRight);
		break;
	case PropertyManager::ACI: return;
	case PropertyManager::COOKBOOK: return;
	case PropertyManager::NOTEBOOK: return;
	default:
		break;
	}
}

// ##################################################################################

// Slots

void PropertyManager::slotItemChanged(ak::ID _itemId) {
	aPropertyGridItem * item = m_propertyGrid->item(_itemId);
	if (item) {
		switch (m_currentView)
		{
		case PropertyManager::NO_VIEW: break;
		case PropertyManager::BODY_WATCH: break;
		case PropertyManager::BODY_WATCH_VIS: bodyWatcherVisualizationPropertyChanged(item); break;
		case PropertyManager::ACI: break;
		default: assert(0); break;
		}
	}
}

// ##################################################################################

// Private functions

void PropertyManager::bodyWatcherVisualizationPropertyChanged(ak::aPropertyGridItem * _item) {
	if (_item->group()->title() == "Data to display") {
		QString n = _item->name();
		if (_item->name() == "Height") {
			m_BodyWatcherVisShowHeight = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_HEIGHT", m_BodyWatcherVisShowHeight);
		}
		else if (_item->name() == "Weight") {
			m_BodyWatcherVisShowWeight = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_WEIGHT", m_BodyWatcherVisShowWeight);
		}
		else if (_item->name() == "Bizeps left") {
			m_BodyWatcherVisShowBizepsLeft = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_BIZEPSL", m_BodyWatcherVisShowBizepsLeft);
		}
		else if (_item->name() == "Bizeps right") {
			m_BodyWatcherVisShowBizepsRight = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_BIZEPSR", m_BodyWatcherVisShowBizepsRight);
		}
		else if (_item->name() == "Belly") {
			m_BodyWatcherVisShowBelly = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_BELLY", m_BodyWatcherVisShowBelly);
		}
		else if (_item->name() == "Thigh left") {
			m_BodyWatcherVisShowThighLeft = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_THIGHL", m_BodyWatcherVisShowThighLeft);
		}
		else if (_item->name() == "Thigh right") {
			m_BodyWatcherVisShowThighRight = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_THIGHR", m_BodyWatcherVisShowThighRight);
		}
		else if (_item->name() == "Calf left") {
			m_BodyWatcherVisShowCalfLeft = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_CALFL", m_BodyWatcherVisShowCalfLeft);
		}
		else if (_item->name() == "Calf right") {
			m_BodyWatcherVisShowCalfRight = _item->getBool();
			uiAPI::settings::setBool("PROPERTY_BODYWATCH_SHOW_CALFR", m_BodyWatcherVisShowCalfRight);
		}
		else {
			assert(0); // Unknown item
			return;
		}
		AppBase::instance()->bodyWatcherVisualization()->slotRebuildGraph();
	}
	else {
		assert(0); // Unknown group
	}
}