#!/usr/bin/env bash
# Regression test: static field initialisers run for non-entry classes too.
set -euo pipefail
BIN="${1:-impl/build/sleela}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
cd "$ROOT"
export SLEELA_SHEET="${SLEELA_SHEET:-$ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"

# Filter the SHA-gate / memory-manager banner lines; keep only program output.
raw="$("$BIN" run impl/tests/static_init/static_init_nonentry.sleela 2>&1)"
actual="$(printf '%s\n' "$raw" | grep -vE '^\[(memory-manager|security|defender)\]|^verification|SHA-256|^sleelvac|execution/diagnostics' || true)"
expected=$'7\n11'
if [[ "$actual" != "$expected" ]]; then
  printf 'static-init regression FAILED\nExpected:\n%s\nActual:\n%s\n' "$expected" "$actual" >&2
  exit 1
fi
printf 'static-init regression: PASS\n'
