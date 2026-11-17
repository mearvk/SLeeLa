#!/usr/bin/env bash
# Deploy the SLeeLa site onto an existing Linux host running Tomcat (10.1+) and Apache httpd (Debian/Ubuntu apache2).
#
#   sudo ./deploy/deploy-apache-tomcat.sh              # back up, build, deploy to Tomcat, then front it with Apache
#   sudo ./deploy/deploy-apache-tomcat.sh --dry-run    # print every step, change nothing
#
# What it does, in order:
#   1. Detects Tomcat (CATALINA_BASE, version, systemd unit, HTTP port) and Apache (apache2ctl, a2enmod, mod_proxy_http).
#      A target that is missing or unsuitable is skipped with the reason; the other one still deploys.
#   2. Backs up what is served at "/" now into /backup/$DATE: Tomcat webapps/ROOT(.war) + ROOT.xml + server.xml,
#      every Apache DocumentRoot, the whole /etc/apache2 tree, and the previous static dir. Writes backup/restore.sh.
#   3. Fetches or refreshes the SLeeLa checkout (content is read live from it) and builds the WAR (mvn, else Docker).
#   4. Tomcat: installs the WAR as ROOT, points it at the checkout via conf/Catalina/localhost/ROOT.xml, restarts,
#      and waits for /healthz. On failure it restores the Tomcat backup automatically (unless --no-rollback).
#   5. Apache: enables proxy + proxy_http (+ headers), serves /assets and /favicon.svg itself from STATIC_DIR,
#      proxies everything else to Tomcat, runs configtest before reload, checks /healthz through Apache.
#      On failure it restores /etc/apache2 from the backup and reloads.
#
# Options:
#   --dry-run        show what would happen, change nothing (no root needed)
#   --tomcat-only    skip Apache
#   --apache-only    skip Tomcat (Tomcat must already serve the site)
#   --no-rollback    keep the failed state for debugging instead of restoring the backup
#   --war FILE       use a prebuilt WAR instead of building
#   -h, --help
#
# Environment overrides (defaults in brackets):
#   SLEELA_SRC [/srv/sleela]  SLEELA_REPO [https://github.com/mearvk/SLeeLa.git]  SLEELA_REF [master]
#   CATALINA_BASE / CATALINA_HOME [auto]  TOMCAT_SERVICE [auto]  TOMCAT_PORT [from server.xml, else 8080]
#   APACHE_ETC [/etc/apache2]  APACHE_URL [http://127.0.0.1]  STATIC_DIR [/var/www/sleela-site]
#   BACKUP_ROOT [/backup]  DATE [now, YYYY-MM-DD_HHMMSS]  HEALTH_TIMEOUT [120 seconds]
set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

DRY_RUN=0; DO_TOMCAT=1; DO_APACHE=1; ROLLBACK=1; WAR_FILE=""
while [ $# -gt 0 ]; do
  case "$1" in
    --dry-run) DRY_RUN=1 ;;
    --tomcat-only) DO_APACHE=0 ;;
    --apache-only) DO_TOMCAT=0 ;;
    --no-rollback) ROLLBACK=0 ;;
    --war) shift; WAR_FILE="${1:?--war needs a file}"; [ -e "$WAR_FILE" ] && WAR_FILE="$(cd "$(dirname "$WAR_FILE")" && pwd)/$(basename "$WAR_FILE")" ;;
    -h|--help) sed -n '2,/^set -E/p' "$0" | sed '$d; s/^# \{0,1\}//'; exit 0 ;;
    *) echo "unknown option: $1 (see --help)" >&2; exit 2 ;;
  esac
  shift
done

SLEELA_SRC="${SLEELA_SRC:-/srv/sleela}"
SLEELA_REPO="${SLEELA_REPO:-https://github.com/mearvk/SLeeLa.git}"
SLEELA_REF="${SLEELA_REF:-master}"
APACHE_ETC="${APACHE_ETC:-/etc/apache2}"
APACHE_URL="${APACHE_URL:-http://127.0.0.1}"
STATIC_DIR="${STATIC_DIR:-/var/www/sleela-site}"
BACKUP_ROOT="${BACKUP_ROOT:-/backup}"
DATE="${DATE:-$(date +%Y-%m-%d_%H%M%S)}"
HEALTH_TIMEOUT="${HEALTH_TIMEOUT:-120}"
CONF_NAME="sleela-site"

