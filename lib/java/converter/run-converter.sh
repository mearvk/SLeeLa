#!/usr/bin/env bash
# Run the Java -> SLeeLa converter (lib/java/converter).
#
# usage: JCONV_IN=<java source root> [JCONV_OUT=<dir>] [JCONV_SCOPE=api|all]
#        [JCONV_FILTER=<prefix>] [JCONV_LIMIT=<n>] [JCONV_INT=int32|int]
#        [JCONV_COMMENTS=keep|drop] [JCONV_BUNDLE=0|1] lib/java/converter/run-converter.sh [path/to/sleela]
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
# default output: ./jdk28-converted of the caller (resolved before the cd below)
JCONV_OUT="${JCONV_OUT:-$PWD/jdk28-converted}"
mkdir -p "$JCONV_OUT"
case "$JCONV_OUT" in /*) ;; *) JCONV_OUT="$(cd "$JCONV_OUT" && pwd)";; esac
export JCONV_OUT
cd "$here"
# Only the main unit is named: the loader pulls the other JConv* units from /lib by
# name. Naming every file makes each one resolve library units again (lib/java/STATUS.md
# section 6, multi-file run), which duplicates classes such as java.lang.Error.
#
# Per-file isolation. A VM runtime error cannot be caught by the program, so a
# converter fault in one Java file would end the whole run. JConvMain writes the
# file it is on to $JCONV_OUT/.jconv-progress (DONE when it finishes) and saves its
# totals after every file. When the VM stops inside a file, this loop restarts the
# converter with JCONV_RESUME_AFTER=<that file>: it is reported as failed, with the
# VM message, and the run continues with the next file.
unset JCONV_RESUME_AFTER JCONV_FAILED_WHY
rm -f "$JCONV_OUT/.jconv-progress" "$JCONV_OUT/.jconv-state"
log="$(mktemp)"
restarts=0
while :; do
  "$sleela" run JConvMain.sleela 2>&1 |
    grep -v -E '^\[memory-manager\]|^verification|\[security\]|\[defender\]' | tee "$log" || true
  prog="$(head -n 1 "$JCONV_OUT/.jconv-progress" 2>/dev/null || true)"
  if [ -z "$prog" ] || [ "$prog" = "DONE" ]; then break; fi
  if [ "$prog" = "${JCONV_RESUME_AFTER:-}" ]; then
    echo "jconv: the resumed run stopped before reaching a new file; giving up" >&2
    break
  fi
  restarts=$((restarts + 1))
  if [ "$restarts" -gt 10000 ]; then echo "jconv: more than 10000 restarts; giving up" >&2; break; fi
  why="$(grep -m 1 -E 'runtime error|error:' "$log" | sed -e 's/^sleelvac: //' | tr '\t' ' ' || true)"
  [ -n "$why" ] || why="the VM stopped"
  echo "jconv: the VM stopped in $prog ($why); restarting after it"
  export JCONV_RESUME_AFTER="$prog" JCONV_FAILED_WHY="VM stopped: $why"
done
rm -f "$log" "$JCONV_OUT/.jconv-state"
# a finished run leaves no marker; a run that gave up keeps it, naming the file it stopped in
[ "$prog" = "DONE" ] && rm -f "$JCONV_OUT/.jconv-progress"
# the compiler writes keyword-supporter notes next to the sources it runs; keep the tree clean
rm -rf "$here/docs"
