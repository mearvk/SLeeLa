#ifndef SLUI_WIDGET_HPP
#define SLUI_WIDGET_HPP
/* =============================================================================
 * SleelaUI widget tree.
 *
 * A Widget is a node that measures itself, lays its children out, paints into
 * the Canvas, and handles input. The whole tree is owned by its root (a window
 * container); children are unique_ptr-owned by their parent so destroying the
 * window frees everything in one pass. The public C ABI hands back borrowed raw
 * pointers into this tree.
 *
 * Layout is a two-pass box model (measure -> arrange) in the GTK/CSS-box
 * tradition: each widget reports a natural size, containers distribute space to
 * expanding children, then everyone is positioned. Painting is strictly
 * back-to-front with the software rasterizer.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui.h"
#include "slui_geometry.hpp"
#include "slui_render.hpp"
#include "slui_theme.hpp"

#include <memory>
#include <string>
#include <vector>

namespace slui {

class Backend;

enum class WidgetKind {
    Box,
    HeaderBar,
    Label,
    Button,
    Toggle,
    Entry,
    Slider,
    Separator,
    Spacer
};

enum class Visual { Normal, Hover, Active, Disabled };

struct Margin {
    int top = 0, right = 0, bottom = 0, left = 0;
};

/* Paint context threaded through the tree each frame. */
struct PaintContext {
    Canvas* canvas = nullptr;
    Backend* backend = nullptr;
    const Theme* theme = nullptr;
};

class Widget {
public:
    explicit Widget(WidgetKind kind) : kind_(kind) {}
    virtual ~Widget() = default;

    WidgetKind kind() const { return kind_; }

    /* --- tree --------------------------------------------------------- */
    Widget* add_child(std::unique_ptr<Widget> child) {
        child->parent_ = this;
        /* Inherit the window's invalidate hook so any descendant can request a
         * repaint without knowing about the Window object. */
        child->set_invalidate(invalidate_fn_, invalidate_ctx_);
        children_.push_back(std::move(child));
        return children_.back().get();
    }
    const std::vector<std::unique_ptr<Widget>>& children() const {
        return children_;
    }
    Widget* parent() const { return parent_; }

    /* --- geometry ----------------------------------------------------- */
    const Rect& bounds() const { return bounds_; }
    void set_bounds(const Rect& r) { bounds_ = r; }

    /* content box = bounds minus margin */
    Rect content() const {
        return bounds_.inset(margin_.top, margin_.right, margin_.bottom,
                             margin_.left);
    }

    /* Measure the natural (preferred) size including this widget's own margin.
     * Containers recurse into children. */
    virtual SLUISize measure(const PaintContext& ctx) = 0;

    /* Position children within `area` (already excludes this widget's margin
     * for leaf widgets; containers receive their content box). Default: leaf. */
    virtual void arrange(const PaintContext& /*ctx*/, const Rect& area) {
        bounds_ = area;
    }

    /* Paint self then children. */
    virtual void paint(PaintContext& ctx) = 0;

    void paint_children(PaintContext& ctx) {
        for (auto& c : children_) {
            if (c->visible_) c->paint(ctx);
        }
    }

    /* --- input -------------------------------------------------------- */
    /* Returns true if the event was consumed. Default: hit-test children
     * front-to-back, then self. */
    virtual bool on_event(const SLUIEvent& ev);

    virtual bool wants_focus() const { return false; }
    void set_focused(bool f) {
        if (focused_ != f) {
            focused_ = f;
            invalidate();
        }
    }
    bool focused() const { return focused_; }

    /* --- common config (mapped from the C ABI) ------------------------ */
    void set_margin(const Margin& m) { margin_ = m; }
    void set_align(SLUIAlign h, SLUIAlign v) { halign_ = h; valign_ = v; }
    void set_expand(bool h, bool v) { hexpand_ = h; vexpand_ = v; }
    void set_size_request(int w, int h) { min_w_ = w; min_h_ = h; }
    void set_sensitive(bool s) { sensitive_ = s; invalidate(); }
    void set_visible(bool v) { visible_ = v; }
    bool visible() const { return visible_; }
    bool hexpand() const { return hexpand_; }
    bool vexpand() const { return vexpand_; }
    SLUIAlign halign() const { return halign_; }
    SLUIAlign valign() const { return valign_; }

    virtual void set_text(const std::string&) {}
    virtual std::string text() const { return {}; }
    virtual void set_toggle(bool) {}
    virtual bool toggle() const { return false; }
    virtual void set_value(double) {}
    virtual double value() const { return 0.0; }
    void set_suggested(bool s) { suggested_ = s; invalidate(); }
    void set_destructive(bool d) { destructive_ = d; invalidate(); }

    void on_activate(SLUIActivateHandler h, void* u) {
        activate_ = h;
        activate_user_ = u;
    }
    void on_value_changed(SLUIValueHandler h, void* u) {
        value_changed_ = h;
        value_changed_user_ = u;
    }

