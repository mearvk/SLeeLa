#ifndef SLEELA_AUDIO_BRIDGE_H
#define SLEELA_AUDIO_BRIDGE_H

#include "sleela_core.h"

#ifdef __cplusplus
extern "C" {
#endif

int sleela_audio_native_render_bridge(
    const char* output_path, uint32_t sample_rate,
    const char* const* input_paths, const double* start_seconds,
    const double* gain_db, size_t input_count,
    double bass_db, double mid_db, double treble_db,
    double master_gain_db, double pan, double left_gain, double right_gain,
    void* user);

#ifdef __cplusplus
}
#endif
#endif
