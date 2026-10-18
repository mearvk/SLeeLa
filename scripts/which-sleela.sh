#!/usr/bin/env bash
# =============================================================================
# which-sleela.sh -- diagnose why `sleela version` shows the wrong banner.
#
# Since commit "build: make sleela and sleelvac independent binaries", the
# `sleela` and `sleelvac` executables are DIFFERENT files and each has its
# product identity baked in at compile time. So `sleela version` can only show
# the "Sleelvac(TM) ... compiler" banner if the thing you are running as
# `sleela` is not actually the freshly built sleela binary (a stale install
# earlier on PATH, an alias/function, or a symlink/copy pointing at sleelvac).
#
# This script prints the facts that pinpoint it. Run it and read the verdict.
#
#   ./scripts/which-sleela.sh
# =============================================================================
set -u

say() { printf '%s\n' "$*"; }
rule() { say "----------------------------------------------------------------"; }

rule
say "SLeeLa / Sleelvac identity diagnostic"
rule

# 1. How the shell resolves `sleela` (alias / function / which file).
say "[1] 'type -a sleela':"
type -a sleela 2>&1 | sed 's/^/    /'
say ""

SLEELA_CMD="$(command -v sleela 2>/dev/null || true)"
if [ -z "$SLEELA_CMD" ]; then
    say "    sleela is not on PATH. Run the built binary directly:"
    say "      <repo>/impl/build/sleela version"
    exit 0
fi

# 2. Resolve to the real file (follow symlinks) and show link target.
REAL="$(readlink -f "$SLEELA_CMD" 2>/dev/null || echo "$SLEELA_CMD")"
say "[2] command -v sleela -> $SLEELA_CMD"
say "    real file (readlink -f) -> $REAL"
ls -l "$SLEELA_CMD" 2>/dev/null | sed 's/^/    /'
say ""

# 3. Is the resolved file actually the sleelvac binary?
base="$(basename "$REAL")"
if [ "$base" = "sleelvac" ]; then
    say "[VERDICT] Your 'sleela' resolves to a file named 'sleelvac'."
    say "          That is the compiler binary, so you get the compiler banner."
    say "          Fix: remove/replace that entry so 'sleela' points at the"
    say "          sleela binary (or run <repo>/impl/build/sleela directly)."
fi

# 4. Compare sleela vs sleelvac byte-for-byte. After the fix they MUST differ.
SLEELVAC_CMD="$(command -v sleelvac 2>/dev/null || true)"
if [ -n "$SLEELVAC_CMD" ] && command -v sha256sum >/dev/null 2>&1; then
    say "[3] byte identity of the two commands:"
    sha256sum "$SLEELA_CMD" "$SLEELVAC_CMD" 2>/dev/null | sed 's/^/    /'
    hs="$(sha256sum "$SLEELA_CMD" 2>/dev/null | cut -d" " -f1)"
    hv="$(sha256sum "$SLEELVAC_CMD" 2>/dev/null | cut -d" " -f1)"
    if [ -n "$hs" ] && [ "$hs" = "$hv" ]; then
        say "    -> IDENTICAL. You are running a build from BEFORE the fix."
        say "       Rebuild: make -C impl clean && make -C impl  (then reinstall)."
    else
        say "    -> different files (expected after the fix)."
    fi
    say ""
fi

# 5. Authoritative check: run the resolved file with the explicit override.
say "[4] '$REAL version' (what the resolved file reports):"
"$REAL" version 2>/dev/null | grep -v '^\[memory' | sed 's/^/    /'
say ""
say "[5] '$REAL version --runtime' (forces the runtime banner from either binary):"
"$REAL" version --runtime 2>/dev/null | grep -v '^\[memory' | sed 's/^/    /'
rule
