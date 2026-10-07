/* =============================================================================
 * SleelaUI public C ABI implementation.
 *
 * This file is the entire surface the installed header promises: it owns the
 * App/Window lifecycle, builds widgets into the tree, and marshals the flat C
 * configuration calls onto the C++ objects. Everything here has C linkage.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"

#include "slui_backend.hpp"
#include "slui_theme.hpp"
#include "slui_widget.hpp"
#include "slui_window.hpp"

#include <cstring>
#include <memory>
#include <string>
#include <vector>

/* ---- App ----------------------------------------------------------------- */
struct SLUIApp {
    std::unique_ptr<slui::Backend> backend;
    std::vector<std::unique_ptr<slui::Window>> windows;
    std::string app_id;
};

/* The public SLUIWindow / SLUIWidget are just aliases for the C++ objects; we
 * reinterpret_cast between them at the boundary. */

static slui::Window* as_window(SLUIWindow* w) {
    return reinterpret_cast<slui::Window*>(w);
}
static slui::Widget* as_widget(SLUIWidget* w) {
    return reinterpret_cast<slui::Widget*>(w);
}
static const slui::Widget* as_widget(const SLUIWidget* w) {
    return reinterpret_cast<const slui::Widget*>(w);
}

extern "C" {

const char* slui_version_string(void) { return SLUI_VERSION_STRING; }

SLUIBackend slui_backend(void) {
#if defined(SLUI_BACKEND_WIN32_BUILD)
    return SLUI_BACKEND_WIN32;
#elif defined(SLUI_BACKEND_COCOA_BUILD)
    return SLUI_BACKEND_COCOA;
#elif defined(SLUI_BACKEND_X11_BUILD)
    return SLUI_BACKEND_X11;
#else
    return SLUI_BACKEND_UNKNOWN;
#endif
}

const char* slui_backend_name(SLUIBackend backend) {
    switch (backend) {
    case SLUI_BACKEND_WIN32: return "win32";
    case SLUI_BACKEND_COCOA: return "cocoa";
    case SLUI_BACKEND_X11: return "x11";
    default: return "unknown";
    }
}

/* ---- App lifecycle ------------------------------------------------------- */
SLUIApp* slui_app_create(const char* app_id) {
    auto app = new (std::nothrow) SLUIApp();
    if (!app) return nullptr;
    app->app_id = app_id ? app_id : "com.mearvk.SleelaUI";
    app->backend = slui::create_backend(app->app_id);
    if (!app->backend) {
        delete app;
        return nullptr;
    }
    return app;
}

void slui_app_destroy(SLUIApp* app) {
    if (!app) return;
    app->windows.clear();
    app->backend.reset();
    delete app;
}

int slui_app_pump(SLUIApp* app, int block) {
    if (!app || !app->backend) return SLUI_ERR_INVALID;
    return app->backend->pump(block != 0);
}

int slui_app_run(SLUIApp* app) {
    if (!app || !app->backend) return SLUI_ERR_INVALID;
    int exit_code = 0;
    for (;;) {
        if (app->backend->should_quit(&exit_code)) break;
        /* Stop when every window has closed. */
        bool any_open = false;
        for (auto& w : app->windows)
            if (!w->closed()) {
                any_open = true;
                break;
            }
        if (!any_open && !app->windows.empty()) break;
        int n = app->backend->pump(true);
        if (n < 0) {
            exit_code = n;
            break;
        }
    }
    return exit_code;
}

void slui_app_quit(SLUIApp* app, int exit_code) {
    if (app && app->backend) app->backend->quit(exit_code);
}

/* ---- Window -------------------------------------------------------------- */
SLUIWindow* slui_window_create(SLUIApp* app, const SLUIWindowConfig* config) {
    if (!app || !config) return nullptr;
    auto win = std::make_unique<slui::Window>(app, app->backend.get(), *config);
    slui::Window* raw = win.get();
    app->windows.push_back(std::move(win));
    return reinterpret_cast<SLUIWindow*>(raw);
}

void slui_window_destroy(SLUIWindow* window) {
    if (!window) return;
    slui::Window* w = as_window(window);
    w->hide();
    /* Ownership stays with the App's vector; mark hidden/closed. The App frees
     * it at destroy time. This keeps the simple handle model safe. */
}

void slui_window_show(SLUIWindow* window) {
    if (window) as_window(window)->show();
}
void slui_window_hide(SLUIWindow* window) {
    if (window) as_window(window)->hide();
}
void slui_window_set_title(SLUIWindow* window, const char* title) {
    if (window) as_window(window)->set_title(title ? title : "");
}
SLUIWidget* slui_window_root(SLUIWindow* window) {
    if (!window) return nullptr;
    return as_window(window)->root_handle();
}
const SLUITheme* slui_window_theme(const SLUIWindow* /*window*/) {
    /* The decoded theme lives inside the Window; we don't retain the ABI copy.
     * Hosts that need the palette should keep the SLUITheme they passed in. */
    return nullptr;
}
void slui_window_set_theme(SLUIWindow* window, const SLUITheme* theme) {
    if (!window || !theme) return;
    as_window(window)->set_theme(slui::Theme::from_abi(*theme));
}
void slui_window_request_redraw(SLUIWindow* window) {
    if (window) as_window(window)->request_redraw();
}
void slui_window_set_event_handler(SLUIWindow* window, SLUIEventHandler handler,
                                   void* user) {
    if (window) as_window(window)->set_event_handler(handler, user);
}

/* ---- Widget construction ------------------------------------------------- */
static SLUIWidget* attach(SLUIWidget* parent, std::unique_ptr<slui::Widget> w) {
    if (!parent) return nullptr;
    slui::Widget* p = as_widget(parent);
    slui::Widget* raw = p->add_child(std::move(w));
    /* Propagate the invalidate hook from the parent so the new widget can ask
     * its window to repaint. */
    return reinterpret_cast<SLUIWidget*>(raw);
}

SLUIWidget* slui_box(SLUIWidget* parent, SLUIOrientation orient, int spacing) {
    return attach(parent, std::make_unique<slui::Box>(orient, spacing));
}
SLUIWidget* slui_header_bar(SLUIWidget* parent) {
    return attach(parent, std::make_unique<slui::HeaderBar>());
}
SLUIWidget* slui_label(SLUIWidget* parent, const char* text) {
    return attach(parent, std::make_unique<slui::Label>(text ? text : ""));
}
SLUIWidget* slui_button(SLUIWidget* parent, const char* text) {
    return attach(parent, std::make_unique<slui::Button>(text ? text : ""));
}
SLUIWidget* slui_toggle(SLUIWidget* parent, const char* text, int initial_on) {
    return attach(parent,
                  std::make_unique<slui::Toggle>(text ? text : "", initial_on != 0));
}
SLUIWidget* slui_entry(SLUIWidget* parent, const char* placeholder) {
    return attach(parent,
                  std::make_unique<slui::Entry>(placeholder ? placeholder : ""));
}
SLUIWidget* slui_slider(SLUIWidget* parent, double min, double max, double value) {
    return attach(parent, std::make_unique<slui::Slider>(min, max, value));
}
SLUIWidget* slui_separator(SLUIWidget* parent, SLUIOrientation orient) {
    return attach(parent, std::make_unique<slui::Separator>(orient));
}
SLUIWidget* slui_spacer(SLUIWidget* parent) {
    return attach(parent, std::make_unique<slui::Spacer>());
}

/* ---- Expanded widget collection ------------------------------------------ */
SLUIWidget* slui_check_box(SLUIWidget* parent, const char* text, int on) {
    return attach(parent,
                  std::make_unique<slui::CheckBox>(text ? text : "", on != 0));
}
SLUIWidget* slui_radio_button(SLUIWidget* parent, const char* text, int group,
                              int on) {
    return attach(parent, std::make_unique<slui::RadioButton>(text ? text : "",
                                                              group, on != 0));
}
SLUIWidget* slui_combo_box(SLUIWidget* parent) {
    return attach(parent, std::make_unique<slui::ComboBox>());
}
void slui_combo_box_add(SLUIWidget* combo, const char* option) {
    if (combo && option) {
        auto* c = dynamic_cast<slui::ComboBox*>(as_widget(combo));
        if (c) c->add_option(option);
    }
}
SLUIWidget* slui_spin_button(SLUIWidget* parent, double min, double max,
                             double step, double value) {
    return attach(parent,
                  std::make_unique<slui::SpinButton>(min, max, step, value));
}
SLUIWidget* slui_progress_bar(SLUIWidget* parent, double fraction) {
    return attach(parent, std::make_unique<slui::ProgressBar>(fraction));
}
SLUIWidget* slui_level_bar(SLUIWidget* parent, double fraction) {
    return attach(parent, std::make_unique<slui::LevelBar>(fraction));
}
SLUIWidget* slui_spinner(SLUIWidget* parent) {
    return attach(parent, std::make_unique<slui::Spinner>());
}
SLUIWidget* slui_scroll_bar(SLUIWidget* parent, SLUIOrientation orient,
                            double value, double page) {
    return attach(parent,
                  std::make_unique<slui::ScrollBar>(orient, value, page));
}
SLUIWidget* slui_frame(SLUIWidget* parent, const char* title) {
    return attach(parent, std::make_unique<slui::Frame>(title ? title : ""));
}
SLUIWidget* slui_card(SLUIWidget* parent) {
    return attach(parent, std::make_unique<slui::Card>());
}
SLUIWidget* slui_grid(SLUIWidget* parent, int columns, int spacing) {
    return attach(parent, std::make_unique<slui::Grid>(columns, spacing));
}
SLUIWidget* slui_status_bar(SLUIWidget* parent) {
    return attach(parent, std::make_unique<slui::StatusBar>());
}
SLUIWidget* slui_image(SLUIWidget* parent, const char* glyph, int w, int h) {
    return attach(parent,
                  std::make_unique<slui::Image>(glyph ? glyph : "", w, h));
}
SLUIWidget* slui_avatar(SLUIWidget* parent, const char* initial, int diameter) {
    return attach(parent, std::make_unique<slui::Avatar>(initial ? initial : "",
                                                        diameter));
}
SLUIWidget* slui_badge(SLUIWidget* parent, const char* text) {
    return attach(parent, std::make_unique<slui::Badge>(text ? text : ""));
}
SLUIWidget* slui_chip(SLUIWidget* parent, const char* text) {
    return attach(parent, std::make_unique<slui::Chip>(text ? text : ""));
}
SLUIWidget* slui_heading(SLUIWidget* parent, const char* text, int size_pt) {
    return attach(parent,
                  std::make_unique<slui::Heading>(text ? text : "", size_pt));
}
SLUIWidget* slui_tooltip(SLUIWidget* parent, const char* text) {
    return attach(parent, std::make_unique<slui::Tooltip>(text ? text : ""));
}
SLUIWidget* slui_link_button(SLUIWidget* parent, const char* text) {
    return attach(parent, std::make_unique<slui::LinkButton>(text ? text : ""));
}
SLUIWidget* slui_search_entry(SLUIWidget* parent, const char* placeholder) {
    return attach(parent,
                  std::make_unique<slui::SearchEntry>(placeholder ? placeholder : ""));
}
SLUIWidget* slui_password_entry(SLUIWidget* parent, const char* placeholder) {
    return attach(parent, std::make_unique<slui::PasswordEntry>(
                              placeholder ? placeholder : ""));
}
SLUIWidget* slui_info_bar(SLUIWidget* parent, const char* text, int severity) {
    return attach(parent,
                  std::make_unique<slui::InfoBar>(text ? text : "", severity));
}

/* ---- Widget configuration ------------------------------------------------ */
void slui_widget_set_margin(SLUIWidget* w, int top, int right, int bottom,
                            int left) {
    if (w) as_widget(w)->set_margin(slui::Margin{top, right, bottom, left});
}
void slui_widget_set_align(SLUIWidget* w, SLUIAlign h, SLUIAlign v) {
    if (w) as_widget(w)->set_align(h, v);
}
void slui_widget_set_expand(SLUIWidget* w, int h_expand, int v_expand) {
    if (w) as_widget(w)->set_expand(h_expand != 0, v_expand != 0);
}
void slui_widget_set_size_request(SLUIWidget* w, int min_w, int min_h) {
    if (w) as_widget(w)->set_size_request(min_w, min_h);
}
void slui_widget_set_sensitive(SLUIWidget* w, int sensitive) {
    if (w) as_widget(w)->set_sensitive(sensitive != 0);
}
void slui_widget_set_visible(SLUIWidget* w, int visible) {
    if (w) as_widget(w)->set_visible(visible != 0);
}

void slui_widget_set_text(SLUIWidget* w, const char* text) {
    if (w) as_widget(w)->set_text(text ? text : "");
}
size_t slui_widget_get_text(const SLUIWidget* w, char* out, size_t cap) {
    if (!w) return 0;
    std::string s = as_widget(w)->text();
    if (out && cap > 0) {
        size_t n = s.size() < cap - 1 ? s.size() : cap - 1;
        std::memcpy(out, s.data(), n);
        out[n] = '\0';
    }
    return s.size();
}

void slui_widget_set_toggle(SLUIWidget* w, int on) {
    if (w) as_widget(w)->set_toggle(on != 0);
}
int slui_widget_get_toggle(const SLUIWidget* w) {
    return w ? (as_widget(w)->toggle() ? 1 : 0) : 0;
}
void slui_widget_set_value(SLUIWidget* w, double value) {
    if (w) as_widget(w)->set_value(value);
}
double slui_widget_get_value(const SLUIWidget* w) {
    return w ? as_widget(w)->value() : 0.0;
}
int slui_widget_take_activated(SLUIWidget* w) {
    return w ? (as_widget(w)->take_activated() ? 1 : 0) : 0;
}
void slui_widget_set_suggested(SLUIWidget* w, int suggested) {
    if (w) as_widget(w)->set_suggested(suggested != 0);
}
void slui_widget_set_destructive(SLUIWidget* w, int destructive) {
    if (w) as_widget(w)->set_destructive(destructive != 0);
}

void slui_widget_on_activate(SLUIWidget* w, SLUIActivateHandler handler,
                             void* user) {
    if (w) as_widget(w)->on_activate(handler, user);
}
void slui_widget_on_value_changed(SLUIWidget* w, SLUIValueHandler handler,
                                  void* user) {
    if (w) as_widget(w)->on_value_changed(handler, user);
}

} /* extern "C" */
