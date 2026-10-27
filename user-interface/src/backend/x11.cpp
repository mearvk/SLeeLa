/* =============================================================================
 * SleelaUI X11 backend (Linux / Unix).
 *
 * The real host surface on Linux/Unix: an Xlib top-level window, a 32-bit
 * XImage we blit the toolkit's Canvas into, FreeType for glyph coverage, and
 * Fontconfig to resolve the configured family to a real face file. No GTK, no
 * Qt, no Cairo -- SleelaUI owns its pixels and only asks X11 to show them.
 *
 * This backend is "correct, not styled": it translates native input into
 * SLUIEvent and pushes the Canvas, and makes no look-and-feel decisions.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "slui_backend.hpp"
#include "slui_window.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include <sys/select.h>
#include <time.h>

#include <fontconfig/fontconfig.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <cstring>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace slui {
namespace {

/* Set both the ICCCM title and the EWMH UTF-8 title properties.
 * XStoreName alone writes WM_NAME using the legacy STRING convention; a
 * UTF-8 title containing characters such as U+2122 TRADE MARK SIGN can then
 * be decoded incorrectly by the window manager. Keep the ICCCM properties
 * for compatibility, and explicitly publish UTF8_STRING EWMH properties for
 * modern window managers. */
void set_utf8_window_title(Display* display, ::Window window,
                           const std::string& title) {
    Xutf8SetWMProperties(display, window, title.c_str(), title.c_str(),
                         nullptr, 0, nullptr, nullptr, nullptr);

    const Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);
    const Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    const Atom net_wm_icon_name =
        XInternAtom(display, "_NET_WM_ICON_NAME", False);
    const auto* bytes =
        reinterpret_cast<const unsigned char*>(title.data());
    const int length = static_cast<int>(title.size());

    XChangeProperty(display, window, net_wm_name, utf8_string, 8,
                    PropModeReplace, bytes, length);
    XChangeProperty(display, window, net_wm_icon_name, utf8_string, 8,
                    PropModeReplace, bytes, length);
}

/* Translate an X11 KeySym into one of SleelaUI's normalised key codes. */
uint32_t map_keysym(KeySym ks) {
    switch (ks) {
    case XK_Return:
    case XK_KP_Enter: return SLUI_KEY_ENTER;
    case XK_Escape: return SLUI_KEY_ESCAPE;
    case XK_BackSpace: return SLUI_KEY_BACKSPACE;
    case XK_Tab: return SLUI_KEY_TAB;
    case XK_space: return SLUI_KEY_SPACE;
    case XK_Left: return SLUI_KEY_LEFT;
    case XK_Right: return SLUI_KEY_RIGHT;
    case XK_Up: return SLUI_KEY_UP;
    case XK_Down: return SLUI_KEY_DOWN;
    case XK_Home: return SLUI_KEY_HOME;
    case XK_End: return SLUI_KEY_END;
    case XK_Delete: return SLUI_KEY_DELETE;
    default: return SLUI_KEY_UNKNOWN;
    }
}

uint32_t map_mods(unsigned state) {
    uint32_t m = 0;
    if (state & ShiftMask) m |= SLUI_MOD_SHIFT;
    if (state & ControlMask) m |= SLUI_MOD_CTRL;
    if (state & Mod1Mask) m |= SLUI_MOD_ALT;
    if (state & Mod4Mask) m |= SLUI_MOD_SUPER;
    return m;
}

class X11Backend; /* fwd */

class X11Window : public NativeWindow {
public:
    X11Window(Display* dpy, ::Window win, GC gc, bool resizable);
    ~X11Window() override;

    void set_title(const std::string& title) override;
    void show() override;
    void hide() override;
    void set_size_limits(int minw, int minh, int maxw, int maxh) override;
    void resize_content(int width, int height) override;
    int width() const override { return width_; }
    int height() const override { return height_; }
    void request_redraw() override { dirty_ = true; }
    void present(const Canvas& canvas) override;
    void set_buffering(SLUIBufferingMode mode) override;

