<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# JSON document connector

Dependency-free structural tokenizer recording byte offsets, token types, and parent-token indices. It checks document structure and string escape shape, but it is not a semantic JSON DOM, does not decode string values, and does not fully validate primitive syntax. Validate untrusted input with a standards-compliant JSON parser before treating it as valid application data. Output capacity must cover all tokens; a zero result indicates malformed input or insufficient capacity. Maximum nesting depth is 256.

Build from the repository root with `make -C lib/connector/json build`, or from this directory with `make build`. The group smoke test is `make -C lib/connector test`.