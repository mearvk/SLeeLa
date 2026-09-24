[CmdletBinding()]
param([string]$Cxx = "g++")
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$Src = Join-Path $Root "native"
$Out = Join-Path $Root "build\windows"
$Client = Join-Path $Out "skya.exe"
$Server = Join-Path $Out "skya-server.exe"
if (-not (Get-Command $Cxx -ErrorAction SilentlyContinue)) { throw "C++ compiler not found: $Cxx" }
New-Item -ItemType Directory -Force -Path $Out | Out-Null
& $Cxx "-std=c++17" "-O2" "-Wall" "-Wextra" "-Wpedantic" (Join-Path $Src "main.cpp") (Join-Path $Src "skya_engine.cpp") "-o" $Client
if ($LASTEXITCODE -ne 0) { throw "Skya build failed." }
& $Cxx "-std=c++17" "-O2" "-Wall" "-Wextra" "-Wpedantic" (Join-Path $Src "skya_server_main.cpp") (Join-Path $Src "skya_engine.cpp") "-o" $Server
if ($LASTEXITCODE -ne 0) { throw "Skya Server build failed." }
Write-Host "Built: $Client"
Write-Host "Built: $Server"
