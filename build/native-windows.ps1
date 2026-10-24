[CmdletBinding()]
param(
    [ValidateSet("UCRT64", "MINGW64")][string]$Environment = "UCRT64",
    [string]$Make = "make"
)
$ErrorActionPreference = "Stop"
$Root = Split-Path -Parent $PSScriptRoot
$env:SLEELA_SHA256_MANIFEST = Join-Path $Root "security\sha256-manifest.json"
if (-not (Get-Command $Make -ErrorAction SilentlyContinue)) {
    throw "GNU Make not found. Install MSYS2 with the $Environment MinGW-w64 toolchain, then run from its shell or add make to PATH."
}
Push-Location $Root
try {
    Write-Host "SLeeLa native build: Windows 10+ / $Environment"
    & $Make -C impl clean all
    if ($LASTEXITCODE -ne 0) { throw "impl build failed ($LASTEXITCODE)" }
    & $Make -C impl test-platform test-threads test-network
    if ($LASTEXITCODE -ne 0) { throw "impl smoke tests failed ($LASTEXITCODE)" }
    & $Make -C regex test
    if ($LASTEXITCODE -ne 0) { throw "regex tests failed ($LASTEXITCODE)" }
    & $Make -C antivirus test
    if ($LASTEXITCODE -ne 0) { throw "antivirus tests failed ($LASTEXITCODE)" }
    Write-Host "Windows native build and selected smoke tests completed."
}
finally { Pop-Location }
