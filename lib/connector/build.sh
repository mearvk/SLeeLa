#!/usr/bin/env sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$ROOT"
if ! command -v make >/dev/null 2>&1; then
  echo "Error: make is required. Install GNU Make and a C/C++ toolchain." >&2
  exit 127
fi
TARGET=${1:-all}
case "$TARGET" in
  all|database|data|registry|test|clean|help|check-deps) exec make "$TARGET" ;;
  *) echo "Usage: $0 [all|database|data|registry|test|clean|help|check-deps]" >&2; exit 2 ;;
esac
