# Opus Codec

**Codec id:** `SLEELA_CODEC_OPUS` · **State:** Backend · **Extensions:** `.opus,.ogg,.webm` · **MIME:** `audio/opus`

IETF Opus (RFC 6716), usually in Ogg or WebM. Identified by the container page/marker; encode/decode require an approved Opus backend.

## Files

| File | Language | Role |
|------|----------|------|
| `opus.c` | C | Plugin vtable (`sleela_codec_plugin_opus`), state + dispatch. |
| `opus_detail.cpp` | C++ | Content signature probe (`sleela_opus_probe_bytes`). |
| `opus_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
