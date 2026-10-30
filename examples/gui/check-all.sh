#!/usr/bin/env sh
# Non-interactive source validation for every SLeeLa GUI example.
# Run from the repository root: sh examples/gui/check-all.sh
set -eu

SLEELA_BIN="${SLEELA_BIN:-sleela}"
if ! command -v "$SLEELA_BIN" >/dev/null 2>&1; then
    echo "error: sleela executable not found (set SLEELA_BIN to its path)" >&2
    exit 127
fi

count=0
for source in examples/gui/*.sleela; do
    [ -f "$source" ] || continue
    echo "==> $source"
    "$SLEELA_BIN" check "$source"
    count=$((count + 1))
done

echo "Validated $count GUI source example(s)."
