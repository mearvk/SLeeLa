#include <gtk/gtk.h>
#include <vte/vte.h>

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

// CMD is the Java native launcher associated with SecureJDK 28.\n// The footer CMD image is a strict image control: no button outline or frame.\nstd::string installer_path(const AppState *state) {
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
    gdk_rgba_parse(&background, "#111111");
    vte_terminal_set_colors(state->terminal, &foreground, &background, nullptr, 0);
}

void install_css() {
    const char *css = R"CSS(
        window { background: #111111; }
        headerbar.sleela-titlebar {
            background: #24103f;
            color: #ffffff;
            min-height: 38px;
            border-bottom: 1px solid #3f2463;
        }
        headerbar.sleela-titlebar label { color: #ffffff; font-weight: 700; }
        headerbar.sleela-titlebar button.titlebutton {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.18);
            -gtk-icon-style: symbolic;
            -gtk-icon-shadow: 0 0 4px rgba(255, 255, 255, 1);
            min-width: 36px;
            min-height: 32px;
            opacity: 1;
            transition: 150ms ease-in-out;
        }
        headerbar.sleela-titlebar button.titlebutton image {
            color: #ffffff;
            opacity: 1;
            -gtk-icon-shadow: 0 0 4px rgba(255, 255, 255, 1);
        }
        headerbar.sleela-titlebar button.titlebutton:hover,
        headerbar.sleela-titlebar button.titlebutton:focus {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.34);
            -gtk-icon-shadow: 0 0 7px rgba(255, 255, 255, 1);
        }
        headerbar.sleela-titlebar button.titlebutton:active {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.44);
            -gtk-icon-shadow: 0 0 8px rgba(255, 255, 255, 1);
        }
        box.sleela-footer {
            min-height: 38px;
            background: linear-gradient(to bottom, #5b2f88, #3a1d5c 48%, #2a1644);
            border-top: 1px solid #7f56aa;
            border-bottom: 1px solid #160d24;
            box-shadow: inset 0 1px rgba(255, 255, 255, 0.28), inset 0 -2px rgba(0, 0, 0, 0.42);
        }
        box.sleela-footer label {
            color: #ffffff;
            font-weight: 600;
            text-shadow: 0 1px 2px rgba(0, 0, 0, 0.8);
        }
        label.sleela-footer-brand { font-weight: 800; letter-spacing: 0.5px; }
        label.sleela-footer-separator { color: rgba(255, 255, 255, 0.42); padding-left: 6px; padding-right: 6px; }
        label.sleela-footer-ticker { padding-left: 8px; padding-right: 8px; }
        button.sleela-footer-java { min-width:34px; min-height:34px; padding:1px; margin:1px 5px; background:rgba(255,255,255,.08); border:1px solid rgba(255,255,255,.28); border-radius:7px; }
        button.sleela-footer-java:hover { background:rgba(255,255,255,.22); }
        popover.sleela-software contents { background:linear-gradient(to bottom,#321a4d,#21112f); border:1px solid #7f56aa; }
        popover.sleela-software label { color:#ffffff; }
        popover.sleela-software button { color:#ffffff; background:rgba(255,255,255,.08); min-width:260px; min-height:32px; text-align:left; }
        popover.sleela-software button:hover { background:rgba(255,255,255,.18); }
        label.sleela-software-note { color:rgba(255,255,255,.72); font-size:.9em; }
        textview.sleela-scan-output { color:#ffffff; background:#130d1c; }
        popover.sleela-settings contents {
            background: linear-gradient(to bottom, #321a4d, #21112f);
            border: 1px solid #7f56aa;
        }
        popover.sleela-settings label { color: #ffffff; }
        popover.sleela-settings label.section { font-weight: 800; margin-top: 5px; }
        popover.sleela-settings button {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.08);
            min-width: 250px;
            min-height: 32px;
            text-align: left;
        }
        popover.sleela-settings button:hover { background: rgba(255, 255, 255, 0.18); }
        label.sleela-trust-note {
            color: rgba(255, 255, 255, 0.70);
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
    GtkPopover *popover = GTK_POPOVER(gtk_popover_new());
    gtk_widget_add_css_class(GTK_WIDGET(popover), "sleela-settings");
    gtk_widget_set_parent(GTK_WIDGET(popover), settings_button_widget);
    gtk_popover_set_has_arrow(popover, TRUE);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_margin_start(box, 12);
    gtk_widget_set_margin_end(box, 12);
    gtk_widget_set_margin_top(box, 10);
    gtk_widget_set_margin_bottom(box, 10);

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

    gtk_popover_set_child(popover, box);
    gtk_popover_set_autohide(popover, TRUE);
    gtk_popover_popup(popover);
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
    gtk_widget_set_margin_top(box, 10); gtk_widget_set_margin_bottom(box, 10);
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

void open_software_menu(GtkButton *, gpointer user_data) {
    auto *button = static_cast<GtkWidget *>(user_data);
    auto *state = static_cast<AppState *>(g_object_get_data(G_OBJECT(button), "sleela-state"));
    make_software_menu(state, button);
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
    static const std::vector<std::string> messages = {
        "SleelaTerminal™ 1.0.0  •  Terminal Ready",
        "SLeeLa  •  Secure Shell Interface  •  MEARVK LLC",
        "M1–M5 Shell Architecture  •  GTK 4  •  VTE",
        "Status  •  Interactive terminal session active",
        "Orientation  •  Local configuration  •  Human-readable declarations"
    };
    return messages;
}

gboolean footer_tick(gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->footer_text == nullptr) return G_SOURCE_CONTINUE;
    const auto &messages = footer_messages();
    if (messages.empty()) return G_SOURCE_CONTINUE;
    const std::string &message = messages[state->footer_position % messages.size()];
    gtk_label_set_text(state->footer_text, message.c_str());
    state->footer_position = (state->footer_position + 1) % messages.size();
    return G_SOURCE_CONTINUE;
}

void child_exited(VteTerminal *, int, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->footer_tick != 0) {
        g_source_remove(state->footer_tick);
        state->footer_tick = 0;
    }
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
    gtk_window_destroy(window);
    g_application_quit(G_APPLICATION(gtk_window_get_application(window)));
    return TRUE;
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

    GtkWidget *settings = gtk_button_new();
    GtkWidget *settings_image = gtk_image_new_from_icon_name("open-menu-symbolic");
    gtk_button_set_child(GTK_BUTTON(settings), settings_image);
    gtk_widget_set_tooltip_text(settings, "SleelaTerminal Settings and Orientation");
    gtk_widget_add_css_class(settings, "titlebutton");
    g_object_set_data(G_OBJECT(settings), "sleela-state", state);
    g_signal_connect(settings, "clicked", G_CALLBACK(open_settings), settings);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header), settings);

    GtkWidget *title = gtk_label_new(kWindowTitle);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), title);
    gtk_window_set_titlebar(state->window, header);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
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
    gtk_widget_set_margin_start(brand, 14);
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

    // CMD footer control: strict image only; the image itself is the clickable surface.\n    GtkWidget *java_button = gtk_button_new();
    gtk_widget_add_css_class(java_button, "sleela-footer-java");
    gtk_widget_set_tooltip_text(java_button, "CMD — Java native launcher / SecureJDK 28 software center");
    GtkWidget *java_image = gtk_image_new_from_file(asset_path(state, "cmd.svg").c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(java_image), 30); gtk_button_set_child(GTK_BUTTON(java_button), java_image);
    g_object_set_data(G_OBJECT(java_button), "sleela-state", state); g_signal_connect(java_button, "clicked", G_CALLBACK(open_software_menu), java_button); gtk_box_append(GTK_BOX(footer), java_button);

    GtkWidget *status = gtk_label_new(kVersion);
    gtk_widget_set_margin_start(status, 8);
    gtk_widget_set_margin_end(status, 14);
    gtk_widget_set_valign(status, GTK_ALIGN_CENTER);
    gtk_box_append(GTK_BOX(footer), status);

    gtk_box_append(GTK_BOX(root), footer);
    gtk_window_set_child(state->window, root);
    g_signal_connect(terminal, "child-exited", G_CALLBACK(child_exited), state);
    g_signal_connect(state->window, "close-request", G_CALLBACK(window_close_request), state);

    state->footer_position = 1;
    state->footer_tick = g_timeout_add(5000, footer_tick, state);

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