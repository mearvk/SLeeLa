#!/usr/bin/env bash
# SleelaTerminal(TM) -- product identity helpers for the SLeeLa terminal.
#
# SleelaTerminal(TM) is a SLeeLa product BUILT ON GNU Bash (free software, GPL,
# (C) Free Software Foundation). These helpers brand the SLeeLa PRODUCT surfaces
# -- the startup banner, the GUI title, the About text, and the installer /
# package name -- with the SleelaTerminal(TM) identity.
#
# Discipline (GPL-compliant):
#   - The SleelaTerminal(TM) product line is printed ALONGSIDE, never instead
#     of, GNU Bash's own version/banner. We query Bash's real --version rather
#     than fabricating one.
#   - We never alter, remove, or reattribute Bash's version string, banner,
#     copyright, or AUTHORS. The engine is credited to the Free Software
#     Foundation under the GPL. See bash/NOTICE and bash/sleelaterminal.h.
#
# This file is a SLeeLa-authored addition; the vendored GNU Bash sources are
# untouched.

# Keep identity in sync with sleelaterminal.h.
: "${SLEELATERMINAL_NAME:=SleelaTerminal}"
: "${SLEELATERMINAL_TRADEMARK:=SleelaTerminal(TM)}"
: "${SLEELATERMINAL_VERSION:=1.0.0}"
: "${SLEELATERMINAL_PACKAGE:=sleelaterminal}"
: "${SLEELATERMINAL_GUI_TITLE:=SleelaTerminal(TM)}"
: "${SLEELATERMINAL_ENGINE_NAME:=GNU Bash}"
: "${SLEELATERMINAL_ENGINE_LICENSE:=GNU General Public License (GPL)}"
: "${SLEELATERMINAL_ENGINE_COPYRIGHT:=(C) Free Software Foundation, Inc.}"

# Report the real GNU Bash engine version, honestly, from Bash itself.
# Falls back to the running shell's $BASH_VERSION if the `bash` binary is not
# on PATH. Never fabricated.
sleelaterminal_engine_version() {
    local v
    if command -v bash >/dev/null 2>&1; then
        v="$(bash --version 2>/dev/null | head -1)"
    fi
    if [ -z "$v" ] && [ -n "$BASH_VERSION" ]; then
        v="GNU bash, version $BASH_VERSION"
    fi
    printf '%s\n' "${v:-GNU Bash (version unavailable)}"
}

# The SleelaTerminal(TM) product version (our layer).
sleelaterminal_version() {
    printf '%s %s\n' "$SLEELATERMINAL_TRADEMARK" "$SLEELATERMINAL_VERSION"
}

# The GUI window / title-bar string for the product.
sleelaterminal_gui_title() {
    printf '%s\n' "$SLEELATERMINAL_GUI_TITLE"
}

# The installer / package identity for the product.
sleelaterminal_package_name() {
    printf '%s\n' "$SLEELATERMINAL_PACKAGE"
}

# The startup banner: the SleelaTerminal(TM) product line printed ALONGSIDE the
# real GNU Bash engine version. This is what a SleelaTerminal(TM) session shows
# on launch; Bash's own banner is not suppressed elsewhere.
sleelaterminal_banner() {
    printf '%s %s\n' "$SLEELATERMINAL_TRADEMARK" "$SLEELATERMINAL_VERSION"
    printf '  powered by %s\n' "$(sleelaterminal_engine_version)"
}

# The About dialog text for the GUI. Brands the product, credits the engine.
sleelaterminal_about() {
    printf '%s %s\n' "$SLEELATERMINAL_TRADEMARK" "$SLEELATERMINAL_VERSION"
    printf 'A SLeeLa product.\n'
    printf '\n'
    printf 'Powered by %s\n' "$SLEELATERMINAL_ENGINE_NAME"
    printf '  %s\n' "$(sleelaterminal_engine_version)"
    printf '  %s\n' "$SLEELATERMINAL_ENGINE_LICENSE"
    printf '  Copyright %s\n' "$SLEELATERMINAL_ENGINE_COPYRIGHT"
    printf '\n'
    printf '%s is free software. The GNU Bash engine retains its own license\n' "$SLEELATERMINAL_ENGINE_NAME"
    printf 'and copyright; see the NOTICE file. SleelaTerminal(TM) branding\n'
    printf 'applies to the SLeeLa-authored product layers only.\n'
}

# Simple CLI: banner (default) | version | engine | gui-title | package | about
sleelaterminal_main() {
    case "${1:-banner}" in
        banner)    sleelaterminal_banner ;;
        version)   sleelaterminal_version ;;
        engine)    sleelaterminal_engine_version ;;
        gui-title) sleelaterminal_gui_title ;;
        package)   sleelaterminal_package_name ;;
        about)     sleelaterminal_about ;;
        *)
            printf 'usage: sleelaterminal.sh {banner|version|engine|gui-title|package|about}\n' >&2
            return 2
            ;;
    esac
}

# Run the CLI only when executed directly, not when sourced.
if [ "${BASH_SOURCE[0]:-$0}" = "$0" ]; then
    sleelaterminal_main "$@"
fi
