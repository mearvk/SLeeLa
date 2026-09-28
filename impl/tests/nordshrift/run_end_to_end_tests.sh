#!/usr/bin/env bash
set -euo pipefail
NS_BIN="${1:?usage: run_end_to_end_tests.sh <nordshrift> <sleela>}"
SLEELA_BIN="${2:?usage: run_end_to_end_tests.sh <nordshrift> <sleela>}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
SST="${ROOT}/impl/tests/nordshrift/e2e.sst"
ARTIFACT="${ROOT}/impl/tests/nordshrift/src/build/e2e.sleela"
LOG="${ROOT}/impl/tests/nordshrift/e2e-runtime.log"
trap 'rm -f "${ARTIFACT}" "${LOG}" "${ARTIFACT}.ledger" "${ARTIFACT}.qr.svg"' EXIT

echo "=== Nordshrift end-to-end SST compilation/execution test ==="
rm -f "${ARTIFACT}" "${ARTIFACT}.ledger" "${ARTIFACT}.qr.svg"
"${NS_BIN}" check "${SST}"
"${NS_BIN}" build "${SST}"
test -s "${ARTIFACT}"
"${SLEELA_BIN}" validate-artifact "${ARTIFACT}"
if ! "${SLEELA_BIN}" run "${ARTIFACT}" >"${LOG}" 2>&1; then
  cat "${LOG}"
  echo "FAIL: generated runnable artifact did not execute"
  exit 1
fi
cat "${LOG}"
grep -q "NORDSHRIFT-E2E-OK" "${LOG}"
echo "PASS: SST -> Sleela source -> runnable artifact -> runtime execution"
