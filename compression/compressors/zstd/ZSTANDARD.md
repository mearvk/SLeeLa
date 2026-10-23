# Zstandard

**Algorithm id:** `SLEELA_COMPRESSION_ZSTD` · **State:** Backend · **Extensions:** `.zst` · **MIME:** `application/zstd`

Zstandard: the frame magic 0x28 0xB5 0x2F 0xFD (little-endian 0xFD2FB528). Compress/decompress require an approved Zstandard backend.

## Files

| File | Language | Role |
|------|----------|------|
| `zstd.c` | C | Plugin vtable (`sleela_compressor_plugin_zstd`), state + dispatch. |
| `zstd_detail.cpp` | C++ | Content signature probe (`sleela_zstd_probe_bytes`). |
| `zstd_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
