# =============================================================================
# install-windows-h3.ps1 — Install the SLeeLa HTTP 3.0 module (app) on Windows
# =============================================================================
#
# Windows counterpart of install-linux-h3.sh / install-macos-h3.sh. Installs the
# packet-builder + HTTP/2 listener stack that lets the SLeeLa HTTP 3.0 envelope
# travel the existing internet:
#   * the Python custom packet builder (http-3.0\http3_transport.py),
#   * the Tomcat/Apache-servable HTTP module (SleelaH3Servlet), and
#   * the sample Tomcat (HTTP/2) and Apache (mod_http2 proxy) deployment config.
#
# Usage (PowerShell):
#   .\scripts\install-windows-h3.ps1
#   .\scripts\install-windows-h3.ps1 -Prefix 'C:\SLeeLa\h3'
#   .\scripts\install-windows-h3.ps1 -CatalinaHome 'C:\tomcat' -ServletApiJar 'C:\lib\jakarta.servlet-api-6.0.0.jar'
#
# Parameters (all optional):
#   -Prefix         Install root                 (default: $Env:ProgramData\SLeeLa\h3)
#   -BinDir         Where launchers go           (default: <Prefix>\bin)
#   -Python         Python interpreter           (default: python)
#   -Javac          Java compiler                (default: javac)
#   -CatalinaHome   Tomcat home; if set, WAR/config is copied there
#   -ServletApiJar  Path to jakarta servlet-api jar (to build the HTTP module)
#
# Requirements:
#   - PowerShell 5+ (Windows PowerShell or PowerShell 7)
#   - Python 3.10+ on PATH (or pass -Python)
#   - a JDK (javac) + a Jakarta Servlet 6.0 API jar to build the HTTP module
#   - (optional) Apache Tomcat 10.1+ and Apache httpd with mod_http2
# =============================================================================
[CmdletBinding()]
param(
    [string]$Prefix        = (Join-Path $Env:ProgramData 'SLeeLa\h3'),
    [string]$BinDir        = '',
    [string]$Python        = 'python',
    [string]$Javac         = 'javac',
    [string]$CatalinaHome  = '',
    [string]$ServletApiJar = ''
)

$ErrorActionPreference = 'Stop'
if ([string]::IsNullOrEmpty($BinDir)) { $BinDir = Join-Path $Prefix 'bin' }

function Write-Log  { param($m) Write-Host "[h3-install] $m" -ForegroundColor Cyan }
function Write-Warn { param($m) Write-Host "[h3-install] $m" -ForegroundColor Yellow }
function Die        { param($m) Write-Error "[h3-install] ERROR: $m"; exit 1 }

# --- Locate the repository root (this script lives in scripts\). -------------
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$RepoRoot  = (Resolve-Path (Join-Path $ScriptDir '..')).Path

$Http3Dir     = Join-Path $RepoRoot 'http-3.0'
$ConnectorSrc = Join-Path $RepoRoot 'connector\java'
$DeployDir    = Join-Path $RepoRoot 'connector\deploy'

# --- Preflight ---------------------------------------------------------------
Write-Log "Repository root: $RepoRoot"
if (-not (Test-Path $Http3Dir))     { Die "http-3.0\ not found at $Http3Dir" }
if (-not (Test-Path $ConnectorSrc)) { Die "connector\java not found at $ConnectorSrc" }
if (-not (Get-Command $Python -ErrorAction SilentlyContinue)) { Die "python not found (pass -Python)" }
Write-Log ("Using Python: " + (& $Python --version 2>&1))

# --- 1. Verify the Python packet builder actually round-trips ----------------
Write-Log "Verifying the HTTP packet builder round-trips through the H3 pipeline..."
$checkPy = @'
import http3_flow as f, http3_transport as t
key = b"\x11" * f.MAC_KEY_BYTES
pipe = f.Pipeline(mac_key=key)
sid, oid = pipe.register("echo", "say", f.RetryClass.READ, lambda p, c: (f.Status.OK, b"ok:" + p))
def env(n):
    e = f.Envelope(service_id=sid, op_id=oid, request_id=1, payload=b"x", nonce=n); e.seal(key); return e
assert pipe.handle_wire(t.parse_http11(t.build_http11(env(1), key))).startswith(b"H3R 0 ")
assert pipe.handle_wire(t.parse_http2(t.build_http2(env(2), key, include_preface=True))).startswith(b"H3R 0 ")
print("packet-builder self-check: PASS")
'@
Push-Location $Http3Dir
try {
    $checkPy | & $Python -
    if ($LASTEXITCODE -ne 0) { Die "packet-builder self-check failed" }
} finally { Pop-Location }