# ------------------------------------------------------------------ helpers
c_ok=$'\e[32m'; c_warn=$'\e[33m'; c_err=$'\e[31m'; c_off=$'\e[0m'
[ -t 1 ] || { c_ok=""; c_warn=""; c_err=""; c_off=""; }
log()  { printf '%s==>%s %s\n' "$c_ok" "$c_off" "$*"; }
warn() { printf '%sWARN%s %s\n' "$c_warn" "$c_off" "$*" >&2; }
die()  { printf '%sERROR%s %s\n' "$c_err" "$c_off" "$*" >&2; exit 1; }
run()  { if [ "$DRY_RUN" = 1 ]; then printf '   [dry-run]'; printf ' %q' "$@"; echo; else "$@"; fi; }
# write_file PATH < content
write_file() {
  local path="$1" tmp
  if [ "$DRY_RUN" = 1 ]; then echo "   [dry-run] write $path:"; sed 's/^/     | /'; return; fi
  tmp="$(mktemp "$(dirname "$path")/.tmp.XXXXXX")"
  cat > "$tmp"; chmod 644 "$tmp"; mv -f "$tmp" "$path"
}
have() { command -v "$1" >/dev/null 2>&1; }

# read a file from a zip/jar: zip_cat ZIP ENTRY
zip_cat() {
  if have unzip; then unzip -p "$1" "$2" 2>/dev/null
  elif have python3; then python3 - "$1" "$2" <<'PY' 2>/dev/null
import sys, zipfile
with zipfile.ZipFile(sys.argv[1]) as z: sys.stdout.write(z.read(sys.argv[2]).decode('utf-8', 'replace'))
PY
  else return 1; fi
}
zip_has() {
  if have unzip; then unzip -l "$1" "$2" >/dev/null 2>&1
  elif have python3; then python3 -c 'import sys,zipfile; sys.exit(0 if sys.argv[2] in zipfile.ZipFile(sys.argv[1]).namelist() else 1)' "$1" "$2"
  else return 1; fi
}

http_ok() {  # http_ok URL [expected-substring]
  local body
  # -L/-k: a port-80 vhost that redirects to HTTPS still counts, even with a certificate not issued for 127.0.0.1
  body="$(curl -fsSLk -m 5 "$1" 2>/dev/null)" || return 1
  [ -z "${2:-}" ] || [[ "$body" == *"$2"* ]]
}
wait_http() {  # wait_http URL substring label
  local i
  printf '    waiting for %s ' "$1"
  for ((i = 0; i < HEALTH_TIMEOUT; i += 3)); do
    if http_ok "$1" "$2"; then echo "ok"; return 0; fi
    printf '.'; sleep 3
  done
  echo " timed out after ${HEALTH_TIMEOUT}s"; return 1
}

if [ "$DRY_RUN" = 0 ] && [ "$(id -u)" -ne 0 ]; then
  die "run as root (sudo), or use --dry-run to preview"
fi
have curl || die "curl is required"
have git  || die "git is required"

