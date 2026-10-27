<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Build and testing notes

## Make

From signal-processing/:

    make
    make test

The default Make target builds the static archive and both example programs.
The test target builds and runs C and C++ regression programs. Build products
go under build/, which is intentionally ignored by Git.

## CMake

    cmake -S . -B build-cmake
    cmake --build build-cmake
    ctest --test-dir build-cmake --output-on-failure

To install headers and the static library into a chosen prefix:

    cmake --install build-cmake --prefix "$HOME/.local"

CMake uses C11 for the C implementation and C++17 for the wrapper. On Windows,
it avoids the Unix math-library flag; on Unix-like systems it links libm.
Cross-compilation should use the target toolchain file and its matching CMake
configuration rather than reusing native build artifacts.

## What to test when extending

Keep correctness tests separate from performance benchmarks. At minimum test:
- empty and one-element signals;
- invalid pointer/length/capacity combinations;
- an impulse, constant signal, and a short known sequence;
- FFT against DFT on small sizes and an FFT/inverse round trip;
- maximum/minimum finite values and NaN/infinity rejection;
- odd/even lengths and all window-boundary cases that apply;
- compile and run on Windows, macOS, and Linux.

The Signal Processing GitHub Actions workflow builds and tests on
ubuntu-latest, macos-latest, and windows-latest. A local pass is useful but
does not replace reviewing CI output for every supported platform.