    /* Window hook: set by the window when the tree is attached. */
    using InvalidateFn = void (*)(void*);
    void set_invalidate(InvalidateFn fn, void* ctx) {
        invalidate_fn_ = fn;
        invalidate_ctx_ = ctx;
    }
    void invalidate() {
        if (invalidate_fn_) invalidate_fn_(invalidate_ctx_);
    }

protected:
    void emit_activate() {
        if (activate_) activate_(reinterpret_cast<SLUIWidget*>(this), activate_user_);
    }
    void emit_value_changed(double v) {
        if (value_changed_)
            value_changed_(reinterpret_cast<SLUIWidget*>(this), v, value_changed_user_);
    }

    Visual visual_state() const {
        if (!sensitive_) return Visual::Disabled;
        if (pressed_) return Visual::Active;
        if (hovered_) return Visual::Hover;
        return Visual::Normal;
    }

    WidgetKind kind_;
    Widget* parent_ = nullptr;
    std::vector<std::unique_ptr<Widget>> children_;

    Rect bounds_{};
    Margin margin_{};
    SLUIAlign halign_ = SLUI_ALIGN_FILL;
    SLUIAlign valign_ = SLUI_ALIGN_FILL;
    bool hexpand_ = false;
    bool vexpand_ = false;
    int min_w_ = 0;
    int min_h_ = 0;

    bool visible_ = true;
    bool sensitive_ = true;
    bool hovered_ = false;
    bool pressed_ = false;
    bool focused_ = false;
    bool suggested_ = false;
    bool destructive_ = false;

    SLUIActivateHandler activate_ = nullptr;
    void* activate_user_ = nullptr;
    SLUIValueHandler value_changed_ = nullptr;
    void* value_changed_user_ = nullptr;

    InvalidateFn invalidate_fn_ = nullptr;
    void* invalidate_ctx_ = nullptr;
};

/* ---- concrete widgets --------------------------------------------------- */

class Box : public Widget {
public:
    Box(SLUIOrientation orient, int spacing)
        : Widget(WidgetKind::Box), orient_(orient), spacing_(spacing) {}
    SLUISize measure(const PaintContext& ctx) override;
    void arrange(const PaintContext& ctx, const Rect& area) override;
    void paint(PaintContext& ctx) override;

private:
    SLUIOrientation orient_;
    int spacing_;
};

/* A header bar is a horizontal box with the chrome fill and bottom hairline. */
class HeaderBar : public Box {
public:
    HeaderBar() : Box(SLUI_ORIENT_HORIZONTAL, 8) {}
    void paint(PaintContext& ctx) override;
};

class Label : public Widget {
public:
    explicit Label(std::string text)
        : Widget(WidgetKind::Label), text_(std::move(text)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
};

class Button : public Widget {
public:
    explicit Button(std::string text)
        : Widget(WidgetKind::Button), text_(std::move(text)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
};

class Toggle : public Widget {
public:
    Toggle(std::string text, bool on)
        : Widget(WidgetKind::Toggle), text_(std::move(text)), on_(on) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }
    void set_toggle(bool on) override { on_ = on; invalidate(); }
    bool toggle() const override { return on_; }

private:
    std::string text_;
    bool on_;
};

class Entry : public Widget {
public:
    explicit Entry(std::string placeholder)
        : Widget(WidgetKind::Entry), placeholder_(std::move(placeholder)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_text(const std::string& t) override {
        text_ = t;
        caret_ = text_.size();
        invalidate();
    }
    std::string text() const override { return text_; }

private:
    std::string placeholder_;
    std::string text_;
    size_t caret_ = 0;
};

class Slider : public Widget {
public:
    Slider(double min, double max, double value)
        : Widget(WidgetKind::Slider), min_(min), max_(max), value_(value) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_value(double v) override;
    double value() const override { return value_; }

private:
    double fraction() const {
        if (max_ <= min_) return 0.0;
        return (value_ - min_) / (max_ - min_);
    }
    double min_, max_, value_;
    bool dragging_ = false;
};

class Separator : public Widget {
public:
    explicit Separator(SLUIOrientation orient)
        : Widget(WidgetKind::Separator), orient_(orient) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;

private:
    SLUIOrientation orient_;
};

class Spacer : public Widget {
public:
    Spacer() : Widget(WidgetKind::Spacer) {
        hexpand_ = true;
        vexpand_ = true;
    }
    SLUISize measure(const PaintContext&) override { return SLUISize{0, 0}; }
    void paint(PaintContext&) override {}
};

/* Shared text helpers used by several widgets (declared here, defined in
 * slui_widget.cpp) so measuring and drawing a run stay consistent. */
double measure_text(Backend* backend, const std::string& utf8);
void draw_text(PaintContext& ctx, const std::string& utf8, int x, int baseline,
               const Color& color);

} // namespace slui

#endif /* SLUI_WIDGET_HPP */
