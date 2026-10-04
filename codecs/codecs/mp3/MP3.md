# MPEG Layer III Codec

**Codec id:** `SLEELA_CODEC_MP3` · **State:** Backend · **Extensions:** `.mp3` · **MIME:** `audio/mpeg`

MPEG-1/2 Audio Layer III. Identified by an ID3 tag or MPEG frame sync; encode/decode require an approved MP3 backend.

## Files

| File | Language | Role |
|------|----------|------|
| `mp3.c` | C | Plugin vtable (`sleela_codec_plugin_mp3`), state + dispatch. |
| `mp3_detail.cpp` | C++ | Content signature probe (`sleela_mp3_probe_bytes`). |
| `mp3_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
