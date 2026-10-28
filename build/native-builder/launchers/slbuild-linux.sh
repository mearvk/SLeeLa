#!/bin/sh
# =============================================================================
# slbuild-linux.sh — initial Linux launcher for the SLeeLa builder/installer.
# Max Rupplin · MEARVK LLC · 2026
#
# Compiles the native slbuild binary for Linux (via make-slbuild.sh), then runs
# it to build the core products and install the main package.
#
# Usage:
#   build/native-builder/launchers/slbuild-linux.sh [slbuild options...]
#
# Any extra arguments are passed through to slbuild, e.g.:
#   slbuild-linux.sh --prefix "$HOME/.local/sleela"
#   slbuild-linux.sh --build-only
#   slbuild-linux.sh --dry-run --plain
# =============================================================================
set -eu

HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)       # .../build/native-builder/launchers
NB=$(CDPATH= cd -- "$HERE/.." && pwd)                   # .../build/native-builder
MAKE_SLBUILD="$NB/make-slbuild.sh"
BIN="$NB/bin/linux/slbuild"

[ "$(uname -s 2>/dev/null)" = "Linux" ] || \
    echo "warning: host does not report as Linux; continuing anyway." >&2

# Toolchain sanity (the binary itself needs a C compiler to build; the SLeeLa
# core build needs g++, make, python3 and X11/fontconfig/freetype dev libs).
have() { command -v "$1" >/dev/null 2>&1; }
have cc || have gcc || have clang || { echo "ERROR: no C compiler (cc/gcc/clang) on PATH." >&2; exit 1; }
have make || { echo "ERROR: GNU make is required." >&2; exit 1; }
have g++  || echo "warning: g++ not found; the SLeeLa core build will fail without it." >&2
have python3 || echo "warning: python3 not found; the SHA-256 verify gate needs it." >&2

[ -f "$MAKE_SLBUILD" ] || { echo "ERROR: $MAKE_SLBUILD not found." >&2; exit 1; }

echo "== SLeeLa Linux launcher: compiling the slbuild binary =="
sh "$MAKE_SLBUILD"

[ -x "$BIN" ] || { echo "ERROR: slbuild binary not produced at $BIN." >&2; exit 1; }

echo "== SLeeLa Linux launcher: running the build + install =="
exec "$BIN" "$@"
