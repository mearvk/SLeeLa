/* =============================================================================
 * SleelaUI built-in theme presets.
 *
 * "Slick Black" is the toolkit's default and signature look: a matte, near-pure
 * black that stays flat (no glossy highlights, no coloured casts) with one cool
 * steel-blue accent and crisp white text. The palette is a single source of
 * truth -- every widget reads these roles, so retheming is one struct edit, in
 * the spirit of the SleelaTerminal UI principles (one palette, restraint over
 * effects, >=WCAG AA contrast, >=32px hit targets).
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "slui_theme.hpp"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>

namespace slui {

static void copy_family(SLUITheme* t, const char* name) {
    std::strncpy(t->font_family, name, sizeof(t->font_family) - 1);
    t->font_family[sizeof(t->font_family) - 1] = '\0';
}

/* ---- palette derivation from one base colour ---------------------------- */
/* A base colour (the window floor) implies a whole family. We lighten it in
 * even perceptual steps for raised surfaces, pull a hairline border a bit
 * brighter, pick a warm complementary accent, and choose text colours that
 * clear WCAG AA against the floor. This is what makes #2B1608 -- or any base a
 * host configures -- re-theme the entire UI from a single value. */
static Color decode(SLUIColor c) {
    return Color{static_cast<uint8_t>((c >> 24) & 0xFF),
                 static_cast<uint8_t>((c >> 16) & 0xFF),
                 static_cast<uint8_t>((c >> 8) & 0xFF),
                 static_cast<uint8_t>(c & 0xFF)};
}
static uint8_t clamp8(double v) {
    if (v < 0) v = 0;
    if (v > 255) v = 255;
    return static_cast<uint8_t>(v + 0.5);
}
/* Lighten toward white by t, keeping the hue (a warm tint stays warm). */
static SLUIColor lighten(Color b, double t) {
    return slui_rgb(clamp8(b.r + (255 - b.r) * t), clamp8(b.g + (255 - b.g) * t),
                    clamp8(b.b + (255 - b.b) * t));
}
/* Relative luminance in [0,1]. */
static double luma(Color c) {
    return (0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b) / 255.0;
}

static void derive_palette_from_base(SLUITheme* out, SLUIColor base) {
    Color b = decode(base);
    out->bg = slui_rgb(b.r, b.g, b.b); /* opaque floor */
    /* Raised surfaces: even lightening steps so panels read without hard edges.
     * Warm bases lift a touch warmer by biasing toward the base's own hue. */
    out->surface = lighten(b, 0.10);
    out->surface_hi = lighten(b, 0.20);
    /* Chrome sits just below the floor so the title bar frames the content. */
    out->chrome = slui_rgb(clamp8(b.r * 0.80), clamp8(b.g * 0.80),
                           clamp8(b.b * 0.80));
    out->border = lighten(b, 0.32);
    /* Text: white if the floor is dark, near-black if it is light, so AA holds
     * for any configured base. */
    bool dark = luma(b) < 0.45;
    out->fg = dark ? slui_rgb(0xFF, 0xF6, 0xEE) /* warm white */
                   : slui_rgb(0x1A, 0x0E, 0x05);
    out->fg_dim = dark ? slui_rgba(0xFF, 0xF6, 0xEE, 0xB3)
                       : slui_rgba(0x1A, 0x0E, 0x05, 0xB3);
    /* Accent: a warm amber that belongs to the brown family (an ember glow),
     * with text chosen for contrast on it. */
    out->accent = slui_rgb(0xE8, 0x9A, 0x3C);     /* warm amber */
    out->accent_fg = slui_rgb(0x24, 0x12, 0x04);  /* dark on amber */
    out->danger = slui_rgb(0xE5, 0x5B, 0x3C);     /* warm red-orange */
    out->hover = slui_rgba(0xFF, 0xF2, 0xDC, 0x14);  /* warm additive tint */
    out->active = slui_rgba(0xFF, 0xF2, 0xDC, 0x28);
}

