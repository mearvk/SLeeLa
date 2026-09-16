#!/usr/bin/env bash
set -euo pipefail
REPO="${SLEELA_SOFTWARE_REPO:-mearvk/Ubuntu.Determinant.Beta.Restricted}"
ROOT="${SLEELA_SOFTWARE_ROOT:-$HOME/.local/share/sleela/software}"
API="https://api.github.com/repos/${REPO}/releases?per_page=20"
usage(){ echo "Usage: $0 scan | install <securejdk28|cmd|asysma|all>"; }
scan(){
  echo "SleelaTerminal Software Scan"
  echo "GitHub source: https://github.com/${REPO}"
  command -v curl >/dev/null 2>&1 || { echo "curl is required." >&2; exit 2; }
  if command -v python3 >/dev/null 2>&1; then
    curl -fsSL "$API" | python3 -c 'import json,sys; d=json.load(sys.stdin); print("Release state:"); [print(f"  {r.get(\"tag_name\",\"untagged\")} — {\"ALPHA/PRE-RELEASE\" if r.get(\"prerelease\") else \"FINAL\"}{\" — DRAFT\" if r.get(\"draft\") else \"\"}") for r in d[:10]]; print("Latest published final: "+next((r.get("tag_name","") for r in d if not r.get("prerelease") and not r.get("draft")),"none"))'
  else
    curl -fsSL "$API" | grep -E '"tag_name"|"prerelease"|"draft"' | head -30 || true
  fi
  echo "Scan is read-only. Installation requires an explicit action."
}
install_one(){
  local product="$1"; local dest="$ROOT/$product"; mkdir -p "$ROOT"
  if [ -e "$dest/.git" ]; then git -C "$dest" fetch --tags --prune origin; git -C "$dest" pull --ff-only origin HEAD || true
  else rm -rf "$dest"; git clone "https://github.com/${REPO}.git" "$dest"; fi
  if [ -f "$dest/install.sh" ]; then (cd "$dest" && bash ./install.sh)
  elif [ -f "$dest/scripts/install.sh" ]; then (cd "$dest" && bash ./scripts/install.sh)
  else echo "Prepared latest GitHub source for $product at: $dest"; echo "No repository-provided installer was found; no privileged system changes were made."; fi
}
case "${1:-}" in scan) scan;; install) case "${2:-}" in securejdk28|cmd|asysma) install_one "$2";; all) for p in securejdk28 cmd asysma; do install_one "$p"; done;; *) usage; exit 2;; esac;; *) usage; exit 2;; esac
