#!/usr/bin/env bash
# Whole-tree semantic check of javaconv output.
#
# "Converted" only means javaconv accepted the Java file. This script asks the
# stronger question: does the emitted SLeeLa pass the semantic checker once all
# emitted files are linked together?
#
# `sleela check` takes one file and does not link siblings, so the tree is
# checked with `sleela run <stub> <every emitted file>` (a no-op main), which
# links and semantically checks everything. Semantic errors carry no file name,
# so each file is attributed its errors by leave-one-out: an error message that
# disappears when the file is removed from the linked set belongs to that file
# (multiset difference, so identical messages from several files count right).
#
# usage: tests/javaconv_check_tree.sh <javaconv-out-dir> [path-to-sleela] [report-dir]
# Exit 0: every emitted file is clean. Exit 1: some files fail the checker. Exit 2: usage/IO.
set -u
out="${1:-}"
[ -n "$out" ] && [ -f "$out/.javaconv-emitted" ] || { echo "usage: $0 <javaconv-out-dir> [sleela] [report-dir]  (needs <dir>/.javaconv-emitted)"; exit 2; }
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/.." && pwd)"
sleela="${2:-$root/impl/build/sleela}"
[ -x "$sleela" ] || { echo "sleela not built: $sleela"; exit 2; }
sleela="$(cd "$(dirname "$sleela")" && pwd)/$(basename "$sleela")"
out="$(cd "$out" && pwd)"
rep="${3:-$out/check-tree}"
mkdir -p "$rep/loo" || exit 2
rep="$(cd "$rep" && pwd)"
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib" SLEELA_SHEET="$root/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="${SLEELA_SHA256_MANIFEST:-$root/security/sha256-manifest.json}"

cat > "$rep/Stub.sleela" <<'EOF'
#sleela 3.2
import java.jdk-28;
material;

class JCheckStub {
  static void main() {
  }
}
EOF

grep '\.sleela$' "$out/.javaconv-emitted" | sort > "$rep/files.txt"
total="$(wc -l < "$rep/files.txt")"

# Messages only, sorted, one per line (the gate prints its own banner lines).
errs() { grep -a 'Semantic error' | sed 's/.*Semantic error: //' | sort; }

# $1 = file to leave out ("" for none)
run_set() {
  (cd "$out" && grep -vxF -- "$1" "$rep/files.txt" | xargs -d '\n' timeout 600 "$sleela" run "$rep/Stub.sleela" 2>&1 | errs)
}

run_set "" > "$rep/baseline.txt"
base_n="$(wc -l < "$rep/baseline.txt")"
export out rep sleela
export -f errs run_set

# Leave-one-out, in parallel. Attributed errors = baseline lines that vanish.
one() {
  f="$1"
  key="$(printf '%s' "$f" | tr '/' '_')"
  run_set "$f" > "$rep/loo/$key.txt"
  comm -23 "$rep/baseline.txt" "$rep/loo/$key.txt" > "$rep/loo/$key.own"
  printf '%s\t%s\n' "$(wc -l < "$rep/loo/$key.own")" "$f"
}
export -f one
xargs -d '\n' -P "$(nproc)" -I{} bash -c 'one "$1"' _ {} < "$rep/files.txt" | sort -t$'\t' -k1,1nr -k2 > "$rep/per-file.tsv"

bad="$(awk -F'\t' '$1>0' "$rep/per-file.tsv" | wc -l)"
clean=$((total - bad))
{
  echo "# javaconv whole-tree semantic check"
  echo
  echo "- emitted SLeeLa files: $total"
  echo "- baseline semantic errors (all linked): $base_n"
  echo "- files with at least one attributed error: $bad"
  echo "- files with none: $clean"
  echo
  echo "Attribution is leave-one-out; errors that only appear when a dependency is"
  echo "missing are not counted, and a cascade from one bad file may land on its dependents."
  echo
  echo "## Error kinds (all linked, names generalised)"
  sed -E "s/'[^']*'/'X'/g" "$rep/baseline.txt" | sort | uniq -c | sort -rn | head -25
} > "$rep/REPORT.md"
cat "$rep/REPORT.md"
[ "$bad" -eq 0 ]
