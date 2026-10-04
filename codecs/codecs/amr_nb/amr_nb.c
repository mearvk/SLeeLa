/*
 * AMR-NB codec -- Backend handler.
 *
 * Adaptive Multi-Rate Narrowband speech. Identified by the '#!AMR\n' magic; encode/decode require an approved AMR-NB backend.
 *
 * This handler identifies the format (probe) and advertises its honest state
 * (SLEELA_CODEC_BACKEND); the C++ signature probe is in amr_nb_detail.cpp. The manager
 * loads it via sleela_codec_plugin_amr_nb().
 */

#include "sleela_codec_plugin.h"
#include "amr_nb_detail.h"

static const sleela_codec_handler k_meta = {
    SLEELA_CODEC_AMR_NB, "AMR-NB", "audio/amr", ".amr",
    SLEELA_CODEC_BACKEND, 1, 1
};

static const sleela_codec_handler *amr_nb_describe(void) { return &k_meta; }

static sleela_codec_result amr_nb_probe(sleela_codec_instance *inst,
                                        const uint8_t *data, size_t len) {
    (void)inst;
    return sleela_amr_nb_probe_bytes(data, len);
}

static sleela_codec_result amr_nb_decode(sleela_codec_instance *inst,
                                         const uint8_t *input, size_t input_len,
                                         sleela_pcm_buffer *out) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_AMR_NB);
    if (b && b->decode) return b->decode(input, input_len, out);
    /* No backend wired: report cleanly rather than fake a decode. Register an
     * approved library adapter via sleela_codec_backend_register(SLEELA_CODEC_AMR_NB, ...). */
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static sleela_codec_result amr_nb_encode(sleela_codec_instance *inst,
                                         const sleela_pcm_buffer *pcm,
                                         uint8_t *output, size_t *inout_len) {
    (void)inst;
    const sleela_codec_backend *b = sleela_codec_backend_get(SLEELA_CODEC_AMR_NB);
    if (b && b->encode) return b->encode(pcm, output, inout_len);
    return SLEELA_CODEC_ERR_UNSUPPORTED;
}

static const sleela_codec_plugin k_plugin = {
    amr_nb_describe, /*load*/ 0, /*unload*/ 0, amr_nb_probe, amr_nb_decode, amr_nb_encode
};

const sleela_codec_plugin *sleela_codec_plugin_amr_nb(void) { return &k_plugin; }
