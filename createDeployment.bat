@ECHO OFF

REM This script requires the following environment variables to be set:
REM 1. UI_CORE_ROOT
REM 2. DEVENV_ROOT
REM 3. QTDIR_ROOT
REM 4. ACI_ROOT
REM 5. QWT_WRAPPER_ROOT
REM 6. AK_TOOL_ROOT

IF "%UI_CORE_ROOT%" == "" (
	ECHO Please specify the following environment variables: UI_CORE_ROOT
	goto END
)

IF "%DEVENV_ROOT%" == "" (
	ECHO Please specify the following environment variables: DEVENV_ROOT
	goto END
)

IF "%QTDIR_ROOT%" == "" (
	ECHO Please specify the following environment variables: QTDIR_ROOT
	goto END
)

IF "%ACI_ROOT%" == "" (
	ECHO Please specify the following environment variables: ACI_ROOT
	goto END
)

IF "%QWT_WRAPPER_ROOT%" == "" (
	ECHO Please specify the following environment variables: QWT_WRAPPER_ROOT
	goto END
)

IF "%AK_TOOL_ROOT%" == "" (
	ECHO Please specify the following environment variables: AK_TOOL_ROOT
	goto END
)

REM Setup eviroment
CALL "%AK_TOOL_ROOT%\setupEnvironment.bat"

IF "%AK_TOOL_DEPLOYMENT_DIR%" == "" (
    SET AK_TOOL_DEPLOYMENT_DIR="%AK_TOOL_ROOT%\Deployment"
)

REM Clean up the Deployment directory

RMDIR /S /Q "%AK_TOOL_DEPLOYMENT_DIR%"
MKDIR "%AK_TOOL_DEPLOYMENT_DIR%"

REM ===========================================================================
REM Copy the library files
REM ===========================================================================

REM Qwt
COPY "%QWT_LIB_ROOT%\lib\qwt.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QWT_POLAR_LIB_ROOT%\lib\qwtpolar.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM QtTabToolbar
COPY "%UI_CORE_ROOT%\Third_Party_Libraries\QtTabToolbar\src\TabToolbar\Release\TabToolbar.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM Qt
COPY "%QTDIR_ROOT%\bin\libEGL.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\libGLESv2.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\opengl32sw.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DAnimation.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DCore.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DExtras.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DInput.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DLogic.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuick.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickAnimation.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickExtras.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickInput.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickRender.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DQuickScene2D.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt53DRender.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Bluetooth.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Charts.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Concurrent.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Core.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5DataVisualization.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5DBus.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Gamepad.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Gui.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Help.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Location.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Multimedia.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5MultimediaQuick.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5MultimediaWidgets.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Network.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5NetworkAuth.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Nfc.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5OpenGL.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Positioning.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5PositioningQuick.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5PrintSupport.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Purchasing.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Qml.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Quick.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickControls2.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickParticles.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickTemplates2.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickTest.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5QuickWidgets.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5RemoteObjects.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Script.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5ScriptTools.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Scxml.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Sensors.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5SerialBus.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5SerialPort.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Sql.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Svg.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Test.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5TextToSpeech.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebChannel.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebEngine.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebEngineCore.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebEngineWidgets.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebSockets.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WebView.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Widgets.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5WinExtras.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5Xml.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%QTDIR_ROOT%\bin\Qt5XmlPatterns.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

MKDIR "%AK_TOOL_DEPLOYMENT_DIR%\plugins"
XCOPY /S "%QTDIR_ROOT%\plugins" "%AK_TOOL_DEPLOYMENT_DIR%\plugins"

REM Copy the icons

MKDIR "%AK_TOOL_DEPLOYMENT_DIR%\icons"
XCOPY /S "%AK_TOOL_ROOT%\Icons" "%AK_TOOL_DEPLOYMENT_DIR%\icons"

REM CURL

COPY "%CURL_DLL%\libcurl.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%CURL_DLL%\zlib1.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM OPENSSL

COPY "%OPENSSL_DLL%\libcrypto-1_1-x64.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%OPENSSL_DLL%\libssl-1_1-x64.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM ZLIB

COPY "%ZLIB_LIB%\zlibwapi.dll" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%ZLIB_LIB%\Release\lib\zlib.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM ===========================================================================
REM Copy the build files
REM ===========================================================================

CALL "%AK_TOOL_ROOT%\updateDeployment.bat"

ECHO [Paths] > "%AK_TOOL_DEPLOYMENT_DIR%\qt.conf"
ECHO Plugins = .\\plugins >> "%AK_TOOL_DEPLOYMENT_DIR%\qt.conf"

