#pragma once

#include "qstring.h"

class AbstractNavigationManagerCallbackObject {
public:
	AbstractNavigationManagerCallbackObject() {};
	virtual ~AbstractNavigationManagerCallbackObject() {};

	virtual void navigationItemSelected(int _itemID) = 0;
	virtual void navigationSelectionCleared(void) = 0;
};