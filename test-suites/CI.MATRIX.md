# CI Verification Matrix

Order:
1. smoke tests;
2. header audit;
3. translation-unit audit;
4. explicit function map;
5. negative/security tests;
6. sanitizer suite where supported;
7. regression corpus;
8. optional runtime coverage instrumentation.

Platforms: Linux GCC/Clang, macOS Apple Clang, Windows 10+ MSVC and/or clang-cl.
Unsupported sanitizer/toolchain combinations are recorded as skipped.
CI preserves logs and coverage artifacts for failed runs.
