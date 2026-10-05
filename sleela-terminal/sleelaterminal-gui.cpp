#include <gtk/gtk.h>
#include <vte/vte.h>

#include <algorithm>
#include <cmath>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace {

constexpr const char *kApplicationId = "com.mearvk.SleelaTerminal";
constexpr const char *kWindowTitle = "SleelaTerminal™ — MEARVK LLC";
constexpr const char *kConfigName = "sleela-terminal.conf";
constexpr const char *kVersion = "1.0.0";

// One "child of wattage": a discrete excitation that flows along the throbber
// strip and locally modulates the base light-blue colour. Many of these,
// summed, make the living field. Motion is integrated to third order
// (jerk -> accel -> vel -> pos) so speed changes are smooth and reflexive.
struct ThrobberCell {
    bool alive = false;
    double x = 0.0;        // position in pixels
    double v = 0.0;        // velocity (px/s), mostly positive = left->right
    double a = 0.0;        // acceleration (px/s^2)
    double j = 0.0;        // jerk (px/s^3) -- the 3rd-order control term
    double base_intensity = 0.0; // the cell's inherent shade strength [0..1]
    double intensity = 0.0;      // live rendered contribution (base * fades)
    double energy = 1.0;         // life/decay budget
    double sigma = 0.0;    // spatial half-width of this cell's glow (px)
    double speed_mul = 1.0;// per-cell speed character (some glide, some race)
    double phase = 0.0;    // per-cell phase for subtle flicker
    bool retro = false;    // a rare child allowed to pause / go right->left
    double retro_px = 0.0; // pixels of retrograde/pause travel remaining
    // Optional colour tint, only expressed under strong amplification.
    // (tr,tg,tb) is a fully-saturated hue; tinted==false means pure base shade.
    bool tinted = false;
    double tr = 0.0, tg = 0.0, tb = 0.0;
};

struct AppConfig {
    std::string font = "Monospace 11";
    std::string foreground = "#FFFFFF";
    std::string input_language = "system";
    std::string output_language = "system";
    bool national_feeds = false;
    bool persons_of_interest = false;
    bool parametric_documents = false;
    bool national_identifiers = false;
};

struct AppState {
    std::string shell_path;
    GtkWindow *window = nullptr;
    VteTerminal *terminal = nullptr;
    GtkLabel *footer_text = nullptr;
    std::string executable_path;
    GPid child_pid = 0;
    guint footer_tick = 0;
    guint footer_position = 0;
    AppConfig config;

    // Title-bar throbber: a thin living light-blue strip along the bottom of
    // the title bar (replacing the static border highlight). It is a flowing
    // field of discrete excitations ("children of wattage") that stream mainly
    // left->right at ~20 Hz; see throbber_tick / throbber_draw.
    GtkWidget *throbber = nullptr;
    guint throbber_tick = 0;
    double throbber_t = 0.0;        // monotonically advancing time (seconds)
    int    throbber_w = 0;          // last known strip width (px)
    double throbber_amp = 0.6;      // slow reflexive "amplification" field [0..1]
    double throbber_amp_v = 0.0;    // its rate of change (for smooth 2nd order)
    double throbber_spawn_accum = 0.0; // fractional spawn accumulator
    std::vector<ThrobberCell> throbber_cells;
};

std::filesystem::path config_path() {
    const char *config_dir = g_get_user_config_dir();
    return std::filesystem::path(config_dir) / "sleela" / kConfigName;
}

void save_config(const AppConfig &config) {
    const auto path = config_path();
    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) return;

    std::ofstream out(path);
    if (!out) return;
    out << "# SleelaTerminal local presentation and data-orientation settings\n";
    out << "# Data source switches are declarative; they do not fetch or transmit data.\n";
    out << "font=" << config.font << '\n';
    out << "foreground=" << config.foreground << '\n';
    out << "input_language=" << config.input_language << '\n';
    out << "output_language=" << config.output_language << '\n';
    out << "national_feeds=" << (config.national_feeds ? "true" : "false") << '\n';
    out << "persons_of_interest=" << (config.persons_of_interest ? "true" : "false") << '\n';
    out << "parametric_documents=" << (config.parametric_documents ? "true" : "false") << '\n';
    out << "national_identifiers=" << (config.national_identifiers ? "true" : "false") << '\n';
}

void load_config(AppConfig &config) {
    std::ifstream in(config_path());
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty() || line.front() == '#') continue;
        const auto equals = line.find('=');
        if (equals == std::string::npos) continue;
        const std::string key = line.substr(0, equals);
        const std::string value = line.substr(equals + 1);

        if (key == "font") config.font = value;
        else if (key == "foreground") config.foreground = value;
        else if (key == "input_language") config.input_language = value;
        else if (key == "output_language") config.output_language = value;
        else if (key == "national_feeds") config.national_feeds = value == "true";
        else if (key == "persons_of_interest") config.persons_of_interest = value == "true";
        else if (key == "parametric_documents") config.parametric_documents = value == "true";
        else if (key == "national_identifiers") config.national_identifiers = value == "true";
    }
}

std::string asset_path(const AppState *state, const char *name) {
    namespace fs = std::filesystem;
    fs::path executable = fs::absolute(state->executable_path);
    const std::vector<fs::path> candidates = {
        executable.parent_path() / "assets" / name,
        executable.parent_path() / ".." / "assets" / name,
        fs::path("sleela-terminal") / "assets" / name
    };
    std::error_code ec;
    for (const auto &candidate : candidates) {
        if (fs::exists(candidate, ec) && !ec) return candidate.lexically_normal().string();
    }
    return name;
}

// CMD is the Java native launcher associated with SecureJDK 28.
// The footer CMD image is a strict image control: no button outline or frame.
std::string installer_path(const AppState *state) {
    namespace fs = std::filesystem;
    fs::path executable = fs::absolute(state->executable_path);
    const std::vector<fs::path> candidates = {
        executable.parent_path() / ".." / "tools" / "install" / "sleela-software.sh",
        fs::path("sleela-terminal") / "tools" / "install" / "sleela-software.sh",
        fs::path("tools") / "install" / "sleela-software.sh"
    };
    std::error_code ec;
    for (const auto &candidate : candidates) {
        if (fs::exists(candidate, ec) && !ec) return candidate.lexically_normal().string();
    }
    return candidates.front().lexically_normal().string();
}

std::string sibling_path(const char *argv0, const char *name) {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path executable = fs::absolute(argv0, ec);
    if (ec) return name;
    fs::path candidate = executable.parent_path() / name;
    if (fs::exists(candidate, ec) && !ec) return candidate.string();
    return name;
}

bool parse_rgba(const std::string &value, GdkRGBA &rgba) {
    return gdk_rgba_parse(&rgba, value.c_str());
}