# ------------------------------------------------------------------ 1. detect Tomcat
TOMCAT_REASON=""
detect_tomcat() {
  local d
  if [ -z "${CATALINA_BASE:-}" ]; then
    for d in /var/lib/tomcat11 /var/lib/tomcat10 /opt/tomcat /opt/tomcat11 /opt/tomcat10 /usr/local/tomcat /usr/share/tomcat; do
      if [ -f "$d/conf/server.xml" ] && [ -d "$d/webapps" ]; then CATALINA_BASE="$d"; break; fi
    done
  fi
  [ -n "${CATALINA_BASE:-}" ] && [ -f "$CATALINA_BASE/conf/server.xml" ] || { TOMCAT_REASON="no Tomcat found (set CATALINA_BASE)"; return 1; }
  [ -d "$CATALINA_BASE/webapps" ] || { TOMCAT_REASON="$CATALINA_BASE has no webapps/ directory"; return 1; }

  if [ -z "${CATALINA_HOME:-}" ]; then
    if [ -f "$CATALINA_BASE/lib/catalina.jar" ]; then CATALINA_HOME="$CATALINA_BASE"
    elif [ -f "/usr/share/$(basename "$CATALINA_BASE")/lib/catalina.jar" ]; then CATALINA_HOME="/usr/share/$(basename "$CATALINA_BASE")"
    else CATALINA_HOME="$CATALINA_BASE"; fi
  fi

  TOMCAT_VERSION="$(zip_cat "$CATALINA_HOME/lib/catalina.jar" org/apache/catalina/util/ServerInfo.properties | sed -n 's/^server.number=//p' | tr -d '\r')"
  [ -n "$TOMCAT_VERSION" ] || { TOMCAT_REASON="cannot read Tomcat version from $CATALINA_HOME/lib/catalina.jar"; return 1; }
  local major minor
  major="${TOMCAT_VERSION%%.*}"; minor="$(echo "$TOMCAT_VERSION" | cut -d. -f2)"
  if [ "$major" -lt 10 ] || { [ "$major" -eq 10 ] && [ "$minor" -lt 1 ]; }; then
    TOMCAT_REASON="Tomcat $TOMCAT_VERSION is too old: the site is a Jakarta Servlet 6 app and needs Tomcat 10.1 or newer"
    return 1
  fi

  if [ -z "${TOMCAT_SERVICE:-}" ] && have systemctl; then
    for d in "$(basename "$CATALINA_BASE")" tomcat11 tomcat10 tomcat tomcat10.1; do
      if systemctl cat "$d.service" >/dev/null 2>&1; then TOMCAT_SERVICE="$d"; break; fi
    done
  fi
  TOMCAT_SERVICE="${TOMCAT_SERVICE:-}"

  if [ -z "${TOMCAT_PORT:-}" ]; then
    if have python3; then
      TOMCAT_PORT="$(python3 - "$CATALINA_BASE/conf/server.xml" <<'PY' 2>/dev/null || true
import sys, xml.etree.ElementTree as ET
for c in ET.parse(sys.argv[1]).getroot().iter('Connector'):
    proto = c.get('protocol', 'HTTP/1.1')
    if ('HTTP/1.1' in proto or 'Http11' in proto) and c.get('SSLEnabled', 'false').lower() != 'true':
        print(c.get('port')); break
PY
)"
    else
      TOMCAT_PORT="$(grep -o '<Connector[^>]*port="[0-9]*"[^>]*HTTP/1.1' "$CATALINA_BASE/conf/server.xml" | grep -o 'port="[0-9]*"' | head -1 | tr -dc 0-9)"
    fi
  fi
  TOMCAT_PORT="${TOMCAT_PORT:-8080}"
  TOMCAT_OWNER="$(stat -c %u:%g "$CATALINA_BASE/webapps")"
  return 0
}

# ------------------------------------------------------------------ 1b. detect Apache
APACHE_REASON=""
APACHE_HEADERS=0
detect_apache() {
  [ -d "$APACHE_ETC" ] || { APACHE_REASON="$APACHE_ETC not found (this script supports the Debian/Ubuntu apache2 layout)"; return 1; }
  have apache2ctl && have a2enmod && have a2enconf || { APACHE_REASON="apache2ctl/a2enmod/a2enconf not found"; return 1; }
  [ -f "$APACHE_ETC/mods-available/proxy.load" ] && [ -f "$APACHE_ETC/mods-available/proxy_http.load" ] \
    || { APACHE_REASON="mod_proxy / mod_proxy_http are not installed in $APACHE_ETC/mods-available"; return 1; }
  [ -d "$APACHE_ETC/conf-available" ] || { APACHE_REASON="$APACHE_ETC/conf-available missing"; return 1; }
  [ -f "$APACHE_ETC/mods-available/headers.load" ] && APACHE_HEADERS=1
  mapfile -t DOC_ROOTS < <(grep -Rhoi '^[[:space:]]*DocumentRoot[[:space:]]\+.*' "$APACHE_ETC/apache2.conf" "$APACHE_ETC/sites-enabled/" 2>/dev/null \
    | awk '{print $2}' | tr -d '"' | sort -u)
  return 0
}

