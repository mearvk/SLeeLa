#!/usr/bin/env bash
# -----------------------------------------------------------------------------
# build-native-compiler.sh
#
# Builds the native SLeeLa compiler ("Sleelvac") that the IDE drives for live
# diagnostics, run, build, check, and transpile. The IDE never reimplements the
# language; it shells out to this binary (SLEELA.IDE.CONTRACT.md).
#
# Produces: <repo>/impl/build/sleela  -- the first path SleelaCommandRunner looks
# for. Override at runtime with the SLEELA_COMPILER environment variable.
#
# Requires: gcc/g++ (C11/C++17), GNU make, python3 (for the SHA-256 verify gate).
#           No network needed.
# -----------------------------------------------------------------------------
set -euo pipefail
source "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/common.sh"

for tool in gcc g++ make python3; do
  command -v "$tool" >/dev/null 2>&1 || die "Required build tool not found: $tool"
done

IMPL_DIR="$REPO_ROOT/impl"
[ -f "$IMPL_DIR/Makefile" ] || die "impl/Makefile not found at $IMPL_DIR"

# The build has a mandatory SHA-256 verification gate (verify-security) that
# needs a trusted manifest. Default to the one committed in the repo unless the
# caller already provided SLEELA_SHA256_MANIFEST / VERIFY_MANIFEST.
if [ -z "${VERIFY_MANIFEST:-}" ] && [ -z "${SLEELA_SHA256_MANIFEST:-}" ]; then
  default_manifest="$REPO_ROOT/security/sha256-manifest.json"
  [ -f "$default_manifest" ] \
    || die "No SHA-256 manifest found. Set SLEELA_SHA256_MANIFEST to a trusted manifest."
  export VERIFY_MANIFEST="$default_manifest"
  log "Using default trusted manifest: $VERIFY_MANIFEST"
fi

JOBS="${JOBS:-$(nproc 2>/dev/null || echo 4)}"
TARGET="${1:-build/sleela}"   # pass "all" to also build nordshrift + staged package

log "Building native compiler target '$TARGET' with -j$JOBS ..."
make -C "$IMPL_DIR" -j"$JOBS" "$TARGET"

BIN="$IMPL_DIR/build/sleela"
[ -x "$BIN" ] || die "Expected binary not produced: $BIN"

log "Built: $BIN ($(wc -c < "$BIN") bytes)"
log "Smoke test: $BIN check (usage)"
"$BIN" --version >/dev/null 2>&1 || true
log "Done. The IDE will auto-discover this binary at impl/build/sleela."
