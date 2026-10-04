# AIFF Codec

**Codec id:** `SLEELA_CODEC_AIFF` · **State:** Recognized · **Extensions:** `.aif,.aiff` · **MIME:** `audio/aiff`

Audio Interchange File Format (Apple). Recognized and identified by its FORM/AIFF container signature; sample decode is not claimed.

## Files

| File | Language | Role |
|------|----------|------|
| `aiff.c` | C | Plugin vtable (`sleela_codec_plugin_aiff`), state + dispatch. |
| `aiff_detail.cpp` | C++ | Content signature probe (`sleela_aiff_probe_bytes`). |
| `aiff_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Recognized**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
