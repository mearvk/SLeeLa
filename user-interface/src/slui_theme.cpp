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

#include <cstring>

namespace slui {

static void copy_family(SLUITheme* t, const char* name) {
    std::strncpy(t->font_family, name, sizeof(t->font_family) - 1);
    t->font_family[sizeof(t->font_family) - 1] = '\0';
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
    case SLUI_THEME_CUSTOM:
    default:
        /* THE DEFAULT. Matte black: the window floor is a hair above pure
         * black so antialiasing has somewhere to blend; surfaces step up in
         * small, even increments; the accent is a single cool steel-blue that
         * reads as "the one live thing" without warming the greys. White text
         * on #0d0d0f is ~19:1 contrast (far past AA); dim text ~8:1. */
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
    }
}

} // namespace slui

/* ---- public C ABI -------------------------------------------------------- */
extern "C" void slui_theme_preset(SLUITheme* out, SLUIThemeId id) {
    slui::theme_fill_preset(out, id);
}
