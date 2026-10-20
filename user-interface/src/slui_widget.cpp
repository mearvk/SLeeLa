/* =============================================================================
 * SleelaUI widget implementation: measure, arrange, paint, and input for every
 * built-in control. All drawing goes through the software rasterizer so the
 * Slick Black look is identical on every backend. Max Rupplin -- MEARVK LLC. */
#include "slui_widget.hpp"

#include "slui_backend.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>

namespace slui {

/* ---- UTF-8 decode (one scalar at a time) -------------------------------- */
static uint32_t next_scalar(const std::string& s, size_t& i) {
    unsigned char c = static_cast<unsigned char>(s[i]);
    uint32_t cp;
    int extra;
    if (c < 0x80) {
        cp = c;
        extra = 0;
    } else if ((c >> 5) == 0x6) {
        cp = c & 0x1F;
        extra = 1;
    } else if ((c >> 4) == 0xE) {
        cp = c & 0x0F;
        extra = 2;
    } else if ((c >> 3) == 0x1E) {
        cp = c & 0x07;
        extra = 3;
    } else {
        cp = 0xFFFD;
        extra = 0;
    }
    ++i;
    for (int k = 0; k < extra && i < s.size(); ++k, ++i) {
        cp = (cp << 6) | (static_cast<unsigned char>(s[i]) & 0x3F);
    }
    return cp;
}

double measure_text(Backend* backend, const std::string& utf8) {
    if (!backend) return 0.0;
    double w = 0.0;
    for (size_t i = 0; i < utf8.size();) {
        uint32_t cp = next_scalar(utf8, i);
        w += backend->glyph_advance(cp);
    }
    return w;
}

void draw_text(PaintContext& ctx, const std::string& utf8, int x, int baseline,
               const Color& color) {
    if (!ctx.backend || !ctx.canvas) return;
    double pen = x;
    for (size_t i = 0; i < utf8.size();) {
        uint32_t cp = next_scalar(utf8, i);
        GlyphBitmap g;
        if (ctx.backend->rasterize_glyph(cp, &g)) {
            ctx.canvas->blit_glyph(g, static_cast<int>(pen + 0.5), baseline, color);
            pen += g.advance;
        } else {
            pen += ctx.backend->glyph_advance(cp);
        }
    }
}

/* ---- base event routing ------------------------------------------------- */
bool Widget::on_event(const SLUIEvent& ev) {
    /* Front-to-back hit test for pointer events; broadcast for others. */
    bool positional = ev.type == SLUI_EVENT_POINTER_DOWN ||
                      ev.type == SLUI_EVENT_POINTER_UP ||
                      ev.type == SLUI_EVENT_POINTER_MOVE;
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        Widget* c = it->get();
        if (!c->visible_) continue;
        if (positional && !c->bounds_.contains(ev.x, ev.y)) {
            if (c->hovered_ && ev.type == SLUI_EVENT_POINTER_MOVE) {
                c->hovered_ = false;
                c->invalidate();
            }
            continue;
        }
        if (c->on_event(ev)) return true;
    }
    return false;
}

/* ======================================================================== */
/* Box / HeaderBar                                                          */
/* ======================================================================== */
SLUISize Box::measure(const PaintContext& ctx) {
    int main = 0, cross = 0;
    int visible_children = 0;
    for (auto& c : children_) {
        if (!c->visible()) continue;
        SLUISize cs = c->measure(ctx);
        if (orient_ == SLUI_ORIENT_HORIZONTAL) {
            main += cs.w;
            cross = std::max(cross, cs.h);
        } else {
            main += cs.h;
            cross = std::max(cross, cs.w);
        }
        ++visible_children;
    }
    if (visible_children > 1) main += spacing_ * (visible_children - 1);
    SLUISize s;
    if (orient_ == SLUI_ORIENT_HORIZONTAL) {
        s.w = main + margin_.left + margin_.right;
        s.h = cross + margin_.top + margin_.bottom;
    } else {
        s.w = cross + margin_.left + margin_.right;
        s.h = main + margin_.top + margin_.bottom;
    }
    s.w = std::max(s.w, min_w_);
    s.h = std::max(s.h, min_h_);
    return s;
}

void Box::arrange(const PaintContext& ctx, const Rect& area) {
    bounds_ = area;
    Rect box = content();

    /* Measure natural sizes, then fit every visible child into the parent's
     * available main-axis extent. Natural measurements are preferences, not
     * permission to allocate beyond the parent's frame. */
    std::vector<SLUISize> nat;
    std::vector<Widget*> vis;
    int natural_main = 0, expanders = 0;
    for (auto& c : children_) {
        if (!c->visible()) continue;
        SLUISize cs = c->measure(ctx);
        nat.push_back(cs);
        vis.push_back(c.get());
        const int child_main = orient_ == SLUI_ORIENT_HORIZONTAL ? cs.w : cs.h;
        natural_main += std::max(0, child_main);
        const bool exp = orient_ == SLUI_ORIENT_HORIZONTAL ? c->hexpand() : c->vexpand();
        if (exp) ++expanders;
    }
    const int count = static_cast<int>(vis.size());
    if (count == 0) return;

    const int avail_main = std::max(0, orient_ == SLUI_ORIENT_HORIZONTAL ? box.w : box.h);
    /* If the requested gaps themselves cannot fit, reduce them before sizing
     * children. This keeps every assigned rectangle inside the parent. */
    const int requested_spacing = std::max(0, spacing_);
    const int effective_spacing = count > 1
        ? std::min(requested_spacing, avail_main / (count - 1)) : 0;
    const int spacing_total = effective_spacing * (count - 1);
    const int content_main = std::max(0, avail_main - spacing_total);

    std::vector<int> main_sizes(static_cast<size_t>(count), 0);
    int assigned_main = 0;
    if (natural_main > content_main && natural_main > 0) {
        /* Compress proportionally when natural content cannot fit. Distribute
         * rounding pixels deterministically without exceeding the parent. */
        for (int i = 0; i < count; ++i) {
            const int natural = std::max(0, orient_ == SLUI_ORIENT_HORIZONTAL ? nat[i].w : nat[i].h);
            main_sizes[static_cast<size_t>(i)] =
                static_cast<int>((static_cast<long long>(natural) * content_main) / natural_main);
            assigned_main += main_sizes[static_cast<size_t>(i)];
        }
        int remainder = content_main - assigned_main;
        for (int i = 0; i < count && remainder > 0; ++i) {
            const int natural = std::max(0, orient_ == SLUI_ORIENT_HORIZONTAL ? nat[i].w : nat[i].h);
            if (natural > 0) {
                ++main_sizes[static_cast<size_t>(i)];
                --remainder;
            }
        }
    } else {
        for (int i = 0; i < count; ++i)
            main_sizes[static_cast<size_t>(i)] =
                std::max(0, orient_ == SLUI_ORIENT_HORIZONTAL ? nat[i].w : nat[i].h);
        int extra = content_main - natural_main;
        if (expanders > 0 && extra > 0) {
            const int per_expander = extra / expanders;
            int remainder = extra % expanders;
            for (int i = 0; i < count; ++i) {
                Widget* child = vis[static_cast<size_t>(i)];
                const bool exp = orient_ == SLUI_ORIENT_HORIZONTAL ? child->hexpand() : child->vexpand();
                if (!exp) continue;
                main_sizes[static_cast<size_t>(i)] += per_expander;
                if (remainder > 0) {
                    ++main_sizes[static_cast<size_t>(i)];
                    --remainder;
                }
            }
        }
    }

    int pos = orient_ == SLUI_ORIENT_HORIZONTAL ? box.x : box.y;
    for (int i = 0; i < count; ++i) {
        Widget* child = vis[static_cast<size_t>(i)];
        const SLUISize cs = nat[static_cast<size_t>(i)];
        const int main_size = main_sizes[static_cast<size_t>(i)];
        Rect cell = orient_ == SLUI_ORIENT_HORIZONTAL
            ? Rect{pos, box.y, main_size, std::max(0, box.h)}
            : Rect{box.x, pos, std::max(0, box.w), main_size};

        /* Cross-axis alignment can reduce a child, never enlarge it past the
         * allocated cell. If its natural size is too large, the cell wins. */
        Rect placed = cell;
        if (orient_ == SLUI_ORIENT_HORIZONTAL) {
            const SLUIAlign va = child->valign();
            if (va != SLUI_ALIGN_FILL && cs.h < cell.h) {
                placed.h = std::max(0, cs.h);
                if (va == SLUI_ALIGN_CENTER) placed.y = cell.y + (cell.h - placed.h) / 2;
                else if (va == SLUI_ALIGN_END) placed.y = cell.bottom() - placed.h;
            }
        } else {
            const SLUIAlign ha = child->halign();
            if (ha != SLUI_ALIGN_FILL && cs.w < cell.w) {
                placed.w = std::max(0, cs.w);
                if (ha == SLUI_ALIGN_CENTER) placed.x = cell.x + (cell.w - placed.w) / 2;
                else if (ha == SLUI_ALIGN_END) placed.x = cell.right() - placed.w;
            }
        }
        child->arrange(ctx, placed);
        pos += main_size + effective_spacing;
    }
}

