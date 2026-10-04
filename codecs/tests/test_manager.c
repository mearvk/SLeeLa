/*
 * Codec Manager test: load -> use -> unload, across native and backend codecs.
 * Exercises the Loader/Unloader/Management Controller that SLeeLa calls.
 */

#include "sleela_codec_manager.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* A stand-in backend adapter: the shape a real libFLAC/libopus adapter takes.
 * It just records that it ran, proving the manager dispatches to a registered
 * backend for a BACKEND-state codec. */
static int g_fake_flac_calls = 0;
static sleela_codec_result fake_flac_decode(const uint8_t *input, size_t len,
                                            sleela_pcm_buffer *out) {
    (void)input; (void)len; (void)out;
    g_fake_flac_calls++;
    return SLEELA_CODEC_OK;
}
static const sleela_codec_backend g_fake_flac = { "fake-flac", fake_flac_decode, NULL };

int main(void) {
    sleela_codec_manager *m = sleela_codec_manager_create();
    assert(m != NULL);
    assert(sleela_codec_manager_count(m) == SLEELA_CODEC_COUNT);

    /* Nothing is loaded until asked. */
    assert(sleela_codec_manager_is_loaded(m, SLEELA_CODEC_PCM_WAV) == 0);

    /* --- Load a codec, confirm the loaded flag, double-load is idempotent. */
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_PCM_WAV) == SLEELA_CODEC_OK);
    assert(sleela_codec_manager_is_loaded(m, SLEELA_CODEC_PCM_WAV) == 1);
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_PCM_WAV) == SLEELA_CODEC_OK);

    /* --- Native PCM/WAV round-trip through the manager. */
    sleela_pcm_buffer pcm;
    int16_t src[8] = { 0, 1000, -1000, 32767, -32768, 500, -500, 7 };
    pcm.samples = src; pcm.frame_count = 8; pcm.sample_rate = 44100; pcm.channels = 1;

    size_t need = 0;
    assert(sleela_codec_manager_encode(m, SLEELA_CODEC_PCM_WAV, &pcm, NULL, &need) == SLEELA_CODEC_OK);
    assert(need == 44 + 8 * 2);
    uint8_t *wav = (uint8_t *)malloc(need);
    size_t cap = need;
    assert(sleela_codec_manager_encode(m, SLEELA_CODEC_PCM_WAV, &pcm, wav, &cap) == SLEELA_CODEC_OK);
    assert(memcmp(wav, "RIFF", 4) == 0 && memcmp(wav + 8, "WAVE", 4) == 0);

    sleela_pcm_buffer back;
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_PCM_WAV, wav, cap, &back) == SLEELA_CODEC_OK);
    assert(back.frame_count == 8 && back.channels == 1 && back.sample_rate == 44100);
    for (int i = 0; i < 8; ++i) assert(back.samples[i] == src[i]);  /* lossless */

    /* Probe recognizes the WAV we just wrote (only loaded codecs probe). */
    assert(sleela_codec_manager_resolve_probe(m, wav, cap) == SLEELA_CODEC_PCM_WAV);
    free(wav);

    /* --- Native G.711 mu-law round-trip (companding is lossy but monotone-ish;
     *     just confirm it runs and sizes are right). */
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_G711_MULAW) == SLEELA_CODEC_OK);
    uint8_t ulaw[8]; size_t ucap = sizeof(ulaw);
    assert(sleela_codec_manager_encode(m, SLEELA_CODEC_G711_MULAW, &pcm, ulaw, &ucap) == SLEELA_CODEC_OK);
    assert(ucap == 8);
    sleela_pcm_buffer udec;
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_G711_MULAW, ulaw, 8, &udec) == SLEELA_CODEC_OK);
    assert(udec.frame_count == 8 && udec.channels == 1);

    /* --- Native AIFF round-trip through the manager (big-endian container). */
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_AIFF) == SLEELA_CODEC_OK);
    const sleela_codec_handler *aiff = sleela_codec_manager_describe(m, SLEELA_CODEC_AIFF);
    assert(aiff && aiff->state == SLEELA_CODEC_NATIVE);
    size_t aneed = 0;
    assert(sleela_codec_manager_encode(m, SLEELA_CODEC_AIFF, &pcm, NULL, &aneed) == SLEELA_CODEC_OK);
    uint8_t *aiffbuf = (uint8_t *)malloc(aneed);
    size_t acap = aneed;
    assert(sleela_codec_manager_encode(m, SLEELA_CODEC_AIFF, &pcm, aiffbuf, &acap) == SLEELA_CODEC_OK);
    assert(memcmp(aiffbuf, "FORM", 4) == 0 && memcmp(aiffbuf + 8, "AIFF", 4) == 0);
    sleela_pcm_buffer aback;
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_AIFF, aiffbuf, acap, &aback) == SLEELA_CODEC_OK);
    assert(aback.frame_count == 8 && aback.channels == 1 && aback.sample_rate == 44100);
    for (int i = 0; i < 8; ++i) assert(aback.samples[i] == src[i]);  /* lossless */
    free(aiffbuf);

    /* --- A backend codec: present and resolvable, but decode fails cleanly. */
    const sleela_codec_handler *flac = sleela_codec_manager_describe(m, SLEELA_CODEC_FLAC);
    assert(flac && flac->state == SLEELA_CODEC_BACKEND);
    assert(sleela_codec_manager_resolve_extension(m, ".flac") == SLEELA_CODEC_FLAC);
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_FLAC) == SLEELA_CODEC_OK);
    sleela_pcm_buffer dummy;
    const uint8_t fake[4] = { 'f','L','a','C' };
    /* No backend registered yet -> clean unsupported. */
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, fake, 4, &dummy) == SLEELA_CODEC_ERR_UNSUPPORTED);

    /* --- Register a backend adapter and confirm the manager dispatches to it. */
    assert(sleela_codec_backend_register(SLEELA_CODEC_FLAC, &g_fake_flac) == SLEELA_CODEC_OK);
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, fake, 4, &dummy) == SLEELA_CODEC_OK);
    assert(g_fake_flac_calls == 1);  /* the adapter's decode ran */
    assert(sleela_codec_backend_register(SLEELA_CODEC_FLAC, NULL) == SLEELA_CODEC_OK); /* clear */
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_FLAC, fake, 4, &dummy) == SLEELA_CODEC_ERR_UNSUPPORTED);

    /* Decoding a NOT-loaded codec returns ERR_STATE, not a crash. */
    assert(sleela_codec_manager_is_loaded(m, SLEELA_CODEC_OPUS) == 0);
    assert(sleela_codec_manager_decode(m, SLEELA_CODEC_OPUS, fake, 4, &dummy) == SLEELA_CODEC_ERR_STATE);

    /* --- Unload and confirm. */
    assert(sleela_codec_manager_unload(m, SLEELA_CODEC_PCM_WAV) == SLEELA_CODEC_OK);
    assert(sleela_codec_manager_is_loaded(m, SLEELA_CODEC_PCM_WAV) == 0);
    assert(sleela_codec_manager_unload(m, SLEELA_CODEC_PCM_WAV) == SLEELA_CODEC_OK); /* idempotent */

    sleela_codec_manager_unload_all(m);
    assert(sleela_codec_manager_is_loaded(m, SLEELA_CODEC_G711_MULAW) == 0);

    sleela_codec_manager_destroy(m);  /* also unloads anything still loaded */
    return 0;
}
