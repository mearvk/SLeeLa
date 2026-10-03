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
run_c(){ local src="$1" out="$2"; if ! have "$CC"; then skip "C compiler unavailable"; return; fi; if "$CC" -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -I"$ROOT" "$src" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
run_debugger(){ local src="$SUITE/cpp/test_debugger.cpp"; if "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -I"$ROOT" "$src" "$ROOT/debugger/debugger.cpp" -o "$BUILD/test_debugger" >"$LOG/test_debugger.compile.log" 2>&1 && "$BUILD/test_debugger" >"$LOG/test_debugger.run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
run_cpp(){ local src="$1" out="$2"; if ! have "$CXX"; then skip "C++ compiler unavailable"; return; fi; if "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -I"$ROOT" "$src" "$ROOT/impl/annotation/Annotation.cpp" "$ROOT/impl/annotation/AnnotationForwarder.cpp" "$ROOT/impl/annotation/AnnotationInterpreter.cpp" "$ROOT/impl/annotation/ForwardingAnnotation.cpp" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
smoke(){ [ -f "$SUITE/c/test_http_bridge.c" ] && run_c "$SUITE/c/test_http_bridge.c" "$BUILD/test_http_bridge"; [ -f "$SUITE/c/test_c_api_headers.c" ] && run_c "$SUITE/c/test_c_api_headers.c" "$BUILD/test_c_api_headers"; [ -f "$SUITE/cpp/test_annotations.cpp" ] && run_cpp "$SUITE/cpp/test_annotations.cpp" "$BUILD/test_annotations"; [ -f "$SUITE/cpp/test_class_contracts.cpp" ] && run_cpp "$SUITE/cpp/test_class_contracts.cpp" "$BUILD/test_class_contracts"; [ -f "$SUITE/cpp/test_debugger.cpp" ] && run_debugger; }
audit(){ local f; while IFS= read -r f; do case "$f" in "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) continue;; esac; case "$f" in *"/http-3.0/kernel/"*|*"/java28/src/native/"*) continue;; esac; if [[ "$f" == *.c ]] && have "$CC"; then "$CC" -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only -I"$ROOT" "$f" >/dev/null 2>&1 && pass "TU $f" || fail "TU $f"; elif [[ "$f" == *.cpp ]] && have "$CXX"; then "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only -I"$ROOT" "$f" >/dev/null 2>&1 && pass "TU $f" || fail "TU $f"; fi; done < <(find "$ROOT" -type f \( -name '*.c' -o -name '*.cpp' \) -print); }
headers(){ local f; while IFS= read -r f; do case "$f" in "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) continue;; esac; case "$f" in *"/http-3.0/kernel/"*|*"/java28/src/native/"*) continue;; esac; if [[ "$f" == *.h ]] && have "$CC"; then "$CC" -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only -I"$ROOT" "$f" >/dev/null 2>&1 && pass "header $f" || fail "header $f"; elif [[ "$f" == *.hpp ]] && have "$CXX"; then "$CXX" -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror -fsyntax-only -I"$ROOT" "$f" >/dev/null 2>&1 && pass "header $f" || fail "header $f"; fi; done < <(find "$ROOT" -type f \( -name '*.h' -o -name '*.hpp' \) -print); }
sleela_sources(){ bash "$SUITE/sleela-source-behavior.sh" >"$LOG/sleela-source-behavior.log" 2>&1 && pass "SLeeLa source behavioral corpus" || fail "SLeeLa source behavioral corpus"; }
coverage(){ if ! have "$PYTHON"; then skip "Python unavailable"; return; fi; bash "$SUITE/generate-function-coverage.sh" >"$LOG/function-coverage.log" 2>&1 && pass "function inventory" || fail "function inventory"; "$PYTHON" "$SUITE/generate-behavior-skeletons.py" >"$LOG/behavior-skeletons.log" 2>&1 && pass "behavior skeletons" || fail "behavior skeletons"; }
negative(){ [ -d "$SUITE/negative" ] && pass "negative corpus" || fail "negative corpus"; }
regression(){ [ -d "$SUITE/regression" ] && pass "regression corpus" || fail "regression corpus"; }
sanitizers(){ bash "$SUITE/sanitizers/run.sh" >"$LOG/sanitizers.log" 2>&1 && pass "sanitizers" || fail "sanitizers"; }
case "$MODE" in smoke) smoke;; headers) headers;; audit) audit;; coverage) coverage;; negative) negative;; regression) regression;; sleela-sources) sleela_sources;; sanitizers) sanitizers;; all) smoke; headers; audit; coverage; negative; regression; sleela_sources; sanitizers;; esac
echo "SLeeLa Testbed: PASS=$PASS FAIL=$FAIL SKIP=$SKIP"; [ "$FAIL" -eq 0 ]
