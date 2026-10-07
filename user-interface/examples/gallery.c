/* =============================================================================
 * SleelaUI gallery -- a pure-C demonstration that the toolkit's public ABI is
 * complete and ergonomic from C (no C++ at the call site). It builds one Slick
 * Black window with a header bar and every built-in control, wires a couple of
 * callbacks, and runs the toolkit's own event loop.
 *
 * Build (Linux/Unix): make example   ->   build/slui-gallery
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"

#include <stdio.h>
#include <string.h>

static SLUIWidget* g_status;

static void on_click(SLUIWidget* w, void* user) {
    (void)w;
    (void)user;
    slui_widget_set_text(g_status, "Button activated.");
}

static void on_toggle(SLUIWidget* w, void* user) {
    (void)user;
    slui_widget_set_text(g_status,
                         slui_widget_get_toggle(w) ? "Dark mode: on"
                                                   : "Dark mode: off");
}

static void on_slider(SLUIWidget* w, double value, void* user) {
    (void)w;
    (void)user;
    char buf[64];
    snprintf(buf, sizeof(buf), "Opacity: %d%%", (int)(value + 0.5));
    slui_widget_set_text(g_status, buf);
}

static int on_window_event(const SLUIEvent* ev, void* user) {
    SLUIApp* app = (SLUIApp*)user;
    if (ev->type == SLUI_EVENT_CLOSE) {
        slui_app_quit(app, 0);
        return 1; /* handled */
    }
    return 0;
}

int main(void) {
    SLUIApp* app = slui_app_create("com.mearvk.SleelaUI.Gallery");
    if (!app) {
        fprintf(stderr,
                "SleelaUI: could not open a display. On Linux/Unix a running\n"
                "X server (DISPLAY) is required.\n");
        return 1;
    }

    printf("SleelaUI %s -- backend: %s\n", slui_version_string(),
           slui_backend_name(slui_backend()));

    SLUITheme theme;
    slui_theme_preset(&theme, SLUI_THEME_SLICK_BLACK); /* the default look */

    SLUIWindowConfig cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.title = "SleelaUI\xe2\x84\xa2 Gallery \xe2\x80\x94 MEARVK LLC";
    cfg.width = 520;
    cfg.height = 460;
    cfg.resizable = 1;
    cfg.theme = &theme;

    SLUIWindow* win = slui_window_create(app, &cfg);
    slui_window_set_event_handler(win, on_window_event, app);
    SLUIWidget* root = slui_window_root(win);

    /* Header bar with a brand label. */
    SLUIWidget* header = slui_header_bar(root);
    slui_widget_set_size_request(header, 0, 44);
    SLUIWidget* brand = slui_label(header, "SleelaUI\xe2\x84\xa2");
    slui_widget_set_margin(brand, 0, 0, 0, 12);
    slui_widget_set_align(brand, SLUI_ALIGN_START, SLUI_ALIGN_CENTER);
    slui_spacer(header);
    SLUIWidget* gear = slui_button(header, "Settings");
    slui_widget_set_margin(gear, 6, 12, 6, 0);

    /* Content column. */
    SLUIWidget* content = slui_box(root, SLUI_ORIENT_VERTICAL, 12);
    slui_widget_set_margin(content, 20, 20, 20, 20);
    slui_widget_set_expand(content, 1, 1);

    SLUIWidget* heading = slui_label(content, "Slick Black controls");
    slui_widget_set_align(heading, SLUI_ALIGN_START, SLUI_ALIGN_START);

    SLUIWidget* entry = slui_entry(content, "Type here\xe2\x80\xa6");
    slui_widget_set_expand(entry, 1, 0);

    SLUIWidget* row = slui_box(content, SLUI_ORIENT_HORIZONTAL, 8);
    SLUIWidget* ok = slui_button(row, "Save");
    slui_widget_set_suggested(ok, 1); /* the single accent action */
    slui_widget_on_activate(ok, on_click, NULL);
    SLUIWidget* cancel = slui_button(row, "Discard");
    slui_widget_set_destructive(cancel, 1);
    slui_spacer(row);

    SLUIWidget* toggle = slui_toggle(content, "Dark mode", 1);
    slui_widget_on_activate(toggle, on_toggle, NULL);

    SLUIWidget* slider = slui_slider(content, 0.0, 100.0, 72.0);
    slui_widget_set_expand(slider, 1, 0);
    slui_widget_on_value_changed(slider, on_slider, NULL);

    /* A few widgets from the expanded collection. */
    slui_check_box(content, "Remember me", 1);
    SLUIWidget* progress = slui_progress_bar(content, 0.62);
    slui_widget_set_expand(progress, 1, 0);

    SLUIWidget* tags = slui_box(content, SLUI_ORIENT_HORIZONTAL, 6);
    slui_chip(tags, "alpha");
    slui_chip(tags, "beta");
    slui_link_button(tags, "Learn more");
    slui_spacer(tags);
    slui_badge(tags, "24");

    slui_info_bar(content, "Welcome to the SleelaUI widget collection.", 0);

    slui_separator(content, SLUI_ORIENT_HORIZONTAL);

    SLUIWidget* disabled = slui_button(content, "Unavailable action");
    slui_widget_set_sensitive(disabled, 0);

    slui_spacer(content);

    g_status = slui_label(content, "Ready.");
    slui_widget_set_align(g_status, SLUI_ALIGN_START, SLUI_ALIGN_END);

    slui_window_show(win);
    int code = slui_app_run(app);
    slui_app_destroy(app);
    return code;
}
