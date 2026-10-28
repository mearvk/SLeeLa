#!/usr/bin/env bash
# =============================================================================
# make-slbuild.sh — compile the SLeeLa OS-specific builder/installer binary.
# Max Rupplin · MEARVK LLC · 2026
#
# Detects the host OS and C toolchain, then compiles build/native-builder/
# slbuild.c into a native `slbuild` binary placed in an OS-specific output
# directory. The produced binary is the compiled front door that runs the real
# SLeeLa build + install (see slbuild.c).
#
#   Linux   -> build/native-builder/bin/linux/slbuild
#   macOS   -> build/native-builder/bin/macos/slbuild
#   *BSD    -> build/native-builder/bin/bsd/slbuild
#   other   -> build/native-builder/bin/posix/slbuild
#
# Usage: build/native-builder/make-slbuild.sh [--cc COMPILER]
# =============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
SRC="$SCRIPT_DIR/slbuild.c"
[ -f "$SRC" ] || { echo "ERROR: $SRC not found" >&2; exit 1; }

# OS detection -> output subdir + any OS-specific flags.
UNAME_S="$(uname -s 2>/dev/null || echo unknown)"
case "$UNAME_S" in
  Linux*)   OSDIR=linux;  OSNAME="Linux"   ;;
  Darwin*)  OSDIR=macos;  OSNAME="macOS"   ;;
  *BSD*)    OSDIR=bsd;    OSNAME="BSD"     ;;
  MINGW*|MSYS*|CYGWIN*) OSDIR=windows; OSNAME="Windows" ;;
  *)        OSDIR=posix;  OSNAME="POSIX"   ;;
esac

# Toolchain: prefer an explicit --cc, else cc, gcc, then clang.
CC="${CC:-}"
while [ $# -gt 0 ]; do
  case "$1" in --cc) CC="$2"; shift 2;; *) echo "unknown arg: $1" >&2; exit 2;; esac
done
if [ -z "$CC" ]; then
  for c in cc gcc clang; do command -v "$c" >/dev/null 2>&1 && { CC="$c"; break; }; done
fi
[ -n "$CC" ] || { echo "ERROR: no C compiler found (tried cc, gcc, clang)." >&2; exit 1; }

OUTDIR="$SCRIPT_DIR/bin/$OSDIR"
OUT="$OUTDIR/slbuild"
mkdir -p "$OUTDIR"

# -Wall -Wextra for real issues; the format/stringop-truncation analyses are
# worst-case-theoretical on our bounded PATH_MAX buffers and only add noise, so
# they are turned off for a clean compile.
CFLAGS="-std=c11 -O2 -Wall -Wextra -Wno-format-truncation -Wno-stringop-truncation"
# macOS: no extra libs needed. Linux/BSD: none needed (pure libc + system()).
echo "slbuild: building for $OSNAME using $CC"
"$CC" $CFLAGS "$SRC" -o "$OUT"
chmod +x "$OUT"

echo "slbuild: built $OUT"
"$OUT" --help >/dev/null 2>&1 && echo "slbuild: smoke check OK (--help)"
echo "slbuild: run it with:  $OUT --help"
