#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
MANIFEST="${1:-${ROOT}/integrity/SHA256SUMS}"

if [[ ! -f "$MANIFEST" || -L "$MANIFEST" ]]; then
  echo "ERROR: integrity manifest is missing or is a symlink: $MANIFEST" >&2
  exit 2
fi

command -v sha256sum >/dev/null 2>&1 || {
  echo "ERROR: sha256sum is required." >&2
  exit 2
}

cd "$ROOT"

while IFS= read -r line || [[ -n "$line" ]]; do
  [[ -z "$line" ]] && continue
  [[ "$line" == \#* ]] && continue

  path="${line#*  }"
  if [[ "$path" == "$line" || -z "$path" ]]; then
    echo "ERROR: malformed manifest line." >&2
    exit 3
  fi

  case "$path" in
    /*|../*|*/../*|./../*)
      echo "ERROR: unsafe manifest path: $path" >&2
      exit 3
      ;;
  esac

  target="$ROOT/$path"
  if [[ ! -f "$target" || -L "$target" ]]; then
    echo "ERROR: manifest target is missing or is a symlink: $path" >&2
    exit 3
  fi

done < "$MANIFEST"

sha256sum --check --strict "$MANIFEST"
echo "AE6E66 integrity verification: PASS"