void Box::paint(PaintContext& ctx) {
    // A container is a hard paint boundary: descendants may measure naturally,
    // but their pixels must never escape the allocated parent rectangle.
    if (ctx.canvas) ctx.canvas->push_clip(bounds_);
    paint_children(ctx);
    if (ctx.canvas) ctx.canvas->pop_clip();
}

void HeaderBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect b = bounds();
    ctx.canvas->fill_rect(b, t.chrome);
    /* 1px bottom hairline -- the quiet frame edge. */
    ctx.canvas->hline(b.x, b.right() - 1, b.bottom() - 1, t.border);
    // Header-bar children obey the same hard boundary as ordinary boxes.
    ctx.canvas->push_clip(b);
    paint_children(ctx);
    ctx.canvas->pop_clip();
}

/* ======================================================================== */
/* Label                                                                    */
/* ======================================================================== */
SLUISize Label::measure(const PaintContext& ctx) {
    double w = measure_text(ctx.backend, text_);
    double h = ctx.backend ? ctx.backend->font_line_height() : 16.0;
    SLUISize s{static_cast<int>(w + 0.5) + margin_.left + margin_.right,
               static_cast<int>(h + 0.5) + margin_.top + margin_.bottom};
    s.w = std::max(s.w, min_w_);
    s.h = std::max(s.h, min_h_);
    return s;
}

void Label::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    double asc = ctx.backend->font_ascent();
    double lh = ctx.backend->font_line_height();
    int baseline = c.y + static_cast<int>((c.h - lh) / 2.0 + asc + 0.5);
    Color col = sensitive_ ? t.fg : t.fg_dim;
    double tw = measure_text(ctx.backend, text_);
    int tx = c.x;
    if (halign_ == SLUI_ALIGN_CENTER) tx = c.x + static_cast<int>((c.w - tw) / 2.0);
    else if (halign_ == SLUI_ALIGN_END) tx = c.right() - static_cast<int>(tw);
    draw_text_styled(ctx, styled_font_, text_, tx, baseline, col);
}

/* ======================================================================== */
/* Button                                                                   */
/* ======================================================================== */
SLUISize Button::measure(const PaintContext& ctx) {
    double tw = measure_text(ctx.backend, text_);
    const Theme& t = *ctx.theme;
    int pad_x = t.unit * 4; /* 16px horizontal padding */
    int w = static_cast<int>(tw + 0.5) + pad_x * 2 + margin_.left + margin_.right;
    int h = t.control_height + margin_.top + margin_.bottom;
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}

void Button::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    Visual vis = visual_state();

    Color fill = suggested_ ? t.accent : t.surface;
    Color text_col = suggested_ ? t.accent_fg : t.fg;
    if (vis == Visual::Disabled) {
        fill = t.surface;
        text_col = t.fg_dim;
    }

    /* Draw base, then layer the small hover/active tint (restraint over
     * effects: a single background-opacity step, no glows). */
    ctx.canvas->fill_round_rect(c, t.radius, fill);
    if (vis == Visual::Hover) {
        Color tint = destructive_ ? t.danger.with_alpha(0x24) : t.hover;
        ctx.canvas->fill_round_rect(c, t.radius, tint);
    } else if (vis == Visual::Active) {
        ctx.canvas->fill_round_rect(c, t.radius, t.active);
    }
    if (!suggested_)
        ctx.canvas->stroke_round_rect(c, t.radius, 1.0, t.border);

    /* Focus ring: accent hairline just outside the fill. */
    if (focused_ && vis != Visual::Disabled) {
        Rect ring = c.inset(-1);
        ctx.canvas->stroke_round_rect(ring, t.radius + 1, 1.0, t.accent);
    }

    double asc = ctx.backend->font_ascent();
    double lh = ctx.backend->font_line_height();
    double tw = measure_text(ctx.backend, text_);
    int tx = c.x + static_cast<int>((c.w - tw) / 2.0 + 0.5);
    int baseline = c.y + static_cast<int>((c.h - lh) / 2.0 + asc + 0.5);
    if (destructive_ && vis != Visual::Disabled && !suggested_) text_col = t.danger;
    draw_text(ctx, text_, tx, baseline, text_col);
}

bool Button::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        if (!hovered_) {
            hovered_ = true;
            invalidate();
        }
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        pressed_ = true;
        set_focused(true);
        invalidate();
        return true;
    case SLUI_EVENT_POINTER_UP:
        if (pressed_) {
            pressed_ = false;
            invalidate();
            if (bounds_.contains(ev.x, ev.y)) emit_activate();
        }
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (focused_ && (ev.keysym == SLUI_KEY_ENTER || ev.keysym == SLUI_KEY_SPACE)) {
            emit_activate();
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* Toggle (switch)                                                          */
/* ======================================================================== */
SLUISize Toggle::measure(const PaintContext& ctx) {
    double tw = measure_text(ctx.backend, text_);
    const Theme& t = *ctx.theme;
    int track_w = t.control_height; /* 2:1 pill */
    int gap = t.unit * 2;
    int w = track_w + gap + static_cast<int>(tw + 0.5) + margin_.left + margin_.right;
    int h = t.control_height + margin_.top + margin_.bottom;
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}

void Toggle::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int track_h = t.control_height / 2;
    int track_w = t.control_height;
    Rect track{c.x, c.y + (c.h - track_h) / 2, track_w, track_h};
    Color track_col = on_ ? t.accent : t.surface_hi;
    if (!sensitive_) track_col = t.surface;
    ctx.canvas->fill_round_rect(track, track_h / 2.0, track_col);
    if (!on_) ctx.canvas->stroke_round_rect(track, track_h / 2.0, 1.0, t.border);

    int knob_d = track_h - 4;
    int knob_x = on_ ? track.right() - knob_d - 2 : track.x + 2;
    Rect knob{knob_x, track.y + 2, knob_d, knob_d};
    ctx.canvas->fill_round_rect(knob, knob_d / 2.0, on_ ? t.accent_fg : t.fg);

    if (focused_) {
        Rect ring = track.inset(-2);
        ctx.canvas->stroke_round_rect(ring, track_h / 2.0 + 2, 1.0, t.accent);
    }

    double asc = ctx.backend->font_ascent();
    double lh = ctx.backend->font_line_height();
    int tx = track.right() + t.unit * 2;
    int baseline = c.y + static_cast<int>((c.h - lh) / 2.0 + asc + 0.5);
    draw_text(ctx, text_, tx, baseline, sensitive_ ? t.fg : t.fg_dim);
}

