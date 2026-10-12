#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# build-all.sh
#
# One-shot: produce everything needed for a fully functional SLeeLa IDE.
#   1. Generate the Gradle wrapper jar          (offline)
#   2. Build the native SLeeLa compiler          (offline)
#   3. Build the installable plugin ZIP          (NEEDS NETWORK for the SDK)
#
# Step 3 needs outbound access to JetBrains + Maven Central. If you only want
# the offline-buildable parts, run with SKIP_PLUGIN=1.
# -----------------------------------------------------------------------------
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$HERE/common.sh"

log "== 1/3  Gradle wrapper jar =="
"$HERE/gen-gradle-wrapper.sh"

log "== 2/3  Native SLeeLa compiler =="
"$HERE/build-native-compiler.sh"

if [ "${SKIP_PLUGIN:-0}" = "1" ]; then
  warn "== 3/3  Plugin ZIP skipped (SKIP_PLUGIN=1) =="
  log "Offline steps complete. Run build-plugin.sh when network is available."
  exit 0
fi

log "== 3/3  Plugin ZIP (requires network) =="
"$HERE/build-plugin.sh" buildPlugin

log "All steps complete."
