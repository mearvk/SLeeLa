/*
 * PCM/WAV codec -- native handler.
 *
 * Reads and writes uncompressed 16-bit signed PCM in a RIFF/WAVE container.
 * This is the one codec SLeeLa implements end to end; it is the PCM boundary
 * every other codec decodes toward and encodes from. The manager loads this
 * plugin via sleela_codec_plugin_pcm_wav().
 *
 * C/C++ split: the byte-level RIFF chunk scanning that benefits from a little
 * structure lives in pcm_wav.cpp (sleela_wav_find_chunk / sleela_wav_header_*),
 * declared in pcm_wav_detail.h; this C file is the plugin vtable and the
 * decode/encode glue.
 */

#include "sleela_codec_plugin.h"
#include "pcm_wav_detail.h"

#include <stdlib.h>
#include <string.h>

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_PCM_WAV, "PCM/WAV", "audio/wav", ".wav",
    SLEELA_CODEC_NATIVE, 1, 1
};

struct sleela_codec_instance { int16_t *owned; };

static const sleela_codec_handler *pcm_wav_describe(void) { return &k_meta; }

static sleela_codec_result pcm_wav_load(sleela_codec_instance **out) {
    sleela_codec_instance *inst = (sleela_codec_instance *)calloc(1, sizeof(*inst));
    if (!inst) return SLEELA_CODEC_ERR_IO;
    *out = inst;
    return SLEELA_CODEC_OK;
}

static void pcm_wav_unload(sleela_codec_instance *inst) {
    if (!inst) return;
    free(inst->owned);
    free(inst);
}

/* "RIFF"...."WAVE" at the top of the file. */
static sleela_codec_result pcm_wav_probe(sleela_codec_instance *inst,
                                         const uint8_t *data, size_t len) {
    (void)inst;
    if (!data || len < 12) return SLEELA_CODEC_ERR_FORMAT;
    if (memcmp(data, "RIFF", 4) != 0 || memcmp(data + 8, "WAVE", 4) != 0)
        return SLEELA_CODEC_ERR_FORMAT;
    return SLEELA_CODEC_OK;
}

static sleela_codec_result pcm_wav_decode(sleela_codec_instance *inst,
                                          const uint8_t *input, size_t input_len,
                                          sleela_pcm_buffer *out) {
    if (!inst || !input || !out) return SLEELA_CODEC_ERR_INVALID;

    sleela_wav_format fmt;
    const uint8_t *pcm; size_t pcm_bytes;
    sleela_codec_result r = sleela_wav_parse(input, input_len, &fmt, &pcm, &pcm_bytes);
    if (r != SLEELA_CODEC_OK) return r;
    if (fmt.bits_per_sample != 16) return SLEELA_CODEC_ERR_UNSUPPORTED; /* 16-bit native path */

    size_t sample_count = pcm_bytes / 2;
    int16_t *samples = (int16_t *)malloc(sample_count * sizeof(int16_t));
    if (!samples) return SLEELA_CODEC_ERR_IO;
    for (size_t i = 0; i < sample_count; ++i)
        samples[i] = (int16_t)((uint16_t)pcm[i * 2] | ((uint16_t)pcm[i * 2 + 1] << 8)); /* LE */

    free(inst->owned);
    inst->owned = samples;
    out->samples = samples;
    out->channels = fmt.channels ? fmt.channels : 1;
    out->sample_rate = fmt.sample_rate ? fmt.sample_rate : 44100;
    out->frame_count = sample_count / (out->channels ? out->channels : 1);
    return SLEELA_CODEC_OK;
}

static sleela_codec_result pcm_wav_encode(sleela_codec_instance *inst,
                                          const sleela_pcm_buffer *pcm,
                                          uint8_t *output, size_t *inout_len) {
    (void)inst;
    if (!pcm || !pcm->samples || !inout_len) return SLEELA_CODEC_ERR_INVALID;
    size_t samples = pcm->frame_count * (pcm->channels ? pcm->channels : 1);
    size_t needed = 44 + samples * 2;                 /* 44-byte header + 16-bit LE PCM */
    if (!output) { *inout_len = needed; return SLEELA_CODEC_OK; } /* size query */
    if (*inout_len < needed) { *inout_len = needed; return SLEELA_CODEC_ERR_IO; }

    sleela_wav_write_header(output, pcm->channels ? pcm->channels : 1,
                            pcm->sample_rate ? pcm->sample_rate : 44100, 16, samples * 2);
    for (size_t i = 0; i < samples; ++i) {
        uint16_t s = (uint16_t)pcm->samples[i];
        output[44 + i * 2]     = (uint8_t)(s & 0xFF);
        output[44 + i * 2 + 1] = (uint8_t)(s >> 8);
    }
    *inout_len = needed;
    return SLEELA_CODEC_OK;
}

static const sleela_codec_plugin k_plugin = {
    pcm_wav_describe, pcm_wav_load, pcm_wav_unload,
    pcm_wav_probe, pcm_wav_decode, pcm_wav_encode
};

const sleela_codec_plugin *sleela_codec_plugin_pcm_wav(void) { return &k_plugin; }
