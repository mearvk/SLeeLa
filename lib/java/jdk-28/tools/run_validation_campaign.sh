#!/usr/bin/env bash
# Full JDK 28 API/library validation campaign.
# Run from any directory; requires Python 3, a JDK (25+), and a source-built SLeeLa runtime.
set -uo pipefail
root="$(cd "$(dirname "$0")/../../../.. && pwd)"
bin="${SLEELA_BIN:-$root/impl/build/sleela}"
report_dir="${JDK28_REPORT_DIR:-$root/build/jdk28-validation}"
mkdir -p "$report_dir"
cd "$root"
overall=0
passed=0
failed=0
partial=0

run_gate() {
  name="$1"; shift
  log="$report_dir/$name.log"
  echo
  echo "=== $name ==="
  if "$@" >"$log" 2>&1; then
    echo "PASS: $name"
    passed=$((passed + 1))
  else
    rc=$?
    echo "FAIL: $name (exit $rc)"
    failed=$((failed + 1))
    overall=1
  fi
  tail -n 24 "$log" || true
}

run_partial() {
  name="$1"; shift
  log="$report_dir/$name.log"
  echo
  echo "=== $name (partial-results diagnostic) ==="
  "$@" >"$log" 2>&1
  rc=$?
  if [ "$rc" -eq 0 ]; then
    echo "PASS: $name (no functional gaps reported)"
    passed=$((passed + 1))
  elif [ "$rc" -eq 2 ]; then
    echo "PARTIAL: $name reports known behavioral gaps; this is not a pass"
    partial=$((partial + 1))
  else
    echo "FAIL: $name (unexpected exit $rc)"
    failed=$((failed + 1))
    overall=1
  fi
  tail -n 30 "$log" || true
}

if [ ! -x "$bin" ]; then
  echo "Missing executable SLeeLa runtime: $bin" >&2
  exit 2
fi
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib" SLEELA_BIN="$bin"

# Build the authoritative API inventory from the actual JDK 28 runtime.
# JDK 25 files remain a comparison baseline only; they are never substituted
# for the JDK 28 reflection output in this campaign.
java_spec="$(java -XshowSettings:properties -version 2>&1 | sed -n 's/^[[:space:]]*java.specification.version = //p' | head -n 1)"
if [ "$java_spec" != "28" ]; then
  echo "JDK 28 is required for this campaign; detected java.specification.version=$java_spec" >&2
  exit 2
fi
reflection_dir="$report_dir/jdk28-reflection"
reflection_classes="$reflection_dir/classes"
mkdir -p "$reflection_classes"
javac -d "$reflection_classes" \
  "$root/lib/java/jdk-25/tools/ApiDump.java" \
  "$root/lib/java/jdk-25/tools/ApiSignatures.java"
java -cp "$reflection_classes" ApiDump > "$reflection_dir/jdk-api.jsonl" 2> "$reflection_dir/api-dump.stderr.log"
java -cp "$reflection_classes" ApiSignatures > "$reflection_dir/jdk-signatures.jsonl" 2> "$reflection_dir/signature-dump.stderr.log"
python3 - "$reflection_dir/jdk-api.jsonl" "$reflection_dir/jdk-signatures.jsonl" <<'PY'
import json, sys
for path in sys.argv[1:]:
    with open(path, encoding="utf-8") as stream:
        rows = [json.loads(line) for line in stream if line.strip()]
    if not rows:
        raise SystemExit(f"JDK 28 reflection produced no records: {path}")
    print(f"{path}: {len(rows)} reflected records")
api = [json.loads(line) for line in open(sys.argv[1], encoding="utf-8") if line.strip()]
sigs = [json.loads(line) for line in open(sys.argv[2], encoding="utf-8") if line.strip()]
api_names = {row["name"] for row in api}
sig_names = {row["name"] for row in sigs}
missing = sorted(api_names - sig_names)
if missing:
    raise SystemExit(f"JDK 28 signature reflection omitted {len(missing)} reflected types; first: {missing[:10]}")
