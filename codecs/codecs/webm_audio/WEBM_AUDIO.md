# WebM Audio Codec

**Codec id:** `SLEELA_CODEC_WEBM_AUDIO` · **State:** Container · **Extensions:** `.webm` · **MIME:** `audio/webm`

WebM container (a Matroska profile). Shares the EBML header; carries Opus or Vorbis audio rather than compressing itself.

## Files

| File | Language | Role |
|------|----------|------|
| `webm_audio.c` | C | Plugin vtable (`sleela_codec_plugin_webm_audio`), state + dispatch. |
| `webm_audio_detail.cpp` | C++ | Content signature probe (`sleela_webm_audio_probe_bytes`). |
| `webm_audio_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Container**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
