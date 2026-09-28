#!/bin/sh
set -eu

CC=${CC:-cc}
CXX=${CXX:-c++}
CFLAGS=${CFLAGS:--std=c11 -Wall -Wextra -Wpedantic}
CXXFLAGS=${CXXFLAGS:--std=c++17 -Wall -Wextra -Wpedantic}

cleanup() {
    rm -f debugger-core-test debugger-line-test debugger-backend-test debugger-actions-test debugger-c-test debugger-line-c-test debug-engine-c-test debug-diagnostics-test
}
trap cleanup EXIT INT TERM

"$CXX" $CXXFLAGS -I.. debugger_core_test.cpp ../debugger.cpp -o debugger-core-test
./debugger-core-test

"$CXX" $CXXFLAGS -I.. debugger_line_test.cpp ../debugger.cpp ../debugger_line.cpp -o debugger-line-test
./debugger-line-test

"$CXX" $CXXFLAGS -I.. debugger_backend_test.cpp ../debugger_backend.cpp -o debugger-backend-test
./debugger-backend-test

"$CXX" $CXXFLAGS -I.. debugger_actions_test.cpp ../debugger.cpp ../debugger_actions.cpp -o debugger-actions-test
./debugger-actions-test

"$CC" $CFLAGS -I.. debugger_c_test.c ../debugger_c.c -o debugger-c-test
./debugger-c-test

"$CC" $CFLAGS -I.. debugger_line_c_test.c ../debugger_c.c ../debugger_line.c -o debugger-line-c-test
./debugger-line-c-test

c++ $CXXFLAGS -I.. ../debug_engine.cpp ../debug_engine_c.cpp debug_engine_c_test.c ../debugger.cpp -o debug-engine-c-test
./debug-engine-c-test
c++ $CXXFLAGS -I.. ../debug_diagnostics.cpp debug_diagnostics_test.cpp -o debug-diagnostics-test
./debug-diagnostics-test

echo "SLeeLa debugger test suite: PASS"
