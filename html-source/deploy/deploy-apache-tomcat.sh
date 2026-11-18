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
#      Builds the premium VMs (slvm-1..11) in the checkout so the Runtime page can offer them; a VM that does not build
#      stays disabled on the page and the deploy continues.
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
#   --install-deps   apt-get install bubblewrap, util-linux and python3 (the runtime sandbox needs them),
#                    plus gcc, g++ and make (build-essential) for the premium VMs
#   --no-premium-vms do not build the premium VMs (the Runtime page then offers the Native VM only)
#   --no-runtime     deploy the site with the Runtime page switched off (sleela.runtime.mode=off)
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

DRY_RUN=0; DO_TOMCAT=1; DO_APACHE=1; ROLLBACK=1; WAR_FILE=""; INSTALL_DEPS=0; RUNTIME_MODE=auto; PREMIUM_VMS=1
while [ $# -gt 0 ]; do
  case "$1" in
    --dry-run) DRY_RUN=1 ;;
    --tomcat-only) DO_APACHE=0 ;;
    --apache-only) DO_TOMCAT=0 ;;
    --no-rollback) ROLLBACK=0 ;;
    --install-deps) INSTALL_DEPS=1 ;;
    --no-runtime) RUNTIME_MODE=off ;;
    --no-premium-vms) PREMIUM_VMS=0 ;;
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

