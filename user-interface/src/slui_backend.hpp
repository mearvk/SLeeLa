#ifndef SLUI_BACKEND_HPP
#define SLUI_BACKEND_HPP
/* =============================================================================
 * SleelaUI platform backend interface.
 *
 * This is the single seam between the toolkit's portable core (widgets, layout,
 * theme, software rasterizer) and the real host window system. Exactly one
 * backend is compiled per build:
 *
 *   SLUI_BACKEND_WIN32  -- src/backend/win32.cpp   (Windows 10+)
 *   SLUI_BACKEND_COCOA  -- src/backend/cocoa.mm    (macOS / Darwin)
 *   SLUI_BACKEND_X11    -- src/backend/x11.cpp     (Linux / Unix)
 *
 * A backend is responsible for four things and nothing else:
 *   1. create/destroy a native top-level window and surface a client-area size;
 *   2. translate native input into SLUIEvent and push it to the owning Window;
 *   3. rasterize a glyph to 8-bit coverage using the real host font engine;
 *   4. blit the Canvas's 32-bit buffer to the window.
 *
 * All look-and-feel decisions live in the portable core, so a new backend only
 * has to be "correct", never "styled".
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "slui_render.hpp"

#include <memory>
#include <string>

namespace slui {

class Window; /* forward: the portable per-window object (slui_window.hpp) */

/* Opaque handle to a backend's native window object. */
class NativeWindow {
public:
    virtual ~NativeWindow() = default;

    virtual void set_title(const std::string& title) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;

    /* Current client-area size in device pixels. */
    virtual int width() const = 0;
    virtual int height() const = 0;

    /* Ask the backend to repaint from the Canvas on the next turn. */
    virtual void request_redraw() = 0;

    /* Push the Canvas pixels to the window surface now. */
    virtual void present(const Canvas& canvas) = 0;
};

/* A backend owns the application connection (display/dispatcher) and the font
 * engine, and manufactures native windows. */
class Backend {
public:
    virtual ~Backend() = default;

    virtual SLUIBackend id() const = 0;

    /* Create a native window bound to the portable `owner`, which the backend
     * calls back into to deliver events and to pull the Canvas for painting. */
    virtual std::unique_ptr<NativeWindow> create_window(Window* owner,
                                                        const std::string& title,
                                                        int width, int height,
                                                        bool resizable) = 0;

    /* Pump native events once. Returns the number dispatched, or <0 on error.
     * block != 0 waits for at least one event. */
    virtual int pump(bool block) = 0;

    /* Request the event loop stop; slui_app_run returns `exit_code`. */
    virtual void quit(int exit_code) = 0;
    virtual bool should_quit(int* exit_code) const = 0;

    /* Font: resolve a family+size once, then rasterize glyphs on demand.
     * `family` is a hint ("Inter", "Cantarell", "system"...); the backend maps
     * it to the closest real face and remembers metrics. */
    virtual bool set_font(const std::string& family, int size_px) = 0;
    virtual double font_ascent() const = 0;
    virtual double font_descent() const = 0;
    virtual double font_line_height() const = 0;

    /* Rasterize one unicode scalar to coverage; false if the glyph is absent. */
    virtual bool rasterize_glyph(uint32_t codepoint, GlyphBitmap* out) = 0;
    /* Pen advance for a scalar without producing a bitmap (for measuring). */
    virtual double glyph_advance(uint32_t codepoint) = 0;
};

/* Factory implemented by exactly one backend translation unit. */
std::unique_ptr<Backend> create_backend(const std::string& app_id);

/* The process-wide backend used for free-standing text (Draw + Font APIs).
 * Lazily created; shared so measuring/rasterizing text needs no window. May be
 * null if no backend could be created (e.g. no display for X11). */
Backend* shared_text_backend();

} // namespace slui

#endif /* SLUI_BACKEND_HPP */
