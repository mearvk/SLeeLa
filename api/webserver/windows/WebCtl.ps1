[CmdletBinding()]
param([ValidateSet("install","upgrade","status","health","validate","port","module")][string]$Action="status")
$Server=if($env:SLEELA_WEB_SERVER){$env:SLEELA_WEB_SERVER}else{"tomcat"}
$Port=if($env:SLEELA_WEB_PORT){[int]$env:SLEELA_WEB_PORT}else{8080}
$HealthUrl=if($env:SLEELA_WEB_HEALTH_URL){$env:SLEELA_WEB_HEALTH_URL}else{"/"}
$Module=$env:SLEELA_WEB_MODULE
function Fail($m){Write-Error "SLeeLa WebCtl: $m";exit 1}
function Admin{$id=[Security.Principal.WindowsIdentity]::GetCurrent();$p=New-Object Security.Principal.WindowsPrincipal($id);if(!$p.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)){Fail "Administrator privileges required."}}
function Check{if($Server -notin @("apache","tomcat")){Fail "SLEELA_WEB_SERVER must be apache or tomcat"};if($Port -lt 1 -or $Port -gt 65535){Fail "SLEELA_WEB_PORT must be 1..65535"};if(!$HealthUrl.StartsWith("/")){Fail "SLEELA_WEB_HEALTH_URL must begin with /"}}
function Service{$n=if($Server -eq "tomcat"){"Tomcat*"}else{"Apache*"};Get-Service -Name $n -ErrorAction SilentlyContinue|Select-Object -First 1}
function Install{Admin;Check;if($Server -eq "tomcat" -and (Get-Command winget -ErrorAction SilentlyContinue)){winget install --id Apache.Tomcat --accept-package-agreements --accept-source-agreements}else{Fail "No supported package installation path; install the selected distribution explicitly."}}
function Upgrade{Admin;Check;if($Server -eq "tomcat" -and (Get-Command winget -ErrorAction SilentlyContinue)){winget upgrade --id Apache.Tomcat --accept-package-agreements --accept-source-agreements}else{Fail "No supported package-manager upgrade path."}}
function Status{Check;$s=Service;if($s){$s|Format-List Name,Status,StartType}else{"SLeeLa webctl: service not found for $Server"}}
function Health{Check;try{$r=Invoke-WebRequest -Uri ("http://127.0.0.1:{0}{1}" -f $Port,$HealthUrl) -UseBasicParsing -TimeoutSec 10;if($r.StatusCode -lt 200 -or $r.StatusCode -ge 400){Fail "HTTP health returned $($r.StatusCode)"};"SLeeLa webctl: HEALTH OK server=$Server port=$Port path=$HealthUrl"}catch{Fail "HTTP health check failed: $($_.Exception.Message)"}}
function XmlPath{$roots=@("$env:ProgramFiles\Apache Software Foundation","$env:ProgramFiles\Tomcat");foreach($r in $roots){if(Test-Path $r){$f=Get-ChildItem $r -Filter server.xml -Recurse -ErrorAction SilentlyContinue|Select-Object -First 1;if($f){return $f.FullName}}}return $null}
function Validate{Check;if($Server -eq "tomcat"){$f=XmlPath;if(!$f){Fail "Tomcat server.xml not found"};[xml](Get-Content -Raw -LiteralPath $f)|Out-Null;"SLeeLa webctl: Tomcat XML configuration parses successfully."}else{"SLeeLa webctl: Apache validation requires a distribution-specific httpd.exe path."}}
function Port{Admin;Check;if($Server -ne "tomcat"){Fail "Apache port changes require a distribution-specific httpd.conf adapter."};$f=XmlPath;if(!$f){Fail "Tomcat server.xml not found"};$s=Get-Content -Raw -LiteralPath $f;$n=[regex]::Replace($s,'(<Connector\b[^>]*\bport=")[0-9]+(")',{param($m)$m.Groups[1].Value+$Port+$m.Groups[2].Value},1);if($n -eq $s){Fail "Tomcat Connector port not found"};Set-Content -LiteralPath $f -Value $n -Encoding UTF8;Validate;$svc=Service;if($svc){Restart-Service -Name $svc.Name -Force};Health}
function Module{Admin;Check;if(!$Module -or !(Test-Path $Module -PathType Leaf) -or $Server -ne "tomcat" -or !$Module.EndsWith(".war")){Fail "Windows module deployment currently accepts an existing local .war for Tomcat only."};$f=XmlPath;if(!$f){Fail "Tomcat installation not found"};$home=Split-Path (Split-Path $f -Parent) -Parent;$d=Join-Path $home "webapps";if(!(Test-Path $d)){Fail "Tomcat webapps directory not found"};Copy-Item $Module $d -Force;$svc=Service;if($svc){Restart-Service -Name $svc.Name -Force};Health}
Check
switch($Action){install{Install};upgrade{Upgrade};status{Status};health{Health};validate{Validate};port{Port};module{Module}}
