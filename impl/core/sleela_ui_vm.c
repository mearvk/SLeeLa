/* ===========================================================================
 * sleela_ui_vm.c -- the ui* SLVM -> SleelaUI C ABI bridge.
 *
 * This is the live wire that connects the SLeeLa GUI object model
 * (/lib/user-interface SL* classes and /lib/gui decoration) to SleelaUI, the
 * project's OWN native C/C++ toolkit (user-interface/, slui_* C ABI, X11/Cocoa/
 * Win32 backends). The compiler lowers each ui* built-in to OP_UI_CALL<id>; the
 * VM pops the arguments and hands them here, and this file calls the genuine
 * slui_* functions, so a SLeeLa program raises a real host window.
 *
 * SleelaUI objects are pointers; SLeeLa handles are integers, so this bridge
 * keeps small handle tables (apps / windows / widgets / themes) and translates
 * int<->pointer at the boundary, the same discipline the VM uses for sockets,
 * files, and idea canvases.
 *
 * Arguments arrive as SLUIArg (a tiny tagged union the core fills from the
 * operand stack); the result is returned the same way so the core can PUSH it.
 * =========================================================================== */
#include "sleela_ui_vm.h"
#include "sleela_ui_ids.h"
#include "wifi_vm_bridge.h"
#include "ballum_vm_bridge.h"

#include "../../user-interface/include/sleela_ui.h"

/* Interaction polling exports from SleelaUI's C ABI. */
extern int slui_widget_is_hovered(SLUIWidget*);
extern int slui_widget_take_pressed(SLUIWidget*);
extern int slui_widget_take_released(SLUIWidget*);
extern int slui_widget_take_hover_entered(SLUIWidget*);
extern int slui_widget_take_hover_left(SLUIWidget*);

#include <stdlib.h>
#include <string.h>

/* ---- handle tables: int handle <-> SleelaUI pointer ---------------------- */
#define SLUI_VM_MAX_APPS    8
#define SLUI_VM_MAX_WINDOWS 64
#define SLUI_VM_MAX_WIDGETS 8192
#define SLUI_VM_MAX_THEMES  64

static SLUIApp*    g_apps[SLUI_VM_MAX_APPS];
static SLUIWindow* g_windows[SLUI_VM_MAX_WINDOWS];
static SLUIWidget* g_widgets[SLUI_VM_MAX_WIDGETS];
static SLUITheme*  g_themes[SLUI_VM_MAX_THEMES];
static int g_nwidgets; /* widgets are append-only within a process run */

static int app_put(SLUIApp* p) {
    if (!p) return -1;
    for (int i = 0; i < SLUI_VM_MAX_APPS; i++) if (!g_apps[i]) { g_apps[i] = p; return i; }
    return -1;
}
static SLUIApp* app_get(long h) {
    return (h >= 0 && h < SLUI_VM_MAX_APPS) ? g_apps[(int)h] : NULL;
}
static int win_put(SLUIWindow* p) {
    if (!p) return -1;
    for (int i = 0; i < SLUI_VM_MAX_WINDOWS; i++) if (!g_windows[i]) { g_windows[i] = p; return i; }
    return -1;
}
static SLUIWindow* win_get(long h) {
    return (h >= 0 && h < SLUI_VM_MAX_WINDOWS) ? g_windows[(int)h] : NULL;
}
static int widget_put(SLUIWidget* p) {
    if (!p) return -1;
    /* Reuse an existing handle if the toolkit returned a pointer we already
     * know (window roots are stable), else append. */
    for (int i = 0; i < g_nwidgets; i++) if (g_widgets[i] == p) return i;
    if (g_nwidgets >= SLUI_VM_MAX_WIDGETS) return -1;
    g_widgets[g_nwidgets] = p; return g_nwidgets++;
}
static SLUIWidget* widget_get(long h) {
    return (h >= 0 && h < g_nwidgets) ? g_widgets[(int)h] : NULL;
}
static int theme_put(SLUITheme* p) {
    if (!p) return -1;
    for (int i = 0; i < SLUI_VM_MAX_THEMES; i++) if (!g_themes[i]) { g_themes[i] = p; return i; }
    return -1;
}
static SLUITheme* theme_get(long h) {
    return (h >= 0 && h < SLUI_VM_MAX_THEMES) ? g_themes[(int)h] : NULL;
}

