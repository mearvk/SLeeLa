#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
for f in "$ROOT"/lib/regex/RegexNatural*.sleela; do test -s "$f"; done
count=$(find "$ROOT/lib/regex" -maxdepth 1 -name 'RegexNatural*.sleela' -type f | wc -l)
test "$count" -ge 5
printf 'SLeeLa Natural Form source inventory: PASS (%s files)\n' "$count"
