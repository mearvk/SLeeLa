#!/usr/bin/env bash
set -euo pipefail
REPO="${SLEELA_SOFTWARE_REPO:-mearvk/Ubuntu.Determinant.Beta.Restricted}"
ROOT="${SLEELA_SOFTWARE_ROOT:-$HOME/.local/share/sleela/software}"
API="https://api.github.com/repos/${REPO}/releases?per_page=20"
usage(){ echo "Usage: $0 scan | install <securejdk28|cmd|asysma|all>"; }
release_tag(){
  curl -fsSL "$API" | python3 -c 'import json,sys; d=json.load(sys.stdin); mode=sys.argv[1]; [print(r.get("tag_name","")) for r in d if not r.get("draft") and ((mode=="final" and not r.get("prerelease")) or (mode=="alpha" and r.get("prerelease")))][:1]' "$1"
}
scan(){
  echo "SleelaTerminal Software Scan"
  echo "GitHub source: https://github.com/${REPO}"
  command -v curl >/dev/null 2>&1 || { echo "curl is required." >&2; exit 2; }
  command -v python3 >/dev/null 2>&1 || { echo "python3 is required for release-state scanning." >&2; exit 2; }
  curl -fsSL "$API" | python3 -c 'import json,sys; d=json.load(sys.stdin); print("Release state:"); [print(f"  {r.get(\"tag_name\",\"untagged\")} — {\"ALPHA/PRE-RELEASE\" if r.get(\"prerelease\") else \"FINAL\"}{\" — DRAFT\" if r.get(\"draft\") else \"\"}") for r in d[:10]]'
  echo "Latest final: $(release_tag final || true)"
  echo "Latest alpha/pre-release: $(release_tag alpha || true)"
  echo "Scan is read-only. Installation requires an explicit action."
}
install_one(){
  local product="$1" mode="${SLEELA_RELEASE_MODE:-final}" tag=""
  command -v curl >/dev/null 2>&1 || { echo "curl is required." >&2; exit 2; }
  command -v python3 >/dev/null 2>&1 || { echo "python3 is required." >&2; exit 2; }
  command -v git >/dev/null 2>&1 || { echo "git is required." >&2; exit 2; }

  # CMD is currently sourced from tools/cmd in Ubuntu.Determinant.Beta.Restricted.
  # It is a source-tree tool; main is authoritative until a CMD release channel exists.
  if [ "$product" = "cmd" ]; then
    local cmd_repo="mearvk/Ubuntu.Determinant.Beta.Restricted"
    local cmd_path="tools/cmd"
    local dest="$ROOT/cmd"
    local source_root="$ROOT/.sources/Ubuntu.Determinant.Beta.Restricted"
    mkdir -p "$ROOT/.sources"
    if [ -d "$source_root/.git" ]; then
      git -C "$source_root" fetch --prune origin
      git -C "$source_root" checkout --detach origin/main
    else
      rm -rf "$source_root"
      git clone --depth 1 "https://github.com/${cmd_repo}.git" "$source_root"
    fi
    rm -rf "$dest"
    mkdir -p "$dest"
    cp -a "$source_root/$cmd_path/." "$dest/"
    echo "Prepared CMD from ${cmd_repo}/${cmd_path} at: $dest"
    if [ -f "$dest/Makefile" ]; then
      echo "Building CMD tools from source..."
      (cd "$dest" && make)
    fi
    return
  fi

  if [ "$mode" != "final" ] && [ "$mode" != "alpha" ]; then echo "SLEELA_RELEASE_MODE must be final or alpha." >&2; exit 2; fi
  tag="$(release_tag "$mode" || true)"
  if [ -z "$tag" ] && [ "$mode" = "final" ]; then
    tag="$(release_tag alpha || true)"
    [ -n "$tag" ] && echo "No published final release was found; using latest alpha/pre-release: $tag"
  fi
  local dest="$ROOT/$product"
  mkdir -p "$ROOT"
  if [ -e "$dest/.git" ]; then
    git -C "$dest" fetch --tags --prune origin
    if [ -n "$tag" ]; then git -C "$dest" checkout --detach "$tag"; else git -C "$dest" pull --ff-only origin HEAD || true; fi
  else
    rm -rf "$dest"
    if [ -n "$tag" ]; then git clone --branch "$tag" --depth 1 "https://github.com/${REPO}.git" "$dest"; else git clone "https://github.com/${REPO}.git" "$dest"; fi
  fi
  echo "Prepared $product from ${tag:-default branch} at: $dest"
  if [ -f "$dest/install.sh" ]; then (cd "$dest" && bash ./install.sh)
  elif [ -f "$dest/scripts/install.sh" ]; then (cd "$dest" && bash ./scripts/install.sh)
  else echo "No repository-provided installer was found; no privileged system changes were made."; fi
}

case "${1:-}" in
  scan) scan ;;
  install)
    case "${2:-}" in
      securejdk28|cmd|asysma) install_one "$2" ;;
      all) for p in securejdk28 cmd asysma; do install_one "$p"; done ;;
      *) usage; exit 2 ;;
    esac ;;
  *) usage; exit 2 ;;
esac
