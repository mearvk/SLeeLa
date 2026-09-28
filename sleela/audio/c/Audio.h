#ifndef SLEELA_AUDIO_H
#define SLEELA_AUDIO_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct SleelaAudioInput{const char*path;double start_seconds;double gain_db;}SleelaAudioInput; typedef struct SleelaAudioControls{double bass_db,mid_db,treble_db,master_gain_db,pan,left_gain,right_gain;}SleelaAudioControls; typedef struct SleelaAudioConfiguration{int sample_rate;const char*output_path;const SleelaAudioInput*inputs;size_t input_count;SleelaAudioControls controls;}SleelaAudioConfiguration; typedef struct SleelaAudioDevice{char name[128];char identifier[128];int sample_rate;int channels;int input;int output;}SleelaAudioDevice; typedef struct SleelaAudioStream{SleelaAudioDevice device;int sample_rate;int channels;int opened;int running;}SleelaAudioStream; typedef struct SleelaAudioSystem{char platform[32];}SleelaAudioSystem;
int sleela_audio_validate_input(const SleelaAudioInput*);int sleela_audio_validate_controls(const SleelaAudioControls*);int sleela_audio_validate_configuration(const SleelaAudioConfiguration*);int sleela_audio_mix(const SleelaAudioConfiguration*,char*,size_t);int sleela_audio_native_render(const char*,const SleelaAudioConfiguration*);void sleela_audio_native_interrupt(void);int sleela_audio_device_available(const SleelaAudioDevice*);int sleela_audio_stream_open(SleelaAudioStream*);int sleela_audio_stream_start(SleelaAudioStream*);void sleela_audio_stream_stop(SleelaAudioStream*);void sleela_audio_stream_close(SleelaAudioStream*);void sleela_audio_system_init(SleelaAudioSystem*);size_t sleela_audio_system_enumerate(const SleelaAudioSystem*,SleelaAudioDevice*,size_t);const char* sleela_audio_system_platform(const SleelaAudioSystem*);
#ifdef __cplusplus
}
#endif
#endif
