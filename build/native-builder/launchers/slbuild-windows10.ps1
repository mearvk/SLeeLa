<#
.SYNOPSIS
    Initial Windows 10+ launcher for the SLeeLa builder/installer.

.DESCRIPTION
    Compiles the native slbuild binary with a modern MinGW-w64 toolchain
    (UCRT, e.g. MSYS2 `mingw-w64-ucrt-x86_64-gcc`) and then runs it to build the
    SLeeLa core products and install the main package.

    slbuild.c is portable C that drives the SLeeLa build through `make` and a
    POSIX-style shell, so it must run inside an MSYS2 / Git-Bash environment
    where `sh`, `make`, and the MinGW gcc are on PATH. This launcher verifies
    that environment, compiles slbuild to bin\windows\slbuild.exe, and runs it.

    Targets Windows 10 and 11 (x64 / ARM64). For older Windows see
    slbuild-windows7.ps1 (Windows 7/8/8.1) and slbuild-windowsxp.cmd
    (Windows XP / Server 2003).

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
    Print slbuild's plan only; do not build or install.

.PARAMETER Plain
    Disable color/box glyphs (also honored via NO_COLOR / non-TTY).

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File slbuild-windows10.ps1
    powershell -ExecutionPolicy Bypass -File slbuild-windows10.ps1 -Prefix C:\Tools\sleela
    powershell -ExecutionPolicy Bypass -File slbuild-windows10.ps1 -ProduceOnly
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

$Here = $PSScriptRoot
$NB   = Split-Path -Parent $Here            # build\native-builder
$Src  = Join-Path $NB "slbuild.c"
$OutDir = Join-Path $NB "bin\windows"
$Bin  = Join-Path $OutDir "slbuild.exe"

Write-Host "== SLeeLa Windows 10+ launcher =="

# Require a POSIX shell + make (MSYS2 / Git-Bash): slbuild drives `make` and sh.
function Need($name, $hint) {
    if (-not (Get-Command $name -ErrorAction SilentlyContinue)) {
        throw "Required tool '$name' not found on PATH. $hint"
    }
}
Need $Cc  "Install MinGW-w64 (MSYS2: pacman -S mingw-w64-ucrt-x86_64-gcc) and add it to PATH."
Need "make" "Install GNU make (MSYS2: pacman -S make) or use mingw32-make."
Need "sh"   "Run from an MSYS2 / Git-Bash shell so slbuild can invoke a POSIX shell."
if (-not (Get-Command "python3" -ErrorAction SilentlyContinue) -and `
    -not (Get-Command "python" -ErrorAction SilentlyContinue)) {
    Write-Warning "python not found; the SHA-256 verify gate needs Python 3."
}
if (-not (Test-Path $Src)) { throw "slbuild.c not found at $Src." }

New-Item -ItemType Directory -Force -Path $OutDir | Out-Null

Write-Host "== compiling slbuild.exe with $Cc (MinGW-w64, UCRT) =="
& $Cc -std=c11 -O2 -Wall -Wextra -Wno-format-truncation -Wno-stringop-truncation `
      $Src -o $Bin
if ($LASTEXITCODE -ne 0) { throw "slbuild.exe compile failed." }
if (-not (Test-Path $Bin)) { throw "slbuild.exe was not produced at $Bin." }
Write-Host "== built $Bin =="

if ($ProduceOnly -or $env:SLBUILD_PRODUCE_ONLY -eq "1") {
    Write-Host "== SLeeLa Windows 10+ launcher: builder/installer binary produced =="
    Write-Output $Bin
    return
}

# Assemble slbuild arguments.
$slArgs = @()
if ($Prefix -ne "") { $slArgs += @("--prefix", $Prefix) }
if ($BuildOnly)     { $slArgs += "--build-only" }
if ($DryRun)        { $slArgs += "--dry-run" }
if ($Plain)         { $slArgs += "--plain" }

Write-Host "== running the build + install =="
& $Bin @slArgs
if ($LASTEXITCODE -ne 0) { throw "slbuild reported a failure (exit $LASTEXITCODE)." }
