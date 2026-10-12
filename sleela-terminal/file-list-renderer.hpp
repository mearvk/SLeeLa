#pragma once

#include <gtk/gtk.h>

#include <algorithm>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

namespace sleela::terminal {

namespace file_items_detail {

struct BrowserState {
    GtkWindow *window = nullptr;
    GtkWidget *path_entry = nullptr;
    GtkWidget *items = nullptr;
    std::filesystem::path directory;
};

inline void show_error(GtkWindow *parent, const std::string &message) {
    GtkWidget *dialog = gtk_message_dialog_new(
        parent, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", message.c_str());
    g_signal_connect(dialog, "response", G_CALLBACK(+[](GtkDialog *d, int, gpointer) {
        gtk_window_destroy(GTK_WINDOW(d));
    }), nullptr);
    gtk_window_present(GTK_WINDOW(dialog));
}

inline void rounded_item(GtkWidget *button, bool directory) {
    gtk_widget_add_css_class(button, "sleela-file-item");
    gtk_widget_add_css_class(button, directory ? "sleela-file-directory" : "sleela-file-regular");
}

inline void populate(BrowserState *state);

inline void navigate(BrowserState *state, const std::filesystem::path &path) {
    std::error_code ec;
    const auto canonical = std::filesystem::weakly_canonical(path, ec);
    if (ec || !std::filesystem::is_directory(canonical, ec)) {
        show_error(state->window, "Cannot open directory: " + path.string());
        return;
    }
    state->directory = canonical;
    gtk_editable_set_text(GTK_EDITABLE(state->path_entry), state->directory.string().c_str());
    populate(state);
}

inline void item_clicked(GtkButton *button, gpointer data) {
    auto *state = static_cast<BrowserState *>(data);
    const char *name = static_cast<const char *>(g_object_get_data(G_OBJECT(button), "sleela-file-name"));
    if (!name) return;
    const auto path = state->directory / name;
    std::error_code ec;
    if (std::filesystem::is_directory(path, ec)) {
        navigate(state, path);
        return;
    }
    GError *error = nullptr;
    const std::string native_path = path.string();
    GFile *file = g_file_new_for_path(native_path.c_str());
    gchar *content_type = g_content_type_guess(native_path.c_str(), nullptr, 0, nullptr);
    GAppInfo *app = content_type ? g_app_info_get_default_for_type(content_type, FALSE) : nullptr;
    g_free(content_type);
    if (app) {
        GList *files = nullptr;
        files = g_list_append(files, file);
        if (!g_app_info_launch(app, files, nullptr, &error)) {
            show_error(state->window, error ? error->message : "Could not open file.");
            g_clear_error(&error);
        }
        g_list_free(files);
        g_object_unref(app);
    } else {
        GError *uri_error = nullptr;
        char *uri = g_file_get_uri(file);
        if (!g_app_info_launch_default_for_uri(uri, nullptr, &uri_error)) {
            show_error(state->window, uri_error ? uri_error->message : "No application is associated with this file.");
            g_clear_error(&uri_error);
        }
        g_free(uri);
    }
    g_object_unref(file);
}

inline void path_activate(GtkEntry *entry, gpointer data) {
    auto *state = static_cast<BrowserState *>(data);
    navigate(state, gtk_editable_get_text(GTK_EDITABLE(entry)));
}

inline void go_clicked(GtkButton *, gpointer data) {
    auto *state = static_cast<BrowserState *>(data);
    path_activate(GTK_ENTRY(state->path_entry), state);
}

inline void parent_clicked(GtkButton *, gpointer data) {
    auto *state = static_cast<BrowserState *>(data);
    navigate(state, state->directory.parent_path());
}

inline void populate(BrowserState *state) {
    GtkWidget *child = gtk_widget_get_first_child(state->items);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_flow_box_remove(GTK_FLOW_BOX(state->items), child);
        child = next;
    }

    GtkWidget *parent = gtk_button_new_with_label("↑  Parent directory");
    gtk_widget_add_css_class(parent, "sleela-file-parent");
    g_signal_connect(parent, "clicked", G_CALLBACK(parent_clicked), state);
    gtk_flow_box_append(GTK_FLOW_BOX(state->items), parent);

    std::vector<std::filesystem::directory_entry> entries;
    std::error_code ec;
    for (std::filesystem::directory_iterator it(state->directory, ec), end;
         !ec && it != end; it.increment(ec)) {
        entries.push_back(*it);
    }
    std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
        std::error_code ea, eb;
        const bool ad = a.is_directory(ea), bd = b.is_directory(eb);
        if (ad != bd) return ad > bd;
        return a.path().filename().string() < b.path().filename().string();
    });

