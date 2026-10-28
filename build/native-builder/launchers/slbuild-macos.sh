#!/bin/sh
# =============================================================================
# slbuild-macos.sh — initial macOS launcher for the SLeeLa builder/installer.
# Max Rupplin · MEARVK LLC · 2026
#
# Compiles the native slbuild binary for macOS (via make-slbuild.sh, which uses
# the clang that ships with the Xcode Command Line Tools), then runs it to build
# the core products and install the main package.
#
# Usage:
#   build/native-builder/launchers/slbuild-macos.sh [--produce-only] [slbuild options...]
#
# --produce-only   compile the slbuild binary, print its path, and stop
#                  (do NOT run the build/install). Also honored via the
#                  SLBUILD_PRODUCE_ONLY=1 environment variable.
#
#   slbuild-macos.sh --prefix "$HOME/.local/sleela"
#   slbuild-macos.sh --build-only
#   slbuild-macos.sh --dry-run --plain
#   slbuild-macos.sh --produce-only          # just build the builder/installer
# =============================================================================
set -eu

# --produce-only: compile the binary only, then stop. Accept it in any position
# and strip it from the args forwarded to slbuild.
PRODUCE_ONLY="${SLBUILD_PRODUCE_ONLY:-0}"
_args=""
for a in "$@"; do
    if [ "$a" = "--produce-only" ]; then PRODUCE_ONLY=1; continue; fi
    _args="$_args $a"
done
# shellcheck disable=SC2086
set -- $_args

HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
NB=$(CDPATH= cd -- "$HERE/.." && pwd)
MAKE_SLBUILD="$NB/make-slbuild.sh"
BIN="$NB/bin/macos/slbuild"

[ "$(uname -s 2>/dev/null)" = "Darwin" ] || \
    echo "warning: host does not report as Darwin/macOS; continuing anyway." >&2

have() { command -v "$1" >/dev/null 2>&1; }

# Xcode Command Line Tools provide clang + make. Check and guide if absent.
if ! have clang && ! have cc; then
    echo "ERROR: no C compiler found. Install the Xcode Command Line Tools:" >&2
    echo "         xcode-select --install" >&2
    exit 1
fi
if ! have xcrun; then
    echo "warning: xcrun not found; install the Xcode Command Line Tools" >&2
    echo "         (xcode-select --install) if the compile fails." >&2
fi
have make || { echo "ERROR: make is required (Xcode Command Line Tools)." >&2; exit 1; }
have g++ || have clang++ || echo "warning: no C++ compiler; the SLeeLa core build needs clang++/g++." >&2
have python3 || echo "warning: python3 not found; the SHA-256 verify gate needs it." >&2

# Default deployment target / architecture, overridable from the environment.
export MACOSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET:-11.0}"
echo "== SLeeLa macOS launcher: target macOS ${MACOSX_DEPLOYMENT_TARGET}, arch $(uname -m) =="

[ -f "$MAKE_SLBUILD" ] || { echo "ERROR: $MAKE_SLBUILD not found." >&2; exit 1; }

echo "== SLeeLa macOS launcher: compiling the slbuild binary =="
sh "$MAKE_SLBUILD"

[ -x "$BIN" ] || { echo "ERROR: slbuild binary not produced at $BIN." >&2; exit 1; }

if [ "$PRODUCE_ONLY" = "1" ]; then
    echo "== SLeeLa macOS launcher: builder/installer binary produced =="
    echo "$BIN"
    exit 0
fi

echo "== SLeeLa macOS launcher: running the build + install =="
exec "$BIN" "$@"
