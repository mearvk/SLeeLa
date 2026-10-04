# G.711 μ-law Codec

**Codec id:** `SLEELA_CODEC_G711_MULAW` · **State:** Native · **Extensions:** `.au`, `.ulaw` · **MIME:** `audio/basic`

ITU-T G.711 μ-law companding, used for North-American/Japanese telephony. Each
byte maps to one 16-bit PCM sample and back. Narrowband, single-channel, 8 kHz.

## Files

| File | Language | Role |
|------|----------|------|
| `g711_mulaw.c` | C | Plugin vtable (`sleela_codec_plugin_g711_mulaw`), decode/encode glue. |
| `g711_detail.cpp` | C++ | Companding math for **both** μ-law and A-law (shared with `g711_alaw`). |
| `g711_detail.h` | — | C-linkage declarations for the companding functions. |

## Capabilities

- **decode** — expands μ-law bytes to 16-bit PCM (mono, 8 kHz).
- **encode** — compands 16-bit PCM to μ-law bytes. Call with `output == NULL` to
  query the byte count.
- **probe** — G.711 is headerless telephony data with no reliable magic, so an
  explicit probe accepts any non-empty buffer; prefer resolving μ-law by
  extension or caller knowledge.

Native and fully implemented — no external backend required.
