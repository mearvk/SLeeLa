#include "sleela_audio.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

using sleela::audio::Config;

static void usage() {
    std::cout << "sleela-audio-native --output OUTPUT --sample-rate RATE"
                 " --input PATH START_SECONDS GAIN_DB ...\n";
}

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--help") { usage(); return 0; }
    Config c;
    for (int i = 1; i < argc;) {
        const std::string a = argv[i++];
        if (a == "--output" && i < argc) c.output_path = argv[i++];
        else if (a == "--sample-rate" && i < argc) c.sample_rate = static_cast<std::uint32_t>(std::stoul(argv[i++]));
        else if (a == "--input" && i + 2 < argc) {
            sleela::audio::Input in;
            in.path = argv[i++];
            in.start_seconds = std::stod(argv[i++]);
            in.gain_db = std::stod(argv[i++]);
            c.inputs.push_back(std::move(in));
        } else if (a == "--master-gain" && i < argc) c.controls.master_gain_db = std::stod(argv[i++]);
        else if (a == "--pan" && i < argc) c.controls.pan = std::stod(argv[i++]);
        else if (a == "--left-gain" && i < argc) c.controls.left_gain = std::stod(argv[i++]);
        else if (a == "--right-gain" && i < argc) c.controls.right_gain = std::stod(argv[i++]);
        else if (a == "--bass" && i < argc) c.controls.bass_db = std::stod(argv[i++]);
        else if (a == "--mid" && i < argc) c.controls.mid_db = std::stod(argv[i++]);
        else if (a == "--treble" && i < argc) c.controls.treble_db = std::stod(argv[i++]);
        else { std::cerr << "unknown or incomplete option: " << a << "\n"; return 2; }
    }
    std::string error;
    if (!sleela::audio::mix_wav(c, error)) { std::cerr << error << "\n"; return 1; }
    return 0;
}
