#ifndef SLEELA_CODEC_FLAC_NATIVE_H
#define SLEELA_CODEC_FLAC_NATIVE_H

/*
 * In-tree, dependency-free FLAC decode backend.
 *
 * This is a real sleela_codec_backend adapter for SLEELA_CODEC_FLAC: it decodes
 * a FLAC (Free Lossless Audio Codec, Xiph.Org) stream to interleaved signed
 * 16-bit PCM using only standard C -- no libFLAC, no third-party dependency.
 *
 * Coverage is the normative core of the FLAC format rather than every optional
 * feature. Decoded subframe types:
 *   - CONSTANT  (one value repeated for the whole subframe)
 *   - VERBATIM  (uncompressed samples)
 *   - FIXED     (fixed linear predictor orders 0..4 with Rice-coded residual,
 *                partition orders 0..n; this is what encoders emit for the many
 *                frames that an adaptive LPC model would not improve)
 * The LPC subframe type (adaptive quantized linear prediction) is detected and
 * reported as unsupported rather than mis-decoded. Only 16-bit sample depth is
 * produced on the PCM boundary (the SLeeLa PCM contract is 16-bit); other depths
 * are reported unsupported. This keeps every decoded sample bit-exact and never
 * fakes a decode.
 *
 * Register it with the codec manager's backend registry so the FLAC plugin's
 * decode() dispatches here:
 *
 *     sleela_flac_native_register();     // registers for SLEELA_CODEC_FLAC
 *     ...
 *     sleela_flac_native_unregister();   // clears it again
 */

#include "sleela_codec_plugin.h"   /* sleela_codec_backend, sleela_codec_result */

#ifdef __cplusplus
extern "C" {
#endif

/* The backend adapter singleton (decode-only; encode returns unsupported). */
const sleela_codec_backend *sleela_flac_native_backend(void);

/* Register / clear this backend for SLEELA_CODEC_FLAC. Returns SLEELA_CODEC_OK. */
sleela_codec_result sleela_flac_native_register(void);
sleela_codec_result sleela_flac_native_unregister(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_CODEC_FLAC_NATIVE_H */
