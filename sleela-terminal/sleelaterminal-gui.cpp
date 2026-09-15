#include <gtk/gtk.h>
#include <vte/vte.h>

#include <filesystem>
#include <string>
#include <vector>

namespace {

constexpr const char *kApplicationId = "com.mearvk.SleelaTerminal";
constexpr const char *kWindowTitle = "SleelaTerminal™ — MEARVK LLC";

struct AppState {
    std::string shell_path;
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
    )CSS";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

void child_exited(VteTerminal *, int, gpointer user_data) {
    GtkWindow *window = GTK_WINDOW(user_data);
    gtk_window_set_title(window, kWindowTitle);
}

void activate(GtkApplication *application, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);

    install_css();

    GtkWidget *window = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(window), kWindowTitle);
    gtk_window_set_default_size(GTK_WINDOW(window), 1100, 700);

    GtkWidget *header = gtk_header_bar_new();
    gtk_widget_add_css_class(header, "sleela-titlebar");
    gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header), TRUE);

    GtkWidget *title = gtk_label_new(kWindowTitle);
    gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header), title);
    gtk_window_set_titlebar(GTK_WINDOW(window), header);

    GtkWidget *terminal = vte_terminal_new();
    gtk_widget_set_hexpand(terminal, TRUE);
    gtk_widget_set_vexpand(terminal, TRUE);
    vte_terminal_set_scrollback_lines(VTE_TERMINAL(terminal), 10000);

    PangoFontDescription *font = pango_font_description_from_string("Monospace 11");
    vte_terminal_set_font(VTE_TERMINAL(terminal), font);
    pango_font_description_free(font);

    gtk_window_set_child(GTK_WINDOW(window), terminal);
    g_signal_connect(terminal, "child-exited", G_CALLBACK(child_exited), window);

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
        nullptr,
        nullptr);

    gtk_window_present(GTK_WINDOW(window));
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
