# HE-AAC Codec

**Codec id:** `SLEELA_CODEC_HE_AAC` · **State:** Backend · **Extensions:** `.aac,.m4a` · **MIME:** `audio/aac`

MPEG-4 High-Efficiency AAC (AAC + SBR/PS) for low bitrates. Requires an approved HE-AAC backend.

## Files

| File | Language | Role |
|------|----------|------|
| `he_aac.c` | C | Plugin vtable (`sleela_codec_plugin_he_aac`), state + dispatch. |
| `he_aac_detail.cpp` | C++ | Content signature probe (`sleela_he_aac_probe_bytes`). |
| `he_aac_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — no reliable byte signature; resolve by extension or container.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