void apply_terminal_style(AppState *state) {
    if (state->terminal == nullptr) return;

    PangoFontDescription *font = pango_font_description_from_string(state->config.font.c_str());
    vte_terminal_set_font(state->terminal, font);
    pango_font_description_free(font);

    GdkRGBA foreground;
    GdkRGBA background;
    if (!parse_rgba(state->config.foreground, foreground)) {
        gdk_rgba_parse(&foreground, "#FFFFFF");
    }
    // Terminal background matches the window background (@sl_bg) so the terminal
    // reads as one surface with the chrome rather than a separate near-black
    // rectangle. See UI-PRINCIPLES.md (principle 2).
    gdk_rgba_parse(&background, "#15101c");

    // A 16-colour ANSI palette tuned to the dark-purple theme: slightly warmer
    // and less saturated than raw VTE defaults so output is legible and calm on
    // the @sl_bg background without clashing with the purple frame.
    static const char *const kPalette[16] = {
        "#1b1526", "#e06c75", "#8fcf8f", "#e5c07b", // black, red, green, yellow
        "#8f9ff0", "#c58af0", "#6fd0d8", "#cfcad8", // blue, magenta, cyan, white
        "#4a4160", "#ff8a93", "#b0e0b0", "#f2d79a", // bright black..yellow
        "#aab6ff", "#d9b0ff", "#9fe6ec", "#ffffff"  // bright blue..white
    };
    GdkRGBA palette[16];
    for (int i = 0; i < 16; ++i) gdk_rgba_parse(&palette[i], kPalette[i]);
    vte_terminal_set_colors(state->terminal, &foreground, &background, palette, 16);
}

void install_css() {
    // One palette, one source of truth (see UI-PRINCIPLES.md). Every colour
    // below is a named token; selectors never carry raw hex values. The window
    // and terminal share @sl_bg so the terminal does not float as a separate
    // near-black rectangle inside the purple chrome.
    //
    // NOTE: this is a C++ raw string literal; newlines here are real newlines.
    // Never write the two characters backslash-n inside it -- GTK's CSS parser
    // rejects the stray backslash and drops the rest of the rule.
    const char *css = R"CSS(
        @define-color sl_bg          #15101c;
        @define-color sl_surface     #21112f;
        @define-color sl_chrome      #24103f;
        @define-color sl_border      #7f56aa;
        @define-color sl_fg          #ffffff;
        @define-color sl_accent      #9d6cff;

        window { background: @sl_bg; }

        /* Title bar: flat chrome. The bottom highlight is no longer a static
           border -- it is the living throbber strip drawn just below the bar
           (see .sleela-throbber and the 20Hz organic update). A transparent 1px
           border keeps the bar's height identical to before. */
        headerbar.sleela-titlebar {
            background: @sl_chrome;
            color: @sl_fg;
            min-height: 38px;
            border-bottom: 1px solid transparent;
        }
        /* The throbber occupies the thin seam between the title bar and the
           terminal; its colour is painted per-frame by the draw function. */
        drawingarea.sleela-throbber { background: @sl_chrome; min-height: 2px; }
        headerbar.sleela-titlebar label { color: @sl_fg; font-weight: 600; }
        image.sleela-titlebar-logo { margin-left: 8px; margin-right: 4px; }
        headerbar.sleela-titlebar button.titlebutton {
            color: @sl_fg;
            background: alpha(@sl_fg, 0.08);
            min-width: 32px;
            min-height: 32px;
            transition: background 120ms ease;
        }
        headerbar.sleela-titlebar button.titlebutton:hover { background: alpha(@sl_fg, 0.16); }
        headerbar.sleela-titlebar button.titlebutton:active { background: alpha(@sl_fg, 0.24); }
        headerbar.sleela-titlebar button.titlebutton:focus { outline: 2px solid @sl_accent; outline-offset: -2px; }

        /* Settings (gear) button: a quiet, flat affordance. No resting
           background or border -- the icon alone sits in the bar; it reveals a
           soft tint only on hover/active. ~25% smaller than the 32px controls. */
        headerbar.sleela-titlebar button.sleela-settings-btn {
            color: alpha(@sl_fg, 0.82);
            background: transparent;
            border: none;
            box-shadow: none;
            min-width: 24px;
            min-height: 24px;
            padding: 2px;
            margin-right: 4px;
            transition: background 120ms ease, color 120ms ease;
        }
        headerbar.sleela-titlebar button.sleela-settings-btn:hover {
            color: @sl_fg;
            background: alpha(@sl_fg, 0.12);
        }
        headerbar.sleela-titlebar button.sleela-settings-btn:active { background: alpha(@sl_fg, 0.20); }
        headerbar.sleela-titlebar button.sleela-settings-btn:focus { outline: 1px solid @sl_accent; outline-offset: -1px; }

        /* Footer: flat chrome bar matching the title bar, one hairline on top. */
        box.sleela-footer {
            min-height: 38px;
            background: @sl_chrome;
            border-top: 1px solid @sl_border;
        }
        box.sleela-footer label { color: @sl_fg; font-weight: 600; }
        label.sleela-footer-brand { font-weight: 800; letter-spacing: 0.5px; }
        label.sleela-footer-separator { color: alpha(@sl_fg, 0.42); padding-left: 8px; padding-right: 8px; }
        label.sleela-footer-ticker { padding-left: 8px; padding-right: 8px; }
        button.sleela-footer-java {
            min-width: 32px; min-height: 32px; padding: 0; margin: 0 8px;
            background: transparent; border: none; box-shadow: none; border-radius: 0;
        }
        button.sleela-footer-java:hover,
        button.sleela-footer-java:active { background: transparent; border: none; box-shadow: none; }
        button.sleela-footer-java:focus { outline: 2px solid @sl_accent; outline-offset: -2px; }
        button.sleela-footer-java image { background: transparent; border: none; box-shadow: none; }

        /* Shared surface styling for popovers and dialogs. */
        window.sleela-installer { background: @sl_surface; color: @sl_fg; }
        window.sleela-installer label { color: @sl_fg; }

        popover.sleela-software contents { background: @sl_surface; border: 1px solid @sl_border; }
        /* Settings is a subframe window; theme the window surface itself. */
        window.sleela-settings { background: @sl_surface; }
        window.sleela-settings scrolledwindow { background: @sl_surface; }

        popover.sleela-software label,
        window.sleela-settings label { color: @sl_fg; }
        window.sleela-settings label.section { font-weight: 800; margin-top: 8px; }

        popover.sleela-software button,
        window.sleela-settings button {
            color: @sl_fg;
            background: alpha(@sl_fg, 0.08);
            min-width: 260px;
            min-height: 32px;
            text-align: left;
            transition: background 120ms ease;
        }
        popover.sleela-software button:hover,
        window.sleela-settings button:hover { background: alpha(@sl_fg, 0.16); }
        popover.sleela-software button:focus,
        window.sleela-settings button:focus { outline: 2px solid @sl_accent; outline-offset: -2px; }
        /* Keep the Close suggested-action compact, not full-width like the list. */
        window.sleela-settings button.suggested-action { min-width: 88px; }

        /* The single suggested action uses the accent; nothing else does. */
        button.suggested-action { background: @sl_accent; color: @sl_fg; }
        button.suggested-action:hover { background: shade(@sl_accent, 1.12); }

        textview.sleela-scan-output { color: @sl_fg; background: @sl_bg; }

        /* Secondary / explanatory notes: one dim colour, slightly smaller. */
        label.sleela-software-note,
        label.sleela-trust-note {
            color: alpha(@sl_fg, 0.70);
            font-size: 0.9em;
        }
    )CSS";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

