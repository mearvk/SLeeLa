[CmdletBinding()]
param()
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
Set-Location (Join-Path $Root "javafx")
mvn -q -DmainClass=com.mearvk.sleela.skya.SkyaConnectApp javafx:run
if ($LASTEXITCODE -ne 0) { throw "Skya Remote Connection GUI failed." }
