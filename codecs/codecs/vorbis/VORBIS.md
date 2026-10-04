# Ogg Vorbis Codec

**Codec id:** `SLEELA_CODEC_VORBIS` · **State:** Backend · **Extensions:** `.ogg` · **MIME:** `audio/ogg`

Xiph.Org Vorbis in an Ogg container. Identified by the 'OggS' page signature; encode/decode require an approved Vorbis backend.

## Files

| File | Language | Role |
|------|----------|------|
| `vorbis.c` | C | Plugin vtable (`sleela_codec_plugin_vorbis`), state + dispatch. |
| `vorbis_detail.cpp` | C++ | Content signature probe (`sleela_vorbis_probe_bytes`). |
| `vorbis_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
