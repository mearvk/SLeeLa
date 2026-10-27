/* =============================================================================
 * SleelaUI Window: lays out and paints the widget tree, and routes input.
 * Max Rupplin -- MEARVK LLC -- 2026. */
#include "slui_window.hpp"

#include "slui_widget.hpp"

#include <algorithm>
#include <functional>
#include <vector>

namespace slui {

static Widget* find_focusable(Widget* w, bool forward, Widget* current,
                              bool& seen);

Window::Window(void*, Backend* backend, const SLUIWindowConfig& cfg)
    : backend_(backend), custom_titlebar_(cfg.custom_titlebar != 0) {
    SLUITheme abi;
    if (cfg.theme) {
        abi = *cfg.theme;
    } else {
        theme_fill_preset(&abi, SLUI_THEME_SLEELA_BASE);
    }
    theme_ = Theme::from_abi(abi);

    /* The root is a vertical box: the whole window content area. */
    root_ = std::make_unique<Box>(SLUI_ORIENT_VERTICAL, 0);
    root_->set_invalidate(&Window::invalidate_thunk, this);

    int w = cfg.width > 0 ? cfg.width : 800;
    int h = cfg.height > 0 ? cfg.height : 560;
    native_ = backend_->create_window(this, cfg.title ? cfg.title : "SleelaUI",
                                      w, h, cfg.resizable != 0);
    canvas_.resize(w, h);
    /* Triple buffering is the native default for smoother rapid redraws,
     * resize, and animation. Applications may explicitly select single/double. */
    if (native_) native_->set_buffering(buffering_);
    refresh_native_size_limits();
}

Window::~Window() = default;

void Window::set_buffering(SLUIBufferingMode mode) {
    buffering_ = mode;
    if (native_) native_->set_buffering(mode);
    request_redraw();
}

SLUISize Window::layout_minimum() const {
    PaintContext ctx;
    ctx.canvas = const_cast<Canvas*>(&canvas_);
    ctx.backend = backend_;
    ctx.theme = &theme_;
    if (backend_) backend_->set_font(theme_.font_family, theme_.font_size);
    SLUISize natural = root_ ? root_->measure(ctx) : SLUISize{0, 0};
    const int padding = 2 * SLUI_WINDOW_CONTENT_PADDING;
    return SLUISize{std::max(0, natural.w) + padding,
                    std::max(0, natural.h) + padding};
}

SLUISize Window::effective_minimum() const {
    const SLUISize natural = layout_minimum();
    int w = std::max(min_width_, natural.w);
    int h = std::max(min_height_, natural.h);
    /* If a later widget mutation grows beyond an explicit maximum, that maximum
     * remains authoritative and the layout compresses inside the client area. */
    if (max_width_ > 0) w = std::min(w, max_width_);
    if (max_height_ > 0) h = std::min(h, max_height_);
    return SLUISize{w, h};
}

void Window::refresh_native_size_limits() {
    if (!native_) return;
    const SLUISize min = effective_minimum();
    if (min.w != applied_min_width_ || min.h != applied_min_height_ ||
        max_width_ != applied_max_width_ || max_height_ != applied_max_height_) {
        /* Cache before calling into the native backend: some window systems can
         * synchronously deliver a resize notification from set_size_limits(). */
        applied_min_width_ = min.w; applied_min_height_ = min.h;
        applied_max_width_ = max_width_; applied_max_height_ = max_height_;
        native_->set_size_limits(min.w, min.h, max_width_, max_height_);
    }
    const int cw = native_->width(), ch = native_->height();
    const int w = std::max(min.w, max_width_ > 0 ? std::min(cw, max_width_) : cw);
    const int h = std::max(min.h, max_height_ > 0 ? std::min(ch, max_height_) : ch);
    if (w != cw || h != ch) native_->resize_content(w, h);
}

SLUIStatus Window::set_min_size(int width, int height) {
    return set_size_limits(width, height, max_width_, max_height_);
}
SLUIStatus Window::set_max_size(int width, int height) {
    return set_size_limits(min_width_, min_height_, width, height);
}
SLUIStatus Window::set_size_limits(int minw, int minh, int maxw, int maxh) {
    if (minw < 0 || minh < 0 || maxw < 0 || maxh < 0) return SLUI_ERR_INVALID;
    if ((maxw > 0 && minw > maxw) || (maxh > 0 && minh > maxh))
        return SLUI_ERR_INVALID;
    const SLUISize natural = layout_minimum();
    if ((maxw > 0 && maxw < std::max(minw, natural.w)) ||
        (maxh > 0 && maxh < std::max(minh, natural.h)))
        return SLUI_ERR_INVALID;
    min_width_ = minw; min_height_ = minh; max_width_ = maxw; max_height_ = maxh;
    /* Apply as one transaction: no field is changed on invalid requests. */
    request_redraw();
    return SLUI_OK;
}
SLUIStatus Window::get_min_size(SLUISize* out) const {
    if (!out) return SLUI_ERR_INVALID;
    *out = effective_minimum();
    return SLUI_OK;
}
SLUIStatus Window::get_max_size(SLUISize* out) const {
    if (!out) return SLUI_ERR_INVALID;
    *out = SLUISize{max_width_, max_height_};
    return SLUI_OK;
}


SLUIWidget* Window::root_handle() {
    return reinterpret_cast<SLUIWidget*>(root_.get());
}

void Window::show() {
    if (native_) native_->show();
}
void Window::hide() {
    if (native_) native_->hide();
}
void Window::set_title(const std::string& title) {
    if (native_) native_->set_title(title);
}

void Window::set_theme(const Theme& t) {
    theme_ = t;
    dirty_ = true;
    request_redraw();
}

void Window::request_redraw() {
    dirty_ = true;
    refresh_native_size_limits();
    if (native_) native_->request_redraw();
}

void Window::relayout() {
    PaintContext ctx;
    ctx.canvas = &canvas_;
    ctx.backend = backend_;
    ctx.theme = &theme_;
    backend_->set_font(theme_.font_family, theme_.font_size);
    int w = native_ ? native_->width() : canvas_.width();
    int h = native_ ? native_->height() : canvas_.height();
    canvas_.resize(w, h);
    const int inset = SLUI_WINDOW_CONTENT_PADDING;
    root_->arrange(ctx, Rect{inset, inset, std::max(0, w - 2 * inset),
                             std::max(0, h - 2 * inset)});
}

const Canvas& Window::render() {
    relayout();
    canvas_.clear(theme_.bg);
    PaintContext ctx;
    ctx.canvas = &canvas_;
    ctx.backend = backend_;
    ctx.theme = &theme_;
    root_->paint(ctx);

    /* Overlay pass: paint floating widgets (popups/menus) LAST so they appear
     * forward of the frame, and UNCLIPPED (a full-canvas clip) so they are not
     * trimmed by any ancestor's clip. Each overlay is measured to its natural
     * size and placed near the lower-right of the window (the status-bar region
     * its trigger lives in), clamped to stay on-screen. */
    std::vector<Widget*> overlays;
    root_->collect_overlays(overlays);
    if (!overlays.empty()) {
        int W = canvas_.width(), H = canvas_.height();
        for (Widget* ov : overlays) {
            SLUISize s = ov->measure(ctx);
            int ow = s.w > 0 ? s.w : 240;
            int oh = s.h > 0 ? s.h : 160;
            if (ow > W) ow = W;
            if (oh > H) oh = H;
            /* Anchor to the lower-right, a small inset above the status bar,
             * which is where the connectivity trigger sits in these demos. */
            int margin = theme_.unit * 3;
            int ox = W - ow - margin;
            int oy = H - oh - (theme_.control_height + margin);
            if (ox < margin) ox = margin;
            if (oy < margin) oy = margin;
            ov->arrange(ctx, Rect{ox, oy, ow, oh});
            canvas_.push_clip(Rect{0, 0, W, H});
            ov->paint(ctx);
            canvas_.pop_clip();
        }
    }

    dirty_ = false;
    return canvas_;
}

/* Depth-first walk visiting every CanvasView (and its Throbber subclass). */
static void walk_canvas_views(Widget* w, double dt, bool advance, double* fps,
                              bool* any) {
    if (!w->visible()) return;
    if (w->kind() == WidgetKind::CanvasView || w->kind() == WidgetKind::Throbber) {
        auto* cv = static_cast<CanvasView*>(w);
        if (any) *any = true;
        if (fps && cv->fps() > *fps) *fps = cv->fps();
        if (advance) cv->advance(dt);
    }
    for (auto& c : w->children())
        walk_canvas_views(c.get(), dt, advance, fps, any);
}

bool Window::animation_tick(double dt, double* out_fps) {
    double fps = 0.0;
    bool any = false;
    walk_canvas_views(root_.get(), dt, /*advance=*/true, &fps, &any);
    if (out_fps) *out_fps = fps;
    if (any) request_redraw();
    return any;
}

bool Window::has_animation(double* out_fps) const {
    double fps = 0.0;
    bool any = false;
    walk_canvas_views(const_cast<Widget*>(root_.get()), 0.0, /*advance=*/false,
                      &fps, &any);
    if (out_fps) *out_fps = fps;
    return any;
}

void Window::handle_event(const SLUIEvent& ev) {
    /* Window-level events first offer the host handler. */
    if (ev.type == SLUI_EVENT_CLOSE) {
        if (handler_ && handler_(&ev, handler_user_)) return;
        closed_ = true;
        hide();
        return;
    }
    if (ev.type == SLUI_EVENT_RESIZE) {
        const SLUISize minimum = effective_minimum();
        const int w = std::max(minimum.w, max_width_ > 0 ? std::min(ev.width, max_width_) : ev.width);
        const int h = std::max(minimum.h, max_height_ > 0 ? std::min(ev.height, max_height_) : ev.height);
        if (w != ev.width || h != ev.height) {
            if (native_) native_->resize_content(w, h);
            return;
        }
        canvas_.resize(ev.width, ev.height);
        request_redraw();
    }

    /* Keyboard Tab moves focus through focusable widgets. */
    if (ev.type == SLUI_EVENT_KEY_DOWN && ev.keysym == SLUI_KEY_TAB) {
        bool forward = !(ev.modifiers & SLUI_MOD_SHIFT);
        bool seen = (focus_ == nullptr);
        Widget* next = find_focusable(root_.get(), forward, focus_, seen);
        if (!next) {
            seen = true;
            next = find_focusable(root_.get(), forward, nullptr, seen);
        }
        if (next && next != focus_) {
            if (focus_) focus_->set_focused(false);
            focus_ = next;
            focus_->set_focused(true);
        }
        request_redraw();
        return;
    }

    /* Keyboard/text events route to the focused widget. */
    if (ev.type == SLUI_EVENT_KEY_DOWN || ev.type == SLUI_EVENT_KEY_UP ||
        ev.type == SLUI_EVENT_TEXT) {
        if (focus_ && focus_->on_event(ev)) {
            request_redraw();
            return;
        }
    }

    /* Pointer-down also updates window focus to whatever got hit. */
    bool consumed = root_->on_event(ev);

    if (ev.type == SLUI_EVENT_POINTER_DOWN) {
        /* Find the deepest focusable widget under the pointer. */
        Widget* hit = nullptr;
        std::function<void(Widget*)> dig = [&](Widget* w) {
            if (!w->visible() || !w->bounds().contains(ev.x, ev.y)) return;
            if (w->wants_focus()) hit = w;
            for (auto& c : w->children()) dig(c.get());
        };
        dig(root_.get());
        if (hit != focus_) {
            if (focus_) focus_->set_focused(false);
            focus_ = hit;
            if (focus_) focus_->set_focused(true);
        }
    }

    if (consumed) request_redraw();

    if (handler_ && !consumed) handler_(&ev, handler_user_);
}

/* Depth-first ordered walk collecting focusable widgets; returns the one after
 * `current` in `forward` order (or before, if !forward). */
static void collect(Widget* w, std::vector<Widget*>& out) {
    if (!w->visible()) return;
    if (w->wants_focus()) out.push_back(w);
    for (auto& c : w->children()) collect(c.get(), out);
}

static Widget* find_focusable(Widget* root, bool forward, Widget* current,
                              bool& /*seen*/) {
    std::vector<Widget*> all;
    collect(root, all);
    if (all.empty()) return nullptr;
    if (!current) return forward ? all.front() : all.back();
    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i] == current) {
            if (forward) return all[(i + 1) % all.size()];
            return all[(i + all.size() - 1) % all.size()];
        }
    }
    return forward ? all.front() : all.back();
}

} // namespace slui
