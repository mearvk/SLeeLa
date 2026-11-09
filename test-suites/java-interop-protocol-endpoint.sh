#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d "\${TMPDIR:-/tmp}/sleela-jlip-java.XXXXXX")
trap 'rm -rf "$TMP"' EXIT HUP INT TERM
command -v javac >/dev/null 2>&1 || { echo "SKIP: javac not installed" >&2; exit 0; }
command -v java >/dev/null 2>&1 || { echo "SKIP: java not installed" >&2; exit 0; }
command -v python3 >/dev/null 2>&1 || { echo "SKIP: python3 not installed" >&2; exit 0; }
mkdir -p "$TMP/classes"
javac --release 17 -Xlint:all -d "$TMP/classes" \
  "$ROOT/connector/java/com/mearvk/sleela/connector/SleelaClassLoader.java" \
  "$ROOT/connector/java/com/mearvk/sleela/connector/SleelaJdkBridge.java" \
  "$ROOT/connector/java/com/mearvk/sleela/connector/SleelaJdkProtocolServer.java"
python3 "$ROOT/test-suites/test-java-protocol.py" "$TMP/classes"
