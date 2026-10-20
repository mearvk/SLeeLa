#ifndef SLEELA_GUI_THEME_H
#define SLEELA_GUI_THEME_H

/* Stable toolkit-neutral theme tokens shared by native C/C++ hosts. */
#ifdef __cplusplus
extern "C" {
#endif

typedef struct SLGuiColor { const char *name; const char *hex; } SLGuiColor;
#define SLGUI_COLOR_TITLE_BAR  "#1D2025"
#define SLGUI_COLOR_MENU_BAR   "#1D2025"
#define SLGUI_COLOR_URL_BAR    "#383C47"
#define SLGUI_COLOR_PANEL      "#292D35"
#define SLGUI_COLOR_SURFACE    "#22262D"
#define SLGUI_COLOR_TEXT       "#E8EAED"
#define SLGUI_COLOR_MUTED_TEXT "#B0B7C3"
#define SLGUI_COLOR_BORDER     "#444A56"
#define SLGUI_COLOR_ACCENT     "#4F8CFF"
#define SLGUI_COLOR_FOCUS      "#8AB4F8"
#define SLGUI_COLOR_DANGER     "#D9534F"
#define SLGUI_COLOR_SUCCESS    "#3FA776"

int slgui_theme_color_count(void);
const SLGuiColor *slgui_theme_color_at(int index);
const SLGuiColor *slgui_theme_color(const char *name);
#ifdef __cplusplus
}
#endif
#endif
