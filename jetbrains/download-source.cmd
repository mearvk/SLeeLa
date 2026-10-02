@ECHO OFF
REM SLeeLa / JetBrains source acquisition launcher for Windows 10+
REM Max Rupplin - MEARVK LLC - 2026
SET "SCRIPT_DIR=%~dp0"
powershell.exe -NoLogo -NoProfile -File "%SCRIPT_DIR%download-source.ps1" %*
IF ERRORLEVEL 1 EXIT /B %ERRORLEVEL%