void set_font(AppState *state, const char *font) {
    state->config.font = font;
    apply_terminal_style(state);
    save_config(state->config);
}

void set_foreground(AppState *state, const char *color) {
    state->config.foreground = color;
    apply_terminal_style(state);
    save_config(state->config);
}

void set_input_language(AppState *state, const char *language) {
    state->config.input_language = language;
    save_config(state->config);
}

void set_output_language(AppState *state, const char *language) {
    state->config.output_language = language;
    save_config(state->config);
}

void set_source(GtkCheckButton *button, bool &target, AppState *state) {
    target = gtk_check_button_get_active(button);
    save_config(state->config);
}

void make_settings_menu(AppState *state, GtkWidget *settings_button_widget) {
    (void) settings_button_widget;

    // Settings opens as a proper subframe window (not a transient popover), so
    // the adjustments stay on screen while the user works through them. It is a
    // modal child of the main window, consistent with the other SleelaTerminal
    // subframes (installer / software output).
    GtkWidget *frame = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(frame), "SleelaTerminal — Settings");
    gtk_window_set_default_size(GTK_WINDOW(frame), 420, 560);
    gtk_window_set_modal(GTK_WINDOW(frame), TRUE);
    if (state->window != nullptr) {
        gtk_window_set_transient_for(GTK_WINDOW(frame), state->window);
    }
    gtk_widget_add_css_class(frame, "sleela-settings");

    // The settings content can be taller than the frame, so make it scrollable.
    GtkWidget *scroller = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroller, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller),
                                   GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12);
    gtk_widget_set_margin_bottom(box, 12);

    GtkWidget *title = gtk_label_new("SleelaTerminal Settings");
    gtk_widget_add_css_class(title, "heading");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), title);

    GtkWidget *appearance = gtk_label_new("Presentation");
    gtk_widget_add_css_class(appearance, "section");
    gtk_widget_set_halign(appearance, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), appearance);

    struct FontChoice { const char *label; const char *value; };
    static const FontChoice fonts[] = {
        {"Monospace 10", "Monospace 10"},
        {"Monospace 11", "Monospace 11"},
        {"Monospace 12", "Monospace 12"},
        {"Sans 11", "Sans 11"},
        {"Serif 11", "Serif 11"}
    };
    for (const auto &choice : fonts) {
        GtkWidget *button = gtk_button_new_with_label(choice.label);
        g_object_set_data_full(G_OBJECT(button), "sleela-value", g_strdup(choice.value), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            set_font(state, static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-value")));
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    struct ColorChoice { const char *label; const char *value; };
    static const ColorChoice colors[] = {
        {"Font: White", "#FFFFFF"},
        {"Font: Soft Violet", "#E8D8FF"},
        {"Font: Ice Blue", "#D8F0FF"},
        {"Font: Mint", "#D8FFE8"},
        {"Font: Amber", "#FFE8B0"}
    };
    for (const auto &choice : colors) {
        GtkWidget *button = gtk_button_new_with_label(choice.label);
        g_object_set_data_full(G_OBJECT(button), "sleela-value", g_strdup(choice.value), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            set_foreground(state, static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-value")));
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    GtkWidget *language = gtk_label_new("Language Orientation");
    gtk_widget_add_css_class(language, "section");
    gtk_widget_set_halign(language, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), language);

    static const char *languages[] = {"System", "English", "Spanish", "French", "German", "Korean", "Chinese", "Japanese"};
    for (const char *name : languages) {
        GtkWidget *button = gtk_button_new_with_label(name);
        g_object_set_data_full(G_OBJECT(button), "sleela-language", g_strdup(name), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(+[](GtkButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            const char *language = static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-language"));
            set_input_language(state, language);
            set_output_language(state, language);
        }), state);
        gtk_box_append(GTK_BOX(box), button);
    }

    GtkWidget *orientation = gtk_label_new("Parametric Ring of Trust / Orientation");
    gtk_widget_add_css_class(orientation, "section");
    gtk_widget_set_halign(orientation, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), orientation);

    GtkWidget *note = gtk_label_new("Local declarations only. Sources remain off until explicitly enabled.");
    gtk_widget_add_css_class(note, "sleela-trust-note");
    gtk_label_set_wrap(GTK_LABEL(note), TRUE);
    gtk_widget_set_halign(note, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), note);

    struct SourceChoice { const char *label; bool *member; };
    SourceChoice sources[] = {
        {"National / channel feeds", &state->config.national_feeds},
        {"Persons of interest", &state->config.persons_of_interest},
        {"Parametric documents / resumes", &state->config.parametric_documents},
        {"National identifiers", &state->config.national_identifiers}
    };
    for (const auto &source : sources) {
        GtkWidget *check = gtk_check_button_new_with_label(source.label);
        gtk_check_button_set_active(GTK_CHECK_BUTTON(check), *source.member);
        g_object_set_data(G_OBJECT(check), "sleela-member", source.member);
        g_signal_connect(check, "toggled", G_CALLBACK(+[](GtkCheckButton *button, gpointer data) {
            auto *state = static_cast<AppState *>(data);
            auto *member = static_cast<bool *>(g_object_get_data(G_OBJECT(button), "sleela-member"));
            set_source(button, *member, state);
        }), state);
        gtk_box_append(GTK_BOX(box), check);
    }

    GtkWidget *trust = gtk_label_new(
        "The ring describes orientation and declared relationships; it does not itself establish identity, authority, truth, or legal status.\n"
        "Configuration is human-readable and stored locally under the user's configuration directory.");
    gtk_widget_add_css_class(trust, "sleela-trust-note");
    gtk_label_set_wrap(GTK_LABEL(trust), TRUE);
    gtk_widget_set_halign(trust, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), trust);

    GtkWidget *done = gtk_button_new_with_label("Close");
    gtk_widget_add_css_class(done, "suggested-action");
    gtk_widget_set_halign(done, GTK_ALIGN_END);
    gtk_widget_set_margin_top(done, 8);
    g_signal_connect_swapped(done, "clicked", G_CALLBACK(gtk_window_destroy), frame);
    gtk_box_append(GTK_BOX(box), done);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroller), box);
    gtk_window_set_child(GTK_WINDOW(frame), scroller);
    gtk_window_present(GTK_WINDOW(frame));
}

void open_settings(GtkButton *, gpointer user_data) {
    auto *button = static_cast<GtkWidget *>(user_data);
    auto *state = static_cast<AppState *>(g_object_get_data(G_OBJECT(button), "sleela-state"));
    make_settings_menu(state, button);
}

