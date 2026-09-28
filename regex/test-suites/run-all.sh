#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
make -C "$ROOT" natural
make -C "$ROOT" java
for f in "$ROOT"/../lib/regex/RegexNatural*.sleela; do test -s "$f"; done
printf '%s\n' 'Natural Form suite: PASS'
