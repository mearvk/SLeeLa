#pragma once

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
    double scale_height = 3.0;

    // Pixels between block centers on the ground grid.
    double block_pitch = 9.0;

    // Screen origin (pixels) where world (0,0) ground point lands.
    double origin_x = 150.0;
    double origin_y = 210.0;
};

// Everything needed to render, resolved from a Config.
struct RenderOptions {
    Theme theme = Theme::Green;
    Viewpoint viewpoint{};
    std::size_t frame_width = 1024;
    std::size_t frame_height = 768;
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

    // Appearance / view
    Theme theme = Theme::Green;
    Viewpoint viewpoint{};
    std::size_t frame_width = 1024;
    std::size_t frame_height = 768;

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

struct Block {
    std::uint16_t height = 0;  // building height in world units (0 = empty lot)
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

    // Procedurally fill the grid from a seed. Same seed => same city.
    void generate(std::uint64_t seed);

    // Serialize / deserialize a portable text form (see MODEL_FORMAT in .cpp).
    std::string serialize(const std::string& user, std::uint64_t seed) const;
    static bool deserialize(const std::string& text, City& out,
                            std::string* user = nullptr,
                            std::uint64_t* seed = nullptr);

private:
    std::uint32_t cols_ = 0;
    std::uint32_t rows_ = 0;
    std::vector<Block> blocks_;
};

// Derive a stable 64-bit seed from a user name (used when config seed == 0).
std::uint64_t seedForUser(const std::string& user) noexcept;

} // namespace sleela::city