void software_done(GObject *source, GAsyncResult *result, gpointer user_data) {
    auto *buffer = GTK_TEXT_BUFFER(user_data);
    GError *error = nullptr;
    gchar *stdout_text = nullptr;
    gchar *stderr_text = nullptr;
    if (!g_subprocess_communicate_utf8_finish(G_SUBPROCESS(source), result, &stdout_text, &stderr_text, &error)) {
        gtk_text_buffer_set_text(buffer, error ? error->message : "Software operation failed", -1);
        if (error) g_error_free(error);
        g_free(stdout_text); g_free(stderr_text); g_object_unref(source); return;
    }
    std::string text = stdout_text ? stdout_text : "";
    if (stderr_text && *stderr_text) { text += "\n"; text += stderr_text; }
    gtk_text_buffer_set_text(buffer, text.c_str(), -1);
    g_free(stdout_text); g_free(stderr_text); g_object_unref(source);
}

void run_software_action(AppState *state, const char *action, const char *product, GtkTextBuffer *buffer) {
    const std::string script = installer_path(state);
    const char *argv[] = {"bash", script.c_str(), action, product, nullptr};
    GError *error = nullptr;
    GSubprocess *process = g_subprocess_newv(argv, static_cast<GSubprocessFlags>(G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE), &error);
    if (!process) {
        gtk_text_buffer_set_text(buffer, error ? error->message : "Unable to start software operation", -1);
        if (error) {
            g_error_free(error);
        }
        return;
    }
    gtk_text_buffer_set_text(buffer, "Working…", -1);
    g_subprocess_communicate_utf8_async(process, nullptr, nullptr, software_done, buffer);
}

void make_software_menu(AppState *state, GtkWidget *anchor) {
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());
    gtk_widget_add_css_class(GTK_WIDGET(popover), "sleela-software");
    gtk_widget_set_parent(GTK_WIDGET(popover), anchor);
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_margin_start(box, 12); gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 12); gtk_widget_set_margin_bottom(box, 12);
    GtkWidget *title = gtk_label_new("SecureJDK 28 / CMD Software Center");
    gtk_widget_add_css_class(title, "heading"); gtk_widget_set_halign(title, GTK_ALIGN_START); gtk_box_append(GTK_BOX(box), title);
    GtkWidget *note = gtk_label_new("Scan the configured GitHub source for release state, then explicitly install or prepare the latest source. Alpha/pre-release and final states are reported separately.");
    gtk_widget_add_css_class(note, "sleela-software-note"); gtk_label_set_wrap(GTK_LABEL(note), TRUE); gtk_widget_set_halign(note, GTK_ALIGN_START); gtk_box_append(GTK_BOX(box), note);
    GtkTextView *view = GTK_TEXT_VIEW(gtk_text_view_new());
    gtk_text_view_set_editable(view, FALSE); gtk_text_view_set_cursor_visible(view, FALSE);
    gtk_widget_add_css_class(GTK_WIDGET(view), "sleela-scan-output"); gtk_widget_set_size_request(GTK_WIDGET(view), 390, 130); gtk_box_append(GTK_BOX(box), GTK_WIDGET(view));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
    struct Payload { AppState *state; std::string action; std::string product; GtkTextBuffer *buffer; };
    auto add = [&](const char *label, const char *action, const char *product) {
        GtkWidget *button = gtk_button_new_with_label(label);
        auto *payload = new Payload{state, action, product, buffer};
        g_signal_connect_data(button, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
            auto *p = static_cast<Payload *>(data);
            run_software_action(p->state, p->action.c_str(), p->product.c_str(), p->buffer);
        }), payload, [](gpointer data, GClosure *) {
            delete static_cast<Payload *>(data);
        }, G_CONNECT_AFTER);
        gtk_box_append(GTK_BOX(box), button);
    };
    add("Scan GitHub Releases", "scan", "all");
    add("Install / Prepare SecureJDK 28", "install", "securejdk28");
    add("Install / Prepare CMD", "install", "cmd");
    add("Install / Prepare Asysma", "install", "asysma");
    add("Install / Prepare All", "install", "all");
    GtkWidget *source = gtk_label_new("Source: github.com/mearvk/Ubuntu.Determinant.Beta.Restricted");
    gtk_widget_add_css_class(source, "sleela-software-note"); gtk_label_set_wrap(GTK_LABEL(source), TRUE); gtk_widget_set_halign(source, GTK_ALIGN_START); gtk_box_append(GTK_BOX(box), source);
    gtk_popover_set_child(popover, box); gtk_popover_set_has_arrow(popover, TRUE); gtk_popover_set_autohide(popover, FALSE); gtk_popover_popup(popover);
}


void show_install_prompt(AppState *state, GtkWidget *, const char *product, const char *display_name) {
    GtkWidget *dialog = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "SleelaTerminal — Java Installer");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 520, 300);
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), state->window);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);
    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);

    GtkWidget *title = gtk_label_new("CMD / Java Program Installer");
    gtk_widget_add_css_class(title, "heading");
    gtk_widget_set_halign(title, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), title);

    std::string prompt =
        "SleelaTerminal is ready to install the latest appropriate release of " +
        std::string(display_name) +
        ".\n\n"
        "The installer will inspect the configured source, distinguish FINAL from "
        "ALPHA/PRE-RELEASE where available, and prepare the selected program locally.\n\n"
        "Proceed with the installation?";

    GtkWidget *message = gtk_label_new(prompt.c_str());
    gtk_label_set_wrap(GTK_LABEL(message), TRUE);
    gtk_widget_set_halign(message, GTK_ALIGN_START);
    gtk_widget_set_valign(message, GTK_ALIGN_START);
    gtk_widget_set_vexpand(message, TRUE);
    gtk_box_append(GTK_BOX(box), message);

    GtkWidget *buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(buttons, GTK_ALIGN_END);
    GtkWidget *other = gtk_button_new_with_label("Other Java Programs…");
    GtkWidget *no = gtk_button_new_with_label("No");
    GtkWidget *yes = gtk_button_new_with_label("Yes — Install");
    gtk_widget_add_css_class(yes, "suggested-action");
    gtk_box_append(GTK_BOX(buttons), other);
    gtk_box_append(GTK_BOX(buttons), no);
    gtk_box_append(GTK_BOX(buttons), yes);
    gtk_box_append(GTK_BOX(box), buttons);

    struct InstallPayload { AppState *state; GtkWindow *dialog; std::string product; std::string display_name; };
    auto *payload = new InstallPayload{state, GTK_WINDOW(dialog), product, display_name};

    g_signal_connect_data(yes, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
        auto *p = static_cast<InstallPayload *>(data);
        GtkWidget *result_window = gtk_window_new();
        gtk_window_set_title(GTK_WINDOW(result_window), "SleelaTerminal — Installer Output");
        gtk_window_set_default_size(GTK_WINDOW(result_window), 620, 360);
        gtk_window_set_modal(GTK_WINDOW(result_window), TRUE);
        gtk_window_set_transient_for(GTK_WINDOW(result_window), p->state->window);

        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
        gtk_widget_set_margin_start(box, 16);
        gtk_widget_set_margin_end(box, 16);
        gtk_widget_set_margin_top(box, 16);
        gtk_widget_set_margin_bottom(box, 16);

        GtkWidget *title = gtk_label_new(("Installing " + p->display_name).c_str());
        gtk_widget_set_halign(title, GTK_ALIGN_START);
        gtk_box_append(GTK_BOX(box), title);

        GtkTextView *view = GTK_TEXT_VIEW(gtk_text_view_new());
        gtk_text_view_set_editable(view, FALSE);
        gtk_text_view_set_cursor_visible(view, FALSE);
        gtk_widget_add_css_class(GTK_WIDGET(view), "sleela-scan-output");
        gtk_widget_set_vexpand(GTK_WIDGET(view), TRUE);
        gtk_box_append(GTK_BOX(box), GTK_WIDGET(view));

        GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
        run_software_action(p->state, "install", p->product.c_str(), buffer);
        gtk_window_set_child(GTK_WINDOW(result_window), box);
        gtk_window_present(GTK_WINDOW(result_window));
        gtk_window_destroy(p->dialog);
    }), payload, [](gpointer data, GClosure *) {
        delete static_cast<InstallPayload *>(data);
    }, G_CONNECT_AFTER);

    g_signal_connect_swapped(no, "clicked", G_CALLBACK(gtk_window_destroy), dialog);
    g_signal_connect(other, "clicked", G_CALLBACK(+[](GtkButton *, gpointer data) {
        auto *p = static_cast<InstallPayload *>(data);
        make_software_menu(p->state, GTK_WIDGET(p->state->terminal));
        gtk_window_destroy(p->dialog);
    }), payload);
    gtk_window_set_child(GTK_WINDOW(dialog), box);
    gtk_window_present(GTK_WINDOW(dialog));
}

