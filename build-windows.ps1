<#
.SYNOPSIS
    Build SLeeLa for Windows 10+ (MinGW-w64 / GCC) and optionally install.

.DESCRIPTION
    The SLeeLa C/C++ core is OS-neutral: threads, sockets, files, pipes, paths,
    terminals, dynamic libraries and time all go through the OS-aware layer in
    impl/core (sleela_thread/net/io/path/terminal/library/time), which has a
    complete Win32 backend (Win32 threads/CONDITION_VARIABLE, Winsock2,
    CreateFile/CreatePipe/named pipes, LoadLibrary, ConPTY,
    QueryPerformanceCounter). This script drives the existing GNU Makefile with
    a MinGW toolchain and links Winsock (ws2_32).

.NOTES
    Requirements:
      * MinGW-w64 GCC/G++ (C11 + C++17), e.g. via MSYS2 (pacman -S
        mingw-w64-ucrt-x86_64-gcc) or choco install mingw.
      * GNU make (mingw32-make or make).
      * Python 3 on PATH (the SHA-256 verify gate runs it as 'py -3' or
        'python').
    MSVC is not supported by this script: the pthread compatibility shim
    (impl/core/pthread.h) relies on GCC's #include_next, a GNU extension.

.PARAMETER BinDir
    Optional install directory. If set, the built .exe files are copied there.

.EXAMPLE
    powershell -ExecutionPolicy Bypass -File build-windows.ps1
    powershell -ExecutionPolicy Bypass -File build-windows.ps1 -BinDir C:\Tools\sleela
#>
[CmdletBinding()]
param(
    [string]$BinDir = "",
    [string]$Cc = "gcc",
    [string]$Cxx = "g++"
)

$ErrorActionPreference = "Stop"
$RepoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$ImplDir = Join-Path $RepoRoot "impl"
$BuildDir = Join-Path $ImplDir "build"

Write-Host "=========================================="
Write-Host "SLeeLa Windows Build (MinGW)"
Write-Host "=========================================="
Write-Host "C Compiler:   $Cc"
Write-Host "C++ Compiler: $Cxx"
Write-Host "Repo root:    $RepoRoot"
Write-Host "Build dir:    $BuildDir"
Write-Host ""

function Require-Command($name) {
    if (-not (Get-Command $name -ErrorAction SilentlyContinue)) {
        throw "Required tool '$name' was not found on PATH. See the notes in this script's header."
    }
}

# Resolve a GNU make program (MSYS2 ships 'make'; standalone MinGW ships
# 'mingw32-make').
$MakeProg = $null
foreach ($candidate in @("mingw32-make", "make")) {
    if (Get-Command $candidate -ErrorAction SilentlyContinue) { $MakeProg = $candidate; break }
}
if (-not $MakeProg) { throw "Neither 'mingw32-make' nor 'make' was found on PATH." }

Require-Command $Cc
Require-Command $Cxx

# The verify-security gate needs Python. Prefer the 'py' launcher, else 'python'.
$Py = $null
foreach ($candidate in @("py", "python", "python3")) {
    if (Get-Command $candidate -ErrorAction SilentlyContinue) { $Py = $candidate; break }
}
if (-not $Py) { throw "Python 3 was not found on PATH (needed for the SHA-256 verify gate)." }

# The Makefile's verify gate requires a trusted manifest. Default to the
# committed one unless the caller already set it.
if (-not $env:SLEELA_SHA256_MANIFEST) {
    $env:SLEELA_SHA256_MANIFEST = Join-Path $RepoRoot "security\sha256-manifest.json"
}
$env:PYTHON = "$Py -3"
if ($Py -eq "py") {
    $env:PYTHON = "py -3"
} elseif ($Py -eq "python" -or $Py -eq "python3") {
    $env:PYTHON = $Py
}
Write-Host "Using Python launcher: $($env:PYTHON)"
Write-Host "Using SHA-256 manifest: $($env:SLEELA_SHA256_MANIFEST)"
Write-Host ""

Write-Host "Building Sleela and Nordshrift..."
Push-Location $ImplDir
try {
    & $MakeProg clean
    # GNU Make on Windows sets OS=Windows_NT, so the Makefile auto-selects the
    # Windows flags/libs (ws2_32, .exe suffix, Win32 threads).
    & $MakeProg CC=$Cc CXX=$Cxx all
    if ($LASTEXITCODE -ne 0) { throw "make failed with exit code $LASTEXITCODE" }
}
finally {
    Pop-Location
}

$SleelaExe = Join-Path $BuildDir "sleela.exe"
$NordshriftExe = Join-Path $BuildDir "nordshrift.exe"
if (-not (Test-Path $SleelaExe) -or -not (Test-Path $NordshriftExe)) {
    throw "Build did not produce sleela.exe and nordshrift.exe in $BuildDir."
}
$StageDir = Join-Path $BuildDir "SLeeLa"
$StageConfig = Join-Path $StageDir "Config"
$StageOptions = Join-Path $StageDir "Options"
$StageConfigFile = Join-Path $StageConfig "sleela.conf"
if (-not (Test-Path $StageConfigFile) -or -not (Test-Path $StageOptions)) {
    throw "Build staging is incomplete: expected $StageConfigFile and $StageOptions."
}

Write-Host ""
Write-Host "Build completed successfully!"
Get-Item $SleelaExe, $NordshriftExe | Format-Table Length, FullName -AutoSize

if ($BinDir -ne "") {
    if (-not (Test-Path $BinDir)) { New-Item -ItemType Directory -Path $BinDir | Out-Null }
    Copy-Item $SleelaExe (Join-Path $BinDir "sleela.exe") -Force
    Copy-Item $NordshriftExe (Join-Path $BinDir "nordshrift.exe") -Force
    Write-Host "Installed to $BinDir"
    Write-Host "Verify: $(Join-Path $BinDir 'sleela.exe') version"
} else {
    Write-Host "Binaries remain in: $BuildDir"
    Write-Host "Staged runtime: $StageDir"
    Write-Host "Consolidated config: $StageConfigFile"
    Write-Host "Verify: $SleelaExe version"
}
Write-Host ""
Write-Host ""
Write-Host "Common configuration:"
Write-Host "  sleela.exe run with Config\\sleela.conf as the staged default runtime configuration"
Write-Host "  SLEELA_CONFIG_FILE may override the selected configuration path."
Write-Host "  Windows target: 10+; source backend uses Win32/ConPTY/Winsock."
Write-Host ""
Write-Host "Build process finished."
