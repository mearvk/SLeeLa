@echo off
rem Run all six GUI examples, including the expandable menubar, from Windows CMD.
setlocal
set "ROOT=%~dp0.."
for %%I in ("%ROOT%") do set "ROOT=%%~fI"
cd /d "%ROOT%"
if not "%SLEELA_BIN%"=="" set "SLEELA=%SLEELA_BIN%"
if "%SLEELA%"=="" if exist "%ROOT%\impl\build\sleela.exe" set "SLEELA=%ROOT%\impl\build\sleela.exe"
if "%SLEELA%"=="" if exist "%ROOT%\impl\build\sleela" set "SLEELA=%ROOT%\impl\build\sleela"
if "%SLEELA%"=="" if exist "%ROOT%\bin\sleela.exe" set "SLEELA=%ROOT%\bin\sleela.exe"
if "%SLEELA%"=="" echo SLeeLa executable not found. Build it or set SLEELA_BIN. 1>&2 & exit /b 1
if "%SLEELA_SHEET%"=="" set "SLEELA_SHEET=%ROOT%\SHEET.sheet"
if "%SLEELA_SHA256_MANIFEST%"=="" set "SLEELA_SHA256_MANIFEST=%ROOT%\security\sha256-manifest.json"
call :run example\mual\gui-mual-text-editor.sleela || exit /b 1
call :run example\gual\gui-gual-settings-console.sleela || exit /b 1
call :run example\thual\gui-thual-effects-showcase.sleela || exit /b 1
call :run example\dual\gui-dual-media-studio.sleela || exit /b 1
call :run example\military\gui-military-command-console.sleela || exit /b 1
call :run example\menus\gui-menubar-expandable.sleela || exit /b 1
echo All six GUI examples completed.
exit /b 0
:run
echo ======================================================================
echo Running %~1
"%SLEELA%" run "%~1"
if errorlevel 1 exit /b 1
exit /b 0