# safe_text: filter for anything we print that came from a tool, a log, or a remote page. The result is plain ASCII that
# is safe to paste into a chat or ticket: CR progress redraws become lines, ANSI/OSC escapes and control bytes are dropped,
# non-ASCII bytes are dropped, common secrets are redacted, and lines are capped at 400 characters.
safe_text() {
  LC_ALL=C tr '\r' '\n' \
    | LC_ALL=C sed -E 's/\x1b\[[0-9;?]*[ -\/]*[@-~]//g; s/\x1b\][^\x07\x1b]*(\x07|\x1b\\)?//g' \
    | LC_ALL=C tr -cd '\11\12\40-\176' \
    | LC_ALL=C sed -E 's/(ghp_|gho_|ghs_|github_pat_|glpat-|xox[abp]-|AKIA|ASIA)[A-Za-z0-9_-]{8,}/[redacted]/g;
                       s/((pass(word|wd)?|secret|token|api[_-]?key|authorization|credential)s?["'\'']?[[:space:]]*[=:][[:space:]]*)["'\'']?(Bearer |Basic )?[^[:space:],;"'\'']+["'\'']?/\1[redacted]/Ig' \
    | LC_ALL=C cut -c1-400
}
# tail_safe FILE [LINES]: print the last LINES (default 60) of FILE through safe_text, indented, with a header.
tail_safe() {
  local f="$1" n="${2:-60}"
  [ -r "$f" ] || { echo "    (no readable log at $f)"; return 0; }
  echo "    ---- last $n lines of $f (plain text, secrets redacted) ----"
  tail -n "$n" "$f" | safe_text | sed 's/^/    | /'
  echo "    ---- end of log ----"
}

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
# verify_page URL LABEL: the page must be the SLeeLa site (meta sleela-site), not another app on the same path
verify_page() {  # verify_page URL LABEL [timeout-seconds]
  local body title
  body="$(curl -fsSLk -m "${3:-8}" "$1" 2>/dev/null)" || { warn "$2: no answer from $1"; return 1; }
  if [[ "$body" == *'name="sleela-site"'* ]]; then return 0; fi
  title="$(printf '%s' "$body" | tr '\n' ' ' | sed -n 's/.*<title>\([^<]*\)<\/title>.*/\1/p' | head -c 80 | safe_text)"
  warn "$2 is not the SLeeLa site (page title: '${title:-none}')"
  return 1
}
# check_runtime URL: report whether the Runtime page is live or disabled, with the server's own reason
check_runtime() {
  local body why
  RUNTIME_STATUS="unknown"
  # The first GET of /runtime runs the sandbox self-test synchronously (SHA-256 integrity check of the compiler tree, then a
  # bwrap run), cached for 60 s. That can take well over 8 s, so allow up to 120 s here.
  body="$(curl -fsSLk -m 120 "$1" 2>/dev/null)" || { RUNTIME_STATUS="page did not answer within 120 s (self-test slow or failing; see the Tomcat log)"; warn "Runtime page: $RUNTIME_STATUS"; return 0; }
  if [[ "$body" == *'data-runtime-state="ready"'* ]]; then RUNTIME_STATUS="ready (sandbox self-test passed)"; return 0; fi
  why="$(printf '%s' "$body" | tr '\n' ' ' | sed -n 's/.*The runtime is not available right now.<\/strong>[[:space:]]*<span>\([^<]*\)<\/span>.*/\1/p' | head -c 400 | safe_text | tr '\n' ' ')"
  RUNTIME_STATUS="DISABLED: ${why:-reason unknown}"
  warn "Runtime page is disabled: ${why:-reason unknown}"
  if [[ "$why" == *"namespace"* || "$why" == *"bwrap"* || "$why" == *"self-test"* || "$why" == *"Operation not permitted"* ]]; then
    echo "      Likely causes: unprivileged user namespaces are blocked. Check:" >&2
    echo "        sysctl kernel.unprivileged_userns_clone user.max_user_namespaces kernel.apparmor_restrict_unprivileged_userns" >&2
    echo "        (Ubuntu 24.04: allow bwrap via AppArmor, or sysctl -w kernel.apparmor_restrict_unprivileged_userns=0)" >&2
    echo "        and the Tomcat systemd unit: RestrictNamespaces / NoNewPrivileges / ProtectProc / PrivateUsers must not forbid it." >&2
  fi
}
RUNTIME_STATUS="not checked"

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

# Tomcat contexts that would answer the site's own paths before ROOT does. Tomcat picks the longest matching context path,
# so a webapp named "examples" (Tomcat's sample app) shadows /examples/ of the SLeeLa site.
SITE_PATHS="examples tutorials modules servers api search runtime healthz assets home error"
TC_CONFLICTS=()   # entries: webapps/NAME[.war]  or  conf/Catalina/localhost/NAME.xml  (relative to CATALINA_BASE)
find_tomcat_conflicts() {
  local n f
  for n in $SITE_PATHS; do
    for f in "webapps/$n" "webapps/$n.war" "conf/Catalina/localhost/$n.xml"; do
      if [ -e "$CATALINA_BASE/$f" ] || [ -L "$CATALINA_BASE/$f" ]; then TC_CONFLICTS+=("$f"); fi
    done
  done
  TC_SERVERXML_CONFLICTS=""
  for n in $SITE_PATHS; do
    if grep -Eq "<Context[^>]*path=\"/$n\"" "$CATALINA_BASE/conf/server.xml" 2>/dev/null; then TC_SERVERXML_CONFLICTS="$TC_SERVERXML_CONFLICTS /$n"; fi
  done
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
  APACHE_CONFLICTS="$(grep -RnE "^[[:space:]]*(Alias|AliasMatch|ScriptAlias|ProxyPass|ProxyPassMatch|RedirectMatch|Redirect|<Location(Match)?|<Directory)[[:space:]]+\"?(~[[:space:]]+)?\"?/?\^?/?(examples|tutorials|modules|servers|api|search|runtime|healthz|assets)(/|[[:space:]]|\"|\\$|>)" "$APACHE_ETC/apache2.conf" "$APACHE_ETC/conf-enabled/" "$APACHE_ETC/sites-enabled/" 2>/dev/null | grep -v "$CONF_NAME.conf" || true)"
  mapfile -t DOC_ROOTS < <(grep -Rhoi '^[[:space:]]*DocumentRoot[[:space:]]\+.*' "$APACHE_ETC/apache2.conf" "$APACHE_ETC/sites-enabled/" 2>/dev/null \
    | awk '{print $2}' | tr -d '"' | sort -u)
  return 0
}

TOMCAT_FOUND=0; APACHE_FOUND=0
if [ "$DO_TOMCAT" = 1 ] || [ "$DO_APACHE" = 1 ]; then
  if detect_tomcat; then TOMCAT_FOUND=1; find_tomcat_conflicts; fi
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

