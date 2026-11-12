#!/usr/bin/env bash
# Sleela.home lineage (SLEELA.syntax section 42): parent, child, parents("---"),
# root, super("constructor"), the -> set operator, Sleela.VM/System/OS, and the
# Security2 boot sequence. Compares exact program output.
set -euo pipefail
BIN="${1:-impl/build/sleela}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
cd "$ROOT"
export SLEELA_HOME="${SLEELA_HOME:-$ROOT}"
export SLEELA_SHEET="${SLEELA_SHEET:-$ROOT/SHEET.sheet}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
run() { "$BIN" run "$1" 2>&1 | sed -E -e '/^verification: PASS/d' -e '/^\[security\]/d' -e '/^\[defender\]/d' -e '/^\[memory-manager\]/d'; }
check() {
  local actual; actual="$(run "$1")"
  if [[ "$actual" != "$2" ]]; then printf '%s failed\nExpected:\n%s\nActual:\n%s\n' "$1" "$2" "$actual" >&2; exit 1; fi
}
check impl/tests/lineage/lineage.sleela $'B\nSleela.System\nB\nC\nD\nD\nSleela.System\nB\nD\nC\nD\nSleela.System\nSleela.System\nD parent:\nSleela.System\nD child:\nC\nC child:\nB\nB child:\nA\nD'
check impl/tests/lineage/boot.sleela $'4\ntrue\ntrue\nP\nSleela.System\nInternational Guidelines 2.0 (none)'
check impl/tests/lineage/jdk28_chain.sleela $'QFilterInputStream\nQInputStream\nQInputStream\nSleela.System\nQFilterInputStream\n0'
printf 'lineage regression: PASS\n'
