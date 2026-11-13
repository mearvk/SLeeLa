#!/usr/bin/env bash
# Run the lib/java Java SE/JDK 28 conformance programs and report each summary.
#
# usage: examples/java/run-conformance.sh [path/to/sleela]
#
# Uses the source-built compiler (impl/build/sleela) by default, falling back to
# working-binaries/sleela. Exits non-zero when any program reports a failure or
# does not reach its summary line.
set -u
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../.." && pwd)"
sleela="${1:-}"
if [ -z "$sleela" ]; then
  if [ -x "$root/impl/build/sleela" ]; then sleela="$root/impl/build/sleela"; else sleela="$root/working-binaries/sleela"; fi
fi
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib"
status=0
for program in "$here"/Java*.sleela; do
  name="$(basename "$program" .sleela)"
  summary="$(cd "$here" && "$sleela" run "$(basename "$program")" 2>&1 | grep -v 'memory-manager\|^verification\|\[security\]\|\[defender\]')"
  last="$(printf '%s\n' "$summary" | tail -n 1)"
  case "$last" in
    "failures=0"|*" misses=0") printf 'PASS  %-34s %s\n' "$name" "$last" ;;
    *) printf 'FAIL  %-34s\n%s\n' "$name" "$summary"; status=1 ;;
  esac
done
exit $status
