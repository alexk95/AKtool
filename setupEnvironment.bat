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

REM Set CURL Directory
SET CURL_ROOT=%UI_CORE_ROOT%\Third_Party_Libraries\curl\curl-7.62.0
SET CURL_LIB=%CURL_ROOT%\build-win-x64\Release\lib
SET CURL_INC=%CURL_ROOT%\build-win-x64\Release\include
SET CURL_DLL=%CURL_ROOT%\build-win-x64\Release\bin
SET CURL_LIBD=%CURL_ROOT%\build-win-x64\Debug\lib
SET CURL_DLLD=%CURL_ROOT%\build-win-x64\Debug\bin

REM Set ZLIB Directory
SET ZLIB_ROOT=%UI_CORE_ROOT%\Third_Party_Libraries\zlib\zlib-1.2.11\x64
SET ZLIB_LIB=%UI_CORE_ROOT%\Third_Party_Libraries\zlib\zlib-1.2.11\x64

REM Setup external eviroment
CALL "%UI_CORE_ROOT%\setupEnvironment.bat"
CALL "%ACI_ROOT%\setupEnvironment.bat"
CALL "%QWT_WRAPPER_ROOT%\setupEnvironment.bat"

:END