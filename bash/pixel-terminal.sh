#!/usr/bin/env bash
# Phraign™ — SLeeLa's frame-based, pixel-accurate terminal control system.
# This is the Phraign™ pixel-addressable terminal shell interface.
# The functions emit a text protocol consumed by the SLeeLa terminal bridge.
# See PHRAIGN.md for the system overview.

: "${SLEELA_PIXEL_TERMINAL:=sleeLa-pixel-terminal}"
: "${SLEELA_BASH_PROTOCOL:=1}"
: "${SLEELA_PHRAIGN_VERSION:=1}"
: "${SLEELA_PHRAIGN_VARIANT:=NATIVE}"

# Establish that this Bash session understands the versioned native Phraign™
# terminal protocol. SLeeLa may use READY/CAPS before sending frame commands.
pixel_terminal_handshake() {
    printf '%s HELLO SLEELA-BASH/%s PHRAIGN/%s VARIANT=%s\n' \
        "$SLEELA_PIXEL_TERMINAL" "$SLEELA_BASH_PROTOCOL" \
        "$SLEELA_PHRAIGN_VERSION" "$SLEELA_PHRAIGN_VARIANT"
    printf '%s READY SLEELA-BASH/%s PHRAIGN/%s VARIANT=%s\n' \
        "$SLEELA_PIXEL_TERMINAL" "$SLEELA_BASH_PROTOCOL" \
        "$SLEELA_PHRAIGN_VERSION" "$SLEELA_PHRAIGN_VARIANT"
    printf '%s CAPS PIXEL_GRANULARITY NATIVE_FRAME RESIZE_EVENTS\n' \
        "$SLEELA_PIXEL_TERMINAL"
}

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

# Report character-cell dimensions. This is always available on a terminal
# with a usable tty, and is used as the portability fallback for pixel size.
pixel_terminal_cell_size() {
    local rows cols
    if read -r rows cols < <(stty size 2>/dev/null); then
        printf '%s CELL_SIZE %s %s\n' "$SLEELA_PIXEL_TERMINAL" "$cols" "$rows"
        return 0
    fi
    return 1
}

# Request the terminal's physical pixel dimensions using the common xterm
# window-size report (CSI 14 t). The terminal response is expected as
# CSI 4 ; height ; width t. Not every terminal supports this query.
pixel_terminal_pixel_size() {
    local response height width old_stty
    old_stty="$(stty -g 2>/dev/null)" || return 1

    stty -echo -icanon min 0 time 5 2>/dev/null || return 1
    printf '\033[14t' > /dev/tty 2>/dev/null || {
        stty "$old_stty" 2>/dev/null
        return 1
    }
    IFS= read -r -d 't' response < /dev/tty 2>/dev/null || true
    stty "$old_stty" 2>/dev/null || true

    if [[ "$response" =~ $'\033\[4;([0-9]+);([0-9]+)' ]]; then
        height="${BASH_REMATCH[1]}"
        width="${BASH_REMATCH[2]}"
        printf '%s PIXEL_SIZE %s %s\n' "$SLEELA_PIXEL_TERMINAL" "$width" "$height"
        return 0
    fi
    return 1
}

# Emit the best available current dimensions and derive the frame center.
# Pixel size is authoritative when supported; cell size remains available as
# a fallback and is explicitly labeled so the bridge can distinguish them.
pixel_terminal_size() {
    local pixel_report center_x center_y width height
    if pixel_report="$(pixel_terminal_pixel_size 2>/dev/null)"; then
        printf '%s\n' "$pixel_report"
        read -r _ _ width height <<< "$pixel_report"
        center_x=$((width / 2))
        center_y=$((height / 2))
        printf '%s CENTER %s %s\n' "$SLEELA_PIXEL_TERMINAL" "$center_x" "$center_y"
        return 0
    fi

    pixel_terminal_cell_size || return 1
    return 0
}

# Capture terminal resize events. Bash receives SIGWINCH when the terminal
# window changes size. The handler refreshes the frame dimensions and center.
pixel_terminal_watch_resize() {
    trap 'pixel_terminal_size' WINCH
    pixel_terminal_size
}

pixel_terminal_unwatch_resize() {
    trap - WINCH
}
