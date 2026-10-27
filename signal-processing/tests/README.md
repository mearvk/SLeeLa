<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Regression tests

test_signal_processing.c exercises the C API with known vectors, expected
lengths, capacities, numeric failures and transform comparisons.
test_signal_processing.cpp checks common vector-wrapper use and exception
behavior.

Run the suite from the subsystem root with make test, or build with CMake and
run ctest --test-dir build-cmake --output-on-failure.

Add tests before or alongside new algorithms. Prefer hand-computable test
vectors and reference comparisons; avoid tests that merely repeat the
implementation formula without checking the expected result independently.
