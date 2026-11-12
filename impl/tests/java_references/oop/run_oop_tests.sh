#!/usr/bin/env bash
# Java-style OOP at the interpreter level (SLEELA.syntax section 43.6):
# super(args), super.m(), virtual dispatch, this(args), interfaces, abstract
# methods, implicit super(), inherited field initializers. Exact output match.
set -uo pipefail
BIN="${1:-impl/build/sleela}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../../.." && pwd)"
cd "$ROOT"; BIN="$(cd "$(dirname "$BIN")" && pwd)/$(basename "$BIN")"
export SLEELA_HOME="${SLEELA_HOME:-$ROOT}"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$ROOT/security/sha256-manifest.json}"
cd "$ROOT/impl/tests/java_references/oop"
fail=0
for f in *.sleela; do
  n="${f%.sleela}"
  got="$("$BIN" run "$f" 2>&1 | sed -E -e '/^verification: PASS/d' -e '/^\[security\]/d' -e '/^\[defender\]/d' -e '/^\[memory-manager\]/d')"
  if [[ "$got" == "$(cat "$n.expected.txt")" ]]; then echo "PASS  $n"; else echo "FAIL  $n"; printf '  expected: %s\n  actual:   %s\n' "$(tr '\n' '|' < "$n.expected.txt")" "$(printf '%s' "$got" | tr '\n' '|')"; fail=1; fi
done
[[ $fail == 0 ]] && echo "java oop regression: PASS" || { echo "java oop regression: FAIL"; exit 1; }