/* ---- argument / result helpers ------------------------------------------- */
static long ai(const SLUIArg* a, int n, int k) { return (k < n && a[k].kind != SLUI_ARG_STR) ? a[k].i : 0; }
static double ad(const SLUIArg* a, int n, int k) {
    if (k >= n) return 0.0;
    return a[k].kind == SLUI_ARG_DOUBLE ? a[k].d : (double)a[k].i;
}
static const char* as_(const SLUIArg* a, int n, int k) {
    return (k < n && a[k].kind == SLUI_ARG_STR && a[k].s) ? a[k].s : "";
}
static SLUIArg r_int(long v)    { SLUIArg r; r.kind = SLUI_ARG_INT; r.i = v; r.d = 0; r.s = NULL; return r; }
static SLUIArg r_dbl(double v)  { SLUIArg r; r.kind = SLUI_ARG_DOUBLE; r.d = v; r.i = 0; r.s = NULL; return r; }
static SLUIArg r_str(const char* v) { SLUIArg r; r.kind = SLUI_ARG_STR; r.s = v; r.i = 0; r.d = 0; return r; }
static SLUIArg r_null(void)     { SLUIArg r; r.kind = SLUI_ARG_INT; r.i = 0; r.d = 0; r.s = NULL; return r; }

/* The SLeeLa-facing arity per id (how many values the core pops). These match
 * the ui* call signatures in /lib/user-interface exactly. */