    ::Window handle() const { return win_; }
    void set_size(int w, int h) {
        width_ = w;
        height_ = h;
    }
    bool take_dirty() {
        bool d = dirty_;
        dirty_ = false;
        return d;
    }

private:
    /* Number of off-screen back pixmaps for the current mode: 0 (single,
     * direct blit), 1 (double), or 2 (triple, a small ring). */
    int back_count() const {
        if (mode_ == SLUI_BUFFERING_TRIPLE) return 2;
        if (mode_ == SLUI_BUFFERING_DOUBLE) return 1;
        return 0;
    }
    void free_backbuffers();

    Display* dpy_;
    ::Window win_;
    GC gc_;
    bool resizable_ = true;
    int width_ = 0, height_ = 0;
    int min_width_ = 0, min_height_ = 0, max_width_ = 0, max_height_ = 0;
    bool dirty_ = true;
    XImage* image_ = nullptr;
    std::vector<uint32_t> buffer_;

    /* Off-screen back buffers (Pixmaps) for double/triple buffering. The Canvas
     * is drawn into the current back pixmap via XPutImage, then copied to the
     * window in one XCopyArea so a resize/redraw lands atomically. A ring of 2
     * (triple) lets the next frame start without waiting on the last copy. */
    SLUIBufferingMode mode_ = SLUI_BUFFERING_DOUBLE;
    Pixmap backbuf_[2] = {0, 0};
    int back_w_ = 0, back_h_ = 0;
    int back_idx_ = 0;
};

class X11Backend : public Backend {
public:
    explicit X11Backend(const std::string& /*app_id*/) {
        dpy_ = XOpenDisplay(nullptr);
        if (dpy_) {
            screen_ = DefaultScreen(dpy_);
            wm_delete_ = XInternAtom(dpy_, "WM_DELETE_WINDOW", False);
        }
        FT_Init_FreeType(&ft_);
        FcInit();
    }
    ~X11Backend() override {
        for (auto& kv : faces_)
            if (kv.second) FT_Done_Face(kv.second);
        if (ft_) FT_Done_FreeType(ft_);
        if (dpy_) XCloseDisplay(dpy_);
    }

    bool ok() const { return dpy_ != nullptr && ft_ != nullptr; }

    SLUIBackend id() const override { return SLUI_BACKEND_X11; }

    /* Primary display size, straight from the open X connection. */
    bool screen_size(int* w, int* h) const override {
        if (!dpy_) return false;
        if (w) *w = DisplayWidth(dpy_, screen_);
        if (h) *h = DisplayHeight(dpy_, screen_);
        return true;
    }

    std::unique_ptr<NativeWindow> create_window(Window* owner,
                                                const std::string& title,
                                                int width, int height,
                                                bool resizable) override {
        if (!dpy_) return nullptr;
        ::Window root = RootWindow(dpy_, screen_);
        XSetWindowAttributes attrs;
        attrs.background_pixel = BlackPixel(dpy_, screen_);
        attrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                           ButtonPressMask | ButtonReleaseMask |
                           PointerMotionMask | StructureNotifyMask |
                           FocusChangeMask;
        ::Window win = XCreateWindow(
            dpy_, root, 0, 0, static_cast<unsigned>(width),
            static_cast<unsigned>(height), 0, CopyFromParent, InputOutput,
            CopyFromParent, CWBackPixel | CWEventMask, &attrs);

        set_utf8_window_title(dpy_, win, title);
        XSetWMProtocols(dpy_, win, &wm_delete_, 1);

        GC gc = XCreateGC(dpy_, win, 0, nullptr);
        auto* nw = new X11Window(dpy_, win, gc, resizable);
        nw->set_size(width, height);
        owners_[win] = owner;
        windows_[win] = nw;
        return std::unique_ptr<NativeWindow>(nw);
    }

