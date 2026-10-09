#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-impl/build/sleela}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
cd "$ROOT"
export SLEELA_SHEET="${SLEELA_SHEET:-$ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
actual="$("$BIN" run impl/tests/class_dispatch/class_instance_dispatch.sleela)"
expected=$'1\n2\n0\n1\n2'
if [[ "$actual" != "$expected" ]]; then
  printf 'class dispatch regression failed\nExpected:\n%s\nActual:\n%s\n' "$expected" "$actual" >&2
  exit 1
fi
printf 'class dispatch regression: PASS\n'
