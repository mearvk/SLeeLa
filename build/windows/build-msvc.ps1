[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")][string]$Configuration = "Release",
    [string]$Generator = "Visual Studio 17 2022",
    [ValidateSet("x64", "Win32", "ARM64")][string]$Architecture = "x64",
    [switch]$Test
)

$ErrorActionPreference = "Stop"
$Root = (Resolve-Path (Join-Path $PSScriptRoot "../..")).Path
$Source = Join-Path $PSScriptRoot "."
$Build = Join-Path $Root "build\msvc-platform"

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "CMake was not found. Install CMake and Visual Studio 2022 with the Desktop development with C++ workload."
}
if (-not (Get-Command ctest -ErrorAction SilentlyContinue)) {
    throw "CTest was not found. Install CMake with the Visual Studio C++ workload."
}

Write-Host "Configuring SLeeLa's existing OS abstraction sources with MSVC..."
cmake -S $Source -B $Build -G $Generator -A $Architecture
if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed ($LASTEXITCODE)" }

cmake --build $Build --config $Configuration --parallel
if ($LASTEXITCODE -ne 0) { throw "MSVC build failed ($LASTEXITCODE)" }

if ($Test) {
    ctest --test-dir $Build -C $Configuration --output-on-failure
    if ($LASTEXITCODE -ne 0) { throw "MSVC smoke tests failed ($LASTEXITCODE)" }
}

Write-Host "MSVC platform abstraction build completed. This does not yet build the complete SLeeLa runtime."
