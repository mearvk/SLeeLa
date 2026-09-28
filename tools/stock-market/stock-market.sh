#!/usr/bin/env bash
# SLeeLa Stock Market Terminal — clean ANSI market dashboard.
# Uses a simple text/ANSI renderer so it works in ordinary Bash terminals.
# Data may be supplied as: TICKER PRICE CHANGE_PERCENT
# Example: ./stock-market.sh AAPL 332.27 1.75

set -u

RESET='\033[0m'
BOLD='\033[1m'
DIM='\033[2m'
GREEN='\033[32m'
RED='\033[31m'
CYAN='\033[36m'
WHITE='\033[97m'

bar() {
    local pct="$1"
    local width=28
    local n
    n=$(awk -v p="$pct" -v w="$width" 'BEGIN { n=int(p*w/100); if(n<0)n=0; if(n>w)n=w; print n }')
    printf '%*s' "$n" '' | tr ' ' '█'
}

quote() {
    local ticker="$1" price="$2" change="$3"
    local color="$GREEN" sign="+"
    if awk -v c="$change" 'BEGIN {exit !(c < 0)}'; then
        color="$RED"
        sign=""
    fi
    printf "  ${BOLD}${WHITE}%-6s${RESET} %10s  ${color}%s%7.2f%%%s${RESET}\n" "$ticker" "$price" "$sign" "$change" "$RESET"
}

clear_screen() {
    printf '\033[2J\033[H'
}

show_header() {
    printf "${CYAN}${BOLD}╔══════════════════════════════════════════════════════════════╗${RESET}\n"
    printf "${CYAN}${BOLD}║${RESET}                 SLeeLa STOCK MARKET                         ${CYAN}${BOLD}║${RESET}\n"
    printf "${CYAN}${BOLD}║${RESET}              terminal market monitor                       ${CYAN}${BOLD}║${RESET}\n"
    printf "${CYAN}${BOLD}╚══════════════════════════════════════════════════════════════╝${RESET}\n\n"
}

show_dashboard() {
    show_header
    printf "${DIM}  Watchlist                         Session movement${RESET}\n"
    printf "  ────────────────────────────────────────────────────────────\n"
    quote AAPL '$332.27' 1.75
    quote MSFT '$495.63' 0.65
    quote NVDA '$218.29' -0.03
    quote TSLA '$365.44' 0.52
    printf "\n${DIM}  Relative movement${RESET}\n"
    printf "  AAPL  ${GREEN}$(bar 100)${RESET}\n"
    printf "  MSFT  ${GREEN}$(bar 37)${RESET}\n"
    printf "  NVDA  ${RED}$(bar 2)${RESET}\n"
    printf "  TSLA  ${GREEN}$(bar 30)${RESET}\n"
    printf "\n  ${DIM}Latest session snapshot: 11 Sep 2026${RESET}\n"
    printf "  ${BOLD}Mode:${RESET} paper portfolio / market observation\n"
    printf "  ${DIM}No brokerage orders are submitted.${RESET}\n\n"
}

case "${1:-}" in
    --help|-h)
        cat <<'EOF'
SLeeLa Stock Market Terminal

Usage:
  stock-market.sh
  stock-market.sh --watch
  stock-market.sh --help

The default screen is an ANSI terminal dashboard. The --watch mode refreshes
once per 30 seconds. Replace the sample data in this script or connect a
separate permitted market-data adapter to feed fresh values.
EOF
        exit 0
        ;;
    --watch)
        while true; do
            clear_screen
            show_dashboard
            sleep 30
        done
        ;;
    *)
        show_dashboard
        ;;
esac
