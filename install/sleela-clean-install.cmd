@echo off
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0sleela-clean-install.ps1" %*
set "SLEELA_INSTALL_EXIT=%ERRORLEVEL%"
endlocal & exit /b %SLEELA_INSTALL_EXIT%
