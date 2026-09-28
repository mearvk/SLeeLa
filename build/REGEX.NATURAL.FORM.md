# Regex Natural Form Build and Test Contract

Version: 1.1.0-dev

Canonical test entry point:

    make -C regex test

Dedicated suite:

    make -C regex/test-suites test

Strict native compilation uses C11 and C++17 with warnings treated as errors. Java is compiled and executed as a conformance smoke test. SLeeLa Natural Form library sources are checked for presence and non-empty source content.

Generated artifacts remain under regex/build.
