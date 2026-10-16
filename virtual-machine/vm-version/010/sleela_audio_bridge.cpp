#include "sleela_audio_bridge.h"
#include "../../audio/cpp/include/sleela_audio.hpp"
#include <cstdint>
#include <string>
#include <utility>

int sleela_audio_native_render_bridge(
    const char* output_path, uint32_t sample_rate,
    const char* const* input_paths, const double* start_seconds,
    const double* gain_db, size_t input_count,
    double bass_db, double mid_db, double treble_db,
    double master_gain_db, double pan, double left_gain, double right_gain,
    void*) {
    if (!output_path || !input_paths || !start_seconds || !gain_db ||
        !input_count || input_count > 128 || !sample_rate) return -1;

    sleela::audio::Config c;
    c.sample_rate = sample_rate;
    c.output_path = output_path;
    c.controls.bass_db = bass_db;
    c.controls.mid_db = mid_db;
    c.controls.treble_db = treble_db;
    c.controls.master_gain_db = master_gain_db;
    c.controls.pan = pan;
    c.controls.left_gain = left_gain;
    c.controls.right_gain = right_gain;
    c.inputs.reserve(input_count);

    for (size_t i = 0; i < input_count; ++i) {
        if (!input_paths[i]) return -1;
        sleela::audio::Input in;
        in.id = "slvm-" + std::to_string(i);
        in.path = input_paths[i];
        in.start_seconds = start_seconds[i];
        in.gain_db = gain_db[i];
        c.inputs.push_back(std::move(in));
    }

    std::string error;
    return sleela::audio::mix_wav(c, error) ? 0 : -1;
}
