#!/usr/bin/env bash
# Regression: one unsupported member must not discard supported siblings.
set -euo pipefail
here="$(cd "$(dirname "$0")/.." && pwd)"
root="$(cd "$here/../../.." && pwd)"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
export JCONV_IN="$here/tests/fixtures"
export JCONV_OUT="$tmp/published"
export JCONV_SCOPE=all
export JCONV_BATCH=20
export JCONV_REVISION=per-member-stub-policy-test
bin="${SLEELA_BIN:-$root/impl/build/sleela}"
"$here/run-converter.sh" "$bin" >"$tmp/run.log" 2>&1
out="$JCONV_OUT/PerMemberStubPolicy.sleela"
[ -s "$out" ] || { cat "$tmp/run.log"; echo "per-member-stub: converted type missing" >&2; exit 1; }
grep -q 'supported(int value)' "$out"
grep -q 'unsupported(int value)' "$out"
grep -q 'UnsupportedOperationException' "$out"
awk -F '\t' '$1 == "converted" && $6 + 0 > 0 { found=1 } END { exit !found }' "$JCONV_OUT/CONVERSION-REPORT.tsv"
echo "per-member-stub: PASS (supported sibling retained; unsupported member fails loudly; report records stubs)"
