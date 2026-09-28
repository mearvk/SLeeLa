param([string]$Prefix = "$env:ProgramFiles\SLeeLa")
$ErrorActionPreference = "Stop"
$Root = Join-Path $Prefix ""
$Dir = Join-Path $Root "server-edition\moral\2"
$Config = Join-Path $Dir "config\server.conf"
if (-not (Test-Path $Config)) { throw "SLeeLa Service 2: configuration not found: $Config" }

$cfg = @{}
Get-Content $Config | ForEach-Object {
  $line = $_.Trim()
  if ($line -and -not $line.StartsWith("#") -and $line.Contains("=")) {
    $pair = $line.Split("=",2)
    $cfg[$pair[0]] = $pair[1]
  }
}
$Engine = if ($cfg.ContainsKey("engine")) { $cfg.engine } else { Join-Path $Prefix "impl\build\sleela.exe" }
$Source = if ($cfg.ContainsKey("server_source")) { $cfg.server_source } else { Join-Path $Dir "src\Server.sleela" }
$Port = if ($cfg.ContainsKey("port")) { [int]$cfg.port } else { 19866 }
$Protocol = if ($cfg.ContainsKey("port_protocol")) { $cfg.port_protocol } else { "tcp" }
$PortCtl = Join-Path $Prefix "server-edition\port-awareness\portctl.ps1"
if (-not (Test-Path $Engine)) { throw "SLeeLa Service 2: engine not found: $Engine" }
if (-not (Test-Path $Source)) { throw "SLeeLa Service 2: source not found: $Source" }
if (-not (Test-Path $PortCtl)) { throw "SLeeLa Service 2: firewall controller not found: $PortCtl" }

& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PortCtl -Action close -Edition "Discord-2" -Port $Port -Protocol $Protocol | Out-Null
& powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PortCtl -Action open -Edition "Discord-2" -Port $Port -Protocol $Protocol
if ($LASTEXITCODE -ne 0) { throw "SLeeLa Service 2: firewall port open failed" }
try {
  $env:SLEELA_SERVER_CONFIG = $Config
  $env:SLEELA_SHA256_MANIFEST = Join-Path $Prefix "security\important-sha256-manifest.json"
  & $Engine run $Source
  if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
}
finally {
  & powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File $PortCtl -Action close -Edition "Discord-2" -Port $Port -Protocol $Protocol | Out-Null
}
