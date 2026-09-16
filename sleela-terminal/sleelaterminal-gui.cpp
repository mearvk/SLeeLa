#include <gtk/gtk.h>
#include <vte/vte.h>

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
    if (ec) return name;
    fs::path candidate = executable.parent_path() / name;
    if (fs::exists(candidate, ec) && !ec) return candidate.string();
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
            background: rgba(255, 255, 255, 0.10);
            -gtk-icon-style: symbolic;
            -gtk-icon-shadow: 0 0 2px rgba(255, 255, 255, 0.85);
            min-width: 34px;
            min-height: 30px;
            opacity: 1;
            transition: 150ms ease-in-out;
        }
        headerbar.sleela-titlebar button.titlebutton image {
            color: #ffffff;
            opacity: 1;
            -gtk-icon-shadow: 0 0 2px rgba(255, 255, 255, 0.85);
        }
        headerbar.sleela-titlebar button.titlebutton:hover,
        headerbar.sleela-titlebar button.titlebutton:focus {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.28);
            -gtk-icon-shadow: 0 0 5px rgba(255, 255, 255, 1);
        }
        headerbar.sleela-titlebar button.titlebutton:hover image,
        headerbar.sleela-titlebar button.titlebutton:focus image {
            color: #ffffff;
            opacity: 1;
            -gtk-icon-shadow: 0 0 5px rgba(255, 255, 255, 1);
        }
        headerbar.sleela-titlebar button.titlebutton:active {
            color: #ffffff;
            background: rgba(255, 255, 255, 0.38);
            -gtk-icon-shadow: 0 0 6px rgba(255, 255, 255, 1);
        }
        headerbar.sleela-titlebar button.titlebutton:active image {
            color: #ffffff;
            opacity: 1;
            -gtk-icon-shadow: 0 0 6px rgba(255, 255, 255, 1);
        }
        headerbar.sleela-titlebar button.titlebutton:backdrop {
            color: #eeeeee;
            background: rgba(255, 255, 255, 0.06);
            -gtk-icon-shadow: 0 0 2px rgba(255, 255, 255, 0.55);
        }
    )CSS";

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_string(provider, css);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(), GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

// Ctrl+C is copy when VTE has a selection. Without a selection it is left to
// the PTY so the shell/foreground program receives the normal interrupt key.
// Ctrl+Shift+C always copies. Ctrl+V and Ctrl+Shift+V paste into the PTY.
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

void terminal_paste(GtkWidget *, gpointer user_data) {
    vte_terminal_paste_clipboard(VTE_TERMINAL(user_data));
}

void terminal_select_all(GtkWidget *, gpointer user_data) {
    vte_terminal_select_all(VTE_TERMINAL(user_data));
}

void terminal_clear_selection(GtkWidget *, gpointer user_data) {
    vte_terminal_unselect_all(VTE_TERMINAL(user_data));
}

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
    double x = 0.0;
    double y = 0.0;
    gtk_gesture_get_point(GTK_GESTURE(gesture), nullptr, &x, &y);
    rect.x = static_cast<int>(x);
    rect.y = static_cast<int>(y);
    rect.width = 1;
    rect.height = 1;
    gtk_popover_set_pointing_to(popover, &rect);
    gtk_popover_popup(popover);
}

void child_exited(VteTerminal *, int, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);
    state->child_pid = 0;

    if (state->window != nullptr) {
        GtkApplication *application = gtk_window_get_application(state->window);
        GtkWindow *window = state->window;
        state->window = nullptr;
        gtk_window_destroy(window);
        if (application != nullptr) {
            g_application_quit(G_APPLICATION(application));
        }
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

    gtk_window_destroy(window);
    g_application_quit(G_APPLICATION(gtk_window_get_application(window)));
    return TRUE;
}

void activate(GtkApplication *application, gpointer user_data) {
    auto *state = static_cast<AppState *>(user_data);

    install_css();

    GtkWidget *window = gtk_application_window_new(application);
    state->window = GTK_WINDOW(window);
    gtk_window_set_title(state->window, kWindowTitle);
    gtk_window_set_default_size(state->window, 1100, 700);

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

    GtkEventController *keys = gtk_event_controller_key_new();
    gtk_event_controller_set_propagation_phase(keys, GTK_PHASE_CAPTURE);
    g_signal_connect(keys, "key-pressed", G_CALLBACK(terminal_key_pressed), terminal);
    gtk_widget_add_controller(terminal, keys);

    GtkGesture *right_click = gtk_gesture_click_new();
    gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(right_click), GDK_BUTTON_SECONDARY);
    g_signal_connect(right_click, "pressed", G_CALLBACK(terminal_mouse_menu), terminal);
    gtk_widget_add_controller(terminal, GTK_EVENT_CONTROLLER(right_click));

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