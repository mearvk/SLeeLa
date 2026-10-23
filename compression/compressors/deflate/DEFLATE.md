# DEFLATE (RFC 1951)

**Algorithm id:** `SLEELA_COMPRESSION_DEFLATE` · **State:** Backend · **Extensions:** `.deflate` · **MIME:** `application/x-deflate`

Raw DEFLATE (RFC 1951) has no header, so there is no reliable signature; it is selected by id/extension, not positively probed. Compress/decompress require an approved DEFLATE (RFC 1951) backend.

## Files

| File | Language | Role |
|------|----------|------|
| `deflate.c` | C | Plugin vtable (`sleela_compressor_plugin_deflate`), state + dispatch. |
| `deflate_detail.cpp` | C++ | Content signature probe (`sleela_deflate_probe_bytes`). |
| `deflate_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Backend**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
