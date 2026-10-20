@echo off
rem Run all five GUI examples from Windows CMD.
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
for %%P in (example\mual\gui-mual-text-editor.sleela example\gual\gui-gual-settings-console.sleela example\thual\gui-thual-effects-showcase.sleela example\dual\gui-dual-media-studio.sleela example\military\gui-military-command-console.sleela) do (
  echo ======================================================================
  echo Running %%P
  "%SLEELA%" run "%%P"
  if errorlevel 1 exit /b %ERRORLEVEL%
)
echo All five GUI examples completed.