if [ "$TOMCAT_FOUND" = 1 ] && [ "${#TC_CONFLICTS[@]}" -gt 0 ]; then
  echo "    Tomcat apps that shadow the site's paths (moved into the backup and undeployed): ${TC_CONFLICTS[*]}"
fi
if [ "$TOMCAT_FOUND" = 1 ] && [ -n "$TC_SERVERXML_CONFLICTS" ]; then
  warn "server.xml declares <Context> for$TC_SERVERXML_CONFLICTS. The script does not edit server.xml; remove those Context elements by hand."
fi
if [ "$APACHE_FOUND" = 1 ] && [ -n "$APACHE_CONFLICTS" ]; then
  warn "Existing Apache directives that touch the site's paths. They may win over the SLeeLa proxy rules; review them:"
  echo "$APACHE_CONFLICTS" | sed 's/^/      /' >&2
fi
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
if [ "$TOMCAT_FOUND" = 1 ]; then
  for f in "${TC_CONFLICTS[@]}"; do backup_path "$CATALINA_BASE/$f" "tomcat/displaced/$f"; done
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
  printf 'DISPLACED=('; for f in "${TC_CONFLICTS[@]:-}"; do [ -z "$f" ] || printf ' %q' "$f"; done; printf ' )\n'
  cat <<'EOS'
if [ "$what" = tomcat ] || [ "$what" = all ]; then
  if [ -n "$CATALINA_BASE" ] && [ -d "$B/tomcat" ]; then
    [ -z "$TOMCAT_SERVICE" ] || systemctl stop "$TOMCAT_SERVICE" || true
    rm -rf "$CATALINA_BASE/webapps/ROOT" "$CATALINA_BASE/webapps/ROOT.war" "$CATALINA_BASE/conf/Catalina/localhost/ROOT.xml"
    [ ! -e "$B/tomcat/webapps/ROOT" ]     || cp -a "$B/tomcat/webapps/ROOT"     "$CATALINA_BASE/webapps/ROOT"
    [ ! -e "$B/tomcat/webapps/ROOT.war" ] || cp -a "$B/tomcat/webapps/ROOT.war" "$CATALINA_BASE/webapps/ROOT.war"
    [ ! -e "$B/tomcat/conf/Catalina/localhost/ROOT.xml" ] || cp -a "$B/tomcat/conf/Catalina/localhost/ROOT.xml" "$CATALINA_BASE/conf/Catalina/localhost/ROOT.xml"
    for rel in "${DISPLACED[@]}"; do
      [ -e "$B/tomcat/displaced/$rel" ] || continue
      mkdir -p "$CATALINA_BASE/$(dirname "$rel")"; rm -rf "${CATALINA_BASE:?}/$rel"; cp -a "$B/tomcat/displaced/$rel" "$CATALINA_BASE/$rel"
    done
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

# ------------------------------------------------------------------ 2b. runtime prerequisites
if [ "$RUNTIME_MODE" = auto ]; then
  missing=()
  have bwrap   || missing+=(bubblewrap)
  have prlimit || missing+=(util-linux)
  have python3 || missing+=(python3)
  if [ "$PREMIUM_VMS" = 1 ]; then { have cc && have c++ && have make && have ar; } || missing+=(build-essential); fi
  if [ "${#missing[@]}" -gt 0 ]; then
    if [ "$INSTALL_DEPS" = 1 ] && have apt-get; then
      log "Installing runtime sandbox packages: ${missing[*]}"
      run env DEBIAN_FRONTEND=noninteractive apt-get install -y "${missing[@]}"
    else
      warn "The Runtime page needs: ${missing[*]}. Install with: sudo apt-get install ${missing[*]}   (or rerun with --install-deps). The site deploys either way; the Runtime stays disabled until they exist."
    fi
  fi
fi

# ------------------------------------------------------------------ 3. content checkout, premium VMs, WAR
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

