// Phraign(TM) City 3D -- model, config, generation, and serialization.

#include "city_model.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <sstream>

namespace sleela::city {

// ---------------------------------------------------------------------------
// Theme / palette
// ---------------------------------------------------------------------------

const char* themeName(Theme theme) noexcept {
    switch (theme) {
        case Theme::Green: return "green";
        case Theme::White: return "white";
        case Theme::Blue:  return "blue";
    }
    return "green";
}

bool parseTheme(const std::string& text, Theme& out) noexcept {
    std::string t;
    t.reserve(text.size());
    for (char c : text) t.push_back(static_cast<char>(std::tolower(
        static_cast<unsigned char>(c))));
    if (t == "green") { out = Theme::Green; return true; }
    if (t == "white") { out = Theme::White; return true; }
    if (t == "blue")  { out = Theme::Blue;  return true; }
    return false;
}

Palette Palette::forTheme(Theme theme) noexcept {
    Palette p;
    switch (theme) {
        case Theme::Green:  // default
            p.background = Color{6, 20, 10};
            p.ground     = Color{18, 54, 28};
            p.roof       = Color{92, 200, 120};
            p.wall_lit   = Color{56, 150, 84};
            p.wall_shade = Color{30, 96, 52};
            break;
        case Theme::White:
            p.background = Color{28, 30, 34};
            p.ground     = Color{120, 122, 128};
            p.roof       = Color{244, 246, 250};
            p.wall_lit   = Color{206, 210, 218};
            p.wall_shade = Color{150, 154, 162};
            break;
        case Theme::Blue:
            p.background = Color{6, 12, 28};
            p.ground     = Color{20, 34, 66};
            p.roof       = Color{120, 176, 246};
            p.wall_lit   = Color{74, 128, 208};
            p.wall_shade = Color{40, 80, 148};
            break;
    }
    return p;
}

// ---------------------------------------------------------------------------
// Config parsing (simple `key = value`, `#` comments)
// ---------------------------------------------------------------------------

namespace {

std::string trim(const std::string& s) {
    std::size_t a = 0, b = s.size();
    while (a < b && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

bool toU64(const std::string& s, std::uint64_t& out) {
    if (s.empty()) return false;
    std::uint64_t v = 0;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
        v = v * 10 + static_cast<std::uint64_t>(c - '0');
    }
    out = v;
    return true;
}

bool toDouble(const std::string& s, double& out) {
    try {
        std::size_t idx = 0;
        double v = std::stod(s, &idx);
        if (idx != s.size()) return false;
        out = v;
        return true;
    } catch (...) {
        return false;
    }
}

} // namespace

Config Config::fromText(const std::string& text, std::string* error) {
    Config cfg;
    std::istringstream in(text);
    std::string line;
    std::string errors;

    auto note = [&](const std::string& msg) {
        if (!errors.empty()) errors += "; ";
        errors += msg;
    };

    while (std::getline(in, line)) {
        // Strip comments.
        const auto hash = line.find('#');
        if (hash != std::string::npos) line = line.substr(0, hash);
        line = trim(line);
        if (line.empty()) continue;

        const auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        const std::string key = trim(line.substr(0, eq));
        const std::string val = trim(line.substr(eq + 1));

        std::uint64_t u = 0;
        double d = 0.0;

        if (key == "grid_cols") {
            if (toU64(val, u) && u > 0) cfg.grid_cols = static_cast<std::uint32_t>(u);
            else note("grid_cols invalid");
        } else if (key == "grid_rows") {
            if (toU64(val, u) && u > 0) cfg.grid_rows = static_cast<std::uint32_t>(u);
            else note("grid_rows invalid");
        } else if (key == "seed") {
            if (toU64(val, u)) cfg.seed = u; else note("seed invalid");
        } else if (key == "user") {
            cfg.user = val;
        } else if (key == "theme") {
            Theme t;
            if (parseTheme(val, t)) cfg.theme = t;
            else note("theme must be green|white|blue");
        } else if (key == "frame_width") {
            if (toU64(val, u) && u > 0) cfg.frame_width = static_cast<std::size_t>(u);
            else note("frame_width invalid");
        } else if (key == "frame_height") {
            if (toU64(val, u) && u > 0) cfg.frame_height = static_cast<std::size_t>(u);
            else note("frame_height invalid");
        } else if (key == "tilt_x") {
            if (toDouble(val, d)) cfg.viewpoint.tilt_x = d; else note("tilt_x invalid");
        } else if (key == "tilt_y") {
            if (toDouble(val, d)) cfg.viewpoint.tilt_y = d; else note("tilt_y invalid");
        } else if (key == "scale_x") {
            if (toDouble(val, d)) cfg.viewpoint.scale_x = d; else note("scale_x invalid");
        } else if (key == "scale_height") {
            if (toDouble(val, d)) cfg.viewpoint.scale_height = d;
            else note("scale_height invalid");
        } else if (key == "block_pitch") {
            if (toDouble(val, d)) cfg.viewpoint.block_pitch = d;
            else note("block_pitch invalid");
        } else if (key == "origin_x") {
            if (toDouble(val, d)) cfg.viewpoint.origin_x = d; else note("origin_x invalid");
        } else if (key == "origin_y") {
            if (toDouble(val, d)) cfg.viewpoint.origin_y = d; else note("origin_y invalid");
        } else if (key == "save_target") {
            cfg.save_target = val;
        } else if (key == "save_location") {
            cfg.save_location = val;
        }
        // Unknown keys are ignored by design (additive/forward compatible).
    }

    if (error) *error = errors;
    return cfg;
}

RenderOptions Config::renderOptions() const noexcept {
    RenderOptions ro;
    ro.theme = theme;
    ro.viewpoint = viewpoint;
    ro.frame_width = frame_width;
    ro.frame_height = frame_height;
    return ro;
}

// ---------------------------------------------------------------------------
// City model
// ---------------------------------------------------------------------------

City::City(std::uint32_t cols, std::uint32_t rows)
    : cols_(cols), rows_(rows),
      blocks_(static_cast<std::size_t>(cols) * rows) {}

const Block& City::at(std::uint32_t x, std::uint32_t y) const {
    return blocks_[static_cast<std::size_t>(y) * cols_ + x];
}

Block& City::at(std::uint32_t x, std::uint32_t y) {
    return blocks_[static_cast<std::size_t>(y) * cols_ + x];
}

namespace {

// A small, dependency-free deterministic PRNG (SplitMix64). Reproducible from a
// seed on any platform, which keeps a user's generated city stable.
struct SplitMix64 {
    std::uint64_t state;
    explicit SplitMix64(std::uint64_t s) : state(s) {}
    std::uint64_t next() {
        std::uint64_t z = (state += 0x9E3779B97F4A7C15ULL);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }
    // Uniform double in [0,1).
    double unit() {
        return static_cast<double>(next() >> 11) * (1.0 / 9007199254740992.0);
    }
};

} // namespace

void City::generate(std::uint64_t seed) {
    SplitMix64 rng(seed ? seed : 0xC17C5EEDULL);

    const double cx = (cols_ - 1) / 2.0;
    const double cy = (rows_ - 1) / 2.0;
    const double maxr = std::max(1.0, std::hypot(cx, cy));

    for (std::uint32_t y = 0; y < rows_; ++y) {
        for (std::uint32_t x = 0; x < cols_; ++x) {
            // Streets: every 8th row/column is a low corridor (empty-ish lot).
            const bool street = (x % 8 == 0) || (y % 8 == 0);

            // Downtown falloff: taller near the center, shorter at the edges.
            const double dist = std::hypot(x - cx, y - cy) / maxr;  // 0..~1
            const double central = 1.0 - dist;                      // ~1 center

            double h = 0.0;
            if (!street) {
                const double base = 2.0 + central * central * 26.0;  // 2..~28
                const double jitter = rng.unit() * 8.0 - 2.0;        // -2..+6
                h = base + jitter;
                // Occasional landmark tower.
                if (rng.unit() < 0.01) h += 12.0 + rng.unit() * 16.0;
            } else {
                // Streets sometimes carry a small structure.
                if (rng.unit() < 0.05) h = 1.0 + rng.unit() * 2.0;
            }
            if (h < 0.0) h = 0.0;
            if (h > 65000.0) h = 65000.0;
            at(x, y).height = static_cast<std::uint16_t>(h + 0.5);
        }
    }
}

// ---------------------------------------------------------------------------
// Serialization
//
// MODEL_FORMAT: a small, diff-friendly, GitHub/public-server-friendly text
// format. Line-oriented UTF-8/ASCII:
//
//   PHRAIGN-CITY 1
//   user <name>
//   seed <u64>
//   grid <cols> <rows>
//   row <y> <h0> <h1> ... <h(cols-1)>
//   ...
//   END
//
// Heights are decimal. A reader ignores unknown leading tokens for forward
// compatibility.
// ---------------------------------------------------------------------------

std::string City::serialize(const std::string& user, std::uint64_t seed) const {
    std::ostringstream out;
    out << "PHRAIGN-CITY 1\n";
    out << "user " << user << "\n";
    out << "seed " << seed << "\n";
    out << "grid " << cols_ << " " << rows_ << "\n";
    for (std::uint32_t y = 0; y < rows_; ++y) {
        out << "row " << y;
        for (std::uint32_t x = 0; x < cols_; ++x) {
            out << " " << at(x, y).height;
        }
        out << "\n";
    }
    out << "END\n";
    return out.str();
}

bool City::deserialize(const std::string& text, City& out,
                       std::string* user, std::uint64_t* seed) {
    std::istringstream in(text);
    std::string tok;
    if (!(in >> tok) || tok != "PHRAIGN-CITY") return false;
    int version = 0;
    if (!(in >> version) || version < 1) return false;

    std::uint32_t cols = 0, rows = 0;
    std::string localUser;
    std::uint64_t localSeed = 0;
    bool haveGrid = false;

    std::string line;
    std::getline(in, line);  // consume rest of header line
    while (std::getline(in, line)) {
        std::istringstream ls(line);
        std::string key;
        if (!(ls >> key)) continue;
        if (key == "user") {
            std::getline(ls, localUser);
            if (!localUser.empty() && localUser.front() == ' ')
                localUser.erase(0, 1);
        } else if (key == "seed") {
            ls >> localSeed;
        } else if (key == "grid") {
            if (!(ls >> cols >> rows) || cols == 0 || rows == 0) return false;
            out = City(cols, rows);
            haveGrid = true;
        } else if (key == "row") {
            if (!haveGrid) return false;
            std::uint32_t y = 0;
            if (!(ls >> y) || y >= rows) return false;
            for (std::uint32_t x = 0; x < cols; ++x) {
                unsigned h = 0;
                if (!(ls >> h)) return false;
                if (h > 65535u) h = 65535u;
                out.at(x, y).height = static_cast<std::uint16_t>(h);
            }
        } else if (key == "END") {
            break;
        }
        // Unknown keys ignored.
    }

    if (!haveGrid) return false;
    if (user) *user = localUser;
    if (seed) *seed = localSeed;
    return true;
}

// ---------------------------------------------------------------------------
// Per-user seed (FNV-1a 64-bit over the user name; stable across platforms)
// ---------------------------------------------------------------------------

std::uint64_t seedForUser(const std::string& user) noexcept {
    std::uint64_t h = 0xCBF29CE484222325ULL;
    for (unsigned char c : user) {
        h ^= c;
        h *= 0x100000001B3ULL;
    }
    // Avoid 0 (reserved as "derive from user / default").
    return h ? h : 0x1ULL;
}

} // namespace sleela::city
