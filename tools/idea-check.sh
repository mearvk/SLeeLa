#!/bin/bash
# tools/idea-check.sh -- build the native sleela toolchain if needed, then
# check and run a SLeeLa source file through it.  Defaults to the Idea(TM)
# System runnable.
#
# Idea System(TM) / Idea(TM).  From Author; distributive.
#
# Usage:
#   tools/idea-check.sh [path/to/program.sleela]
#
# Environment:
#   SLEELA_BIN   use a prebuilt sleela binary instead of building impl/
#                (e.g. SLEELA_BIN=working-binaries/sleela tools/idea-check.sh)
set -e

REPO="$(cd "$(dirname "$0")/.." && pwd)"
TARGET="${1:-$REPO/lib/idea/IdeaSystem.sleela}"

# Pick a sleela binary: explicit override, prebuilt in working-binaries, or
# the freshly built impl artifact (built on demand).
if [ -n "$SLEELA_BIN" ] && [ -x "$SLEELA_BIN" ]; then
    SLEELA="$SLEELA_BIN"
elif [ -x "$REPO/impl/build/sleela" ]; then
    SLEELA="$REPO/impl/build/sleela"
elif [ -x "$REPO/working-binaries/sleela" ]; then
    SLEELA="$REPO/working-binaries/sleela"
else
    echo "idea-check: building native toolchain (impl/) ..."
    make -C "$REPO/impl" all
    SLEELA="$REPO/impl/build/sleela"
fi

echo "idea-check: using $SLEELA"
echo "idea-check: target  $TARGET"
echo "== check =="
"$SLEELA" check "$TARGET"
echo "== run =="
"$SLEELA" run "$TARGET"
