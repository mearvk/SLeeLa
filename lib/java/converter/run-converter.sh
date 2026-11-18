#!/usr/bin/env bash
# Whole-run transactional Java -> SLeeLa conversion (native converter).
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$here/../../.." && pwd)"
sleela="${1:-}"
if [ -z "$sleela" ]; then
  if [ -x "$root/impl/build/sleela" ]; then sleela="$root/impl/build/sleela"; else sleela="$root/working-binaries/sleela"; fi
fi
export SLEELA_HOME="$root" SLEELA_LIB="$root/lib"
: "${JCONV_IN:?set JCONV_IN to the root of the Java source tree}"
case "$JCONV_IN" in /*) ;; *) JCONV_IN="$(cd "$JCONV_IN" && pwd)";; esac
export JCONV_IN
JCONV_OUT="${JCONV_OUT:-$PWD/jdk28-converted}"
mkdir -p "$(dirname "$JCONV_OUT")"
case "$JCONV_OUT" in /*) ;; *) JCONV_OUT="$(cd "$(dirname "$JCONV_OUT")" && pwd)/$(basename "$JCONV_OUT")";; esac
FINAL_OUT="$JCONV_OUT"
export JCONV_OUT

if [ -L "$JCONV_OUT" ]; then echo "jconv: refusing symlink output root" >&2; exit 2; fi
parent="$(dirname "$JCONV_OUT")"
base="$(basename "$JCONV_OUT")"
stage="$(mktemp -d "$parent/.${base}.jconv-stage.XXXXXX")"
backup="$parent/.${base}.jconv-backup.$$"
published=0
cleanup() {
  rc=$?
  if [ "$published" -eq 0 ] && [ -d "$backup" ] && [ ! -e "$FINAL_OUT" ]; then mv "$backup" "$FINAL_OUT" || true; fi
  [ -d "$stage" ] && rm -rf "$stage"
  exit "$rc"
}
trap cleanup EXIT INT TERM
if [ -e "$JCONV_OUT" ]; then
  [ -d "$JCONV_OUT" ] || { echo "jconv: output exists and is not a directory" >&2; exit 2; }
  if find "$JCONV_OUT" -type l -print -quit | grep -q .; then echo "jconv: refusing symlink in previous output tree" >&2; exit 2; fi
  if find "$JCONV_OUT" ! -type d ! -type f -print -quit | grep -q .; then echo "jconv: refusing special file in previous output tree" >&2; exit 2; fi
  cp -a "$JCONV_OUT/." "$stage/"
fi
export JCONV_OUT="$stage"
cd "$here"
unset JCONV_RESUME_AFTER JCONV_FAILED_WHY JCONV_RESUME_BATCH
rm -f "$JCONV_OUT/.jconv-progress" "$JCONV_OUT/.jconv-state"
log="$(mktemp)"
restarts=0
prog=""
while :; do
  "$sleela" run JConvMain.sleela 2>&1 |
    grep -v -E '^\[memory-manager\]|^verification|\[security\]|\[defender\]' | tee "$log" || true
  prog="$(head -n 1 "$JCONV_OUT/.jconv-progress" 2>/dev/null || true)"
  if [ -z "$prog" ] || [ "$prog" = "DONE" ]; then break; fi
  case "$prog" in
    "BATCH "*)
      if [ "${JCONV_RESUME_BATCH:-}" = "1" ] && [ "${prog#BATCH }" = "${JCONV_RESUME_AFTER:-}" ]; then
        echo "jconv: a batch run stopped before converting a file; refusing publication" >&2; exit 1
      fi
      export JCONV_RESUME_AFTER="${prog#BATCH }" JCONV_RESUME_BATCH=1
      unset JCONV_FAILED_WHY
      continue
      ;;
  esac
  unset JCONV_RESUME_BATCH
  if [ "$prog" = "${JCONV_RESUME_AFTER:-}" ]; then echo "jconv: resumed run stopped before reaching a new file; refusing publication" >&2; exit 1; fi
  restarts=$((restarts + 1))
  if [ "$restarts" -gt 10000 ]; then echo "jconv: restart limit exceeded; refusing publication" >&2; exit 1; fi
  why="$(grep -m 1 -E 'runtime error|error:' "$log" | sed -e 's/^sleelavc: //' | tr '\t' ' ' || true)"
  [ -n "$why" ] || why="the VM stopped"
  echo "jconv: the VM stopped in $prog ($why); restarting after it"
  export JCONV_RESUME_AFTER="$prog" JCONV_FAILED_WHY="VM stopped: $why"
done
rm -f "$log" "$JCONV_OUT/.jconv-state"
[ "$prog" = "DONE" ] || { echo "jconv: conversion did not reach DONE; refusing publication" >&2; exit 1; }
rm -f "$JCONV_OUT/.jconv-progress"
rm -rf "$here/docs"

python3 - "$root" "$JCONV_IN" "$JCONV_OUT" <<'PY'
import hashlib, json, os, pathlib, sys
root, src, out = map(pathlib.Path, sys.argv[1:])
def digest(path):
    h = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""): h.update(block)
    return h.hexdigest()
def tree_files(base, suffix=None):
    return sorted(p for p in base.rglob("*") if p.is_file() and not p.is_symlink()
                  and (suffix is None or p.name.endswith(suffix)))
inputs = tree_files(src, ".java")
model = pathlib.Path(os.environ.get("JCONV_MODEL", str(root / "impl/subjects/conversion/conversion.model")))
converter = sorted(list((root / "lib/java/converter").glob("*.sleela")) + [root / "lib/java/converter/run-converter.sh", root / "lib/java/converter/verify-provenance.py"])
outputs = [p for p in tree_files(out) if p.name != "CONVERSION-PROVENANCE.json"]
release = src / "release"
jdk_identity = os.environ.get("JCONV_JDK_ID", "")
if not jdk_identity and release.is_file():
    jdk_identity = next((line.split("=",1)[1].strip().strip('"') for line in release.read_text(errors="replace").splitlines() if line.startswith("JAVA_VERSION=")), "")
if not jdk_identity: jdk_identity = "unverified-source-tree"
manifest = {
  "format": "sleela-java-converter-provenance-v1",
  "converter": "lib/java/converter",
  "converterRevision": os.environ.get("JCONV_REVISION", "working-tree-or-unpinned"),
  "jdkIdentity": jdk_identity,
  "jdkIdentityVerified": bool(release.is_file() or os.environ.get("JCONV_JDK_ID")),
  "options": {k: os.environ.get(k, "") for k in ("JCONV_SCOPE","JCONV_FILTER","JCONV_LIMIT","JCONV_INT","JCONV_COMMENTS","JCONV_BUNDLE","JCONV_BATCH")},
  "model": {"path": "impl/subjects/conversion/conversion.model", "sha256": digest(model)},
  "converterSources": [{"path": p.relative_to(root).as_posix(), "sha256": digest(p)} for p in converter],
  "inputs": [{"path": p.relative_to(src).as_posix(), "sha256": digest(p)} for p in inputs],
  "outputs": [{"path": p.relative_to(out).as_posix(), "sha256": digest(p)} for p in outputs],
}
manifest_path = out / "CONVERSION-PROVENANCE.json"
manifest_bytes = (json.dumps(manifest, sort_keys=True, indent=2) + "\n").encode()
if not manifest_path.is_file() or manifest_path.read_bytes() != manifest_bytes:
    manifest_path.write_bytes(manifest_bytes)
PY
python3 "$here/verify-provenance.py" "$JCONV_OUT/CONVERSION-PROVENANCE.json" "$root" "$JCONV_IN" "$JCONV_OUT"

if [ -e "$FINAL_OUT" ]; then mv "$FINAL_OUT" "$backup"; fi
if ! mv "$stage" "$FINAL_OUT"; then
  [ ! -d "$backup" ] || mv "$backup" "$FINAL_OUT"
  echo "jconv: publication failed; previous generation restored" >&2
  exit 1
fi
published=1
[ ! -d "$backup" ] || rm -rf "$backup"
echo "jconv: published complete generation at $FINAL_OUT"