int sleela_ui_arity(int id) {
    switch (id) {
        /* 0 args */
        case SLUI_FN_BACKEND: case SLUI_FN_VERSION: case SLUI_FN_WIFI_OPEN_SETTINGS:
        case SLUI_FN_WIFI_SCAN: case SLUI_FN_WIFI_NETWORK_COUNT: case SLUI_FN_WIFI_DISCONNECT:
        case SLUI_FN_BALLUM_SUPPORTED: case SLUI_FN_BALLUM_BACKEND_NAME:
        case SLUI_FN_BALLUM_ENUMERATE: case SLUI_FN_BALLUM_DISPLAY_COUNT:
        case SLUI_FN_BALLUM_HAS_INPUT: case SLUI_FN_BALLUM_POLL:
        case SLUI_FN_BALLUM_EVENT_X: case SLUI_FN_BALLUM_EVENT_Y:
        case SLUI_FN_BALLUM_EVENT_BUTTON: case SLUI_FN_BALLUM_EVENT_SOURCE:
        case SLUI_FN_BALLUM_EVENT_CONTACT: case SLUI_FN_BALLUM_EVENT_MODIFIERS:
        case SLUI_FN_BALLUM_HAS_KEYBOARD: case SLUI_FN_BALLUM_POLL_KEY:
        case SLUI_FN_BALLUM_LOGO_READY: case SLUI_FN_BALLUM_FX_COUNT:
            return 0;
        /* 1 arg */
        case SLUI_FN_APP_CREATE: case SLUI_FN_APP_RUN: case SLUI_FN_APP_DESTROY:
        case SLUI_FN_BACKEND_NAME: case SLUI_FN_WINDOW_SHOW: case SLUI_FN_WINDOW_HIDE:
        case SLUI_FN_WINDOW_ROOT: case SLUI_FN_WINDOW_REDRAW: case SLUI_FN_CARD:
        case SLUI_FN_HEADER_BAR: case SLUI_FN_STATUS_BAR: case SLUI_FN_SPACER:
        case SLUI_FN_SPINNER: case SLUI_FN_COMBO_BOX: case SLUI_FN_THEME_PRESET:
        case SLUI_FN_THEME_DESTROY: case SLUI_FN_WIDGET_GET_TEXT:
        case SLUI_FN_WIDGET_GET_TOGGLE: case SLUI_FN_WIDGET_GET_VALUE:
        case SLUI_FN_WIDGET_TAKE_ACTIVATED:
        case SLUI_FN_WIDGET_IS_HOVERED: case SLUI_FN_WIDGET_TAKE_PRESSED:
        case SLUI_FN_WIDGET_TAKE_RELEASED: case SLUI_FN_WIDGET_TAKE_HOVER_ENTERED:
        case SLUI_FN_WIDGET_TAKE_HOVER_LEFT:
        case SLUI_FN_SCREEN_WIDTH: case SLUI_FN_SCREEN_HEIGHT:
        case SLUI_FN_SCROLLED_SCROLL_X: case SLUI_FN_SCROLLED_SCROLL_Y:
        case SLUI_FN_POPUP_CREATE: case SLUI_FN_POPUP_IS_OPEN:
        case SLUI_FN_WIFI_NETWORK_SSID: case SLUI_FN_WIFI_NETWORK_BSSID:
        case SLUI_FN_WIFI_NETWORK_SIGNAL: case SLUI_FN_WIFI_NETWORK_SECURITY:
        case SLUI_FN_WIFI_STATUS_NAME:
        case SLUI_FN_WINDOW_BUFFERING:
        case SLUI_FN_WINDOW_MIN_WIDTH: case SLUI_FN_WINDOW_MIN_HEIGHT:
        case SLUI_FN_WINDOW_MAX_WIDTH: case SLUI_FN_WINDOW_MAX_HEIGHT:
        case SLUI_FN_BALLUM_DISPLAY_ID: case SLUI_FN_BALLUM_DISPLAY_NAME:
        case SLUI_FN_BALLUM_DISPLAY_WIDTH: case SLUI_FN_BALLUM_DISPLAY_HEIGHT:
        case SLUI_FN_BALLUM_DISPLAY_PRIMARY: case SLUI_FN_BALLUM_DISPLAY_CAPS:
        case SLUI_FN_BALLUM_STATUS_NAME: case SLUI_FN_BALLUM_EVENT_TYPE_NAME:
        case SLUI_FN_BALLUM_LOGO_LOAD: case SLUI_FN_BALLUM_LOGO_SET_EFFECT:
        case SLUI_FN_BALLUM_FX_NAME: case SLUI_FN_BALLUM_FX_BY_NAME:
            return 1;
        /* 2 args */
        case SLUI_FN_APP_PUMP: case SLUI_FN_APP_QUIT: case SLUI_FN_WINDOW_SET_TITLE:
        case SLUI_FN_WINDOW_SET_MIN_SIZE: case SLUI_FN_WINDOW_SET_MAX_SIZE:
        case SLUI_FN_WINDOW_SET_BUFFERING:
        case SLUI_FN_WINDOW_SET_THEME: case SLUI_FN_FRAME: case SLUI_FN_SEPARATOR:
        case SLUI_FN_BUTTON: case SLUI_FN_LINK_BUTTON: case SLUI_FN_LABEL:
        case SLUI_FN_BADGE: case SLUI_FN_CHIP: case SLUI_FN_TOOLTIP:
        case SLUI_FN_ENTRY: case SLUI_FN_SEARCH_ENTRY: case SLUI_FN_PASSWORD_ENTRY:
        case SLUI_FN_COMBO_BOX_ADD: case SLUI_FN_PROGRESS_BAR: case SLUI_FN_LEVEL_BAR:
        case SLUI_FN_THROBBER: case SLUI_FN_THROBBER_SET_WIDTH:
        case SLUI_FN_THROBBER_SET_INTENSITY: case SLUI_FN_THROBBER_SET_HUE:
        case SLUI_FN_WIDGET_SET_TEXT: case SLUI_FN_WIDGET_SET_TOGGLE:
        case SLUI_FN_WIDGET_SET_VALUE: case SLUI_FN_WIDGET_SET_SENSITIVE:
        case SLUI_FN_WIDGET_SET_VISIBLE: case SLUI_FN_POPUP_SET_OPEN: case SLUI_FN_WIFI_CONNECT:
        case SLUI_FN_WIDGET_SET_SUGGESTED:
        case SLUI_FN_WIDGET_SET_DESTRUCT:
        case SLUI_FN_THEME_LOAD_CONFIG: case SLUI_FN_THEME_SET_BASE:
        case SLUI_FN_THEME_SET_BG: case SLUI_FN_THEME_SET_SURFACE:
        case SLUI_FN_THEME_SET_SURFACE_HI: case SLUI_FN_THEME_SET_CHROME:
        case SLUI_FN_THEME_SET_BORDER: case SLUI_FN_THEME_SET_FG:
        case SLUI_FN_THEME_SET_FG_DIM: case SLUI_FN_THEME_SET_ACCENT:
        case SLUI_FN_THEME_SET_ACCENT_FG: case SLUI_FN_THEME_SET_DANGER:
        case SLUI_FN_THEME_SET_RADIUS: case SLUI_FN_THEME_SET_CONTROL_H:
        case SLUI_FN_BALLUM_SET_BRIGHTNESS: case SLUI_FN_BALLUM_SET_WARMTH:
        case SLUI_FN_BALLUM_SET_GAMMA:
            return 2;
        /* 3 args */
        case SLUI_FN_BOX: case SLUI_FN_GRID: case SLUI_FN_TOGGLE:
        case SLUI_FN_CHECK_BOX: case SLUI_FN_HEADING: case SLUI_FN_INFO_BAR:
        case SLUI_FN_WIDGET_SET_ALIGN: case SLUI_FN_WIDGET_SET_EXPAND:
        case SLUI_FN_WIDGET_SET_SIZE_REQ: case SLUI_FN_AVATAR:
        case SLUI_FN_THEME_SET_FONT:
        case SLUI_FN_SCROLLED_VIEW:
        case SLUI_FN_SCROLLED_SCROLL_TO: case SLUI_FN_SCROLLED_SCROLL_BY:
        case SLUI_FN_WIDGET_SET_ICON:
        case SLUI_FN_BALLUM_GET_PIXEL:
            return 3;
        /* 4 args */
        case SLUI_FN_IMAGE: case SLUI_FN_RADIO_BUTTON: case SLUI_FN_SCROLL_BAR:
        case SLUI_FN_SLIDER:
            return 4;
        /* 5 args */
        case SLUI_FN_WINDOW_CREATE: case SLUI_FN_SPIN_BUTTON:
        case SLUI_FN_WINDOW_SET_SIZE_LIMITS:
        case SLUI_FN_WIDGET_SET_MARGIN:
            return 5;
        /* 7 args */
        case SLUI_FN_BALLUM_SET_PIXEL:
        case SLUI_FN_BALLUM_LOGO_BLEND:
            return 7;
        default: return 0;
    }
}

