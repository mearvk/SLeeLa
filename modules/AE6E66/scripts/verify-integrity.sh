#!/usr/bin/env bash
set -euo pipefail

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
MANIFEST="${1:-${ROOT}/integrity/SHA256SUMS}"

if [[ ! -f "$MANIFEST" ]]; then
  echo "ERROR: integrity manifest not found: $MANIFEST" >&2
  echo "Create a reviewed manifest from a trusted checkout before execution." >&2
  exit 2
fi

command -v sha256sum >/dev/null 2>&1 || {
  echo "ERROR: sha256sum is required." >&2
  exit 2
}

cd "$ROOT"

# The manifest must contain relative paths only and may not escape the module.
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
done < "$MANIFEST"

sha256sum --check --strict "$MANIFEST"
echo "AE6E66 integrity verification: PASS"
