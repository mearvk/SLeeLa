#ifndef SLEELA_AUDIO_MIXER_H
#define SLEELA_AUDIO_MIXER_H
#include <stddef.h>
#include <stdint.h>
#include "sleela_media.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum { SL_AUDIO_ROLE_MASTER=1, SL_AUDIO_ROLE_SECONDARY=2, SL_AUDIO_ROLE_INPUT=3 } SLAudioTrackRole;
typedef enum { SL_AUDIO_SOURCE_LIVE=1, SL_AUDIO_SOURCE_FILE=2 } SLAudioSourceKind;
typedef struct {
    float bass_db;
    float mid_db;
    float treble_db;
    float gain_db;
    float pan;
    float channel_gain[8];
} SLAudioMixerControls;
typedef struct {
    uint64_t id;
    SLAudioTrackRole role;
    SLAudioSourceKind source_kind;
    const char *source;
    int64_t start_offset_ns;
    float quality;
    float gain_db;
    uint32_t channels;
    uint32_t sample_rate;
} SLAudioTrackSpec;
typedef struct {
    uint64_t output_sequence;
    uint64_t timestamp_ns;
    uint32_t sample_rate;
    uint32_t channels;
    size_t frames;
    float *samples;
} SLAudioMixOutput;
typedef struct {
    SLAudioMixerControls controls;
    uint32_t sample_rate;
    uint32_t channels;
    float bass_state[8];
    float treble_state[8];
    uint64_t sequence;
} SLAudioMixer;

void slaudio_mixer_init(SLAudioMixer *mixer, uint32_t sample_rate, uint32_t channels);
int slaudio_mixer_set_controls(SLAudioMixer *mixer, const SLAudioMixerControls *controls);
int slaudio_mixer_process(SLAudioMixer *mixer, const SLAudioTrackSpec *track,
                          const float *input, size_t frames, uint64_t timestamp_ns,
                          SLAudioMixOutput *output);
int slaudio_mixer_mix(SLAudioMixer *mixer, const SLAudioTrackSpec *tracks, size_t track_count,
                      const float *const *inputs, const size_t *frame_counts,
                      uint64_t timeline_ns, SLAudioMixOutput *output);
int slaudio_mixer_apply_eq(SLAudioMixer *mixer, float *samples, size_t frames);
int slaudio_mixer_role_name(SLAudioTrackRole role, const char **name);
int slaudio_mixer_source_name(SLAudioSourceKind kind, const char **name);

#ifdef __cplusplus
}
#endif
#endif
