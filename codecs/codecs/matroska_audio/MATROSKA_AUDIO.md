# Matroska Audio Codec

**Codec id:** `SLEELA_CODEC_MATROSKA_AUDIO` · **State:** Container · **Extensions:** `.mka` · **MIME:** `audio/x-matroska`

Matroska container (audio). Identified by the EBML 0x1A45DFA3 header; it transports a codec such as FLAC/Opus/AAC rather than compressing itself.

## Files

| File | Language | Role |
|------|----------|------|
| `matroska_audio.c` | C | Plugin vtable (`sleela_codec_plugin_matroska_audio`), state + dispatch. |
| `matroska_audio_detail.cpp` | C++ | Content signature probe (`sleela_matroska_audio_probe_bytes`). |
| `matroska_audio_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Container**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