TOMCAT_FOUND=0; APACHE_FOUND=0
if [ "$DO_TOMCAT" = 1 ] || [ "$DO_APACHE" = 1 ]; then
  if detect_tomcat; then TOMCAT_FOUND=1; fi
fi
if [ "$DO_APACHE" = 1 ] && detect_apache; then APACHE_FOUND=1; fi

log "Targets"
if [ "$TOMCAT_FOUND" = 1 ]; then
  echo "    Tomcat $TOMCAT_VERSION  base=$CATALINA_BASE  home=$CATALINA_HOME  port=$TOMCAT_PORT  service=${TOMCAT_SERVICE:-<none, relying on autoDeploy>}"
else
  echo "    Tomcat: not usable - $TOMCAT_REASON"
fi
if [ "$DO_APACHE" = 0 ]; then echo "    Apache: skipped (--tomcat-only)"
elif [ "$APACHE_FOUND" = 1 ]; then echo "    Apache: $APACHE_ETC  DocumentRoot(s): ${DOC_ROOTS[*]:-none}  mod_headers: $([ "$APACHE_HEADERS" = 1 ] && echo yes || echo no)"
else echo "    Apache: not usable - $APACHE_REASON"; fi

[ "$DO_TOMCAT" = 0 ] || [ "$TOMCAT_FOUND" = 1 ] || die "cannot deploy to Tomcat: $TOMCAT_REASON"
if [ "$DO_APACHE" = 1 ] && [ "$APACHE_FOUND" = 1 ] && [ "$TOMCAT_FOUND" = 0 ]; then
  die "Apache can only proxy to a Tomcat on this host, and none is usable: $TOMCAT_REASON"
fi

# ------------------------------------------------------------------ 2. backup
BACKUP_DIR="$BACKUP_ROOT/$DATE"
n=1; while [ -e "$BACKUP_DIR" ]; do BACKUP_DIR="$BACKUP_ROOT/$DATE.$n"; n=$((n + 1)); done
log "Backing up the current web roots to $BACKUP_DIR"
run mkdir -p "$BACKUP_DIR"
[ "$DRY_RUN" = 1 ] || chmod 700 "$BACKUP_DIR"
MANIFEST=()
backup_path() {  # backup_path SRC DEST-RELATIVE
  if [ -e "$1" ] || [ -L "$1" ]; then
    run mkdir -p "$(dirname "$BACKUP_DIR/$2")"
    run cp -a "$1" "$BACKUP_DIR/$2"
    MANIFEST+=("$2	$1")
    echo "    saved $1"
  fi
}
if [ "$TOMCAT_FOUND" = 1 ]; then
  backup_path "$CATALINA_BASE/webapps/ROOT"                    tomcat/webapps/ROOT
  backup_path "$CATALINA_BASE/webapps/ROOT.war"                tomcat/webapps/ROOT.war
  backup_path "$CATALINA_BASE/conf/Catalina/localhost/ROOT.xml" tomcat/conf/Catalina/localhost/ROOT.xml
  backup_path "$CATALINA_BASE/conf/server.xml"                 tomcat/conf/server.xml
fi
if [ "$APACHE_FOUND" = 1 ]; then
  backup_path "$APACHE_ETC" apache/etc
  for d in "${DOC_ROOTS[@]}"; do backup_path "$d" "apache/docroot$(echo "$d" | sed 's|/*$||')"; done
  backup_path "$STATIC_DIR" apache/static
fi

