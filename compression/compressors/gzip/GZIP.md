# gzip (RFC 1952)

**Algorithm id:** `SLEELA_COMPRESSION_GZIP` · **State:** Backend · **Extensions:** `.gz` · **MIME:** `application/gzip`

gzip (RFC 1952): magic 0x1F 0x8B followed by compression method 0x08 (deflate). Compress/decompress require an approved gzip (RFC 1952) backend.

## Files

| File | Language | Role |
|------|----------|------|
| `gzip.c` | C | Plugin vtable (`sleela_compressor_plugin_gzip`), state + dispatch. |
| `gzip_detail.cpp` | C++ | Content signature probe (`sleela_gzip_probe_bytes`). |
| `gzip_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
