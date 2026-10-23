#ifndef SLEELA_EDITOR_THEME_H
#define SLEELA_EDITOR_THEME_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct SleelaThemeColor { const char *name; const char *hex; const char *role; } SleelaThemeColor;
/* Theme IDs: american, eurasia, russian-slavic, silk-road, east-asian. */
const char *sleela_theme_names(unsigned int *count);
const SleelaThemeColor *sleela_theme_palette_for(const char *theme, unsigned int *count);
const SleelaThemeColor *sleela_theme_find_for(const char *theme, const char *name);
/* Backward-compatible default palette: American Professional. */
const SleelaThemeColor *sleela_theme_palette(unsigned int *count);
const SleelaThemeColor *sleela_theme_find(const char *name);
int sleela_theme_print_terminal(void);
int sleela_theme_print_terminal_for(const char *theme);
#ifdef __cplusplus
}
#endif
#endif
