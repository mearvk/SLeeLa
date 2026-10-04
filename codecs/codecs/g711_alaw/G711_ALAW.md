# G.711 A-law Codec

**Codec id:** `SLEELA_CODEC_G711_ALAW` · **State:** Native · **Extensions:** `.au`, `.alaw` · **MIME:** `audio/basic`

ITU-T G.711 A-law companding, the European telephony variant of G.711. Each
byte maps to one 16-bit PCM sample and back. Narrowband, single-channel, 8 kHz.

## Files

| File | Language | Role |
|------|----------|------|
| `g711_alaw.c` | C | Plugin vtable (`sleela_codec_plugin_g711_alaw`), decode/encode glue. |
| `g711_alaw_detail.cpp` | C++ | A-law-specific helper. |
| `g711_alaw_detail.h` | — | C-linkage declaration. |

The companding math itself is shared with the μ-law codec via
`../g711_mulaw/g711_detail.{h,cpp}`.

## Capabilities

- **decode** — expands A-law bytes to 16-bit PCM (mono, 8 kHz).
- **encode** — compands 16-bit PCM to A-law bytes. Call with `output == NULL` to
  query the byte count.
- **probe** — headerless; an explicit probe accepts any non-empty buffer.

Native and fully implemented — no external backend required.