void theme_fill_preset(SLUITheme* out, SLUIThemeId id) {
    if (!out) return;
    std::memset(out, 0, sizeof(*out));
    out->unit = 4;
    out->radius = 8;
    out->control_height = 34; /* clears the 32px minimum hit target */
    out->font_size = 11;

    switch (id) {
    case SLUI_THEME_GRAPHITE:
        /* A lighter neutral alternative for users who want more contrast off
         * the floor; still flat, still one accent. */
        out->id = SLUI_THEME_GRAPHITE;
        out->bg = slui_rgb(0x24, 0x26, 0x2b);
        out->surface = slui_rgb(0x2d, 0x30, 0x36);
        out->surface_hi = slui_rgb(0x37, 0x3b, 0x42);
        out->chrome = slui_rgb(0x1f, 0x21, 0x25);
        out->border = slui_rgb(0x45, 0x49, 0x51);
        out->fg = slui_rgb(0xf2, 0xf3, 0xf5);
        out->fg_dim = slui_rgba(0xf2, 0xf3, 0xf5, 0xB0);
        out->accent = slui_rgb(0x4c, 0x8d, 0xff);
        out->accent_fg = slui_rgb(0x0b, 0x12, 0x1f);
        out->danger = slui_rgb(0xe0, 0x55, 0x55);
        out->hover = slui_rgba(0xff, 0xff, 0xff, 0x14);
        out->active = slui_rgba(0xff, 0xff, 0xff, 0x24);
        copy_family(out, "system");
        break;

    case SLUI_THEME_SLICK_BLACK:
        /* The original matte-black theme, kept for hosts that want it. */
        out->id = SLUI_THEME_SLICK_BLACK;
        out->bg = slui_rgb(0x0d, 0x0d, 0x0f);        /* matte near-black floor */
        out->surface = slui_rgb(0x16, 0x16, 0x19);   /* panels / cards         */
        out->surface_hi = slui_rgb(0x20, 0x21, 0x25); /* hovered / gradient top */
        out->chrome = slui_rgb(0x10, 0x10, 0x13);    /* title bar strip        */
        out->border = slui_rgb(0x2a, 0x2b, 0x30);    /* hairline separators    */
        out->fg = slui_rgb(0xff, 0xff, 0xff);        /* primary text           */
        out->fg_dim = slui_rgba(0xff, 0xff, 0xff, 0xB3); /* ~0.70 secondary    */
        out->accent = slui_rgb(0x5e, 0x9c, 0xff);    /* cool steel-blue accent */
        out->accent_fg = slui_rgb(0x07, 0x0c, 0x16); /* text on the accent     */
        out->danger = slui_rgb(0xe5, 0x4b, 0x4b);    /* destructive / close    */
        out->hover = slui_rgba(0xff, 0xff, 0xff, 0x12);  /* +~.07 tint         */
        out->active = slui_rgba(0xff, 0xff, 0xff, 0x24); /* +~.14 tint         */
        copy_family(out, "system");
        break;

    case SLUI_THEME_WHITE:
        /* Standard Pattern: WHITE. A light surface family with dark text and a
         * blue accent -- the classic light desktop look. */
        out->id = SLUI_THEME_WHITE;
        out->bg = slui_rgb(0xF4, 0xF5, 0xF7);        /* light application floor */
        out->surface = slui_rgb(0xFF, 0xFF, 0xFF);   /* white panels / cards    */
        out->surface_hi = slui_rgb(0xEA, 0xEC, 0xF0); /* hovered raised surface */
        out->chrome = slui_rgb(0xE6, 0xE8, 0xEC);    /* light title / header    */
        out->border = slui_rgb(0xD0, 0xD4, 0xDA);    /* soft hairline           */
        out->fg = slui_rgb(0x1A, 0x1C, 0x20);        /* near-black primary text */
        out->fg_dim = slui_rgba(0x1A, 0x1C, 0x20, 0xA0);
        out->accent = slui_rgb(0x2F, 0x6F, 0xED);    /* blue accent             */
        out->accent_fg = slui_rgb(0xFF, 0xFF, 0xFF);
        out->danger = slui_rgb(0xD0, 0x3A, 0x34);
        out->hover = slui_rgba(0x00, 0x00, 0x00, 0x0E);  /* subtle dark tint    */
        out->active = slui_rgba(0x00, 0x00, 0x00, 0x1C);
        copy_family(out, "system");
        break;

    case SLUI_THEME_BLACK:
        /* Standard Pattern: BLACK. Deep black surfaces with light text; a
         * stronger, flatter sibling of SLICK_BLACK. */
        out->id = SLUI_THEME_BLACK;
        out->bg = slui_rgb(0x08, 0x08, 0x0A);
        out->surface = slui_rgb(0x12, 0x12, 0x15);
        out->surface_hi = slui_rgb(0x1C, 0x1D, 0x22);
        out->chrome = slui_rgb(0x0B, 0x0B, 0x0E);
        out->border = slui_rgb(0x26, 0x27, 0x2D);
        out->fg = slui_rgb(0xF3, 0xF4, 0xF6);
        out->fg_dim = slui_rgba(0xF3, 0xF4, 0xF6, 0xA6);
        out->accent = slui_rgb(0x6C, 0xA6, 0xFF);
        out->accent_fg = slui_rgb(0x06, 0x0A, 0x14);
        out->danger = slui_rgb(0xE5, 0x4B, 0x4B);
        out->hover = slui_rgba(0xFF, 0xFF, 0xFF, 0x12);
        out->active = slui_rgba(0xFF, 0xFF, 0xFF, 0x24);
        copy_family(out, "system");
        break;

    case SLUI_THEME_BLUE:
        /* Standard Pattern: BLUE. Blue-tinted dark surfaces with light text and
         * a brighter blue accent. */
        out->id = SLUI_THEME_BLUE;
        out->bg = slui_rgb(0x10, 0x1A, 0x2E);
        out->surface = slui_rgb(0x17, 0x24, 0x3E);
        out->surface_hi = slui_rgb(0x20, 0x31, 0x52);
        out->chrome = slui_rgb(0x0C, 0x14, 0x24);
        out->border = slui_rgb(0x2C, 0x41, 0x68);
        out->fg = slui_rgb(0xEC, 0xF1, 0xFB);
        out->fg_dim = slui_rgba(0xEC, 0xF1, 0xFB, 0xAA);
        out->accent = slui_rgb(0x4F, 0x8C, 0xFF);
        out->accent_fg = slui_rgb(0x08, 0x12, 0x24);
        out->danger = slui_rgb(0xE5, 0x5B, 0x55);
        out->hover = slui_rgba(0xFF, 0xFF, 0xFF, 0x12);
        out->active = slui_rgba(0xFF, 0xFF, 0xFF, 0x22);
        copy_family(out, "system");
        break;

    case SLUI_THEME_GRAY:
        /* Standard Pattern: GRAY. Neutral mid-gray surfaces, light text, cool
         * accent -- a quiet, professional neutral. */
        out->id = SLUI_THEME_GRAY;
        out->bg = slui_rgb(0x3A, 0x3D, 0x42);
        out->surface = slui_rgb(0x45, 0x48, 0x4E);
        out->surface_hi = slui_rgb(0x52, 0x56, 0x5D);
        out->chrome = slui_rgb(0x30, 0x33, 0x38);
        out->border = slui_rgb(0x5A, 0x5E, 0x66);
        out->fg = slui_rgb(0xF0, 0xF1, 0xF3);
        out->fg_dim = slui_rgba(0xF0, 0xF1, 0xF3, 0xA8);
        out->accent = slui_rgb(0x7F, 0xB0, 0xFF);
        out->accent_fg = slui_rgb(0x0D, 0x14, 0x20);
        out->danger = slui_rgb(0xE0, 0x5A, 0x55);
        out->hover = slui_rgba(0xFF, 0xFF, 0xFF, 0x12);
        out->active = slui_rgba(0xFF, 0xFF, 0xFF, 0x22);
        copy_family(out, "system");
        break;

    case SLUI_THEME_SLEELA_BASE:
    case SLUI_THEME_CUSTOM:
    default:
        /* THE DEFAULT. Sleela's base colour is a deep warm brown, #2B1608 --
         * a dark toasted umber. The whole palette is DERIVED from it: raised
         * surfaces are even warm lightenings of the floor, the chrome sits just
         * below it, the border is a warm hairline, the accent is an ember amber,
         * and text is a warm white that clears WCAG AA on the floor. Change the
         * one base value (code or config) and the family re-tunes around it. */
        out->id = SLUI_THEME_SLEELA_BASE;
        derive_palette_from_base(out, SLUI_BASE_COLOR_DEFAULT);
        copy_family(out, "system");
        break;
    }
}

} // namespace slui

