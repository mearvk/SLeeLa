/* =============================================================================
 * SleelaUI Window: lays out and paints the widget tree, and routes input.
 * Max Rupplin -- MEARVK LLC -- 2026. */
#include "slui_window.hpp"

#include "slui_widget.hpp"

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
}

Window::~Window() = default;

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
    root_->arrange(ctx, Rect{0, 0, w, h});
}

const Canvas& Window::render() {
    relayout();
    canvas_.clear(theme_.bg);
    PaintContext ctx;
    ctx.canvas = &canvas_;
    ctx.backend = backend_;
    ctx.theme = &theme_;
    root_->paint(ctx);
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
