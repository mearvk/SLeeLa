#!/usr/bin/env bash
set -u
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
SUITE="$ROOT/test-suites"; BUILD="$SUITE/.build"; LOG="$SUITE/logs"
CC=cc; CXX=c++; PYTHON=python3; MODE=all
case "${1:-}" in
  --smoke) MODE=smoke;; --headers) MODE=headers;; --audit) MODE=audit;;
  --coverage) MODE=coverage;; --negative) MODE=negative;; --sanitizers) MODE=sanitizers;;
  --regression) MODE=regression;; --all|"") MODE=all;;
  --clean) rm -rf "$BUILD" "$LOG" "$SUITE/.sanitizers" "$SUITE/generated"; exit 0;;
  -h|--help) echo "Usage: $0 [--smoke|--headers|--audit|--coverage|--negative|--sanitizers|--regression|--clean|--all]"; exit 0;;
  *) echo "Unknown option: $1" >&2; exit 2;;
esac
mkdir -p "$BUILD" "$LOG"; FAIL=0; SKIP=0; PASS=0
pass(){ PASS=$((PASS+1)); echo "PASS: $*"; }; fail(){ FAIL=$((FAIL+1)); echo "FAIL: $*" >&2; }; skip(){ SKIP=$((SKIP+1)); echo "SKIP: $*"; }; have(){ command -v "$1" >/dev/null 2>&1; }
run_c(){ local src="$1" out="$2"; if ! have "$CC"; then skip "C compiler unavailable"; return; fi; if "$CC" -std=c11 -Wall -Wextra -I"$ROOT" "$src" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; printf '  diagnostics: '; sed -n '1,6p' "$LOG/$(basename "$out").compile.log" >&2; fi; }
run_c_with_cpp(){ local src="$1" impl="$2" out="$3"; if ! have "$CXX"; then skip "C++ compiler unavailable"; return; fi; if "$CXX" -std=c++17 -Wall -Wextra -I"$ROOT" "$src" "$impl" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; printf '  diagnostics: '; sed -n '1,8p' "$LOG/$(basename "$out").compile.log" >&2; fi; }
run_debugger(){ local src="$SUITE/cpp/test_debugger.cpp"; if "$CXX" -std=c++17 -Wall -Wextra -I"$ROOT" "$src" "$ROOT/debugger/debugger.cpp" -o "$BUILD/test_debugger" >"$LOG/test_debugger.compile.log" 2>&1 && "$BUILD/test_debugger" >"$LOG/test_debugger.run.log" 2>&1; then pass "$src"; else fail "$src"; fi; }
run_cpp(){ local src="$1" out="$2"; if ! have "$CXX"; then skip "C++ compiler unavailable"; return; fi; if "$CXX" -std=c++17 -Wall -Wextra -I"$ROOT" -I"$ROOT/api/include" -I"$ROOT/decompiler/include" -I"$ROOT/telephony-skya/drivers/include" -I"$ROOT/telephony-skya/drivers/src" -I"$ROOT/impl/core" -I"$ROOT/impl/frontend" -I"$ROOT/impl/subjects/native" -I"$ROOT/runtime" -I"$ROOT/terminal_pixel" -I"$ROOT/bash" -I"$ROOT/impl/subjects/chemistry" -I"$ROOT/impl/subjects/astrophysics" -I"$ROOT/impl/subjects/sociology" "$src" "$ROOT/impl/annotation/Annotation.cpp "$ROOT/impl/annotation/AnnotationForwarder.cpp" "$ROOT/impl/annotation/AnnotationInterpreter.cpp" "$ROOT/impl/annotation/ForwardingAnnotation.cpp" -o "$out" >"$LOG/$(basename "$out").compile.log" 2>&1 && "$out" >"$LOG/$(basename "$out").run.log" 2>&1; then pass "$src"; else fail "$src"; sed -n "1,8p" "$LOG/$(basename "$out").compile.log" >&2; fi; }
smoke(){ [ -f "$SUITE/c/test_http_bridge.c" ] && run_c_with_cpp "$SUITE/c/test_http_bridge.c" "$ROOT/http-servers/common/annotation_http_bridge.cpp" "$BUILD/test_http_bridge"; [ -f "$SUITE/c/test_c_api_headers.c" ] && run_c "$SUITE/c/test_c_api_headers.c" "$BUILD/test_c_api_headers"; [ -f "$SUITE/cpp/test_annotations.cpp" ] && run_cpp "$SUITE/cpp/test_annotations.cpp" "$BUILD/test_annotations"; [ -f "$SUITE/cpp/test_class_contracts.cpp" ] && run_cpp "$SUITE/cpp/test_class_contracts.cpp" "$BUILD/test_class_contracts"; [ -f "$SUITE/cpp/test_debugger.cpp" ] && run_debugger; }
audit(){ local f log std; while IFS= read -r f; do
  case "$f" in
    "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) continue;;
    "$ROOT/http-3.0/kernel/"*) skip "kernel TU $f (validated by Kbuild/kernel workflow)"; continue;;
  esac
  case "$f" in
    *_windows.cpp|*_windows.c|*/windows/*|*_windows.hpp|*_windows.h) [[ "$(uname -s)" != "MINGW"* && "$(uname -s)" != "MSYS"* && "$(uname -s)" != "CYGWIN"* ]] && { skip "Windows TU $f"; continue; };;
    *_macos.cpp|*_macos.c|*_macos.hpp|*_macos.h|*/macos/*|*/darwin/*) [[ "$(uname -s)" != "Darwin" ]] && { skip "macOS TU $f"; continue; };;
    *_linux.cpp|*_linux.c|*/linux/*) [[ "$(uname -s)" != "Linux" ]] && { skip "Linux TU $f"; continue; };;
    "$ROOT/java28/"*) if [[ "$f" == *sleela_java28_bridge.c ]]; then skip "Java/JNI-dependent TU $f"; continue; fi;;
    "$ROOT/sleela-terminal/sleelaterminal-gui.cpp") skip "GTK-dependent TU $f"; continue;;
    "$ROOT/http-servers/2/http2/http2_server.cpp") skip "nghttp2-dependent TU $f"; continue;;
  esac
  log="$LOG/audit-$(printf '%s' "$f" | sha256sum | cut -d' ' -f1).log"
  if [[ "$f" == *.c ]] && have "$CC"; then
    if "$CC" -std=c11 -D_POSIX_C_SOURCE=200809L -fsyntax-only -I"$ROOT" -I"$ROOT/api/include" -I"$ROOT/decompiler/include" -I"$ROOT/telephony-skya/drivers/include" -I"$ROOT/telephony-skya/drivers/src" -I"$ROOT/impl/core" -I"$ROOT/impl/frontend" -I"$ROOT/impl/subjects/native" -I"$ROOT/runtime" -I"$ROOT/terminal_pixel" -I"$ROOT/bash" -I"$ROOT/impl/subjects/chemistry" -I"$ROOT/impl/subjects/astrophysics" -I"$ROOT/impl/subjects/sociology" "$f" >"$log" 2>&1; then pass "TU $f"; else fail "TU $f"; sed -n '1,5p' "$log" >&2; fi
  elif [[ "$f" == *.cpp ]] && have "$CXX"; then
    std=c++17
    case "$f" in "$ROOT/decompiler/"*) std=c++20;; esac
    if "$CXX" -std="$std" -fsyntax-only -I"$ROOT" -I"$ROOT/api/include" -I"$ROOT/decompiler/include" -I"$ROOT/telephony-skya/drivers/include" -I"$ROOT/telephony-skya/drivers/src" -I"$ROOT/impl/core" -I"$ROOT/impl/frontend" -I"$ROOT/impl/subjects/native" -I"$ROOT/runtime" -I"$ROOT/terminal_pixel" -I"$ROOT/bash" "$f" >"$log" 2>&1; then pass "TU $f"; else fail "TU $f"; sed -n '1,8p' "$log" >&2; fi
  fi
 done < <(find "$ROOT" -type f \( -name '*.c' -o -name '*.cpp' \) -print); }
headers(){ local f std; while IFS= read -r f; do
  case "$f" in "$ROOT/test-suites/"*|"$ROOT/bash/"*|"$ROOT/.git/"*) continue;; esac
  case "$f" in
    *_windows.hpp|*_windows.h|*/windows/*) [[ "$(uname -s)" != "MINGW"* && "$(uname -s)" != "MSYS"* && "$(uname -s)" != "CYGWIN"* ]] && { skip "Windows header $f"; continue; };;
    *_macos.hpp|*_macos.h|*/macos/*|*/darwin/*) [[ "$(uname -s)" != "Darwin" ]] && { skip "macOS header $f"; continue; };;
    *_linux.hpp|*_linux.h|*/linux/*) [[ "$(uname -s)" != "Linux" ]] && { skip "Linux header $f"; continue; };;
  esac
  if [[ "$f" == *.h ]] && have "$CC"; then
    "$CC" -std=c11 -D_POSIX_C_SOURCE=200809L -fsyntax-only -I"$ROOT" -I"$ROOT/api/include" -I"$ROOT/decompiler/include" -I"$ROOT/telephony-skya/drivers/include" -I"$ROOT/telephony-skya/drivers/src" -I"$ROOT/impl/core" -I"$ROOT/impl/frontend" -I"$ROOT/impl/subjects/native" -I"$ROOT/runtime" -I"$ROOT/terminal_pixel" -I"$ROOT/bash" "$f" >/dev/null 2>&1 && pass "header $f" || { fail "header $f"; echo "  diagnostics:" >&2; "$CC" -std=c11 -D_POSIX_C_SOURCE=200809L -fsyntax-only -I"$ROOT"  "$f" 2>&1 | sed -n "1,5p" >&2; }
  elif [[ "$f" == *.hpp ]] && have "$CXX"; then
    std=c++17; case "$f" in "$ROOT/decompiler/"*) std=c++20;; esac
    "$CXX" -std="$std" -fsyntax-only -I"$ROOT" -I"$ROOT/api/include" -I"$ROOT/decompiler/include" -I"$ROOT/telephony-skya/drivers/include" -I"$ROOT/telephony-skya/drivers/src" -I"$ROOT/impl/core" -I"$ROOT/impl/frontend" -I"$ROOT/impl/subjects/native" -I"$ROOT/runtime" -I"$ROOT/terminal_pixel" -I"$ROOT/bash" "$f" >/dev/null 2>&1 && pass "header $f" || { fail "header $f"; echo "  diagnostics:" >&2; "$CXX" -std="$std" -fsyntax-only -I"$ROOT" -I"$ROOT/api/include" -I"$ROOT/decompiler/include" -I"$ROOT/telephony-skya/drivers/include" -I"$ROOT/telephony-skya/drivers/src" -I"$ROOT/impl/core" -I"$ROOT/impl/frontend" -I"$ROOT/impl/subjects/native" -I"$ROOT/runtime" -I"$ROOT/terminal_pixel" -I"$ROOT/bash" -I"$ROOT/impl/subjects/chemistry" -I"$ROOT/impl/subjects/astrophysics" -I"$ROOT/impl/subjects/sociology" "$f" 2>&1 | sed -n "1,5p" >&2; }
  fi
 done < <(find "$ROOT" -type f \( -name '*.h' -o -name '*.hpp' \) -print); }
coverage(){ if ! have "$PYTHON"; then skip "Python unavailable"; return; fi; "$SUITE/generate-function-coverage.sh" >"$LOG/function-coverage.log" 2>&1 && pass "function inventory" || fail "function inventory"; "$PYTHON" "$SUITE/generate-behavior-skeletons.py" >"$LOG/behavior-skeletons.log" 2>&1 && pass "behavior skeletons" || fail "behavior skeletons"; }
negative(){ [ -d "$SUITE/negative" ] && pass "negative corpus" || fail "negative corpus"; }
regression(){ [ -d "$SUITE/regression" ] && pass "regression corpus" || fail "regression corpus"; }
sanitizers(){ bash "$SUITE/sanitizers/run.sh" >"$LOG/sanitizers.log" 2>&1 && pass "sanitizers" || fail "sanitizers"; }
case "$MODE" in smoke) smoke;; headers) headers;; audit) audit;; coverage) coverage;; negative) negative;; regression) regression;; sanitizers) sanitizers;; all) smoke; headers; audit; coverage; negative; regression; sanitizers;; esac
echo "SLeeLa Testbed: PASS=$PASS FAIL=$FAIL SKIP=$SKIP"; [ "$FAIL" -eq 0 ]
