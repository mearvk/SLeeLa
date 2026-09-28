#ifndef SLEELA_CODECS_H
#define SLEELA_CODECS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum sleela_codec_state {
    SLEELA_CODEC_NATIVE = 0,
    SLEELA_CODEC_BACKEND = 1,
    SLEELA_CODEC_RECOGNIZED = 2,
    SLEELA_CODEC_CONTAINER = 3,
    SLEELA_CODEC_EVENT = 4
} sleela_codec_state;

typedef enum sleela_codec_id {
    SLEELA_CODEC_PCM_WAV = 0,
    SLEELA_CODEC_AIFF,
    SLEELA_CODEC_FLAC,
    SLEELA_CODEC_ALAC,
    SLEELA_CODEC_MP3,
    SLEELA_CODEC_AAC,
    SLEELA_CODEC_HE_AAC,
    SLEELA_CODEC_VORBIS,
    SLEELA_CODEC_OPUS,
    SLEELA_CODEC_SPEEX,
    SLEELA_CODEC_WMA,
    SLEELA_CODEC_AC3,
    SLEELA_CODEC_EAC3,
    SLEELA_CODEC_AMR_NB,
    SLEELA_CODEC_AMR_WB,
    SLEELA_CODEC_G711_MULAW,
    SLEELA_CODEC_G711_ALAW,
    SLEELA_CODEC_MIDI,
    SLEELA_CODEC_MATROSKA_AUDIO,
    SLEELA_CODEC_WEBM_AUDIO,
    SLEELA_CODEC_COUNT
} sleela_codec_id;

typedef struct sleela_codec_handler {
    sleela_codec_id id;
    const char *name;
    const char *mime;
    const char *extensions;
    sleela_codec_state state;
    int can_decode;
    int can_encode;
} sleela_codec_handler;

size_t sleela_codec_count(void);
const sleela_codec_handler *sleela_codec_at(size_t index);
const sleela_codec_handler *sleela_codec_by_id(sleela_codec_id id);
const sleela_codec_handler *sleela_codec_by_extension(const char *extension);

#ifdef __cplusplus
}
#endif

#endif
