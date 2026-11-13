#!/usr/bin/env bash
# Differential test for the sleelactl Java -> SLeeLa converter.
#
# For every sleelactl/convert/tests/java/<Main>.java:
#   1. run it on a real JDK (javac + java)           -> expected output
#   2. convert it with javaconv                      -> SLeeLa source
#   3. run the converted program on the SLeeLa VM    -> actual output
#   4. require the two outputs to be identical.
# Also asserts that constructs the model does not cover FAIL CLOSED (exit 1, no output).
#
# usage: tests/javaconv_differential_test.sh [path-to-sleela] [path-to-javaconv]
set -u
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/.." && pwd)"
conv="${2:-$root/sleelactl/convert/build/javaconv}"
sleela="${1:-}"
if [ -z "$sleela" ]; then
  if [ -x "$root/impl/build/sleela" ]; then sleela="$root/impl/build/sleela"; else sleela="$root/working-binaries/sleela"; fi
fi
[ -x "$conv" ] || { echo "javaconv not built: make -C sleelactl/convert"; exit 2; }
command -v javac >/dev/null 2>&1 || { echo "SKIP: no JDK on PATH (javac)"; exit 0; }
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib" SLEELA_SHEET="$root/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$root/security/sha256-manifest.json}"
tests="$root/sleelactl/convert/tests"
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
filter() { grep -v 'memory-manager\|^verification\|\[security\]\|\[defender\]'; }
fail=0; pass=0

for j in "$tests"/java/*.java; do
  name="$(basename "$j" .java)"
  mkdir -p "$tmp/$name/cls" "$tmp/$name/sl"
  if ! javac -d "$tmp/$name/cls" "$j" 2> "$tmp/$name/javac.err"; then echo "FAIL  $name (javac)"; cat "$tmp/$name/javac.err"; fail=$((fail+1)); continue; fi
  java -cp "$tmp/$name/cls" "$name" > "$tmp/$name/expected.out" 2>&1
  flags="$(sed -n 's|^// flags: ||p' "$j" | head -1)"
  # shellcheck disable=SC2086
  if ! "$conv" --file "$j" --out "$tmp/$name/sl" --quiet $flags; then
    echo "FAIL  $name (converter failed closed)"; "$conv" --file "$j" --out "$tmp/$name/sl" $flags | sed 's/^/      /'; fail=$((fail+1)); continue
  fi
  (cd "$tmp/$name/sl" && timeout 300 "$sleela" run "$name.sleela" 2>&1 | filter) > "$tmp/$name/actual.out"
  if diff -u "$tmp/$name/expected.out" "$tmp/$name/actual.out" > "$tmp/$name/diff"; then
    printf 'PASS  %-12s %s lines identical to the JDK\n' "$name" "$(wc -l < "$tmp/$name/expected.out")"; pass=$((pass+1))
  else
    printf 'FAIL  %s\n' "$name"; head -40 "$tmp/$name/diff"; fail=$((fail+1))
  fi
done

# fail-closed checks: each must be refused with a diagnostic, and must emit nothing
for j in "$tests"/reject/*.java; do
  [ -e "$j" ] || continue
  name="$(basename "$j" .java)"
  want="$(sed -n 's|^// expect: ||p' "$j" | head -1)"
  mkdir -p "$tmp/rej/$name"
  out="$("$conv" --file "$j" --out "$tmp/rej/$name" 2>&1)"; rc=$?
  produced="$(ls "$tmp/rej/$name"/*.sleela 2>/dev/null | wc -l)"
  if [ "$rc" -eq 1 ] && [ "$produced" -eq 0 ] && printf '%s' "$out" | grep -q "\[$want\]"; then
    printf 'PASS  %-12s refused [%s], no output written\n' "reject/$name" "$want"; pass=$((pass+1))
  else
    printf 'FAIL  reject/%s (rc=%s produced=%s wanted [%s])\n%s\n' "$name" "$rc" "$produced" "$want" "$out"; fail=$((fail+1))
  fi
done

echo "javaconv differential: pass=$pass fail=$fail"
[ "$fail" -eq 0 ]
