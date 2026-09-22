param([string]$Prefix = "$env:ProgramFiles\SLeeLa")
$ErrorActionPreference = "Stop"
$Dest = Join-Path $Prefix "server-edition\moral\2"
if (Test-Path $Dest) { Remove-Item $Dest -Recurse -Force }
Write-Host "Removed SLeeLa Server Edition Service 2 from $Dest"
