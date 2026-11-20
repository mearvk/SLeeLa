#!/usr/bin/env bash
# Build the live SleelaDoc site, contributor-facing docs/API site, and a
# versioned SLeeLa API snapshot (edition 3.2 unless API_EDITION says otherwise) from the same /lib source inventory.
set -euo pipefail

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

PYTHON="${PYTHON:-python3}"
# The language API edition the snapshot belongs to. 3.2 is the current edition.
# 2026-11-20: this default was 3.1 ("the current edition, also written 3.01"). 3.01 means 3.1, not 3.2.
# The 3.1 and 3.0 editions under api/sleela/ are preserved and no longer written to.
EDITION="${API_EDITION:-3.2}"
if [ -n "${API_SNAPSHOT:-}" ]; then
  SNAPSHOT="$API_SNAPSHOT"
else
  NEXT=1
  for existing in "api/sleela/$EDITION"/version-[0-9][0-9][0-9]; do
    [ -d "$existing" ] || continue
    number="${existing##*version-}"
    # Force base-10: zero-padded values like 008/009 are otherwise parsed as
    # (invalid) octal by $(( )) and abort the build.
    number=$((10#$number))
    if [ "$number" -ge "$NEXT" ]; then NEXT=$((number + 1)); fi
  done
  SNAPSHOT="$(printf 'version-%03d' "$NEXT")"
fi
case "$SNAPSHOT" in
  version-[0-9][0-9][0-9]) ;;
  *) echo "API_SNAPSHOT must use version-NNN (for example version-002)." >&2; exit 2 ;;
esac

SNAPSHOT_DIR="api/sleela/$EDITION/$SNAPSHOT"
if [ -e "$SNAPSHOT_DIR/index.html" ]; then
  echo "Refusing to overwrite immutable API snapshot: $SNAPSHOT_DIR" >&2
  echo "Choose the next snapshot, for example: API_SNAPSHOT=version-002 make api-docs" >&2
  exit 2
fi

"$PYTHON" tools/generate-sleeladoc-api.py
"$PYTHON" tools/generate-sleeladoc-api.py --out docs/api
"$PYTHON" tools/generate-sleeladoc-api.py --out "$SNAPSHOT_DIR"

echo "API documentation built:"
echo "  live:     sleeladoc-api/index.html"
echo "  docs:     docs/api/index.html"
echo "  snapshot: $SNAPSHOT_DIR/index.html"
