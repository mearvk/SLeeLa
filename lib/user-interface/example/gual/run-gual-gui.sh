#!/bin/sh
# Cross-platform POSIX launcher for the GUAL GUI example (Linux/macOS).
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
cd "$ROOT"
SLEELA="${SLEELA_BIN:-}"
if [ -z "$SLEELA" ]; then
  for candidate in "$ROOT/impl/build/sleela" "$ROOT/impl/build/sleela.exe" "$ROOT/bin/sleela"; do
    if [ -x "$candidate" ]; then SLEELA="$candidate"; break; fi
  done
fi
if [ -z "$SLEELA" ] || [ ! -x "$SLEELA" ]; then
  echo "SLeeLa executable not found. Build it or set SLEELA_BIN." >&2
  exit 1
fi
export SLEELA_SHEET="${SLEELA_SHEET:-$ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
exec "$SLEELA" run "example/gual/gui-gual-settings-console.sleela" "$@"
