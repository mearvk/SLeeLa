# JSON document connector

Dependency-free structural tokenizer recording byte offsets, token type, and parent token. This is not a semantic JSON DOM and does not fully validate primitive syntax or decode string escapes; validate input with a standards-compliant parser before using it as trusted application data. Output capacity must cover all tokens; zero means malformed input or insufficient capacity. Maximum nesting depth is 256.
