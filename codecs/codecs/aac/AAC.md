# AAC Codec

**Codec id:** `SLEELA_CODEC_AAC` · **State:** Backend · **Extensions:** `.aac,.m4a` · **MIME:** `audio/aac`

MPEG-2/4 Advanced Audio Coding (ADTS or MP4). Requires an approved AAC backend for encode/decode.

## Files

| File | Language | Role |
|------|----------|------|
| `aac.c` | C | Plugin vtable (`sleela_codec_plugin_aac`), state + dispatch. |
| `aac_detail.cpp` | C++ | Content signature probe (`sleela_aac_probe_bytes`). |
| `aac_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — no reliable byte signature; resolve by extension or container.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
