# PCM/WAV Codec

**Codec id:** `SLEELA_CODEC_PCM_WAV` · **State:** Native · **Extensions:** `.wav` · **MIME:** `audio/wav`

Uncompressed 16-bit signed PCM in a RIFF/WAVE container. This is the one codec
SLeeLa implements end to end, and it is the **PCM boundary** every other codec
decodes toward and encodes from.

## Files

| File | Language | Role |
|------|----------|------|
| `pcm_wav.c` | C | Plugin vtable (`sleela_codec_plugin_pcm_wav`), decode/encode glue. |
| `pcm_wav.cpp` | C++ | RIFF chunk scanning (`sleela_wav_parse`) and canonical header writer (`sleela_wav_write_header`). |
| `pcm_wav_detail.h` | — | C-linkage declarations shared by the two. |

## Capabilities

- **probe** — recognizes `RIFF`…`WAVE` at offset 0.
- **decode** — parses the `fmt ` and `data` chunks and returns interleaved
  16-bit PCM (`sleela_pcm_buffer`). Non-PCM or non-16-bit WAVE is reported as
  `SLEELA_CODEC_ERR_UNSUPPORTED` rather than mis-decoded.
- **encode** — writes a 44-byte canonical PCM WAVE header followed by 16-bit
  little-endian samples. Call with `output == NULL` to query the required size.

## Loading

```c
sleela_codec_manager *m = sleela_codec_manager_create();
sleela_codec_manager_load(m, SLEELA_CODEC_PCM_WAV);
sleela_pcm_buffer pcm;
sleela_codec_manager_decode(m, SLEELA_CODEC_PCM_WAV, bytes, len, &pcm);
sleela_codec_manager_destroy(m);   /* unloads the codec */
```

SLeeLa calls the manager, never this plugin directly.
