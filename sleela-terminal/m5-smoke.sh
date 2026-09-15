#!/bin/sh
set -eu
SLSH=${1:-./build/slsh}
ROOT=$(pwd)

printf '1\n' | "$SLSH" -c 'PS3="pick> "; select x in alpha beta; do echo "$x:$REPLY"; break; done' 2>/tmp/select.err >/tmp/select.out
cat /tmp/select.out
cat /tmp/select.err

test "$(printf '%s\n' 'cat <(printf hi)' | "$SLSH")" = hi

test "$(printf '%s\n' 'printf hi > >(cat)' | "$SLSH")" = hi

test "$(printf '%s\n' "trap 'echo trapped' USR1" 'kill -USR1 $$' | "$SLSH")" = trapped

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir" /tmp/select.err /tmp/select.out' EXIT
mkdir -p "$tmpdir/a/include" "$tmpdir/b/include"
touch "$tmpdir/a/include/x.hpp" "$tmpdir/b/include/y.hpp" "$tmpdir/a/include/evil;echo-INJECTED.hpp"
actual=$(cd "$tmpdir" && "$ROOT/$SLSH" -c 'echo */include/*.hpp')
case "$actual" in
  *a/include/x.hpp* ) : ;;
  * ) echo "multi-segment glob failed: $actual" >&2; exit 1 ;;
esac

# A metacharacter-bearing filename must remain data; it must never become a
# second command through source rewriting.
actual=$(cd "$tmpdir" && "$ROOT/$SLSH" -c 'printf "%s\\n" */include/*')
case "$actual" in
  *'evil;echo-INJECTED.hpp'*) : ;;
  *) echo "metacharacter filename was lost: $actual" >&2; exit 1 ;;
esac
if printf 'PS3="pick> "\\n' | "$SLSH" -c 'cat >/dev/null' 2>/dev/null; then :; fi

echo 'M5 smoke: PASS'