bool Toggle::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        if (!hovered_) {
            hovered_ = true;
            invalidate();
        }
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        return true;
    case SLUI_EVENT_POINTER_UP:
        if (bounds_.contains(ev.x, ev.y)) {
            on_ = !on_;
            invalidate();
            emit_activate();
            emit_value_changed(on_ ? 1.0 : 0.0);
        }
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (focused_ && (ev.keysym == SLUI_KEY_ENTER || ev.keysym == SLUI_KEY_SPACE)) {
            on_ = !on_;
            invalidate();
            emit_activate();
            emit_value_changed(on_ ? 1.0 : 0.0);
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* Entry (single-line text field)                                           */
/* ======================================================================== */
SLUISize Entry::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int w = std::max(min_w_, 180) + margin_.left + margin_.right;
    int h = t.control_height + margin_.top + margin_.bottom;
    return SLUISize{w, std::max(h, min_h_)};
}

void Entry::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, t.radius, t.surface);
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, focused_ ? t.accent : t.border);

    int pad = t.unit * 3;
    double asc = ctx.backend->font_ascent();
    double lh = ctx.backend->font_line_height();
    int baseline = c.y + static_cast<int>((c.h - lh) / 2.0 + asc + 0.5);

    if (text_.empty() && !placeholder_.empty()) {
        draw_text(ctx, placeholder_, c.x + pad, baseline, t.fg_dim);
    } else {
        draw_text(ctx, text_, c.x + pad, baseline, t.fg);
    }

    if (focused_) {
        std::string before = text_.substr(0, caret_);
        int caret_x = c.x + pad + static_cast<int>(measure_text(ctx.backend, before) + 0.5);
        int cy0 = c.y + static_cast<int>((c.h - lh) / 2.0 + 2);
        int cy1 = cy0 + static_cast<int>(lh) - 4;
        ctx.canvas->vline(caret_x, cy0, cy1, t.accent);
    }
}

bool Entry::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        hovered_ = true;
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        caret_ = text_.size();
        invalidate();
        return true;
    case SLUI_EVENT_TEXT:
        if (focused_ && ev.codepoint >= 0x20 && ev.codepoint != 0x7F) {
            /* append utf-8 encoding of the scalar at the caret */
            std::string enc;
            uint32_t cp = ev.codepoint;
            if (cp < 0x80) {
                enc.push_back(static_cast<char>(cp));
            } else if (cp < 0x800) {
                enc.push_back(static_cast<char>(0xC0 | (cp >> 6)));
                enc.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            } else if (cp < 0x10000) {
                enc.push_back(static_cast<char>(0xE0 | (cp >> 12)));
                enc.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                enc.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            } else {
                enc.push_back(static_cast<char>(0xF0 | (cp >> 18)));
                enc.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
                enc.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
                enc.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
            }
            text_.insert(caret_, enc);
            caret_ += enc.size();
            invalidate();
            emit_value_changed(0.0);
            return true;
        }
        return false;
    case SLUI_EVENT_KEY_DOWN:
        if (!focused_) return false;
        if (ev.keysym == SLUI_KEY_BACKSPACE && caret_ > 0) {
            /* remove the whole scalar before the caret */
            size_t start = caret_ - 1;
            while (start > 0 &&
                   (static_cast<unsigned char>(text_[start]) & 0xC0) == 0x80)
                --start;
            text_.erase(start, caret_ - start);
            caret_ = start;
            invalidate();
            emit_value_changed(0.0);
            return true;
        }
        if (ev.keysym == SLUI_KEY_ENTER) {
            emit_activate();
            return true;
        }
        if (ev.keysym == SLUI_KEY_LEFT && caret_ > 0) {
            --caret_;
            while (caret_ > 0 &&
                   (static_cast<unsigned char>(text_[caret_]) & 0xC0) == 0x80)
                --caret_;
            invalidate();
            return true;
        }
        if (ev.keysym == SLUI_KEY_RIGHT && caret_ < text_.size()) {
            ++caret_;
            while (caret_ < text_.size() &&
                   (static_cast<unsigned char>(text_[caret_]) & 0xC0) == 0x80)
                ++caret_;
            invalidate();
            return true;
        }
        if (ev.keysym == SLUI_KEY_HOME) {
            caret_ = 0;
            invalidate();
            return true;
        }
        if (ev.keysym == SLUI_KEY_END) {
            caret_ = text_.size();
            invalidate();
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* Slider                                                                   */
/* ======================================================================== */
void Slider::set_value(double v) {
    v = std::clamp(v, std::min(min_, max_), std::max(min_, max_));
    if (v != value_) {
        value_ = v;
        invalidate();
        emit_value_changed(value_);
    }
}

SLUISize Slider::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int w = std::max(min_w_, 160) + margin_.left + margin_.right;
    int h = t.control_height + margin_.top + margin_.bottom;
    return SLUISize{w, std::max(h, min_h_)};
}

void Slider::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int track_h = 4;
    Rect track{c.x, c.y + (c.h - track_h) / 2, c.w, track_h};
    ctx.canvas->fill_round_rect(track, track_h / 2.0, t.surface_hi);

    int fill_w = static_cast<int>(track.w * fraction() + 0.5);
    Rect fill{track.x, track.y, fill_w, track_h};
    ctx.canvas->fill_round_rect(fill, track_h / 2.0, t.accent);

    int knob_d = t.unit * 4;
    int knob_x = track.x + fill_w - knob_d / 2;
    knob_x = std::clamp(knob_x, track.x, track.right() - knob_d);
    Rect knob{knob_x, c.y + (c.h - knob_d) / 2, knob_d, knob_d};
    ctx.canvas->fill_round_rect(knob, knob_d / 2.0, t.fg);
    if (focused_ || dragging_)
        ctx.canvas->stroke_round_rect(knob.inset(-2), knob_d / 2.0 + 2, 1.0, t.accent);
}

bool Slider::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    auto value_at = [&](int px) {
        Rect c = content();
        if (c.w <= 0) return min_;
        double f = std::clamp((px - c.x) / static_cast<double>(c.w), 0.0, 1.0);
        return min_ + f * (max_ - min_);
    };
    switch (ev.type) {
    case SLUI_EVENT_POINTER_DOWN:
        dragging_ = true;
        set_focused(true);
        set_value(value_at(ev.x));
        return true;
    case SLUI_EVENT_POINTER_MOVE:
        hovered_ = true;
        if (dragging_) set_value(value_at(ev.x));
        return true;
    case SLUI_EVENT_POINTER_UP:
        dragging_ = false;
        invalidate();
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (!focused_) return false;
        if (ev.keysym == SLUI_KEY_LEFT || ev.keysym == SLUI_KEY_DOWN) {
            set_value(value_ - (max_ - min_) / 100.0);
            return true;
        }
        if (ev.keysym == SLUI_KEY_RIGHT || ev.keysym == SLUI_KEY_UP) {
            set_value(value_ + (max_ - min_) / 100.0);
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* Separator                                                                */
/* ======================================================================== */
SLUISize Separator::measure(const PaintContext&) {
    if (orient_ == SLUI_ORIENT_HORIZONTAL)
        return SLUISize{min_w_, 1 + margin_.top + margin_.bottom};
    return SLUISize{1 + margin_.left + margin_.right, min_h_};
}

void Separator::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    if (orient_ == SLUI_ORIENT_HORIZONTAL)
        ctx.canvas->hline(c.x, c.right() - 1, c.y + c.h / 2, t.border);
    else
        ctx.canvas->vline(c.x + c.w / 2, c.y, c.bottom() - 1, t.border);
}

/* ======================================================================== */
/* Shared helpers for the expanded collection                               */
/* ======================================================================== */
namespace {
/* Baseline that vertically centres one text line inside a rect. */
int centered_baseline(const PaintContext& ctx, const Rect& r) {
    double asc = ctx.backend->font_ascent();
    double lh = ctx.backend->font_line_height();
    return r.y + static_cast<int>((r.h - lh) / 2.0 + asc + 0.5);
}
} // namespace

/* ======================================================================== */
/* CheckBox                                                                 */
/* ======================================================================== */
SLUISize CheckBox::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double tw = measure_text(ctx.backend, text_);
    int boxd = t.control_height - t.unit * 2;
    int w = boxd + t.unit * 2 + static_cast<int>(tw + 0.5) + margin_.left +
            margin_.right;
    int h = t.control_height + margin_.top + margin_.bottom;
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}

