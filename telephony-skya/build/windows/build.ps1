[CmdletBinding()]
param([string]$Cxx = "g++")
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$Src = Join-Path $Root "telephony-skya\native"
$Out = Join-Path $Root "telephony-skya\build\windows"
$Exe = Join-Path $Out "skya.exe"
if (-not (Get-Command $Cxx -ErrorAction SilentlyContinue)) { throw "C++ compiler not found: $Cxx" }
New-Item -ItemType Directory -Force -Path $Out | Out-Null
& $Cxx "-std=c++17" "-O2" "-Wall" "-Wextra" "-Wpedantic" (Join-Path $Src "main.cpp") (Join-Path $Src "skya_engine.cpp") "-o" $Exe
if ($LASTEXITCODE -ne 0) { throw "Skya Windows build failed." }
Write-Host "Built: $Exe"
