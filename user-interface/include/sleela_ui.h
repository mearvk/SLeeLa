#ifndef SLEELA_UI_H
#define SLEELA_UI_H
/* =============================================================================
 * SleelaUI(TM) -- SLeeLa's own, original cross-platform User Interface toolkit.
 *
 * This is the stable C Application Binary Interface for the toolkit. Everything
 * a host program (C, C++, or the SLeeLa VM/OS bridge) needs to open a window,
 * build a widget tree, theme it, and run an event loop is reachable from this
 * header with C linkage. The C++ implementation lives behind it in src/.
 *
 * SleelaUI is an ORIGINAL toolkit: it is not GTK, Qt, wxWidgets, or a wrapper
 * around them. It draws its own widgets with its own software rasterizer so the
 * look -- the "Slick Black" matte default -- is pixel-identical on every target.
 * It talks to the real host window system through one backend interface with
 * three implementations:
 *
 *   * Windows 10+ ....... Win32 (user32/gdi32) backend
 *   * macOS (Darwin) .... Cocoa (AppKit) backend
 *   * Linux / Unix ...... X11 (Xlib) backend
 *
 * The design mirrors SLeeLa's impl/core OS-abstraction philosophy: one portable
 * surface, native backends underneath, the runtime able to report which backend
 * is live.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Version
 * ------------------------------------------------------------------------- */
#define SLUI_VERSION_MAJOR 1
#define SLUI_VERSION_MINOR 0
#define SLUI_VERSION_PATCH 0
#define SLUI_VERSION_STRING "1.0.0"

/* --------------------------------------------------------------------------
 * Host window-system backend identity. Mirrors SLPlatform in impl/core so the
 * runtime can report which real window system is driving the pixels.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_BACKEND_UNKNOWN = 0,
    SLUI_BACKEND_WIN32 = 1,   /* Windows 10+                  */
    SLUI_BACKEND_COCOA = 2,   /* macOS (Darwin, AppKit)       */
    SLUI_BACKEND_X11 = 3      /* Linux / Unix (Xlib)          */
} SLUIBackend;

/* The backend that was compiled in and is driving this process. */
SLUIBackend slui_backend(void);
const char *slui_backend_name(SLUIBackend backend);
const char *slui_version_string(void);

/* --------------------------------------------------------------------------
 * Status codes. Non-negative is success; negatives are errors.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_OK = 0,
    SLUI_ERR_ALLOC = -1,       /* out of memory                              */
    SLUI_ERR_BACKEND = -2,     /* the host window system refused the request */
    SLUI_ERR_INVALID = -3,     /* a NULL / out-of-range argument             */
    SLUI_ERR_UNSUPPORTED = -4  /* not available on this backend              */
} SLUIStatus;

/* --------------------------------------------------------------------------
 * Geometry and colour. Colours are straight-alpha 0xRRGGBBAA packed words;
 * helpers below keep call sites readable.
 * ------------------------------------------------------------------------- */
typedef struct { int x, y; } SLUIPoint;
typedef struct { int w, h; } SLUISize;
typedef struct { int x, y, w, h; } SLUIRect;
typedef uint32_t SLUIColor; /* 0xRRGGBBAA */

static inline SLUIColor slui_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((SLUIColor)r << 24) | ((SLUIColor)g << 16) | ((SLUIColor)b << 8) |
           (SLUIColor)a;
}
static inline SLUIColor slui_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return slui_rgba(r, g, b, 0xFF);
}

/* --------------------------------------------------------------------------
 * Theme. The toolkit ships a configurable palette; the DEFAULT is "Slick
 * Black" -- a deep matte-black family with a single cool accent. A theme is a
 * flat set of named role colours plus a corner radius and base metrics, so a
 * host can restyle the whole UI by editing one struct (or loading a .conf).
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_THEME_SLICK_BLACK = 0, /* the default matte-black theme             */
    SLUI_THEME_GRAPHITE = 1,    /* a lighter neutral alternative             */
    SLUI_THEME_CUSTOM = 2       /* fully host-defined via SLUITheme fields   */
} SLUIThemeId;

