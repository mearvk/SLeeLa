#include "sleela_gui_theme.h"
#include <cassert>
#include <cstring>
int main() {
    assert(slgui_theme_color_count() == 11);
    const SLGuiColor *title = slgui_theme_color("title-bar");
    const SLGuiColor *url = slgui_theme_color("url-bar");
    assert(title && std::strcmp(title->hex, "#1D2025") == 0);
    assert(url && std::strcmp(url->hex, "#383C47") == 0);
    assert(slgui_theme_color("panel") != nullptr);
    assert(slgui_theme_color("missing") == nullptr);
    assert(slgui_theme_color(nullptr) == nullptr);
    assert(slgui_theme_color_at(-1) == nullptr);
    assert(slgui_theme_color_at(slgui_theme_color_count()) == nullptr);
    for (int i = 0; i < slgui_theme_color_count(); ++i) {
        const SLGuiColor *c = slgui_theme_color_at(i);
        assert(c && c->name && c->hex && std::strlen(c->hex) == 7 && c->hex[0] == '#');
    }
    return 0;
}
