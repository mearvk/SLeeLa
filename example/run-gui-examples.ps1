# Run all five GUI examples on Windows PowerShell.
$ErrorActionPreference = "Stop"
$Root = (Resolve-Path (Join-Path $PSScriptRoot "..")).Path
$Candidates = @()
if ($env:SLEELA_BIN) { $Candidates += $env:SLEELA_BIN }
$Candidates += @((Join-Path $Root "impl/build/sleela.exe"), (Join-Path $Root "impl/build/sleela"), (Join-Path $Root "bin/sleela.exe"))
$Sleela = $Candidates | Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } | Select-Object -First 1
if (-not $Sleela) { throw "SLeeLa executable not found. Build it or set SLEELA_BIN." }
if (-not $env:SLEELA_SHEET) { $env:SLEELA_SHEET = Join-Path $Root "SHEET.sheet" }
if (-not $env:SLEELA_SHA256_MANIFEST) { $env:SLEELA_SHA256_MANIFEST = Join-Path $Root "security/sha256-manifest.json" }
Set-Location $Root
$Examples = @(
  "example/mual/gui-mual-text-editor.sleela",
  "example/gual/gui-gual-settings-console.sleela",
  "example/thual/gui-thual-effects-showcase.sleela",
  "example/dual/gui-dual-media-studio.sleela",
  "example/military/gui-military-command-console.sleela"
)
foreach ($Example in $Examples) {
  Write-Host ("=" * 72)
  Write-Host "Running $Example"
  & $Sleela run $Example
  if ($LASTEXITCODE -ne 0) { throw "Example failed: $Example (exit $LASTEXITCODE)" }
}
Write-Host "All five GUI examples completed."
