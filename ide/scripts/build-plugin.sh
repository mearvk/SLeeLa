#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# build-plugin.sh
#
# Builds the installable SLeeLa IntelliJ plugin ZIP.
#
# Produces: sleela-intellij/build/distributions/sleela-intellij-<version>.zip
#
# Requires:
#   - JDK 21 (auto-detected; override with JAVA_HOME).
#   - The Gradle wrapper jar (run gen-gradle-wrapper.sh first, or this script
#     will fall back to a system 'gradle').
#   - NETWORK ACCESS on first run: Gradle downloads the IntelliJ Platform SDK
#     (IC 2026.1), Kotlin, and the IntelliJ Platform Gradle Plugin from
#     JetBrains and Maven Central. This step cannot run fully offline.
#
# Usage:
#   ./build-plugin.sh            # buildPlugin (the installable ZIP)
#   ./build-plugin.sh build      # compile + run unit tests
#   ./build-plugin.sh runIde     # launch a sandbox IDE with the plugin loaded
#   ./build-plugin.sh verifyPlugin
# -----------------------------------------------------------------------------
set -euo pipefail
source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/common.sh"

ensure_jdk21

TASK="${1:-buildPlugin}"
GRADLE="$(gradle_cmd)"

# Preflight note: this task needs the network for the IntelliJ SDK.
warn "Note: '$TASK' downloads the IntelliJ Platform SDK (IC 2026.1) on first run."
warn "      It requires outbound access to JetBrains + Maven Central and cannot run offline."

log "Running: $GRADLE $TASK (in $PLUGIN_DIR)"
# $GRADLE may be "gradle" or "sh /path/gradlew"; leave it unquoted so it splits
# into words. $TASK is a single known token, so this is safe.
( cd "$PLUGIN_DIR" && $GRADLE "$TASK" )

if [ "$TASK" = "buildPlugin" ]; then
  DIST="$PLUGIN_DIR/build/distributions"
  ZIP="$(ls -1 "$DIST"/*.zip 2>/dev/null | head -n1 || true)"
  if [ -n "$ZIP" ]; then
    log "Plugin ZIP: $ZIP ($(wc -c < "$ZIP") bytes)"
    log "Install via IntelliJ: Settings -> Plugins -> gear -> Install Plugin from Disk..."
  else
    warn "buildPlugin finished but no ZIP was found under $DIST"
  fi
fi
