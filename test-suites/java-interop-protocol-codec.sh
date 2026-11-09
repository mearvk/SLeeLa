#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d "${TMPDIR:-/tmp}/sleela-jlip-c.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
CC=${CC:-cc}
command -v "$CC" >/dev/null 2>&1 || { echo "SKIP: C compiler not installed" >&2; exit 0; }
"$CC" -std=c11 -Wall -Wextra -Werror -pedantic -I"$ROOT/runtime" \
  "$ROOT/runtime/java_interop_protocol.c" \
  "$ROOT/test-suites/test-java-protocol-codec.c" -o "$TMP/test-jlip"
"$TMP/test-jlip"
