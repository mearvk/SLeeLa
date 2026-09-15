#include <gtk/gtk.h>
#include <vte/vte.h>
#include <gdk/gdkkeysyms.h>

#include <filesystem>
#include <csignal>
#include <string>
#include <vector>

namespace {

constexpr const char *kApplicationId = "com.mearvk.SleelaTerminal";
constexpr const char *kWindowTitle = "SleelaTerminal™ — MEARVK LLC";

struct AppState {
    std::string shell_path;
    GtkWindow *window = nullptr;
    GPid child_pid = 0;
};

std::string sibling_path(const char *argv0, const char *name) {
    namespace fs = std::filesystem;
    std::error_code ec;
    fs::path executable = fs::absolute(argv0, ec);
    if (ec) {
        return name;
    }
    fs::path candidate = executable.parent_path() / name;
    if (fs::exists(candidate, ec) && !ec) {
        return candidate.string();
    }
    return name;
}

void install_css() {
    const char *css = R"CSS(
        window {
            background: #111111;
        }
        headerbar.sleela-titlebar {
            background: #24103f;
            color: #ffffff;
            min-height: 38px;
            border-bottom: 1px solid #3f2463;
        }
        headerbar.sleela-titlebar label {
            color: #ffffff;
            font-weight: 700;
        }
        headerbar.sleela-titlebar button.titlebutton {
            color: #ffffff;
            background: transparent;
            -gtk-icon-style: symbolic;
            opacity: 1;
        }
        headerbar.sleela-titlebar button.titlebutton:hover,
        headerbar.sleela-titlebar button.titlebutton:focus {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.10);
        }
        headerbar.sleela-titlebar button.titlebutton:active {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.18);
        }
    )CSS";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

void child_exited(VteTerminal *, int, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    state->child_pid = 0;
    if (state->window != nullptr) {
        gtk_window_set_title(state->window, kWindowTitle);
    }
}

// VTE's GTK4 API does not expose vte_terminal_get_child_pid(). The spawn
// completion callback supplies the child PID, so retain it in AppState and
// use that PID when the window is closed.
void shell_spawned(VteTerminal *, GPid child_pid, GError *error, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (error != nullptr || child_pid <= 0) {
        state->child_pid = 0;
        return;
    }
    state->child_pid = child_pid;
}

// Closing the GUI window must close the PTY-backed shell as well. Without an
// explicit close handler GTK can destroy the window while the VTE child keeps
// running, leaving slsh alive after the terminal window disappears.
gboolean window_close_request(GtkWindow *window, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    if (state->child_pid > 0) {
        ::kill(static_cast<pid_t>(state->child_pid), SIGHUP);
        state->child_pid = 0;
    }

    // Explicitly quit the GtkApplication so closing the terminal window has
    // the same lifecycle semantics as typing "exit" in slsh.
    gtk_window_destroy(window);
    g_application_quit(G_APPLICATION(gtk_window_get_application(window)));
    return TRUE;
}

// Ctrl+C and Ctrl+X are treated as GUI-level close shortcuts. Capture-phase
// handling is intentional: VTE would normally consume Ctrl+C as SIGINT, so
// the window must see the shortcut before it reaches the terminal PTY.
gboolean key_pressed(GtkEventControllerKey *, guint keyval, guint, GdkModifierType modifiers,
                     gpointer user_data) {
    if ((modifiers & GDK_CONTROL_MASK) == 0) {
        return FALSE;
    }

    if (keyval != GDK_KEY_c && keyval != GDK_KEY_C &&
        keyval != GDK_KEY_x && keyval != GDK_KEY_X) {
        return FALSE;
    }

    auto *state = static_cast<AppState *>(user_data);
    if (state->window != nullptr) {
        window_close_request(state->window, state);
    }
    return TRUE;
}

void activate(GtkApplication *application, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);

    install_css();

    GtkWidget *window = gtk_application_window_new(application);
    state->window = GTK_WINDOW(window);
    gtk_window_set_title(state->window, kWindowTitle);
    gtk_window_set_default_size(state->window, 1100, 700);

    GtkEventController *key_controller = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(
        GTK_EVENT_CONTROLLER(key_controller), GTK_PHASE_CAPTURE);
    g_signal_connect(key_controller, "key-pressed", G_CALLBACK(key_pressed), state);
    gtk_widget_add_controller(window, key_controller);

    GtkWidget *header = gtk_header_bar_new();
    gtk_widget_add_css_class(header, "sleela-titlebar");
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE);

    GtkWidget *title = gtk_label_new(kWindowTitle);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), title);
    gtk_window_set_titlebar(state->window, header);

    GtkWidget *terminal = vte_terminal_new();
    gtk_widget_set_hexpand(terminal, TRUE);
    gtk_widget_set_vexpand(terminal, TRUE);
    vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal), 10000);

    PangoFontDescription *font = pango_font_description_from_string("Monospace 11");
    vte_terminal_set_font(VTE_TERMINAL(terminal), font);
    pango_font_description_free(font);

    gtk_window_set_child(state->window, terminal);
    g_signal_connect(terminal, "child-exited", G_CALLBACK(child_exited), state);
    g_signal_connect(state->window, "close-request", G_CALLBACK(window_close_request), state);

    std::vector<char *> shell_argv;
    shell_argv.push_back(const_cast<char *>(state->shell_path.c_str()));
    shell_argv.push_back(nullptr);

    vte_terminal_spawn_async(
        VTE_TERMINAL(terminal),
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
    if (argc > 1) {
        state.shell_path = argv[1];
    } else {
        state.shell_path = sibling_path(argv[0], "slsh");
    }

    GtkApplication *application = gtk_application_new(
        kApplicationId, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(application, "activate", G_CALLBACK(activate), &state);

    int status = g_application_run(G_APPLICATION(application), argc, argv);
    g_object_unref(application);
    return status;
}