void CheckBox::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int boxd = t.control_height - t.unit * 2;
    Rect box{c.x, c.y + (c.h - boxd) / 2, boxd, boxd};
    Color fill = on_ ? t.accent : t.surface;
    if (!sensitive_) fill = t.surface;
    ctx.canvas->fill_round_rect(box, 5.0, fill);
    if (!on_) ctx.canvas->stroke_round_rect(box, 5.0, 1.0, t.border);
    if (on_) {
        /* A tick: two strokes forming a check, drawn as short thick lines. */
        Color m = t.accent_fg;
        int x0 = box.x + boxd / 4;
        int y0 = box.y + boxd / 2;
        int x1 = box.x + boxd / 2 - 1;
        int y1 = box.bottom() - boxd / 4;
        int x2 = box.right() - boxd / 4;
        int y2 = box.y + boxd / 4;
        for (int o = -1; o <= 1; ++o) {
            ctx.canvas->hline(x0, x1, y0 + o, m);
            ctx.canvas->vline(x1 + o, y0, y1, m);
            ctx.canvas->hline(x1, x2, y1 + o, m);
            ctx.canvas->vline(x2 + o, y2, y1, m);
        }
    }
    if (focused_)
        ctx.canvas->stroke_round_rect(box.inset(-2), 7.0, 1.0, t.accent);

    int baseline = centered_baseline(ctx, c);
    draw_text(ctx, text_, box.right() + t.unit * 2, baseline,
              sensitive_ ? t.fg : t.fg_dim);
}

bool CheckBox::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        hovered_ = true;
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        return true;
    case SLUI_EVENT_POINTER_UP:
        if (bounds_.contains(ev.x, ev.y)) {
            on_ = !on_;
            invalidate();
            emit_activate();
            emit_value_changed(on_ ? 1.0 : 0.0);
        }
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (focused_ && (ev.keysym == SLUI_KEY_SPACE || ev.keysym == SLUI_KEY_ENTER)) {
            on_ = !on_;
            invalidate();
            emit_activate();
            emit_value_changed(on_ ? 1.0 : 0.0);
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* RadioButton                                                              */
/* ======================================================================== */
SLUISize RadioButton::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double tw = measure_text(ctx.backend, text_);
    int d = t.control_height - t.unit * 2;
    int w = d + t.unit * 2 + static_cast<int>(tw + 0.5) + margin_.left +
            margin_.right;
    int h = t.control_height + margin_.top + margin_.bottom;
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}

void RadioButton::select_in_group() {
    if (!parent_) return;
    for (auto& sib : parent_->children()) {
        if (sib.get() == this) continue;
        if (sib->kind() == WidgetKind::RadioButton) {
            RadioButton* r = static_cast<RadioButton*>(sib.get());
            if (r->group() == group_ && r->toggle()) {
                r->set_toggle(false);
            }
        }
    }
    on_ = true;
}

void RadioButton::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int d = t.control_height - t.unit * 2;
    Rect disc{c.x, c.y + (c.h - d) / 2, d, d};
    ctx.canvas->fill_round_rect(disc, d / 2.0, on_ ? t.accent : t.surface);
    if (!on_) ctx.canvas->stroke_round_rect(disc, d / 2.0, 1.0, t.border);
    if (on_) {
        int id = d / 3;
        Rect dot{disc.x + (d - id) / 2, disc.y + (d - id) / 2, id, id};
        ctx.canvas->fill_round_rect(dot, id / 2.0, t.accent_fg);
    }
    if (focused_)
        ctx.canvas->stroke_round_rect(disc.inset(-2), d / 2.0 + 2, 1.0, t.accent);

    int baseline = centered_baseline(ctx, c);
    draw_text(ctx, text_, disc.right() + t.unit * 2, baseline,
              sensitive_ ? t.fg : t.fg_dim);
}

bool RadioButton::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        hovered_ = true;
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        return true;
    case SLUI_EVENT_POINTER_UP:
        if (bounds_.contains(ev.x, ev.y) && !on_) {
            select_in_group();
            invalidate();
            emit_activate();
            emit_value_changed(1.0);
        }
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (focused_ && (ev.keysym == SLUI_KEY_SPACE || ev.keysym == SLUI_KEY_ENTER) &&
            !on_) {
            select_in_group();
            invalidate();
            emit_activate();
            emit_value_changed(1.0);
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* ProgressBar                                                              */
/* ======================================================================== */
void ProgressBar::set_value(double v) {
    value_ = std::clamp(v, 0.0, 1.0);
    invalidate();
}
SLUISize ProgressBar::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int w = std::max(min_w_, 160) + margin_.left + margin_.right;
    int h = std::max(t.unit * 2, 8) + margin_.top + margin_.bottom;
    return SLUISize{w, std::max(h, min_h_)};
}
void ProgressBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int h = std::min(c.h, t.unit * 2);
    Rect track{c.x, c.y + (c.h - h) / 2, c.w, h};
    ctx.canvas->fill_round_rect(track, h / 2.0, t.surface_hi);
    int fw = static_cast<int>(c.w * value_ + 0.5);
    if (fw > 0) {
        Rect fill{track.x, track.y, fw, h};
        ctx.canvas->fill_round_rect(fill, h / 2.0, t.accent);
    }
}

/* ======================================================================== */
/* LevelBar                                                                 */
/* ======================================================================== */
void LevelBar::set_value(double v) {
    value_ = std::clamp(v, 0.0, 1.0);
    invalidate();
}
SLUISize LevelBar::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int w = std::max(min_w_, 160) + margin_.left + margin_.right;
    int h = std::max(t.unit * 3, 10) + margin_.top + margin_.bottom;
    return SLUISize{w, std::max(h, min_h_)};
}
void LevelBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int segments = 10;
    int gap = t.unit;
    int segw = (c.w - gap * (segments - 1)) / segments;
    if (segw < 1) segw = 1;
    int lit = static_cast<int>(value_ * segments + 0.5);
    int h = std::min(c.h, t.unit * 3);
    int y = c.y + (c.h - h) / 2;
    for (int i = 0; i < segments; ++i) {
        Rect seg{c.x + i * (segw + gap), y, segw, h};
        ctx.canvas->fill_round_rect(seg, 2.0, i < lit ? t.accent : t.surface_hi);
    }
}

