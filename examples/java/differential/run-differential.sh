#!/usr/bin/env bash
# Differential tests: run each <Name>.java on a real JDK and <Name>.sleela on the
# SLeeLa VM, and require identical output. usage: run-differential.sh [sleela]
set -u
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../../.." && pwd)"
sleela="${1:-}"
[ -z "$sleela" ] && { [ -x "$root/impl/build/sleela" ] && sleela="$root/impl/build/sleela" || sleela="$root/working-binaries/sleela"; }
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib"
tmp="$(mktemp -d)"
status=0
for j in "$here"/*.java; do
  name="$(basename "$j" .java)"
  javac -d "$tmp" "$j" && java -cp "$tmp" "$name" > "$tmp/$name.java.out" 2>&1
  (cd "$here" && "$sleela" run "$name.sleela" 2>&1 | grep -v 'memory-manager\|^verification\|\[security\]\|\[defender\]') > "$tmp/$name.sleela.out"
  if diff -u "$tmp/$name.java.out" "$tmp/$name.sleela.out" > "$tmp/$name.diff"; then
    printf 'PASS  %-28s %s lines identical\n' "$name" "$(wc -l < "$tmp/$name.java.out")"
  else
    printf 'FAIL  %s\n' "$name"; cat "$tmp/$name.diff"; status=1
  fi
done
rm -rf "$tmp"
exit $status
