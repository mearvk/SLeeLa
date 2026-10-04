<#
.SYNOPSIS
  Stop the SLeeLa side started by sleela-up.ps1 (Windows).
#>
[CmdletBinding()]
param()
$ErrorActionPreference = "SilentlyContinue"
$Root = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$PidFile = Join-Path $Root "build\windows\.skya-sleela.pids"
if (-not (Test-Path $PidFile)) { Write-Host "sleela-down: nothing to stop ($PidFile missing)"; exit 0 }
Get-Content $PidFile | Where-Object { $_ -match '^\d+$' } | ForEach-Object {
    Stop-Process -Id ([int]$_) -Force -ErrorAction SilentlyContinue
}
Remove-Item $PidFile -ErrorAction SilentlyContinue
Write-Host "Skya SLeeLa side stopped."
