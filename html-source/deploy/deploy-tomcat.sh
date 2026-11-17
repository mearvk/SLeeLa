#!/usr/bin/env bash
# Deploy the self-contained WAR (content bundled) to an existing Tomcat 10.1+ / 11.
#   CATALINA_BASE=/opt/tomcat ./deploy/deploy-tomcat.sh
set -euo pipefail
cd "$(dirname "$0")/.."
: "${CATALINA_BASE:?set CATALINA_BASE to your Tomcat directory}"
mvn -q -B package
cp target/sleela-site.war "$CATALINA_BASE/webapps/ROOT.war.tmp"
rm -rf "$CATALINA_BASE/webapps/ROOT"
mv "$CATALINA_BASE/webapps/ROOT.war.tmp" "$CATALINA_BASE/webapps/ROOT.war"
echo "deployed to $CATALINA_BASE/webapps/ROOT.war (Tomcat auto-deploys; check /healthz)"
