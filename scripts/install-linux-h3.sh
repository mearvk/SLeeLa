#!/bin/bash
# =============================================================================
# install-linux-h3.sh — Install the SLeeLa HTTP 3.0 module (app) on Linux
# =============================================================================
#
# Installs the packet-builder + HTTP/2 listener stack that lets the SLeeLa
# HTTP 3.0 envelope travel the existing internet:
#   * the Python custom packet builder (http-3.0/http3_transport.py),
#   * the Tomcat/Apache-servable HTTP module (SleelaH3Servlet), and
#   * the sample Tomcat (HTTP/2) and Apache (mod_http2 proxy) deployment config.
#
# Usage:
#   ./scripts/install-linux-h3.sh                 # build + stage into PREFIX
#   PREFIX=/opt/sleela ./scripts/install-linux-h3.sh
#   CATALINA_HOME=/opt/tomcat ./scripts/install-linux-h3.sh   # also deploy to Tomcat
#
# Environment variables (all optional):
#   PREFIX          Install root                 (default: /opt/sleela/h3)
#   BIN_DIR         Where launchers/wrappers go  (default: $PREFIX/bin)
#   PYTHON          Python interpreter           (default: python3)
#   JAVAC           Java compiler                (default: javac)
#   CATALINA_HOME   Tomcat home; if set, the WAR/config is copied there
#   SERVLET_API_JAR Path to jakarta servlet-api jar (for compiling the servlet)
#
# Requirements:
#   - bash, coreutils
#   - python3 (3.10+) for the packet builder
#   - a JDK (javac) + a Jakarta Servlet 6.0 API jar to build the HTTP module
#   - (optional) Apache Tomcat 10.1+ and Apache httpd with mod_http2
# =============================================================================
set -euo pipefail

# --- Locate the repository root (this script lives in scripts/). -------------
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." >/dev/null 2>&1 && pwd)"

# --- Configuration -----------------------------------------------------------
PREFIX="${PREFIX:-/opt/sleela/h3}"
BIN_DIR="${BIN_DIR:-$PREFIX/bin}"
PYTHON="${PYTHON:-python3}"
JAVAC="${JAVAC:-javac}"

HTTP3_DIR="$REPO_ROOT/http-3.0"
CONNECTOR_SRC="$REPO_ROOT/connector/java"
DEPLOY_DIR="$REPO_ROOT/connector/deploy"

log()  { printf '\033[1;34m[h3-install]\033[0m %s\n' "$*"; }
warn() { printf '\033[1;33m[h3-install]\033[0m %s\n' "$*" >&2; }
die()  { printf '\033[1;31m[h3-install] ERROR:\033[0m %s\n' "$*" >&2; exit 1; }

# --- Preflight ---------------------------------------------------------------
log "Repository root: $REPO_ROOT"
[ -d "$HTTP3_DIR" ]     || die "http-3.0/ not found at $HTTP3_DIR"
[ -d "$CONNECTOR_SRC" ] || die "connector/java not found at $CONNECTOR_SRC"

command -v "$PYTHON" >/dev/null 2>&1 || die "python3 not found (set PYTHON=...)"
log "Using Python: $($PYTHON --version 2>&1)"

# --- 1. Verify the Python packet builder actually round-trips ----------------
log "Verifying the HTTP packet builder round-trips through the H3 pipeline..."
( cd "$HTTP3_DIR" && "$PYTHON" - <<'PYEOF'
import http3_flow as f, http3_transport as t
key = b"\x11" * f.MAC_KEY_BYTES
pipe = f.Pipeline(mac_key=key)
sid, oid = pipe.register("echo", "say", f.RetryClass.READ, lambda p, c: (f.Status.OK, b"ok:" + p))
def env(n):
    e = f.Envelope(service_id=sid, op_id=oid, request_id=1, payload=b"x", nonce=n); e.seal(key); return e
assert pipe.handle_wire(t.parse_http11(t.build_http11(env(1), key))).startswith(b"H3R 0 ")
assert pipe.handle_wire(t.parse_http2(t.build_http2(env(2), key, include_preface=True))).startswith(b"H3R 0 ")
print("packet-builder self-check: PASS")
PYEOF
) || die "packet-builder self-check failed"

