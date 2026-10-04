# Speex Codec

**Codec id:** `SLEELA_CODEC_SPEEX` · **State:** Backend · **Extensions:** `.spx,.ogg` · **MIME:** `audio/ogg`

Xiph.Org Speex speech codec in Ogg. Identified by the container; encode/decode require an approved Speex backend.

## Files

| File | Language | Role |
|------|----------|------|
| `speex.c` | C | Plugin vtable (`sleela_codec_plugin_speex`), state + dispatch. |
| `speex_detail.cpp` | C++ | Content signature probe (`sleela_speex_probe_bytes`). |
| `speex_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
