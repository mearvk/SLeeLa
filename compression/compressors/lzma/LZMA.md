# LZMA

**Algorithm id:** `SLEELA_COMPRESSION_LZMA` · **State:** Backend · **Extensions:** `.lzma` · **MIME:** `application/x-lzma`

LZMA (.lzma / alone format): a properties byte of 0x5D (lc=3,lp=0,pb=2) followed by a little-endian dictionary size. Compress/decompress require an approved LZMA backend.

## Files

| File | Language | Role |
|------|----------|------|
| `lzma.c` | C | Plugin vtable (`sleela_compressor_plugin_lzma`), state + dispatch. |
| `lzma_detail.cpp` | C++ | Content signature probe (`sleela_lzma_probe_bytes`). |
| `lzma_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