/* ======================================================================== */
/* Spinner                                                                  */
/* ======================================================================== */
SLUISize Spinner::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int d = t.control_height;
    return SLUISize{d + margin_.left + margin_.right,
                    d + margin_.top + margin_.bottom};
}
void Spinner::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int d = std::min(c.w, c.h);
    Rect ring{c.x + (c.w - d) / 2, c.y + (c.h - d) / 2, d, d};
    /* Faint full ring, then a bright accent arc whose head follows phase_. */
    ctx.canvas->stroke_round_rect(ring, d / 2.0, 2.0, t.surface_hi);
    /* Approximate the arc with a short bright chord near the phase angle by
     * tinting a small inset wedge rect; keeps the rasterizer simple while
     * reading as "activity". */
    double cx = ring.x + d / 2.0;
    double cy = ring.y + d / 2.0;
    double r = d / 2.0;
    for (int k = 0; k < 90; ++k) {
        double ang = (phase_ * 360.0 + k) * 3.14159265 / 180.0;
        int px = static_cast<int>(cx + std::cos(ang) * r + 0.5);
        int py = static_cast<int>(cy + std::sin(ang) * r + 0.5);
        ctx.canvas->fill_rect(Rect{px - 1, py - 1, 2, 2}, t.accent);
    }
}

/* ======================================================================== */
/* Frame                                                                    */
/* ======================================================================== */
SLUISize Frame::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int pad = t.unit * 3;
    int titleH = title_.empty() ? 0 : static_cast<int>(ctx.backend->font_line_height());
    int inner_w = 0, inner_h = 0;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        inner_w = std::max(inner_w, cs.w);
        inner_h += cs.h;
    }
    int w = inner_w + pad * 2 + margin_.left + margin_.right;
    int h = inner_h + pad * 2 + titleH + margin_.top + margin_.bottom;
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}
void Frame::arrange(const PaintContext& ctx, const Rect& area) {
    bounds_ = area;
    const Theme& t = *ctx.theme;
    int pad = t.unit * 3;
    int titleH = title_.empty() ? 0 : static_cast<int>(ctx.backend->font_line_height());
    Rect box = content().inset(pad + titleH, pad, pad, pad);
    int y = box.y;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        ch->arrange(ctx, Rect{box.x, y, box.w, cs.h});
        y += cs.h;
    }
}
void Frame::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, t.border);
    if (!title_.empty()) {
        int baseline = c.y + static_cast<int>(ctx.backend->font_ascent()) + t.unit;
        draw_text(ctx, title_, c.x + t.unit * 3, baseline, t.fg_dim);
    }
    paint_children(ctx);
}

/* ======================================================================== */
/* Card                                                                     */
/* ======================================================================== */
SLUISize Card::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int pad = t.unit * 4;
    int inner_w = 0, inner_h = 0, n = 0;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        inner_w = std::max(inner_w, cs.w);
        inner_h += cs.h;
        ++n;
    }
    if (n > 1) inner_h += t.unit * 2 * (n - 1);
    return SLUISize{std::max(inner_w + pad * 2 + margin_.left + margin_.right, min_w_),
                    std::max(inner_h + pad * 2 + margin_.top + margin_.bottom, min_h_)};
}
void Card::arrange(const PaintContext& ctx, const Rect& area) {
    bounds_ = area;
    const Theme& t = *ctx.theme;
    int pad = t.unit * 4;
    Rect box = content().inset(pad);
    int y = box.y;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        ch->arrange(ctx, Rect{box.x, y, box.w, cs.h});
        y += cs.h + t.unit * 2;
    }
}
void Card::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, t.radius, t.surface);
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, t.border);
    paint_children(ctx);
}

/* ======================================================================== */
/* Grid                                                                     */
/* ======================================================================== */
SLUISize Grid::measure(const PaintContext& ctx) {
    int cellw = 0, cellh = 0, n = 0;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        cellw = std::max(cellw, cs.w);
        cellh = std::max(cellh, cs.h);
        ++n;
    }
    int rows = (n + columns_ - 1) / columns_;
    if (rows < 1) rows = 1;
    int w = cellw * columns_ + spacing_ * (columns_ - 1) + margin_.left +
            margin_.right;
    int h = cellh * rows + spacing_ * (rows - 1) + margin_.top + margin_.bottom;
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}
void Grid::arrange(const PaintContext& ctx, const Rect& area) {
    bounds_ = area;
    Rect box = content();
    int n = 0, cellw = 0, cellh = 0;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        cellw = std::max(cellw, cs.w);
        cellh = std::max(cellh, cs.h);
        ++n;
    }
    /* Expand cells to fill the available width evenly. */
    int availw = (box.w - spacing_ * (columns_ - 1)) / columns_;
    if (availw > cellw) cellw = availw;
    int i = 0;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        int col = i % columns_;
        int row = i / columns_;
        int x = box.x + col * (cellw + spacing_);
        int y = box.y + row * (cellh + spacing_);
        ch->arrange(ctx, Rect{x, y, cellw, cellh});
        ++i;
    }
}
void Grid::paint(PaintContext& ctx) { paint_children(ctx); }

/* ======================================================================== */
/* Image (themed placeholder tile)                                          */
/* ======================================================================== */
SLUISize Image::measure(const PaintContext&) {
    return SLUISize{w_ + margin_.left + margin_.right,
                    h_ + margin_.top + margin_.bottom};
}
void Image::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    Rect tile{c.x, c.y, w_, h_};
    ctx.canvas->fill_round_rect(tile, t.radius, t.surface_hi);
    ctx.canvas->stroke_round_rect(tile, t.radius, 1.0, t.border);
    if (!glyph_.empty()) {
        double tw = measure_text(ctx.backend, glyph_);
        int baseline = centered_baseline(ctx, tile);
        draw_text(ctx, glyph_, tile.x + static_cast<int>((w_ - tw) / 2.0 + 0.5),
                  baseline, t.fg_dim);
    }
}

/* ======================================================================== */
/* Avatar                                                                   */
/* ======================================================================== */
SLUISize Avatar::measure(const PaintContext&) {
    return SLUISize{diameter_ + margin_.left + margin_.right,
                    diameter_ + margin_.top + margin_.bottom};
}
void Avatar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    Rect disc{c.x, c.y, diameter_, diameter_};
    ctx.canvas->fill_round_rect(disc, diameter_ / 2.0, t.accent);
    if (!initial_.empty()) {
        double tw = measure_text(ctx.backend, initial_);
        int baseline = centered_baseline(ctx, disc);
        draw_text(ctx, initial_,
                  disc.x + static_cast<int>((diameter_ - tw) / 2.0 + 0.5),
                  baseline, t.accent_fg);
    }
}

/* ======================================================================== */
/* Badge                                                                    */
/* ======================================================================== */
SLUISize Badge::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double tw = measure_text(ctx.backend, text_);
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit;
    int w = std::max(static_cast<int>(tw + 0.5) + t.unit * 2, h);
    return SLUISize{w + margin_.left + margin_.right,
                    h + margin_.top + margin_.bottom};
}
void Badge::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit;
    double tw = measure_text(ctx.backend, text_);
    int w = std::max(static_cast<int>(tw + 0.5) + t.unit * 2, h);
    Rect pill{c.x, c.y + (c.h - h) / 2, w, h};
    ctx.canvas->fill_round_rect(pill, h / 2.0, t.accent);
    int baseline = centered_baseline(ctx, pill);
    draw_text(ctx, text_, pill.x + static_cast<int>((w - tw) / 2.0 + 0.5),
              baseline, t.accent_fg);
}

/* ======================================================================== */
/* Chip                                                                     */
/* ======================================================================== */
SLUISize Chip::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double tw = measure_text(ctx.backend, text_);
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit * 2;
    int w = static_cast<int>(tw + 0.5) + t.unit * 4;
    return SLUISize{w + margin_.left + margin_.right,
                    h + margin_.top + margin_.bottom};
}
void Chip::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit * 2;
    Rect pill{c.x, c.y + (c.h - h) / 2, c.w, h};
    ctx.canvas->fill_round_rect(pill, h / 2.0, t.surface);
    ctx.canvas->stroke_round_rect(pill, h / 2.0, 1.0, t.border);
    int baseline = centered_baseline(ctx, pill);
    draw_text(ctx, text_, pill.x + t.unit * 2, baseline, t.fg);
}

