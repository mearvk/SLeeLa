<#
.SYNOPSIS
  Bring up the SLeeLa side of Skya for the GUI (Windows).
.DESCRIPTION
  Starts the upstream Skya server (SkyaServer.sleela, SKYA/1 on :8443) and the
  Guia control agent (SkyaClient.sleela, Guia/1 on :8700) in the background, so
  the JavaFX GUI has a live SLeeLa client to talk to.
  Writes PIDs to build\windows\.skya-sleela.pids; stop with sleela-down.ps1.
  Honors $env:SLEELA_COMMAND (default: sleela) as the .sleela runner.
#>
[CmdletBinding()]
param()
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$PidFile = Join-Path $Root "build\windows\.skya-sleela.pids"
$Runner = if ($env:SLEELA_COMMAND) { $env:SLEELA_COMMAND } else { "sleela" }

if (-not (Get-Command $Runner -ErrorAction SilentlyContinue)) {
    Write-Error "sleela-up: SLeeLa runner '$Runner' not found on PATH. Build the runtime (make -C impl) and put it on PATH, or set SLEELA_COMMAND."
    exit 127
}

Set-Content -Path $PidFile -Value $null
$server = Start-Process -FilePath $Runner -ArgumentList @("run", (Join-Path $Root "sleela\SkyaServer.sleela")) `
    -RedirectStandardOutput (Join-Path $Root "build\windows\skya-server.log") -RedirectStandardError (Join-Path $Root "build\windows\skya-server.err.log") -PassThru -WindowStyle Hidden
Add-Content -Path $PidFile -Value $server.Id
Start-Sleep -Seconds 1
$client = Start-Process -FilePath $Runner -ArgumentList @("run", (Join-Path $Root "sleela\SkyaClient.sleela")) `
    -RedirectStandardOutput (Join-Path $Root "build\windows\skya-client.log") -RedirectStandardError (Join-Path $Root "build\windows\skya-client.err.log") -PassThru -WindowStyle Hidden
Add-Content -Path $PidFile -Value $client.Id
Write-Host "Skya SLeeLa side up: server (:8443) + Guia agent (:8700). PIDs in $PidFile"
