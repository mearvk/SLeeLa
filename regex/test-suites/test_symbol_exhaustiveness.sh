#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
SYMBOLS="$ROOT/regex/natural/SYMBOLS.md"
REFERENCE="$ROOT/regex/natural/SYMBOL.EXHAUSTIVENESS.md"

test -s "$SYMBOLS"
test -s "$REFERENCE"

# The normative reference contains exactly 19 semantic entries.
# Count table rows whose first column is one of the semantic families. The pipe
# must be escaped in the regex; an unescaped `^|` matches every line.
entries=$(awk 'BEGIN{n=0} /^\| (Atom|Quantity|Group|Logic|Anchor) / {n++} END{print n}' "$REFERENCE")
test "$entries" -eq 19

# Every required semantic family remains represented in the normative
# Natural Form symbol specification.
for token in     'any' 'digit' 'letter' 'space' 'quoted text' '[abc]' '[^abc]'     'one' 'optional' 'some' 'many' '{n}' '{n,m}'     '(...)' '<name: ...>' '(?:...)' '|' 'or' 'begin' 'end'
do
    grep -F "$token" "$SYMBOLS" >/dev/null
done

# The exhaustiveness reference itself must state the implementation proof
# boundary, preventing an inventory-only result from being called complete.
grep -F 'RequiredSymbols = DocumentedSymbols = TestedSymbols = AcceptedCoreSymbols' "$REFERENCE" >/dev/null
grep -F 'implementation exhaustiveness' "$REFERENCE" >/dev/null

printf 'SLeeLa regex symbol exhaustiveness reference: PASS (%s semantic entries)\n' "$entries"