# --- 2. Stage into Prefix ----------------------------------------------------
Write-Log "Staging into $Prefix ..."
New-Item -ItemType Directory -Force -Path (Join-Path $Prefix 'http-3.0') | Out-Null
New-Item -ItemType Directory -Force -Path (Join-Path $Prefix 'deploy')   | Out-Null
New-Item -ItemType Directory -Force -Path $BinDir | Out-Null
Copy-Item -Force (Join-Path $Http3Dir 'http3_transport.py') (Join-Path $Prefix 'http-3.0')
Copy-Item -Force (Join-Path $Http3Dir 'http3_flow.py')      (Join-Path $Prefix 'http-3.0')
if (Test-Path $DeployDir) {
    Copy-Item -Force (Join-Path $DeployDir '*') (Join-Path $Prefix 'deploy') -ErrorAction SilentlyContinue
}

# --- 3. Compile the HTTP module (servlet) if a servlet API jar is available --
if ($ServletApiJar -and (Get-Command $Javac -ErrorAction SilentlyContinue)) {
    if (-not (Test-Path $ServletApiJar)) { Die "ServletApiJar does not exist: $ServletApiJar" }
    Write-Log "Compiling SleelaH3Servlet against $ServletApiJar ..."
    $classesDir = Join-Path $Prefix 'classes'
    New-Item -ItemType Directory -Force -Path $classesDir | Out-Null
    $servlet = Join-Path $ConnectorSrc 'com\mearvk\sleela\connector\http\SleelaH3Servlet.java'
    & $Javac -cp $ServletApiJar -d $classesDir $servlet
    if ($LASTEXITCODE -ne 0) { Die "servlet compilation failed" }

    Write-Log "Building WAR (sleela-h3.war) ..."
    $warStage = Join-Path ([System.IO.Path]::GetTempPath()) ("sleela-h3-" + [System.Guid]::NewGuid().ToString('N'))
    $webInf   = Join-Path $warStage 'WEB-INF\classes'
    New-Item -ItemType Directory -Force -Path $webInf | Out-Null
    Copy-Item -Recurse -Force (Join-Path $classesDir '*') $webInf
    $webXml = Join-Path $DeployDir 'web.xml'
    if (Test-Path $webXml) { Copy-Item -Force $webXml (Join-Path $warStage 'WEB-INF\web.xml') }
    Push-Location $warStage
    try {
        & jar -cf (Join-Path $Prefix 'sleela-h3.war') .
        if ($LASTEXITCODE -ne 0) { Die "jar/war build failed" }
    } finally { Pop-Location }
    Remove-Item -Recurse -Force $warStage
    Write-Log ("WAR built: " + (Join-Path $Prefix 'sleela-h3.war'))
} else {
    Write-Warn "Skipping servlet build (pass -ServletApiJar C:\path\to\jakarta.servlet-api.jar and ensure javac is present)."
    Write-Warn "The Python packet builder is still installed and usable on its own."
}

# --- 4. Optional: deploy to Tomcat if -CatalinaHome is set -------------------
if ($CatalinaHome) {
    if (-not (Test-Path $CatalinaHome)) { Die "CatalinaHome is not a directory: $CatalinaHome" }
    $war = Join-Path $Prefix 'sleela-h3.war'
    if (Test-Path $war) {
        Write-Log "Deploying WAR to Tomcat: $CatalinaHome\webapps\"
        Copy-Item -Force $war (Join-Path $CatalinaHome 'webapps\sleela-h3.war')
    }
    Write-Log ("Sample HTTP/2 connector config: " + (Join-Path $Prefix 'deploy\tomcat-server.xml.sample'))
    Write-Warn "Merge the <Connector> blocks from tomcat-server.xml.sample into $CatalinaHome\conf\server.xml to enable HTTP/2."
}

# --- 5. Install a small launcher (.cmd) for the Python packet builder --------
$launcher = Join-Path $BinDir 'sleela-h3.cmd'
$h3Path   = (Join-Path $Prefix 'http-3.0')
@"
@echo off
REM Launcher for the SLeeLa HTTP 3.0 packet builder / transport helpers.
"$Python" -c "import sys; sys.path.insert(0, r'$h3Path'); import http3_transport as t; print('SLeeLa H3 transport ready:', [n for n in dir(t) if n.startswith(('build_','parse_','wrap','unwrap'))])" %*
"@ | Set-Content -Encoding ASCII $launcher

Write-Log "Done."
Write-Log ("  Packet builder : " + (Join-Path $Prefix 'http-3.0\http3_transport.py'))
Write-Log ("  Launcher       : $launcher")
Write-Log ("  Deploy config  : " + (Join-Path $Prefix 'deploy') + " (tomcat-server.xml.sample, apache-h3.conf, web.xml)")
if (Test-Path (Join-Path $Prefix 'sleela-h3.war')) { Write-Log ("  HTTP module WAR: " + (Join-Path $Prefix 'sleela-h3.war')) }
Write-Log "Apache HTTP/2 front: enable mod_http2 + mod_proxy_http2 in httpd.conf, include deploy\apache-h3.conf, and reload."