void open_software_menu(GtkButton *, gpointer user_data) {
    auto *button = static_cast<GtkWidget *>(user_data);
    auto *state = static_cast<AppState *>(g_object_get_data(G_OBJECT(button), "sleela-state"));
    show_install_prompt(state, button, "cmd", "CMD");
}
// Ctrl+C is copy when VTE has a selection. Without a selection it is left to
// the PTY so the shell/foreground program receives the normal interrupt key.
gboolean terminal_key_pressed(GtkEventControllerKey *, guint keyval, guint,
                              GdkModifierType state, gpointer user_data) {
    auto *terminal = VTE_TERMINAL(user_data);
    const bool ctrl = (state & GDK_CONTROL_MASK) != 0;
    const bool shift = (state & GDK_SHIFT_MASK) != 0;

    if (ctrl && keyval == GDK_KEY_c) {
        if (shift || vte_terminal_get_has_selection(terminal)) {
            vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
            return TRUE;
        }
        return FALSE;
    }
    if (ctrl && keyval == GDK_KEY_v) {
        vte_terminal_paste_clipboard(terminal);
        return TRUE;
    }
    return FALSE;
}

void terminal_copy(GtkWidget *, gpointer user_data) {
    auto *terminal = VTE_TERMINAL(user_data);
    if (vte_terminal_get_has_selection(terminal)) {
        vte_terminal_copy_clipboard_format(terminal, VTE_FORMAT_TEXT);
    }
}

void terminal_paste(GtkWidget *, gpointer user_data) { vte_terminal_paste_clipboard(VTE_TERMINAL(user_data)); }
void terminal_select_all(GtkWidget *, gpointer user_data) { vte_terminal_select_all(VTE_TERMINAL(user_data)); }
void terminal_clear_selection(GtkWidget *, gpointer user_data) { vte_terminal_unselect_all(VTE_TERMINAL(user_data)); }

void terminal_mouse_menu(GtkGestureClick *gesture, int, double, double, gpointer user_data) {
    auto *terminal = VTE_TERMINAL(user_data);
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());
    gtk_widget_set_parent(GTK_WIDGET(popover), GTK_WIDGET(terminal));
    gtk_popover_set_has_arrow(popover, TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_margin_start(box, 8);
    gtk_widget_set_margin_end(box, 8);
    gtk_widget_set_margin_top(box, 8);
    gtk_widget_set_margin_bottom(box, 8);

    GtkWidget *selection = gtk_label_new("Terminal Text Selection");
    gtk_widget_add_css_class(selection, "heading");
    gtk_widget_set_halign(selection, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), selection);
    GtkWidget *left = gtk_label_new("Left Mouse — Select Terminal Text");
    gtk_widget_set_halign(left, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), left);
    GtkWidget *right = gtk_label_new("Right Mouse — Open Terminal Menu");
    gtk_widget_set_halign(right, GTK_ALIGN_START);
    gtk_box_append(GTK_BOX(box), right);
    GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_append(GTK_BOX(box), separator);

    GtkWidget *copy = gtk_button_new_with_label("Copy Selected Text");
    g_signal_connect(copy, "clicked", G_CALLBACK(terminal_copy), terminal);
    g_signal_connect_swapped(copy, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), copy);
    GtkWidget *paste = gtk_button_new_with_label("Paste Text");
    g_signal_connect(paste, "clicked", G_CALLBACK(terminal_paste), terminal);
    g_signal_connect_swapped(paste, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), paste);
    GtkWidget *select_all = gtk_button_new_with_label("Select All Terminal Text");
    g_signal_connect(select_all, "clicked", G_CALLBACK(terminal_select_all), terminal);
    g_signal_connect_swapped(select_all, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), select_all);
    GtkWidget *clear = gtk_button_new_with_label("Clear Selection");
    g_signal_connect(clear, "clicked", G_CALLBACK(terminal_clear_selection), terminal);
    g_signal_connect_swapped(clear, "clicked", G_CALLBACK(gtk_widget_unparent), popover);
    gtk_box_append(GTK_BOX(box), clear);

    gtk_popover_set_child(popover, box);
    GdkRectangle rect;
    double x = 0.0, y = 0.0;
    gtk_gesture_get_point(GTK_GESTURE(gesture), nullptr, &x, &y);
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
    rect.width = 1;
    rect.height = 1;
    gtk_popover_set_pointing_to(popover, &rect);
    gtk_popover_popup(popover);
}

const std::vector<std::string> &footer_messages() {
    // A single, static status line. The footer is a calm frame, not a ticker:
    // moving text competes with terminal output. See UI-PRINCIPLES.md (6).
    static const std::vector<std::string> messages = {
        "Secure Shell Interface  •  Interactive session"
    };
    return messages;
}

void child_exited(VteTerminal *, int, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->footer_tick != 0) {
        g_source_remove(state->footer_tick);
        state->footer_tick = 0;
    }
    if (state->throbber_tick != 0) {
        g_source_remove(state->throbber_tick);
        state->throbber_tick = 0;
    }
    state->throbber = nullptr;
    state->child_pid = 0;
    if (state->window != nullptr) {
        GtkApplication *application = gtk_window_get_application(state->window);
        GtkWindow *window = state->window;
        state->window = nullptr;
        gtk_window_destroy(window);
        if (application != nullptr) g_application_quit(G_APPLICATION(application));
    }
}

