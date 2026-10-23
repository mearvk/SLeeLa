# ZIP archive

**Algorithm id:** `SLEELA_COMPRESSION_ZIP` · **State:** Container · **Extensions:** `.zip` · **MIME:** `application/zip`

ZIP container: the 'PK' signature 0x50 0x4B followed by a record marker (0x03 0x04 local file, 0x05 0x06 end-of-central-directory, or 0x07 0x08 spanned). Compress/decompress require an approved ZIP archive backend.

## Files

| File | Language | Role |
|------|----------|------|
| `zip.c` | C | Plugin vtable (`sleela_compressor_plugin_zip`), state + dispatch. |
| `zip_detail.cpp` | C++ | Content signature probe (`sleela_zip_probe_bytes`). |
| `zip_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **compress / decompress** — this handler is **Container**; it returns
  `SLEELA_COMPRESSION_ERR_UNSUPPORTED` from compress/decompress until an approved
  backend is wired, rather than faking success. The manager reports the state so
  callers fail cleanly.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
