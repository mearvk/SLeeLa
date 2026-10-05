#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
SUITE="$ROOT/test-suites"; BUILD="$SUITE/.build"; LOG="$SUITE/logs"
CC="${CC:-cc}"; CXX="${CXX:-c++}"; PYTHON="${PYTHON:-python3}"; MODE=all
case "${1:-}" in
  --smoke) MODE=smoke;; --headers) MODE=headers;; --audit) MODE=audit;;
  --coverage) MODE=coverage;; --negative) MODE=negative;; --sanitizers) MODE=sanitizers;;
  --regression) MODE=regression;; --sleela-sources) MODE=sleela-sources;; --all|"") MODE=all;;
  --clean) rm -rf "$BUILD" "$LOG" "$SUITE/.sanitizers" "$SUITE/generated"; exit 0;;
  -h|--help) echo "Usage: $0 [--smoke|--headers|--audit|--coverage|--negative|--sanitizers|--regression|--sleela-sources|--clean|--all]"; exit 0;;
  *) echo "Unknown option: $1" >&2; exit 2;;
esac
mkdir -p "$BUILD" "$LOG"; FAIL=0; SKIP=0; PASS=0
pass(){ PASS=$((PASS+1)); echo "PASS: $*"; }; fail(){ FAIL=$((FAIL+1)); echo "FAIL: $*" >&2; }; skip(){ SKIP=$((SKIP+1)); echo "SKIP: $*"; }; have(){ command -v "$1" >/dev/null 2>&1; }

# Standard-conformance language levels. The compiler/decompiler and some subject
# libraries use C++20 (std::span, concepts), so the standalone audit must match
# the standard the real build uses rather than failing modern-but-correct code.
C_STD="${C_STD:-c11}"; CXX_STD="${CXX_STD:-c++20}"

# Set of git-tracked paths, each wrapped as <path> for substring lookup. Used to
# keep untracked build artifacts out of the audit. Empty (disabled) if git is
# unavailable or this is not a work tree, in which case all source is audited.
AUDIT_TRACKED=""
if command -v git >/dev/null 2>&1 && git -C "$ROOT" rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  while IFS= read -r _p; do AUDIT_TRACKED="$AUDIT_TRACKED<$_p>"; done < <(git -C "$ROOT" ls-files)
fi

# audit_excluded PATH -> true for trees that are not SLeeLa-authored source and
# are never part of the audit tally (so the PASS/FAIL/SKIP numbers stay about
# the project's own code): the harness itself, the vendored GNU Bash tree, the
# git metadata, and transient build output (build/ / .build/, which also holds
# the unpacked vendored nghttp2 library tree). These are dropped silently.
audit_excluded(){
  case "$1" in
    "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) return 0;;
    *"/build/"*|*"/.build/"*) return 0;;
    *"/http-3.0/kernel/"*|*"/java28/src/native/"*) return 0;;
  esac
  # Only audit committed source. Transient build output (e.g. the compiled
  # smoke-test binaries the tests/ Makefile drops into tests/) is untracked and
  # must never be fed to the compiler as if it were a translation unit.
  if [ -n "${AUDIT_TRACKED:-}" ]; then
    case "$AUDIT_TRACKED" in *"<${1#$ROOT/}>"*) : ;; *) return 0;; esac
  fi
  return 1
}

# audit_skip PATH -> true for SLeeLa-authored translation units that genuinely
# cannot be compiled standalone on this host because they target a foreign OS
# SDK that is absent here: the macOS CoreAudio backend and the Windows SDK /
# WASAPI / WRL backend. These only build on their own OS and are exercised by
# the per-OS build workflows; the audit reports them as SKIP (not FAIL).
audit_skip(){
  case "$1" in
    *"/platform/macos/"*|*"/platform/windows/"*|*_macos_*|*_windows_*|*_win32_*|*_darwin_*) return 0;;
  esac
  # GUI translation units that need a desktop toolkit SDK not installed on the
  # headless audit host (GTK). Detected by a direct toolkit include.
  if [ "${1##*.}" = cpp ] || [ "${1##*.}" = c ]; then
    if grep -qE '#include[[:space:]]*[<"]gtk/|#include[[:space:]]*[<"]gtkmm' "$1" 2>/dev/null; then return 0; fi
  fi
  return 1
}

# Include roots that the per-module Makefiles put on every compile line and that
# cross-module translation units rely on: the repository root, the repo-level
# include/ directory, and the compiler front end + subject-library directories
# that impl/Makefile adds via -Ifrontend / the SUBJECT_INCLUDES set. Headers in
# these are referenced by bare name from many modules.
AUDIT_COMMON_INC="-I$ROOT"
for _d in include impl/core impl/frontend impl/catalog impl/xclass \
          impl/subjects/native impl/subjects/math \
          impl/subjects/physics impl/subjects/economics impl/subjects/inference \
          impl/subjects/astrophysics impl/subjects/sociology impl/subjects/chemistry \
          impl/subjects/finance runtime bash; do
  [ -d "$ROOT/$_d" ] && AUDIT_COMMON_INC="$AUDIT_COMMON_INC -I$ROOT/$_d"
done