void shell_spawned(VteTerminal *, GPid child_pid, GError *error, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (error != nullptr || child_pid <= 0) {
        state->child_pid = 0;
        return;
    }
    state->child_pid = child_pid;
}

gboolean window_close_request(GtkWindow *window, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->child_pid > 0) {
        ::kill(static_cast<pid_t>(state->child_pid), SIGHUP);
        state->child_pid = 0;
    }
    if (state->footer_tick != 0) {
        g_source_remove(state->footer_tick);
        state->footer_tick = 0;
    }
    if (state->throbber_tick != 0) {
        g_source_remove(state->throbber_tick);
        state->throbber_tick = 0;
    }
    state->throbber = nullptr;
    gtk_window_destroy(window);
    g_application_quit(G_APPLICATION(gtk_window_get_application(window)));
    return TRUE;
}

// Render the living field. The strip is NOT one uniform level: it rests at the
// title-bar chrome colour and is modulated locally by every excitation ("child
// of wattage"), each adding a relative darker/lighter shade near its position.
// The sum, drawn per column, is a texture that flows left->right. Under strong
// amplification, latent per-cell hues (orange/green/blue/red) surface.
void throbber_draw(GtkDrawingArea *, cairo_t *cr, int width, int height, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (width <= 0 || height <= 0) return;
    state->throbber_w = width;

    // At rest the strip IS the title bar: the resting shade equals the chrome
    // colour (@sl_chrome #24103f). Excitations move a column, relative to that
    // chrome base, toward a darker purple or an airy light blue -- so the seam
    // reads as the title-bar colour coming alive, not a separate band.
    //   dark ~ #190b2c    rest(chrome) ~ #24103f    light ~ #a9dcff
    auto mix = [](double c0, double c1, double k) { return c0 + (c1 - c0) * k; };
    const double rest_r = 0.141, rest_g = 0.063, rest_b = 0.247; // #24103f

    // Strong-amplification colour: only when the wattage field is driven hard
    // (~18/20) do latent hues (orange/green/blue/red) surface. This ramps from
    // 0 below ~0.80 amp to full by ~0.95, so normal operation stays on-brand.
    const double amp = state->throbber_amp;
    const double colour_gate = std::max(0.0, (amp - 0.80) / 0.15); // 0..1
    const double colour_gate_c = colour_gate > 1.0 ? 1.0 : colour_gate;

    for (int x = 0; x < width; ++x) {
        const double px = x + 0.5;

        // Accumulate the scalar field (how far this column departs from rest)
        // and, separately, the hue weighted by each tinted cell's contribution.
        double f = 0.0;
        double hr = 0.0, hg = 0.0, hb = 0.0, hw = 0.0;
        for (const auto &c : state->throbber_cells) {
            if (!c.alive) continue;
            double dx = px - c.x;
            // Asymmetric width: trailing (left) side longer -> comet tail that
            // points back the way it came, reinforcing left->right flow.
            double s = (dx < 0.0) ? c.sigma * 1.6 : c.sigma * 0.8;
            if (s < 0.5) s = 0.5;
            double e = dx / s;
            double bump = std::exp(-0.5 * e * e);
            double flick = 0.85 + 0.15 * std::sin(state->throbber_t * 6.0 + c.phase);
            double contrib = c.intensity * bump * flick;
            f += contrib;
            if (c.tinted) { hr += c.tr * contrib; hg += c.tg * contrib; hb += c.tb * contrib; hw += contrib; }
        }
        f *= (0.5 + 0.9 * amp);
        if (f > 1.2) f = 1.2;

        // Relative shade of the chrome base.
        double k = f;
        double r, g, b;
        if (k >= 0.0) {
            double kk = k > 1.0 ? 1.0 : k;
            r = mix(rest_r, 0.663, kk);   // chrome -> #a9dcff
            g = mix(rest_g, 0.863, kk);
            b = mix(rest_b, 1.000, kk);
        } else {
            double kk = -k; if (kk > 1.0) kk = 1.0;
            r = mix(rest_r, 0.098, kk);   // chrome -> #190b2c
            g = mix(rest_g, 0.043, kk);
            b = mix(rest_b, 0.173, kk);
        }

        // Under strong amplification, bend bright columns toward the local hue.
        if (colour_gate_c > 0.0 && hw > 0.0 && k > 0.0) {
            double thr = (hr / hw), tg2 = (hg / hw), tb2 = (hb / hw);
            double w = colour_gate_c * std::min(1.0, k);  // only where it's bright
            r = mix(r, thr, w);
            g = mix(g, tg2, w);
            b = mix(b, tb2, w);
        }

        r = std::clamp(r, 0.0, 1.0);
        g = std::clamp(g, 0.0, 1.0);
        b = std::clamp(b, 0.0, 1.0);
        cairo_set_source_rgb(cr, r, g, b);
        cairo_rectangle(cr, x, 0, 1, height);
        cairo_fill(cr);
    }
}

// Spawn a fresh excitation. Birth position spans the whole 0-100% of the strip
// width: most children enter at the left edge (preserving the dominant
// left->right feed), but a share are born anywhere across 0-100% and simply
// fade in at that spot (intensity eases up from 0). Either way the pulse then
// travels left->right all the way through to 100%.
void throbber_spawn(AppState *state, int W) {
    ThrobberCell c;
    c.alive = true;

    // Birth position: odds favour starting around 35% of the width, so the
    // flow then does most of its pulsing through the central sweet spot before
    // fading by ~65%. A minority still enter from the left edge (keeping the
    // left->right feed) or appear elsewhere, but the bulk cluster near 0.35W.
    const double roll = g_random_double();
    if (roll < 0.65) {
        // cluster around 35% with a modest spread (approx normal via two rolls)
        double gauss = (g_random_double() + g_random_double() - 1.0); // ~[-1,1], centre-weighted
        double frac = 0.35 + gauss * 0.12;                            // mostly 0.23..0.47
        if (frac < 0.0) frac = 0.0;
        if (frac > 1.0) frac = 1.0;
        c.x = frac * W;
    } else if (roll < 0.85) {
        c.x = -4.0 + g_random_double() * 6.0;        // enter from just off the left
    } else {
        c.x = g_random_double() * W;                 // occasional anywhere in 0-100%
    }

    // Varied speed: some children glide, some race. Wider spread so the field
    // reads as many independent reflexes rather than a uniform conveyor.
    c.v = 14.0 + g_random_double() * g_random_double() * 150.0; // skewed: mostly slow, a few fast
    c.a = 0.0;
    c.j = 0.0;
    c.base_intensity = 0.35 + g_random_double() * 0.6; // relative shade strength
    c.intensity = 0.0;                                  // eases up from birth (fade-in)
    c.energy = 1.0;
    c.sigma = 6.0 + g_random_double() * 14.0;        // glow half-width
    c.speed_mul = 0.45 + g_random_double() * g_random_double() * 2.6; // 0.45..~3x, skewed slow
    c.phase = g_random_double() * 6.2831853;
    // 1 in 30 children may pause or run right->left for 20-50 px.
    c.retro = (g_random_int_range(0, 30) == 0);
    c.retro_px = c.retro ? (20.0 + g_random_double() * 30.0) : 0.0;

    // Each child carries a latent hue (orange / green / blue / red). It stays
    // dormant at normal wattage and only surfaces under strong amplification
    // (see throbber_draw). Give most children a hue so colour can appear when
    // the field is driven hard.
    static const double hues[4][3] = {
        {1.00, 0.55, 0.10},  // orange
        {0.20, 0.85, 0.35},  // green
        {0.30, 0.60, 1.00},  // blue
        {1.00, 0.25, 0.25},  // red
    };
    c.tinted = (g_random_double() < 0.75);
    if (c.tinted) {
        const int h = g_random_int_range(0, 4);
        c.tr = hues[h][0]; c.tg = hues[h][1]; c.tb = hues[h][2];
    }

    state->throbber_cells.push_back(c);
}

