#!/usr/bin/env bash
# =============================================================================
# SLeeLa Clean Install (macOS) — the calm, beautiful guided installer for macOS.
# Max Rupplin · MEARVK LLC · 2026
#
# This is the macOS edition of the advanced ("beautiful") build. It is a
# faithful port of install/sleela-clean-install.sh (the Linux edition), made
# safe for the stock macOS shell: Apple ships /bin/bash 3.2, which lacks the
# bash-4 features `${var,,}` (lowercase expansion) and `mapfile`/`readarray`.
# Those are replaced here with portable equivalents (a tr-based lowercase and a
# while-read array build). Everything else — the four-block, yes/no guided
# selection, the integrity-gated core build, staging, and the Record of Build —
# mirrors the Linux edition exactly.
#
# WHAT IT INSTALLS / NEVER INSTALLS, and the EXPERIENCE: identical to the Linux
# edition (see install/sleela-clean-install.sh). Run it directly, or let the
# root `make build` / `advanced-build` select it automatically on macOS.
# =============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

PRODUCT="SLeeLa"
RELEASE="0.3.30-dev"
SYNTAX_RANGE="1.0 - 2.8"
TAGLINE="THE NOW. THE INGREDIENT."

# Portable lowercase (bash 3.2 has no ${var,,}).
lc() { printf '%s' "$1" | tr '[:upper:]' '[:lower:]'; }

# ---------------------------------------------------------------------------
# Presentation layer: color, glyphs, rules, boxes. All optional and safe.
# ---------------------------------------------------------------------------
USE_STYLE=1
for arg in "$@"; do [ "$arg" = "--plain" ] && USE_STYLE=0; done
if [ -n "${NO_COLOR:-}" ] || [ ! -t 1 ]; then USE_STYLE=0; fi

if [ "$USE_STYLE" -eq 1 ]; then
  ESC=$(printf '\033')
  RESET="${ESC}[0m";  BOLD="${ESC}[1m";  DIM="${ESC}[2m";  ITAL="${ESC}[3m"
  GREEN="${ESC}[38;5;43m"
  GREENB="${ESC}[1;38;5;43m"
  WHITE="${ESC}[38;5;231m"
  GREY="${ESC}[38;5;245m"
  RED="${ESC}[38;5;203m"
  AMBER="${ESC}[38;5;215m"
else
  ESC=""; RESET=""; BOLD=""; DIM=""; ITAL=""; GREEN=""; GREENB=""; WHITE=""; GREY=""; RED=""; AMBER=""
fi

G_OK="[✓]"; G_RUN="[»]"; G_WARN="[!]"; G_ERR="[x]"; G_DOT="·"
if [ "$USE_STYLE" -eq 0 ]; then G_OK="[ok]"; G_RUN="[..]"; G_WARN="[!]"; G_ERR="[x]"; G_DOT="-"; fi

rule() {
  local w="${COLUMNS:-72}"; [ "$w" -gt 78 ] && w=78
  local ch='-'; [ "$USE_STYLE" -eq 1 ] && ch='─'
  local line; line="$(printf '%*s' "$w" '' | tr ' ' "$ch")"
  printf "%s%s%s\n" "$GREY" "$line" "$RESET"
}
say()   { printf "%s\n" "$*"; }
step()  { printf "  %s%s%s %s\n" "$GREEN" "$G_RUN" "$RESET" "$*"; }
ok()    { printf "  %s%s%s %s\n" "$GREEN" "$G_OK" "$RESET" "$*"; }
warn()  { printf "  %s%s%s %s\n" "$AMBER" "$G_WARN" "$RESET" "$*"; }
die()   { printf "  %s%s %s%s\n" "$RED" "$G_ERR" "$*" "$RESET" >&2; exit 1; }
item()  { printf "    %s%s%s %s\n" "$GREEN" "$G_DOT" "$RESET" "$*"; }
skip()  { printf "    %s%s %s%s\n" "$GREY" "$G_DOT" "$*" "$RESET"; }

