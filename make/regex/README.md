# SLeeLa Regex

Version: 1.0.0-dev

Cross-platform regular-expression subsystem for SLeeLa with a stable C ABI, C++ typed API, H/HPP public interfaces, tests, and SLeeLa library objects.

Operations: compile/validate, full match, search, captures, replacement, split, literal escaping, flags, offsets, diagnostics.

The C implementation uses POSIX ERE. The C++ implementation uses std::regex and exposes ECMAScript/extended grammar selection. The portable SLeeLa contract is explicit about dialect differences rather than silently claiming identical support.

Build C: cc -std=c11 -Wall -Wextra -pedantic -Iinclude src/sleela_regex.c tests/test_regex.c -o regex-c && ./regex-c

Build C++: c++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/sleela_regex.cpp tests/test_regex.cpp -o regex-cpp && ./regex-cpp

SLeeLa — MEARVK LLC — 2026