{ # restore script with the detected paths baked in
  echo '#!/usr/bin/env bash'
  echo "# Restores what was served before the SLeeLa deploy of $DATE.  usage: $BACKUP_DIR/restore.sh [tomcat|apache|all]"
  echo 'set -euo pipefail'
  echo 'B="$(cd "$(dirname "$0")" && pwd)"; what="${1:-all}"'
  printf 'CATALINA_BASE=%q; TOMCAT_SERVICE=%q; APACHE_ETC=%q; STATIC_DIR=%q; TOMCAT_OWNER=%q\n' \
    "${CATALINA_BASE:-}" "${TOMCAT_SERVICE:-}" "$APACHE_ETC" "$STATIC_DIR" "${TOMCAT_OWNER:-}"
  cat <<'EOS'
if [ "$what" = tomcat ] || [ "$what" = all ]; then
  if [ -n "$CATALINA_BASE" ] && [ -d "$B/tomcat" ]; then
    [ -z "$TOMCAT_SERVICE" ] || systemctl stop "$TOMCAT_SERVICE" || true
    rm -rf "$CATALINA_BASE/webapps/ROOT" "$CATALINA_BASE/webapps/ROOT.war" "$CATALINA_BASE/conf/Catalina/localhost/ROOT.xml"
    [ ! -e "$B/tomcat/webapps/ROOT" ]     || cp -a "$B/tomcat/webapps/ROOT"     "$CATALINA_BASE/webapps/ROOT"
    [ ! -e "$B/tomcat/webapps/ROOT.war" ] || cp -a "$B/tomcat/webapps/ROOT.war" "$CATALINA_BASE/webapps/ROOT.war"
    [ ! -e "$B/tomcat/conf/Catalina/localhost/ROOT.xml" ] || cp -a "$B/tomcat/conf/Catalina/localhost/ROOT.xml" "$CATALINA_BASE/conf/Catalina/localhost/ROOT.xml"
    [ -z "$TOMCAT_SERVICE" ] || systemctl start "$TOMCAT_SERVICE"
    echo "tomcat restored"
  fi
fi
if [ "$what" = apache ] || [ "$what" = all ]; then
  if [ -d "$B/apache/etc" ]; then
    rm -rf "$APACHE_ETC.restore-tmp"; cp -a "$B/apache/etc" "$APACHE_ETC.restore-tmp"
    rm -rf "$APACHE_ETC"; mv "$APACHE_ETC.restore-tmp" "$APACHE_ETC"
    rm -rf "$STATIC_DIR"; [ ! -e "$B/apache/static" ] || cp -a "$B/apache/static" "$STATIC_DIR"
    # apache/docroot/ holds copies of every DocumentRoot as it was; the deploy does not modify them, so nothing to put back.
    apache2ctl configtest && systemctl reload apache2
    echo "apache restored"
  fi
fi
EOS
} | write_file "$BACKUP_DIR/restore.sh"
run chmod 700 "$BACKUP_DIR/restore.sh"
printf '%s\n' "# saved-path<TAB>original-path, deploy $DATE" "${MANIFEST[@]}" | write_file "$BACKUP_DIR/MANIFEST.tsv"
echo "    restore with: $BACKUP_DIR/restore.sh [tomcat|apache|all]"

# ------------------------------------------------------------------ 3. content checkout + WAR
log "SLeeLa content at $SLEELA_SRC"
if [ -d "$SLEELA_SRC/.git" ]; then
  run git -C "$SLEELA_SRC" pull --ff-only
elif [ -d "$SLEELA_SRC/tutorial" ] || [ -d "$SLEELA_SRC/lib" ]; then
  echo "    using existing content (not a git checkout)"
else
  run mkdir -p "$(dirname "$SLEELA_SRC")"
  run git clone --depth 1 --branch "$SLEELA_REF" "$SLEELA_REPO" "$SLEELA_SRC"
  run chmod -R a+rX "$SLEELA_SRC"
fi

if [ -z "$WAR_FILE" ]; then
  log "Building the WAR"
  WAR_FILE="$PROJECT_DIR/target/sleela-site.war"
  if have mvn; then
    run mvn -q -B -f "$PROJECT_DIR/pom.xml" -Dnobundle package
  elif have docker; then
    run docker run --rm -v "$PROJECT_DIR":/src -w /src -v sleela-m2:/root/.m2 maven:3.9-eclipse-temurin-21 mvn -q -B -Dnobundle package
  else
    die "need Maven (mvn, with Java 11+) or Docker to build the WAR, or pass --war FILE"
  fi
