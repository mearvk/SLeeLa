#!/usr/bin/env bash
set -euo pipefail

BIN="${1:?usage: run_compiler_resolution_tests.sh <nordshrift-binary>}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
FIXTURE="${ROOT}/impl/tests/nordshrift/invalid-source.sst"

echo "=== Nordshrift source-validation negative test ==="
set +e
OUTPUT="$("${BIN}" check "${FIXTURE}" 2>&1)"
RC=$?
set -e

printf '%s\n' "${OUTPUT}"

if [[ ${RC} -eq 0 ]]; then
  echo "FAIL: invalid source was accepted"
  exit 1
fi

if ! grep -q "NSS-E-SRC-003" <<<"${OUTPUT}"; then
  echo "FAIL: expected NSS-E-SRC-003 source-parse diagnostic"
  exit 1
fi

echo "PASS: invalid source rejected by SST check"
