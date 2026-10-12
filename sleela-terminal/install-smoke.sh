#!/bin/sh
# Staged-install regression test. Uses a private DESTDIR and never writes to
# the host's /usr or /usr/local tree.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
TMP=$(mktemp -d "${TMPDIR:-/tmp}/slsh-install-smoke.XXXXXX")
cleanup() {
  rm -rf "$TMP"
}
trap cleanup EXIT HUP INT TERM

make -C "$ROOT" install DESTDIR="$TMP/stage" PREFIX=/usr
BIN="$TMP/stage/usr/bin/slsh"

test -x "$BIN"
output=$("$BIN" -c 'printf install-ok')
test "$output" = install-ok

make -C "$ROOT" uninstall DESTDIR="$TMP/stage" PREFIX=/usr
test ! -e "$BIN"

echo "Install smoke: PASS"
