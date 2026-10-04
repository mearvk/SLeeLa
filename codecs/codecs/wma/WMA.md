# Windows Media Audio Codec

**Codec id:** `SLEELA_CODEC_WMA` · **State:** Backend · **Extensions:** `.wma` · **MIME:** `audio/x-ms-wma`

Windows Media Audio family (ASF container). Requires an approved WMA backend for encode/decode.

## Files

| File | Language | Role |
|------|----------|------|
| `wma.c` | C | Plugin vtable (`sleela_codec_plugin_wma`), state + dispatch. |
| `wma_detail.cpp` | C++ | Content signature probe (`sleela_wma_probe_bytes`). |
| `wma_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — no reliable byte signature; resolve by extension or container.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