fi
if [ "$DRY_RUN" = 0 ]; then
  [ -f "$WAR_FILE" ] || die "WAR not found: $WAR_FILE"
  zip_has "$WAR_FILE" WEB-INF/classes/org/sleela/site/web/HomeServlet.class || die "$WAR_FILE does not look like the SLeeLa site WAR"
  zip_has "$WAR_FILE" assets/css/sleela.css || die "$WAR_FILE has no assets/css/sleela.css"
fi
echo "    WAR: $WAR_FILE"

# ------------------------------------------------------------------ 4. Tomcat
TOMCAT_STATUS="skipped"; APACHE_STATUS="skipped"
deploy_tomcat() {
  log "Deploying to Tomcat ($CATALINA_BASE)"
  local ctxdir="$CATALINA_BASE/conf/Catalina/localhost"
  run mkdir -p "$ctxdir"
  write_file "$ctxdir/ROOT.xml" <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!-- SLeeLa site, deployed $DATE by html-source/deploy/deploy-apache-tomcat.sh. Content is read live from the checkout below. -->
<Context>
  <Parameter name="sleela.home" value="$SLEELA_SRC" override="false"/>
</Context>
EOF
  if [ -n "$TOMCAT_SERVICE" ]; then run systemctl stop "$TOMCAT_SERVICE"; fi
  run rm -rf "$CATALINA_BASE/webapps/ROOT" "$CATALINA_BASE/webapps/ROOT.war"
  run install -m 0644 -o "${TOMCAT_OWNER%:*}" -g "${TOMCAT_OWNER#*:}" "$WAR_FILE" "$CATALINA_BASE/webapps/.ROOT.war.new"
  run mv -f "$CATALINA_BASE/webapps/.ROOT.war.new" "$CATALINA_BASE/webapps/ROOT.war"
  if [ -n "$TOMCAT_SERVICE" ]; then run systemctl start "$TOMCAT_SERVICE"; fi
  [ "$DRY_RUN" = 1 ] && { TOMCAT_STATUS="dry-run"; return 0; }
  if wait_http "http://127.0.0.1:$TOMCAT_PORT/healthz" '"status":"ok"'; then
    TOMCAT_STATUS="ok  http://127.0.0.1:$TOMCAT_PORT/"
    return 0
  fi
  warn "Tomcat did not report healthy. Look at: ${TOMCAT_SERVICE:+journalctl -u $TOMCAT_SERVICE -n 100, }$CATALINA_BASE/logs/"
  { curl -sS -m 5 "http://127.0.0.1:$TOMCAT_PORT/healthz" 2>&1 | head -5; echo; } >&2 || true
  if [ "$ROLLBACK" = 1 ]; then warn "rolling Tomcat back"; "$BACKUP_DIR/restore.sh" tomcat || warn "Tomcat rollback failed; restore manually from $BACKUP_DIR"; fi
  TOMCAT_STATUS="FAILED (rolled back: $([ "$ROLLBACK" = 1 ] && echo yes || echo no))"
  return 1
}

