# bzip2

**Algorithm id:** `SLEELA_COMPRESSION_BZIP2` · **State:** Backend · **Extensions:** `.bz2` · **MIME:** `application/x-bzip2`

bzip2: ASCII magic 'BZh' (0x42 0x5A 0x68). Compress/decompress require an approved bzip2 backend.

## Files

| File | Language | Role |
|------|----------|------|
| `bzip2.c` | C | Plugin vtable (`sleela_compressor_plugin_bzip2`), state + dispatch. |
| `bzip2_detail.cpp` | C++ | Content signature probe (`sleela_bzip2_probe_bytes`). |
| `bzip2_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
