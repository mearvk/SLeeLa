#ifndef SLEELA_EDITOR_THEME_H
#define SLEELA_EDITOR_THEME_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct SleelaThemeColor { const char *name; const char *hex; const char *role; } SleelaThemeColor;
const SleelaThemeColor *sleela_theme_palette(unsigned int *count);
const SleelaThemeColor *sleela_theme_find(const char *name);
int sleela_theme_print_terminal(void);
#ifdef __cplusplus
}
#endif
#endif