    int pump(bool block) override {
        if (!dpy_) return SLUI_ERR_BACKEND;
        int dispatched = 0;

        /* Flush any pending redraws before (potentially) blocking. */
        for (auto& kv : windows_) {
            if (kv.second->take_dirty()) {
                Window* owner = owners_[kv.first];
                if (owner) kv.second->present(owner->render());
            }
        }

        /* Does any window have an animated widget? If so, pick the fastest
         * requested refresh so we drive a frame timer while idle. */
        double fps = 0.0;
        bool animated = false;
        for (auto& kv : owners_) {
            double f = 0.0;
            if (kv.second && kv.second->has_animation(&f)) {
                animated = true;
                if (f > fps) fps = f;
            }
        }
        if (fps <= 0.0) fps = 60.0;
        double frame_s = 1.0 / fps;

        if (!XPending(dpy_)) {
            if (!block && !animated) return 0;
            /* Wait on the X connection fd, but no longer than one frame so an
             * animated window gets ticked on timeout. */
            int fd = ConnectionNumber(dpy_);
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(fd, &rfds);
            struct timeval tv;
            double wait = animated ? frame_s : (block ? 1.0 : 0.0);
            tv.tv_sec = static_cast<long>(wait);
            tv.tv_usec = static_cast<long>((wait - tv.tv_sec) * 1e6);
            int rc = select(fd + 1, &rfds, nullptr, nullptr,
                            (block || animated) ? &tv : &tv);
            if (rc == 0 && animated) {
                /* Frame timeout: advance animation and repaint. */
                double now = now_seconds();
                double dt = last_anim_ > 0 ? now - last_anim_ : frame_s;
                last_anim_ = now;
                if (dt > 0.25) dt = 0.25;
                for (auto& kv : owners_) {
                    if (kv.second) kv.second->animation_tick(dt, nullptr);
                }
            }
        }

        while (XPending(dpy_)) {
            XEvent xe;
            XNextEvent(dpy_, &xe);
            dispatched += dispatch(xe) ? 1 : 0;
        }

        /* Repaint windows whose handlers marked themselves dirty. */
        for (auto& kv : windows_) {
            if (kv.second->take_dirty()) {
                Window* owner = owners_[kv.first];
                if (owner) kv.second->present(owner->render());
            }
        }
        return dispatched;
    }

    void quit(int exit_code) override {
        quit_ = true;
        exit_code_ = exit_code;
    }
    bool should_quit(int* exit_code) const override {
        if (quit_ && exit_code) *exit_code = exit_code_;
        return quit_;
    }

