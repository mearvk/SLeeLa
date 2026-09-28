# Regex Natural Form Test Suite

Version: 1.1.0-dev

The Natural Form test suite validates the same finite language contract across C, C++, Java, and SLeeLa.

Coverage includes core words and symbolic aliases, canonical symbol mapping, valid patterns, unknown-word rejection, delimiter and quote diagnostics, cross-language smoke tests, and source inventory checks.

Run:

    make -C regex test
    make -C regex/test-suites test
