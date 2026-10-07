#ifndef SLUI_THEME_HPP
#define SLUI_THEME_HPP
/* Internal decoded theme: the public SLUITheme (0xRRGGBBAA words) resolved into
 * slui::Color so the renderer and widgets use it directly. The DEFAULT preset
 * is "Slick Black": a deep matte-black surface family with a single cool steel
 * accent and crisp white text, tuned to clear WCAG AA on every role pairing.
 * Max Rupplin -- MEARVK LLC -- 2026. */
#include "sleela_ui.h"
#include "slui_geometry.hpp"

#include <string>

namespace slui {

struct Theme {
    Color bg, surface, surface_hi, chrome, border;
    Color fg, fg_dim;
    Color accent, accent_fg, danger;
    Color hover, active;
    int radius = 8;
    int unit = 4;
    int control_height = 34;
    std::string font_family = "system";
    int font_size = 11;

    static Theme from_abi(const SLUITheme& t) {
        Theme r;
        r.bg = Color::from_abi(t.bg);
        r.surface = Color::from_abi(t.surface);
        r.surface_hi = Color::from_abi(t.surface_hi);
        r.chrome = Color::from_abi(t.chrome);
        r.border = Color::from_abi(t.border);
        r.fg = Color::from_abi(t.fg);
        r.fg_dim = Color::from_abi(t.fg_dim);
        r.accent = Color::from_abi(t.accent);
        r.accent_fg = Color::from_abi(t.accent_fg);
        r.danger = Color::from_abi(t.danger);
        r.hover = Color::from_abi(t.hover);
        r.active = Color::from_abi(t.active);
        r.radius = t.radius;
        r.unit = t.unit > 0 ? t.unit : 4;
        r.control_height = t.control_height > 0 ? t.control_height : 34;
        r.font_family = t.font_family[0] ? t.font_family : "system";
        r.font_size = t.font_size > 0 ? t.font_size : 11;
        return r;
    }
};

/* Fill a public SLUITheme with one of the presets. Shared by the C ABI
 * slui_theme_preset() and the default-window path. */
void theme_fill_preset(SLUITheme* out, SLUIThemeId id);

} // namespace slui

#endif /* SLUI_THEME_HPP */
