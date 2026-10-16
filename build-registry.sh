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

echo "=========================================="
echo "Sleela Registry build + table materialize"
echo "=========================================="
echo "Registry dir : $REG_DIR"
echo "Output dir   : $OUT_DIR"
echo ""

# 1. build the registry core + reviewer tool
echo "[1/3] building registry core..."
make -C "$REG_DIR" all

TOOL="$REG_DIR/build/sleela-registry"
if [ ! -x "$TOOL" ]; then
    echo "ERROR: registry tool not built at $TOOL" >&2
    exit 1
fi

mkdir -p "$OUT_DIR"
STORE="$OUT_DIR/sleela.sleelareg"
SQL="$OUT_DIR/registry.sql"

# 2. seed the baseline registry table (text store)
echo "[2/3] seeding baseline registry table -> $STORE"
"$TOOL" seed "$STORE" >/dev/null

# 3. export the MySQL table script (if the tool supports export; else via sleela)
echo "[3/3] exporting MySQL table script -> $SQL"
SLEELA="$REPO_ROOT/impl/build/sleela"
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
