# Regex Natural Form Build

Version: 1.1.0-dev

Supported implementations: C11, C++17, Java, and SLeeLa library objects.

Build from repository root:

    make -C regex all

Natural Form only:

    make -C regex natural
    make -C regex java

Tests:

    make -C regex test
    make -C regex/test-suites test

Strict warnings are enabled and warnings are errors by default. Generated binaries belong only in disposable build output.