# Premium VMs for the Runtime page. The sandbox mounts the checkout read-only, so `sleela vm run premium N` cannot build a
# VM during a visitor run; build them here. Only the Core engine archive is needed (not the whole toolchain that the root
# `make premium-vms` builds first). make is incremental, so a redeploy rebuilds only what the pull changed.
PREMIUM_STATUS="not built (--no-premium-vms)"
if [ "$RUNTIME_MODE" = off ]; then
  PREMIUM_STATUS="not built (runtime off)"
elif [ "$PREMIUM_VMS" = 1 ]; then
  log "Building the premium VMs (slvm-1..11)"
  PVM_LOG="$BACKUP_DIR/premium-vms.log"; [ "$DRY_RUN" = 1 ] && PVM_LOG=/dev/stdout
  if ! { have cc && have c++ && have make && have ar; }; then
    PREMIUM_STATUS="not built (needs gcc, g++ and make: rerun with --install-deps)"
    warn "Premium VMs $PREMIUM_STATUS"
  elif ! run env SLEELA_SHA256_MANIFEST="$SLEELA_SRC/security/sha256-manifest.json" make -C "$SLEELA_SRC/impl" -j"$(nproc 2>/dev/null || echo 2)" libslcore >"$PVM_LOG" 2>&1; then
    PREMIUM_STATUS="not built (Core engine build failed, see $PVM_LOG)"
    warn "Premium VMs $PREMIUM_STATUS"
    tail_safe "$PVM_LOG" 40
  else
    built=0; failed=()
    for n in 001 002 003 004 005 006 007 008 009 010 011; do
      if run make -C "$SLEELA_SRC/sleela-virtual-machine/$n" all >>"$PVM_LOG" 2>&1 \
         && run make -C "$SLEELA_SRC/sleela-virtual-machine/$n" test >>"$PVM_LOG" 2>&1; then
        built=$((built + 1))
      else
        failed+=("$n")
        # a VM that fails its selfcheck must not be offered: the site enables a VM when its binary exists
        [ "$DRY_RUN" = 1 ] || rm -f "$SLEELA_SRC/sleela-virtual-machine/$n/vm-build/slvm-${n:1}"
      fi
    done
    run chmod -R a+rX "$SLEELA_SRC/impl/build" "$SLEELA_SRC/sleela-virtual-machine"
    PREMIUM_STATUS="$built of 11 built and selfchecked"
    if [ "${#failed[@]}" -gt 0 ]; then
      PREMIUM_STATUS="$PREMIUM_STATUS (failed: ${failed[*]}, see $PVM_LOG)"
      warn "Premium VMs: $PREMIUM_STATUS"
      tail_safe "$PVM_LOG" 40
    fi
    echo "    premium VMs: $PREMIUM_STATUS"
  fi
fi

if [ -z "$WAR_FILE" ]; then
  log "Building the WAR"
  WAR_FILE="$PROJECT_DIR/target/sleela-site.war"
  WAR_LOG="$BACKUP_DIR/war-build.log"; [ "$DRY_RUN" = 1 ] && WAR_LOG=/dev/stdout
  if have mvn; then
    war_cmd=(mvn -B -ntp -Dstyle.color=never -f "$PROJECT_DIR/pom.xml" -Dnobundle package)
  elif have docker; then
    war_cmd=(docker run --rm -v "$PROJECT_DIR":/src -w /src -v sleela-m2:/root/.m2 maven:3.9-eclipse-temurin-21 mvn -B -ntp -Dstyle.color=never -Dnobundle package)
  else
    die "need Maven (mvn, with Java 11+) or Docker to build the WAR, or pass --war FILE"
  fi
  # the raw build output goes to a log file; the screen gets a plain-text, redacted tail on failure
  if ! run "${war_cmd[@]}" >"$WAR_LOG" 2>&1; then
    tail_safe "$WAR_LOG" 80
    die "WAR build failed (full log: $WAR_LOG)"
  fi
  [ "$DRY_RUN" = 1 ] || echo "    WAR build log: $WAR_LOG"
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
  <Parameter name="sleela.runtime.mode" value="$RUNTIME_MODE" override="false"/>
