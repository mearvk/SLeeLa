#!/usr/bin/env bash
set -euo pipefail
BIN="${1:?usage: run_semantic_analysis_tests.sh <nordshrift-binary>}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
FIXTURE="${ROOT}/impl/tests/nordshrift/semantic-invalid.sst"
echo "=== Nordshrift semantic-analysis negative test ==="
set +e
OUTPUT="$("${BIN}" check "${FIXTURE}" 2>&1)"
RC=$?
set -e
printf '%s\n' "${OUTPUT}"
if [[ ${RC} -eq 0 ]]; then echo "FAIL: semantically invalid source was accepted"; exit 1; fi
if ! grep -q "NSS-E-SEM-001" <<<"${OUTPUT}"; then echo "FAIL: expected NSS-E-SEM-001 semantic diagnostic"; exit 1; fi
echo "PASS: semantic type/control-flow errors rejected by SST check"
