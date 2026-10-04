# Dolby Digital Plus Codec

**Codec id:** `SLEELA_CODEC_EAC3` · **State:** Backend · **Extensions:** `.eac3,.ec3` · **MIME:** `audio/eac3`

Dolby Digital Plus (E-AC-3). Shares the 0x0B77 sync word with AC-3; encode/decode require an approved E-AC-3 backend.

## Files

| File | Language | Role |
|------|----------|------|
| `eac3.c` | C | Plugin vtable (`sleela_codec_plugin_eac3`), state + dispatch. |
| `eac3_detail.cpp` | C++ | Content signature probe (`sleela_eac3_probe_bytes`). |
| `eac3_detail.h` | — | C-linkage declaration. |

## Capabilities

- **probe** — recognizes the format signature.
- **decode / encode** — this handler is **Backend**; it returns
  `SLEELA_CODEC_ERR_UNSUPPORTED` from decode/encode until an approved backend is
  wired, rather than faking success. The manager reports the state so callers
  fail cleanly.

SLeeLa calls the Codec Manager, which loads this plugin by id; it never calls
the plugin directly.
