# flac-native — in-tree FLAC decode backend

A real, dependency-free decode backend for `SLEELA_CODEC_FLAC`. It turns FLAC
from "recognized, no backend wired" into a codec that **actually decodes** a
FLAC stream to interleaved signed 16-bit PCM — with no libFLAC and no other
third-party dependency.

It plugs into the existing codec architecture through the documented backend
ABI (`sleela_codec_backend` in `codecs/include/sleela_codec_plugin.h`): the FLAC
plugin's `decode()` already dispatches to whatever backend is registered for its
id, and this adapter is that backend.

## Use

```c
#include "flac_native.h"

sleela_flac_native_register();            // register for SLEELA_CODEC_FLAC
// ... now sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, bytes, len, &pcm)
//     returns SLEELA_CODEC_OK with real samples ...
sleela_flac_native_unregister();          // clear it again
```

## Coverage (honest scope)

Decoded, bit-exact, following the FLAC format specification (Xiph.Org):

- **STREAMINFO** metadata parsing (sample rate, channels, bit depth, total
  samples); other metadata blocks are skipped.
- Frame headers with fixed/variable blocking and the UTF-8 coded frame/sample
  number; all standard block-size and sample-rate codes.
- Subframe types **CONSTANT**, **VERBATIM**, and **FIXED** (fixed linear
  predictor orders 0–4) with **Rice-coded residuals** (coding methods 0 and 1,
  including the raw-bits escape, and partition orders 0..n).
- Stereo decorrelation: independent, left/side, right/side, and mid/side.
- Wasted-bits subframes.

**Deliberately not decoded** (reported as a clean
`SLEELA_CODEC_ERR_UNSUPPORTED`, never mis-decoded):

- The **LPC** subframe type (adaptive quantized linear prediction). Many FLAC
  files use LPC for most frames, so this backend fully decodes CONSTANT/
  VERBATIM/FIXED streams and honestly declines LPC ones rather than producing
  wrong samples. A future revision can add the LPC restoration filter.
- Non-16-bit sample depths (the SLeeLa PCM boundary is 16-bit).
- Encoding (`encode` returns unsupported; this is a decode backend).

This matches the project's codec principle: the management layer never fakes a
successful decode, and capability reporting stays truthful.

## Tests

`codecs/tests/test_flac_native.c` builds real FLAC streams byte by byte and
decodes them through the public manager path, asserting:

1. a VERBATIM stereo frame decodes to bit-exact interleaved L/R samples;
2. a FIXED order-0 frame with a Rice-coded residual decodes bit-exact
   (exercising the unary+binary Rice decoder and the fixed filter);
3. an LPC subframe is reported unsupported rather than mis-decoded;
4. clearing the backend restores a clean unsupported result.

Run with `make test` in `codecs/`.
