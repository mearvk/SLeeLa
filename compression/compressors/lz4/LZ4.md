# LZ4

**Algorithm id:** `SLEELA_COMPRESSION_LZ4` · **State:** Backend · **Extensions:** `.lz4` · **MIME:** `application/x-lz4`

LZ4 frame format: the magic 0x04 0x22 0x4D 0x18 (little-endian 0x184D2204). Compress/decompress require an approved LZ4 backend.

## Files

| File | Language | Role |
|------|----------|------|
| `lz4.c` | C | Plugin vtable (`sleela_compressor_plugin_lz4`), state + dispatch. |
| `lz4_detail.cpp` | C++ | Content signature probe (`sleela_lz4_probe_bytes`). |
| `lz4_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