    /* ---- fonts --------------------------------------------------------- */
    bool set_font(const std::string& family, int size_px) override {
        /* size_px is really points here; convert to pixels at 96 dpi. */
        int px = static_cast<int>(size_px * 96.0 / 72.0 + 0.5);
        std::string key = family + "@" + std::to_string(px);
        if (key == current_key_ && active_face_) return true;

        FT_Face face = faces_.count(key) ? faces_[key] : nullptr;
        if (!face) {
            std::string path = resolve_face(family);
            if (path.empty()) return false;
            if (FT_New_Face(ft_, path.c_str(), 0, &face) != 0) return false;
            faces_[key] = face;
        }
        FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(px));
        active_face_ = face;
        current_key_ = key;
        current_px_ = px;
        glyph_cache_.clear();
        return true;
    }

    double font_ascent() const override {
        if (!active_face_) return 12.0;
        return active_face_->size->metrics.ascender / 64.0;
    }
    double font_descent() const override {
        if (!active_face_) return 3.0;
        return -active_face_->size->metrics.descender / 64.0;
    }
    double font_line_height() const override {
        if (!active_face_) return 16.0;
        return active_face_->size->metrics.height / 64.0;
    }

    bool rasterize_glyph(uint32_t cp, GlyphBitmap* out) override {
        if (!active_face_) return false;
        /* Coherent Dignity typography (section 4): below 12px, force MONOCHROME
         * hinting. Grayscale sub-pixel AA at tiny sizes smears thin stems and,
         * on dark backgrounds, produces coloured fringing; a crisp 1-bpp render
         * expanded to full-coverage keeps small technical text ultra-sharp.
         * At 12px and up we keep the antialiased render for smooth larger text. */
        const bool mono = current_px_ < 12;
        FT_Int32 flags = FT_LOAD_RENDER | (mono ? FT_LOAD_TARGET_MONO : 0);
        if (FT_Load_Char(active_face_, cp, flags) != 0) return false;
        FT_GlyphSlot g = active_face_->glyph;
        out->left = g->bitmap_left;
        out->top = g->bitmap_top;
        out->width = static_cast<int>(g->bitmap.width);
        out->height = static_cast<int>(g->bitmap.rows);
        out->advance = g->advance.x / 64.0;
        out->coverage.assign(static_cast<size_t>(out->width) * out->height, 0);
        if (g->bitmap.pixel_mode == FT_PIXEL_MODE_MONO) {
            /* 1 bit per pixel, MSB-first within each byte; expand to 0/255. */
            for (int row = 0; row < out->height; ++row) {
                const unsigned char* src = g->bitmap.buffer + row * g->bitmap.pitch;
                for (int x = 0; x < out->width; ++x) {
                    unsigned char bit = (src[x >> 3] >> (7 - (x & 7))) & 1u;
                    out->coverage[static_cast<size_t>(row) * out->width + x] =
                        bit ? 255 : 0;
                }
            }
        } else {
            /* 8-bit grayscale coverage (antialiased). */
            for (int row = 0; row < out->height; ++row) {
                const unsigned char* src = g->bitmap.buffer + row * g->bitmap.pitch;
                std::memcpy(out->coverage.data() + static_cast<size_t>(row) * out->width,
                            src, static_cast<size_t>(out->width));
            }
        }
        return true;
    }

    double glyph_advance(uint32_t cp) override {
        if (!active_face_) return 8.0;
        auto it = glyph_cache_.find(cp);
        if (it != glyph_cache_.end()) return it->second;
        double adv = 8.0;
        if (FT_Load_Char(active_face_, cp, FT_LOAD_DEFAULT) == 0)
            adv = active_face_->glyph->advance.x / 64.0;
        glyph_cache_[cp] = adv;
        return adv;
    }

    Display* display() const { return dpy_; }
    int screen() const { return screen_; }

