#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
BUILD="$ROOT/build/natural-tests"
CC_BIN=${CC:-cc}
CXX_BIN=${CXX:-c++}
JAVA_BIN=${JAVA:-java}
JAVAC_BIN=${JAVAC:-javac}
CFLAGS=${CFLAGS:-"-std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror"}
CXXFLAGS=${CXXFLAGS:-"-std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror"}

mkdir -p "$BUILD/java"

$CC_BIN $CFLAGS -I"$ROOT/include" "$ROOT/src/sleela_regex_natural.c" "$ROOT/tests/test_regex_natural.c" -o "$BUILD/natural-c"
"$BUILD/natural-c"

$CXX_BIN $CXXFLAGS -I"$ROOT/include" "$ROOT/src/sleela_regex_natural.cpp" "$ROOT/tests/test_regex_natural.cpp" -o "$BUILD/natural-cpp"
"$BUILD/natural-cpp"

$JAVAC_BIN -d "$BUILD/java" "$ROOT/java/SleelaRegexNatural.java" "$ROOT/java/SleelaRegexNaturalParser.java" "$ROOT/java/SleelaRegexNaturalTest.java"
$JAVA_BIN -cp "$BUILD/java" sleela.regex.SleelaRegexNaturalTest

"$ROOT/test-suites/test_sleela_sources.sh"

printf '%s\n' 'Natural Form C/C++/Java tests and SLeeLa source inventory: PASS'