/* ======================================================================== */
/* LinkButton                                                               */
/* ======================================================================== */
SLUISize LinkButton::measure(const PaintContext& ctx) {
    double tw = measure_text(ctx.backend, text_);
    double h = ctx.backend->font_line_height();
    return SLUISize{static_cast<int>(tw + 0.5) + margin_.left + margin_.right,
                    static_cast<int>(h + 0.5) + margin_.top + margin_.bottom};
}
void LinkButton::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    int baseline = centered_baseline(ctx, c);
    Color col = sensitive_ ? t.accent : t.fg_dim;
    double tw = measure_text(ctx.backend, text_);
    draw_text(ctx, text_, c.x, baseline, col);
    /* Underline on hover/focus. */
    if (hovered_ || focused_)
        ctx.canvas->hline(c.x, c.x + static_cast<int>(tw), baseline + 2, col);
}
bool LinkButton::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        hovered_ = true;
        invalidate();
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        return true;
    case SLUI_EVENT_POINTER_UP:
        if (bounds_.contains(ev.x, ev.y)) emit_activate();
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (focused_ && (ev.keysym == SLUI_KEY_ENTER || ev.keysym == SLUI_KEY_SPACE)) {
            emit_activate();
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* SearchEntry / PasswordEntry                                              */
/* ======================================================================== */
SLUISize SearchEntry::measure(const PaintContext& ctx) {
    SLUISize s = Entry::measure(ctx);
    s.w += ctx.theme->control_height; /* room for the leading glyph */
    return s;
}
void SearchEntry::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    /* Draw the entry surface + border like the base, then a magnifier glyph. */
    ctx.canvas->fill_round_rect(c, t.radius, t.surface);
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, focused_ ? t.accent : t.border);
    int gy = c.y + c.h / 2;
    int gx = c.x + t.unit * 3;
    int r = t.unit;
    /* a tiny circle + handle approximated with rects */
    ctx.canvas->stroke_round_rect(Rect{gx, gy - r, r * 2, r * 2}, r, 1.0, t.fg_dim);
    ctx.canvas->hline(gx + r * 2, gx + r * 2 + r, gy + r, t.fg_dim);
    int baseline = centered_baseline(ctx, c);
    int tx = gx + r * 3 + t.unit;
    std::string shown = text();
    if (shown.empty())
        draw_text(ctx, "Search", tx, baseline, t.fg_dim);
    else
        draw_text(ctx, shown, tx, baseline, t.fg);
}
void PasswordEntry::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, t.radius, t.surface);
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, focused_ ? t.accent : t.border);
    int pad = t.unit * 3;
    int baseline = centered_baseline(ctx, c);
    std::string shown = text();
    if (shown.empty()) {
        draw_text(ctx, "Password", c.x + pad, baseline, t.fg_dim);
    } else {
        /* Count scalars and draw that many bullet dots. */
        std::string dots;
        for (size_t i = 0; i < shown.size(); ++i)
            if ((static_cast<unsigned char>(shown[i]) & 0xC0) != 0x80)
                dots += "\xe2\x80\xa2"; /* U+2022 bullet */
        draw_text(ctx, dots, c.x + pad, baseline, t.fg);
    }
}

/* ======================================================================== */
/* Heading                                                                  */
/* ======================================================================== */
SLUISize Heading::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    ctx.backend->set_font(t.font_family, size_);
    double tw = measure_text(ctx.backend, text_);
    double h = ctx.backend->font_line_height();
    ctx.backend->set_font(t.font_family, t.font_size); /* restore */
    return SLUISize{static_cast<int>(tw + 0.5) + margin_.left + margin_.right,
                    static_cast<int>(h + 0.5) + margin_.top + margin_.bottom};
}
void Heading::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.backend->set_font(t.font_family, size_);
    int baseline = centered_baseline(ctx, c);
    draw_text_styled(ctx, styled_font_, text_, c.x, baseline, t.fg);
    ctx.backend->set_font(t.font_family, t.font_size); /* restore base font */
}

/* ======================================================================== */
/* ScrollBar                                                                */
/* ======================================================================== */
void ScrollBar::set_value(double v) {
    value_ = std::clamp(v, 0.0, 1.0);
    invalidate();
    emit_value_changed(value_);
}
SLUISize ScrollBar::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int thick = t.unit * 3;
    if (orient_ == SLUI_ORIENT_VERTICAL)
        return SLUISize{thick + margin_.left + margin_.right,
                        std::max(min_h_, 80) + margin_.top + margin_.bottom};
    return SLUISize{std::max(min_w_, 80) + margin_.left + margin_.right,
                    thick + margin_.top + margin_.bottom};
}
void ScrollBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, std::min(c.w, c.h) / 2.0, t.surface);
    double page = std::clamp(page_, 0.05, 1.0);
    if (orient_ == SLUI_ORIENT_VERTICAL) {
        int th = static_cast<int>(c.h * page);
        int ty = c.y + static_cast<int>((c.h - th) * value_);
        Rect thumb{c.x, ty, c.w, th};
        ctx.canvas->fill_round_rect(thumb, c.w / 2.0, focused_ ? t.accent : t.border);
    } else {
        int tw = static_cast<int>(c.w * page);
        int tx = c.x + static_cast<int>((c.w - tw) * value_);
        Rect thumb{tx, c.y, tw, c.h};
        ctx.canvas->fill_round_rect(thumb, c.h / 2.0, focused_ ? t.accent : t.border);
    }
}
bool ScrollBar::on_event(const SLUIEvent& ev) {
    auto val_at = [&](int px, int py) {
        Rect c = content();
        if (orient_ == SLUI_ORIENT_VERTICAL)
            return c.h > 0 ? std::clamp((py - c.y) / static_cast<double>(c.h), 0.0, 1.0) : 0.0;
        return c.w > 0 ? std::clamp((px - c.x) / static_cast<double>(c.w), 0.0, 1.0) : 0.0;
    };
    switch (ev.type) {
    case SLUI_EVENT_POINTER_DOWN:
        dragging_ = true;
        set_focused(true);
        set_value(val_at(ev.x, ev.y));
        return true;
    case SLUI_EVENT_POINTER_MOVE:
        if (dragging_) set_value(val_at(ev.x, ev.y));
        return true;
    case SLUI_EVENT_POINTER_UP:
        dragging_ = false;
        return true;
    default:
        return false;
    }
}

/* ======================================================================== */
/* ScrolledView                                                             */
/* ======================================================================== */
ScrolledView::ScrolledView(int policy_h, int policy_v)
    : Widget(WidgetKind::ScrolledView),
      policy_h_(policy_h), policy_v_(policy_v) {
    /* Own two scroll bars. They live in children_ (so they paint and route
     * events like any child) but are NOT the scroll content. Each bar drives
     * the matching content offset through its value_changed handler. */
    auto v = std::make_unique<ScrollBar>(SLUI_ORIENT_VERTICAL, 0.0, 1.0);
    vbar_ = v.get();
    add_child(std::move(v));
    auto h = std::make_unique<ScrollBar>(SLUI_ORIENT_HORIZONTAL, 0.0, 1.0);
    hbar_ = h.get();
    add_child(std::move(h));

    vbar_->on_value_changed(
        [](SLUIWidget*, double val, void* u) {
            auto* self = static_cast<ScrolledView*>(u);
            self->set_scroll_y(
                static_cast<int>(val * self->max_offset_y() + 0.5));
        },
        this);
    hbar_->on_value_changed(
        [](SLUIWidget*, double val, void* u) {
            auto* self = static_cast<ScrolledView*>(u);
            self->set_scroll_x(
                static_cast<int>(val * self->max_offset_x() + 0.5));
        },
        this);
}

