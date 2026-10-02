@echo off
setlocal
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0quick-safe-install.ps1" %*
exit /b %ERRORLEVEL%
