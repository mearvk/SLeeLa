# PowerShell launcher for the GUAL GUI example (Windows).
$ErrorActionPreference = "Stop"
$ExampleDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$Root = (Resolve-Path (Join-Path $ExampleDir "../..")).Path
Set-Location $Root
$Candidates = @()
if ($env:SLEELA_BIN) { $Candidates += $env:SLEELA_BIN }
$Candidates += @((Join-Path $Root "impl/build/sleela.exe"), (Join-Path $Root "impl/build/sleela"), (Join-Path $Root "bin/sleela.exe"))
$Sleela = $Candidates | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } | Select-Object -First 1
if (-not $Sleela) { throw "SLeeLa executable not found. Build it or set SLEELA_BIN." }
if (-not $env:SLEELA_SHEET) { $env:SLEELA_SHEET = Join-Path $Root "SHEET.sheet" }
if (-not $env:SLEELA_SHA256_MANIFEST) { $env:SLEELA_SHA256_MANIFEST = Join-Path $Root "security/sha256-manifest.json" }
& $Sleela run "example/gual/gui-gual-settings-console.sleela" @args
exit $LASTEXITCODE
