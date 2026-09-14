#!/usr/bin/env sh
# SLeeLa verified build entry point.
# Verification must succeed before any compiler object or executable is built.
set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
IMPL_DIR="$ROOT_DIR/impl"
MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT_DIR/security/sha256-manifest.json}"

if [ ! -f "$MANIFEST" ]; then
    echo "SLeeLa build refused: SHA-256 manifest not found: $MANIFEST" >&2
    echo "Set SLEELA_SHA256_MANIFEST to a trusted manifest." >&2
    exit 1
fi

python3 "$ROOT_DIR/tools/verify-before-execution.py" --manifest "$MANIFEST" --root "$ROOT_DIR"

# Export the resolved (absolute) manifest so the Makefile's own verify-security
# gate and the runtime SHA-256 gate agree on the same trusted manifest.
export SLEELA_SHA256_MANIFEST="$MANIFEST"
exec make -C "$IMPL_DIR" "$@"
