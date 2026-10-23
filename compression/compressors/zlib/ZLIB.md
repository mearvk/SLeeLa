# zlib (RFC 1950)

**Algorithm id:** `SLEELA_COMPRESSION_ZLIB` · **State:** Backend · **Extensions:** `.zz` · **MIME:** `application/zlib`

zlib (RFC 1950): the two-byte header has CM=8 (deflate) in the low nibble of byte 0 and (CMF*256 + FLG) is a multiple of 31. Compress/decompress require an approved zlib (RFC 1950) backend.

## Files

| File | Language | Role |
|------|----------|------|
| `zlib.c` | C | Plugin vtable (`sleela_compressor_plugin_zlib`), state + dispatch. |
| `zlib_detail.cpp` | C++ | Content signature probe (`sleela_zlib_probe_bytes`). |
| `zlib_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
