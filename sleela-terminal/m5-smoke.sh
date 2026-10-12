#!/bin/sh
set -eu
SLSH=${1:-./build/slsh}
ROOT=$(pwd)
tmpdir=$(mktemp -d)
cleanup() {
  rm -rf "$tmpdir"
}
trap cleanup EXIT
trap 'exit 1' HUP INT TERM
find_m5_fifos() {
  for path in /tmp/slsh-m5-*; do
    [ -p "$path" ] && printf '%s\n' "$path"
  done | sort
}
case "$SLSH" in
  /*) SLSH_ABS=$SLSH ;;
  *)  SLSH_ABS=$ROOT/$SLSH ;;
esac

printf '1\n' | "$SLSH" -c 'PS3="pick> "; select x in alpha beta; do echo "$x:$REPLY"; break; done' 2>"$tmpdir/select.err" >"$tmpdir/select.out"
cat "$tmpdir/select.out"
cat "$tmpdir/select.err"

# Process-substitution FIFOs must be removed after the owning command exits.
fifo_before=$(find_m5_fifos)
test "$(printf '%s\n' 'cat <(printf hi)' | "$SLSH")" = hi

# Verify ordinary utilities are resolved as external executables through PATH.
test "$(printf '%s\n' 'printf external-ok' | "$SLSH")" = external-ok
test "$(printf '%s\n' 'uname -s' | "$SLSH")" != ""

# Pipeline status is the status of the final command, and $? reflects it.
test "$(printf '%s\n' 'false | true; echo $?' | "$SLSH")" = 0
test "$(printf '%s\n' 'true | false; echo $?' | "$SLSH")" = 1

# The help index and feature topics are available in non-interactive mode too.
"$SLSH" -c 'help' >"$tmpdir/help.txt"
grep -q 'Topics: expansion, control, jobs, io, process, traps, select, prompt, soros' "$tmpdir/help.txt"
"$SLSH" -c 'help expansion' >"$tmpdir/help-expansion.txt"
grep -q 'quote fidelity has known gaps' "$tmpdir/help-expansion.txt"
if "$SLSH" -c 'help no-such-topic' >"$tmpdir/help-invalid.txt" 2>&1; then
  echo "help accepted an unknown topic" >&2
  exit 1
fi


# exit is a shell builtin and must terminate the shell with its requested status.
set +e
"$SLSH" -c 'printf before; exit 7; printf after' >"$tmpdir/exit.out" 2>"$tmpdir/exit.err"
exit_status=$?
set -e
test "$exit_status" -eq 7
test "$(cat "$tmpdir/exit.out")" = before

test "$(printf '%s\n' 'printf hi > >(cat)' | "$SLSH")" = hi
fifo_after=$(find_m5_fifos)
test "$fifo_before" = "$fifo_after" || {
  echo "process-substitution FIFO leaked into /tmp" >&2
  echo "before: $fifo_before" >&2
  echo "after:  $fifo_after" >&2
  exit 1
}

test "$(printf '%s\n' "trap 'echo trapped' USR1" 'kill -USR1 $$' | "$SLSH")" = trapped

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