banner() {
  local IW=55
  local tl tr bl br h v
  if [ "$USE_STYLE" -eq 1 ]; then tl='╭'; tr='╮'; bl='╰'; br='╯'; h='─'; v='│'
  else tl='+'; tr='+'; bl='+'; br='+'; h='-'; v='|'; fi
  local bar; bar="$(printf '%*s' "$((IW+2))" '' | tr ' ' "$h")"
  bline() {
    local vis="$1" col="$2" pad
    pad=$(( IW - ${#vis} )); [ "$pad" -lt 0 ] && pad=0
    printf "   %s%s%s %s%*s %s%s%s\n" "$GREEN" "$v" "$RESET" "$col" "$pad" "" "$GREEN" "$v" "$RESET"
  }
  printf "\n   %s%s%s%s%s\n" "$GREEN" "$tl" "$bar" "$tr" "$RESET"
  bline "" ""
  bline "  S L e e L a    Clean Install (macOS)" "  ${BOLD}${WHITE}S L e e L a${RESET}    ${GREEN}Clean Install${RESET} ${GREY}(macOS)${RESET}"
  bline "  $TAGLINE" "  ${DIM}${ITAL}${TAGLINE}${RESET}"
  bline "" ""
  bline "  release $RELEASE    syntax $SYNTAX_RANGE" "  ${GREY}release${RESET} ${WHITE}${RELEASE}${RESET}    ${GREY}syntax${RESET} ${WHITE}${SYNTAX_RANGE}${RESET}"
  bline "" ""
  printf "   %s%s%s%s%s\n\n" "$GREEN" "$bl" "$bar" "$br" "$RESET"
}

heading() { printf "\n%s%s %s%s\n" "$GREENB" "▌" "$*" "$RESET"; rule; }

ask_yn() {
  local prompt="$1" default="$2" answer hint
  if [ "$default" = yes ]; then hint="${BOLD}Y${RESET}/n"; else hint="y/${BOLD}N${RESET}"; fi
  if [ ! -t 0 ]; then
    printf "  %s?%s %s %s(no terminal; using default: %s)%s\n" "$GREEN" "$RESET" "$prompt" "$GREY" "$default" "$RESET" >&2
    echo "$default"; return 0
  fi
  printf "  %s?%s %s %s(%s)%s " "$GREEN" "$RESET" "$prompt" "$GREY" "$hint" "$RESET" >&2
  read -r answer || answer=""
  answer="${answer:-$default}"
  case "$(lc "$answer")" in y|yes) echo yes;; *) echo no;; esac
}
ask_value() { local prompt="$1" default="$2" answer
  printf "  %s?%s %s %s[%s]%s " "$GREEN" "$RESET" "$prompt" "$GREY" "$default" "$RESET" >&2
  read -r answer || answer=""; echo "${answer:-$default}"; }

# ---------------------------------------------------------------------------
# Scope and catalog — identical structure to the Linux edition.
# ---------------------------------------------------------------------------
CORE_RESIDENT=(
  core cpu opcodes vm runtime memory native reflection security registry
  config process modules public sleela
)
BLOCK1_NAME="Language, Compiler & Text"
BLOCK1=(
  compiler decompiler debugger sst nordshrift languages alphabet character
  boolean regex xml html text sldocument sleeladoc
)
BLOCK2_NAME="Standard Library, Data & Media"
BLOCK2=(
  collections data data-structures math crypto compression codecs audio video
  signal-processing io filesystem database db-connector sleela-sql ledger inputs
  inference-model
)
BLOCK3_NAME="Systems, Networking & Protocols"
BLOCK3=(
  net rmi munction synchro connector drivers operating-system systems bash
  terminal terminal_pixel sleela-terminal telephony-skya cdma wifi
  http http-1.0 http-2.0 http-3.0 http-4.0 http-5.0 http-6.0 http-7.0 http-8.0
  http-9.0 http-servers server-edition web-server-technology website api
  us-infrastructure
)
BLOCK4_NAME="Applications, UI, Tooling & Society"
BLOCK4=(
  gui ui user-interface idea ballum autocad ide ide-workbench linux-editors
  common-rails coorenagraph deliberation gameplay journey sommeliers
  social-model citizen manifesto moral-capacitor philosophers politico
  international-criminal-court psychiatry java java28 antivirus churn xlt
)
BLOCK_NAMES=("$BLOCK1_NAME" "$BLOCK2_NAME" "$BLOCK3_NAME" "$BLOCK4_NAME")

EXCLUDE_LIB=()
is_excluded() { local name="$1" e; for e in "${EXCLUDE_LIB[@]:-}"; do [ "$e" = "$name" ] && return 0; done; return 1; }
in_list() { local n="$1"; shift; local e; for e in "$@"; do [ "$e" = "$n" ] && return 0; done; return 1; }

include_group() {
  local keep=() e m matched
  for e in "${EXCLUDE_LIB[@]:-}"; do
    matched=0
    for m in "$@"; do [ "$e" = "$m" ] && { matched=1; break; }; done
    [ "$matched" -eq 0 ] && keep+=("$e")
  done
  EXCLUDE_LIB=("${keep[@]:-}")
}

INCLUDED_GROUPS=""
note_group() { INCLUDED_GROUPS="${INCLUDED_GROUPS:+$INCLUDED_GROUPS, }$1"; }

BLOCK_EXTRA=()
compute_catalog() {
  EXCLUDE_LIB=(); BLOCK_EXTRA=()
  local entry name
  for entry in "$REPO_ROOT"/lib/*/; do
    name="$(basename "$entry")"
    in_list "$name" "${CORE_RESIDENT[@]}" && continue
    EXCLUDE_LIB+=("$name")
    if ! in_list "$name" "${BLOCK1[@]}" "${BLOCK2[@]}" "${BLOCK3[@]}" "${BLOCK4[@]}"; then
      BLOCK_EXTRA+=("$name")
    fi
  done
}

present_members() {
  local out=() m
  for m in "$@"; do [ -d "$REPO_ROOT/lib/$m" ] && out+=("$m"); done
  printf '%s\n' "${out[@]:-}"
}

# Walk one block. bash-3.2-safe array build (no mapfile): read the present
# members line by line into an array.
select_block() {
  local block_name="$1"; shift
  local members=() line
  while IFS= read -r line; do [ -n "$line" ] && members+=("$line"); done < <(present_members "$@")
  [ "${#members[@]}" -eq 0 ] && return 0
  heading "Block: $block_name  ${GREY}(${#members[@]} modules)${RESET}"
  local shown="" m
  for m in "${members[@]}"; do shown="${shown:+$shown }$m"; done
  printf "  %s%s%s\n\n" "$GREY" "$shown" "$RESET"
  if [ "$(ask_yn "Include the entire \"$block_name\" block?" yes)" = yes ]; then
    include_group "${members[@]}"; note_group "$block_name (all)"
    ok "including all ${#members[@]} modules of \"$block_name\""
    return 0
  fi
  say "  ${GREY}Choosing modules individually for this block:${RESET}"
  local picked=0
  for m in "${members[@]}"; do
    if [ "$(ask_yn "  include ${WHITE}$m${RESET}?" no)" = yes ]; then
      include_group "$m"; picked=$((picked+1))
    fi
  done
  if [ "$picked" -gt 0 ]; then
    note_group "$block_name ($picked)"; ok "$picked module(s) included from \"$block_name\""
  else
    skip "no modules included from \"$block_name\""
  fi
  return 0
}

select_environment() {
  compute_catalog
  heading "Choose your SLeeLa environment"
  say "  ${GREY}The core runtime is always installed. Beyond it, the full SLeeLa${RESET}"
  say "  ${GREY}environment is organized into four blocks of resident software.${RESET}"
  say "  ${GREY}You will decide each block with a yes/no (then, if no, each module).${RESET}"
  printf "\n"
  if [ "$(ask_yn "Install EVERYTHING — every module in every block?" no)" = yes ]; then
    include_group "${EXCLUDE_LIB[@]:-}"; note_group "complete environment (all blocks)"
    ok "the complete SLeeLa environment will be installed"
    return 0
  fi
  select_block "$BLOCK1_NAME" "${BLOCK1[@]}"
  select_block "$BLOCK2_NAME" "${BLOCK2[@]}"
  select_block "$BLOCK3_NAME" "${BLOCK3[@]}"
  select_block "$BLOCK4_NAME" "${BLOCK4[@]}"
  if [ "${#BLOCK_EXTRA[@]}" -gt 0 ]; then
    select_block "Additional modules" "${BLOCK_EXTRA[@]}"
  fi
  return 0
}

print_contents() {
  heading "What this installs"
  printf "  %sBinaries%s %s(always installed)%s\n" "$WHITE" "$RESET" "$GREY" "$RESET"
  item "sleela      ${GREY}— the SLeeLa language runtime (native C/C++ Core VM)${RESET}"
  item "sleelvac    ${GREY}— the SLeeLa compiler${RESET}"
  item "nordshrift  ${GREY}— the .sst → Java/SLeeLa/C transpiler${RESET}"
  printf "\n  %sResident core%s %s(always installed)%s\n" "$WHITE" "$RESET" "$GREY" "$RESET"
  item "core · cpu · vm · runtime · memory · native · reflection · security · registry · config"
  printf "\n  %sThe rest of the environment%s %s(you choose, block by block)%s\n" "$WHITE" "$RESET" "$GREY" "$RESET"
  item "1) ${WHITE}$BLOCK1_NAME${RESET}"
  item "2) ${WHITE}$BLOCK2_NAME${RESET}"
  item "3) ${WHITE}$BLOCK3_NAME${RESET}"
  item "4) ${WHITE}$BLOCK4_NAME${RESET}"
  printf "\n  %sYou will be asked yes/no for each block (and, if you decline a block,%s\n" "$GREY" "$RESET"
  printf "  %sfor each module in it). You may also install EVERYTHING at once.%s\n\n" "$GREY" "$RESET"
}

# ---------------------------------------------------------------------------
# Build + stage + install the main package (macOS paths).
# ---------------------------------------------------------------------------
STAGE_DIR=""
INSTALLED_LOG=""
LIB_KEPT=0
LIB_DROPPED=0
cleanup() { [ -n "$STAGE_DIR" ] && rm -rf "$STAGE_DIR"; return 0; }
trap cleanup EXIT

BUILD_LOG=""
BUILD_STARTED=""
BUILD_ENDED=""

summarize_build() {
  local log="$1"
  local compiles links warns total
  total=$(wc -l <"$log" 2>/dev/null | tr -d ' '); : "${total:=0}"
  compiles=$(grep -cE '(^| )-c ' "$log" 2>/dev/null || true); : "${compiles:=0}"
  links=$(grep -cE ' -o build/(sleela|sleelvac|nordshrift)( |$)' "$log" 2>/dev/null || true); : "${links:=0}"
  warns=$(grep -ciE 'warning:' "$log" 2>/dev/null || true); : "${warns:=0}"
  local u_unit="units" u_prod="products"
  [ "$compiles" = 1 ] && u_unit="unit"
  [ "$links" = 1 ] && u_prod="product"
  printf "    %scompiled%s %s %s   %slinked%s %s %s   %swarnings%s %s   %slog lines%s %s\n" \
    "$GREY" "$RESET" "$compiles" "$u_unit" "$GREY" "$RESET" "$links" "$u_prod" \
    "$GREY" "$RESET" "$warns" "$GREY" "$RESET" "$total"
  if [ "$warns" -gt 0 ]; then
    warn "compiler emitted ${warns} warning(s); first few:"
    grep -iE 'warning:' "$log" | head -n 3 | sed 's/^/        /'
  fi
}

install_main() {
  local install_root="$1" bin_dest="$2" config_dir="$3" mode="$4"

  heading "Building SLeeLa"
  local manifest="${SLEELA_SHA256_MANIFEST:-$REPO_ROOT/security/sha256-manifest.json}"
  local stamp; stamp="$(date +%Y%m%d-%H%M%S)"
  mkdir -p "$REPO_ROOT/install/logs"
  BUILD_LOG="$REPO_ROOT/install/logs/build-$stamp.log"
  BUILD_STARTED="$(date '+%Y-%m-%d %H:%M:%S %Z')"
  {
    printf 'SLeeLa Clean Install (macOS) — verbatim build log\n'
    printf 'started: %s\n' "$BUILD_STARTED"
    printf 'command: make -C %s/impl build/sleela build/sleelvac build/nordshrift\n' "$REPO_ROOT"
    printf 'manifest: %s\n' "$manifest"
    printf '%s\n' "----------------------------------------------------------------------"
  } >"$BUILD_LOG"

  say "  ${GREY}building the core products; verbatim output captured to the build log${RESET}"
  local product built_ok=1
  for product in sleela sleelvac nordshrift; do
    step "build ${WHITE}$product${RESET}"
    printf '\n===== make build/%s =====\n' "$product" >>"$BUILD_LOG"
    if env SLEELA_SHA256_MANIFEST="$manifest" \
          make -C "$REPO_ROOT/impl" "build/$product" >>"$BUILD_LOG" 2>&1; then
      ok "$product built"
    else
      built_ok=0
      warn "build of ${WHITE}$product${RESET} failed — exact messages (tail):"
      tail -n 14 "$BUILD_LOG" | sed 's/^/        /'
      break
    fi
  done
  BUILD_ENDED="$(date '+%Y-%m-%d %H:%M:%S %Z')"
  printf '\n----------------------------------------------------------------------\nended: %s\n' \
    "$BUILD_ENDED" >>"$BUILD_LOG"

  if [ "$built_ok" -ne 1 ]; then
    say "  ${GREY}full verbatim build log:${RESET} ${WHITE}$BUILD_LOG${RESET}"
    die "build failed — the exact messages above and the full log are retained for debugging."
  fi
  ok "build complete"
  say "  ${GREY}summary${RESET}"
  summarize_build "$BUILD_LOG"
  say "  ${GREY}verbatim log:${RESET} ${WHITE}$BUILD_LOG${RESET}"

  heading "Staging the main package"
  STAGE_DIR="$(mktemp -d "${TMPDIR:-/tmp}/sleela-clean.XXXXXX")"
  mkdir -p "$STAGE_DIR/bin" "$STAGE_DIR/lib" "$STAGE_DIR/config" "$STAGE_DIR/tools" "$STAGE_DIR/security"

  step "copying binaries"
  local product2
  for product2 in sleela sleelvac nordshrift; do
    local built="$REPO_ROOT/impl/build/$product2"
    [ -x "$built" ] || die "required compiled product missing: $built"
    install -m 755 "$built" "$STAGE_DIR/bin/$product2"
    item "$product2"
  done

  step "staging the standard library (resident core + the modules you selected)"
  local entry name
  LIB_KEPT=0; LIB_DROPPED=0
  for entry in "$REPO_ROOT"/lib/*/; do
    name="$(basename "$entry")"
    if is_excluded "$name"; then LIB_DROPPED=$((LIB_DROPPED+1)); continue; fi
    cp -a "$entry" "$STAGE_DIR/lib/"
    LIB_KEPT=$((LIB_KEPT+1))
  done
  cp -a "$REPO_ROOT"/lib/*.md "$STAGE_DIR/lib/" 2>/dev/null || true
  ok "library staged: ${LIB_KEPT} packages kept, ${LIB_DROPPED} excluded"

  step "staging the integrity manifest and verifier"
  cp -a "$REPO_ROOT/tools/verify-before-execution.py" "$STAGE_DIR/tools/"
  cp -a "$REPO_ROOT/security/sha256-manifest.json" "$STAGE_DIR/security/"
  [ -f "$REPO_ROOT/SHEET.sheet" ] && cp -a "$REPO_ROOT/SHEET.sheet" "$STAGE_DIR/"
  ok "manifest + verifier staged"

  local st_servers=excluded st_ides=excluded st_wifi=excluded
  is_excluded http   || st_servers=included
  is_excluded ide    || st_ides=included
  is_excluded wifi   || st_wifi=included
  cat > "$STAGE_DIR/config/startup.conf" <<EOF
# Generated by SLeeLa Clean Install (macOS)
install.mode=$mode
install.root=$install_root
package=main
platform=macos
servers=$st_servers
ides=$st_ides
wifi=$st_wifi
EOF

  heading "Installing"
  if [ "$mode" = user ]; then
    mkdir -p "$install_root"
    cp -a "$STAGE_DIR/." "$install_root/"
    ok "installed to ${WHITE}$install_root${RESET}"
  else
    command -v sudo >/dev/null 2>&1 || die "system install needs sudo or an already-privileged shell."
    sudo mkdir -p "$install_root" "$bin_dest" "$config_dir"
    sudo cp -a "$STAGE_DIR/." "$install_root/"
    sudo cp "$STAGE_DIR/config/startup.conf" "$config_dir/startup.conf"
    for product in sleela sleelvac nordshrift; do
      sudo ln -sfn "$install_root/bin/$product" "$bin_dest/$product"
    done
    ok "installed to ${WHITE}$install_root${RESET} (links in $bin_dest)"
  fi
}

# ===========================================================================
# Flow
# ===========================================================================
clear 2>/dev/null || true
banner
say "  ${GREY}This installs the SLeeLa open-source library, its binaries,${RESET}"
say "  ${GREY}protocols, and support modules onto your Mac.${RESET}"

print_contents

heading "Your decision"
PROCEED="$(ask_yn "Install the SLeeLa main package now?" yes)"
if [ "$PROCEED" != yes ]; then
  printf "\n  %sNo changes made.%s You can run this installer again any time.\n\n" "$GREY" "$RESET"
  exit 0
fi

heading "Where to install"
say "  1) ${WHITE}User-local${RESET}   ${GREY}no administrator access, recommended${RESET}"
say "  2) ${WHITE}System-wide${RESET}  ${GREY}/usr/local, may request sudo${RESET}"
SCOPE="$(ask_value "Choose (1|2)" "1")"
case "$SCOPE" in
  1) MODE=user;   INSTALL_ROOT="${SLEELA_INSTALL_ROOT:-$HOME/.local/sleela}"; BIN_DEST="$INSTALL_ROOT/bin"; CONFIG_DIR="$INSTALL_ROOT/config" ;;
  2) MODE=system; INSTALL_ROOT="${SLEELA_SYSTEM_ROOT:-/usr/local/lib/sleela}"; BIN_DEST="/usr/local/bin"; CONFIG_DIR="/usr/local/etc/sleela" ;;
  *) die "invalid scope (choose 1 or 2)";;
esac

select_environment

install_main "$INSTALL_ROOT" "$BIN_DEST" "$CONFIG_DIR" "$MODE"

if [ -n "$BUILD_LOG" ] && [ -f "$BUILD_LOG" ]; then
  if [ "$MODE" = user ]; then
    mkdir -p "$INSTALL_ROOT/logs" && cp -a "$BUILD_LOG" "$INSTALL_ROOT/logs/" 2>/dev/null || true
    INSTALLED_LOG="$INSTALL_ROOT/logs/$(basename "$BUILD_LOG")"
  else
    sudo mkdir -p "$INSTALL_ROOT/logs" && sudo cp -a "$BUILD_LOG" "$INSTALL_ROOT/logs/" 2>/dev/null || true
    INSTALLED_LOG="$INSTALL_ROOT/logs/$(basename "$BUILD_LOG")"
  fi
fi

heading "Installed"
ok "SLeeLa ${RELEASE} main package is installed."
say "  ${GREY}Binaries:${RESET} ${WHITE}$INSTALL_ROOT/bin${RESET}  ${GREY}(sleela · sleelvac · nordshrift)${RESET}"
say "  ${GREY}Try:${RESET}      ${WHITE}$INSTALL_ROOT/bin/sleela version${RESET}"

record_of_build() {
  local f_prod f_lib f_dest f_log f_when
  f_prod="sleela, sleelvac, nordshrift"
  f_lib="${LIB_KEPT} packages included · ${LIB_DROPPED} excluded"
  f_dest="$INSTALL_ROOT"
  f_log="${INSTALLED_LOG:-$BUILD_LOG}"
  f_when="$BUILD_STARTED → $BUILD_ENDED"
  heading "Record of Build"
  printf "  %sBe it recorded that the SLeeLa main package was built and installed%s\n" "$GREY" "$RESET"
  printf "  %son macOS in good order, as attested below.%s\n\n" "$GREY" "$RESET"
  printf "    %s%-18s%s %s\n" "$GREY" "Product" "$RESET" "SLeeLa ${RELEASE}  (syntax ${SYNTAX_RANGE})"
  printf "    %s%-18s%s %s\n" "$GREY" "Binaries" "$RESET" "$f_prod"
  printf "    %s%-18s%s %s\n" "$GREY" "Library" "$RESET" "$f_lib"
  local f_incl="(resident core only)"; [ -n "$INCLUDED_GROUPS" ] && f_incl="$INCLUDED_GROUPS"
  printf "    %s%-18s%s %s\n" "$GREY" "Blocks chosen" "$RESET" "$f_incl"
  printf "    %s%-18s%s %s\n" "$GREY" "Scope" "$RESET" "$MODE"
  printf "    %s%-18s%s %s\n" "$GREY" "Platform" "$RESET" "macOS"
  printf "    %s%-18s%s %s\n" "$GREY" "Destination" "$RESET" "$f_dest"
  printf "    %s%-18s%s %s\n" "$GREY" "Build window" "$RESET" "$f_when"
  printf "    %s%-18s%s %s\n" "$GREY" "Integrity" "$RESET" "SHA-256 manifest honored (fail-closed)"
  printf "    %s%-18s%s %s%s%s\n" "$GREY" "Verbatim log" "$RESET" "$WHITE" "$f_log" "$RESET"
  printf "\n  %s%sSo attested, this build complete and in order.%s\n" "$GREEN" "$ITAL" "$RESET"
}
record_of_build

heading "Done"
ok "Enjoy SLeeLa."
say "  ${DIM}${ITAL}${TAGLINE}${RESET}"
printf "\n"
