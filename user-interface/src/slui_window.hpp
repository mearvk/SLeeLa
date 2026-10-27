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

    /* Animation: advance every CanvasView/Throbber in the tree by `dt` seconds
     * and mark the window dirty. Returns true if the tree contains any animated
     * widget, so the backend knows whether to keep running an animation timer.
     * `out_fps` receives the fastest requested refresh rate (0 if none). */
    bool animation_tick(double dt, double* out_fps);
    bool has_animation(double* out_fps) const;

    NativeWindow* native() { return native_.get(); }
    bool closed() const { return closed_; }

    /* Presentation buffering: remembered on the window and pushed to the native
     * surface, so it is re-applied if the native window is ever recreated. */
    void set_buffering(SLUIBufferingMode mode);
    SLUIBufferingMode buffering() const { return buffering_; }
    SLUIStatus set_min_size(int width, int height);
    SLUIStatus set_max_size(int width, int height);
    SLUIStatus set_size_limits(int min_width, int min_height, int max_width, int max_height);
    SLUIStatus get_min_size(SLUISize* out) const;
    SLUIStatus get_max_size(SLUISize* out) const;

private:
    void relayout();
    SLUISize layout_minimum() const;
    SLUISize effective_minimum() const;
    void refresh_native_size_limits();
    static void invalidate_thunk(void* self) {
        static_cast<Window*>(self)->request_redraw();
    }

    Backend* backend_;
    Theme theme_;
    bool custom_titlebar_ = false;
    bool dirty_ = true;
    bool closed_ = false;
    SLUIBufferingMode buffering_ = SLUI_BUFFERING_TRIPLE;
    int min_width_ = 0, min_height_ = 0, max_width_ = 0, max_height_ = 0;
    int applied_min_width_ = -1, applied_min_height_ = -1;
    int applied_max_width_ = -1, applied_max_height_ = -1;

    std::unique_ptr<Widget> root_;
    std::unique_ptr<NativeWindow> native_;
    Canvas canvas_;

    Widget* focus_ = nullptr;

    SLUIEventHandler handler_ = nullptr;
    void* handler_user_ = nullptr;
};

} // namespace slui

#endif /* SLUI_WINDOW_HPP */
