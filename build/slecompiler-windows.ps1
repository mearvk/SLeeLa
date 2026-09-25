[CmdletBinding()]
param(
    [string]$CMake = "cmake",
    [string]$Generator = "",
    [ValidateSet("x64","Win32","ARM64")]
    [string]$Architecture = "x64",
    [ValidateSet("Release","Debug","RelWithDebInfo")]
    [string]$Configuration = "Release"
)
$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$Src = Join-Path $Root "decompiler"
$Out = Join-Path $PSScriptRoot "slecompiler\windows"
$Build = Join-Path $Out ("cmake-" + $Architecture.ToLowerInvariant())

if (-not (Get-Command $CMake -ErrorAction SilentlyContinue)) {
    throw "CMake not found. Install CMake 3.20+ and ensure it is on PATH."
}
if (-not (Test-Path (Join-Path $Src "CMakeLists.txt"))) {
    throw "Slecompiler CMake project not found at $Src."
}

New-Item -ItemType Directory -Force -Path $Out | Out-Null

$configureArgs = @("-S", $Src, "-B", $Build, "-DCMAKE_BUILD_TYPE=$Configuration")
if ($Generator -ne "") {
    $configureArgs += @("-G", $Generator)
    if ($Generator -match "Visual Studio") {
        $configureArgs += @("-A", $Architecture)
    }
}

& $CMake @configureArgs
if ($LASTEXITCODE -ne 0) { throw "Slecompiler Windows CMake configure failed." }

& $CMake --build $Build --config $Configuration
if ($LASTEXITCODE -ne 0) { throw "Slecompiler Windows build failed." }

Write-Host "Slecompiler Windows build: $Build"
Write-Host "Architecture: $Architecture"
Write-Host "Configuration: $Configuration"
if ($Generator -ne "") { Write-Host "Generator: $Generator" }
