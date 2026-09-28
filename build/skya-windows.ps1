[CmdletBinding()]
param([string]$Cxx = "g++",[string]$Room = "lobby")
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$Src = Join-Path $Root "telephony-skya\native"
$Out = Join-Path $PSScriptRoot "skya\windows"
$Exe = Join-Path $Out "skya.exe"
if (-not (Get-Command $Cxx -ErrorAction SilentlyContinue)) { throw "C++ compiler not found." }
New-Item -ItemType Directory -Force -Path $Out | Out-Null
& $Cxx "-std=c++17" "-O2" "-Wall" "-Wextra" "-Wpedantic" (Join-Path $Src "main.cpp") (Join-Path $Src "skya_engine.cpp") "-o" $Exe
if ($LASTEXITCODE -ne 0) { throw "Skya Windows build failed." }
Write-Host "Skya Windows 10+ binary: $Exe"
& $Exe "--both" "--room" $Room