Widget* ScrolledView::content_child() const {
    for (auto& c : children_) {
        if (c.get() == vbar_ || c.get() == hbar_) continue;
        return c.get();
    }
    return nullptr;
}

bool ScrolledView::bar_visible_x() const {
    if (policy_h_ == SCROLL_NEVER) return false;
    if (policy_h_ == SCROLL_ALWAYS) return true;
    return overflow_x();
}
bool ScrolledView::bar_visible_y() const {
    if (policy_v_ == SCROLL_NEVER) return false;
    if (policy_v_ == SCROLL_ALWAYS) return true;
    return overflow_y();
}

void ScrolledView::set_scroll_x(int px) {
    int mx = max_offset_x();
    offset_x_ = std::clamp(px, 0, mx);
    invalidate();
}
void ScrolledView::set_scroll_y(int px) {
    int my = max_offset_y();
    offset_y_ = std::clamp(px, 0, my);
    invalidate();
}

SLUISize ScrolledView::measure(const PaintContext& ctx) {
    /* A viewport reports the size it wants to OCCUPY, which is intentionally
     * decoupled from its content's (possibly larger) size -- reporting the
     * content size here is exactly what would make the viewport overflow its
     * own parent, defeating the point. So:
     *   - a pinned axis (uiWidgetSizeRequest) reports that fixed size;
     *   - an un-pinned axis reports a MODEST default viewport, capped so it is
     *     never larger than a sane window region, and never the full content.
     * Set hexpand/vexpand (uiWidgetExpand) to have the parent grow the viewport
     * to fill the available space; the content then scrolls inside whatever the
     * parent allocates. The parent box still decides the final allocation. */
    SLUISize cs{0, 0};
    if (Widget* c = content_child()) cs = c->measure(ctx);
    /* Default viewport extents: large enough to be useful, small enough to fit
     * inside a typical window so the view does not push its parent wider/taller
     * than the screen. Content beyond these scrolls. */
    const int kDefaultViewW = 320;
    const int kDefaultViewH = 240;
    int wantW = (min_w_ > 0) ? min_w_ : std::min(cs.w, kDefaultViewW);
    int wantH = (min_h_ > 0) ? min_h_ : std::min(cs.h, kDefaultViewH);
    SLUISize s;
    s.w = wantW + margin_.left + margin_.right;
    s.h = wantH + margin_.top + margin_.bottom;
    return s;
}

void ScrolledView::arrange(const PaintContext& ctx, const Rect& area) {
    bounds_ = area;
    Rect box = content();           /* viewport minus this widget's margin */
    const Theme& t = *ctx.theme;
    int gutter = t.unit * 3;         /* matches ScrollBar::measure thickness */

    Widget* child = content_child();
    SLUISize cs{0, 0};
    if (child) cs = child->measure(ctx);
    content_w_ = cs.w;
    content_h_ = cs.h;

    /* Decide bar visibility. Reserving a gutter for one bar reduces the other
     * axis' viewport, which can itself trigger overflow; resolve in two passes
     * so a corner case (content just fits until a gutter appears) settles. */
    bool showV = false, showH = false;
    for (int pass = 0; pass < 2; ++pass) {
        int vw = box.w - (showV ? gutter : 0);
        int vh = box.h - (showH ? gutter : 0);
        bool ofx = content_w_ > vw;
        bool ofy = content_h_ > vh;
        showV = (policy_v_ == SCROLL_ALWAYS) ||
                (policy_v_ == SCROLL_AUTO && ofy);
        showH = (policy_h_ == SCROLL_ALWAYS) ||
                (policy_h_ == SCROLL_AUTO && ofx);
    }

    view_w_ = box.w - (showV ? gutter : 0);
    view_h_ = box.h - (showH ? gutter : 0);
    if (view_w_ < 0) view_w_ = 0;
    if (view_h_ < 0) view_h_ = 0;

    /* Re-clamp the offsets to the (possibly new) max and place the content at
     * the negative offset so scrolling reveals material below/right. */
    offset_x_ = std::clamp(offset_x_, 0, max_offset_x());
    offset_y_ = std::clamp(offset_y_, 0, max_offset_y());

    if (child) {
        int cw = std::max(content_w_, view_w_);   /* fill the viewport if smaller */
        int ch = std::max(content_h_, view_h_);
        Rect placed{box.x - offset_x_, box.y - offset_y_, cw, ch};
        child->arrange(ctx, placed);
    }

    /* Place the bars in their gutters along the inner edges. */
    vbar_->set_visible(showV);
    hbar_->set_visible(showH);
    if (showV) {
        vbar_->arrange(ctx, Rect{box.right() - gutter, box.y, gutter, view_h_});
    }
    if (showH) {
        hbar_->arrange(ctx, Rect{box.x, box.bottom() - gutter, view_w_, gutter});
    }
    sync_bars();
}

void ScrolledView::sync_bars() {
    /* page = fraction of content visible (thumb size); value = offset fraction. */
    if (content_h_ > 0) {
        double page = std::clamp(view_h_ / static_cast<double>(content_h_), 0.05, 1.0);
        double val = max_offset_y() > 0
                         ? offset_y_ / static_cast<double>(max_offset_y())
                         : 0.0;
        vbar_->set_page(page);
        vbar_->set_value_silent(val);
    }
    if (content_w_ > 0) {
        double page = std::clamp(view_w_ / static_cast<double>(content_w_), 0.05, 1.0);
        double val = max_offset_x() > 0
                         ? offset_x_ / static_cast<double>(max_offset_x())
                         : 0.0;
        hbar_->set_page(page);
        hbar_->set_value_silent(val);
    }
}

void ScrolledView::paint(PaintContext& ctx) {
    Rect box = content();
    /* Clip the content to the viewport (minus any reserved gutters) so the
     * overflow is hidden rather than spilling past the frame. */
    ctx.canvas->push_clip(Rect{box.x, box.y, view_w_, view_h_});
    if (Widget* child = content_child()) {
        if (child->visible()) child->paint(ctx);
    }
    ctx.canvas->pop_clip();
    /* The bars paint unclipped in their gutters. */
    if (vbar_->visible()) vbar_->paint(ctx);
    if (hbar_->visible()) hbar_->paint(ctx);
}

bool ScrolledView::on_event(const SLUIEvent& ev) {
    /* Give the scroll bars first refusal (they sit in the gutters), then route
     * positional events into the clipped content with the scroll offset applied
     * so hit-testing matches what the user sees. */
    bool positional = ev.type == SLUI_EVENT_POINTER_DOWN ||
                      ev.type == SLUI_EVENT_POINTER_UP ||
                      ev.type == SLUI_EVENT_POINTER_MOVE;
    if (vbar_->visible() && (!positional || vbar_->bounds().contains(ev.x, ev.y))) {
        if (vbar_->on_event(ev)) return true;
    }
    if (hbar_->visible() && (!positional || hbar_->bounds().contains(ev.x, ev.y))) {
        if (hbar_->on_event(ev)) return true;
    }
    Widget* child = content_child();
    if (child && child->visible()) {
        if (positional) {
            Rect box = content();
            /* Only deliver pointer events that fall inside the visible viewport. */
            if (!Rect{box.x, box.y, view_w_, view_h_}.contains(ev.x, ev.y))
                return false;
        }
        return child->on_event(ev);
    }
    return false;
}

