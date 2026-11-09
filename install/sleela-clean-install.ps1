# SLeeLa Clean Install for Windows 10+ — native PowerShell guided installer.
# Max Rupplin - MEARVK LLC - 2026
[CmdletBinding()]
param([string]$Destination = "", [switch]$NoBuild, [switch]$Plain)
$ErrorActionPreference = "Stop"
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot = Split-Path -Parent $ScriptDir
$Release = "1.3.2-pro"
function Heading([string]$Text) {
  Write-Host ""; Write-Host ("-" * 72) -ForegroundColor DarkGray
  Write-Host ("  SLeeLa | " + $Text) -ForegroundColor Green
  Write-Host ("-" * 72) -ForegroundColor DarkGray
}
function Ask-YesNo([string]$Prompt, [string]$Default) {
  if ([Console]::IsInputRedirected) { Write-Host "  ? $Prompt (non-interactive; default: $Default)"; return $Default }
  $hint = if ($Default -eq "yes") { "Y/n" } else { "y/N" }
  $answer = Read-Host "$Prompt [$hint]"
  if ([string]::IsNullOrWhiteSpace($answer)) { $answer = $Default }
  if ($answer -match '^(?i:y|yes)$') { return "yes" }; return "no"
}
function Ask-Value([string]$Prompt, [string]$Default) {
  if ([Console]::IsInputRedirected) { return $Default }
  $answer = Read-Host "$Prompt [$Default]"
  if ([string]::IsNullOrWhiteSpace($answer)) { return $Default }; return $answer
}
Write-Host "S L e e L a    Clean Install" -ForegroundColor Green
Write-Host "THE NOW. THE INGREDIENT."
Write-Host "Release $Release | Windows 10+ | Native PowerShell"
Write-Host "Installs sleela.exe, sleelvac.exe, nordshrift.exe and the selected library modules."
if ((Ask-YesNo "Install the SLeeLa main package now?" "yes") -ne "yes") { Write-Host "No changes made."; exit 0 }
Heading "Installation scope"
Write-Host "  1) User-local (recommended; no administrator rights)"
Write-Host "  2) System-wide (requires elevated PowerShell)"
$scope = Ask-Value "Choose (1|2)" "1"
if ($scope -eq "1") {
  if ([string]::IsNullOrWhiteSpace($Destination)) { $Destination = Join-Path $env:LOCALAPPDATA "SLeeLa" }
  $Mode = "user"
} elseif ($scope -eq "2") {
  $admin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
  if (-not $admin) { throw "System-wide install requires elevated PowerShell. No installation was changed." }
  if ([string]::IsNullOrWhiteSpace($Destination)) { $Destination = Join-Path $env:ProgramFiles "SLeeLa" }
  $Mode = "system"
} else { throw "Choose 1 (user-local) or 2 (system-wide)." }

$core = @("core","cpu","opcodes","vm","runtime","memory","native","reflection","security","registry","config","process","modules","public","sleela")
$blocks = @(
 @{ Name="Language, Compiler & Text"; Members=@("compiler","decompiler","debugger","sst","nordshrift","languages","alphabet","character","boolean","regex","xml","html","text","sldocument","sleeladoc") },
 @{ Name="Standard Library, Data & Media"; Members=@("collections","data","data-structures","math","crypto","compression","codecs","audio","video","signal-processing","io","filesystem","database","db-connector","sleela-sql","ledger","inputs","inference-model") },
 @{ Name="Systems, Networking & Protocols"; Members=@("net","rmi","munction","synchro","connector","drivers","operating-system","systems","bash","terminal","terminal_pixel","sleela-terminal","telephony-skya","cdma","wifi","http","http-1.0","http-2.0","http-3.0","http-4.0","http-5.0","http-6.0","http-7.0","http-8.0","http-9.0","http-servers","server-edition","web-server-technology","website","api","us-infrastructure") },
 @{ Name="Applications, UI, Tooling & Society"; Members=@("gui","ui","user-interface","idea","ballum","autocad","ide","ide-workbench","linux-editors","common-rails","coorenagraph","deliberation","gameplay","journey","sommeliers","social-model","citizen","manifesto","moral-capacitor","philosophers","politico","international-criminal-court","psychiatry","java","java28","antivirus","churn","xlt") }
)
# Top-level programs that are NOT lib/ packages (identical set to the Linux/
# macOS editions): pi-generator (Pi spigot) and artificial-intelligence (the AI
# subsystem: large-language-models with RRS/RLM/code-LLMs, neural-nets,
# character-recognition, rrs). These live as their own top-level directories, so
# the lib/-scoped catalog and the file manifest do not cover them; they are
# offered and staged explicitly below. Only those present on disk are offered.
$toplevelName = "Top-level programs (Pi generator, Artificial Intelligence)"
$toplevelAll = @("pi-generator","artificial-intelligence") | Where-Object { Test-Path (Join-Path $RepoRoot $_) -PathType Container }
$toplevelInclude = @()