// 20 Hz update of the living field.
gboolean throbber_tick(gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->throbber == nullptr) return G_SOURCE_REMOVE;

    const double dt = 1.0 / 20.0;
    state->throbber_t += dt;
    const int W = state->throbber_w > 0 ? state->throbber_w : 600;

    // --- Reflexive amplification field (smooth, 2nd order) --------------------
    // A slowly varying global gain that makes the whole field speed up / glow
    // more at "pertinent" moments, then relax. Driven to a wandering target
    // through a critically-damped-ish spring so it never snaps.
    double amp_target = 0.55
        + 0.30 * std::sin(state->throbber_t * 0.21)
        + 0.12 * std::sin(state->throbber_t * 0.53 + 1.3);
    if (amp_target < 0.1) amp_target = 0.1;
    if (amp_target > 1.0) amp_target = 1.0;
    double amp_acc = (amp_target - state->throbber_amp) * 2.2 - state->throbber_amp_v * 1.6;
    state->throbber_amp_v += amp_acc * dt;
    state->throbber_amp   += state->throbber_amp_v * dt;
    if (state->throbber_amp < 0.0) state->throbber_amp = 0.0;
    if (state->throbber_amp > 1.0) state->throbber_amp = 1.0;

    // --- Spawn rate scales with amplification (more wattage -> more children) -
    double rate = 6.0 + 10.0 * state->throbber_amp;   // children per second
    state->throbber_spawn_accum += rate * dt;
    while (state->throbber_spawn_accum >= 1.0) {
        state->throbber_spawn_accum -= 1.0;
        if (state->throbber_cells.size() < 64) throbber_spawn(state, W);
    }

    // --- Advance each child with 3rd-order (jerk-driven) motion ---------------
    for (auto &c : state->throbber_cells) {
        if (!c.alive) continue;

        // Reflexive, amplified desired speed, scaled by this cell's own speed
        // character so some pulses glide and others race across the sweet spot.
        double desired_v = (18.0 + 70.0 * state->throbber_amp) * c.speed_mul;

        if (c.retro && c.retro_px > 0.0) {
            // This rare child pauses or glides right->left for its allotment.
            desired_v = -(10.0 + 25.0 * state->throbber_amp);
            c.retro_px -= std::fabs(c.v) * dt;
        }

        // 3rd order: steer JERK toward closing the velocity error, integrate
        // jerk -> accel -> vel -> pos. This keeps the *rate of change of
        // acceleration* bounded, so speed-ups/slow-downs are gentle, not steppy.
        double v_err = desired_v - c.v;
        double jerk_target = v_err * 6.0;                       // reflexive gain
        c.j += (jerk_target - c.j) * 0.35;                      // ease the jerk
        c.a += c.j * dt;
        if (c.a >  900.0) c.a =  900.0;
        if (c.a < -900.0) c.a = -900.0;
        c.v += c.a * dt;
        c.x += c.v * dt;

        // --- Centre sweet spot (40-60%), soft shoulders to ~35/65% ----------
        // The pulsing concentrates in the middle of the strip. A column's
        // "centre weight" is ~1 across 40-60% and falls off outside it; pulses
        // that drift away from centre both glow less AND decay faster, so the
        // visible life of a pulse is spent mainly in the 35-65% band.
        const double frac = (W > 0) ? (c.x / W) : 0.5;
        double dist = std::fabs(frac - 0.50);
        double center_weight;
        if (dist <= 0.10) {
            center_weight = 1.0;                         // flat top across 40-60%
        } else {
            // Gaussian shoulders beyond +/-10%: ~0.37 at the 35/65% edges, less
            // further out, so off-centre pulses clearly recede.
            double e = (dist - 0.10) / 0.07;
            center_weight = std::exp(-0.5 * e * e);
        }

        // Energy (the "amplification series") decays faster the farther the
        // pulse is from the sweet spot: in-band it lingers, out-of-band it dies
        // off quickly because it "wasn't of the sweet spot".
        double decay = 0.10 + 0.05 * state->throbber_amp
                     + 0.9 * (1.0 - center_weight);       // off-centre penalty
        c.energy -= dt * decay;
        if (c.energy < 0.0) c.energy = 0.0;

        double edge_fade = 1.0;
        if (c.x > W * 0.90) edge_fade = std::max(0.0, (W - c.x) / (W * 0.10));
        double breath = 0.9 + 0.1 * std::sin(state->throbber_t * 1.7 + c.phase);
        // Brightness is gated by centre proximity, so the pulsing lives in the
        // middle and less-centred pulses are already fading relative to it.
        double target_i = c.base_intensity * center_weight * edge_fade * c.energy * breath;
        // Ease the live intensity toward its target so fade-in / fade-out smooth.
        c.intensity += (target_i - c.intensity) * 0.30;

        // The pulse still travels left->right across the whole width; it only
        // dies on exit at the right edge (or once it has faded to nothing well
        // past the sweet spot, so spent pulses don't linger invisibly).
        if (c.x > W + 6.0) c.alive = false;
        else if (frac > 0.70 && c.energy <= 0.0 && c.intensity < 0.01) c.alive = false;
    }

    // Compact the pool: drop dead cells.
    state->throbber_cells.erase(
        std::remove_if(state->throbber_cells.begin(), state->throbber_cells.end(),
                       [](const ThrobberCell &c) { return !c.alive; }),
        state->throbber_cells.end());

    gtk_widget_queue_draw(state->throbber);
    return G_SOURCE_CONTINUE;
}

