#!/usr/bin/env bash
set -euo pipefail
SERVER="\${SLEELA_WEB_SERVER:-tomcat}"
PORT="\${SLEELA_WEB_PORT:-8080}"
HEALTH_URL="\${SLEELA_WEB_HEALTH_URL:-/}"
MODULE="\${SLEELA_WEB_MODULE:-}"
die(){ printf 'SLeeLa webctl: ERROR: %s\n' "$*" >&2; exit 1; }
need(){ command -v "$1" >/dev/null 2>&1 || die "required command not found: $1"; }
root(){ [[ $EUID -eq 0 ]] || die "administrator privileges required; rerun with sudo"; }
service_name(){ case "$SERVER" in tomcat) printf '%s' "\${SLEELA_TOMCAT_SERVICE:-tomcat10}" ;; apache) printf '%s' "\${SLEELA_APACHE_SERVICE:-apache2}" ;; *) die "server must be apache or tomcat" ;; esac; }
package_name(){ if [[ "$SERVER" == apache ]]; then command -v apt-get >/dev/null 2>&1 && printf '%s' apache2 || printf '%s' httpd; else command -v apt-get >/dev/null 2>&1 && printf '%s' tomcat10 || printf '%s' tomcat; fi; }
install_server(){ root; if command -v apt-get >/dev/null 2>&1; then apt-get update; apt-get install -y "$(package_name)"; elif command -v dnf >/dev/null 2>&1; then dnf install -y "$(package_name)"; elif command -v yum >/dev/null 2>&1; then yum install -y "$(package_name)"; else die "no supported Linux package manager found"; fi; }
upgrade_server(){ root; if command -v apt-get >/dev/null 2>&1; then apt-get update; apt-get install --only-upgrade -y "$(package_name)"; elif command -v dnf >/dev/null 2>&1; then dnf upgrade -y "$(package_name)"; elif command -v yum >/dev/null 2>&1; then yum update -y "$(package_name)"; else die "no supported Linux package manager found"; fi; }
status_server(){ need systemctl; systemctl status "$(service_name)" --no-pager; }
health(){ need curl; curl --fail --silent --show-error --max-time 10 "http://127.0.0.1:$PORT$HEALTH_URL" >/dev/null; printf 'SLeeLa webctl: HEALTH OK server=%s port=%s path=%s\n' "$SERVER" "$PORT" "$HEALTH_URL"; }
apache_port(){ root; local f="/etc/apache2/ports.conf"; [[ -f "$f" ]] || f="/etc/httpd/conf/httpd.conf"; [[ -f "$f" ]] || die "Apache configuration not found"; cp -p "$f" "$f.sleela-backup"; sed -i -E "s/^[[:space:]]*Listen[[:space:]]+[0-9]+/Listen $PORT/" "$f"; }
tomcat_port(){ root; local f="/etc/tomcat10/server.xml"; [[ -f "$f" ]] || f="/etc/tomcat/server.xml"; [[ -f "$f" ]] || die "Tomcat server.xml not found"; cp -p "$f" "$f.sleela-backup"; python3 - "$f" "$PORT" <<'PY'
import re, sys
path, port = sys.argv[1], sys.argv[2]
data = open(path, encoding="utf-8").read()
new, n = re.subn(r'(<Connector\b[^>]*\bport=")[0-9]+(")', r'\g<1>' + port + r'\2', data, count=1)
if n != 1: raise SystemExit("could not locate exactly one Tomcat Connector port")
open(path, "w", encoding="utf-8").write(new)
PY
}
validate(){ case "$SERVER" in apache) need apachectl; apachectl configtest ;; tomcat) local base="/usr/share/tomcat10"; [[ -x "$base/bin/catalina.sh" ]] || base="/usr/share/tomcat"; [[ -x "$base/bin/catalina.sh" ]] || die "Tomcat catalina.sh not found"; "$base/bin/catalina.sh" configtest ;; *) die "server must be apache or tomcat" ;; esac; }
change_port(){ [[ "$PORT" =~ ^[0-9]+$ ]] || die "SLEELA_WEB_PORT must be numeric"; (( PORT >= 1 && PORT <= 65535 )) || die "port must be 1..65535"; [[ "$SERVER" == apache ]] && apache_port || tomcat_port; validate; need systemctl; systemctl restart "$(service_name)"; health; }
deploy_module(){ root; [[ -n "$MODULE" && -f "$MODULE" ]] || die "SLEELA_WEB_MODULE must name an existing local file"; [[ "$SERVER" == tomcat ]] || die "Apache module deployment is distribution-specific; use an explicit Apache adapter"; [[ "$MODULE" == *.war ]] || die "Tomcat deployment requires a .war file"; local webapps="/var/lib/tomcat10/webapps"; [[ -d "$webapps" ]] || webapps="/var/lib/tomcat/webapps"; [[ -d "$webapps" ]] || die "Tomcat webapps directory not found"; install -m 0644 "$MODULE" "$webapps/"; systemctl restart "$(service_name)"; health; }
case "\${1:-help}" in install) install_server ;; upgrade) upgrade_server ;; status) status_server ;; health) health ;; validate) validate ;; port) change_port ;; module) deploy_module ;; help|*) printf '%s\n' 'Usage: webctl.sh {install|upgrade|status|health|validate|port|module}';; esac
