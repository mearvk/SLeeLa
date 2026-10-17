#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../../.." && pwd)"
BIN="${1:-$ROOT/impl/build/sleela}"
cd "$ROOT"
export SLEELA_SHEET="$ROOT/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$ROOT/security/sha256-manifest.json"
python3 tools/verify-before-execution.py --manifest "$SLEELA_SHA256_MANIFEST" --root "$ROOT"
"$BIN" check example/gui-military-command-console.sleela
echo "PASS: source library declarations resolved for the military GUI example"