void activate(GtkApplication *application, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    install_css();
    load_config(state->config);

    GtkWidget *window = gtk_application_window_new(application);
    state->window = GTK_WINDOW(window);
    gtk_window_set_title(state->window, kWindowTitle);
    gtk_window_set_default_size(state->window, 1100, 700);

    GtkWidget *header = gtk_header_bar_new();
    gtk_widget_add_css_class(header, "sleela-titlebar");
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE);

    // Brand logo in the upper-left of the title bar. The asset is pre-trimmed
    // to the logo's minimum 2D content box with a transparent background (see
    // tools/logo/Trim.java), so it sits flush at the left with no surrounding
    // whitespace. Packed first so it is the leftmost title-bar element.
    GtkWidget *logo = gtk_image_new_from_file(asset_path(state, "titlebar-logo.png").c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(logo), 22);
    gtk_widget_add_css_class(logo, "sleela-titlebar-logo");
    gtk_widget_set_valign(logo, GTK_ALIGN_CENTER);
    gtk_widget_set_tooltip_text(logo, "SleelaTerminal™ — Debian / Windows Terminal");
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), logo);

    // Settings control on the RIGHT of the title bar: a gear icon (the
    // conventional Settings glyph), flat/quiet by default with only a soft
    // hover tint, ~25% smaller than the other title controls. Clicking it opens
    // the Settings subframe window. Packed at the end so it sits on the right.
    GtkWidget *settings = gtk_button_new();
    GtkWidget *settings_image = gtk_image_new_from_icon_name("emblem-system-symbolic");
    gtk_image_set_pixel_size(GTK_IMAGE(settings_image), 16);
    gtk_button_set_child(GTK_BUTTON(settings), settings_image);
    gtk_widget_set_tooltip_text(settings, "SleelaTerminal Settings");
    gtk_widget_add_css_class(settings, "sleela-settings-btn");
    gtk_widget_set_valign(settings, GTK_ALIGN_CENTER);
    g_object_set_data(G_OBJECT(settings), "sleela-state", state);
    g_signal_connect(settings, "clicked", G_CALLBACK(open_settings), settings);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header), settings);

    GtkWidget *title = gtk_label_new(kWindowTitle);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), title);
    gtk_window_set_titlebar(state->window, header);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // Living throbber strip: a thin drawing area immediately below the title
    // bar, replacing the old static bottom border. Its light-blue shade is
    // repainted ~20x/second by throbber_tick while the terminal is open.
    GtkWidget *throbber = gtk_drawing_area_new();
    state->throbber = throbber;
    gtk_widget_add_css_class(throbber, "sleela-throbber");
    gtk_widget_set_hexpand(throbber, TRUE);
    gtk_widget_set_size_request(throbber, -1, 2);
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(throbber), throbber_draw, state, nullptr);
    gtk_box_append(GTK_BOX(root), throbber);

    GtkWidget *terminal = vte_terminal_new();
    state->terminal = VTE_TERMINAL(terminal);
    gtk_widget_set_hexpand(terminal, TRUE);
    gtk_widget_set_vexpand(terminal, TRUE);
    vte_terminal_set_scrollback_lines(state->terminal, 10000);
    apply_terminal_style(state);

    GtkEventController *keys = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(keys, GTK_PHASE_CAPTURE);
    g_signal_connect(keys, "key-pressed", G_CALLBACK(terminal_key_pressed), terminal);
    gtk_widget_add_controller(terminal, keys);

    GtkGesture *right_click = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(right_click), GDK_BUTTON_SECONDARY);
    g_signal_connect(right_click, "pressed", G_CALLBACK(terminal_mouse_menu), terminal);
    gtk_widget_add_controller(terminal, GTK_EVENT_CONTROLLER(right_click));
    gtk_box_append(GTK_BOX(root), terminal);

    GtkWidget *footer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class(footer, "sleela-footer");
    gtk_widget_set_hexpand(footer, TRUE);

    GtkWidget *brand = gtk_label_new("SLeeLa");
    gtk_widget_add_css_class(brand, "sleela-footer-brand");
    gtk_widget_set_margin_start(brand, 12);
    gtk_widget_set_margin_end(brand, 4);
    gtk_widget_set_valign(brand, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), brand);

    GtkWidget *separator = gtk_label_new("│");
    gtk_widget_add_css_class(separator, "sleela-footer-separator");
    gtk_widget_set_valign(separator, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), separator);

    GtkWidget *footer_text = gtk_label_new(footer_messages().front().c_str());
    state->footer_text = GTK_LABEL(footer_text);
    gtk_widget_add_css_class(footer_text, "sleela-footer-ticker");
    gtk_widget_set_hexpand(footer_text, TRUE);
    gtk_widget_set_halign(footer_text, GTK_ALIGN_START);
    gtk_widget_set_valign(footer_text, GTK_ALIGN_CENTER);
    gtk_label_set_ellipsize(state->footer_text, PANGO_ELLIPSIZE_END);
    gtk_box_append(GTK_BOX(footer), footer_text);

    // CMD footer control: strict image only; the image itself is the clickable surface.
    GtkWidget *java_button = gtk_button_new();
    gtk_widget_add_css_class(java_button, "sleela-footer-java");
    gtk_widget_set_tooltip_text(java_button, "CMD — Java native launcher / SecureJDK 28 software center");
    GtkWidget *java_image = gtk_image_new_from_file(asset_path(state, "cmd.svg").c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(java_image), 24); gtk_button_set_child(GTK_BUTTON(java_button), java_image);
    g_object_set_data(G_OBJECT(java_button), "sleela-state", state); g_signal_connect(java_button, "clicked", G_CALLBACK(open_software_menu), java_button); gtk_box_append(GTK_BOX(footer), java_button);

    GtkWidget *status = gtk_label_new(kVersion);
    gtk_widget_set_margin_start(status, 8);
    gtk_widget_set_margin_end(status, 12);
    gtk_widget_set_valign(status, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), status);

    gtk_box_append(GTK_BOX(root), footer);
    gtk_window_set_child(state->window, root);
    g_signal_connect(terminal, "child-exited", G_CALLBACK(child_exited), state);
    g_signal_connect(state->window, "close-request", G_CALLBACK(window_close_request), state);

    // Resting-state status is static: moving/scrolling text competes with the
    // terminal content, which is the hero. See UI-PRINCIPLES.md (principle 6).
    // The footer shows a single clear status string; no rotation timer runs.
    state->footer_position = 0;
    state->footer_tick = 0;

    // Drive the throbber at 20 Hz (every 50 ms) for as long as the window lives.
    // The timer is removed in child_exited / window_close_request.
    state->throbber_tick = g_timeout_add(50, throbber_tick, state);

    std::vector<char *> shell_argv;
    shell_argv.push_back(const_cast<char *>(state->shell_path.c_str()));
    shell_argv.push_back(nullptr);
    vte_terminal_spawn_async(
        state->terminal,
        VTE_PTY_DEFAULT,
        nullptr,
        shell_argv.data(),
        nullptr,
        G_SPAWN_DEFAULT,
        nullptr,
        nullptr,
        nullptr,
        -1,
        nullptr,
        shell_spawned,
        state);

    gtk_window_present(state->window);
}

} // namespace

int main(int argc, char **argv) {
    AppState state;
    if (argc > 1) state.shell_path = argv[1];
    else state.shell_path = sibling_path(argv[0], "slsh");
    state.executable_path = argv[0];

    GtkApplication *application = gtk_application_new(
        kApplicationId, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(application, "activate", G_CALLBACK(activate), &state);
    int status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);
    return status;
}