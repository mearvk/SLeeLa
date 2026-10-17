#!/bin/sh
# SLeeLa toolchain autocheck: verify the local build and security assets,
# then search for stale verifier-discovery patterns across build files.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$ROOT"
fail=0
echo "== SLeeLa autocheck: root and toolchain =="
for p in tools/verify-before-execution.py security/sha256-manifest.json impl/Makefile \
         impl/xlt/Makefile lib/xlt/README.md lib/xlt/examples/epyc-ddr2.xlt; do
  if [ -f "$p" ]; then echo "PASS: $p"; else echo "FAIL: missing $p"; fail=1; fi
done
if [ -x impl/build/sleela ]; then
  echo "INFO: local executable: $ROOT/impl/build/sleela"
else
  echo "WARN: impl/build/sleela is not built yet"
fi
if [ -x impl/xlt/build/xltc ]; then
  echo "INFO: local executable: $ROOT/impl/xlt/build/xltc"
else
  echo "WARN: impl/xlt/build/xltc is not built yet (run scripts/clean-build.sh)"
fi
echo "== GUI version lines: each line carries an SLPackage.sleela manifest =="
for g in lib/gui/version-mual lib/gui/version-gual lib/gui/version-thual lib/gui/version-dual lib/gui/version-military; do
  if [ -d "$g" ]; then
    if [ -f "$g/SLPackage.sleela" ]; then echo "PASS: $g/SLPackage.sleela"; else echo "FAIL: missing $g/SLPackage.sleela"; fail=1; fi
  else
    echo "WARN: GUI version line not present: $g"
  fi
done
if command -v python3 >/dev/null 2>&1; then
  python3 -m json.tool security/sha256-manifest.json >/dev/null 2>&1 && echo "PASS: manifest JSON parses" || { echo "FAIL: manifest JSON invalid"; fail=1; }
else
  echo "WARN: python3 not found; skipped manifest JSON parse"
fi
echo "== Search: stale verifier-discovery diagnostics and hard-coded paths =="
if grep -R -n --include='Makefile' --include='*.mk' --include='*.cpp' --include='*.h' --include='*.py'   -E 'searched upward from|verification tool not found|SLEELA_HOME.*security/sha256-manifest' impl tools . 2>/dev/null; then
  echo "INFO: matches above; review for stale path assumptions"
else
  echo "INFO: no matching stale verifier diagnostics found"
fi
exit "$fail"
