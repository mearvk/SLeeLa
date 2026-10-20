#include "sleela_gui_theme.h"
#include <cstring>

namespace {
const SLGuiColor kColors[] = {
    {"title-bar", SLGUI_COLOR_TITLE_BAR}, {"menu-bar", SLGUI_COLOR_MENU_BAR},
    {"url-bar", SLGUI_COLOR_URL_BAR},
    {"panel", SLGUI_COLOR_PANEL}, {"surface", SLGUI_COLOR_SURFACE},
    {"text", SLGUI_COLOR_TEXT}, {"muted-text", SLGUI_COLOR_MUTED_TEXT},
    {"border", SLGUI_COLOR_BORDER}, {"accent", SLGUI_COLOR_ACCENT},
    {"focus", SLGUI_COLOR_FOCUS}, {"danger", SLGUI_COLOR_DANGER},
    {"success", SLGUI_COLOR_SUCCESS}
};
constexpr int kColorCount = static_cast<int>(sizeof(kColors) / sizeof(kColors[0]));
}
extern "C" int slgui_theme_color_count(void) { return kColorCount; }
extern "C" const SLGuiColor *slgui_theme_color_at(int index) {
    if (index < 0 || index >= kColorCount) return nullptr;
    return &kColors[index];
}
extern "C" const SLGuiColor *slgui_theme_color(const char *name) {
    if (name == nullptr) return nullptr;
    for (int i = 0; i < kColorCount; ++i)
        if (std::strcmp(kColors[i].name, name) == 0) return &kColors[i];
    return nullptr;
}
