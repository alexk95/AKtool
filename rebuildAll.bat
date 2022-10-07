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

REM Setup eviroment ########################################################################
CALL "%AK_TOOL_ROOT%\setupEnvironment.bat"

REM Clean up build logs ####################################################################

if "%3"=="SKIP" (
	ECHO Skipping build log cleanup
	goto BUILD_START
)

DEL buildLog_Debug.txt
DEL buildLog_Release.txt
DEL buildLog_Summary.txt

REM BUILD ##################################################################################
:BUILD_START

if "%2"=="" (
	SET ARG_2=REBUILD
)

if "%2"=="REBUILD" (
	SET ARG_2=REBUILD
)

if "%2"=="BUILD" (
	SET ARG_2=BUILD
)

ECHO ===============================================================
ECHO Build Library: uiCore
ECHO ===============================================================
CALL "%UI_CORE_ROOT%\rebuild.bat" BOTH %ARG_2% SKIP

CALL "%ACI_ROOT%\rebuildAll.bat" BOTH %ARG_2% SKIP

ECHO ===============================================================
ECHO Build Library: QWT Wrapper
ECHO ===============================================================
CALL "%QWT_WRAPPER%\build.bat" BOTH %ARG_2% SKIP

ECHO ===============================================================
ECHO Build Executable: AKtool
ECHO ===============================================================
CALL "%AK_TOOL_ROOT%\build.bat" BOTH %ARG_2% SKIP

REM Create build summary ###################################################################

if "%3"=="SKIP" (
	ECHO Skipping build log summary
	goto END
)

if "%2"=="" (
	SET searchString="Rebuild All:"
) ELSE IF "%2"=="REBUILD" (
	SET searchString="Rebuild All:"
) ELSE (
	SET searchString="Build:"
)

FIND %searchString% "%AK_TOOL_ROOT%\buildLog_Debug.txt" "%AK_TOOL_ROOT%\buildLog_Release.txt" > "%AK_TOOL_ROOT%\buildLog_Summary.txt"

ECHO DONE

:END