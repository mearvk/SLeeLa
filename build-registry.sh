#!/bin/bash
# =============================================================================
# build-registry.sh — Build the Sleela Registry and materialize its table.
# =============================================================================
# Idea System(TM) / Idea(TM). From Author; distributive.
#
# Builds the registry C/C++ core (registry/) and then MATERIALIZES the registry
# table a fresh install reviews:
#   - seeds the baseline registry (System / Settings / Features / Artifacts),
#   - writes the portable text store  registry/build/sleela.sleelareg,
#   - exports the MySQL table script  registry/build/registry.sql.
#
# The master build fires this (root Makefile `registry` target, part of `all`),
# so a built tree ships a reviewable registry table out of the box.
#
# Usage:
#   ./build-registry.sh                 # build + seed + export (default dir)
#   REGISTRY_OUT=/path ./build-registry.sh
#   CC=clang CXX=clang++ ./build-registry.sh
#
# Env:
#   CC, CXX        compilers (passed through to make)
#   REGISTRY_OUT   output dir for the materialized table (default registry/build)
# =============================================================================
set -e

REPO_ROOT="$(cd "$(dirname "$0")" && pwd)"
REG_DIR="$REPO_ROOT/registry"
OUT_DIR="${REGISTRY_OUT:-$REG_DIR/build}"
EXE=""
if [ "${OS:-}" = "Windows_NT" ]; then EXE=".exe"; fi

echo "=========================================="
echo "Sleela Registry build + table materialize"
echo "=========================================="
echo "Registry dir : $REG_DIR"
echo "Output dir   : $OUT_DIR"
echo ""

# 1. build the registry core + reviewer tool
echo "[1/4] building registry core..."
"$MAKE_CMD" -C "$REG_DIR" all

# Registry 3.0 record, compatibility migration, and C++ facade regression tests.
echo "[2/4] testing Registry 3.0..."
make -C "$REG_DIR" test

TOOL="$REG_DIR/build/sleela-registry$EXE"
if [ ! -x "$TOOL" ]; then
    echo "ERROR: registry tool not built at $TOOL" >&2
    exit 1
fi

mkdir -p "$OUT_DIR"
STORE="$OUT_DIR/sleela.sleelareg"
SQL="$OUT_DIR/registry.sql"

# 2. seed the baseline registry table (text store)
echo "[3/4] seeding baseline registry table -> $STORE"
"$TOOL" seed "$STORE" >/dev/null

# 3. export the MySQL table script (if the tool supports export; else via sleela)
echo "[4/4] exporting MySQL table script -> $SQL"
SLEELA="$REPO_ROOT/impl/build/sleela$EXE"
if [ -x "$SLEELA" ]; then
    SLEELA_REGISTRY="$STORE" "$SLEELA" registry export "$SQL" >/dev/null 2>&1 \
        && echo "    exported via sleela registry export" \
        || echo "    (sleela registry export unavailable; text store is the table of record)"
else
    echo "    (impl/build/sleela not built yet; text store $STORE is the table of record)"
fi

echo ""
echo "Registry table materialized:"
echo "  text store : $STORE"
[ -f "$SQL" ] && echo "  mysql table: $SQL"
echo ""
echo "Review it:  $TOOL review \"$STORE\""
echo "Done."
