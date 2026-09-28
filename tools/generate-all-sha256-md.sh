#!/usr/bin/env bash
set -euo pipefail

repo_root=$(git rev-parse --show-toplevel)
cd "$repo_root"

output="SHA256-DIGESTS.md"
tmp=$(mktemp)
trap 'rm -f "$tmp"' EXIT

{
  echo "# SLeeLa Repository SHA-256 Digests"
  echo
  echo "Generated from the Git-tracked files in this branch."
  echo
  echo "- Algorithm: SHA-256"
  echo "- Scope: every Git-tracked file except this digest document itself."
  echo "- Ordering: lexicographic by repository-relative path."
  echo "- The digest document is excluded because including its own digest would create a self-referential file-content cycle."
  echo
  echo "## Digests"
  echo
  echo "DIGEST  PATH"
  git ls-files -z -- ':!SHA256-DIGESTS.md' |
    sort -z |
    while IFS= read -r -d "" path; do
      printf "%s  %s\\n" "$(sha256sum -- "$path" | cut -d" " -f1)" "$path"
    done
} > "$tmp"

mv "$tmp" "$output"
