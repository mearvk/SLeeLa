#pragma once

// SleelaTerminal(TM) -- SLeeLa's terminal product, built on GNU Bash
// (GPL, (C) Free Software Foundation). SLeeLa-authored addition; the
// vendored GNU Bash sources keep their own GPL headers and copyright.
// Phraign(TM) City 3D -- sprawling city model for the Phraign frame renderer.
//
// A city is a square grid of blocks (default 64x64 = 4096, "~4000 square
// blocks"). Each block carries a building with an integer height. The model is
// generated per-user from a seed so a given user reproduces the same city, and
// it can be serialized to a portable text format that lives well on GitHub or a
// public server.
//
// This header defines the data model, the render/config options, and the
// public API. Rendering targets the Phraign native pixel frame
// (sleela::terminal::PixelTerminal), so the city is drawn on a per-pixel basis.
//
// See README.md and CONFIG.md for the system overview and config format.

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace sleela::city {

// ---------------------------------------------------------------------------
// Color
// ---------------------------------------------------------------------------

// A Phraign pixel value is a 32-bit RGBA word, packed 0xRRGGBBAA. Alpha 0xFF is
// fully opaque. This is the encoding City 3D writes into Phraign Pixel::value.
struct Color {
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0xFF;

    constexpr std::uint32_t rgba() const noexcept {
        return (static_cast<std::uint32_t>(r) << 24) |
               (static_cast<std::uint32_t>(g) << 16) |
               (static_cast<std::uint32_t>(b) << 8) |
               static_cast<std::uint32_t>(a);
    }
};

// The user-selectable color theme. Green is the default.
enum class Theme {
    Green,
    White,
    Blue
};

const char* themeName(Theme theme) noexcept;
bool parseTheme(const std::string& text, Theme& out) noexcept;

// A resolved palette derived from a Theme. The renderer shades building faces
// by tinting the base tone lighter (top / lit side) or darker (shaded side).
struct Palette {
    Color background;  // empty sky/ground behind the city
    Color ground;      // the ground plane / streets
    Color roof;        // building top face
    Color wall_lit;    // building side facing the light (brighter)
    Color wall_shade;  // building side away from the light (darker)

    static Palette forTheme(Theme theme) noexcept;
};

// ---------------------------------------------------------------------------
// Viewpoint / render configuration
// ---------------------------------------------------------------------------

// The camera is an oblique/dimetric projection: the city is seen from the top
// and off to one side at a slight angle. The viewpoint is fully described by a
// handful of scalars so it can be driven from a config file.
struct Viewpoint {
    // Oblique projection factors. Each unit of world Y (north/depth) shifts the
    // screen position by (tilt_x, tilt_y) pixels; this produces the "seen from
    // slightly to the side and above" look. Defaults give a gentle angle.
    double tilt_x = 0.55;  // horizontal skew per world-Y unit
    double tilt_y = 0.42;  // vertical foreshortening per world-Y unit (0<..<=1)

    // Pixels per world unit along X (east/west) and per unit of building height.
    double scale_x = 9.0;
    double scale_height = 1.1;

    // Pixels between block centers on the ground grid.
    double block_pitch = 9.0;

    // Screen origin (pixels) where world (0,0) ground point lands.
    double origin_x = 150.0;
    double origin_y = 380.0;
};

// Everything needed to render, resolved from a Config.
struct RenderOptions {
    Theme theme = Theme::Green;
    Viewpoint viewpoint{};
    std::size_t frame_width = 1024;
    std::size_t frame_height = 768;
    bool draw_bridges = true;
    bool draw_windows = true;
    bool draw_cylinders = true;  // render the cityscape-graph cylinder pairs
};

// ---------------------------------------------------------------------------
// Native legislature (the civic regime that shapes the city's lines)
// ---------------------------------------------------------------------------

// A person's *native legislature* is the defining characteristic of their
// nature and, in turn, their design. In City 3D it selects a civic regime whose
// rules produce the city's lines and linear outcomes: how regular the grid is,
// how strongly corridors run in straight lines, and how they are oriented and
// connected. It is a named, enumerated regime (not a numeric knob).
enum class Legislature {
    Federal,        // strong straight grid, balanced two-axis layout
    Parliamentary,  // orderly grid with a dominant primary axis
    Municipal,      // dense, regular local blocks (very grid-like)
    Bicameral,      // two strong axes -> pronounced crossing lines
    Unicameral,     // one dominant axis -> strongly linear, banded city
    Direct          // looser, more organic lines (least regimented)
};

const char* legislatureName(Legislature l) noexcept;
bool parseLegislature(const std::string& text, Legislature& out) noexcept;

// The linear "shape" a legislature imposes, resolved from the enum. These are
// the lines/linear outcomes the regime produces.
struct LegislatureProfile {
    double regularity = 0.7;      // 0 organic .. 1 perfectly regular grid
    double linearity = 0.6;       // how strongly corridors run in straight lines
    double axis_bias = 0.5;       // 0 favor one axis .. 0.5 balanced .. 1 other
    double connectivity = 0.6;    // how well corridors interconnect (bridges/roads)
    std::int32_t road_spacing_delta = 0;  // adjust road_spacing (tighter/looser)
    std::int32_t bridge_count_delta = 0;  // adjust bridge_count

    static LegislatureProfile forLegislature(Legislature l) noexcept;
};

// ---------------------------------------------------------------------------
// Year / IQ / legislature / quality / finality parameters
// ---------------------------------------------------------------------------

// Generation parameters that describe the *kind* of city to build. Three
// drivers shape the design: the Year (modernity), the person's IQ (design
// quality / order), and the person's native Legislature (the regime that
// produces the city's lines and linear outcomes). Buildings are generated
// around city-wide targets, then each building's finality (quality of
// condition) is computed from its attributes and these drivers, with weights.
struct CityParams {
    // The city's Year. A larger, more modern year makes the city newer: taller
    // buildings, more floors and windows, more glass, and better condition.
    // Concretely, modernity = clamp((year - year_baseline) / year_span, 0..1).
    std::uint32_t year = 2807;
    std::uint32_t year_baseline = 2000;  // year mapped to modernity 0
    double year_span = 1000.0;           // years above baseline for modernity 1

    // The person's IQ. This is *both* a person-level input and, in the project's
    // terms, a system design-quality metric (IQ = insight/quality, not a human
    // psychometric rating). A higher IQ yields a better-planned city: more order
    // and regularity, better connectivity, and higher baseline condition.
    // designQuality = clamp((iq - iq_baseline) / iq_span, 0..1).
    std::uint32_t iq = 130;
    std::uint32_t iq_baseline = 100;   // iq mapped to design quality 0
    double iq_span = 100.0;            // iq points above baseline for quality 1

    // The person's native legislature: the regime that shapes the city's lines.
    Legislature legislature = Legislature::Federal;

    // Building form targets (a building varies around these).
    double avg_floors = 14.0;         // target mean number of floors
    double floor_height = 3.0;        // world units per floor (height = floors*this)
    double windows_per_floor = 6.0;   // target windows on a visible face per floor

    // Street / bridge layout (real features).
    std::uint32_t road_spacing = 8;   // every Nth row/col is a road corridor
    std::uint32_t bridge_count = 6;   // number of bridges spanning the city

    // Finality weights: how much each factor contributes to a building's
    // quality-of-condition score (finality, 0..1). They are normalized, so only
    // their relative sizes matter.
    double w_year = 1.0;              // newer year -> higher finality
    double w_iq = 0.7;               // higher IQ (design quality) -> higher finality
    double w_floors = 0.6;           // more floors -> more "finished"/dense
    double w_windows = 0.6;          // more windows -> more modern/glassy
    double w_road_proximity = 0.8;   // closer to a road -> better serviced
    double w_bridge_proximity = 0.5; // closer to a bridge -> better connected

    // Distance (in blocks) at which road/bridge proximity benefit fades to 0.
    double proximity_falloff = 6.0;

    // General cityscape-graph dimensions (see CityscapeModel). Kept here so a
    // serialized city carries them and the graph is reproducible on reload.
    std::uint32_t radix = 3;      // branching base of the distribution graph
    double diameter = 24.0;       // radial reach from the center of centricity
    double randomness = 0.35;     // 0..1 seeded variability

    // Modernity in [0,1] derived from the Year.
    double modernity() const noexcept;
    // Design quality in [0,1] derived from the IQ.
    double designQuality() const noexcept;
    // The linear regime profile derived from the legislature.
    LegislatureProfile profile() const noexcept;
};

// ---------------------------------------------------------------------------
// Config file
// ---------------------------------------------------------------------------

// Parsed representation of a City 3D config file (simple key = value text).
// Unknown keys are ignored; missing keys keep their defaults.
struct Config {
    // Model
    std::uint32_t grid_cols = 64;   // 64 x 64 = 4096 blocks (~4000)
    std::uint32_t grid_rows = 64;
    std::uint64_t seed = 0;         // 0 => derive from user name
    std::string user;               // per-user identity

    // Year, quality targets, proximity/finality weights.
    CityParams params{};

    // General cityscape-graph dimensions live on `params` (radix/diameter/
    // randomness) so they serialize with the city. Convenience accessors:
    std::uint32_t radix() const noexcept { return params.radix; }
    double diameter() const noexcept { return params.diameter; }
    double randomness() const noexcept { return params.randomness; }
    bool draw_cylinders = true;   // render the mating cylinder pairs + links

    // Appearance / view
    Theme theme = Theme::Green;
    Viewpoint viewpoint{};
    std::size_t frame_width = 1024;
    std::size_t frame_height = 768;
    bool draw_bridges = true;
    bool draw_windows = true;

    // Persistence: where a saved model is meant to live.
    //   save_target: "github" | "server" | "local"
    std::string save_target = "local";
    std::string save_location;      // repo slug, URL, or path

    RenderOptions renderOptions() const noexcept;

    // Parse from text (config file contents). Returns true; malformed numeric
    // values keep the previous/default value. `error` receives a note when a
    // value could not be parsed (parsing still succeeds best-effort).
    static Config fromText(const std::string& text, std::string* error = nullptr);
};

// ---------------------------------------------------------------------------
// City model
// ---------------------------------------------------------------------------

// What occupies a grid cell.
enum class Cell : std::uint8_t {
    Building = 0,  // a lot with a building (height may still be 0 for empty)
    Road = 1,      // a street/road corridor
    Bridge = 2     // a bridge span (an elevated road feature)
};

// One grid cell: a building with quality attributes, or a road/bridge feature.
// The finality field is the building's quality-of-condition score in [0,1].
struct Block {
    Cell cell = Cell::Building;
    std::uint16_t height = 0;   // building height in world units (0 = empty lot)
    std::uint16_t floors = 0;   // number of floors
    std::uint16_t windows = 0;  // windows on a visible face (detailing cue)

    // Computed proximities, in blocks, to the nearest road / bridge.
    float road_distance = 0.0f;
    float bridge_distance = 0.0f;

    // Quality of condition ("finality") in [0,1]: how finished, modern, and
    // well-conditioned this building is. Higher = newer/better.
    float finality = 0.0f;

    bool isBuilding() const noexcept { return cell == Cell::Building; }
};

class City {
public:
    City() = default;
    City(std::uint32_t cols, std::uint32_t rows);

    std::uint32_t cols() const noexcept { return cols_; }
    std::uint32_t rows() const noexcept { return rows_; }
    std::size_t blockCount() const noexcept {
        return static_cast<std::size_t>(cols_) * rows_;
    }

    const Block& at(std::uint32_t x, std::uint32_t y) const;
    Block& at(std::uint32_t x, std::uint32_t y);

    // The generation parameters that produced (or should produce) this city.
    const CityParams& params() const noexcept { return params_; }

    // City-wide finality: the mean quality-of-condition over all buildings, in
    // [0,1]. Represents the city's overall "model of finality".
    double cityFinality() const noexcept;

    // Procedurally build the city from a seed and parameters. Lays roads and
    // bridges (real features), computes each building's proximity to the
    // nearest road and bridge, derives floors/windows/height from the Year, and
    // computes per-building finality. Same seed + params => same city.
    void generate(std::uint64_t seed, const CityParams& params);
    // Back-compat convenience: generate with default parameters.
    void generate(std::uint64_t seed) { generate(seed, CityParams{}); }

    // Serialize / deserialize a portable text form (see MODEL_FORMAT in .cpp).
    // Serialization writes format v2 (with attributes); deserialization reads
    // both v2 and the older v1 (height-only) format.
    std::string serialize(const std::string& user, std::uint64_t seed) const;
    static bool deserialize(const std::string& text, City& out,
                            std::string* user = nullptr,
                            std::uint64_t* seed = nullptr);

private:
    void computeProximities();
    void computeFinality();

    std::uint32_t cols_ = 0;
    std::uint32_t rows_ = 0;
    std::vector<Block> blocks_;
    CityParams params_{};
};

// Derive a stable 64-bit seed from a user name (used when config seed == 0).
std::uint64_t seedForUser(const std::string& user) noexcept;

} // namespace sleela::city
