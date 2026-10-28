/*
 * operating-system/include/sleela_os_theme.h
 * SLeeLa OS desktop theme model -- the C backing of lib/os/SLDesktopTheme.sleela
 * (the THEMING capability: appearance of a constructed OS's desktop).
 *
 * A "theme" is the appearance a constructed OS's desktop adopts: a named set of
 * role colors (background/foreground/accent/...), plus the desktop-asset
 * identifiers an OS rollout applies -- GTK/widget theme, icon set, cursor
 * theme, and wallpaper. It is the first-class home for the kind of assets the
 * `ubuntu-white` edition carries (gtk.css, icons, greeting), expressed as a
 * model so the OS Creator can emit a desktop appearance for future users and
 * developers.
 *
 * The role-color vocabulary is deliberately the SAME as the editor palettes
 * (linux-editors/sleela_editor_theme.h: SleelaThemeColor{name,hex,role}) plus
 * SleelaUI's `accent`, so themes are consistent across the editor, the toolkit,
 * and the OS desktop rather than a third parallel scheme.
 *
 * Pure value logic: no syscalls, no allocation. Static strings owned by the
 * library; callers must not free them. Mirrors SLDesktopTheme.sleela; the two
 * are the same source of truth expressed once in each language.
 */
#ifndef SLEELA_OS_THEME_H
#define SLEELA_OS_THEME_H

#include "sleela_os_shape.h"

#ifdef __cplusplus
extern "C" {
#endif

/* One named role color. Same shape as the editor palette's SleelaThemeColor so
 * the two systems share a vocabulary. `role` is one of the shared role names:
 * background, foreground, accent, surface, border, keyword, type, string,
 * comment (the editor roles are reused where a desktop maps onto them). */
typedef struct sleela_os_theme_color {
    const char *name;  /* human label, e.g. "Slick Black background" */
    const char *hex;   /* "#RRGGBB" */
    const char *role;  /* role token */
} sleela_os_theme_color;

/* A complete desktop theme: an id, a role-color palette, and the desktop-asset
 * identifiers an OS rollout applies. The asset ids are the names of the GTK
 * theme / icon set / cursor theme / wallpaper the `desktop-loader` warehouse
 * component supplies; the bytes are fetched from there, not carried here. */
typedef struct sleela_os_theme {
    const char                  *id;        /* "slick-black", "white", ... */
    const char                  *label;     /* "Slick Black", "White Edition" */
    const sleela_os_theme_color *palette;   /* role colors */
    unsigned int                 palette_len;
    const char                  *gtk_theme; /* widget/GTK theme name */
    const char                  *icon_set;  /* icon theme name */
    const char                  *cursor;    /* cursor theme name */
    const char                  *wallpaper; /* wallpaper asset id */
} sleela_os_theme;

/* The built-in theme ids, NUL-terminated list. `count` out-param optional. */
const char *const *sleela_os_theme_names(unsigned int *count);

/* Look up a theme by id. Returns NULL for an unknown id. Known ids:
 * "slick-black" (the SleelaUI-aligned dark default), "white" (the White
 * Edition, aligned with ubuntu-white), "graphite", and the per-shape defaults
 * "linux-default" / "windows-default" / "macos-default". */
const sleela_os_theme *sleela_os_theme_by_id(const char *id);

/* The default desktop theme for an OS shape (Linux -> Adwaita-ish dark,
 * Windows -> Fluent-ish, macOS -> Aqua-ish). Never NULL. */
const sleela_os_theme *sleela_os_theme_default_for(sleela_os_shape shape);

/* Find a single role color within a theme by role name (e.g. "accent").
 * Returns NULL if the theme or role is unknown. */
const sleela_os_theme_color *sleela_os_theme_role(const sleela_os_theme *theme,
                                                  const char *role);

/* The hex for a role, or `fallback` (which may be NULL) when absent. */
const char *sleela_os_theme_role_hex(const sleela_os_theme *theme,
                                     const char *role,
                                     const char *fallback);

/* Count of built-in themes. */
unsigned int sleela_os_theme_count(void);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_THEME_H */
