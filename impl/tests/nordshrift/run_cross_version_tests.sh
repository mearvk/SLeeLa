#!/usr/bin/env bash
set -euo pipefail
BIN="${1:?usage: run_cross_version_tests.sh <nordshrift-binary>}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../.." && pwd)"
OLD="${ROOT}/impl/tests/nordshrift/compat-old.sst"
NEW="${ROOT}/impl/tests/nordshrift/compat-new.sst"
CURRENT="${ROOT}/impl/tests/nordshrift/compat-current.sst"

echo "=== Nordshrift cross-version compiler compatibility test ==="
set +e
OLD_OUTPUT="$("${BIN}" check "${OLD}" 2>&1)"
OLD_RC=$?
NEW_OUTPUT="$("${BIN}" check "${NEW}" 2>&1)"
NEW_RC=$?
CURRENT_OUTPUT="$("${BIN}" check "${CURRENT}" 2>&1)"
CURRENT_RC=$?
set -e

printf '%s\n' "--- below supported range ---"
printf '%s\n' "${OLD_OUTPUT}"
printf '%s\n' "--- supported version ---"
printf '%s\n' "${CURRENT_OUTPUT}"
printf '%s\n' "--- above supported range ---"
printf '%s\n' "${NEW_OUTPUT}"

if [[ ${OLD_RC} -eq 0 ]]; then
  echo "FAIL: below-floor syntax 1.2 was accepted"
  exit 1
fi
if ! grep -Eq "TooOld|older than this compiler|NSS-E-SRC-002" <<<"${OLD_OUTPUT}"; then
  echo "FAIL: missing below-floor compatibility diagnostic"
  exit 1
fi
if [[ ${CURRENT_RC} -ne 0 ]]; then
  echo "FAIL: supported syntax 1.3 was rejected"
  exit 1
fi
if ! grep -Eq "COMPAT-CURRENT|1.3" <<<"${CURRENT_OUTPUT}"; then
  echo "FAIL: supported-version fixture produced no expected 1.3 evidence"
  exit 1
fi
if [[ ${NEW_RC} -eq 0 ]]; then
  echo "FAIL: above-ceiling syntax 1.7 was accepted"
  exit 1
fi
if ! grep -Eq "TooNew|exceeds this compiler|NSS-E-SRC-002" <<<"${NEW_OUTPUT}"; then
  echo "FAIL: missing above-ceiling compatibility diagnostic"
  exit 1
fi
echo "PASS: compiler rejects syntax outside supported range 1.3 .. 1.6"
