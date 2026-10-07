/* =============================================================================
 * SleelaUI widget implementation: measure, arrange, paint, and input for every
 * built-in control. All drawing goes through the software rasterizer so the
 * Slick Black look is identical on every backend. Max Rupplin -- MEARVK LLC. */
#include "slui_widget.hpp"

#include "slui_backend.hpp"

#include <algorithm>

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

    /* First pass: natural sizes and count expanders along the main axis. */
    std::vector<SLUISize> nat;
    std::vector<Widget*> vis;
    int natural_main = 0, expanders = 0, count = 0;
    for (auto& c : children_) {
        if (!c->visible()) continue;
        SLUISize cs = c->measure(ctx);
        nat.push_back(cs);
        vis.push_back(c.get());
        bool exp = (orient_ == SLUI_ORIENT_HORIZONTAL) ? c->hexpand() : c->vexpand();
        natural_main += (orient_ == SLUI_ORIENT_HORIZONTAL) ? cs.w : cs.h;
        if (exp) ++expanders;
        ++count;
    }
    if (count == 0) return;

    int avail_main = (orient_ == SLUI_ORIENT_HORIZONTAL) ? box.w : box.h;
    int spacing_total = spacing_ * (count - 1);
    int extra = avail_main - natural_main - spacing_total;
    int per_expander = (expanders > 0 && extra > 0) ? extra / expanders : 0;
    int remainder = (expanders > 0 && extra > 0) ? extra % expanders : 0;

    int pos = (orient_ == SLUI_ORIENT_HORIZONTAL) ? box.x : box.y;
    for (size_t i = 0; i < vis.size(); ++i) {
        Widget* c = vis[i];
        SLUISize cs = nat[i];
        bool exp = (orient_ == SLUI_ORIENT_HORIZONTAL) ? c->hexpand() : c->vexpand();
        int main_size = (orient_ == SLUI_ORIENT_HORIZONTAL) ? cs.w : cs.h;
        if (exp && extra > 0) {
            main_size += per_expander;
            if (remainder > 0) {
                main_size += 1;
                --remainder;
            }
        }
        Rect cell;
        if (orient_ == SLUI_ORIENT_HORIZONTAL) {
            cell = Rect{pos, box.y, main_size, box.h};
        } else {
            cell = Rect{box.x, pos, box.w, main_size};
        }
        /* Cross-axis alignment: shrink the cell to the child's natural cross
         * size unless the child fills, then offset for start/center/end. */
        Rect placed = cell;
        if (orient_ == SLUI_ORIENT_HORIZONTAL) {
            SLUIAlign va = c->valign();
            if (va != SLUI_ALIGN_FILL && cs.h < cell.h) {
                placed.h = cs.h;
                if (va == SLUI_ALIGN_CENTER)
                    placed.y = cell.y + (cell.h - cs.h) / 2;
                else if (va == SLUI_ALIGN_END)
                    placed.y = cell.bottom() - cs.h;
            }
        } else {
            SLUIAlign ha = c->halign();
            if (ha != SLUI_ALIGN_FILL && cs.w < cell.w) {
                placed.w = cs.w;
                if (ha == SLUI_ALIGN_CENTER)
                    placed.x = cell.x + (cell.w - cs.w) / 2;
                else if (ha == SLUI_ALIGN_END)
                    placed.x = cell.right() - cs.w;
            }
        }
        c->arrange(ctx, placed);
        pos += main_size + spacing_;
    }
}

void Box::paint(PaintContext& ctx) {
    paint_children(ctx);
}

void HeaderBar::paint(PaintContext& ctx) {
    const Theme& t = *ctx.theme;
    Rect b = bounds();
    ctx.canvas->fill_rect(b, t.chrome);
    /* 1px bottom hairline -- the quiet frame edge. */
    ctx.canvas->hline(b.x, b.right() - 1, b.bottom() - 1, t.border);
    paint_children(ctx);
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
    draw_text(ctx, text_, tx, baseline, col);
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

} // namespace slui
