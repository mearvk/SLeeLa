// SleelaTerminal(TM) -- SLeeLa's terminal product, built on GNU Bash
// (GPL, (C) Free Software Foundation). SLeeLa-authored addition; the
// vendored GNU Bash sources keep their own GPL headers and copyright.
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
// Year -> modernity
// ---------------------------------------------------------------------------

double CityParams::modernity() const noexcept {
    const double span = year_span > 0.0 ? year_span : 1.0;
    const double m = (static_cast<double>(year) -
                      static_cast<double>(year_baseline)) / span;
    return m < 0.0 ? 0.0 : (m > 1.0 ? 1.0 : m);
}

double CityParams::designQuality() const noexcept {
    const double span = iq_span > 0.0 ? iq_span : 1.0;
    const double q = (static_cast<double>(iq) -
                      static_cast<double>(iq_baseline)) / span;
    return q < 0.0 ? 0.0 : (q > 1.0 ? 1.0 : q);
}

LegislatureProfile CityParams::profile() const noexcept {
    return LegislatureProfile::forLegislature(legislature);
}

// ---------------------------------------------------------------------------
// Legislature (regime -> lines / linear outcomes)
// ---------------------------------------------------------------------------

const char* legislatureName(Legislature l) noexcept {
    switch (l) {
        case Legislature::Federal:       return "federal";
        case Legislature::Parliamentary: return "parliamentary";
        case Legislature::Municipal:     return "municipal";
        case Legislature::Bicameral:     return "bicameral";
        case Legislature::Unicameral:    return "unicameral";
        case Legislature::Direct:        return "direct";
    }
    return "federal";
}

bool parseLegislature(const std::string& text, Legislature& out) noexcept {
    std::string t;
    t.reserve(text.size());
    for (char c : text) t.push_back(static_cast<char>(std::tolower(
        static_cast<unsigned char>(c))));
    if (t == "federal")       { out = Legislature::Federal;       return true; }
    if (t == "parliamentary") { out = Legislature::Parliamentary; return true; }
    if (t == "municipal")     { out = Legislature::Municipal;     return true; }
    if (t == "bicameral")     { out = Legislature::Bicameral;     return true; }
    if (t == "unicameral")    { out = Legislature::Unicameral;    return true; }
    if (t == "direct")        { out = Legislature::Direct;        return true; }
    return false;
}

