#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
OUT="$ROOT/test-suites/logs/runtime-coverage"
mkdir -p "$OUT"
if command -v llvm-cov >/dev/null 2>&1 && command -v llvm-profdata >/dev/null 2>&1; then
  echo "LLVM coverage tooling detected."
  echo "Build selected targets with profile instrumentation, run them, merge profiles, then report with llvm-cov."
  exit 0
fi
if command -v gcov >/dev/null 2>&1; then
  echo "gcov detected. Rebuild selected targets with --coverage and collect reports."
  exit 0
fi
echo "SKIP: no supported runtime coverage toolchain detected."
exit 0
