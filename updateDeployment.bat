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
CALL "%ACI_ROOT%\updateDefaultScripts.bat"

IF "%AK_TOOL_DEPLOYMENT_DIR%" == "" (
    SET AK_TOOL_DEPLOYMENT_DIR="%AK_TOOL_ROOT%\Deployment"
)

REM Clean files
DEL "%AK_TOOL_DEPLOYMENT_DIR%\uiCore.dll"
DEL "%AK_TOOL_DEPLOYMENT_DIR%\aciCore.dll"
DEL "%AK_TOOL_DEPLOYMENT_DIR%\QwtWrapper.dll"
DEL "%AK_TOOL_DEPLOYMENT_DIR%\AKtool.exe"
DEL "%AK_TOOL_DEPLOYMENT_DIR%\AKtool.ico"

REM UI Core
COPY "%UI_CORE_ROOT%\x64\Release\uiCore.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM ACI Core
COPY "%ACI_CORE%\x64\Release\aciCore.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM QWT Wrapper
COPY "%QWT_WRAPPER%\x64\Release\QwtWrapper.dll" "%AK_TOOL_DEPLOYMENT_DIR%"

REM AKtool
COPY "%AK_TOOL_ROOT%\x64\Release\AKtool.exe" "%AK_TOOL_DEPLOYMENT_DIR%"
COPY "%AK_TOOL_ROOT%\x64\Release\AKtool.ico" "%AK_TOOL_DEPLOYMENT_DIR%"