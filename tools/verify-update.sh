#!/bin/sh
set -eu

# SLeeLa verification gate.
# Usage: verify-update.sh MANIFEST FILE...
# Manifest format: one SHA-256 hex digest followed by whitespace and a file path.
# The file paths supplied on the command line must be present in the manifest.

manifest=${1:?manifest required}
shift

[ -f "$manifest" ] || { echo "sleeLa verify: manifest not found: $manifest" >&2; exit 2; }
[ "$#" -gt 0 ] || { echo "sleeLa verify: no files supplied" >&2; exit 2; }

for file in "$@"; do
    [ -f "$file" ] || { echo "sleeLa verify: file not found: $file" >&2; exit 2; }

    expected=$(awk -v target="$file" '$0 !~ /^[[:space:]]*#/ && NF >= 2 { path=$2; for (i=3; i<=NF; i++) path=path " " $i; if (path == target) { print $1; exit } }' "$manifest")
    [ -n "$expected" ] || { echo "sleeLa verify: no manifest entry for: $file" >&2; exit 3; }
    case "$expected" in
        *[!0123456789abcdefABCDEF]*|????????????????????????????????????????????????????????????????) ;;
        *) echo "sleeLa verify: invalid SHA-256 for: $file" >&2; exit 3 ;;
    esac
    actual=$(sha256sum "$file" | awk '{print $1}')
    if [ "$actual" != "$expected" ]; then
        echo "sleeLa verify: SHA-256 mismatch: $file" >&2
        echo "  expected: $expected" >&2
        echo "  actual:   $actual" >&2
        exit 4
    fi
    echo "sleeLa verify: OK $file ($actual)"
done

exit 0
