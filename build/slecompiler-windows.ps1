[CmdletBinding()]
param([string]$CMake = "cmake")
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Src = Join-Path $Root "decompiler"
$Out = Join-Path $PSScriptRoot "slecompiler\windows"
$Build = Join-Path $Out "cmake"
if (-not (Get-Command $CMake -ErrorAction SilentlyContinue)) { throw "CMake not found." }
New-Item -ItemType Directory -Force -Path $Out | Out-Null
& $CMake -S $Src -B $Build -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { throw "Slecompiler CMake configure failed." }
& $CMake --build $Build --config Release
if ($LASTEXITCODE -ne 0) { throw "Slecompiler Windows build failed." }
Write-Host "Slecompiler Windows build: $Build"