typedef struct {
    SLUIThemeId id;
    /* Surfaces, from the window floor up to raised panels. */
    SLUIColor bg;         /* window / root background (the matte black)      */
    SLUIColor surface;    /* panels, cards, menus                            */
    SLUIColor surface_hi; /* hovered surface / gradient top                  */
    SLUIColor chrome;     /* title bar / header strip                        */
    SLUIColor border;     /* 1px hairline separators                         */
    /* Text. */
    SLUIColor fg;         /* primary text and glyphs                         */
    SLUIColor fg_dim;     /* secondary / disabled text                       */
    /* Accent + state tints. */
    SLUIColor accent;     /* focus ring, suggested action, selection         */
    SLUIColor accent_fg;  /* text drawn on top of the accent                 */
    SLUIColor danger;     /* destructive action / close-hover                */
    SLUIColor hover;      /* additive hover tint (small alpha)               */
    SLUIColor active;     /* additive pressed tint (small alpha)             */
    /* Metrics. The 4px spacing scale and a single corner radius. */
    int radius;           /* corner radius for buttons/cards (px)            */
    int unit;             /* base spacing unit, 4px                          */
    int control_height;   /* minimum hit target height (>= 32px)             */
    /* Typography. Family is a backend-resolved name; sizes are in points.   */
    char font_family[64];
    int font_size;        /* UI text size (pt)                               */
} SLUITheme;

/* Fill `out` with one of the built-in palettes. SLUI_THEME_SLICK_BLACK is the
 * documented default and what slui_window_create() uses when given no theme. */
void slui_theme_preset(SLUITheme *out, SLUIThemeId id);

/* --------------------------------------------------------------------------
 * Opaque handles. The toolkit owns these; a widget is owned by its parent and
 * freed when the window (its root) is destroyed.
 * ------------------------------------------------------------------------- */
typedef struct SLUIApp SLUIApp;
typedef struct SLUIWindow SLUIWindow;
typedef struct SLUIWidget SLUIWidget;
/* Defined in sleela_ui_draw.h; forward-declared here for the canvas-view API. */
typedef struct SLUIDrawContext SLUIDrawContext;

/* --------------------------------------------------------------------------
 * Events. The event model is a small, explicit tagged union so a C host can
 * handle input without C++ and the VM bridge can marshal it trivially.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_EVENT_NONE = 0,
    SLUI_EVENT_CLOSE,          /* the window's close button / WM close       */
    SLUI_EVENT_RESIZE,         /* window content size changed                */
    SLUI_EVENT_POINTER_MOVE,
    SLUI_EVENT_POINTER_DOWN,
    SLUI_EVENT_POINTER_UP,
    SLUI_EVENT_KEY_DOWN,
    SLUI_EVENT_KEY_UP,
    SLUI_EVENT_TEXT,           /* a committed unicode scalar (utf-32)        */
    SLUI_EVENT_FOCUS,
    SLUI_EVENT_BLUR
} SLUIEventType;

typedef enum {
    SLUI_BUTTON_NONE = 0,
    SLUI_BUTTON_LEFT = 1,
    SLUI_BUTTON_MIDDLE = 2,
    SLUI_BUTTON_RIGHT = 3
} SLUIPointerButton;

/* Modifier bitmask. */
enum {
    SLUI_MOD_SHIFT = 1u << 0,
    SLUI_MOD_CTRL = 1u << 1,
    SLUI_MOD_ALT = 1u << 2,
    SLUI_MOD_SUPER = 1u << 3
};

typedef struct {
    SLUIEventType type;
    SLUIWindow *window;
    /* pointer */
    int x, y;
    SLUIPointerButton button;
    /* keyboard */
    uint32_t keysym;      /* backend-normalised key code (see SLUI_KEY_*)    */
    uint32_t codepoint;   /* for SLUI_EVENT_TEXT: the utf-32 scalar          */
    uint32_t modifiers;   /* SLUI_MOD_* bitmask                              */
    /* resize */
    int width, height;
} SLUIEvent;

/* A handful of normalised, backend-independent key codes. */
enum {
    SLUI_KEY_UNKNOWN = 0,
    SLUI_KEY_ENTER = 0x0D,
    SLUI_KEY_ESCAPE = 0x1B,
    SLUI_KEY_BACKSPACE = 0x08,
    SLUI_KEY_TAB = 0x09,
    SLUI_KEY_SPACE = 0x20,
    SLUI_KEY_LEFT = 0x0100,
    SLUI_KEY_RIGHT = 0x0101,
    SLUI_KEY_UP = 0x0102,
    SLUI_KEY_DOWN = 0x0103,
    SLUI_KEY_HOME = 0x0104,
    SLUI_KEY_END = 0x0105,
    SLUI_KEY_DELETE = 0x0106
};

/* --------------------------------------------------------------------------
 * Application lifecycle.
 * ------------------------------------------------------------------------- */
SLUIApp *slui_app_create(const char *app_id);
void slui_app_destroy(SLUIApp *app);

/* Process pending host events once. Returns the number of events dispatched,
 * or a negative SLUIStatus on backend failure. `block` != 0 waits for at least
 * one event. */
int slui_app_pump(SLUIApp *app, int block);

/* Run the toolkit's own event loop until every window has closed (or
 * slui_app_quit() is called). Returns the process-style exit code. */