print(f"JDK 28 reflection parity: {len(api_names)} API types have signature records")
PY
export JDK28_API_JSONL="$reflection_dir/jdk-api.jsonl"
export JDK28_SIGNATURES_JSONL="$reflection_dir/jdk-signatures.jsonl"

run_gate "jdk-signature-index-tests" python3 -m unittest discover -s "$root/lib/java/jdk-25/tools" -p "test_jdk_api_signatures.py"
run_gate "jdk-signature-index-audit" python3 "$root/lib/java/jdk-25/tools/jdk_api_signatures.py" --audit
run_gate "jdk28-generator-tests" python3 -m unittest discover -s "$root/lib/java/jdk-28/tools" -p "test_generate_jdk28.py"
run_gate "jdk28-generator-syntax" python3 -m py_compile "$root/lib/java/jdk-28/tools/generate_jdk28.py" "$root/lib/java/jdk-25/tools/jdk_api_signatures.py"
run_gate "jdk28-signature-index-audit" python3 "$root/lib/java/jdk-28/tools/generate_jdk28.py" --audit
run_gate "jdk28-regenerate" python3 "$root/lib/java/jdk-28/tools/generate_jdk28.py"
run_gate "jdk28-generated-diff-check" git diff --check -- lib/java/jdk-28
run_gate "source-integrity-manifest-refresh" python3 "$root/tools/generate-sha256-manifest.py" --root "$root" --output "$root/security/sha256-manifest.json"
run_gate "jdk28-inventory" python3 "$root/lib/java/jdk-28/tools/validate_jdk28.py"
run_gate "java-library-manifest" python3 "$root/lib/java/validate.py"
run_gate "jdk28-api-envelope-contract" python3 "$root/tests/jdk28_functional_equivalence.py"
run_gate "jdk28-compiler-check" env SLEELA_BIN="$bin" REPORT="$report_dir/CHECK-REPORT.txt" bash "$root/lib/java/jdk-28/tools/check_jdk28.sh" "${JDK28_CHECK_JOBS:-8}"
run_gate "java-conformance" bash "$root/examples/java/run-conformance.sh" "$bin"
run_gate "java-differential" bash "$root/examples/java/differential/run-differential.sh" "$bin"
run_gate "converter-repeat-transaction" env SLEELA_BIN="$bin" bash "$root/lib/java/converter/tests/repeat-run-transaction.sh"
run_gate "converter-per-member-stub-policy" env SLEELA_BIN="$bin" bash "$root/lib/java/converter/tests/per-member-stub-policy.sh"
run_gate "jdk28-tier-coverage-comparison" python3 "$root/lib/java/jdk-25/tools/jdk_api_coverage.py" --tier jdk-28 --markdown "$report_dir/COVERAGE-JDK28-REFERENCE.md"

# The functional gate is deliberately separate: the JDK 28 tree contains many
# exact API declarations whose bodies are still fail-loud stubs. Exit 2 is an
# honest partial result, not a campaign pass and not a reason to hide the report.
run_partial "jdk28-functional-equivalence" python3 "$root/tests/jdk28_functional_equivalence.py" --functional

# Keep the regenerated output reviewable without silently committing it.
git diff --stat -- lib/java/jdk-28 > "$report_dir/GENERATED-DIFF-STAT.txt" || true
git diff --binary -- lib/java/jdk-28 > "$report_dir/GENERATED-JDK28.patch" || true

{
  echo "# JDK 28 validation campaign"
  echo
  echo "- Runtime: $("$bin" version 2>/dev/null | head -n 1)"
  echo "- Gates passed: $passed"
  echo "- Gates failed: $failed"
  echo "- Functional-equivalence diagnostics marked partial: $partial"
  echo "- Detailed logs: $report_dir"
  echo
  echo "A partial functional-equivalence result is not a full JDK behavior pass."
  echo "API identity, signature presence, compiler checks, and behavioral coverage are reported separately."
} > "$report_dir/SUMMARY.md"

echo
cat "$report_dir/SUMMARY.md"
exit "$overall"
