<#
.SYNOPSIS
    Initial Windows 7+ launcher for the SLeeLa builder/installer (Windows 7/8/8.1).

.DESCRIPTION
    Same purpose as slbuild-windows10.ps1 — compile the native slbuild binary
    with MinGW-w64 and run it to build + install — but written for the older
    Windows 7 / 8 / 8.1 era:

      * Avoids PowerShell 3.0+ only idioms so it runs under the Windows 7-stock
        Windows PowerShell 2.0 (and newer). No `&&`, no `$PSScriptRoot`-only
        assumptions (it is computed with a 2.0-safe fallback).
      * Enables TLS 1.2 defensively for any download helpers a user adds, since
        Windows 7's stock .NET defaults to SSL3/TLS1.0.
      * A legacy MinGW-w64 (e.g. the SjLj/Dwarf GCC 8.x builds) is fine; it only
        needs C11. Still run it from an MSYS2 / Git-Bash shell so slbuild can
        drive `make` and a POSIX `sh`.

    For Windows 10/11 prefer slbuild-windows10.ps1; for Windows XP / Server 2003
    use slbuild-windowsxp.cmd.

.PARAMETER Prefix
    Install root passed to slbuild (default: %USERPROFILE%\.local\sleela).

.PARAMETER Cc
    C compiler used to compile slbuild.exe (default: gcc).

.PARAMETER ProduceOnly
    Compile slbuild.exe, print its path, and stop. Do NOT run the build/install.
    Also honored via the SLBUILD_PRODUCE_ONLY=1 environment variable.

.PARAMETER BuildOnly
    Build the core products only; do not stage/install.

.PARAMETER DryRun
    Print slbuild's plan only.

.PARAMETER Plain
    Disable color/box glyphs.

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File slbuild-windows7.ps1
    powershell -ExecutionPolicy Bypass -File slbuild-windows7.ps1 -ProduceOnly
#>
[CmdletBinding()]
param(
    [string]$Prefix = "",
    [string]$Cc = "gcc",
    [switch]$ProduceOnly,
    [switch]$BuildOnly,
    [switch]$DryRun,
    [switch]$Plain
)
$ErrorActionPreference = "Stop"

# PowerShell 2.0-safe script directory resolution.
if ($PSScriptRoot) { $Here = $PSScriptRoot }
else { $Here = Split-Path -Parent $MyInvocation.MyCommand.Path }

$NB = Split-Path -Parent $Here             # build\native-builder
$Src = Join-Path $NB "slbuild.c"
$OutDir = Join-Path $NB "bin\windows"
$Bin = Join-Path $OutDir "slbuild.exe"

Write-Host "== SLeeLa Windows 7+ launcher (Windows 7 / 8 / 8.1) =="

# Windows 7's stock .NET negotiates SSL3/TLS1.0; enable TLS 1.2 defensively so
# any user-added download helpers reach modern endpoints.
try {
    [System.Net.ServicePointManager]::SecurityProtocol = `
        [System.Net.ServicePointManager]::SecurityProtocol -bor 3072  # Tls12
} catch { }

function Need($name, $hint) {
    if (-not (Get-Command $name -ErrorAction SilentlyContinue)) {
        throw ("Required tool '" + $name + "' not found on PATH. " + $hint)
    }
}
Need $Cc  "Install MinGW-w64 (a GCC 8.x+ build is fine) and add it to PATH."
Need "make" "Install GNU make or use mingw32-make."
Need "sh"   "Run from an MSYS2 / Git-Bash shell so slbuild can invoke a POSIX shell."
if (-not (Get-Command "python3" -ErrorAction SilentlyContinue) -and `
    -not (Get-Command "python" -ErrorAction SilentlyContinue)) {
    Write-Warning "python not found; the SHA-256 verify gate needs Python 3."
}
if (-not (Test-Path $Src)) { throw ("slbuild.c not found at " + $Src) }

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

Write-Host ("== compiling slbuild.exe with " + $Cc + " (MinGW-w64) ==")
& $Cc -std=c11 -O2 -Wall -Wextra -Wno-format-truncation -Wno-stringop-truncation $Src -o $Bin
if ($LASTEXITCODE -ne 0) { throw "slbuild.exe compile failed." }
if (-not (Test-Path $Bin)) { throw ("slbuild.exe was not produced at " + $Bin) }
Write-Host ("== built " + $Bin + " ==")

if ($ProduceOnly -or $env:SLBUILD_PRODUCE_ONLY -eq "1") {
    Write-Host "== SLeeLa Windows 7+ launcher: builder/installer binary produced =="
    Write-Output $Bin
    return
}

$slArgs = @()
if ($Prefix -ne "") { $slArgs += @("--prefix", $Prefix) }
if ($BuildOnly)     { $slArgs += "--build-only" }
if ($DryRun)        { $slArgs += "--dry-run" }
if ($Plain)         { $slArgs += "--plain" }

Write-Host "== running the build + install =="
& $Bin @slArgs
if ($LASTEXITCODE -ne 0) { throw ("slbuild reported a failure (exit " + $LASTEXITCODE + ").") }
