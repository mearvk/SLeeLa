#!/usr/bin/env bash
set -euo pipefail
SERVER="\${SLEELA_WEB_SERVER:-tomcat}"
PORT="\${SLEELA_WEB_PORT:-8080}"
HEALTH_URL="\${SLEELA_WEB_HEALTH_URL:-/}"
MODULE="\${SLEELA_WEB_MODULE:-}"
die(){ printf 'SLeeLa webctl: ERROR: %s\n' "$*" >&2; exit 1; }
root(){ [[ $EUID -eq 0 ]] || die "administrator privileges required; rerun with sudo"; }
pkg(){ [[ "$SERVER" == apache ]] && printf httpd || printf tomcat; }
prefix(){ brew --prefix "$(pkg)"; }
health(){ command -v curl >/dev/null || die "curl required"; curl --fail --silent --show-error --max-time 10 "http://127.0.0.1:$PORT$HEALTH_URL" >/dev/null; printf 'SLeeLa webctl: HEALTH OK server=%s port=%s path=%s\n' "$SERVER" "$PORT" "$HEALTH_URL"; }
validate(){ if [[ "$SERVER" == apache ]]; then "$(prefix)/bin/apachectl" -t; elif [[ "$SERVER" == tomcat ]]; then "$(prefix)/libexec/bin/catalina.sh" configtest; else die "server must be apache or tomcat"; fi; }
install_server(){ root; command -v brew >/dev/null || die "Homebrew is required"; brew update; brew install "$(pkg)"; }
upgrade_server(){ root; command -v brew >/dev/null || die "Homebrew is required"; brew update; brew upgrade "$(pkg)"; }
status_server(){ brew services list | grep -E '^(httpd|tomcat)[[:space:]]' || true; }
port_server(){ root; [[ "$PORT" =~ ^[0-9]+$ ]] || die "SLEELA_WEB_PORT must be numeric"; if [[ "$SERVER" == apache ]]; then f="$(prefix)/etc/httpd/httpd.conf"; cp -p "$f" "$f.sleela-backup"; sed -i '' -E "s/^[[:space:]]*Listen[[:space:]]+[0-9]+/Listen $PORT/" "$f"; else f="$(prefix)/libexec/conf/server.xml"; cp -p "$f" "$f.sleela-backup"; python3 - "$f" "$PORT" <<'PY'
import re,sys
p,port=sys.argv[1],sys.argv[2]
s=open(p,encoding="utf-8").read()
s,n=re.subn(r'(<Connector\b[^>]*\bport=")[0-9]+(")',r'\g<1>'+port+r'\2',s,count=1)
if n != 1: raise SystemExit("Tomcat Connector port not found")
open(p,"w",encoding="utf-8").write(s)
PY
fi; validate; brew services restart "$(pkg)"; health; }
module_server(){ root; [[ "$SERVER" == tomcat && "$MODULE" == *.war && -f "$MODULE" ]] || die "Tomcat module deployment requires an existing .war"; d="$(prefix)/libexec/webapps"; [[ -d "$d" ]] || die "Tomcat webapps directory not found"; install -m 0644 "$MODULE" "$d/"; brew services restart tomcat; health; }
case "\${1:-help}" in install) install_server;; upgrade) upgrade_server;; status) status_server;; health) health;; validate) validate;; port) port_server;; module) module_server;; *) echo 'Usage: webctl.sh {install|upgrade|status|health|validate|port|module}';; esac
