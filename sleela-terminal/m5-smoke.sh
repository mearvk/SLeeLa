#!/bin/sh
set -eu
SLSH=${1:-./build/slsh}

printf '1\n' | "$SLSH" -c 'PS3="pick> "; select x in alpha beta; do echo "$x:$REPLY"; break; done' 2>/tmp/select.err >/tmp/select.out
cat /tmp/select.out
cat /tmp/select.err

test "$(printf '%s\n' 'cat <(printf hi)' | "$SLSH")" = hi

test "$(printf '%s\n' 'printf hi > >(cat)' | "$SLSH")" = hi

test "$(printf '%s\n' "trap 'echo trapped' USR1" 'kill -USR1 $$' | "$SLSH")" = trapped

rm -f /tmp/select.err /tmp/select.out
echo 'M5 smoke: PASS'
