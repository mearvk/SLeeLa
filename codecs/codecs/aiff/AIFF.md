# AIFF Codec

**Codec id:** `SLEELA_CODEC_AIFF` · **State:** Native · **Extensions:** `.aif`, `.aiff` · **MIME:** `audio/aiff`

Audio Interchange File Format (Apple): uncompressed PCM in a `FORM`/`AIFF`
container with **big-endian** samples. Like PCM/WAV this is pure byte
manipulation and needs no third-party library, so AIFF is implemented
**natively** — not merely recognized.

## Files

| File | Language | Role |
|------|----------|------|
| `aiff.c` | C | Plugin vtable (`sleela_codec_plugin_aiff`), decode/encode glue. |
| `aiff_detail.cpp` | C++ | `FORM`/`AIFF` chunk reader/writer, including the IEEE-754 80-bit extended sample-rate field in the `COMM` chunk. |
| `aiff_detail.h` | — | C-linkage declarations. |

## Capabilities

- **probe** — recognizes `FORM`…`AIFF` at offset 0.
- **decode** — parses `COMM` + `SSND` and returns interleaved 16-bit PCM
  (converting from big-endian). Non-16-bit AIFF is reported as
  `SLEELA_CODEC_ERR_UNSUPPORTED`.
- **encode** — writes a canonical 54-byte AIFF header (`FORM`/`COMM`/`SSND`,
  with the sample rate encoded as an 80-bit extended float) followed by
  big-endian 16-bit samples. Call with `output == NULL` to query the size.

Round-trips losslessly through the Codec Manager (verified in
`tests/test_manager.c`). Native and fully implemented — no external backend
required.
