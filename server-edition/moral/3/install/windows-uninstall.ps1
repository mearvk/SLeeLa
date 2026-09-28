param([string]$Prefix = "$env:ProgramFiles\SLeeLa")
$ErrorActionPreference = "Stop"
$Dest = Join-Path $Prefix "server-edition\moral\3"
if (Test-Path $Dest) { Remove-Item $Dest -Recurse -Force }
Write-Host "Removed SLeeLa Server Edition Service 3 from $Dest"