/* ---- config parsing helpers --------------------------------------------- */
namespace slui {
/* Parse "#RRGGBB", "0xRRGGBB", or a bare hex into an opaque 0xRRGGBBAA word.
 * Returns false if it does not look like a colour. */
static bool parse_hex_color(const std::string& in, SLUIColor* out) {
    std::string s;
    for (char c : in)
        if (!std::isspace(static_cast<unsigned char>(c))) s += c;
    size_t pos = 0;
    if (s.rfind("#", 0) == 0) pos = 1;
    else if (s.rfind("0x", 0) == 0 || s.rfind("0X", 0) == 0) pos = 2;
    std::string hex = s.substr(pos);
    if (hex.size() != 6 && hex.size() != 8) return false;
    for (char c : hex)
        if (!std::isxdigit(static_cast<unsigned char>(c))) return false;
    unsigned long v = std::strtoul(hex.c_str(), nullptr, 16);
    if (hex.size() == 6) {
        *out = (static_cast<SLUIColor>(v) << 8) | 0xFF; /* add opaque alpha */
    } else {
        *out = static_cast<SLUIColor>(v);
    }
    return true;
}
} // namespace slui

/* ---- public C ABI -------------------------------------------------------- */
extern "C" void slui_theme_preset(SLUITheme* out, SLUIThemeId id) {
    slui::theme_fill_preset(out, id);
}

