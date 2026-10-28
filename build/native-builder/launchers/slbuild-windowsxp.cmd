@echo off
rem ============================================================================
rem slbuild-windowsxp.cmd - initial Windows XP+ launcher for the SLeeLa
rem builder/installer (Windows XP / Server 2003 and newer).
rem Max Rupplin - MEARVK LLC - 2026
rem
rem Windows XP shipped without PowerShell and with a limited cmd.exe, so this
rem launcher is a classic batch file. It compiles the native slbuild binary with
rem an XP-capable MinGW (a GCC 3.4.5 / 4.x "mingw32" build still targets XP) and
rem then runs it to build + install.
rem
rem slbuild.c drives the SLeeLa build through 'make' and a POSIX 'sh', so run
rem this from an MSYS / MinGW shell environment where gcc, make and sh are on
rem PATH (classic MSYS 1.0 works on XP).
rem
rem Usage:
rem   slbuild-windowsxp.cmd [--produce-only] [--prefix DIR] [--build-only]
rem                         [--dry-run] [--plain]
rem
rem   --produce-only   compile slbuild.exe, print its path, and stop (do NOT run
rem                    the build/install). Also honored via SLBUILD_PRODUCE_ONLY=1.
rem
rem For Windows 7/8/8.1 use slbuild-windows7.ps1; for Windows 10/11 use
rem slbuild-windows10.ps1.
rem ============================================================================
setlocal enabledelayedexpansion

rem --- locate this script's directory and the native-builder dir -------------
set "HERE=%~dp0"
rem HERE ends with a backslash; NB is its parent (build\native-builder).
for %%I in ("%HERE%..") do set "NB=%%~fI"
set "SRC=%NB%\slbuild.c"
set "OUTDIR=%NB%\bin\windows"
set "BIN=%OUTDIR%\slbuild.exe"

set "CC=gcc"
if not "%SLBUILD_CC%"=="" set "CC=%SLBUILD_CC%"

rem --- parse --produce-only out of the arguments; collect the rest in SLARGS --
set "PRODUCE_ONLY=0"
if "%SLBUILD_PRODUCE_ONLY%"=="1" set "PRODUCE_ONLY=1"
set "SLARGS="
for %%A in (%*) do (
  if /I "%%~A"=="--produce-only" (
    set "PRODUCE_ONLY=1"
  ) else (
    set "SLARGS=!SLARGS! %%A"
  )
)

echo == SLeeLa Windows XP+ launcher ==

rem --- toolchain checks (cmd.exe-era 'where' may be absent on XP; probe via the
rem     compiler's own version string instead) -----------------------------
%CC% --version >nul 2>&1
if errorlevel 1 (
  echo ERROR: C compiler "%CC%" not found on PATH.
  echo        Install an XP-capable MinGW ^(mingw32 GCC 3.4.5/4.x^) and add it to PATH,
  echo        or set SLBUILD_CC to your compiler.
  exit /b 1
)
make --version >nul 2>&1
if errorlevel 1 (
  echo ERROR: GNU make not found on PATH ^(classic MSYS 1.0 provides it^).
  exit /b 1
)
sh --version >nul 2>&1
if errorlevel 1 (
  echo WARNING: POSIX 'sh' not found; slbuild drives the build through a shell.
  echo          Run this from an MSYS / MinGW shell environment.
)

if not exist "%SRC%" (
  echo ERROR: slbuild.c not found at "%SRC%".
  exit /b 1
)

if not exist "%OUTDIR%" mkdir "%OUTDIR%"

echo == compiling slbuild.exe with %CC% ==
rem -Wno-* flags are ignored by compilers that do not know them; XP-era GCC is OK.
%CC% -std=c11 -O2 -Wall -Wextra -Wno-format-truncation -Wno-stringop-truncation "%SRC%" -o "%BIN%"
if errorlevel 1 (
  echo ERROR: slbuild.exe compile failed.
  exit /b 1
)
if not exist "%BIN%" (
  echo ERROR: slbuild.exe was not produced at "%BIN%".
  exit /b 1
)
echo == built %BIN% ==

if "%PRODUCE_ONLY%"=="1" (
  echo == SLeeLa Windows XP+ launcher: builder/installer binary produced ==
  echo %BIN%
  endlocal
  exit /b 0
)

echo == running the build + install ==
"%BIN%" !SLARGS!
set "RC=%ERRORLEVEL%"
if not "%RC%"=="0" (
  echo ERROR: slbuild reported a failure ^(exit %RC%^).
  exit /b %RC%
)

endlocal
