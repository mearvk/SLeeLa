/*
 * operating-system/src/sleela_os_theme.c
 * Implementation of the OS desktop theme model (see sleela_os_theme.h).
 * Palettes mirror the shared role vocabulary of the editor themes
 * (linux-editors/sleela_editor_theme.c) and SleelaUI (`accent`). Keep in
 * lockstep with lib/os/SLDesktopTheme.sleela.
 */
#include "sleela_os_theme.h"
#include <string.h>
#include <stddef.h>

#define C(n,h,r) { n, h, r }

/* ---- Palettes -----------------------------------------------------------
 * Each theme carries the shared role set: background, foreground, accent,
 * surface, border, keyword, type, string, comment. The dark "slick-black"
 * palette aligns with SleelaUI's matte preset; "white" aligns with the White
 * Edition look (ubuntu-white); the per-shape defaults track each OS's native
 * look (Adwaita dark, Fluent, Aqua). */

static const sleela_os_theme_color pal_slick_black[] = {
    C("Slick Black background", "#101014", "background"),
    C("Slick Black surface",    "#1A1A20", "surface"),
    C("Slick Black border",     "#2A2A33", "border"),
    C("Slick Black foreground", "#F4F7FA", "foreground"),
    C("Slick Black accent",     "#7FB3FF", "accent"),
    C("Slick Black keyword",    "#7FB3FF", "keyword"),
    C("Slick Black type",       "#73DACA", "type"),
    C("Slick Black string",     "#F2C078", "string"),
    C("Slick Black comment",    "#8A96A3", "comment")
};

static const sleela_os_theme_color pal_white[] = {
    C("White Edition background", "#FAFAFA", "background"),
    C("White Edition surface",    "#FFFFFF", "surface"),
    C("White Edition border",     "#D9DCE1", "border"),
    C("White Edition foreground", "#1C1F24", "foreground"),
    C("White Edition accent",     "#E95420", "accent"),   /* Ubuntu-orange accent */
    C("White Edition keyword",    "#2C5AA0", "keyword"),
    C("White Edition type",       "#117A65", "type"),
    C("White Edition string",     "#B5651D", "string"),
    C("White Edition comment",    "#6B7280", "comment")
};

static const sleela_os_theme_color pal_graphite[] = {
    C("Graphite background", "#202733", "background"),
    C("Graphite surface",    "#28303D", "surface"),
    C("Graphite border",     "#3A4453", "border"),
    C("Graphite foreground", "#E8EAF0", "foreground"),
    C("Graphite accent",     "#A9C0DD", "accent"),
    C("Graphite keyword",    "#A9C0DD", "keyword"),
    C("Graphite type",       "#8FC4B2", "type"),
    C("Graphite string",     "#D6BD8B", "string"),
    C("Graphite comment",    "#9AA3AF", "comment")
};

static const sleela_os_theme_color pal_linux[] = {
    C("Linux default background", "#1E1E1E", "background"),
    C("Linux default surface",    "#2D2D2D", "surface"),
    C("Linux default border",     "#3C3C3C", "border"),
    C("Linux default foreground", "#F0F0F0", "foreground"),
    C("Linux default accent",     "#3584E4", "accent"),   /* Adwaita blue */
    C("Linux default keyword",    "#3584E4", "keyword"),
    C("Linux default type",       "#26A269", "type"),
    C("Linux default string",     "#E5A50A", "string"),
    C("Linux default comment",    "#9A9996", "comment")
};

static const sleela_os_theme_color pal_windows[] = {
    C("Windows default background", "#202020", "background"),
    C("Windows default surface",    "#2B2B2B", "surface"),
    C("Windows default border",     "#3B3B3B", "border"),
    C("Windows default foreground", "#FFFFFF", "foreground"),
    C("Windows default accent",     "#0078D4", "accent"),   /* Fluent blue */
    C("Windows default keyword",    "#0078D4", "keyword"),
    C("Windows default type",       "#4EC9B0", "type"),
    C("Windows default string",     "#CE9178", "string"),
    C("Windows default comment",    "#6A9955", "comment")
};

static const sleela_os_theme_color pal_macos[] = {
    C("macOS default background", "#1E1E1E", "background"),
    C("macOS default surface",    "#2A2A2A", "surface"),
    C("macOS default border",     "#3A3A3A", "border"),
    C("macOS default foreground", "#F5F5F7", "foreground"),
    C("macOS default accent",     "#0A84FF", "accent"),   /* Aqua blue */
    C("macOS default keyword",    "#0A84FF", "keyword"),
    C("macOS default type",       "#5AC8A8", "type"),
    C("macOS default string",     "#D8A657", "string"),
    C("macOS default comment",    "#8E8E93", "comment")
};

#define PAL(a) (a), (unsigned int)(sizeof(a)/sizeof((a)[0]))

static const sleela_os_theme k_themes[] = {
    { "slick-black",     "Slick Black",       PAL(pal_slick_black),
      "SleelaUI-Dark",   "Sleela",      "Adwaita",       "slick-black-default" },
    { "white",           "White Edition",     PAL(pal_white),
      "Adwaita",         "Yaru",        "Yaru",          "ubuntu-white-default" },
    { "graphite",        "Graphite",          PAL(pal_graphite),
      "SleelaUI-Graphite","Sleela",     "Adwaita",       "graphite-default" },
    { "linux-default",   "Linux Default",     PAL(pal_linux),
      "Adwaita-dark",    "Adwaita",     "Adwaita",       "linux-default" },
    { "windows-default", "Windows Default",   PAL(pal_windows),
      "Fluent",          "Fluent",      "aero",          "windows-default" },
    { "macos-default",   "macOS Default",     PAL(pal_macos),
      "Aqua",            "SF Symbols",  "macOS",         "macos-default" }
};

#define THEME_COUNT (sizeof(k_themes)/sizeof(k_themes[0]))

static const char *k_theme_names[] = {
    "slick-black", "white", "graphite",
    "linux-default", "windows-default", "macos-default", 0
};

const char *const *sleela_os_theme_names(unsigned int *count) {
    if (count) *count = (unsigned int)THEME_COUNT;
    return k_theme_names;
}

unsigned int sleela_os_theme_count(void) {
    return (unsigned int)THEME_COUNT;
}

const sleela_os_theme *sleela_os_theme_by_id(const char *id) {
    size_t i;
    if (id == NULL) return NULL;
    for (i = 0; i < THEME_COUNT; ++i) {
        if (strcmp(k_themes[i].id, id) == 0) return &k_themes[i];
    }
    return NULL;
}

const sleela_os_theme *sleela_os_theme_default_for(sleela_os_shape shape) {
    switch (shape) {
        case SLEELA_OS_SHAPE_WINDOWS: return sleela_os_theme_by_id("windows-default");
        case SLEELA_OS_SHAPE_MACOS:   return sleela_os_theme_by_id("macos-default");
        default:                      return sleela_os_theme_by_id("linux-default");
    }
}

const sleela_os_theme_color *sleela_os_theme_role(const sleela_os_theme *theme,
                                                  const char *role) {
    unsigned int i;
    if (theme == NULL || role == NULL) return NULL;
    for (i = 0; i < theme->palette_len; ++i) {
        if (strcmp(theme->palette[i].role, role) == 0) return &theme->palette[i];
    }
    return NULL;
}

const char *sleela_os_theme_role_hex(const sleela_os_theme *theme,
                                     const char *role,
                                     const char *fallback) {
    const sleela_os_theme_color *c = sleela_os_theme_role(theme, role);
    return c != NULL ? c->hex : fallback;
}