extern "C" void slui_theme_set_base_color(SLUITheme* out, SLUIColor base) {
    if (!out) return;
    slui::derive_palette_from_base(out, base);
    out->id = SLUI_THEME_SLEELA_BASE;
}

extern "C" SLUIStatus slui_theme_load_config(SLUITheme* out, const char* path) {
    if (!out || !path) return SLUI_ERR_INVALID;
    std::ifstream in(path);
    if (!in) return SLUI_ERR_INVALID;
    std::string line;
    while (std::getline(in, line)) {
        /* strip comments and surrounding whitespace */
        auto hash = line.find('#');
        /* keep '#' if it begins a colour value after '='; only treat a leading
         * '#' (comment) specially */
        size_t first = line.find_first_not_of(" \t");
        if (first == std::string::npos) continue;
        if (line[first] == '#' || line[first] == ';') continue;
        (void)hash;
        auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);
        auto trim = [](std::string& s) {
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == std::string::npos) { s.clear(); return; }
            s = s.substr(a, b - a + 1);
        };
        trim(key);
        trim(val);
        if (key == "base_color") {
            SLUIColor c;
            if (slui::parse_hex_color(val, &c)) {
                slui::derive_palette_from_base(out, c);
                out->id = SLUI_THEME_SLEELA_BASE;
            }
        } else if (key == "accent") {
            SLUIColor c;
            if (slui::parse_hex_color(val, &c)) out->accent = c;
        } else if (key == "font_family") {
            std::strncpy(out->font_family, val.c_str(),
                         sizeof(out->font_family) - 1);
            out->font_family[sizeof(out->font_family) - 1] = '\0';
        } else if (key == "font_size") {
            int n = std::atoi(val.c_str());
            if (n > 0) out->font_size = n;
        } else if (key == "radius") {
            int n = std::atoi(val.c_str());
            if (n >= 0) out->radius = n;
        }
    }
    return SLUI_OK;
}
