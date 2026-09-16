from pathlib import Path

path = Path('sleela-terminal/sleelaterminal-gui.cpp')
s = path.read_text()
if 'sleela-footer-java' in s:
    raise SystemExit('footer integration already present')

s = s.replace('constexpr const char *kConfigName = "sleela-terminal.conf";\n', 'constexpr const char *kConfigName = "sleela-terminal.conf";\nconstexpr const char *kVersion = "1.0.0";\n')
s = s.replace('GtkLabel *footer_text = nullptr;\n', 'GtkLabel *footer_text = nullptr;\n    std::string executable_path;\n')
marker = 'std::string sibling_path(const char *argv0, const char *name) {'
helpers = r'''std::string asset_path(const AppState *state, const char *name) {
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

'''
s = s.replace(marker, helpers + marker)
css = '''        button.sleela-footer-java { min-width:34px; min-height:34px; padding:1px; margin:1px 5px; background:rgba(255,255,255,.08); border:1px solid rgba(255,255,255,.28); border-radius:7px; }
        button.sleela-footer-java:hover { background:rgba(255,255,255,.22); }
        popover.sleela-software contents { background:linear-gradient(to bottom,#321a4d,#21112f); border:1px solid #7f56aa; }
        popover.sleela-software label { color:#ffffff; }
        popover.sleela-software button { color:#ffffff; background:rgba(255,255,255,.08); min-width:260px; min-height:32px; text-align:left; }
        popover.sleela-software button:hover { background:rgba(255,255,255,.18); }
        label.sleela-software-note { color:rgba(255,255,255,.72); font-size:.9em; }
        textview.sleela-scan-output { color:#ffffff; background:#130d1c; }
'''
s = s.replace('        label.sleela-footer-ticker { padding-left: 8px; padding-right: 8px; }\n', '        label.sleela-footer-ticker { padding-left: 8px; padding-right: 8px; }\n' + css)
funcs = r'''
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
    GSubprocess *process = g_subprocess_newv(argv, G_SUBPROCESS_FLAGS_STDOUT_PIPE | G_SUBPROCESS_FLAGS_STDERR_PIPE, &error);
    if (!process) {
        gtk_text_buffer_set_text(buffer, error ? error->message : "Unable to start software operation", -1);
        if (error) g_error_free(error); return;
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
        }), payload, [](gpointer data) { delete static_cast<Payload *>(data); }, G_CONNECT_AFTER);
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
'''
s = s.replace('// Ctrl+C is copy when VTE has a selection.', funcs + '\n// Ctrl+C is copy when VTE has a selection.')
footer = '''    GtkWidget *java_button = gtk_button_new();
    gtk_widget_add_css_class(java_button, "sleela-footer-java");
    gtk_widget_set_tooltip_text(java_button, "SecureJDK 28 / CMD — Scan GitHub and install or prepare latest software");
    GtkWidget *java_image = gtk_image_new_from_file(asset_path(state, "securejdk28-cmd.svg").c_str());
    gtk_image_set_pixel_size(GTK_IMAGE(java_image), 30); gtk_button_set_child(GTK_BUTTON(java_button), java_image);
    g_object_set_data(G_OBJECT(java_button), "sleela-state", state); g_signal_connect(java_button, "clicked", G_CALLBACK(open_software_menu), java_button); gtk_box_append(GTK_BOX(footer), java_button);

'''
s = s.replace('    GtkWidget *status = gtk_label_new("1.0.0");', footer + '    GtkWidget *status = gtk_label_new(kVersion);')
s = s.replace('    if (argc > 1) state.shell_path = argv[1];\n    else state.shell_path = sibling_path(argv[0], "slsh");', '    if (argc > 1) state.shell_path = argv[1];\n    else state.shell_path = sibling_path(argv[0], "slsh");\n    state.executable_path = argv[0];')
path.write_text(s)
