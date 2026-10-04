# MIDI Codec

**Codec id:** `SLEELA_CODEC_MIDI` · **State:** Event · **Extensions:** `.mid,.midi` · **MIME:** `audio/midi`

Standard MIDI note/event data (not sampled audio). Identified by the 'MThd' header chunk; it describes events, so there is no PCM decode.

## Files

| File | Language | Role |
|------|----------|------|
| `midi.c` | C | Plugin vtable (`sleela_codec_plugin_midi`), state + dispatch. |
| `midi_detail.cpp` | C++ | Content signature probe (`sleela_midi_probe_bytes`). |
| `midi_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Event**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