# --- 2. Stage into PREFIX ----------------------------------------------------
log "Staging into $PREFIX ..."
install -d "$PREFIX/http-3.0" "$PREFIX/deploy" "$BIN_DIR"
install -m 0644 "$HTTP3_DIR/http3_transport.py" "$PREFIX/http-3.0/"
install -m 0644 "$HTTP3_DIR/http3_flow.py"      "$PREFIX/http-3.0/"
if [ -d "$DEPLOY_DIR" ]; then
    cp -f "$DEPLOY_DIR/"* "$PREFIX/deploy/" 2>/dev/null || true
fi

# --- 3. Compile the HTTP module (servlet) if a servlet API jar is available --
if [ -n "${SERVLET_API_JAR:-}" ] && command -v "$JAVAC" >/dev/null 2>&1; then
    [ -f "$SERVLET_API_JAR" ] || die "SERVLET_API_JAR does not exist: $SERVLET_API_JAR"
    log "Compiling SleelaH3Servlet against $SERVLET_API_JAR ..."
    CLASSES_DIR="$PREFIX/classes"; install -d "$CLASSES_DIR"
    "$JAVAC" -cp "$SERVLET_API_JAR" -d "$CLASSES_DIR" \
        "$CONNECTOR_SRC/com/mearvk/sleela/connector/http/SleelaH3Servlet.java" \
        || die "servlet compilation failed"
    log "Building WAR (sleela-h3.war) ..."
    WAR_STAGE="$(mktemp -d)"
    install -d "$WAR_STAGE/WEB-INF/classes"
    cp -r "$CLASSES_DIR/"* "$WAR_STAGE/WEB-INF/classes/"
    [ -f "$DEPLOY_DIR/web.xml" ] && install -m 0644 "$DEPLOY_DIR/web.xml" "$WAR_STAGE/WEB-INF/web.xml"
    ( cd "$WAR_STAGE" && jar -cf "$PREFIX/sleela-h3.war" . ) || die "jar/war build failed"
    rm -rf "$WAR_STAGE"
    log "WAR built: $PREFIX/sleela-h3.war"
else
    warn "Skipping servlet build (set SERVLET_API_JAR=/path/to/jakarta.servlet-api.jar and ensure javac is present)."
    warn "The Python packet builder is still installed and usable on its own."
fi

# --- 4. Optional: deploy to Tomcat if CATALINA_HOME is set -------------------
if [ -n "${CATALINA_HOME:-}" ]; then
    [ -d "$CATALINA_HOME" ] || die "CATALINA_HOME is not a directory: $CATALINA_HOME"
    if [ -f "$PREFIX/sleela-h3.war" ]; then
        log "Deploying WAR to Tomcat: $CATALINA_HOME/webapps/"
        install -m 0644 "$PREFIX/sleela-h3.war" "$CATALINA_HOME/webapps/sleela-h3.war"
    fi
    log "Sample HTTP/2 connector config: $PREFIX/deploy/tomcat-server.xml.sample"
    warn "Merge the <Connector> blocks from tomcat-server.xml.sample into $CATALINA_HOME/conf/server.xml to enable HTTP/2."
fi

# --- 5. Install a small launcher for the Python packet builder ---------------
LAUNCHER="$BIN_DIR/sleela-h3"
cat > "$LAUNCHER" <<EOF
#!/bin/bash
# Launcher for the SLeeLa HTTP 3.0 packet builder / transport helpers.
exec "${PYTHON}" -c "import sys; sys.path.insert(0, '${PREFIX}/http-3.0'); import http3_transport as t; print('SLeeLa H3 transport ready:', [n for n in dir(t) if n.startswith(('build_','parse_','wrap','unwrap'))])" "\$@"
EOF
chmod 0755 "$LAUNCHER"

log "Done."
log "  Packet builder : $PREFIX/http-3.0/http3_transport.py"
log "  Launcher       : $LAUNCHER"
log "  Deploy config  : $PREFIX/deploy/ (tomcat-server.xml.sample, apache-h3.conf, web.xml)"
[ -f "$PREFIX/sleela-h3.war" ] && log "  HTTP module WAR: $PREFIX/sleela-h3.war"
log "Apache HTTP/2 front: enable modules then use deploy/apache-h3.conf:"
log "  a2enmod http2 proxy proxy_http proxy_http2 ssl headers && apachectl -k graceful"
