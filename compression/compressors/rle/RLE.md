# Run-Length Encoding

**Algorithm id:** `SLEELA_COMPRESSION_RLE` · **State:** Native · **Extensions:** `.rle` · **MIME:** `application/x-rle`

Lossless PackBits-style run-length encoding. A control byte introduces either a
literal run (`0x00..0x7F` → next *n+1* bytes verbatim) or a repeat run
(`0x80..0xFF` → one following byte repeated *257−n* times, i.e. 2..128 copies).
The scheme is self-describing, lossless for any input, and bounded.

## Files

| File | Language | Role |
|------|----------|------|
| `rle.c` | C | Plugin vtable (`sleela_compressor_plugin_rle`), buffer ownership. |
| `rle_detail.cpp` | C++ | Byte-level encode/decode (`sleela_rle_encode` / `sleela_rle_decode`). |
| `rle_detail.h` | — | C-linkage declaration. |

## Capabilities

- **compress / decompress** — fully in-package, lossless, two-phase sized
  (NULL output returns the required byte count), writes into a plugin-owned
  buffer. Decode rejects truncated streams with `SLEELA_COMPRESSION_ERR_FORMAT`.
- **probe** — `SLEELA_COMPRESSION_ERR_FORMAT`; RLE is a bare byte stream with no
  magic, selected by id/extension.

SLeeLa calls the Compression Manager, which loads this plugin by id; it never
calls the plugin directly.
