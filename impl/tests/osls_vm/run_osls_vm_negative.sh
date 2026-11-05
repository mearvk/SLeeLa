#!/usr/bin/env bash
set -euo pipefail
if [[ $# -ne 1 ]]; then
  echo "usage: run_osls_vm_negative.sh /path/to/sleela" >&2
  exit 2
fi
sleela_bin="$1"
tmp_base="$(mktemp "${TMPDIR:-/tmp}/sleela-osls.XXXXXX")"
tmp="${tmp_base}.sleela"
mv "$tmp_base" "$tmp"
log="$(mktemp "${TMPDIR:-/tmp}/sleela-osls-log.XXXXXX")"
cleanup() { rm -f "$tmp" "$log"; }
trap cleanup EXIT
cat >"$tmp" <<'SLEELA'
#sleela 2.5
material;
class OSLSInvalidArgumentTest {
  void main() {
    String result = oslsGetFileInfo(42);
    print(result);
  }
}
SLEELA
if "$sleela_bin" run "$tmp" >"$log" 2>&1; then
  echo "OSLS type-safety test failed: invalid integer path was accepted" >&2
  exit 1
fi
if ! grep -q "OSLS binding argument type mismatch" "$log"; then
  cat "$log" >&2
  echo "OSLS type-safety test failed: invocation did not fail at the typed VM boundary" >&2
  exit 1
fi
echo "OSLS VM invalid-argument rejection: PASS"
