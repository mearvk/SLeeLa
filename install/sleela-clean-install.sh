#!/usr/bin/env bash
# =============================================================================
# SLeeLa Clean Install — a calm, beautiful installer for the SLeeLa open-source
# library, binaries, protocols, and support modules.
# Max Rupplin · MEARVK LLC · 2026
#
# WHAT IT INSTALLS
#   The main package only: the SLeeLa binaries (sleela, sleelvac, nordshrift),
#   the standard library, protocols, and support modules.
#
# WHAT IT NEVER INSTALLS
#   Servers, IDEs/editors, Wi-Fi implementations, and religious / moral /
#   political systems. Those library trees are excluded from the staged lib/.
#   (Note: the HTTP server objects are statically linked inside the sleela
#   binary upstream; "no servers" here means none of the server/IDE/wifi/
#   religious-moral *library source trees* are shipped — the core binary is
#   unchanged.)
#
# EXPERIENCE
#   1) One primary decision: install the main package (yes/no).
#   2) On success, optionally add curated enhancement modules.
#   Styled output degrades gracefully to plain text when stdout is not a
#   terminal, when NO_COLOR is set, or when --plain is passed.
# =============================================================================
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

PRODUCT="SLeeLa"
RELEASE="0.3.30-dev"
SYNTAX_RANGE="1.0 - 2.5"
TAGLINE="THE NOW. THE INGREDIENT."

# ---------------------------------------------------------------------------
# Presentation layer: color, glyphs, rules, boxes. All optional and safe.
# ---------------------------------------------------------------------------
USE_STYLE=1
for arg in "$@"; do [ "$arg" = "--plain" ] && USE_STYLE=0; done
if [ -n "${NO_COLOR:-}" ] || [ ! -t 1 ]; then USE_STYLE=0; fi

if [ "$USE_STYLE" -eq 1 ]; then
  ESC=$(printf '\033')
  RESET="${ESC}[0m";  BOLD="${ESC}[1m";  DIM="${ESC}[2m";  ITAL="${ESC}[3m"
  # Brand palette (approx of #00D68F green, #0B0F0F obsidian, white) via 256-color.
  GREEN="${ESC}[38;5;43m"     # glimmering SLeeLa green
  GREENB="${ESC}[1;38;5;43m"
  WHITE="${ESC}[38;5;231m"
  GREY="${ESC}[38;5;245m"
  RED="${ESC}[38;5;203m"
  AMBER="${ESC}[38;5;215m"
else
  ESC=""; RESET=""; BOLD=""; DIM=""; ITAL=""; GREEN=""; GREENB=""; WHITE=""; GREY=""; RED=""; AMBER=""
fi

# Semantic glyphs (always paired with color, so meaning survives --plain).
G_OK="[✓]"; G_RUN="[»]"; G_WARN="[!]"; G_ERR="[x]"; G_DOT="·"
if [ "$USE_STYLE" -eq 0 ]; then G_OK="[ok]"; G_RUN="[..]"; G_WARN="[!]"; G_ERR="[x]"; G_DOT="-"; fi

