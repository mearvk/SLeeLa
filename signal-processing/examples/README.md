<!-- Author: Max Rupplin — MEARVK LLC — 2026 -->

# Examples

- c/basic.c — a plain C caller using smoothing, RMS, and the half-spectrum.
- cpp/basic.cpp — a C++17 caller using vector helpers and frequency analysis.

Build both from the subsystem root:

    make examples

or use the CMake targets sleela_sp_c_example and sleela_sp_cpp_example.

Examples should stay short and show correct error checking, buffer sizing,
and transform conventions. More involved demonstrations should use synthetic
signals with known expected behavior so users can reproduce the result.
