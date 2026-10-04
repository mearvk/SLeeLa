[CmdletBinding()]
param()
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$env:SKYA_SLEEELA_CIRCUIT = Join-Path $Root "sleela\SkyaClient.sleela"

# The admin monitor is a Guia client of the running SLeeLa agent; bring the
# SLeeLa side up (server + Guia agent on :8700) and tear it down on exit.
$sleelaUp = $false
try { & (Join-Path $Root "build\windows\sleela-up.ps1"); $sleelaUp = $true }
catch { Write-Warning "client_monitor.ps1: continuing without the SLeeLa side (monitor will show agent offline)." }

try {
    Set-Location (Join-Path $Root "javafx")
    mvn -q -Dskya.mainClass=com.mearvk.sleela.skya.SkyaApp javafx:run
    if ($LASTEXITCODE -ne 0) { throw "Skya Client Monitor GUI failed." }
}
finally {
    if ($sleelaUp) { & (Join-Path $Root "build\windows\sleela-down.ps1") 2>$null }
}
