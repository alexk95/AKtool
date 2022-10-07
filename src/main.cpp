// AK header
#include <akAPI/uiAPI.h>

// Project header
#include "AppBase.h"

#include <thread>

using namespace ak;

std::thread::id g_mainThreadId;

int main(int _argc, char ** _argv) {
	g_mainThreadId = std::this_thread::get_id();
	
	// Initialize the API. This function call is mandatory,
	// otherwise the API will not be able to create objects.
	// The initialization will create all core objects required for the API to work
	uiAPI::ini("AK Software", "AKtool");

	// Setup icon manager
	// We need to place the addIconSearchPath in a try block since a exception will be thrown if there is no valid search path
	int pathFoundCounter{ 0 };

	// Installation directory
	try { ak::uiAPI::addIconSearchPath(".\\Icons\\"); ++pathFoundCounter; }
	catch (...) {}
	// Repository icon folder
	try { ak::uiAPI::addIconSearchPath(QString(qgetenv("CNC_ROOT") + "\\Libraries\\Icons\\")); ++pathFoundCounter; }
	catch (...) {}

	// If no repository was found throw an exception
	if (pathFoundCounter == 0) {
		assert(0);
		return -1;
	}

	try {
		// Create the main class that is managing the functions of the UI
		AppBase * app = AppBase::instance();

		// Start the Qt event queue
		int returnValue = uiAPI::exec();

		return returnValue;
	}
	catch (const std::exception & e) {
		uiAPI::promptDialog::show(e.what(), "Fatal error");
		return -2;
	}
	catch (...) {
		uiAPI::promptDialog::show("Unknown error", "Fatal error");
		return -3;
	}
}