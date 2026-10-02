#!/usr/bin/env bash
# SLeeLa / JetBrains source acquisition
# Max Rupplin - MEARVK LLC - 2026
set -euo pipefail

REPO_URL="${JETBRAINS_SOURCE_REPO:-https://github.com/JetBrains/intellij-community.git}"
DESTINATION="${JETBRAINS_SOURCE_DIR:-$HOME/JetBrains/intellij-community}"
BRANCH="${JETBRAINS_SOURCE_BRANCH:-master}"
DEPTH="1"
UPDATE=0

usage() {
    cat <<'EOF'
Usage: download-source.sh [options]

Options:
  --destination PATH   Source checkout directory.
  --branch NAME        Git branch to clone/update. Default: master.
  --full               Clone complete Git history.
  --update             Update an existing checkout.
  --help               Show this help.
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --destination) [[ $# -ge 2 ]] || { echo "Missing value for --destination" >&2; exit 2; }; DESTINATION="$2"; shift 2 ;;
        --branch) [[ $# -ge 2 ]] || { echo "Missing value for --branch" >&2; exit 2; }; BRANCH="$2"; shift 2 ;;
        --full) DEPTH=""; shift ;;
        --update) UPDATE=1; shift ;;
        --help|-h) usage; exit 0 ;;
        *) echo "Unknown option: $1" >&2; usage >&2; exit 2 ;;
    esac
done

command -v git >/dev/null 2>&1 || { echo "Git is required but was not found." >&2; exit 1; }
mkdir -p "$(dirname "$DESTINATION")"

if [[ -d "$DESTINATION/.git" ]]; then
    if [[ "$UPDATE" -ne 1 ]]; then
        echo "Source already exists: $DESTINATION"
        echo "Use --update to fetch and fast-forward it."
        exit 0
    fi
    git -C "$DESTINATION" fetch origin "$BRANCH"
    git -C "$DESTINATION" checkout "$BRANCH"
    git -C "$DESTINATION" merge --ff-only "origin/$BRANCH"
    echo "Updated JetBrains source: $DESTINATION"
    exit 0
fi

if [[ -e "$DESTINATION" ]]; then
    echo "Destination exists but is not a Git checkout: $DESTINATION" >&2
    exit 1
fi

if [[ -n "$DEPTH" ]]; then
    git clone --depth "$DEPTH" --branch "$BRANCH" "$REPO_URL" "$DESTINATION"
else
    git clone --branch "$BRANCH" "$REPO_URL" "$DESTINATION"
fi

echo "JetBrains source acquired at: $DESTINATION"