private:
    /* Resolve one requested family to a real installed face file, or "". */
    std::string match_one(const char* req) {
        FcPattern* pat = FcNameParse(reinterpret_cast<const FcChar8*>(req));
        FcConfigSubstitute(nullptr, pat, FcMatchPattern);
        FcDefaultSubstitute(pat);
        FcResult res;
        FcPattern* match = FcFontMatch(nullptr, pat, &res);
        std::string path;
        if (match) {
            FcChar8* file = nullptr;
            if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch && file)
                path = reinterpret_cast<const char*>(file);
            FcPatternDestroy(match);
        }
        FcPatternDestroy(pat);
        return path;
    }
    std::string resolve_face(const std::string& family) {
        /* A specific requested family resolves directly through Fontconfig. */
        if (!(family == "system" || family.empty()))
            return match_one(family.c_str());
        /* Coherent Dignity typography (sections 1-2): "system" prefers the
         * indestructible Neo-Grotesque workhorses in order, then any sans. The
         * first family Fontconfig resolves to a REAL file wins; if a name is not
         * installed, Fontconfig still returns its best substitute for the final
         * "sans-serif" request, so a face is always found. We only accept an
         * EXACT family match for the preferred names so we don't stop early on a
         * substitute; the final sans-serif entry is the guaranteed fallback. */
        static const char* kPreferred[] = {
            "Inter", "SF Pro Text", "SF Pro", "Segoe UI", "Roboto", "sans-serif"
        };
        for (const char* fam : kPreferred) {
            FcPattern* pat = FcNameParse(reinterpret_cast<const FcChar8*>(fam));
            FcConfigSubstitute(nullptr, pat, FcMatchPattern);
            FcDefaultSubstitute(pat);
            FcResult res;
            FcPattern* match = FcFontMatch(nullptr, pat, &res);
            std::string path, got;
            if (match) {
                FcChar8* file = nullptr;
                if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch && file)
                    path = reinterpret_cast<const char*>(file);
                FcChar8* famGot = nullptr;
                if (FcPatternGetString(match, FC_FAMILY, 0, &famGot) == FcResultMatch && famGot)
                    got = reinterpret_cast<const char*>(famGot);
                FcPatternDestroy(match);
            }
            FcPatternDestroy(pat);
            bool isFallback = (std::string(fam) == "sans-serif");
            /* Accept an exact family hit, or the guaranteed final fallback. */
            if (!path.empty() && (isFallback || got == fam)) return path;
            if (isFallback && !path.empty()) return path;
        }
        return match_one("sans-serif");
    }

    bool dispatch(const XEvent& xe) {
        auto wit = owners_.find(xe.xany.window);
        if (wit == owners_.end()) return false;
        Window* owner = wit->second;
        X11Window* nw = windows_[xe.xany.window];

        SLUIEvent ev{};
        ev.window = reinterpret_cast<SLUIWindow*>(owner);
        switch (xe.type) {
        case Expose:
            if (xe.xexpose.count == 0) nw->request_redraw();
            return true;
        case ConfigureNotify:
            if (xe.xconfigure.width != nw->width() ||
                xe.xconfigure.height != nw->height()) {
                nw->set_size(xe.xconfigure.width, xe.xconfigure.height);
                ev.type = SLUI_EVENT_RESIZE;
                ev.width = xe.xconfigure.width;
                ev.height = xe.xconfigure.height;
                owner->handle_event(ev);
            }
            return true;
        case ClientMessage:
            if (static_cast<Atom>(xe.xclient.data.l[0]) == wm_delete_) {
                ev.type = SLUI_EVENT_CLOSE;
                owner->handle_event(ev);
            }
            return true;
        case MotionNotify:
            ev.type = SLUI_EVENT_POINTER_MOVE;
            ev.x = xe.xmotion.x;
            ev.y = xe.xmotion.y;
            ev.modifiers = map_mods(xe.xmotion.state);
            owner->handle_event(ev);
            return true;
        case ButtonPress:
        case ButtonRelease: {
            ev.type = xe.type == ButtonPress ? SLUI_EVENT_POINTER_DOWN
                                             : SLUI_EVENT_POINTER_UP;
            ev.x = xe.xbutton.x;
            ev.y = xe.xbutton.y;
            ev.modifiers = map_mods(xe.xbutton.state);
            switch (xe.xbutton.button) {
            case Button1: ev.button = SLUI_BUTTON_LEFT; break;
            case Button2: ev.button = SLUI_BUTTON_MIDDLE; break;
            case Button3: ev.button = SLUI_BUTTON_RIGHT; break;
            default: ev.button = SLUI_BUTTON_NONE; break;
            }
            if (ev.button != SLUI_BUTTON_NONE) owner->handle_event(ev);
            return true;
        }
        case KeyPress:
        case KeyRelease: {
            char buf[16];
            KeySym ks = 0;
            XKeyEvent ke = xe.xkey;
            int n = XLookupString(&ke, buf, sizeof(buf), &ks, nullptr);
            ev.type = xe.type == KeyPress ? SLUI_EVENT_KEY_DOWN : SLUI_EVENT_KEY_UP;
            ev.keysym = map_keysym(ks);
            ev.modifiers = map_mods(xe.xkey.state);
            owner->handle_event(ev);
            /* Printable text commits as a TEXT event on key press. */
            if (xe.type == KeyPress && n > 0 &&
                static_cast<unsigned char>(buf[0]) >= 0x20) {
                SLUIEvent te{};
                te.type = SLUI_EVENT_TEXT;
                te.window = ev.window;
                /* decode first utf-8 scalar from buf */
                unsigned char c0 = buf[0];
                uint32_t cp = c0;
                int extra = 0;
                if ((c0 >> 5) == 0x6) { cp = c0 & 0x1F; extra = 1; }
                else if ((c0 >> 4) == 0xE) { cp = c0 & 0x0F; extra = 2; }
                else if ((c0 >> 3) == 0x1E) { cp = c0 & 0x07; extra = 3; }
                for (int k = 1; k <= extra && k < n; ++k)
                    cp = (cp << 6) | (static_cast<unsigned char>(buf[k]) & 0x3F);
                te.codepoint = cp;
                owner->handle_event(te);
            }
            return true;
        }
        case FocusIn:
            ev.type = SLUI_EVENT_FOCUS;
            owner->handle_event(ev);
            return true;
        case FocusOut:
            ev.type = SLUI_EVENT_BLUR;
            owner->handle_event(ev);
            return true;
        default:
            return false;
        }
    }

    static double now_seconds() {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return ts.tv_sec + ts.tv_nsec / 1e9;
    }

    Display* dpy_ = nullptr;
    int screen_ = 0;
    Atom wm_delete_ = 0;
    bool quit_ = false;
    int exit_code_ = 0;
    double last_anim_ = 0.0;

    FT_Library ft_ = nullptr;
    FT_Face active_face_ = nullptr;
    std::string current_key_;
    int current_px_ = 14;   /* active face pixel size (for sub-12px hinting) */
    std::unordered_map<std::string, FT_Face> faces_;
    std::unordered_map<uint32_t, double> glyph_cache_;

    std::map<::Window, Window*> owners_;
    std::map<::Window, X11Window*> windows_;
};