# ------------------------------------------------------------------ 5. Apache
deploy_apache() {
  log "Deploying to Apache ($APACHE_ETC)"
  local mods=(proxy proxy_http)
  if [ "$APACHE_HEADERS" = 1 ]; then mods+=(headers); fi
  run a2enmod -q "${mods[@]}"

  # static files Apache serves itself, taken from the WAR so they match the deployed version
  run rm -rf "$STATIC_DIR.new"
  run mkdir -p "$STATIC_DIR.new"
  if [ "$DRY_RUN" = 0 ]; then
    if have unzip; then unzip -q -o "$WAR_FILE" 'assets/*' favicon.svg -d "$STATIC_DIR.new"
    else python3 - "$WAR_FILE" "$STATIC_DIR.new" <<'PY'
import sys, zipfile
z = zipfile.ZipFile(sys.argv[1])
z.extractall(sys.argv[2], [n for n in z.namelist() if n.startswith('assets/') or n == 'favicon.svg'])
PY
    fi
    chmod -R a+rX "$STATIC_DIR.new"
  else
    echo "   [dry-run] extract assets/* and favicon.svg from the WAR into $STATIC_DIR.new"
  fi
  run rm -rf "$STATIC_DIR"
  run mv "$STATIC_DIR.new" "$STATIC_DIR"

  write_file "$APACHE_ETC/conf-available/$CONF_NAME.conf" <<EOF
# SLeeLa site, deployed $DATE by html-source/deploy/deploy-apache-tomcat.sh
# Serves the site at / on every virtual host: static assets from $STATIC_DIR, everything else from Tomcat.
# Disable with: a2disconf $CONF_NAME && systemctl reload apache2
<IfModule mod_proxy.c>
    ProxyRequests Off
    ProxyPreserveHost On
    ProxyPass        /assets/     !
    ProxyPass        /favicon.svg !
    ProxyPass        /  http://127.0.0.1:$TOMCAT_PORT/ retry=0 timeout=60
    ProxyPassReverse /  http://127.0.0.1:$TOMCAT_PORT/
</IfModule>

Alias /assets/     "$STATIC_DIR/assets/"
Alias /favicon.svg "$STATIC_DIR/favicon.svg"
<Directory "$STATIC_DIR">
    Options -Indexes
    AllowOverride None
    Require all granted
</Directory>

<IfModule mod_headers.c>
    <LocationMatch "^/(assets/|favicon\.svg)">
        Header set Cache-Control "public, max-age=3600"
        Header set X-Content-Type-Options "nosniff"
    </LocationMatch>
</IfModule>
EOF
  run a2enconf -q "$CONF_NAME"
  [ "$DRY_RUN" = 1 ] && { run apache2ctl configtest; run systemctl reload apache2; APACHE_STATUS="dry-run"; return 0; }

  if ! apache2ctl configtest; then
    warn "apache2ctl configtest failed; leaving the running Apache untouched"
    a2disconf -q "$CONF_NAME" || true
    [ "$ROLLBACK" = 1 ] && { "$BACKUP_DIR/restore.sh" apache >/dev/null 2>&1 || warn "Apache rollback failed; restore manually from $BACKUP_DIR"; }
    APACHE_STATUS="FAILED (configtest)"
    return 1
  fi
  systemctl reload apache2
  if wait_http "$APACHE_URL/healthz" '"status":"ok"' && http_ok "$APACHE_URL/assets/css/sleela.css" "--green"; then
    APACHE_STATUS="ok  $APACHE_URL/"
    return 0
  fi
  warn "the site is not reachable through Apache at $APACHE_URL. Look at /var/log/apache2/error.log"
  if [ "$ROLLBACK" = 1 ]; then warn "rolling Apache back"; "$BACKUP_DIR/restore.sh" apache || warn "Apache rollback failed; restore manually from $BACKUP_DIR"; fi
  APACHE_STATUS="FAILED (rolled back: $([ "$ROLLBACK" = 1 ] && echo yes || echo no))"
  return 1
}

rc=0
if [ "$DO_TOMCAT" = 1 ]; then deploy_tomcat || rc=1; fi
if [ "$DO_APACHE" = 1 ]; then
  if [ "$APACHE_FOUND" = 0 ]; then APACHE_STATUS="not deployed: $APACHE_REASON"
  elif [ "$rc" -ne 0 ]; then APACHE_STATUS="not deployed: Tomcat is not serving the site"
  else deploy_apache || rc=1; fi
fi

log "Summary"
echo "    Tomcat : $TOMCAT_STATUS"
echo "    Apache : $APACHE_STATUS"
echo "    Backup : $BACKUP_DIR   (restore: $BACKUP_DIR/restore.sh [tomcat|apache|all])"
[ "$APACHE_STATUS" = "${APACHE_STATUS#ok}" ] || echo "    Note   : $APACHE_ETC/conf-enabled/$CONF_NAME.conf maps / on every Apache virtual host to the site"
echo "    Content: $SLEELA_SRC   (to update: git -C $SLEELA_SRC pull && systemctl restart ${TOMCAT_SERVICE:-<tomcat>})"
exit "$rc"
