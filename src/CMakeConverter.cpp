#include "CMakeConverter.h"
#include "CMakeVSConverter.h"
#include "AppBase.h"

// AK widgets
#include <akAPI/uiAPI.h>
#include <akWidgets/aWindow.h>

// Qt header
#include <qfiledialog.h>

void CMakeConverter::runVSToCMake(void) {

	// User file promt
	QString vsProjFileName = QFileDialog::getOpenFileName(AppBase::instance()->window(), "Visual Studio C++ Project file", ak::uiAPI::settings::getString("CMAKE_CONVERTER_CXX_VS_DIR", QDir::currentPath()), "Visual Studio C++ Project File(*.vcxproj)");
	if (vsProjFileName.isEmpty()) return;
	
	// Check if file exists
	QFile vsProjFile(vsProjFileName);
	if (!vsProjFile.exists()) return;

	// Open file for reading
	if (!vsProjFile.open(QIODevice::ReadOnly)) return;

	// Read file
	QByteArray vsProjFileContent = vsProjFile.readAll();
	vsProjFile.close();

	// Parse file
	CMakeVSConverter::VS_CXX_Project project;
	if (!project.setFromProjectFile(vsProjFileContent)) return;

	ak::uiAPI::settings::setString("CMAKE_CONVERTER_CXX_VS_DIR", vsProjFileName);
}