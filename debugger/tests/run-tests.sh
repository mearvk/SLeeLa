#!/bin/sh
set -eu

CXX=${CXX:-c++}
CXXFLAGS=${CXXFLAGS:--std=c++17 -Wall -Wextra -Wpedantic}

"$CXX" $CXXFLAGS -I.. debugger_core_test.cpp ../debugger.cpp -o debugger-core-test
./debugger-core-test
rm -f debugger-core-test