int slui_app_run(SLUIApp *app);
void slui_app_quit(SLUIApp *app, int exit_code);

/* --------------------------------------------------------------------------
 * Windows. A window owns a root container widget; add children to it.
 * ------------------------------------------------------------------------- */
typedef struct {
    const char *title;
    int width, height;
    int resizable;              /* 0/1                                       */
    const SLUITheme *theme;     /* NULL -> Slick Black default               */
    int custom_titlebar;        /* 1 -> SleelaUI draws its own title bar      */
} SLUIWindowConfig;

SLUIWindow *slui_window_create(SLUIApp *app, const SLUIWindowConfig *config);
void slui_window_destroy(SLUIWindow *window);
void slui_window_show(SLUIWindow *window);
void slui_window_hide(SLUIWindow *window);
void slui_window_set_title(SLUIWindow *window, const char *title);
SLUIWidget *slui_window_root(SLUIWindow *window);
const SLUITheme *slui_window_theme(const SLUIWindow *window);
void slui_window_set_theme(SLUIWindow *window, const SLUITheme *theme);
void slui_window_request_redraw(SLUIWindow *window);

/* A per-window event callback, called for events the widget tree did not fully
 * consume (e.g. window CLOSE). Return non-zero to mark the event handled. */
typedef int (*SLUIEventHandler)(const SLUIEvent *event, void *user);
void slui_window_set_event_handler(SLUIWindow *window, SLUIEventHandler handler,
                                   void *user);

/* --------------------------------------------------------------------------
 * Widgets. A compact but complete GNOME-grade control set: containers that lay
 * their children out, plus leaf controls. All constructors attach the new
 * widget to `parent` and return a borrowed handle (owned by the tree).
 * ------------------------------------------------------------------------- */
typedef enum {
    SLUI_ALIGN_START = 0,
    SLUI_ALIGN_CENTER = 1,
    SLUI_ALIGN_END = 2,
    SLUI_ALIGN_FILL = 3
} SLUIAlign;

typedef enum {
    SLUI_ORIENT_VERTICAL = 0,
    SLUI_ORIENT_HORIZONTAL = 1
} SLUIOrientation;

/* Containers. */
SLUIWidget *slui_box(SLUIWidget *parent, SLUIOrientation orient, int spacing);
SLUIWidget *slui_header_bar(SLUIWidget *parent); /* a title-bar strip         */

/* Leaf controls. */
SLUIWidget *slui_label(SLUIWidget *parent, const char *text);
SLUIWidget *slui_button(SLUIWidget *parent, const char *text);
SLUIWidget *slui_toggle(SLUIWidget *parent, const char *text, int initial_on);
SLUIWidget *slui_entry(SLUIWidget *parent, const char *placeholder);
SLUIWidget *slui_slider(SLUIWidget *parent, double min, double max, double value);
SLUIWidget *slui_separator(SLUIWidget *parent, SLUIOrientation orient);
SLUIWidget *slui_spacer(SLUIWidget *parent); /* an expanding flexible gap     */

/* --------------------------------------------------------------------------
 * Expanded widget collection. Every one is drawn by the same software
 * rasterizer and reads only theme roles, so each is pixel-identical on every
 * backend. Constructors attach to `parent` and return a borrowed handle.
 * ------------------------------------------------------------------------- */

/* Selection / boolean controls. */
SLUIWidget *slui_check_box(SLUIWidget *parent, const char *text, int on);
SLUIWidget *slui_radio_button(SLUIWidget *parent, const char *text, int group,
                              int on);
SLUIWidget *slui_combo_box(SLUIWidget *parent); /* add options, then show      */
void slui_combo_box_add(SLUIWidget *combo, const char *option);
SLUIWidget *slui_spin_button(SLUIWidget *parent, double min, double max,
                             double step, double value);

/* Indicators. */
SLUIWidget *slui_progress_bar(SLUIWidget *parent, double fraction); /* [0,1]   */
SLUIWidget *slui_level_bar(SLUIWidget *parent, double fraction);    /* [0,1]   */
SLUIWidget *slui_spinner(SLUIWidget *parent);  /* indeterminate activity       */
SLUIWidget *slui_scroll_bar(SLUIWidget *parent, SLUIOrientation orient,
                            double value, double page);

/* Containers / structure. */
SLUIWidget *slui_frame(SLUIWidget *parent, const char *title);
SLUIWidget *slui_card(SLUIWidget *parent);
SLUIWidget *slui_grid(SLUIWidget *parent, int columns, int spacing);
SLUIWidget *slui_status_bar(SLUIWidget *parent);

