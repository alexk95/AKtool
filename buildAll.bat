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

if "%1"=="" (
	DEL buildLog_Debug.txt
	DEL buildLog_Release.txt
)
	
if "%1"=="BOTH" (
	DEL buildLog_Debug.txt
	DEL buildLog_Release.txt
)
	
if "%1"=="RELEASE" (
	DEL buildLog_Release.txt
)
	
if "%1"=="DEBUG" (
	DEL buildLog_Debug.txt
)
DEL buildLog_Summary.txt

ECHO Build UI core
CALL "%UI_CORE_ROOT%\rebuild.bat" %1 %2

ECHO Build ACI
CALL "%ACI_ROOT%\rebuildAll.bat" %1 %2

ECHO Build QWT Wrapper
CALL "%QWT_WRAPPER%\build.bat" %1 %2

ECHO Build Application
CALL "%AK_TOOL_ROOT%\build.bat" %1 %2

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