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
touch "$tmpdir/a/include/x.hpp" "$tmpdir/b/include/y.hpp"
actual=$(cd "$tmpdir" && "$ROOT/$SLSH" -c 'echo */include/*.hpp')
case "$actual" in
  *a/include/x.hpp* ) : ;;
  * ) echo "multi-segment glob failed: $actual" >&2; exit 1 ;;
esac

echo 'M5 smoke: PASS'
