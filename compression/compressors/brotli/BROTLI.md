# Brotli

**Algorithm id:** `SLEELA_COMPRESSION_BROTLI` · **State:** Backend · **Extensions:** `.br` · **MIME:** `application/x-brotli`

Brotli has no fixed magic signature (the stream begins with a window-size descriptor), so it is selected by id/extension, not positively probed. Compress/decompress require an approved Brotli backend.

## Files

| File | Language | Role |
|------|----------|------|
| `brotli.c` | C | Plugin vtable (`sleela_compressor_plugin_brotli`), state + dispatch. |
| `brotli_detail.cpp` | C++ | Content signature probe (`sleela_brotli_probe_bytes`). |
| `brotli_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
