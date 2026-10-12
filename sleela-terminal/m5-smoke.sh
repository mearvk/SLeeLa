#!/bin/sh
set -eu
SLSH=${1:-./build/slsh}
ROOT=$(pwd)
case "$SLSH" in
  /*) SLSH_ABS=$SLSH ;;
  *)  SLSH_ABS=$ROOT/$SLSH ;;
esac

printf '1\n' | "$SLSH" -c 'PS3="pick> "; select x in alpha beta; do echo "$x:$REPLY"; break; done' 2>/tmp/select.err >/tmp/select.out
cat /tmp/select.out
cat /tmp/select.err

test "$(printf '%s\n' 'cat <(printf hi)' | "$SLSH")" = hi

# Verify ordinary utilities are resolved as external executables through PATH.
test "$(printf '%s\n' 'printf external-ok' | "$SLSH")" = external-ok
test "$(printf '%s\n' 'uname -s' | "$SLSH")" != ""

# Pipeline status is the status of the final command, and $? reflects it.
test "$(printf '%s\n' 'false | true; echo $?' | "$SLSH")" = 0
test "$(printf '%s\n' 'true | false; echo $?' | "$SLSH")" = 1

# exit is a shell builtin and must terminate the shell with its requested status.
set +e
"$SLSH" -c 'printf before; exit 7; printf after' >/tmp/exit.out 2>/tmp/exit.err
exit_status=$?
set -e
test "$exit_status" -eq 7
test "$(cat /tmp/exit.out)" = before

test "$(printf '%s\n' 'printf hi > >(cat)' | "$SLSH")" = hi

test "$(printf '%s\n' "trap 'echo trapped' USR1" 'kill -USR1 $$' | "$SLSH")" = trapped

tmpdir=$(mktemp -d)
trap 'rm -rf "$tmpdir" /tmp/select.err /tmp/select.out /tmp/exit.out /tmp/exit.err' EXIT
mkdir -p "$tmpdir/a/include" "$tmpdir/b/include"
touch "$tmpdir/a/include/x.hpp" "$tmpdir/b/include/y.hpp" "$tmpdir/a/include/evil;echo-INJECTED.hpp"
actual=$(cd "$tmpdir" && "$SLSH_ABS" -c 'echo */include/*.hpp')
case "$actual" in
  *a/include/x.hpp* ) : ;;
  * ) echo "multi-segment glob failed: $actual" >&2; exit 1 ;;
esac

# A metacharacter-bearing filename must remain data; it must never become a
# second command through source rewriting.
actual=$(cd "$tmpdir" && "$SLSH_ABS" -c 'printf "%s\\n" */include/*')
case "$actual" in
  *'evil;echo-INJECTED.hpp'*) : ;;
  *) echo "metacharacter filename was lost: $actual" >&2; exit 1 ;;
esac
if printf 'PS3="pick> "\\n' | "$SLSH" -c 'cat >/dev/null' 2>/dev/null; then :; fi

echo 'M5 smoke: PASS'
