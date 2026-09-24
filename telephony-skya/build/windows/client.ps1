[CmdletBinding()]
param()
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
& (Join-Path $Root "build\windows\firewall-check.ps1")
& (Join-Path $Root "build\windows\build.ps1")
$env:SKYA_SLEEELA_CIRCUIT = Join-Path $Root "sleela\SkyaClient.sleela"
Set-Location (Join-Path $Root "javafx")
mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaClientApp javafx:run
if ($LASTEXITCODE -ne 0) { throw "Skya Audio/Video/File client GUI failed." }
