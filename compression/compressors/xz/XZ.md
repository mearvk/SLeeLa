# xz (LZMA2)

**Algorithm id:** `SLEELA_COMPRESSION_XZ` · **State:** Backend · **Extensions:** `.xz` · **MIME:** `application/x-xz`

xz: the six-byte magic 0xFD '7zXZ' 0x00. Compress/decompress require an approved xz (LZMA2) backend.

## Files

| File | Language | Role |
|------|----------|------|
| `xz.c` | C | Plugin vtable (`sleela_compressor_plugin_xz`), state + dispatch. |
| `xz_detail.cpp` | C++ | Content signature probe (`sleela_xz_probe_bytes`). |
| `xz_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