/* Dispatch one ui* call. Args are popped by the core (left-to-right in a[]).
 * Returns the result as an SLUIArg; the core PUSHes it. */
SLUIArg sleela_ui_dispatch(int id, const SLUIArg* a, int n) {
    switch (id) {
        /* --- application / process --- */
        case SLUI_FN_APP_CREATE:  return r_int(app_put(slui_app_create(as_(a,n,0))));
        case SLUI_FN_APP_RUN:     return r_int(slui_app_run(app_get(ai(a,n,0))));
        case SLUI_FN_APP_PUMP:    return r_int(slui_app_pump(app_get(ai(a,n,0)), (int)ai(a,n,1)));
        case SLUI_FN_APP_QUIT:    slui_app_quit(app_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_APP_DESTROY: { int h=(int)ai(a,n,0); SLUIApp* p=app_get(h); if(p){slui_app_destroy(p); if(h>=0&&h<SLUI_VM_MAX_APPS)g_apps[h]=NULL;} return r_null(); }
        case SLUI_FN_BACKEND:     return r_int((long)slui_backend());
        case SLUI_FN_BACKEND_NAME:return r_str(slui_backend_name((SLUIBackend)ai(a,n,0)));
        case SLUI_FN_VERSION:     return r_str(slui_version_string());

        /* --- screen / display geometry (0 when the backend cannot report) --- */
        case SLUI_FN_SCREEN_WIDTH:  { int w=0,h=0; return r_int(slui_screen_size(app_get(ai(a,n,0)),&w,&h)==SLUI_OK ? w : 0); }
        case SLUI_FN_SCREEN_HEIGHT: { int w=0,h=0; return r_int(slui_screen_size(app_get(ai(a,n,0)),&w,&h)==SLUI_OK ? h : 0); }

        /* --- window --- */
        case SLUI_FN_WINDOW_CREATE: {
            SLUIApp* app = app_get(ai(a,n,0));
            SLUITheme* th = theme_get(ai(a,n,4));
            SLUIWindowConfig cfg; memset(&cfg,0,sizeof(cfg));
            cfg.title = as_(a,n,1); cfg.width = (int)ai(a,n,2); cfg.height = (int)ai(a,n,3);
            cfg.resizable = 1; cfg.theme = th;
            SLUIWindow* created = slui_window_create(app, &cfg);
            /* SLeeLa-created GUI windows default to triple buffering. */
            if (created) slui_window_set_buffering(created, SLUI_BUFFERING_TRIPLE);
            return r_int(win_put(created));
        }
        case SLUI_FN_WINDOW_SHOW:   slui_window_show(win_get(ai(a,n,0))); return r_null();
        case SLUI_FN_WINDOW_HIDE:   slui_window_hide(win_get(ai(a,n,0))); return r_null();
        case SLUI_FN_WINDOW_ROOT:   return r_int(widget_put(slui_window_root(win_get(ai(a,n,0)))));
        case SLUI_FN_WINDOW_SET_TITLE: slui_window_set_title(win_get(ai(a,n,0)), as_(a,n,1)); return r_null();
        case SLUI_FN_WINDOW_SET_MIN_SIZE:
            return r_int(slui_window_set_min_size(win_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2)));
        case SLUI_FN_WINDOW_SET_MAX_SIZE:
            return r_int(slui_window_set_max_size(win_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2)));
        case SLUI_FN_WINDOW_SET_SIZE_LIMITS:
            return r_int(slui_window_set_size_limits(win_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2), (int)ai(a,n,3), (int)ai(a,n,4)));
        case SLUI_FN_WINDOW_MIN_WIDTH: case SLUI_FN_WINDOW_MIN_HEIGHT:
        case SLUI_FN_WINDOW_MAX_WIDTH: case SLUI_FN_WINDOW_MAX_HEIGHT: {
            SLUISize size = {0, 0};
            SLUIWindow* window = win_get(ai(a,n,0));
            int status;
            if (id == SLUI_FN_WINDOW_MIN_WIDTH || id == SLUI_FN_WINDOW_MIN_HEIGHT)
                status = slui_window_get_min_size(window, &size);
            else
                status = slui_window_get_max_size(window, &size);
            if (status != SLUI_OK) return r_int(-1);
            if (id == SLUI_FN_WINDOW_MIN_WIDTH || id == SLUI_FN_WINDOW_MAX_WIDTH) return r_int(size.w);
            return r_int(size.h);
        }
        case SLUI_FN_WINDOW_SET_BUFFERING: slui_window_set_buffering(win_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_WINDOW_BUFFERING: return r_int(slui_window_buffering(win_get(ai(a,n,0))));
        case SLUI_FN_WINDOW_SET_THEME: slui_window_set_theme(win_get(ai(a,n,0)), theme_get(ai(a,n,1))); return r_null();
        case SLUI_FN_WINDOW_REDRAW:  slui_window_request_redraw(win_get(ai(a,n,0))); return r_null();

        /* --- containers --- */
        case SLUI_FN_BOX:        return r_int(widget_put(slui_box(widget_get(ai(a,n,0)), (SLUIOrientation)ai(a,n,1), (int)ai(a,n,2))));
        case SLUI_FN_GRID:       return r_int(widget_put(slui_grid(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2))));
        case SLUI_FN_FRAME:      return r_int(widget_put(slui_frame(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_CARD:       return r_int(widget_put(slui_card(widget_get(ai(a,n,0)))));
        case SLUI_FN_HEADER_BAR: return r_int(widget_put(slui_header_bar(widget_get(ai(a,n,0)))));
        case SLUI_FN_STATUS_BAR: return r_int(widget_put(slui_status_bar(widget_get(ai(a,n,0)))));
        case SLUI_FN_SEPARATOR:  return r_int(widget_put(slui_separator(widget_get(ai(a,n,0)), (SLUIOrientation)ai(a,n,1))));
        case SLUI_FN_SPACER:     return r_int(widget_put(slui_spacer(widget_get(ai(a,n,0)))));
        case SLUI_FN_SCROLLED_VIEW: return r_int(widget_put(slui_scrolled_view(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2))));
        case SLUI_FN_SCROLLED_SCROLL_TO: slui_scrolled_view_scroll_to(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2)); return r_int(0);
        case SLUI_FN_SCROLLED_SCROLL_BY: slui_scrolled_view_scroll_by(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2)); return r_int(0);
        case SLUI_FN_SCROLLED_SCROLL_X:  return r_int(slui_scrolled_view_scroll_x(widget_get(ai(a,n,0))));
        case SLUI_FN_SCROLLED_SCROLL_Y:  return r_int(slui_scrolled_view_scroll_y(widget_get(ai(a,n,0))));

        /* --- buttons / actions --- */
        case SLUI_FN_BUTTON:      return r_int(widget_put(slui_button(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_LINK_BUTTON: return r_int(widget_put(slui_link_button(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_TOGGLE:      return r_int(widget_put(slui_toggle(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2))));

        /* --- selection / boolean --- */
        case SLUI_FN_CHECK_BOX:    return r_int(widget_put(slui_check_box(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2))));
        case SLUI_FN_RADIO_BUTTON: return r_int(widget_put(slui_radio_button(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2), (int)ai(a,n,3))));
        case SLUI_FN_COMBO_BOX:    return r_int(widget_put(slui_combo_box(widget_get(ai(a,n,0)))));
        case SLUI_FN_COMBO_BOX_ADD:slui_combo_box_add(widget_get(ai(a,n,0)), as_(a,n,1)); return r_null();
        case SLUI_FN_SPIN_BUTTON:  return r_int(widget_put(slui_spin_button(widget_get(ai(a,n,0)), ad(a,n,1), ad(a,n,2), ad(a,n,3), ad(a,n,4))));

        /* --- text input --- */
        case SLUI_FN_ENTRY:          return r_int(widget_put(slui_entry(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_SEARCH_ENTRY:   return r_int(widget_put(slui_search_entry(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_PASSWORD_ENTRY: return r_int(widget_put(slui_password_entry(widget_get(ai(a,n,0)), as_(a,n,1))));

        /* --- ranges / indicators --- */
        case SLUI_FN_SLIDER:       return r_int(widget_put(slui_slider(widget_get(ai(a,n,0)), ad(a,n,1), ad(a,n,2), ad(a,n,3))));
        case SLUI_FN_SCROLL_BAR:   return r_int(widget_put(slui_scroll_bar(widget_get(ai(a,n,0)), (SLUIOrientation)ai(a,n,1), ad(a,n,2), ad(a,n,3))));
        case SLUI_FN_PROGRESS_BAR: return r_int(widget_put(slui_progress_bar(widget_get(ai(a,n,0)), ad(a,n,1))));
        case SLUI_FN_LEVEL_BAR:    return r_int(widget_put(slui_level_bar(widget_get(ai(a,n,0)), ad(a,n,1))));
        case SLUI_FN_SPINNER:      return r_int(widget_put(slui_spinner(widget_get(ai(a,n,0)))));

        /* --- text / display --- */
        case SLUI_FN_LABEL:   return r_int(widget_put(slui_label(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_HEADING: return r_int(widget_put(slui_heading(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2))));
        case SLUI_FN_IMAGE:   return r_int(widget_put(slui_image(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2), (int)ai(a,n,3))));
        case SLUI_FN_AVATAR:  return r_int(widget_put(slui_avatar(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2))));

        /* --- tags / feedback --- */
        case SLUI_FN_BADGE:    return r_int(widget_put(slui_badge(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_CHIP:     return r_int(widget_put(slui_chip(widget_get(ai(a,n,0)), as_(a,n,1))));
        case SLUI_FN_INFO_BAR: return r_int(widget_put(slui_info_bar(widget_get(ai(a,n,0)), as_(a,n,1), (int)ai(a,n,2))));
        case SLUI_FN_TOOLTIP:  return r_int(widget_put(slui_tooltip(widget_get(ai(a,n,0)), as_(a,n,1))));

        /* --- motion / drawing --- */
        case SLUI_FN_THROBBER:               return r_int(widget_put(slui_throbber(widget_get(ai(a,n,0)), (int)ai(a,n,1))));
        case SLUI_FN_THROBBER_SET_WIDTH:     slui_throbber_set_width(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_THROBBER_SET_INTENSITY: slui_throbber_set_intensity(widget_get(ai(a,n,0)), ad(a,n,1)); return r_null();
        case SLUI_FN_THROBBER_SET_HUE:       slui_throbber_set_hue(widget_get(ai(a,n,0)), ad(a,n,1)); return r_null();

        /* --- widget setters --- */
        case SLUI_FN_WIDGET_SET_MARGIN:    slui_widget_set_margin(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2), (int)ai(a,n,3), (int)ai(a,n,4)); return r_null();
        case SLUI_FN_WIDGET_SET_ALIGN:     slui_widget_set_align(widget_get(ai(a,n,0)), (SLUIAlign)ai(a,n,1), (SLUIAlign)ai(a,n,2)); return r_null();
        case SLUI_FN_WIDGET_SET_EXPAND:    slui_widget_set_expand(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2)); return r_null();
        case SLUI_FN_WIDGET_SET_SIZE_REQ:  slui_widget_set_size_request(widget_get(ai(a,n,0)), (int)ai(a,n,1), (int)ai(a,n,2)); return r_null();
        case SLUI_FN_WIDGET_SET_SENSITIVE: slui_widget_set_sensitive(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_WIDGET_SET_VISIBLE:   slui_widget_set_visible(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_WIDGET_SET_SUGGESTED: slui_widget_set_suggested(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_WIDGET_SET_DESTRUCT:  slui_widget_set_destructive(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_WIDGET_SET_TEXT:      slui_widget_set_text(widget_get(ai(a,n,0)), as_(a,n,1)); return r_null();
        case SLUI_FN_WIDGET_SET_ICON:      slui_widget_set_icon(widget_get(ai(a,n,0)), as_(a,n,1), as_(a,n,2)); return r_null();
        case SLUI_FN_POPUP_CREATE:        return r_int(widget_put(slui_popup_create(widget_get(ai(a,n,0)))));
        case SLUI_FN_POPUP_SET_OPEN:       slui_popup_set_open(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_POPUP_IS_OPEN:        return r_int(slui_popup_is_open(widget_get(ai(a,n,0))));
        case SLUI_FN_WIFI_OPEN_SETTINGS:    return r_int(slui_wifi_open_settings());
        case SLUI_FN_WIFI_SCAN: return r_int(sleela_wifi_vm_scan());
        case SLUI_FN_WIFI_NETWORK_COUNT: return r_int(sleela_wifi_vm_network_count());
        case SLUI_FN_WIFI_NETWORK_SSID: return r_str(sleela_wifi_vm_ssid((int)ai(a,n,0)));
        case SLUI_FN_WIFI_NETWORK_BSSID: return r_str(sleela_wifi_vm_bssid((int)ai(a,n,0)));
        case SLUI_FN_WIFI_NETWORK_SIGNAL: return r_int(sleela_wifi_vm_signal((int)ai(a,n,0)));
        case SLUI_FN_WIFI_NETWORK_SECURITY: return r_str(sleela_wifi_vm_security((int)ai(a,n,0)));
        case SLUI_FN_WIFI_CONNECT: return r_int(sleela_wifi_vm_connect((int)ai(a,n,0), as_(a,n,1)));
        case SLUI_FN_WIFI_DISCONNECT: return r_int(sleela_wifi_vm_disconnect());
        case SLUI_FN_WIFI_STATUS_NAME: return r_str(sleela_wifi_vm_status_name((int)ai(a,n,0)));

        /* --- Ballum: pixel-level display control --- */
        case SLUI_FN_BALLUM_SUPPORTED:       return r_int(sleela_ballum_vm_supported());
        case SLUI_FN_BALLUM_BACKEND_NAME:    return r_str(sleela_ballum_vm_backend_name());
        case SLUI_FN_BALLUM_ENUMERATE:       return r_int(sleela_ballum_vm_enumerate());
        case SLUI_FN_BALLUM_DISPLAY_COUNT:   return r_int(sleela_ballum_vm_display_count());
        case SLUI_FN_BALLUM_DISPLAY_ID:      return r_int(sleela_ballum_vm_display_id((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_DISPLAY_NAME:    return r_str(sleela_ballum_vm_display_name((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_DISPLAY_WIDTH:   return r_int(sleela_ballum_vm_display_width((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_DISPLAY_HEIGHT:  return r_int(sleela_ballum_vm_display_height((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_DISPLAY_PRIMARY: return r_int(sleela_ballum_vm_display_is_primary((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_DISPLAY_CAPS:    return r_int(sleela_ballum_vm_display_caps((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_SET_PIXEL:       return r_int(sleela_ballum_vm_set_pixel(
                                                 (int)ai(a,n,0), (int)ai(a,n,1), (int)ai(a,n,2),
                                                 (int)ai(a,n,3), (int)ai(a,n,4), (int)ai(a,n,5), (int)ai(a,n,6)));
        case SLUI_FN_BALLUM_GET_PIXEL:       return r_int(sleela_ballum_vm_get_pixel(
                                                 (int)ai(a,n,0), (int)ai(a,n,1), (int)ai(a,n,2)));
        case SLUI_FN_BALLUM_SET_BRIGHTNESS:  return r_int(sleela_ballum_vm_set_brightness((int)ai(a,n,0), (int)ai(a,n,1)));
        case SLUI_FN_BALLUM_SET_WARMTH:      return r_int(sleela_ballum_vm_set_warmth((int)ai(a,n,0), (int)ai(a,n,1)));
        case SLUI_FN_BALLUM_SET_GAMMA:       return r_int(sleela_ballum_vm_set_gamma((int)ai(a,n,0), (int)ai(a,n,1)));
        case SLUI_FN_BALLUM_STATUS_NAME:     return r_str(sleela_ballum_vm_status_name((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_HAS_INPUT:       return r_int(sleela_ballum_vm_has_input());
        case SLUI_FN_BALLUM_POLL:            return r_int(sleela_ballum_vm_poll());
        case SLUI_FN_BALLUM_EVENT_X:         return r_int(sleela_ballum_vm_event_x());
        case SLUI_FN_BALLUM_EVENT_Y:         return r_int(sleela_ballum_vm_event_y());
        case SLUI_FN_BALLUM_EVENT_BUTTON:    return r_int(sleela_ballum_vm_event_button());
        case SLUI_FN_BALLUM_EVENT_SOURCE:    return r_int(sleela_ballum_vm_event_source());
        case SLUI_FN_BALLUM_EVENT_CONTACT:   return r_int(sleela_ballum_vm_event_contact());
        case SLUI_FN_BALLUM_EVENT_MODIFIERS: return r_int(sleela_ballum_vm_event_modifiers());
        case SLUI_FN_BALLUM_EVENT_TYPE_NAME: return r_str(sleela_ballum_vm_event_type_name((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_HAS_KEYBOARD:    return r_int(sleela_ballum_vm_has_keyboard());
        case SLUI_FN_BALLUM_POLL_KEY:        return r_int(sleela_ballum_vm_poll_key());
        case SLUI_FN_BALLUM_LOGO_READY:      return r_int(sleela_ballum_vm_logo_ready());
        case SLUI_FN_BALLUM_LOGO_BLEND:      return r_int(sleela_ballum_vm_logo_blend(
                                                 (int)ai(a,n,0), (int)ai(a,n,1), (int)ai(a,n,2),
                                                 (int)ai(a,n,3), (int)ai(a,n,4), (int)ai(a,n,5), (int)ai(a,n,6)));
        case SLUI_FN_BALLUM_LOGO_LOAD:       return r_int(sleela_ballum_vm_logo_load(as_(a,n,0)));
        case SLUI_FN_BALLUM_LOGO_SET_EFFECT: return r_int(sleela_ballum_vm_logo_set_effect((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_FX_COUNT:        return r_int(sleela_ballum_vm_fx_count());
        case SLUI_FN_BALLUM_FX_NAME:         return r_str(sleela_ballum_vm_fx_name((int)ai(a,n,0)));
        case SLUI_FN_BALLUM_FX_BY_NAME:      return r_int(sleela_ballum_vm_fx_by_name(as_(a,n,0)));

        case SLUI_FN_WIDGET_SET_TOGGLE:    slui_widget_set_toggle(widget_get(ai(a,n,0)), (int)ai(a,n,1)); return r_null();
        case SLUI_FN_WIDGET_SET_VALUE:     slui_widget_set_value(widget_get(ai(a,n,0)), ad(a,n,1)); return r_null();

        /* --- widget getters --- */
        case SLUI_FN_WIDGET_GET_TEXT: {
            static char buf[1024];
            buf[0] = '\0';
            slui_widget_get_text(widget_get(ai(a,n,0)), buf, sizeof(buf));
            return r_str(buf);
        }
        case SLUI_FN_WIDGET_GET_TOGGLE:    return r_int(slui_widget_get_toggle(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_GET_VALUE:     return r_dbl(slui_widget_get_value(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_TAKE_ACTIVATED:return r_int(slui_widget_take_activated(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_IS_HOVERED:return r_int(slui_widget_is_hovered(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_TAKE_PRESSED:return r_int(slui_widget_take_pressed(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_TAKE_RELEASED:return r_int(slui_widget_take_released(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_TAKE_HOVER_ENTERED:return r_int(slui_widget_take_hover_entered(widget_get(ai(a,n,0))));
        case SLUI_FN_WIDGET_TAKE_HOVER_LEFT:return r_int(slui_widget_take_hover_left(widget_get(ai(a,n,0))));

        /* --- theme --- */
        case SLUI_FN_THEME_PRESET: {
            SLUITheme* t = (SLUITheme*)calloc(1, sizeof(SLUITheme));
            if (t) slui_theme_preset(t, (SLUIThemeId)ai(a,n,0));
            return r_int(theme_put(t));
        }
        case SLUI_FN_THEME_DESTROY: { int h=(int)ai(a,n,0); SLUITheme* t=theme_get(h); if(t){free(t); if(h>=0&&h<SLUI_VM_MAX_THEMES)g_themes[h]=NULL;} return r_null(); }
        case SLUI_FN_THEME_LOAD_CONFIG: { SLUITheme* t=theme_get(ai(a,n,0)); return r_int(t?(long)slui_theme_load_config(t, as_(a,n,1)):-1); }
        case SLUI_FN_THEME_SET_BASE:       { SLUITheme* t=theme_get(ai(a,n,0)); if(t) slui_theme_set_base_color(t,(SLUIColor)ai(a,n,1)); return r_null(); }
        case SLUI_FN_THEME_SET_BG:         { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->bg=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_SURFACE:    { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->surface=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_SURFACE_HI: { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->surface_hi=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_CHROME:     { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->chrome=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_BORDER:     { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->border=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_FG:         { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->fg=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_FG_DIM:     { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->fg_dim=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_ACCENT:     { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->accent=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_ACCENT_FG:  { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->accent_fg=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_DANGER:     { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->danger=(SLUIColor)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_RADIUS:     { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->radius=(int)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_CONTROL_H:  { SLUITheme* t=theme_get(ai(a,n,0)); if(t) t->control_height=(int)ai(a,n,1); return r_null(); }
        case SLUI_FN_THEME_SET_FONT:       { SLUITheme* t=theme_get(ai(a,n,0)); if(t){ const char* fam=as_(a,n,1); strncpy(t->font_family,fam,sizeof(t->font_family)-1); t->font_family[sizeof(t->font_family)-1]='\0'; t->font_size=(int)ai(a,n,2);} return r_null(); }

        default: return r_null();
    }
}
