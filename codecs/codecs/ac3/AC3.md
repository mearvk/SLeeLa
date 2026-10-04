# Dolby Digital AC-3 Codec

**Codec id:** `SLEELA_CODEC_AC3` · **State:** Backend · **Extensions:** `.ac3` · **MIME:** `audio/ac3`

Dolby Digital AC-3 multichannel. Identified by the 0x0B77 sync word; encode/decode require an approved AC-3 backend.

## Files

| File | Language | Role |
|------|----------|------|
| `ac3.c` | C | Plugin vtable (`sleela_codec_plugin_ac3`), state + dispatch. |
| `ac3_detail.cpp` | C++ | Content signature probe (`sleela_ac3_probe_bytes`). |
| `ac3_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
