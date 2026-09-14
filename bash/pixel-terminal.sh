#!/usr/bin/env bash
# SLeeLa pixel-addressable terminal interface.
# The functions emit a text protocol consumed by the SLeeLa terminal bridge.

: "${SLEELA_PIXEL_TERMINAL:=sleeLa-pixel-terminal}"

pixel_terminal_begin() {
    local width="$1" height="$2"
    printf '%s BEGIN %s %s\n' "$SLEELA_PIXEL_TERMINAL" "$width" "$height"
}

pixel_terminal_set() {
    local x="$1" y="$2" value="$3"
    printf '%s SET %s %s %s\n' "$SLEELA_PIXEL_TERMINAL" "$x" "$y" "$value"
}

pixel_terminal_fill() {
    local value="$1"
    printf '%s FILL %s\n' "$SLEELA_PIXEL_TERMINAL" "$value"
}

pixel_terminal_clear() {
    printf '%s CLEAR\n' "$SLEELA_PIXEL_TERMINAL"
}

pixel_terminal_show() {
    printf '%s SHOW\n' "$SLEELA_PIXEL_TERMINAL"
}

pixel_terminal_end() {
    printf '%s END\n' "$SLEELA_PIXEL_TERMINAL"
}
