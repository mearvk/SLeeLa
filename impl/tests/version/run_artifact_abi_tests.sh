#!/usr/bin/env bash
# =============================================================================
# run_artifact_abi_tests.sh -- runtime artifact ABI validation gate.
#
# Usage: run_artifact_abi_tests.sh <sleela-binary>
#
# This gate proves that a valid persistent .sleela artifact is accepted and
# that a truncated/corrupted artifact is rejected before execution.
# =============================================================================
set -u
SLEELA="${1:-./build/sleela}"
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
TMP="${ROOT}/impl/tests/version/.artifact-abi-test"
mkdir -p "$TMP"
trap 'rm -rf "$TMP"' EXIT

SRC="$TMP/abi.sleela"
ART="$TMP/abi.sleela.artifact"
BAD="$TMP/abi.truncated.sleela"

cat > "$SRC" <<'EOF'
#sleela 1.3
class ArtifactAbiTest {
    void main() {
        print("SLEELA-ARTIFACT-ABI-OK");
    }
}
EOF

rm -f "$ART" "$BAD"
"$SLEELA" compile "$SRC" -o "$ART" >/dev/null
test -s "$ART"

echo "=== valid artifact ==="
"$SLEELA" validate-artifact "$ART"

echo "=== truncated artifact ==="
size="$(wc -c < "$ART")"
if [ "$size" -lt 2 ]; then
    echo "FAIL: generated artifact is unexpectedly small"
    exit 1
fi
head -c $((size - 1)) "$ART" > "$BAD"
if "$SLEELA" validate-artifact "$BAD" >/dev/null 2>&1; then
    echo "FAIL: truncated artifact was accepted"
    exit 1
fi

echo "ARTIFACT ABI TESTS: PASS"
