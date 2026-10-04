/*
 * Codec Manager test: load -> use -> unload, across native and backend codecs.
 * Exercises the Loader/Unloader/Management Controller that SLeeLa calls.
 */

#include "sleela_codec_manager.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

    /* --- A backend codec: present and resolvable, but decode fails cleanly. */
    const sleela_codec_handler *flac = sleela_codec_manager_describe(m, SLEELA_CODEC_FLAC);
    assert(flac && flac->state == SLEELA_CODEC_BACKEND);
    assert(sleela_codec_manager_resolve_extension(m, ".flac") == SLEELA_CODEC_FLAC);
    assert(sleela_codec_manager_load(m, SLEELA_CODEC_FLAC) == SLEELA_CODEC_OK);
    sleela_pcm_buffer dummy;
    const uint8_t fake[4] = { 'f','L','a','C' };
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
