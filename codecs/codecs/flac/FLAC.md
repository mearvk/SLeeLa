# FLAC Codec

**Codec id:** `SLEELA_CODEC_FLAC` · **State:** Backend · **Extensions:** `.flac` · **MIME:** `audio/flac`

Free Lossless Audio Codec (Xiph). Identified by the 'fLaC' stream marker; encode/decode require an approved FLAC backend.

## Files

| File | Language | Role |
|------|----------|------|
| `flac.c` | C | Plugin vtable (`sleela_codec_plugin_flac`), state + dispatch. |
| `flac_detail.cpp` | C++ | Content signature probe (`sleela_flac_probe_bytes`). |
| `flac_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