rule() { # a full-width light rule
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
  local IW=55  # inner content width
  local tl tr bl br h v
  if [ "$USE_STYLE" -eq 1 ]; then tl='╭'; tr='╮'; bl='╰'; br='╯'; h='─'; v='│'
  else tl='+'; tr='+'; bl='+'; br='+'; h='-'; v='|'; fi
  local bar; bar="$(printf '%*s' "$((IW+2))" '' | tr ' ' "$h")"
  # bline VISIBLE_TEXT COLORED_TEXT  — pads by VISIBLE length, prints COLORED.
  bline() {
    local vis="$1" col="$2" pad
    pad=$(( IW - ${#vis} )); [ "$pad" -lt 0 ] && pad=0
    printf "   %s%s%s %s%*s %s%s%s\n" "$GREEN" "$v" "$RESET" "$col" "$pad" "" "$GREEN" "$v" "$RESET"
  }
  printf "\n   %s%s%s%s%s\n" "$GREEN" "$tl" "$bar" "$tr" "$RESET"
  bline "" ""
  bline "  S L e e L a    Clean Install" "  ${BOLD}${WHITE}S L e e L a${RESET}    ${GREEN}Clean Install${RESET}"
  bline "  $TAGLINE" "  ${DIM}${ITAL}${TAGLINE}${RESET}"
  bline "" ""
  bline "  release $RELEASE    syntax $SYNTAX_RANGE" "  ${GREY}release${RESET} ${WHITE}${RELEASE}${RESET}    ${GREY}syntax${RESET} ${WHITE}${SYNTAX_RANGE}${RESET}"
  bline "" ""
  printf "   %s%s%s%s%s\n\n" "$GREEN" "$bl" "$bar" "$br" "$RESET"
}

heading() { printf "\n%s%s %s%s\n" "$GREENB" "▌" "$*" "$RESET"; rule; }

ask_yn() { # prompt default(yes|no) -> echoes yes|no (prompt -> stderr so only
           # the answer is captured by $(...))
  local prompt="$1" default="$2" answer hint
  if [ "$default" = yes ]; then hint="${BOLD}Y${RESET}/n"; else hint="y/${BOLD}N${RESET}"; fi
  printf "  %s?%s %s %s(%s)%s " "$GREEN" "$RESET" "$prompt" "$GREY" "$hint" "$RESET" >&2
  read -r answer || answer=""
  answer="${answer:-$default}"
  case "${answer,,}" in y|yes) echo yes;; *) echo no;; esac
}
ask_value() { local prompt="$1" default="$2" answer
  printf "  %s?%s %s %s[%s]%s " "$GREEN" "$RESET" "$prompt" "$GREY" "$default" "$RESET" >&2
  read -r answer || answer=""; echo "${answer:-$default}"; }

# ---------------------------------------------------------------------------
# Scope of the main package.
#   INCLUDED : binaries + the standard library minus the excluded trees.
#   EXCLUDED : servers, IDEs/editors, Wi-Fi, religious/moral/political trees.
# ---------------------------------------------------------------------------
EXCLUDE_LIB=(
  # servers
  http http-1.0 http-2.0 http-3.0 http-4.0 http-5.0 http-6.0 http-7.0 http-8.0 http-9.0
  http-servers server-edition web-server-technology
  # IDEs / editors
  ide ide-workbench linux-editors
  # Wi-Fi
  wifi
  # religious / moral / political
  moral-capacitor philosophers manifesto politico international-criminal-court psychiatry citizen
)
is_excluded() { local name="$1" e; for e in "${EXCLUDE_LIB[@]}"; do [ "$e" = "$name" ] && return 0; done; return 1; }

# Curated optional enhancement modules offered AFTER a successful main install.
# Each is a top-level lib/ package that is safe, non-excluded, and genuinely
# "enhances the experience" rather than being part of the minimal core.
OPT_NAMES=(audio video signal-processing codecs compression gui ui database connector)
OPT_DESC=(
  "Audio playback and synthesis library"
  "Video decode/display library"
  "Signal processing (FFT, filters, spectra)"
  "Media codec registry and plugins"
  "Compression library"
  "Graphical UI widgets (SleelaUI)"
  "Core UI layout and quality system"
  "SQL/data model support"
  "Optional native data-source connectors"
)

print_contents() {
  heading "Contents of the main package"
  printf "  %sBinaries%s\n" "$WHITE" "$RESET"
  item "sleela      ${GREY}— the SLeeLa language runtime (native C/C++ Core VM)${RESET}"
  item "sleelvac    ${GREY}— the SLeeLa compiler${RESET}"
  item "nordshrift  ${GREY}— the .sst → Java/SLeeLa/C transpiler${RESET}"
  printf "\n  %sLibrary, protocols & support modules%s %s(selected)%s\n" "$WHITE" "$RESET" "$GREY" "$RESET"
  item "crypto · codecs · compression · regex · data-structures · collections"
  item "runtime · resolver · route · rmi · munction · connector · registry"
  item "native · drivers · modules · io · net · text · math · reflection"
  item "the full standard library, minus the excluded trees below"
  printf "\n  %sExcluded by design%s\n" "$WHITE" "$RESET"
  skip "servers        ${DIM}(http, http-1.0…9.0, http-servers, server-edition, web-server-technology)${RESET}"
  skip "IDEs / editors ${DIM}(ide, ide-workbench, linux-editors)${RESET}"
  skip "Wi-Fi          ${DIM}(wifi)${RESET}"
  skip "religious / moral / political systems"
  printf "\n"
}

# ---------------------------------------------------------------------------
# Build + stage + install the main package.
# ---------------------------------------------------------------------------
STAGE_DIR=""
INSTALLED_LOG=""
LIB_KEPT=0
LIB_DROPPED=0
# Always returns success so the EXIT trap never overrides the script's real exit
# status (a bare `[ -n "$x" ]` that is false would otherwise surface as exit 1).
# NOTE: the build log ($BUILD_LOG) is intentionally NOT removed here — it is the
# persistent, verbatim record kept for debugging.
cleanup() { [ -n "$STAGE_DIR" ] && rm -rf "$STAGE_DIR"; return 0; }
trap cleanup EXIT

# The verbatim build log. Written first under the repo's install/logs/ (which
# always exists), then copied into <install_root>/logs/ on a successful install.
BUILD_LOG=""
BUILD_STARTED=""
BUILD_ENDED=""

# Parse the verbatim log into a concise, dignified summary and print it. Also
# records the product -> status table used by the closing Record of Build.
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
  # Build ONLY the core products the main package installs (sleela, sleelvac,
  # nordshrift) via the integrity-gated impl build. We deliberately do NOT run
  # the whole-repo `make all`: that also builds servers, IDE bridges, the
  # java28 JNI bridge, premium VMs, etc. — subsystems this installer excludes,
  # and which pull in heavy external toolchains (e.g. a JDK for jni.h). Building
  # just the core keeps the install fast, dependency-light, and consistent with
  # "main package only".
  #
  # The on-screen view is a calm, summarized progress report; the EXACT,
  # verbatim compiler/linker messages are captured to $BUILD_LOG for debugging.
  local manifest="${SLEELA_SHA256_MANIFEST:-$REPO_ROOT/security/sha256-manifest.json}"
  local stamp; stamp="$(date +%Y%m%d-%H%M%S)"
  mkdir -p "$REPO_ROOT/install/logs"
  BUILD_LOG="$REPO_ROOT/install/logs/build-$stamp.log"
  BUILD_STARTED="$(date '+%Y-%m-%d %H:%M:%S %Z')"
  {
    printf 'SLeeLa Clean Install — verbatim build log\n'
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
  local product
  for product in sleela sleelvac nordshrift; do
    local built="$REPO_ROOT/impl/build/$product"
    [ -x "$built" ] || die "required compiled product missing: $built"
    install -m 755 "$built" "$STAGE_DIR/bin/$product"
    item "$product"
  done

  step "staging the standard library (excluding servers, IDEs, Wi-Fi, religious/moral)"
  local entry name
  LIB_KEPT=0; LIB_DROPPED=0
  for entry in "$REPO_ROOT"/lib/*/; do
    name="$(basename "$entry")"
    if is_excluded "$name"; then LIB_DROPPED=$((LIB_DROPPED+1)); continue; fi
    cp -a "$entry" "$STAGE_DIR/lib/"
    LIB_KEPT=$((LIB_KEPT+1))
  done
  # top-level lib/*.md inventory files travel with the library
  cp -a "$REPO_ROOT"/lib/*.md "$STAGE_DIR/lib/" 2>/dev/null || true
  ok "library staged: ${LIB_KEPT} packages kept, ${LIB_DROPPED} excluded"

  step "staging the integrity manifest and verifier"
  cp -a "$REPO_ROOT/tools/verify-before-execution.py" "$STAGE_DIR/tools/"
  cp -a "$REPO_ROOT/security/sha256-manifest.json" "$STAGE_DIR/security/"
  [ -f "$REPO_ROOT/SHEET.sheet" ] && cp -a "$REPO_ROOT/SHEET.sheet" "$STAGE_DIR/"
  ok "manifest + verifier staged"

  cat > "$STAGE_DIR/config/startup.conf" <<EOF
# Generated by SLeeLa Clean Install
install.mode=$mode
install.root=$install_root
package=main
servers=excluded
ides=excluded
wifi=excluded
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

install_optional_module() {
  local name="$1" install_root="$2" mode="$3"
  local src="$REPO_ROOT/lib/$name"
  [ -d "$src" ] || { warn "module '$name' not found; skipping"; return 0; }
  if [ "$mode" = user ]; then
    cp -a "$src" "$install_root/lib/"
  else
    sudo cp -a "$src" "$install_root/lib/"
  fi
  ok "added ${WHITE}$name${RESET}"
}

# ===========================================================================
# Flow
# ===========================================================================
clear 2>/dev/null || true
banner
say "  ${GREY}This installs the SLeeLa open-source library, its binaries,${RESET}"
say "  ${GREY}protocols, and support modules onto your computer.${RESET}"

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
  2) MODE=system; INSTALL_ROOT="${SLEELA_SYSTEM_ROOT:-/usr/local/lib/sleela}"; BIN_DEST="/usr/local/bin"; CONFIG_DIR="/etc/sleela" ;;
  *) die "invalid scope (choose 1 or 2)";;
esac

install_main "$INSTALL_ROOT" "$BIN_DEST" "$CONFIG_DIR" "$MODE"

# Preserve the verbatim build log alongside the installation for debugging.
if [ -n "$BUILD_LOG" ] && [ -f "$BUILD_LOG" ]; then
  if [ "$MODE" = user ]; then
    mkdir -p "$INSTALL_ROOT/logs" && cp -a "$BUILD_LOG" "$INSTALL_ROOT/logs/" 2>/dev/null || true
    INSTALLED_LOG="$INSTALL_ROOT/logs/$(basename "$BUILD_LOG")"
  else
    sudo mkdir -p "$INSTALL_ROOT/logs" && sudo cp -a "$BUILD_LOG" "$INSTALL_ROOT/logs/" 2>/dev/null || true
    INSTALLED_LOG="$INSTALL_ROOT/logs/$(basename "$BUILD_LOG")"
  fi
fi

# ---- success + optional enhancements --------------------------------------
heading "Installed"
ok "SLeeLa ${RELEASE} main package is installed."
say "  ${GREY}Binaries:${RESET} ${WHITE}$INSTALL_ROOT/bin${RESET}  ${GREY}(sleela · sleelvac · nordshrift)${RESET}"
say "  ${GREY}Try:${RESET}      ${WHITE}$INSTALL_ROOT/bin/sleela version${RESET}"

# ---- Record of Build: a formal, orderly attestation of what was done. -------
# Clean dressing appropriate for a State Captain / Attorney General: a plainly
# worded, signed-off record, with the verbatim build log cited for inspection.
record_of_build() {
  local lw=58
  local f_prod f_lib f_dest f_log f_when
  f_prod="sleela, sleelvac, nordshrift"
  f_lib="${LIB_KEPT} packages included · ${LIB_DROPPED} excluded"
  f_dest="$INSTALL_ROOT"
  f_log="${INSTALLED_LOG:-$BUILD_LOG}"
  f_when="$BUILD_STARTED → $BUILD_ENDED"
  heading "Record of Build"
  printf "  %sBe it recorded that the SLeeLa main package was built and installed%s\n" "$GREY" "$RESET"
  printf "  %sin good order, as attested below.%s\n\n" "$GREY" "$RESET"
  printf "    %s%-18s%s %s\n" "$GREY" "Product" "$RESET" "SLeeLa ${RELEASE}  (syntax ${SYNTAX_RANGE})"
  printf "    %s%-18s%s %s\n" "$GREY" "Binaries" "$RESET" "$f_prod"
  printf "    %s%-18s%s %s\n" "$GREY" "Library" "$RESET" "$f_lib"
  printf "    %s%-18s%s %s\n" "$GREY" "Excluded" "$RESET" "servers · IDEs/editors · Wi-Fi · religious/moral/political"
  printf "    %s%-18s%s %s\n" "$GREY" "Scope" "$RESET" "$MODE"
  printf "    %s%-18s%s %s\n" "$GREY" "Destination" "$RESET" "$f_dest"
  printf "    %s%-18s%s %s\n" "$GREY" "Build window" "$RESET" "$f_when"
  printf "    %s%-18s%s %s\n" "$GREY" "Integrity" "$RESET" "SHA-256 manifest honored (fail-closed)"
  printf "    %s%-18s%s %s%s%s\n" "$GREY" "Verbatim log" "$RESET" "$WHITE" "$f_log" "$RESET"
  printf "\n  %s%sSo attested, this build complete and in order.%s\n" "$GREEN" "$ITAL" "$RESET"
}
record_of_build

heading "Enhance your experience (optional)"
say "  ${GREY}The main package is ready. You may now add optional modules${RESET}"
say "  ${GREY}that further enhance SLeeLa. Each is independent.${RESET}"
printf "\n"
WANT_OPT="$(ask_yn "Browse optional enhancement modules?" no)"
if [ "$WANT_OPT" = yes ]; then
  printf "\n"
  i=0
  while [ "$i" -lt "${#OPT_NAMES[@]}" ]; do
    printf "    %s%2d%s  %s%-18s%s %s%s%s\n" "$GREEN" "$((i+1))" "$RESET" \
      "$WHITE" "${OPT_NAMES[$i]}" "$RESET" "$GREY" "${OPT_DESC[$i]}" "$RESET"
    i=$((i+1))
  done
  printf "\n"
  say "  ${GREY}Enter numbers separated by spaces (e.g. 1 3 6), 'all', or blank to skip.${RESET}"
  SELECTION="$(ask_value "Modules to add" "")"
  if [ -n "$SELECTION" ]; then
    heading "Adding modules"
    if [ "$SELECTION" = all ]; then
      for name in "${OPT_NAMES[@]}"; do install_optional_module "$name" "$INSTALL_ROOT" "$MODE"; done
    else
      for n in $SELECTION; do
        case "$n" in
          *[!0-9]*) warn "ignoring '$n' (not a number)";;
          *) idx=$((n-1))
             if [ "$idx" -ge 0 ] && [ "$idx" -lt "${#OPT_NAMES[@]}" ]; then
               install_optional_module "${OPT_NAMES[$idx]}" "$INSTALL_ROOT" "$MODE"
             else warn "ignoring '$n' (out of range)"; fi;;
        esac
      done
    fi
  else
    skip "no modules added"
  fi
fi

heading "Done"
ok "Enjoy SLeeLa."
say "  ${DIM}${ITAL}${TAGLINE}${RESET}"
printf "\n"
