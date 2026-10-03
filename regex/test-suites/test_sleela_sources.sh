#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
LIB="$ROOT/lib/regex"
TMP="${TMPDIR:-/tmp}/sleela-regex-inventory.$$"
trap 'rm -f "$TMP" "$TMP.expected" "$TMP.actual"' EXIT HUP INT TERM

cat >"$TMP.expected" <<'EOF'
Regex.sleela
RegexCapability.sleela
RegexCapture.sleela
RegexCompiler.sleela
RegexDialect.sleela
RegexEngine.sleela
RegexError.sleela
RegexFlags.sleela
RegexIterator.sleela
RegexLiteral.sleela
RegexMatch.sleela
RegexMatcher.sleela
RegexNatural.sleela
RegexNaturalGrammar.sleela
RegexNaturalGroup.sleela
RegexNaturalParser.sleela
RegexNaturalSymbol.sleela
RegexOptions.sleela
RegexPattern.sleela
RegexReplacement.sleela
RegexReplacer.sleela
RegexResult.sleela
RegexScanner.sleela
RegexSplitter.sleela
RegexSubject.sleela
RegexSystem.sleela
RegexValidator.sleela
EOF

# Use a fixed byte collation so the inventory order is deterministic across
# locales (the expected list above is in C/ASCII order).
find "$LIB" -maxdepth 1 -type f -name 'Regex*.sleela' -exec basename {} \; | LC_ALL=C sort >"$TMP.actual"

while IFS= read -r file; do
    test -s "$LIB/$file"
done <"$TMP.expected"

# Portable comparison: `cmp`/`diff` are not guaranteed present, so compare the
# captured file contents as shell strings.
EXPECTED_CONTENT=$(cat "$TMP.expected")
ACTUAL_CONTENT=$(cat "$TMP.actual")
if [ "$EXPECTED_CONTENT" != "$ACTUAL_CONTENT" ]; then
    echo "SLeeLa regex source inventory: FAIL" >&2
    echo "Expected:" >&2
    cat "$TMP.expected" >&2
    echo "Actual:" >&2
    cat "$TMP.actual" >&2
    exit 1
fi

count=$(wc -l <"$TMP.actual" | tr -d ' ')
test "$count" -eq 27
printf 'SLeeLa regex source inventory: PASS (%s files)\n' "$count"
