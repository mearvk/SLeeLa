#ifndef SLUI_WINDOW_HPP
#define SLUI_WINDOW_HPP
/* The portable per-window object: owns the widget tree root, the Canvas, the
 * resolved theme, and a borrowed Backend. The backend's NativeWindow calls back
 * into Window to deliver input (handle_event) and to pull a freshly painted
 * Canvas (render). Max Rupplin -- MEARVK LLC -- 2026. */
#include "sleela_ui.h"
#include "slui_backend.hpp"
#include "slui_render.hpp"
#include "slui_theme.hpp"
#include "slui_widget.hpp"

#include <memory>

namespace slui {

class Window {
public:
    /* `app` is the opaque host SLUIApp*; the window does not dereference it. */
    Window(void* app, Backend* backend, const SLUIWindowConfig& cfg);
    ~Window();

    SLUIWidget* root_handle();
    Widget* root() { return root_.get(); }

    void show();
    void hide();
    void set_title(const std::string& title);

    const Theme& theme() const { return theme_; }
    void set_theme(const Theme& t);

    void set_event_handler(SLUIEventHandler h, void* u) {
        handler_ = h;
        handler_user_ = u;
    }

    /* Called by the backend. */
    void handle_event(const SLUIEvent& ev);
    const Canvas& render(); /* lay out + paint into the Canvas, return it */
    void request_redraw();

    NativeWindow* native() { return native_.get(); }
    bool closed() const { return closed_; }

private:
    void relayout();
    static void invalidate_thunk(void* self) {
        static_cast<Window*>(self)->request_redraw();
    }

    Backend* backend_;
    Theme theme_;
    bool custom_titlebar_ = false;
    bool dirty_ = true;
    bool closed_ = false;

    std::unique_ptr<Widget> root_;
    std::unique_ptr<NativeWindow> native_;
    Canvas canvas_;

    Widget* focus_ = nullptr;

    SLUIEventHandler handler_ = nullptr;
    void* handler_user_ = nullptr;
};

} // namespace slui

#endif /* SLUI_WINDOW_HPP */
