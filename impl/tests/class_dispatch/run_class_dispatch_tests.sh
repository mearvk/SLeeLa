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

# Constructor syntax is parsed today, but argument resolution and constructor
# body execution are not implemented. These programs must fail semantic
# analysis instead of producing objects whose initialization was skipped.
tmp="$(mktemp)"
trap 'rm -f "$tmp"' EXIT
if "$BIN" run impl/tests/class_dispatch/constructor_arguments_rejected.sleela >"$tmp" 2>&1; then
  printf 'constructor argument guard failed: unsupported construction was accepted\n' >&2
  exit 1
fi
if ! grep -F "constructors with arguments are not yet supported" "$tmp" >/dev/null; then
  printf 'constructor argument guard failed: expected diagnostic not found\n' >&2
  cat "$tmp" >&2
  exit 1
fi
if "$BIN" run impl/tests/class_dispatch/explicit_default_constructor_rejected.sleela >"$tmp" 2>&1; then
  printf 'explicit constructor guard failed: constructor body was silently skipped\n' >&2
  exit 1
fi
if ! grep -F "explicit constructors for class" "$tmp" >/dev/null; then
  printf 'explicit constructor guard failed: expected diagnostic not found\n' >&2
  cat "$tmp" >&2
  exit 1
fi
printf 'constructor safety regressions: PASS\n'
