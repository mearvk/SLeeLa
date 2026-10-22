# HTTP/1.0 security and conformance tests

Run from the repository root: make -C http-1.0 clean all; make -C http-1.0 syntax; make -C http-1.0 test.

The native self-test covers duplicate Content-Length, unsupported Transfer-Encoding, invalid decimal lengths, control bytes in header values, and resumable-download round trips. Duplicate Content-Length and all Transfer-Encoding are rejected because chunked decoding is not implemented.

Sanitizer test: make -C http-1.0 clean, then make -C http-1.0 test CFLAGS="-std=c11 -O1 -g -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fno-omit-frame-pointer".

The fuzz_download_metadata.c file exports a libFuzzer entry point. With Clang and the native HTTP dependencies available, compile with -fsanitize=fuzzer,address,undefined and feed valid and malformed SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE records.

Security boundary: native code owns wire-level validation; SLeeLa policy classes are declarative. The current server is single-connection-at-a-time, so read deadlines are still necessary to prevent slow-client denial of service. Plain HTTP has no confidentiality; use native TLS or a trusted TLS-terminating proxy. Do not trust forwarded identity headers from arbitrary peers. Resume state should bind to an authenticated principal and stable file identity, and completed artifacts should be digest-verified.