/* ---- X11Window out-of-line ---------------------------------------------- */
X11Window::X11Window(Display* dpy, ::Window win, GC gc, bool resizable)
    : dpy_(dpy), win_(win), gc_(gc), resizable_(resizable) {}

X11Window::~X11Window() {
    if (image_) {
        image_->data = nullptr; /* buffer_ owns the storage */
        XDestroyImage(image_);
    }
    free_backbuffers();
    if (gc_) XFreeGC(dpy_, gc_);
    if (win_) XDestroyWindow(dpy_, win_);
}

void X11Window::free_backbuffers() {
    for (int i = 0; i < 2; ++i) {
        if (backbuf_[i]) {
            XFreePixmap(dpy_, backbuf_[i]);
            backbuf_[i] = 0;
        }
    }
    back_w_ = back_h_ = 0;
    back_idx_ = 0;
}

void X11Window::set_buffering(SLUIBufferingMode mode) {
    if (mode == mode_) return;
    mode_ = mode;
    /* Drop any existing back pixmaps; present() lazily (re)creates the right
     * number for the new mode at the current size. */
    free_backbuffers();
    dirty_ = true;
}

void X11Window::set_title(const std::string& title) {
    set_utf8_window_title(dpy_, win_, title);
}
void X11Window::show() {
    XMapWindow(dpy_, win_);
    XFlush(dpy_);
}
void X11Window::hide() {
    XUnmapWindow(dpy_, win_);
    XFlush(dpy_);
}
void X11Window::set_size_limits(int minw, int minh, int maxw, int maxh) {
    min_width_ = minw; min_height_ = minh; max_width_ = maxw; max_height_ = maxh;
    XSizeHints hints{};
    if (!resizable_) {
        hints.flags = PMinSize | PMaxSize;
        hints.min_width = hints.max_width = std::max(1, width_);
        hints.min_height = hints.max_height = std::max(1, height_);
    } else {
        if (minw > 0 || minh > 0) { hints.flags |= PMinSize; hints.min_width = std::max(1, minw); hints.min_height = std::max(1, minh); }
        if (maxw > 0 || maxh > 0) { hints.flags |= PMaxSize; hints.max_width = maxw > 0 ? maxw : 65535; hints.max_height = maxh > 0 ? maxh : 65535; }
    }
    XSetWMNormalHints(dpy_, win_, &hints); XFlush(dpy_);
}
void X11Window::resize_content(int w, int h) {
    w = std::max(w, min_width_); h = std::max(h, min_height_);
    if (max_width_ > 0) w = std::min(w, max_width_);
    if (max_height_ > 0) h = std::min(h, max_height_);
    if (w > 0 && h > 0) { XResizeWindow(dpy_, win_, static_cast<unsigned>(w), static_cast<unsigned>(h)); XFlush(dpy_); }
}

