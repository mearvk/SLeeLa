#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::audio {
struct Input { std::string id,path; double start_seconds{},gain_db{}; };
struct Controls { double bass_db{},mid_db{},treble_db{},master_gain_db{},pan{},left_gain{1},right_gain{1}; };
struct Config { std::uint32_t sample_rate{}; std::vector<Input> inputs; Controls controls; std::string output_path; };
bool validate(const Config&,std::string&error);
bool mix_wav(const Config&,std::string&error);
}
