param([string]$Version='', [string]$SourceRoot=(Get-Location).Path, [string]$Prefix="$env:ProgramFiles\SLeeLa\http", [switch]$All, [switch]$NoBuild)
$ErrorActionPreference='Stop'
$cv=Get-ItemProperty 'HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion'
Write-Host "SLeeLa HTTP installer: $($cv.ProductName), build $($cv.CurrentBuild), arch $env:PROCESSOR_ARCHITECTURE"
Write-Host 'Windows support is capability/build based; no unverified Windows 12 build is assumed.'
New-Item -ItemType Directory -Force -Path $Prefix | Out-Null
function Install-One([string]$v) {
  $module=Join-Path $SourceRoot ("http-"+$v); if(!(Test-Path $module -PathType Container)){throw "Missing module: $module"}
  $make=Get-Command make -ErrorAction SilentlyContinue
  if(!$NoBuild -and $make -and (Test-Path (Join-Path $module 'build\Makefile'))){ & $make.Source -C (Join-Path $module 'build') syntax; if($LASTEXITCODE -ne 0){throw "Build validation failed for HTTP $v"} }
  $dest=Join-Path $Prefix $v; if(Test-Path $dest){Remove-Item -Recurse -Force $dest}; Copy-Item -Recurse -Force $module $dest
  @("sleela-http-version=$v","os=windows","arch=$env:PROCESSOR_ARCHITECTURE","build=$($cv.CurrentBuild)","source-root=$SourceRoot") | Set-Content (Join-Path $dest 'INSTALL.MANIFEST') -Encoding UTF8
  Write-Host "Installed SLeeLa HTTP $v -> $dest"
}
if($All){Get-ChildItem $SourceRoot -Directory -Filter 'http-*.0' | ForEach-Object {Install-One $_.Name.Substring(5)}} else {if([string]::IsNullOrWhiteSpace($Version)){throw 'Specify -Version X.Y or -All'}; Install-One $Version}
