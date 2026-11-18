#!/usr/bin/env bash
# Integration regression: deterministic repeat-run mtimes and rollback on failure.
set -euo pipefail
here="$(cd "$(dirname "$0")/.." && pwd)"
root="$(cd "$here/../../.." && pwd)"
JCONV_IN="${JCONV_IN:-$here/tests/fixtures}"
export JCONV_IN
tmp="$(mktemp -d)"
trap 'rm -rf "$tmp"' EXIT
export JCONV_OUT="$tmp/published"
export JCONV_SCOPE="${JCONV_SCOPE:-all}"
export JCONV_BATCH="${JCONV_BATCH:-50}"
export JCONV_REVISION="${JCONV_REVISION:-repeat-run-test}"
runner="$here/run-converter.sh"
bin="${SLEELA_BIN:-$root/impl/build/sleela}"
snapshot() {
  python3 - "$JCONV_OUT" <<'PY'
import hashlib, pathlib, sys
root = pathlib.Path(sys.argv[1])
for p in sorted(x for x in root.rglob("*") if x.is_file()):
    h = hashlib.sha256(p.read_bytes()).hexdigest()
    print(f"{p.relative_to(root).as_posix()}\t{h}\t{p.stat().st_mtime_ns}")
PY
}
"$runner" "$bin"
snapshot > "$tmp/first"
"$runner" "$bin"
snapshot > "$tmp/second"
diff -u "$tmp/first" "$tmp/second"
# A failed invocation must not replace the last complete generation.
if "$runner" "$tmp/does-not-exist" >/dev/null 2>&1; then
  echo "repeat-run: expected the deliberately missing runtime to fail" >&2
  exit 1
fi
snapshot > "$tmp/after-failure"
diff -u "$tmp/second" "$tmp/after-failure"
python3 "$here/verify-provenance.py" "$JCONV_OUT/CONVERSION-PROVENANCE.json" "$root" "$JCONV_IN" "$JCONV_OUT"
echo "repeat-run: PASS (identical bytes, paths, mtimes; failed run preserved prior generation)"
