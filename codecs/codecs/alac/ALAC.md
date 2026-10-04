# ALAC Codec

**Codec id:** `SLEELA_CODEC_ALAC` · **State:** Backend · **Extensions:** `.m4a,.caf` · **MIME:** `audio/alac`

Apple Lossless Audio Codec, carried in MP4/CAF containers. Requires an approved ALAC backend for encode/decode.

## Files

| File | Language | Role |
|------|----------|------|
| `alac.c` | C | Plugin vtable (`sleela_codec_plugin_alac`), state + dispatch. |
| `alac_detail.cpp` | C++ | Content signature probe (`sleela_alac_probe_bytes`). |
| `alac_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — no reliable byte signature; resolve by extension or container.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