# audit_incflags PATH -> echo the include flags a translation unit needs to find
# its project headers when compiled standalone: the common roots above, the
# file's own directory, and every include/ and src/ directory inside the file's
# top-level module (which is how the per-module Makefiles resolve "sibling" and
# cross-component headers). Results are memoized per module to keep the sweep fast.
_MODINC_CACHE_KEYS=""; 
audit_incflags(){
  local f="$1" d rel top key inc id
  d="$(dirname "$f")"; rel="${f#$ROOT/}"; top="${rel%%/*}"
  key="MODINC_$(printf '%s' "$top" | tr -c 'A-Za-z0-9' '_')"
  if ! printf '%s' "$_MODINC_CACHE_KEYS" | grep -q "<$key>"; then
    inc=""
    for id in $(find "$ROOT/$top" -type d \( -name include -o -name src \) 2>/dev/null | grep -v '/build/\|/\.build/'); do
      inc="$inc -I$id"
    done
    eval "$key=\$inc"; _MODINC_CACHE_KEYS="$_MODINC_CACHE_KEYS<$key>"
  fi
  # The file's own directory and its module's include/src dirs come FIRST so a
  # module's own headers win over same-named headers elsewhere in the repo
  # (several subsystems, e.g. the root include/ tree vs sleela-virtual-machine/1/
  # include/, define different structs under the same header name). The common
  # repo-wide roots come last as a fallback.
  eval "printf '%s' \"-I\$d \$$key \$AUDIT_COMMON_INC\""
}
run_c(){ local src="$1" out="$2"; if ! have "$CC"; then skip "C compiler unavailable"; return; fi; if "$CC" -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -I"$ROOT" "$src" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
run_debugger(){ local src="$SUITE/cpp/test_debugger.cpp"; if "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -I"$ROOT" "$src" "$ROOT/debugger/debugger.cpp" -o "$BUILD/test_debugger" >"$LOG/test_debugger.compile.log" 2>&1 && "$BUILD/test_debugger" >"$LOG/test_debugger.run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
run_cpp(){ local src="$1" out="$2"; if ! have "$CXX"; then skip "C++ compiler unavailable"; return; fi; if "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -I"$ROOT" "$src" "$ROOT/impl/annotation/Annotation.cpp" "$ROOT/impl/annotation/AnnotationForwarder.cpp" "$ROOT/impl/annotation/AnnotationInterpreter.cpp" "$ROOT/impl/annotation/ForwardingAnnotation.cpp" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
smoke(){ [ -f "$SUITE/c/test_http_bridge.c" ] && run_c "$SUITE/c/test_http_bridge.c" "$BUILD/test_http_bridge"; [ -f "$SUITE/c/test_c_api_headers.c" ] && run_c "$SUITE/c/test_c_api_headers.c" "$BUILD/test_c_api_headers"; [ -f "$SUITE/cpp/test_annotations.cpp" ] && run_cpp "$SUITE/cpp/test_annotations.cpp" "$BUILD/test_annotations"; [ -f "$SUITE/cpp/test_class_contracts.cpp" ] && run_cpp "$SUITE/cpp/test_class_contracts.cpp" "$BUILD/test_class_contracts"; [ -f "$SUITE/cpp/test_debugger.cpp" ] && run_debugger; }
audit(){ local f inc; while IFS= read -r f; do if audit_excluded "$f"; then continue; fi; if audit_skip "$f"; then skip "TU $f (foreign OS SDK)"; continue; fi; inc="$(audit_incflags "$f")"; if [[ "$f" == *.c ]] && have "$CC"; then "$CC" -std=$C_STD -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only $inc "$f" >/dev/null 2>&1 && pass "TU $f" || fail "TU $f"; elif [[ "$f" == *.cpp ]] && have "$CXX"; then "$CXX" -std=$CXX_STD -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only $inc "$f" >/dev/null 2>&1 && pass "TU $f" || fail "TU $f"; fi; done < <(find "$ROOT" -type f \( -name '*.c' -o -name '*.cpp' \) -print); }
headers(){ local f inc; while IFS= read -r f; do if audit_excluded "$f"; then continue; fi; if audit_skip "$f"; then skip "header $f (foreign OS SDK)"; continue; fi; inc="$(audit_incflags "$f")"; if [[ "$f" == *.h ]] && have "$CC"; then "$CC" -std=$C_STD -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only $inc "$f" >/dev/null 2>&1 && pass "header $f" || fail "header $f"; elif [[ "$f" == *.hpp ]] && have "$CXX"; then "$CXX" -std=$CXX_STD -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only $inc "$f" >/dev/null 2>&1 && pass "header $f" || fail "header $f"; fi; done < <(find "$ROOT" -type f \( -name '*.h' -o -name '*.hpp' \) -print); }
sleela_sources(){ bash "$SUITE/sleela-source-behavior.sh" >"$LOG/sleela-source-behavior.log" 2>&1 && pass "SLeeLa source behavioral corpus" || fail "SLeeLa source behavioral corpus"; }
coverage(){ if ! have "$PYTHON"; then skip "Python unavailable"; return; fi; bash "$SUITE/generate-function-coverage.sh" >"$LOG/function-coverage.log" 2>&1 && pass "function inventory" || fail "function inventory"; "$PYTHON" "$SUITE/generate-behavior-skeletons.py" >"$LOG/behavior-skeletons.log" 2>&1 && pass "behavior skeletons" || fail "behavior skeletons"; }
negative(){ [ -d "$SUITE/negative" ] && pass "negative corpus" || fail "negative corpus"; }
regression(){ [ -d "$SUITE/regression" ] && pass "regression corpus" || fail "regression corpus"; }
sanitizers(){ bash "$SUITE/sanitizers/run.sh" >"$LOG/sanitizers.log" 2>&1 && pass "sanitizers" || fail "sanitizers"; }
case "$MODE" in smoke) smoke;; headers) headers;; audit) audit;; coverage) coverage;; negative) negative;; regression) regression;; sleela-sources) sleela_sources;; sanitizers) sanitizers;; all) smoke; headers; audit; coverage; negative; regression; sleela_sources; sanitizers;; esac
echo "SLeeLa Testbed: PASS=$PASS FAIL=$FAIL SKIP=$SKIP"; [ "$FAIL" -eq 0 ]