/* Display / ornament. */
SLUIWidget *slui_image(SLUIWidget *parent, const char *glyph, int w, int h);
SLUIWidget *slui_avatar(SLUIWidget *parent, const char *initial, int diameter);
SLUIWidget *slui_badge(SLUIWidget *parent, const char *text);
SLUIWidget *slui_chip(SLUIWidget *parent, const char *text);
SLUIWidget *slui_heading(SLUIWidget *parent, const char *text, int size_pt);
SLUIWidget *slui_tooltip(SLUIWidget *parent, const char *text);

/* Text input variants. */
SLUIWidget *slui_link_button(SLUIWidget *parent, const char *text);
SLUIWidget *slui_search_entry(SLUIWidget *parent, const char *placeholder);
SLUIWidget *slui_password_entry(SLUIWidget *parent, const char *placeholder);

/* Feedback. INFO=0, WARNING=1, ERROR=2. */
SLUIWidget *slui_info_bar(SLUIWidget *parent, const char *text, int severity);

/* --------------------------------------------------------------------------
 * Motion & custom drawing. These are animated widgets: while visible they are
 * advanced by the window's frame loop and repainted at their requested rate.
 * See sleela_ui_draw.h for the full developer draw surface a canvas view uses.
 * ------------------------------------------------------------------------- */

/* A width-adjustable, full-motion, colour-predictive, water-like flowing
 * activity throbber. `width_px` is the initial width; adjust it later with
 * slui_throbber_set_width. Height follows the widget's size request (a slim
 * seam by default). */
SLUIWidget *slui_throbber(SLUIWidget *parent, int width_px);
void slui_throbber_set_width(SLUIWidget *throbber, int width_px);
/* Flow vigour, 0 (calm trickle) .. 1 (vigorous). Drives speed + colour spread. */
void slui_throbber_set_intensity(SLUIWidget *throbber, double intensity);
/* Base hue in degrees the predictive colouring flows around (205 ~ water). */
void slui_throbber_set_hue(SLUIWidget *throbber, double hue_degrees);

/* A general animated drawing surface. Register a draw callback with
 * slui_canvas_view_set_draw; it receives the view's own double-buffered draw
 * context (see sleela_ui_draw.h), the animation time, and the per-frame delta,
 * each frame, and draws whatever it likes. The view presents + blits the
 * result into the window automatically. */
SLUIWidget *slui_canvas_view(SLUIWidget *parent, int width, int height);
typedef void (*SLUICanvasDrawFn)(SLUIDrawContext *dc, double time_s,
                                 double dt_s, void *user);
void slui_canvas_view_set_draw(SLUIWidget *view, SLUICanvasDrawFn fn,
                               void *user);
void slui_canvas_view_set_fps(SLUIWidget *view, double fps);

/* Common widget configuration. */
void slui_widget_set_margin(SLUIWidget *w, int top, int right, int bottom,
                            int left);
void slui_widget_set_align(SLUIWidget *w, SLUIAlign h, SLUIAlign v);
void slui_widget_set_expand(SLUIWidget *w, int h_expand, int v_expand);
void slui_widget_set_size_request(SLUIWidget *w, int min_w, int min_h);
void slui_widget_set_sensitive(SLUIWidget *w, int sensitive); /* grey out     */
void slui_widget_set_visible(SLUIWidget *w, int visible);

/* Text accessors (label/button/entry). text buffers are copied in; `out`/`cap`
 * read the current value and return the full length (even if truncated). */
void slui_widget_set_text(SLUIWidget *w, const char *text);
size_t slui_widget_get_text(const SLUIWidget *w, char *out, size_t cap);

/* State accessors. */
void slui_widget_set_toggle(SLUIWidget *w, int on);
int slui_widget_get_toggle(const SLUIWidget *w);
void slui_widget_set_value(SLUIWidget *w, double value); /* slider            */
double slui_widget_get_value(const SLUIWidget *w);

/* One-shot activation latch for polling hosts (e.g. the SLeeLa `ui*` bridge):
 * returns 1 at most once per activation and clears the latch. The callback path
 * (slui_widget_on_activate) still works independently. */
int slui_widget_take_activated(SLUIWidget *w);

/* Mark a button/entry as the single suggested (accent) action in its group. */
void slui_widget_set_suggested(SLUIWidget *w, int suggested);
/* Mark a button as destructive (uses the danger colour on hover). */
void slui_widget_set_destructive(SLUIWidget *w, int destructive);

/* Callbacks. `activate` fires on button click / entry Enter / toggle flip. */
typedef void (*SLUIActivateHandler)(SLUIWidget *w, void *user);
void slui_widget_on_activate(SLUIWidget *w, SLUIActivateHandler handler,
                             void *user);
typedef void (*SLUIValueHandler)(SLUIWidget *w, double value, void *user);
void slui_widget_on_value_changed(SLUIWidget *w, SLUIValueHandler handler,
                                  void *user);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_UI_H */
