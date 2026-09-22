param([string]$Prefix = "$env:ProgramFiles\SLeeLa")
$ErrorActionPreference = "Stop"
$Root = (Resolve-Path (Join-Path $PSScriptRoot "..\..\..\..")).Path
$Dest = Join-Path $Prefix "server-edition\moral\2"
New-Item -ItemType Directory -Force -Path $Dest | Out-Null
Copy-Item (Join-Path $Root "server-edition\moral\2\*") $Dest -Recurse -Force
$cfg = Join-Path $Dest "config\server.conf"
Copy-Item (Join-Path $Dest "config\server.conf.example") $cfg -Force
(Get-Content $cfg) -replace '^engine=.*',"engine=$Prefix\impl\build\sleela.exe" -replace '^inbox=.*',"inbox=$Prefix\server-edition\inbox\requests.txt" -replace '^state=.*',"state=$Prefix\server-edition\state" -replace '^server_source=.*',"server_source=$Dest\src\Server.sleela" -replace '^log=.*',"log=$Prefix\server-edition\state\results.log" -replace '^sha256_manifest=.*',"sha256_manifest=$Prefix\security\important-sha256-manifest.json" | Set-Content $cfg
Write-Host "Installed SLeeLa Server Edition Service 2 at $Dest"
