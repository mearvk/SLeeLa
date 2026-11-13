#!/usr/bin/env bash
# Run the Java -> SLeeLa converter (lib/java/converter).
#
# usage: JCONV_IN=<java source root> [JCONV_OUT=<dir>] [JCONV_SCOPE=api|all]
#        [JCONV_FILTER=<prefix>] [JCONV_LIMIT=<n>] [JCONV_INT=int32|int]
#        [JCONV_COMMENTS=keep|drop] lib/java/converter/run-converter.sh [path/to/sleela]
#
# The converter is a SLeeLa program; sleela run reads every argument as a source
# file, so settings travel in the environment. Uses the source-built compiler
# (impl/build/sleela) when present, else working-binaries/sleela.
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../../.." && pwd)"
sleela="${1:-}"
if [ -z "$sleela" ]; then
  if [ -x "$root/impl/build/sleela" ]; then sleela="$root/impl/build/sleela"; else sleela="$root/working-binaries/sleela"; fi
fi
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib"
: "${JCONV_IN:?set JCONV_IN to the root of the Java source tree (for example an unpacked JDK src.zip)}"
case "$JCONV_IN" in /*) ;; *) JCONV_IN="$(cd "$JCONV_IN" && pwd)";; esac
export JCONV_IN
if [ -n "${JCONV_OUT:-}" ]; then
  mkdir -p "$JCONV_OUT"
  case "$JCONV_OUT" in /*) ;; *) JCONV_OUT="$(cd "$JCONV_OUT" && pwd)";; esac
  export JCONV_OUT
fi
cd "$here"
# Only the main unit is named: the loader pulls the other JConv* units from /lib by
# name. Naming every file makes each one resolve library units again (lib/java/STATUS.md
# section 6, multi-file run), which duplicates classes such as java.lang.Error.
"$sleela" run JConvMain.sleela 2>&1 |
  grep -v -E '^\[memory-manager\]|^verification|\[security\]|\[defender\]' || true
# the compiler writes keyword-supporter notes next to the sources it runs; keep the tree clean
rm -rf "$here/docs"
