#!/usr/bin/env bash
# spark Preonem (SLEELA.syntax section 43) and qualified Java extends.
set -euo pipefail
BIN="${1:-impl/build/sleela}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)"
HERE="$ROOT/impl/tests/java_references/spark"
cd "$ROOT"; BIN="$(cd "$(dirname "$BIN")" && pwd)/$(basename "$BIN")"
export SLEELA_HOME="${SLEELA_HOME:-$ROOT}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
filt() { sed -E -e '/^verification: PASS/d' -e '/^\[security\]/d' -e '/^\[defender\]/d' -e '/^\[memory-manager\]/d'; }
cd "$HERE"
got="$("$BIN" run spark_ok.sleela 2>&1 | filt)"
[[ "$got" == "$(cat spark_ok.expected.txt)" ]] || { printf 'spark_ok failed:\n%s\n' "$got" >&2; exit 1; }
got="$("$BIN" run spark_old.sleela 2>&1 | filt || true)"
[[ "$got" == *"the spark document marker requires #sleela 3.0"* ]] || { printf 'spark_old failed:\n%s\n' "$got" >&2; exit 1; }
echo "spark regression: PASS"