    for (const auto &entry : entries) {
        const std::string name = entry.path().filename().string();
        std::error_code type_error;
        const bool is_dir = entry.is_directory(type_error);
        const std::string label = std::string(is_dir ? "▰  " : "▤  ") + name;
        GtkWidget *button = gtk_button_new_with_label(label.c_str());
        rounded_item(button, is_dir);
        gtk_widget_set_tooltip_text(button, entry.path().string().c_str());
        g_object_set_data_full(G_OBJECT(button), "sleela-file-name", g_strdup(name.c_str()), g_free);
        g_signal_connect(button, "clicked", G_CALLBACK(item_clicked), state);
        gtk_flow_box_append(GTK_FLOW_BOX(state->items), button);
    }
    if (ec) show_error(state->window, "Unable to read directory: " + state->directory.string());
}

inline void open(GtkWindow *parent, const std::filesystem::path &initial_directory) {
    auto *state = new BrowserState();
    GtkWidget *window = gtk_window_new();
    state->window = GTK_WINDOW(window);
    gtk_window_set_title(state->window, "SleelaTerminal — File Items");
    gtk_window_set_default_size(state->window, 820, 560);
    gtk_window_set_transient_for(state->window, parent);
    gtk_window_set_destroy_with_parent(state->window, TRUE);

    GtkWidget *root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_widget_add_css_class(root, "sleela-file-browser");
    gtk_widget_set_margin_top(root, 14);
    gtk_widget_set_margin_bottom(root, 14);
    gtk_widget_set_margin_start(root, 14);
    gtk_widget_set_margin_end(root, 14);

    GtkWidget *path_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    GtkWidget *path = gtk_entry_new();
    state->path_entry = path;
    gtk_widget_set_hexpand(path, TRUE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(path), "Directory path");
    g_signal_connect(path, "activate", G_CALLBACK(path_activate), state);
    GtkWidget *refresh = gtk_button_new_with_label("Go");
    g_signal_connect(refresh, "clicked", G_CALLBACK(go_clicked), state);
    gtk_box_append(GTK_BOX(path_row), path);
    gtk_box_append(GTK_BOX(path_row), refresh);
    gtk_box_append(GTK_BOX(root), path_row);

    GtkWidget *scroller = gtk_scrolled_window_new();
    gtk_widget_set_vexpand(scroller, TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroller), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    state->items = gtk_flow_box_new();
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(state->items), GTK_SELECTION_NONE);
    gtk_flow_box_set_homogeneous(GTK_FLOW_BOX(state->items), FALSE);
    gtk_flow_box_set_min_children_per_line(GTK_FLOW_BOX(state->items), 2);
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(state->items), 6);
    gtk_flow_box_set_row_spacing(GTK_FLOW_BOX(state->items), 8);
    gtk_flow_box_set_column_spacing(GTK_FLOW_BOX(state->items), 8);
    gtk_flow_box_set_activate_on_single_click(GTK_FLOW_BOX(state->items), FALSE);
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scroller), state->items);
    gtk_box_append(GTK_BOX(root), scroller);
    gtk_window_set_child(state->window, root);
    g_object_set_data_full(G_OBJECT(window), "sleela-file-browser-state", state,
        [](gpointer p) { delete static_cast<BrowserState *>(p); });

    navigate(state, initial_directory);
    gtk_window_present(state->window);
}

} // namespace file_items_detail
} // namespace sleela::terminal
