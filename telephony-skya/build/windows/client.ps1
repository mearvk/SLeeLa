[CmdletBinding()]
param()
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
& (Join-Path $Root "build\windows\firewall-check.ps1")
$env:SKYA_SLEEELA_CIRCUIT = Join-Path $Root "sleela\SkyaClient.sleela"

# The user client flow is GUI -> Guia -> SkyaClient.sleela -> SKYA/1 (SLeeLa
# runtime, not the native C engine). Bring up the SLeeLa side; tear down on exit.
$sleelaUp = $false
try { & (Join-Path $Root "build\windows\sleela-up.ps1"); $sleelaUp = $true }
catch { Write-Warning "client.ps1: continuing without the SLeeLa side (GUI will show CLIENT.OFFLINE)." }

try {
    Set-Location (Join-Path $Root "javafx")
    mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaClientApp javafx:run
    if ($LASTEXITCODE -ne 0) { throw "Skya Audio/Video/File client GUI failed." }
}
finally {
    if ($sleelaUp) { & (Join-Path $Root "build\windows\sleela-down.ps1") 2>$null }
}
