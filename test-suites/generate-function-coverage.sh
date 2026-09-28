#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
PY="${PYTHON:-python3}"
if ! command -v "$PY" >/dev/null 2>&1; then echo "SKIP: Python unavailable for function inventory"; exit 0; fi
exec "$PY" "$ROOT/test-suites/generate-function-coverage.py"
