# Snappy

**Algorithm id:** `SLEELA_COMPRESSION_SNAPPY` · **State:** Backend · **Extensions:** `.sz` · **MIME:** `application/x-snappy-framed`

Snappy framing format: a stream-identifier chunk 0xFF 0x06 0x00 0x00 followed by 'sN' ('sNaPpY'). Compress/decompress require an approved Snappy backend.

## Files

| File | Language | Role |
|------|----------|------|
| `snappy.c` | C | Plugin vtable (`sleela_compressor_plugin_snappy`), state + dispatch. |
| `snappy_detail.cpp` | C++ | Content signature probe (`sleela_snappy_probe_bytes`). |
| `snappy_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
