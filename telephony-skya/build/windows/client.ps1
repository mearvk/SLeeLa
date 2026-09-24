[CmdletBinding()]
param()
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
& (Join-Path $Root "build\windows\build.ps1")
$NativeClient = Join-Path $Root "build\windows\skya.exe"
if (-not (Test-Path $NativeClient)) { throw "Skya native client was not built." }
& $NativeClient --client