$libRoot = Join-Path $RepoRoot "lib"
$allLib = @(Get-ChildItem $libRoot -Directory | ForEach-Object { $_.Name })
$excluded = @($allLib | Where-Object { $core -notcontains $_ })
$known = @($blocks | ForEach-Object { $_.Members })
$extra = @($excluded | Where-Object { $known -notcontains $_ })
Heading "Choose your SLeeLa environment"
Write-Host "Core runtime modules are always installed. Choose whole blocks or individual modules."
if ((Ask-YesNo "Install EVERYTHING — every optional module?" "no") -eq "yes") {
  $excluded = @()
  $toplevelInclude = @($toplevelAll)
} else {
  foreach ($block in $blocks) {
    $members = @($block.Members | Where-Object { $allLib -contains $_ })
    if ($members.Count -eq 0) { continue }
    Heading "$($block.Name) ($($members.Count) modules)"
    Write-Host ($members -join ", ")
    if ((Ask-YesNo "Include the entire '$($block.Name)' block?" "yes") -eq "yes") {
      $excluded = @($excluded | Where-Object { $members -notcontains $_ })
    } else {
      foreach ($module in $members) {
        if ((Ask-YesNo "Include module '$module'?" "no") -eq "yes") { $excluded = @($excluded | Where-Object { $_ -ne $module }) }
      }
    }
  }
  if ($extra.Count -gt 0) {
    Heading "Additional modules ($($extra.Count))"; Write-Host ($extra -join ", ")
    if ((Ask-YesNo "Include all additional modules?" "no") -eq "yes") { $excluded = @($excluded | Where-Object { $extra -notcontains $_ }) }
    else { foreach ($module in $extra) { if ((Ask-YesNo "Include additional module '$module'?" "no") -eq "yes") { $excluded = @($excluded | Where-Object { $_ -ne $module }) } } }
  }
  if ($toplevelAll.Count -gt 0) {
    Heading "$toplevelName ($($toplevelAll.Count) programs)"; Write-Host ($toplevelAll -join ", ")
    if ((Ask-YesNo "Include the entire '$toplevelName' block?" "yes") -eq "yes") { $toplevelInclude = @($toplevelAll) }
    else { foreach ($prog in $toplevelAll) { if ((Ask-YesNo "Include program '$prog'?" "no") -eq "yes") { $toplevelInclude += $prog } } }
  }
}
if (-not $NoBuild) {
  Heading "Building SLeeLa with the native Windows toolchain"
  $buildScript = Join-Path $RepoRoot "build-windows.ps1"
  if (-not (Test-Path $buildScript -PathType Leaf)) { throw "Windows build entry point is missing: $buildScript" }
  & $buildScript
  if ($LASTEXITCODE -ne 0) { throw "Windows native build failed; installation was not changed." }
}
Heading "Staging and integrity verification"
$stage = Join-Path ([IO.Path]::GetTempPath()) ("sleela-clean-" + [guid]::NewGuid().ToString("N"))
try {
  # Installed executables live at <prefix>\bin. The runtime root MUST be
  # <prefix>\lib\sleela because impl/frontend/driver.cpp derives it from the
  # executable path and then looks for lib\, tools\, security\ and SHEET.sheet.
  $runtimeRoot = Join-Path $stage "lib\sleela"
  foreach ($d in @("bin","lib","config","tools","security")) { New-Item -ItemType Directory -Force -Path (Join-Path $runtimeRoot $d) | Out-Null }
  New-Item -ItemType Directory -Force -Path (Join-Path $stage "bin") | Out-Null
  $manifestPath = Join-Path $RepoRoot "security\sha256-manifest.json"
  $verifier = Join-Path $RepoRoot "tools\verify-before-execution.py"
  if (-not (Test-Path $manifestPath) -or -not (Test-Path $verifier)) { throw "Integrity manifest/verifier missing." }
  Copy-Item -Force $manifestPath (Join-Path $runtimeRoot "security")
  Copy-Item -Force $verifier (Join-Path $runtimeRoot "tools")
  $manifest = Get-Content $manifestPath -Raw | ConvertFrom-Json
  foreach ($entry in $manifest.files) {
    $relative = $entry.path.Replace("/", "\")
    $source = Join-Path $RepoRoot $relative
    if (-not (Test-Path $source -PathType Leaf)) { throw "Manifest source missing: $($entry.path)" }
    $target = Join-Path $runtimeRoot $relative
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $target) | Out-Null
    Copy-Item -Force $source $target
  }
  $py = Get-Command py -ErrorAction SilentlyContinue
  if ($py) { & $py.Source -3 (Join-Path $runtimeRoot "tools\verify-before-execution.py") --manifest (Join-Path $runtimeRoot "security\sha256-manifest.json") --root $runtimeRoot }
  else {
    $python = Get-Command python -ErrorAction SilentlyContinue
    if (-not $python) { throw "Python 3 is required for SHA-256 integrity verification." }
    & $python.Source (Join-Path $runtimeRoot "tools\verify-before-execution.py") --manifest (Join-Path $runtimeRoot "security\sha256-manifest.json") --root $runtimeRoot
  }
  if ($LASTEXITCODE -ne 0) { throw "Staged SHA-256 verification failed. Installation was not changed." }
  foreach ($product in @("sleela","sleelvac","nordshrift")) {
    $built = Join-Path $RepoRoot "impl\build\$product.exe"
    if (-not (Test-Path $built -PathType Leaf)) { $built = $null }
    if (-not $built) { throw "Required Windows product missing: impl\build\$product.exe. Build the native Windows artifacts first; no install was changed." }
    Copy-Item -Force $built (Join-Path $stage "bin\$product.exe")
  }
  foreach ($entry in Get-ChildItem $libRoot -Directory) {
    if ($excluded -contains $entry.Name) { continue }
    Copy-Item -Recurse -Force $entry.FullName (Join-Path $runtimeRoot "lib")
  }
  Get-ChildItem $libRoot -File -Filter "*.md" | Copy-Item -Destination (Join-Path $stage "lib") -Force
  # Stage selected top-level (non-lib/) programs into their own directories.
  foreach ($prog in $toplevelInclude) {
    $progSrc = Join-Path $RepoRoot $prog
    if (Test-Path $progSrc -PathType Container) { Copy-Item -Recurse -Force $progSrc $stage }
  }
  if (Test-Path (Join-Path $RepoRoot "SHEET.sheet")) { Copy-Item -Force (Join-Path $RepoRoot "SHEET.sheet") $stage }
  $servers = if ($excluded -contains "http") { "excluded" } else { "included" }
  $ides = if ($excluded -contains "ide") { "excluded" } else { "included" }
  $wifi = if ($excluded -contains "wifi") { "excluded" } else { "included" }
  @("# Generated by SLeeLa Clean Install for Windows","install.mode=$Mode","install.root=$Destination","package=main","servers=$servers","ides=$ides","wifi=$wifi") | Set-Content -Encoding UTF8 (Join-Path $runtimeRoot "config\startup.conf")
  # Final layout guard: fail before touching the destination if the installed
  # executable-relative runtime contract is not satisfied.
  foreach ($required in @(
    (Join-Path $stage "bin\sleela.exe"),
    (Join-Path $stage "bin\sleelvac.exe"),
    (Join-Path $stage "bin\nordshrift.exe"),
    (Join-Path $runtimeRoot "tools\verify-before-execution.py"),
    (Join-Path $runtimeRoot "security\sha256-manifest.json"),
    (Join-Path $runtimeRoot "lib")
  )) {
    if (-not (Test-Path $required)) { throw "Staged Windows runtime is incomplete: $required. Installation was not changed." }
  }
  Heading "Installing"
  if (Test-Path $Destination) {
    $backupRoot = if ($Mode -eq "user") { Join-Path $env:LOCALAPPDATA "SLeeLa-backups" } else { Join-Path $env:ProgramData "SLeeLa-backups" }
    New-Item -ItemType Directory -Force -Path $backupRoot | Out-Null
    $backup = Join-Path $backupRoot (Get-Date -Format "yyyyMMdd-HHmmss")
    Copy-Item -Recurse -Force $Destination $backup
    Write-Host "Backup: $backup"
  }
  New-Item -ItemType Directory -Force -Path $Destination | Out-Null
  Copy-Item -Recurse -Force (Join-Path $stage "*") $Destination
  $bin = Join-Path $Destination "bin"
  $scopePath = if ($Mode -eq "user") { "User" } else { "Machine" }
  $path = [Environment]::GetEnvironmentVariable("Path",$scopePath)
  $parts = @($path -split ';' | Where-Object { $_ -and $_ -ne $bin })
  [Environment]::SetEnvironmentVariable("Path", (($parts + $bin) -join ';'), $scopePath)
  Write-Host "SLeeLa $Release installed." -ForegroundColor Green
  Write-Host "Binaries: $bin"; Write-Host "Runtime: $(Join-Path $Destination 'lib\sleela')"; Write-Host "Configuration: $(Join-Path $Destination 'lib\sleela\config\startup.conf')"
  Write-Host "Modules excluded: $($excluded.Count)"
  Write-Host "Top-level programs installed: $($toplevelInclude.Count)$(if ($toplevelInclude.Count -gt 0) { ' (' + ($toplevelInclude -join ', ') + ')' })"
} finally { if (Test-Path $stage) { Remove-Item -Recurse -Force $stage } }
