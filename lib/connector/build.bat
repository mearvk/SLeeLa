@echo off
setlocal
cd /d "%~dp0"
where mingw32-make >nul 2>nul
if %errorlevel%==0 (
  set "MAKE_CMD=mingw32-make"
) else (
  where make >nul 2>nul
  if errorlevel 1 (
    echo Error: Install GNU Make through MSYS2/MinGW and ensure it is on PATH.
    exit /b 127
  )
  set "MAKE_CMD=make"
)
set "TARGET=%~1"
if "%TARGET%"=="" set "TARGET=all"
if not "%TARGET%"=="all" if not "%TARGET%"=="database" if not "%TARGET%"=="data" if not "%TARGET%"=="registry" if not "%TARGET%"=="test" if not "%TARGET%"=="clean" if not "%TARGET%"=="help" if not "%TARGET%"=="check-deps" (
  echo Usage: build.bat [all^|database^|data^|registry^|test^|clean^|help^|check-deps]
  exit /b 2
)
%MAKE_CMD% %TARGET%
exit /b %errorlevel%
