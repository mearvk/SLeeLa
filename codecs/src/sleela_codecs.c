#include "sleela_codecs.h"

#include <ctype.h>
#include <string.h>

#define C(id, name, mime, ext, state, dec, enc) \
    { id, name, mime, ext, state, dec, enc }

static const sleela_codec_handler handlers[SLEELA_CODEC_COUNT] = {
    C(SLEELA_CODEC_PCM_WAV, "PCM/WAV", "audio/wav", ".wav", SLEELA_CODEC_NATIVE, 1, 1),
    C(SLEELA_CODEC_AIFF, "AIFF", "audio/aiff", ".aif,.aiff", SLEELA_CODEC_NATIVE, 1, 1),
    C(SLEELA_CODEC_FLAC, "FLAC", "audio/flac", ".flac", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_ALAC, "ALAC", "audio/alac", ".m4a,.caf", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_MP3, "MPEG Layer III", "audio/mpeg", ".mp3", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_AAC, "AAC", "audio/aac", ".aac,.m4a", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_HE_AAC, "HE-AAC", "audio/aac", ".aac,.m4a", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_VORBIS, "Ogg Vorbis", "audio/ogg", ".ogg", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_OPUS, "Opus", "audio/opus", ".opus,.ogg,.webm", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_SPEEX, "Speex", "audio/ogg", ".spx,.ogg", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_WMA, "Windows Media Audio", "audio/x-ms-wma", ".wma", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_AC3, "Dolby Digital AC-3", "audio/ac3", ".ac3", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_EAC3, "Dolby Digital Plus", "audio/eac3", ".eac3,.ec3", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_AMR_NB, "AMR-NB", "audio/amr", ".amr", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_AMR_WB, "AMR-WB", "audio/amr-wb", ".awb", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_G711_MULAW, "G.711 mu-law", "audio/basic", ".au,.ulaw", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_G711_ALAW, "G.711 A-law", "audio/basic", ".au,.alaw", SLEELA_CODEC_BACKEND, 1, 1),
    C(SLEELA_CODEC_MIDI, "MIDI", "audio/midi", ".mid,.midi", SLEELA_CODEC_EVENT, 1, 1),
    C(SLEELA_CODEC_MATROSKA_AUDIO, "Matroska Audio", "audio/x-matroska", ".mka", SLEELA_CODEC_CONTAINER, 1, 1),
    C(SLEELA_CODEC_WEBM_AUDIO, "WebM Audio", "audio/webm", ".webm", SLEELA_CODEC_CONTAINER, 1, 1)
};

size_t sleela_codec_count(void) {
    return SLEELA_CODEC_COUNT;
}

const sleela_codec_handler *sleela_codec_at(size_t index) {
    return index < SLEELA_CODEC_COUNT ? &handlers[index] : NULL;
}

const sleela_codec_handler *sleela_codec_by_id(sleela_codec_id id) {
    return id >= 0 && id < SLEELA_CODEC_COUNT ? &handlers[id] : NULL;
}

static int extension_equals(const char *candidate, const char *extension) {
    while (*candidate && *extension) {
        if (tolower((unsigned char)*candidate) !=
            tolower((unsigned char)*extension)) return 0;
        ++candidate;
        ++extension;
    }
    return *candidate == '\0' && *extension == '\0';
}

const sleela_codec_handler *sleela_codec_by_extension(const char *extension) {
    if (!extension || !*extension) return NULL;

    for (size_t i = 0; i < SLEELA_CODEC_COUNT; ++i) {
        const char *p = handlers[i].extensions;
        while (*p) {
            char token[16];
            size_t n = 0;
            while (*p && *p != ',' && n + 1 < sizeof(token))
                token[n++] = *p++;
            token[n] = '\0';
            if (extension_equals(extension, token)) return &handlers[i];
            if (*p == ',') ++p;
        }
    }
    return NULL;
}
