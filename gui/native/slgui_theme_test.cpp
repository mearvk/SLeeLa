#include "sleela_gui_theme.h"
#include <cstring>

int main() {
    if (slgui_theme_color_count() != 12) return 1;
    const SLGuiColor *title = slgui_theme_color("title-bar");
    const SLGuiColor *menu = slgui_theme_color("menu-bar");
    const SLGuiColor *url = slgui_theme_color("url-bar");
    if (!title || std::strcmp(title->hex, "#1D2025") != 0) return 2;
    if (!menu || std::strcmp(menu->hex, "#1D2025") != 0) return 3;
    if (!url || std::strcmp(url->hex, "#383C47") != 0) return 4;
    if (!slgui_theme_color("panel") || slgui_theme_color("missing") ||
        slgui_theme_color(nullptr) || slgui_theme_color_at(-1) ||
        slgui_theme_color_at(slgui_theme_color_count())) return 5;
    for (int i = 0; i < slgui_theme_color_count(); ++i) {
        const SLGuiColor *color = slgui_theme_color_at(i);
        if (!color || !color->name || !color->hex ||
            std::strlen(color->hex) != 7 || color->hex[0] != '#') return 6;
    }
    return 0;
}
