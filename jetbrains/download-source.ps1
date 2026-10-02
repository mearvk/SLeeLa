# SLeeLa / JetBrains source acquisition for Windows 10+
# Max Rupplin - MEARVK LLC - 2026
[CmdletBinding()]
param(
  [string]$Destination = "",
  [string]$Branch = "master",
  [switch]$Full,
  [switch]$Update
)

$ErrorActionPreference = "Stop"
$RepoUrl = if ($env:JETBRAINS_SOURCE_REPO) { $env:JETBRAINS_SOURCE_REPO } else { "https://github.com/JetBrains/intellij-community.git" }
if ([string]::IsNullOrWhiteSpace($Destination)) {
  if ($env:JETBRAINS_SOURCE_DIR) { $Destination = $env:JETBRAINS_SOURCE_DIR }
  else { $Destination = Join-Path $HOME "JetBrains\intellij-community" }
}

if (-not (Get-Command git -ErrorAction SilentlyContinue)) { throw "Git is required but was not found in PATH." }
$parent = Split-Path -Parent $Destination
if ($parent) { New-Item -ItemType Directory -Force -Path $parent | Out-Null }

if (Test-Path (Join-Path $Destination ".git")) {
  if (-not $Update) { Write-Host "Source already exists: $Destination"; Write-Host "Use -Update to update it."; exit 0 }
  & git -C $Destination fetch origin $Branch
  & git -C $Destination checkout $Branch
  & git -C $Destination merge --ff-only "origin/$Branch"
  Write-Host "Updated JetBrains source: $Destination"
  exit 0
}

if (Test-Path $Destination) { throw "Destination exists but is not a Git checkout: $Destination" }
$cloneArgs = @("clone")
if (-not $Full) { $cloneArgs += @("--depth", "1") }
$cloneArgs += @("--branch", $Branch, $RepoUrl, $Destination)
& git @cloneArgs
if ($LASTEXITCODE -ne 0) { throw "Git clone failed with exit code $LASTEXITCODE." }
Write-Host "JetBrains source acquired at: $Destination"
