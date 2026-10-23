# Lempel-Ziv-Welch

**Algorithm id:** `SLEELA_COMPRESSION_LZW` · **State:** Native · **Extensions:** `.lzw`, `.Z` · **MIME:** `application/x-lzw`

Variable-width LZW over a byte alphabet. Codes start at 9 bits and grow up to 12
bits (dictionary cap 4096); an explicit CLEAR code (256) resets the dictionary
when it fills, keeping code width and memory bounded. Self-describing header:

```
magic    "SLZW"        (4 bytes)
orig_len u64 LE        (8 bytes)
payload  packed MSB-first variable-width code stream
```

Codes 0..255 are the literal bytes and code 256 is CLEAR; dictionary entries
begin at 257.

## Files

| File | Language | Role |
|------|----------|------|
| `lzw.c` | C | Plugin vtable (`sleela_compressor_plugin_lzw`), buffer ownership. |
| `lzw_detail.cpp` | C++ | Dictionary coder + bit I/O (`sleela_lzw_encode` / `sleela_lzw_decode`). |
| `lzw_detail.h` | — | C-linkage declaration. |

## Capabilities

- **compress / decompress** — fully in-package, lossless, two-phase sized.
  Decode validates the magic, handles the KwKwK special case, and rejects
  invalid codes / truncated streams with `SLEELA_COMPRESSION_ERR_FORMAT`.
- **probe** — recognizes the `"SLZW"` magic.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
