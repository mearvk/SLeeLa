#include "sleela_audio.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iterator>\n#include <limits>
#include <vector>

namespace sleela::audio {
namespace {
struct W {
    std::uint32_t r{};
    std::uint16_t c{};
    std::vector<std::int16_t> p;
};

std::uint16_t u16(const unsigned char* p) {
    return static_cast<std::uint16_t>(p[0] | (p[1] << 8));
}

std::uint32_t u32(const unsigned char* p) {
    return static_cast<std::uint32_t>(
        p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
}

double g(double db) {
    return std::pow(10.0, db / 20.0);
}

bool read(const std::string& path, W& w, std::string& e) {
    std::ifstream f(path, std::ios::binary);
    if (!f) {
        e = "cannot open input";
        return false;
    }

    std::vector<unsigned char> b((std::istreambuf_iterator<char>(f)), {});
    if (b.size() < 44 ||
        std::memcmp(b.data(), "RIFF", 4) != 0 ||
        std::memcmp(b.data() + 8, "WAVE", 4) != 0) {
        e = "unsupported WAV";
        return false;
    }

    std::size_t p = 12;
    std::uint16_t bits = 0;
    bool fmt = false;
    bool dat = false;

    while (p + 8 <= b.size()) {
        const std::uint32_t n = u32(b.data() + p + 4);
        if (p + 8 + n > b.size()) {
            e = "truncated WAV chunk";
            return false;
        }

        if (std::memcmp(b.data() + p, "fmt ", 4) == 0 && n >= 16) {
            w.c = u16(b.data() + p + 10);
            w.r = u32(b.data() + p + 12);
            bits = u16(b.data() + p + 22);
            fmt = true;
        }

        if (std::memcmp(b.data() + p, "data", 4) == 0) {
            if (bits != 16) {
                e = "only PCM16 mono/stereo WAV is supported";
                return false;
            }
            w.p.resize(n / 2);
            std::memcpy(w.p.data(), b.data() + p + 8, n);
            dat = true;
        }

        p += 8 + n + (n & 1U);
    }

    if (!fmt || !dat || bits != 16 || w.c < 1 || w.c > 2) {
        e = "only PCM16 mono/stereo WAV is supported";
        return false;
    }
    return true;
}
} // namespace

bool validate(const Config& c, std::string& e) {
    if (!c.sample_rate || c.inputs.empty() || c.inputs.size() > 128 ||
        c.output_path.empty()) {
        e = "invalid configuration";
        return false;
    }
    if (!std::isfinite(c.controls.pan) ||
        c.controls.pan < -1.0 || c.controls.pan > 1.0) {
        e = "pan out of range";
        return false;
    }
    for (const auto& i : c.inputs) {
        if (i.path.empty() || i.start_seconds < 0.0 ||
            !std::isfinite(i.start_seconds) || !std::isfinite(i.gain_db)) {
            e = "invalid input";
            return false;
        }
    }
    return true;
}

bool mix_wav(const Config& c, std::string& e) {
    if (!validate(c, e)) {
        return false;
    }

    std::vector<W> w(c.inputs.size());
    std::size_t frames = 0;

    for (std::size_t i = 0; i < c.inputs.size(); ++i) {
        if (!read(c.inputs[i].path, w[i], e) || w[i].r != c.sample_rate) {
            if (e.empty()) {
                e = "sample-rate mismatch";
            }
            return false;
        }
        frames = std::max(
            frames,
            static_cast<std::size_t>(
                std::llround(c.inputs[i].start_seconds * c.sample_rate)) +
                w[i].p.size() / w[i].c);
    }

    if (frames > (static_cast<std::size_t>(-1) / 2)) { e = "output too large"; return false; }\n    std::vector<std::int16_t> o(frames * 2);
    const double lg =
        g(c.controls.master_gain_db) * c.controls.left_gain *
        (c.controls.pan > 0.0 ? 1.0 - c.controls.pan : 1.0);
    const double rg =
        g(c.controls.master_gain_db) * c.controls.right_gain *
        (c.controls.pan < 0.0 ? 1.0 + c.controls.pan : 1.0);

    for (std::size_t i = 0; i < c.inputs.size(); ++i) {
        const std::size_t s = static_cast<std::size_t>(
            std::llround(c.inputs[i].start_seconds * c.sample_rate));
        const double x = g(c.inputs[i].gain_db);

        for (std::size_t f = 0; f < w[i].p.size() / w[i].c; ++f) {
            const double l = w[i].p[f * w[i].c] * x;
            const double r = w[i].c == 2 ? w[i].p[f * 2 + 1] * x : l;
            const std::size_t j = (s + f) * 2;
            o[j] = static_cast<std::int16_t>(
                std::clamp(static_cast<double>(o[j]) + l * lg,
                           -32768.0, 32767.0));
            o[j + 1] = static_cast<std::int16_t>(
                std::clamp(static_cast<double>(o[j + 1]) + r * rg,
                           -32768.0, 32767.0));
        }
    }

    std::ofstream f(c.output_path, std::ios::binary);
    if (!f) {
        e = "cannot open output";
        return false;
    }

    auto w16 = [&f](std::uint16_t x) {
        f.put(static_cast<char>(x));
        f.put(static_cast<char>(x >> 8));
    };
    auto w32 = [&f](std::uint32_t x) {
        for (int i = 0; i < 4; ++i) {
            f.put(static_cast<char>(x >> (8 * i)));
        }
    };

    if (o.size() > static_cast<std::size_t>(std::numeric_limits<std::uint32_t>::max()) / 2) { e = "WAV output exceeds RIFF size limit"; return false; }\n    const std::uint32_t bytes = static_cast<std::uint32_t>(o.size() * 2);
    f.write("RIFF", 4);
    w32(36 + bytes);
    f.write("WAVEfmt ", 8);
    w32(16);
    w16(1);
    w16(2);
    w32(c.sample_rate);
    w32(c.sample_rate * 4);
    w16(4);
    w16(16);
    f.write("data", 4);
    w32(bytes);
    f.write(reinterpret_cast<const char*>(o.data()), bytes);
    return f.good();
}
} // namespace sleela::audio