/* ======================================================================== */
/* StatusBar                                                                */
/* ======================================================================== */
SLUISize StatusBar::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit * 2;
    int w = 0;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        w += cs.w + t.unit * 2;
    }
    return SLUISize{std::max(w, min_w_), std::max(h, min_h_)};
}
void StatusBar::arrange(const PaintContext& ctx, const Rect& area) {
    bounds_ = area;
    const Theme& t = *ctx.theme;
    Rect box = content().inset(0, t.unit * 2, 0, t.unit * 2);
    int x = box.x;
    for (auto& ch : children_) {
        if (!ch->visible()) continue;
        SLUISize cs = ch->measure(ctx);
        ch->arrange(ctx, Rect{x, box.y, cs.w, box.h});
        x += cs.w + t.unit * 2;
    }
}
void StatusBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_rect(c, t.chrome);
    ctx.canvas->hline(c.x, c.right() - 1, c.y, t.border);
    paint_children(ctx);
}

/* ======================================================================== */
/* InfoBar                                                                  */
/* ======================================================================== */
SLUISize InfoBar::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double tw = measure_text(ctx.backend, text_);
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit * 3;
    return SLUISize{std::max(static_cast<int>(tw + 0.5) + t.unit * 6, min_w_) +
                        margin_.left + margin_.right,
                    std::max(h, min_h_) + margin_.top + margin_.bottom};
}
void InfoBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    Color base = (severity_ == WARNING)
                     ? Color{0xE0, 0xA5, 0x3A, 0xFF}
                     : (severity_ == ERROR) ? t.danger : t.accent;
    ctx.canvas->fill_round_rect(c, t.radius, base.with_alpha(0x2A));
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, base.with_alpha(0x80));
    /* severity accent bar on the left */
    ctx.canvas->fill_round_rect(Rect{c.x, c.y, t.unit, c.h}, 1.0, base);
    int baseline = centered_baseline(ctx, c);
    draw_text(ctx, text_, c.x + t.unit * 3, baseline, t.fg);
}

/* ======================================================================== */
/* Tooltip                                                                  */
/* ======================================================================== */
SLUISize Tooltip::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double tw = measure_text(ctx.backend, text_);
    int h = static_cast<int>(ctx.backend->font_line_height()) + t.unit * 2;
    return SLUISize{static_cast<int>(tw + 0.5) + t.unit * 4 + margin_.left +
                        margin_.right,
                    h + margin_.top + margin_.bottom};
}
void Tooltip::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, 6.0, t.surface_hi);
    ctx.canvas->stroke_round_rect(c, 6.0, 1.0, t.border);
    int baseline = centered_baseline(ctx, c);
    draw_text(ctx, text_, c.x + t.unit * 2, baseline, t.fg);
}

/* ======================================================================== */
/* ComboBox                                                                 */
/* ======================================================================== */
void ComboBox::set_value(double v) {
    if (options_.empty()) return;
    long i = static_cast<long>(v + 0.5);
    if (i < 0) i = 0;
    if (i >= static_cast<long>(options_.size())) i = options_.size() - 1;
    index_ = static_cast<size_t>(i);
    invalidate();
}
SLUISize ComboBox::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    double widest = 0;
    for (auto& o : options_) widest = std::max(widest, measure_text(ctx.backend, o));
    int w = static_cast<int>(widest + 0.5) + t.unit * 4 + t.control_height;
    int h = t.control_height;
    return SLUISize{std::max(w, min_w_) + margin_.left + margin_.right,
                    std::max(h, min_h_) + margin_.top + margin_.bottom};
}
void ComboBox::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, t.radius, t.surface);
    if (hovered_) ctx.canvas->fill_round_rect(c, t.radius, t.hover);
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, focused_ ? t.accent : t.border);
    int baseline = centered_baseline(ctx, c);
    draw_text(ctx, text(), c.x + t.unit * 3, baseline, t.fg);
    /* chevron on the right */
    int cx = c.right() - t.unit * 4;
    int cy = c.y + c.h / 2;
    for (int k = 0; k < t.unit + 1; ++k) {
        ctx.canvas->hline(cx - k, cx + k, cy - t.unit / 2 + k, t.fg_dim);
    }
}
bool ComboBox::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_MOVE:
        hovered_ = true;
        invalidate();
        return true;
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        return true;
    case SLUI_EVENT_POINTER_UP:
        if (bounds_.contains(ev.x, ev.y) && !options_.empty()) {
            index_ = (index_ + 1) % options_.size();
            invalidate();
            emit_activate();
            emit_value_changed(static_cast<double>(index_));
        }
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (!focused_ || options_.empty()) return false;
        if (ev.keysym == SLUI_KEY_DOWN || ev.keysym == SLUI_KEY_RIGHT) {
            index_ = (index_ + 1) % options_.size();
            invalidate();
            emit_value_changed(static_cast<double>(index_));
            return true;
        }
        if (ev.keysym == SLUI_KEY_UP || ev.keysym == SLUI_KEY_LEFT) {
            index_ = (index_ + options_.size() - 1) % options_.size();
            invalidate();
            emit_value_changed(static_cast<double>(index_));
            return true;
        }
        return false;
    default:
        return false;
    }
}

/* ======================================================================== */
/* SpinButton                                                               */
/* ======================================================================== */
void SpinButton::set_value(double v) {
    value_ = std::clamp(v, min_, max_);
    invalidate();
    emit_value_changed(value_);
}
SLUISize SpinButton::measure(const PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    int w = t.control_height * 2 + 80;
    int h = t.control_height;
    return SLUISize{std::max(w, min_w_) + margin_.left + margin_.right,
                    std::max(h, min_h_) + margin_.top + margin_.bottom};
}
void SpinButton::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect c = content();
    ctx.canvas->fill_round_rect(c, t.radius, t.surface);
    ctx.canvas->stroke_round_rect(c, t.radius, 1.0, focused_ ? t.accent : t.border);
    int bw = t.control_height;
    Rect minus{c.x, c.y, bw, c.h};
    Rect plus{c.right() - bw, c.y, bw, c.h};
    /* steppers */
    ctx.canvas->vline(minus.right(), c.y + 4, c.bottom() - 4, t.border);
    ctx.canvas->vline(plus.x, c.y + 4, c.bottom() - 4, t.border);
    int my = c.y + c.h / 2;
    ctx.canvas->hline(minus.x + bw / 3, minus.right() - bw / 3, my, t.fg);
    ctx.canvas->hline(plus.x + bw / 3, plus.right() - bw / 3, my, t.fg);
    ctx.canvas->vline(plus.x + bw / 2, my - bw / 6, my + bw / 6, t.fg);
    /* value, centred */
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%g", value_);
    std::string s(buf);
    double tw = measure_text(ctx.backend, s);
    int baseline = centered_baseline(ctx, c);
    draw_text(ctx, s, c.x + static_cast<int>((c.w - tw) / 2.0 + 0.5), baseline, t.fg);
}
bool SpinButton::on_event(const SLUIEvent& ev) {
    if (!sensitive_) return false;
    const int bw = 34;
    switch (ev.type) {
    case SLUI_EVENT_POINTER_DOWN:
        set_focused(true);
        if (ev.x < bounds_.x + bw) set_value(value_ - step_);
        else if (ev.x > bounds_.right() - bw) set_value(value_ + step_);
        return true;
    case SLUI_EVENT_KEY_DOWN:
        if (!focused_) return false;
        if (ev.keysym == SLUI_KEY_UP || ev.keysym == SLUI_KEY_RIGHT) {
            set_value(value_ + step_);
            return true;
        }
        if (ev.keysym == SLUI_KEY_DOWN || ev.keysym == SLUI_KEY_LEFT) {
            set_value(value_ - step_);
            return true;
        }
        return false;
    default:
        return false;
    }
}

} // namespace slui
