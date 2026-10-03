#!/usr/bin/env bash
# =============================================================================
# download-native-deps.sh -- fetch the native C/C++ libraries SLeeLa's build
# links against, into libs/native/. Only artifacts <= 50 MB are kept.
#
# Linked by the Makefiles (confirmed via `grep -rhoE '\-l<name>'`):
#   nghttp2 (-lnghttp2)      HTTP/2 server (impl)
#   openssl (-lssl -lcrypto) crypto / TLS
#   readline (-lreadline)    sleela terminal
#   termcap/ncurses (-ltermcap) sleela terminal
# ws2_32 is a Windows system import lib (not downloadable); excluded.
#
# Default method: the system package manager's DOWNLOAD-ONLY mode -- it does NOT
# install anything and does NOT elevate privileges. A source-tarball fallback
# is provided (commented) for environments without a package manager.
#
# Requires network access to the distro mirrors / upstream release hosts. In an
# offline sandbox this fails at the download step by design.
# =============================================================================
set -euo pipefail

HERE="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
DEST="$HERE/native"
MAX_BYTES=$((50 * 1024 * 1024))      # 50 MB hard cap per artifact
mkdir -p "$DEST"

# Package names per family, for the two common package managers.
#   name|dnf-packages|apt-packages
PKGS=(
  "nghttp2|libnghttp2-devel libnghttp2|libnghttp2-dev"
  "openssl|openssl-devel openssl-libs|libssl-dev"
  "readline|readline-devel readline|libreadline-dev"
  "termcap|ncurses-devel ncurses-libs|libncurses-dev libtinfo-dev"
)

# Upstream source tarballs (fallback). Each is well under 50 MB.
#   name|url
SRC_TARBALLS=(
  "nghttp2|https://github.com/nghttp2/nghttp2/releases/download/v1.64.0/nghttp2-1.64.0.tar.gz"
  "openssl|https://github.com/openssl/openssl/releases/download/openssl-3.4.0/openssl-3.4.0.tar.gz"
  "readline|https://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz"
  "termcap|https://ftp.gnu.org/gnu/termcap/termcap-1.3.1.tar.gz"
)

enforce_cap() {
  # Remove any file in DEST larger than the cap.
  find "$DEST" -type f -size +"$((MAX_BYTES))"c -print -exec rm -f {} \; \
    | sed 's/^/  SKIP (> 50 MB): /' || true
}

download_only_dnf() {
  echo "[dnf] download-only into $DEST (no install, no sudo)"
  local names=""
  for row in "${PKGS[@]}"; do names="$names ${row#*|}"; names="${names%|*}"; done
  # Re-extract dnf package list cleanly.
  local dnf_pkgs=()
  for row in "${PKGS[@]}"; do
    IFS='|' read -r _name dnf _apt <<< "$row"
    # shellcheck disable=SC2206
    dnf_pkgs+=($dnf)
  done
  dnf download --downloaddir="$DEST" --resolve "${dnf_pkgs[@]}" 2>&1 || \
    dnf install --downloadonly --downloaddir="$DEST" -y "${dnf_pkgs[@]}" 2>&1 || \
    return 1
}

download_only_apt() {
  echo "[apt] download into $DEST (no install, no sudo)"
  local apt_pkgs=()
  for row in "${PKGS[@]}"; do
    IFS='|' read -r _name _dnf apt <<< "$row"
    # shellcheck disable=SC2206
    apt_pkgs+=($apt)
  done
  ( cd "$DEST" && apt-get download "${apt_pkgs[@]}" ) 2>&1 || return 1
}

download_sources() {
  echo "[source] fetching upstream tarballs into $DEST"
  for row in "${SRC_TARBALLS[@]}"; do
    IFS='|' read -r name url <<< "$row"
    local out="$DEST/$(basename "$url")"
    echo "  fetch: $url"
    if command -v curl >/dev/null 2>&1; then curl -fL --retry 3 -o "$out" "$url" || { echo "    FAILED"; continue; }
    elif command -v wget >/dev/null 2>&1; then wget -q -O "$out" "$url" || { echo "    FAILED"; continue; }
    else echo "error: neither curl nor wget available" >&2; return 127; fi
  done
}

echo "=== SLeeLa native dependencies -> $DEST (<= 50 MB each) ==="

if command -v dnf >/dev/null 2>&1; then
  download_only_dnf || { echo "[dnf] failed; trying source tarballs"; download_sources; }
elif command -v apt-get >/dev/null 2>&1; then
  download_only_apt || { echo "[apt] failed; trying source tarballs"; download_sources; }
else
  echo "no supported package manager found; using source tarballs"
  download_sources
fi

enforce_cap
echo "=== done. Fetched artifacts in: $DEST ==="
ls -la "$DEST" 2>/dev/null || true
echo
echo "Note: ws2_32 (Windows Winsock) is a system import library shipped with"
echo "the Windows SDK/MinGW toolchain and is intentionally not downloaded."
