#ifndef SLEELA_AUDIO_H
#define SLEELA_AUDIO_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_audio_input { const char *id; const char *path; double start_seconds; double gain_db; } sleela_audio_input;
typedef struct sleela_audio_controls { double bass_db, mid_db, treble_db, master_gain_db, pan, left_gain, right_gain; } sleela_audio_controls;
typedef struct sleela_audio_config { uint32_t sample_rate; const sleela_audio_input *inputs; uint32_t input_count; sleela_audio_controls controls; const char *output_path; } sleela_audio_config;
int sleela_audio_validate(const sleela_audio_config *config);
int sleela_audio_mix_wav(const sleela_audio_config *config);
const char *sleela_audio_last_error(void);
#ifdef __cplusplus
}
#endif
#endif