LegislatureProfile LegislatureProfile::forLegislature(Legislature l) noexcept {
    LegislatureProfile p;
    switch (l) {
        case Legislature::Federal:  // strong balanced grid
            p.regularity = 0.85; p.linearity = 0.75; p.axis_bias = 0.50;
            p.connectivity = 0.80; p.road_spacing_delta = 0; p.bridge_count_delta = 0;
            break;
        case Legislature::Parliamentary:  // orderly, dominant primary axis
            p.regularity = 0.80; p.linearity = 0.80; p.axis_bias = 0.30;
            p.connectivity = 0.70; p.road_spacing_delta = 0; p.bridge_count_delta = 1;
            break;
        case Legislature::Municipal:  // dense, very regular local blocks
            p.regularity = 0.95; p.linearity = 0.70; p.axis_bias = 0.50;
            p.connectivity = 0.85; p.road_spacing_delta = -2; p.bridge_count_delta = 2;
            break;
        case Legislature::Bicameral:  // two strong crossing axes
            p.regularity = 0.82; p.linearity = 0.90; p.axis_bias = 0.50;
            p.connectivity = 0.75; p.road_spacing_delta = 0; p.bridge_count_delta = 2;
            break;
        case Legislature::Unicameral:  // one dominant axis -> banded, linear city
            p.regularity = 0.78; p.linearity = 0.95; p.axis_bias = 0.15;
            p.connectivity = 0.65; p.road_spacing_delta = 0; p.bridge_count_delta = 0;
            break;
        case Legislature::Direct:  // looser, more organic lines
            p.regularity = 0.55; p.linearity = 0.45; p.axis_bias = 0.50;
            p.connectivity = 0.55; p.road_spacing_delta = 1; p.bridge_count_delta = -1;
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

bool toBool(const std::string& s, bool& out) {
    std::string t;
    for (char c : s) t.push_back(static_cast<char>(std::tolower(
        static_cast<unsigned char>(c))));
    if (t == "1" || t == "true" || t == "yes" || t == "on") { out = true; return true; }
    if (t == "0" || t == "false" || t == "no" || t == "off") { out = false; return true; }
    return false;
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

        // ---- Year / modernity ----
        } else if (key == "year") {
            if (toU64(val, u) && u > 0) cfg.params.year = static_cast<std::uint32_t>(u);
            else note("year invalid");
        } else if (key == "year_baseline") {
            if (toU64(val, u)) cfg.params.year_baseline = static_cast<std::uint32_t>(u);
            else note("year_baseline invalid");
        } else if (key == "year_span") {
            if (toDouble(val, d) && d > 0.0) cfg.params.year_span = d;
            else note("year_span invalid");

        // ---- IQ (person + system design quality) ----
        } else if (key == "iq") {
            if (toU64(val, u)) cfg.params.iq = static_cast<std::uint32_t>(u);
            else note("iq invalid");
        } else if (key == "iq_baseline") {
            if (toU64(val, u)) cfg.params.iq_baseline = static_cast<std::uint32_t>(u);
            else note("iq_baseline invalid");
        } else if (key == "iq_span") {
            if (toDouble(val, d) && d > 0.0) cfg.params.iq_span = d;
            else note("iq_span invalid");

        // ---- Native legislature (regime -> lines / linear outcomes) ----
        } else if (key == "legislature") {
            Legislature lg;
            if (parseLegislature(val, lg)) cfg.params.legislature = lg;
            else note("legislature must be federal|parliamentary|municipal|"
                      "bicameral|unicameral|direct");

        // ---- General cityscape-graph dimensions ----
        } else if (key == "radix") {
            if (toU64(val, u) && u >= 1) cfg.params.radix = static_cast<std::uint32_t>(u);
            else note("radix must be >= 1");
        } else if (key == "diameter") {
            if (toDouble(val, d) && d > 0.0) cfg.params.diameter = d;
            else note("diameter must be > 0");
        } else if (key == "randomness") {
            if (toDouble(val, d) && d >= 0.0 && d <= 1.0) cfg.params.randomness = d;
            else note("randomness must be in 0..1");
        } else if (key == "draw_cylinders") {
            bool bv; if (toBool(val, bv)) cfg.draw_cylinders = bv;
            else note("draw_cylinders invalid");

        // ---- Building form / quality targets ----
        } else if (key == "avg_floors") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.avg_floors = d;
            else note("avg_floors invalid");
        } else if (key == "floor_height") {
            if (toDouble(val, d) && d > 0.0) cfg.params.floor_height = d;
            else note("floor_height invalid");
        } else if (key == "windows_per_floor") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.windows_per_floor = d;
            else note("windows_per_floor invalid");

        // ---- Roads / bridges (real features) ----
        } else if (key == "road_spacing") {
            if (toU64(val, u) && u >= 2) cfg.params.road_spacing = static_cast<std::uint32_t>(u);
            else note("road_spacing must be >= 2");
        } else if (key == "bridge_count") {
            if (toU64(val, u)) cfg.params.bridge_count = static_cast<std::uint32_t>(u);
            else note("bridge_count invalid");

        // ---- Finality weights ----
        } else if (key == "w_year") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.w_year = d; else note("w_year invalid");
        } else if (key == "w_iq") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.w_iq = d; else note("w_iq invalid");
        } else if (key == "w_floors") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.w_floors = d; else note("w_floors invalid");
        } else if (key == "w_windows") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.w_windows = d; else note("w_windows invalid");
        } else if (key == "w_road_proximity") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.w_road_proximity = d;
            else note("w_road_proximity invalid");
        } else if (key == "w_bridge_proximity") {
            if (toDouble(val, d) && d >= 0.0) cfg.params.w_bridge_proximity = d;
            else note("w_bridge_proximity invalid");
        } else if (key == "proximity_falloff") {
            if (toDouble(val, d) && d > 0.0) cfg.params.proximity_falloff = d;
            else note("proximity_falloff invalid");

        // ---- Render detailing toggles ----
        } else if (key == "draw_bridges") {
            bool bv; if (toBool(val, bv)) cfg.draw_bridges = bv; else note("draw_bridges invalid");
        } else if (key == "draw_windows") {
            bool bv; if (toBool(val, bv)) cfg.draw_windows = bv; else note("draw_windows invalid");

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
    ro.draw_bridges = draw_bridges;
    ro.draw_windows = draw_windows;
    ro.draw_cylinders = draw_cylinders;
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

void City::generate(std::uint64_t seed, const CityParams& params) {
    params_ = params;
    SplitMix64 rng(seed ? seed : 0xC17C5EEDULL);

    const double cx = (cols_ - 1) / 2.0;
    const double cy = (rows_ - 1) / 2.0;
    const double maxr = std::max(1.0, std::hypot(cx, cy));
    const double modern = params_.modernity();      // 0 (old) .. 1 (new/modern)
    const double quality = params_.designQuality(); // 0 .. 1 from IQ
    const LegislatureProfile reg = params_.profile();

    // The legislature and IQ set the city's lines. Road spacing tightens with a
    // regime's spacing delta; axis_bias decides which axis carries more lines
    // (linear/banded regimes favor one axis). Higher IQ + regularity make the
    // grid more orderly (less jitter).
    long spacingL = static_cast<long>(params_.road_spacing) + reg.road_spacing_delta;
    const std::uint32_t spacing = static_cast<std::uint32_t>(
        std::max<long>(2, spacingL));
    // With a strong axis bias, the minor axis's roads are spaced further apart,
    // producing the banded/linear look. axis_bias 0.5 => both axes equal.
    const double bias = reg.axis_bias;                 // 0..1
    const std::uint32_t colSpacing = std::max<std::uint32_t>(2,
        static_cast<std::uint32_t>(spacing * (1.0 + (0.5 - bias) * 2.0 * 0.8 * reg.linearity)));
    const std::uint32_t rowSpacing = std::max<std::uint32_t>(2,
        static_cast<std::uint32_t>(spacing * (1.0 + (bias - 0.5) * 2.0 * 0.8 * reg.linearity)));

    // Order: high IQ and high regularity reduce random jitter, giving cleaner
    // lines and heights. order in [0,1]; jitterScale shrinks as order rises.
    const double order = std::min(1.0, 0.5 * quality + 0.5 * reg.regularity);
    const double jitterScale = 1.0 - 0.8 * order;      // 1 (chaotic) .. 0.2 (crisp)

    // --- Pass 1: lay roads and place buildings -----------------------------
    for (std::uint32_t y = 0; y < rows_; ++y) {
        for (std::uint32_t x = 0; x < cols_; ++x) {
            Block& b = at(x, y);
            b = Block{};  // reset

            // Roads run as straight lines on each axis; spacing per axis creates
            // the legislature's linear outcome. A low-linearity (organic) regime
            // occasionally drops a road cell so lines are less rigid.
            bool road = (x % colSpacing == 0) || (y % rowSpacing == 0);
            if (road && reg.linearity < 0.6 && rng.unit() > 0.5 + reg.linearity) {
                road = false;  // break the line -> more organic
            }
            if (road) {
                b.cell = Cell::Road;
                continue;
            }

            b.cell = Cell::Building;

            // Downtown falloff: taller near the center, shorter at the edges.
            const double dist = std::hypot(x - cx, y - cy) / maxr;  // 0..~1
            const double central = 1.0 - dist;                      // ~1 center

            // Floors scale with the target, modernity, and centrality. Higher
            // design quality lifts the baseline (better-planned density).
            const double floorBase =
                params_.avg_floors * (0.4 + 0.9 * central * central) *
                (0.7 + 0.6 * modern) * (0.85 + 0.3 * quality);
            const double floorJitter =
                (rng.unit() * (4.0 + 8.0 * modern) - 2.0) * jitterScale;
            double floors = floorBase + floorJitter;
            // Occasional landmark tower (more common / taller in a modern city).
            if (rng.unit() < 0.008 + 0.02 * modern) {
                floors += 8.0 + rng.unit() * (12.0 + 30.0 * modern);
            }
            if (floors < 1.0) floors = 1.0;
            if (floors > 60000.0) floors = 60000.0;

            const double height = floors * params_.floor_height;

            // Windows: per-floor target, richer (more glass) in a modern city.
            const double wpf = params_.windows_per_floor * (0.6 + 0.8 * modern);
            double windows = wpf * floors;
            if (windows < 0.0) windows = 0.0;
            if (windows > 65000.0) windows = 65000.0;

            b.floors = static_cast<std::uint16_t>(floors + 0.5);
            b.height = static_cast<std::uint16_t>(
                std::min(height + 0.5, 65000.0));
            b.windows = static_cast<std::uint16_t>(windows + 0.5);
        }
    }

    // --- Pass 2: place bridges (real features) -----------------------------
    // A bridge is a straight span laid along a road corridor. Connectivity
    // (from IQ + the regime) adds bridges; the bridge_count delta adjusts too.
    long bridgeL = static_cast<long>(params_.bridge_count) + reg.bridge_count_delta;
    bridgeL += static_cast<long>(std::lround(reg.connectivity * 4.0 * (0.5 + quality)));
    const std::uint32_t bridges = static_cast<std::uint32_t>(std::max<long>(0, bridgeL));
    const std::uint32_t nColLines = std::max<std::uint32_t>(1, cols_ / colSpacing);
    const std::uint32_t nRowLines = std::max<std::uint32_t>(1, rows_ / rowSpacing);
    for (std::uint32_t i = 0; i < bridges && cols_ > 4 && rows_ > 4; ++i) {
        // Bias bridge orientation toward the regime's dominant axis so the
        // linear outcome is reinforced. bias<0.5 favors horizontal lines.
        const double roll = rng.unit();
        const bool horizontal = roll > bias;
        if (horizontal) {
            std::uint32_t line = (static_cast<std::uint32_t>(rng.next() % nRowLines)) * rowSpacing;
            if (line >= rows_) line = (rows_ - 1) / rowSpacing * rowSpacing;
            const std::uint32_t len = 4 + static_cast<std::uint32_t>(rng.next() % (cols_ / 2));
            std::uint32_t start = static_cast<std::uint32_t>(rng.next() % cols_);
            for (std::uint32_t k = 0; k < len && start + k < cols_; ++k) {
                Block& b = at(start + k, line);
                if (b.cell == Cell::Road) b.cell = Cell::Bridge;
            }
        } else {
            std::uint32_t line = (static_cast<std::uint32_t>(rng.next() % nColLines)) * colSpacing;
            if (line >= cols_) line = (cols_ - 1) / colSpacing * colSpacing;
            const std::uint32_t len = 4 + static_cast<std::uint32_t>(rng.next() % (rows_ / 2));
            std::uint32_t start = static_cast<std::uint32_t>(rng.next() % rows_);
            for (std::uint32_t k = 0; k < len && start + k < rows_; ++k) {
                Block& b = at(line, start + k);
                if (b.cell == Cell::Road) b.cell = Cell::Bridge;
            }
        }
    }

    // --- Pass 3: real proximities, then finality ---------------------------
    computeProximities();
    computeFinality();
}

// Multi-source BFS over the grid (4-connected) computes, for every cell, the
// Chebyshev-ish block distance to the nearest road cell and to the nearest
// bridge cell. This is a real, computed proximity -- not a config weight.
void City::computeProximities() {
    const std::size_t n = blocks_.size();
    if (n == 0) return;

    auto bfs = [&](bool wantBridge) {
        std::vector<float> dist(n, -1.0f);
        std::vector<std::size_t> q;
        q.reserve(n);
        for (std::uint32_t y = 0; y < rows_; ++y) {
            for (std::uint32_t x = 0; x < cols_; ++x) {
                const Block& b = at(x, y);
                const bool src = wantBridge ? (b.cell == Cell::Bridge)
                                            : (b.cell == Cell::Road ||
                                               b.cell == Cell::Bridge);
                if (src) {
                    const std::size_t idx = static_cast<std::size_t>(y) * cols_ + x;
                    dist[idx] = 0.0f;
                    q.push_back(idx);
                }
            }
        }
        // If there are no sources, leave distances at a large sentinel.
        if (q.empty()) {
            std::fill(dist.begin(), dist.end(),
                      static_cast<float>(cols_ + rows_));
            return dist;
        }
        std::size_t head = 0;
        while (head < q.size()) {
            const std::size_t idx = q[head++];
            const std::uint32_t x = static_cast<std::uint32_t>(idx % cols_);
            const std::uint32_t y = static_cast<std::uint32_t>(idx / cols_);
            const float d = dist[idx] + 1.0f;
            const int dx[4] = {1, -1, 0, 0};
            const int dy[4] = {0, 0, 1, -1};
            for (int k = 0; k < 4; ++k) {
                const long nx = static_cast<long>(x) + dx[k];
                const long ny = static_cast<long>(y) + dy[k];
                if (nx < 0 || ny < 0 || nx >= static_cast<long>(cols_) ||
                    ny >= static_cast<long>(rows_)) continue;
                const std::size_t nidx =
                    static_cast<std::size_t>(ny) * cols_ + static_cast<std::size_t>(nx);
                if (dist[nidx] < 0.0f) {
                    dist[nidx] = d;
                    q.push_back(nidx);
                }
            }
        }
        return dist;
    };

    const std::vector<float> roadDist = bfs(/*wantBridge=*/false);
    const std::vector<float> bridgeDist = bfs(/*wantBridge=*/true);
    for (std::size_t i = 0; i < n; ++i) {
        blocks_[i].road_distance = roadDist[i];
        blocks_[i].bridge_distance = bridgeDist[i];
    }
}

// Finality: a per-building quality-of-condition score in [0,1], a weighted mix
// of modernity (Year), floors, windows, and computed proximity to roads and
// bridges. Weights come from the params and are normalized so only relative
// sizes matter.
void City::computeFinality() {
    const CityParams& p = params_;
    const double modern = p.modernity();
    const double quality = p.designQuality();  // IQ -> design quality (0..1)
    const double falloff = p.proximity_falloff > 0.0 ? p.proximity_falloff : 1.0;

    // Normalization references for floors/windows (avoid divide-by-zero).
    const double floorRef = std::max(1.0, p.avg_floors * 2.0);
    const double windowRef = std::max(1.0, p.windows_per_floor * p.avg_floors * 2.0);

    const double wSum = p.w_year + p.w_iq + p.w_floors + p.w_windows +
                        p.w_road_proximity + p.w_bridge_proximity;
    const double inv = wSum > 0.0 ? 1.0 / wSum : 0.0;

    for (Block& b : blocks_) {
        if (b.cell != Cell::Building) { b.finality = 0.0f; continue; }

        const double fFloors = std::min(1.0, b.floors / floorRef);
        const double fWindows = std::min(1.0, b.windows / windowRef);
        // Proximity: 1 when adjacent, fading to 0 at the falloff distance.
        const double fRoad = std::max(0.0, 1.0 - b.road_distance / falloff);
        const double fBridge = std::max(0.0, 1.0 - b.bridge_distance / falloff);

        const double score =
            (p.w_year * modern +
             p.w_iq * quality +
             p.w_floors * fFloors +
             p.w_windows * fWindows +
             p.w_road_proximity * fRoad +
             p.w_bridge_proximity * fBridge) * inv;

        b.finality = static_cast<float>(score < 0.0 ? 0.0 : (score > 1.0 ? 1.0 : score));
    }
}

double City::cityFinality() const noexcept {
    double sum = 0.0;
    std::size_t count = 0;
    for (const Block& b : blocks_) {
        if (b.cell == Cell::Building && b.height > 0) {
            sum += b.finality;
            ++count;
        }
    }
    return count ? sum / static_cast<double>(count) : 0.0;
}

// ---------------------------------------------------------------------------
// Serialization
//
// MODEL_FORMAT: a small, diff-friendly, GitHub/public-server-friendly text
// format. Line-oriented ASCII.
//
// v4 (current):
//   PHRAIGN-CITY 4
//   user <name>
//   seed <u64>
//   year <u32>
//   iq <u32>
//   legislature <name>            (federal|parliamentary|municipal|
//                                   bicameral|unicameral|direct)
//   radix <u32>                   (cityscape-graph branching base)
//   diameter <double>             (radial reach from centricity)
//   randomness <double 0..1>      (seeded graph/driver variability)
//   finality <city-finality 0..1>
//   grid <cols> <rows>
//
// The cityscape graph (nodes / mating cylinder pairs / spheres) is regenerated
// deterministically from (seed, year, iq, legislature, radix, diameter,
// randomness), so it is not dumped node-by-node.
//   cell <y> <c0> <c1> ...        (0=building 1=road 2=bridge, per column)
//   row  <y> <h0> <h1> ...        (heights)
//   floors  <y> <f0> <f1> ...
//   windows <y> <w0> <w1> ...
//   fin <y> <q0> <q1> ...         (per-building finality, 0..1000 integer)
//   END
//
// v2/v1 (legacy, still readable): v2 omits iq/legislature; v1 is height-only.
//
// A reader ignores unknown leading tokens for forward compatibility.
// ---------------------------------------------------------------------------

std::string City::serialize(const std::string& user, std::uint64_t seed) const {
    std::ostringstream out;
    out << "PHRAIGN-CITY 4\n";
    out << "user " << user << "\n";
    out << "seed " << seed << "\n";
    out << "year " << params_.year << "\n";
    out << "iq " << params_.iq << "\n";
    out << "legislature " << legislatureName(params_.legislature) << "\n";
    out << "radix " << params_.radix << "\n";
    out << "diameter " << params_.diameter << "\n";
    out << "randomness " << params_.randomness << "\n";
    out << "finality " << cityFinality() << "\n";
    out << "grid " << cols_ << " " << rows_ << "\n";
    for (std::uint32_t y = 0; y < rows_; ++y) {
        out << "cell " << y;
        for (std::uint32_t x = 0; x < cols_; ++x)
            out << " " << static_cast<unsigned>(at(x, y).cell);
        out << "\n";
        out << "row " << y;
        for (std::uint32_t x = 0; x < cols_; ++x) out << " " << at(x, y).height;
        out << "\n";
        out << "floors " << y;
        for (std::uint32_t x = 0; x < cols_; ++x) out << " " << at(x, y).floors;
        out << "\n";
        out << "windows " << y;
        for (std::uint32_t x = 0; x < cols_; ++x) out << " " << at(x, y).windows;
        out << "\n";
        out << "fin " << y;
        for (std::uint32_t x = 0; x < cols_; ++x) {
            const int q = static_cast<int>(at(x, y).finality * 1000.0f + 0.5f);
            out << " " << (q < 0 ? 0 : (q > 1000 ? 1000 : q));
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
    std::uint32_t year = 0;
    std::uint32_t iq = 0;
    bool haveIq = false;
    Legislature legislature = Legislature::Federal;
    bool haveLeg = false;
    std::uint32_t radix = 0;
    bool haveRadix = false;
    double diameter = 0.0;
    bool haveDiameter = false;
    double randomness = -1.0;
    bool haveRandomness = false;
    bool haveGrid = false;

    auto clampl = [](long v, long lo, long hi) -> long {
        return v < lo ? lo : (v > hi ? hi : v);
    };
    auto readRow = [&](std::istringstream& ls, auto setter) -> bool {
        if (!haveGrid) return false;
        std::uint32_t y = 0;
        if (!(ls >> y) || y >= rows) return false;
        for (std::uint32_t x = 0; x < cols; ++x) {
            long v = 0;
            if (!(ls >> v)) return false;
            setter(x, y, v);
        }
        return true;
    };

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
        } else if (key == "year") {
            ls >> year;
        } else if (key == "iq") {
            if (ls >> iq) haveIq = true;
        } else if (key == "legislature") {
            std::string name;
            if (ls >> name && parseLegislature(name, legislature)) haveLeg = true;
        } else if (key == "radix") {
            if (ls >> radix) haveRadix = true;
        } else if (key == "diameter") {
            if (ls >> diameter) haveDiameter = true;
        } else if (key == "randomness") {
            if (ls >> randomness) haveRandomness = true;
        } else if (key == "finality") {
            // Informational; recomputed on generation. Ignored on read.
        } else if (key == "grid") {
            if (!(ls >> cols >> rows) || cols == 0 || rows == 0) return false;
            out = City(cols, rows);
            haveGrid = true;
        } else if (key == "cell") {
            if (!readRow(ls, [&](std::uint32_t x, std::uint32_t y, long v) {
                    if (v < 0 || v > 2) v = 0;
                    out.at(x, y).cell = static_cast<Cell>(v);
                })) return false;
        } else if (key == "row") {
            if (!readRow(ls, [&](std::uint32_t x, std::uint32_t y, long v) {
                    out.at(x, y).height = static_cast<std::uint16_t>(clampl(v, 0, 65535));
                })) return false;
        } else if (key == "floors") {
            if (!readRow(ls, [&](std::uint32_t x, std::uint32_t y, long v) {
                    out.at(x, y).floors = static_cast<std::uint16_t>(clampl(v, 0, 65535));
                })) return false;
        } else if (key == "windows") {
            if (!readRow(ls, [&](std::uint32_t x, std::uint32_t y, long v) {
                    out.at(x, y).windows = static_cast<std::uint16_t>(clampl(v, 0, 65535));
                })) return false;
        } else if (key == "fin") {
            if (!readRow(ls, [&](std::uint32_t x, std::uint32_t y, long v) {
                    out.at(x, y).finality = static_cast<float>(clampl(v, 0, 1000)) / 1000.0f;
                })) return false;
        } else if (key == "END") {
            break;
        }
        // Unknown keys ignored (forward compatible).
    }

    if (!haveGrid) return false;
    if (year > 0) out.params_.year = year;
    if (haveIq) out.params_.iq = iq;
    if (haveLeg) out.params_.legislature = legislature;
    if (haveRadix && radix >= 1) out.params_.radix = radix;
    if (haveDiameter && diameter > 0.0) out.params_.diameter = diameter;
    if (haveRandomness && randomness >= 0.0 && randomness <= 1.0)
        out.params_.randomness = randomness;
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
