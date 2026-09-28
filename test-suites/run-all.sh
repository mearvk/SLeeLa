#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
SUITE="$ROOT/test-suites"
BUILD="$SUITE/.build"
LOG="$SUITE/logs"
CC="${CC:-cc}"
CXX="${CXX:-c++}"
MODE="all"

case "${1:-}" in
  --smoke) MODE="smoke" ;;
  --headers) MODE="headers" ;;
  --audit) MODE="audit" ;;
  --clean) rm -rf "$BUILD" "$LOG"; exit 0 ;;
  --all|"") MODE="all" ;;
  -h|--help) echo "Usage: $0 [--smoke|--headers|--audit|--clean|--all]"; exit 0 ;;
  *) echo "Unknown option: $1" >&2; exit 2 ;;
esac

mkdir -p "$BUILD" "$LOG"
FAIL=0
SKIP=0
PASS=0
pass(){ PASS=$((PASS+1)); echo "PASS: $*"; }
fail(){ FAIL=$((FAIL+1)); echo "FAIL: $*" >&2; }
skip(){ SKIP=$((SKIP+1)); echo "SKIP: $*"; }
have(){ command -v "$1" >/dev/null 2>&1; }

run_c(){
  local src="$1" out="$2"
  if ! have "$CC"; then skip "C compiler $CC unavailable"; return; fi
  if "$CC" -std=c11 -Wall -Wextra -I"$ROOT" "$src" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1; then
    if "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src runtime"; fi
  else fail "$src compile"; fi
}

run_cpp(){
  local src="$1" out="$2"
  if ! have "$CXX"; then skip "C++ compiler $CXX unavailable"; return; fi
  if "$CXX" -std=c++17 -Wall -Wextra -I"$ROOT" "$src"       "$ROOT/impl/annotation/Annotation.cpp"       "$ROOT/impl/annotation/AnnotationForwarder.cpp"       "$ROOT/impl/annotation/AnnotationInterpreter.cpp"       "$ROOT/impl/annotation/ForwardingAnnotation.cpp"       -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1; then
    if "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src runtime"; fi
  else fail "$src compile"; fi
}

smoke(){
  if [ -f "$SUITE/c/test_http_bridge.c" ]; then
    if have "$CC"; then
      if "$CC" -std=c11 -Wall -Wextra -I"$ROOT" "$SUITE/c/test_http_bridge.c"           "$ROOT/http-servers/common/annotation_http_bridge.cpp" -lstdc++           -o "$BUILD/test_http_bridge" >"$LOG/test_http_bridge.compile.log" 2>&1; then
        if "$BUILD/test_http_bridge" >"$LOG/test_http_bridge.run.log" 2>&1; then pass "C HTTP bridge"; else fail "C HTTP bridge runtime"; fi
      else skip "C compiler unavailable"; fi
    else skip "C compiler unavailable"; fi
  fi
  [ -f "$SUITE/c/test_c_api_headers.c" ] && run_c "$SUITE/c/test_c_api_headers.c" "$BUILD/test_c_api_headers"
  [ -f "$SUITE/cpp/test_annotations.cpp" ] && run_cpp "$SUITE/cpp/test_annotations.cpp" "$BUILD/test_annotations"
  [ -f "$SUITE/cpp/test_class_contracts.cpp" ] && run_cpp "$SUITE/cpp/test_class_contracts.cpp" "$BUILD/test_class_contracts"
}

header_audit(){
  if ! have "$CC" && ! have "$CXX"; then skip "no C/C++ compiler available"; return; fi
  local f n
  while IFS= read -r f; do
    case "$f" in "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) continue ;; esac
    n="$(printf '%s' "$f" | sed 's#[^A-Za-z0-9_]#_#g')"
    if [[ "$f" == *.h ]] && have "$CC"; then
      if "$CC" -std=c11 -fsyntax-only -I"$ROOT" "$f" >"$LOG/header_$n.log" 2>&1; then pass "header $f"; else fail "header $f"; fi
    elif [[ "$f" == *.hpp ]] && have "$CXX"; then
      if "$CXX" -std=c++17 -fsyntax-only -I"$ROOT" "$f" >"$LOG/header_$n.log" 2>&1; then pass "header $f"; else fail "header $f"; fi
    fi
  done < <(find "$ROOT" -type f ( -name '*.h' -o -name '*.hpp' ) -print)
}

source_audit(){
  if ! have "$CC" && ! have "$CXX"; then skip "no C/C++ compiler available"; return; fi
  local f n
  while IFS= read -r f; do
    case "$f" in "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) continue ;; esac
    n="$(printf '%s' "$f" | sed 's#[^A-Za-z0-9_]#_#g')"
    if [[ "$f" == *.c ]] && have "$CC"; then
      if "$CC" -std=c11 -fsyntax-only -I"$ROOT" "$f" >"$LOG/tu_$n.log" 2>&1; then pass "TU $f"; else fail "TU $f"; fi
    elif [[ "$f" == *.cpp ]] && have "$CXX"; then
      if "$CXX" -std=c++17 -fsyntax-only -I"$ROOT" "$f" >"$LOG/tu_$n.log" 2>&1; then pass "TU $f"; else fail "TU $f"; fi
    fi
  done < <(find "$ROOT" -type f ( -name '*.c' -o -name '*.cpp' ) -print)
}

case "$MODE" in
  smoke) smoke ;;
  headers) header_audit ;;
  audit) source_audit ;;
  all) smoke; header_audit; source_audit ;;
esac

echo
echo "SLeeLa Testbed: PASS=$PASS FAIL=$FAIL SKIP=$SKIP"
[ "$FAIL" -eq 0 ]