void X11Window::present(const Canvas& canvas) {
    int w = canvas.width(), h = canvas.height();
    if (w <= 0 || h <= 0) return;
    size_t count = static_cast<size_t>(w) * h;
    buffer_.assign(canvas.pixels(), canvas.pixels() + count);

    int depth = DefaultDepth(dpy_, DefaultScreen(dpy_));
    if (!image_ || image_->width != w || image_->height != h) {
        if (image_) {
            image_->data = nullptr;
            XDestroyImage(image_);
            image_ = nullptr;
        }
        ::Visual* vis = DefaultVisual(dpy_, DefaultScreen(dpy_));
        image_ = XCreateImage(dpy_, vis, static_cast<unsigned>(depth), ZPixmap, 0,
                              reinterpret_cast<char*>(buffer_.data()), w, h, 32,
                              w * 4);
    } else {
        image_->data = reinterpret_cast<char*>(buffer_.data());
    }

    int backs = back_count();
    if (backs == 0) {
        /* SINGLE buffering: blit the Canvas straight to the window. Cheapest,
         * but a partially-copied frame can be visible during a resize.
         * Canvas packs 0xAARRGGBB; on little-endian X with a 24/32-bit
         * TrueColor visual the ZPixmap byte order is BGRA, which matches. */
        XPutImage(dpy_, win_, gc_, image_, 0, 0, 0, 0,
                  static_cast<unsigned>(w), static_cast<unsigned>(h));
        XFlush(dpy_);
        return;
    }

    /* DOUBLE / TRIPLE buffering: render the whole frame into an off-screen
     * Pixmap, then copy it to the window in a single XCopyArea so the window
     * never shows a half-drawn frame (no tearing on resize/redraw). TRIPLE
     * rings two pixmaps so consecutive frames reuse alternate buffers. */
    if (back_w_ != w || back_h_ != h) {
        free_backbuffers();
        back_w_ = w;
        back_h_ = h;
    }
    if (back_idx_ >= backs) back_idx_ = 0;
    if (!backbuf_[back_idx_]) {
        backbuf_[back_idx_] =
            XCreatePixmap(dpy_, win_, static_cast<unsigned>(w),
                          static_cast<unsigned>(h), static_cast<unsigned>(depth));
    }
    Pixmap target = backbuf_[back_idx_];
    XPutImage(dpy_, target, gc_, image_, 0, 0, 0, 0,
              static_cast<unsigned>(w), static_cast<unsigned>(h));
    XCopyArea(dpy_, target, win_, gc_, 0, 0, static_cast<unsigned>(w),
              static_cast<unsigned>(h), 0, 0);
    XFlush(dpy_);
    /* Advance the ring for triple buffering (no-op for double, backs==1). */
    back_idx_ = (backs > 1) ? ((back_idx_ + 1) % backs) : 0;
}

} // namespace

std::unique_ptr<Backend> create_backend(const std::string& app_id) {
    auto be = std::make_unique<X11Backend>(app_id);
    if (!be->ok()) return nullptr;
    return be;
}

} // namespace slui