</Context>
EOF
  if [ -n "$TOMCAT_SERVICE" ]; then run systemctl stop "$TOMCAT_SERVICE"; fi
  for f in "${TC_CONFLICTS[@]}"; do run rm -rf "${CATALINA_BASE:?}/$f"; done
  run rm -rf "$CATALINA_BASE/webapps/ROOT" "$CATALINA_BASE/webapps/ROOT.war"
  run install -m 0644 -o "${TOMCAT_OWNER%:*}" -g "${TOMCAT_OWNER#*:}" "$WAR_FILE" "$CATALINA_BASE/webapps/.ROOT.war.new"
  run mv -f "$CATALINA_BASE/webapps/.ROOT.war.new" "$CATALINA_BASE/webapps/ROOT.war"
  if [ -n "$TOMCAT_SERVICE" ]; then run systemctl start "$TOMCAT_SERVICE"; fi
  [ "$DRY_RUN" = 1 ] && { TOMCAT_STATUS="dry-run"; return 0; }
  if wait_http "http://127.0.0.1:$TOMCAT_PORT/healthz" '"status":"ok"'; then
    # /runtime is checked by check_runtime below: it is optional and slow on its first hit, so it never triggers a rollback
    local bad=0 badpaths="" pth
    for pth in / /examples/; do
      verify_page "http://127.0.0.1:$TOMCAT_PORT$pth" "Tomcat $pth" || { bad=1; badpaths="$badpaths $pth"; }
    done
    if [ "$bad" = 0 ]; then
      TOMCAT_STATUS="ok  http://127.0.0.1:$TOMCAT_PORT/"
      check_runtime "http://127.0.0.1:$TOMCAT_PORT/runtime"
      return 0
    fi
    warn "Tomcat is healthy, but these site paths are not answered by the SLeeLa site:$badpaths"
  else
    warn "Tomcat did not report healthy on /healthz within ${HEALTH_TIMEOUT}s."
  fi
  warn "Tomcat deploy failed. Look at: ${TOMCAT_SERVICE:+journalctl -u $TOMCAT_SERVICE -n 100, }$CATALINA_BASE/logs/"
  { curl -sS -m 5 "http://127.0.0.1:$TOMCAT_PORT/healthz" 2>&1 | head -5 | safe_text; echo; } >&2 || true
  {
    if [ -n "$TOMCAT_SERVICE" ] && have journalctl; then
      echo "    ---- journalctl -u $TOMCAT_SERVICE (last 60 lines, plain text, secrets redacted) ----"
      journalctl -u "$TOMCAT_SERVICE" -n 60 --no-pager 2>&1 | safe_text | sed 's/^/    | /'
    fi
    tcl="$(ls -t "$CATALINA_BASE"/logs/catalina.*.log "$CATALINA_BASE"/logs/catalina.out 2>/dev/null | head -1 || true)"
    [ -z "$tcl" ] || tail_safe "$tcl" 60
  } >&2 || true
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
  if wait_http "$APACHE_URL/healthz" '"status":"ok"' && http_ok "$APACHE_URL/assets/css/sleela.css" "--green" \
     && verify_page "$APACHE_URL/" "Apache /" && verify_page "$APACHE_URL/examples/" "Apache /examples/" && verify_page "$APACHE_URL/runtime" "Apache /runtime"; then
    APACHE_STATUS="ok  $APACHE_URL/"
    return 0
  fi
  if [ -n "${APACHE_CONFLICTS:-}" ]; then warn "These existing Apache directives may be answering the site's paths instead:"; echo "$APACHE_CONFLICTS" | sed 's/^/      /' >&2; fi
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
echo "    Runtime: $RUNTIME_STATUS"
echo "    Premium VMs: $PREMIUM_STATUS"
echo "    Backup : $BACKUP_DIR   (restore: $BACKUP_DIR/restore.sh [tomcat|apache|all])"
[ "$APACHE_STATUS" = "${APACHE_STATUS#ok}" ] || echo "    Note   : $APACHE_ETC/conf-enabled/$CONF_NAME.conf maps / on every Apache virtual host to the site"
echo "    Content: $SLEELA_SRC   (to update: git -C $SLEELA_SRC pull && systemctl restart ${TOMCAT_SERVICE:-<tomcat>})"
exit "$rc"
