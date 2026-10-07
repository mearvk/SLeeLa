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
    /* Original set */
    Box,
    HeaderBar,
    Label,
    Button,
    Toggle,
    Entry,
    Slider,
    Separator,
    Spacer,
    /* Expanded collection */
    CheckBox,
    RadioButton,
    ProgressBar,
    LevelBar,
    Spinner,
    Frame,
    Card,
    Grid,
    Image,
    Avatar,
    Badge,
    Chip,
    LinkButton,
    SearchEntry,
    PasswordEntry,
    Heading,
    ScrollBar,
    StatusBar,
    InfoBar,
    Tooltip,
    ComboBox,
    SpinButton,
    /* Motion / drawing */
    Throbber,
    CanvasView
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

    /* Activation latch: set on every activate, read-and-cleared by the host.
     * The C callback path (on_activate) and this latch coexist, so a C host can
     * use callbacks and a polling host (e.g. the SLeeLa `ui*` bridge) can use
     * take_activated() for exactly-once observation. */
    bool take_activated() {
        bool a = activated_;
        activated_ = false;
        return a;
    }

protected:
    void emit_activate() {
        activated_ = true;
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
    bool activated_ = false; /* one-shot activation latch (take_activated) */

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

/* ======================================================================== */
/* Expanded widget collection                                               */
/* The look of every new widget is drawn by the same software rasterizer and  */
/* reads only theme roles, so each is pixel-identical on every backend and     */
/* obeys the UI principles (one palette, one accent, >=32px hit targets).      */
/* ======================================================================== */

/* A labelled check box: a rounded square that shows an accent tick when on. */
class CheckBox : public Widget {
public:
    CheckBox(std::string text, bool on)
        : Widget(WidgetKind::CheckBox), text_(std::move(text)), on_(on) {}
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

/* A labelled radio button: a circle with an accent dot when selected. Radios
 * sharing a group ordinal are mutually exclusive within their parent. */
class RadioButton : public Widget {
public:
    RadioButton(std::string text, int group, bool on)
        : Widget(WidgetKind::RadioButton), text_(std::move(text)),
          group_(group), on_(on) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }
    void set_toggle(bool on) override { on_ = on; invalidate(); }
    bool toggle() const override { return on_; }
    int group() const { return group_; }

private:
    void select_in_group();
    std::string text_;
    int group_;
    bool on_;
};

/* A determinate progress bar in [0,1] (uses value()/set_value()). */
class ProgressBar : public Widget {
public:
    explicit ProgressBar(double fraction)
        : Widget(WidgetKind::ProgressBar), value_(fraction) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_value(double v) override;
    double value() const override { return value_; }

private:
    double value_;
};

/* A segmented level bar (battery/volume style) over [0,1]. */
class LevelBar : public Widget {
public:
    explicit LevelBar(double fraction)
        : Widget(WidgetKind::LevelBar), value_(fraction) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_value(double v) override;
    double value() const override { return value_; }

private:
    double value_;
};

/* An indeterminate activity spinner: an accent arc on a faint ring. The
 * fraction of the ring that is bright advances with the animation phase. */
class Spinner : public Widget {
public:
    Spinner() : Widget(WidgetKind::Spinner) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_value(double v) override { phase_ = v; invalidate(); }
    double value() const override { return phase_; }

private:
    double phase_ = 0.0; /* 0..1 position of the bright arc */
};

/* A titled, bordered container (a group frame). Lays out children in a
 * vertical stack inside the border, below the title. */
class Frame : public Widget {
public:
    explicit Frame(std::string title)
        : Widget(WidgetKind::Frame), title_(std::move(title)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void arrange(const PaintContext& ctx, const Rect& area) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { title_ = t; invalidate(); }
    std::string text() const override { return title_; }

private:
    std::string title_;
};

/* A raised surface panel (a card): rounded surface fill + hairline, children
 * stacked vertically inside padding. */
class Card : public Widget {
public:
    Card() : Widget(WidgetKind::Card) {}
    SLUISize measure(const PaintContext& ctx) override;
    void arrange(const PaintContext& ctx, const Rect& area) override;
    void paint(PaintContext& ctx) override;
};

/* A simple fixed row/column grid container. Children fill cells in row-major
 * order; the grid sizes columns/rows to the largest child in each line. */
class Grid : public Widget {
public:
    Grid(int columns, int spacing)
        : Widget(WidgetKind::Grid), columns_(columns < 1 ? 1 : columns),
          spacing_(spacing) {}
    SLUISize measure(const PaintContext& ctx) override;
    void arrange(const PaintContext& ctx, const Rect& area) override;
    void paint(PaintContext& ctx) override;

private:
    int columns_;
    int spacing_;
};

/* A placeholder image/icon tile: a rounded surface with a centred glyph. In a
 * real asset pipeline this would blit a decoded image; here it draws a themed
 * monogram so layouts are complete and backend-independent. */
class Image : public Widget {
public:
    Image(std::string glyph, int w, int h)
        : Widget(WidgetKind::Image), glyph_(std::move(glyph)), w_(w), h_(h) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { glyph_ = t; invalidate(); }
    std::string text() const override { return glyph_; }

private:
    std::string glyph_;
    int w_, h_;
};

/* A round avatar showing an initial on an accent disc. */
class Avatar : public Widget {
public:
    Avatar(std::string initial, int diameter)
        : Widget(WidgetKind::Avatar), initial_(std::move(initial)),
          diameter_(diameter) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { initial_ = t; invalidate(); }
    std::string text() const override { return initial_; }

private:
    std::string initial_;
    int diameter_;
};

/* A small count/status pill (badge) drawn in the accent colour. */
class Badge : public Widget {
public:
    explicit Badge(std::string text)
        : Widget(WidgetKind::Badge), text_(std::move(text)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
};

/* A chip / tag: a rounded surface pill with a hairline and text. */
class Chip : public Widget {
public:
    explicit Chip(std::string text)
        : Widget(WidgetKind::Chip), text_(std::move(text)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
};

/* A hyperlink-style button: accent-coloured text, activates like a button. */
class LinkButton : public Widget {
public:
    explicit LinkButton(std::string text)
        : Widget(WidgetKind::LinkButton), text_(std::move(text)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
};

/* An entry with a leading search glyph. Behaves like Entry otherwise. */
class SearchEntry : public Entry {
public:
    explicit SearchEntry(std::string placeholder) : Entry(std::move(placeholder)) {
        kind_ = WidgetKind::SearchEntry;
    }
    void paint(PaintContext& ctx) override;
    SLUISize measure(const PaintContext& ctx) override;
};

/* An entry that renders dots instead of its characters. */
class PasswordEntry : public Entry {
public:
    explicit PasswordEntry(std::string placeholder) : Entry(std::move(placeholder)) {
        kind_ = WidgetKind::PasswordEntry;
    }
    void paint(PaintContext& ctx) override;
};

/* A large title label (heading). */
class Heading : public Widget {
public:
    Heading(std::string text, int levelPt)
        : Widget(WidgetKind::Heading), text_(std::move(text)), size_(levelPt) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
    int size_;
};

/* A thin scroll bar indicator (position 0..1, thumb covers `page` fraction). */
class ScrollBar : public Widget {
public:
    ScrollBar(SLUIOrientation orient, double value, double page)
        : Widget(WidgetKind::ScrollBar), orient_(orient), value_(value),
          page_(page) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_value(double v) override;
    double value() const override { return value_; }

private:
    SLUIOrientation orient_;
    double value_, page_;
    bool dragging_ = false;
};

/* A footer status bar: chrome strip + top hairline, lays children horizontally. */
class StatusBar : public Widget {
public:
    StatusBar() : Widget(WidgetKind::StatusBar) {}
    SLUISize measure(const PaintContext& ctx) override;
    void arrange(const PaintContext& ctx, const Rect& area) override;
    void paint(PaintContext& ctx) override;
};

/* An inline info/notice bar: a tinted surface strip with a message. The tint
 * follows a severity (info/accent, warning, error/danger). */
class InfoBar : public Widget {
public:
    enum Severity { INFO = 0, WARNING = 1, ERROR = 2 };
    InfoBar(std::string text, int severity)
        : Widget(WidgetKind::InfoBar), text_(std::move(text)),
          severity_(severity) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
    int severity_;
};

/* A floating tooltip bubble: a small raised surface with a hairline + text. */
class Tooltip : public Widget {
public:
    explicit Tooltip(std::string text)
        : Widget(WidgetKind::Tooltip), text_(std::move(text)) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    void set_text(const std::string& t) override { text_ = t; invalidate(); }
    std::string text() const override { return text_; }

private:
    std::string text_;
};

/* A drop-down combo box showing the current selection and a chevron. Cycles
 * through its options on click (a compact, dependency-free selection control). */
class ComboBox : public Widget {
public:
    ComboBox() : Widget(WidgetKind::ComboBox) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void add_option(const std::string& o) {
        options_.push_back(o);
        invalidate();
    }
    void set_value(double v) override;
    double value() const override { return static_cast<double>(index_); }
    std::string text() const override {
        return options_.empty() ? std::string() : options_[index_];
    }

private:
    std::vector<std::string> options_;
    size_t index_ = 0;
};

/* A numeric spin button: a value with - and + steppers. */
class SpinButton : public Widget {
public:
    SpinButton(double min, double max, double step, double value)
        : Widget(WidgetKind::SpinButton), min_(min), max_(max), step_(step),
          value_(value) {}
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;
    bool on_event(const SLUIEvent& ev) override;
    bool wants_focus() const override { return true; }
    void set_value(double v) override;
    double value() const override { return value_; }

private:
    double min_, max_, step_, value_;
};

/* ======================================================================== */
/* Throbber -- a width-adjustable, full-motion, colour-predictive, water-like  */
/* flowing activity field. See slui_throbber.{hpp,cpp}.                        */
/* ======================================================================== */

/* A developer/agent draw callback for the CanvasView widget. It is handed the
 * view's own double-buffered draw context each frame plus the animation time
 * and the per-frame delta, and draws whatever it likes with the Draw API.
 * SLUIDrawContext is the opaque type from sleela_ui.h / sleela_ui_draw.h, which
 * is a struct at global scope (declared outside this namespace below). */
typedef void (*CanvasDrawFn)(::SLUIDrawContext* dc, double time_s, double dt_s,
                             void* user);

/* A general animated drawing surface: a widget that owns a double-buffered
 * draw context sized to its bounds and, each frame, calls a developer draw
 * callback then presents + blits the result. This is the host for custom
 * visualisations and the substrate the Throbber is built on. It animates while
 * visible and reports a target refresh rate the window's loop can honour. */
class CanvasView : public Widget {
public:
    CanvasView(int min_w, int min_h);
    ~CanvasView() override;
    SLUISize measure(const PaintContext& ctx) override;
    void paint(PaintContext& ctx) override;

    void set_draw_fn(CanvasDrawFn fn, void* user) {
        draw_fn_ = fn;
        draw_user_ = user;
    }
    void set_fps(double fps) { fps_ = fps > 0 ? fps : 0.0; }
    double fps() const { return fps_; }
    /* Advance animation time; the window's animation loop calls this. */
    void advance(double dt) {
        time_ += dt;
        last_dt_ = dt;
        invalidate();
    }
    bool animated() const { return true; }

protected:
    int req_w_, req_h_;
    double time_ = 0.0;
    double last_dt_ = 0.0;
    double fps_ = 60.0;
    CanvasDrawFn draw_fn_ = nullptr;
    void* draw_user_ = nullptr;
    SLUIDrawContext* ctx_ = nullptr; /* owned draw context (lazy)            */
    int ctx_w_ = 0, ctx_h_ = 0;

    void ensure_context(int w, int h);
};

/* The Throbber is a CanvasView whose draw callback is an internal water-like
 * flow field: a smoothly advected height/velocity field whose crests light up
 * in a predictive hue that leads the motion. Width is adjustable; the field
 * rescales to any width while keeping the same physical feel. */
class Throbber : public CanvasView {
public:
    explicit Throbber(int width_px);
    ~Throbber() override;
    void set_width_px(int w);
    int width_px() const { return req_w_; }
    /* 0 = calm trickle ... 1 = vigorous flow. Drives speed + colour spread. */
    void set_intensity(double v);
    double intensity() const { return intensity_; }
    /* Base hue (degrees) the predictive colouring flows around. */
    void set_hue(double degrees);
    double hue() const { return hue_; }

private:
    double intensity_ = 0.6;
    double hue_ = 205.0; /* airy light-blue by default (water) */
};

/* Shared text helpers used by several widgets (declared here, defined in
 * slui_widget.cpp) so measuring and drawing a run stay consistent. */
double measure_text(Backend* backend, const std::string& utf8);
void draw_text(PaintContext& ctx, const std::string& utf8, int x, int baseline,
               const Color& color);

} // namespace slui

#endif /* SLUI_WIDGET_HPP */
