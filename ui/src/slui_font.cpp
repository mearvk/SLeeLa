/* =============================================================================
 * SleelaUI Font & Font-effect implementation.
 *
 * Backs sleela_ui_font.h. A font is a face (family/size/weight/slant/spacing)
 * plus an ordered stack of effects. Rendering is a two-stage pipeline over the
 * toolkit's rasterizer:
 *
 *   1. COVERAGE. We rasterize the string once into an 8-bit coverage MASK (a
 *      little scratch bitmap) via the shared text backend -- the union of every
 *      glyph's coverage, with letter spacing and a synthetic oblique shear if
 *      requested. Measuring the mask once lets every effect reuse it.
 *   2. COMPOSITE. Effects are drawn back-to-front into the draw context from the
 *      mask: drop/inner shadows (offset + box-blurred dark copies), a soft glow
 *      (blurred, additive, coloured halo), an outline (the mask dilated by the
 *      stroke width), a relief emboss/engrave (shade the mask by a lighting
 *      normal built from the coverage gradient), a gradient or flat FILL, and a
 *      directional light sheen on the glyph face. An EMITTER effect doesn't draw
 *      -- it radiates into a light scene (reserving nothing, or binding as a
 *      source on an anchor) exactly like the Lighting layer's emitters.
 *
 * Quality (draft..ultra) scales the blur taps and the outline/relief angular
 * samples, so a developer can dial richness up or down per font.
 *
 * All pixels come from the toolkit's own rasterizer, so a styled string is
 * identical on every backend.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui_font.h"

#include "sleela_ui_light.h"
#include "slui_backend.hpp"
#include "slui_render.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

using slui::Backend;
using slui::Color;
using slui::GlyphBitmap;

/* ---- UTF-8 decode (one scalar) ------------------------------------------ */
static uint32_t next_scalar(const std::string& s, size_t& i) {
    unsigned char c = static_cast<unsigned char>(s[i]);
    uint32_t cp;
    int extra;
    if (c < 0x80) { cp = c; extra = 0; }
    else if ((c >> 5) == 0x6) { cp = c & 0x1F; extra = 1; }
    else if ((c >> 4) == 0xE) { cp = c & 0x0F; extra = 2; }
    else if ((c >> 3) == 0x1E) { cp = c & 0x07; extra = 3; }
    else { cp = 0xFFFD; extra = 0; }
    ++i;
    for (int k = 0; k < extra && i < s.size(); ++k, ++i)
        cp = (cp << 6) | (static_cast<unsigned char>(s[i]) & 0x3F);
    return cp;
}

/* ---- SLUIFont ----------------------------------------------------------- */
struct SLUIFont {
    std::string family = "system";
    double size = 11.0;
    SLUIFontWeight weight = SLUI_FONT_REGULAR;
    SLUIFontSlant slant = SLUI_SLANT_UPRIGHT;
    double letter_spacing = 0.0;
    double line_height_mult = 1.3;
    SLUIFontQuality quality = SLUI_QUALITY_MEDIUM;
    std::vector<SLUIFontEffect> effects;
};

/* A single-channel coverage mask with an origin offset so effects know where
 * the baseline/pen sits inside it. */
struct Mask {
    int w = 0, h = 0;
    int ox = 0, oy = 0; /* pen origin (x,baseline) inside the mask           */
    std::vector<uint8_t> a;
    uint8_t at(int x, int y) const {
        if (x < 0 || y < 0 || x >= w || y >= h) return 0;
        return a[static_cast<size_t>(y) * w + x];
    }
};

static Color decode(SLUIColor c) {
    return Color{static_cast<uint8_t>((c >> 24) & 0xFF),
                 static_cast<uint8_t>((c >> 16) & 0xFF),
                 static_cast<uint8_t>((c >> 8) & 0xFF),
                 static_cast<uint8_t>(c & 0xFF)};
}
static double clampd(double v, double lo, double hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static void apply_face(Backend* be, SLUIFont* f) {
    be->set_font(f->family, static_cast<int>(f->size + 0.5));
}

/* Rasterize `utf8` to a coverage mask with padding `pad` around it. */
static Mask build_mask(Backend* be, SLUIFont* f, const std::string& utf8,
                       int pad) {
    apply_face(be, f);
    double asc = be->font_ascent(), desc = be->font_descent();
    /* First pass: total advance. */
    double adv = 0;
    for (size_t i = 0; i < utf8.size();) {
        uint32_t cp = next_scalar(utf8, i);
        adv += be->glyph_advance(cp) + f->letter_spacing;
    }
    int tw = static_cast<int>(adv + 0.5);
    if (tw < 1) tw = 1;
    int th = static_cast<int>(asc + desc + 0.5);
    if (th < 1) th = 1;

    Mask m;
    m.w = tw + pad * 2;
    m.h = th + pad * 2;
    m.ox = pad;
    m.oy = pad + static_cast<int>(asc + 0.5); /* baseline row */
    m.a.assign(static_cast<size_t>(m.w) * m.h, 0);

    /* Synthetic oblique shear slope (used when slant requested; a real italic
     * face would already shear, but we add a gentle extra for OBLIQUE). */
    double shear = (f->slant == SLUI_SLANT_OBLIQUE) ? 0.22 : 0.0;

    double pen = 0;
    for (size_t i = 0; i < utf8.size();) {
        uint32_t cp = next_scalar(utf8, i);
        GlyphBitmap g;
        if (be->rasterize_glyph(cp, &g)) {
            int gx = m.ox + static_cast<int>(pen + 0.5) + g.left;
            int gy = m.oy - g.top;
            for (int row = 0; row < g.height; ++row) {
                int sh = static_cast<int>(shear * (g.top - row));
                for (int colp = 0; colp < g.width; ++colp) {
                    uint8_t cov = g.coverage[static_cast<size_t>(row) * g.width + colp];
                    if (!cov) continue;
                    int x = gx + colp + sh;
                    int y = gy + row;
                    if (x < 0 || y < 0 || x >= m.w || y >= m.h) continue;
                    uint8_t& dst = m.a[static_cast<size_t>(y) * m.w + x];
                    dst = std::max(dst, cov); /* union of glyph coverage */
                }
            }
            pen += g.advance + f->letter_spacing;
        } else {
            pen += be->glyph_advance(cp) + f->letter_spacing;
        }
    }
    /* Faux-bold: dilate the coverage by 1px for heavy weights. */
    if (f->weight >= SLUI_FONT_BOLD) {
        std::vector<uint8_t> src = m.a;
        for (int y = 0; y < m.h; ++y)
            for (int x = 0; x < m.w; ++x) {
                uint8_t mx = src[static_cast<size_t>(y) * m.w + x];
                if (x + 1 < m.w) mx = std::max(mx, src[static_cast<size_t>(y) * m.w + x + 1]);
                m.a[static_cast<size_t>(y) * m.w + x] = mx;
            }
    }
    return m;
}

/* Separable box blur of a coverage mask, `radius` px, `taps` passes. Returns a
 * new float field (0..1) the caller composites. */
static std::vector<double> blur_mask(const Mask& m, double radius, int taps) {
    std::vector<double> cur(static_cast<size_t>(m.w) * m.h);
    for (size_t i = 0; i < cur.size(); ++i) cur[i] = m.a[i] / 255.0;
    int r = std::max(1, static_cast<int>(radius + 0.5));
    for (int pass = 0; pass < taps; ++pass) {
        std::vector<double> tmp(cur.size(), 0.0);
        /* horizontal */
        for (int y = 0; y < m.h; ++y) {
            for (int x = 0; x < m.w; ++x) {
                double s = 0;
                int n = 0;
                for (int k = -r; k <= r; ++k) {
                    int xx = x + k;
                    if (xx < 0 || xx >= m.w) continue;
                    s += cur[static_cast<size_t>(y) * m.w + xx];
                    ++n;
                }
                tmp[static_cast<size_t>(y) * m.w + x] = n ? s / n : 0;
            }
        }
        /* vertical */
        for (int x = 0; x < m.w; ++x) {
            for (int y = 0; y < m.h; ++y) {
                double s = 0;
                int n = 0;
                for (int k = -r; k <= r; ++k) {
                    int yy = y + k;
                    if (yy < 0 || yy >= m.h) continue;
                    s += tmp[static_cast<size_t>(yy) * m.w + x];
                    ++n;
                }
                cur[static_cast<size_t>(y) * m.w + x] = n ? s / n : 0;
            }
        }
    }
    return cur;
}

static int quality_taps(SLUIFontQuality q) {
    switch (q) {
    case SLUI_QUALITY_DRAFT: return 1;
    case SLUI_QUALITY_LOW: return 1;
    case SLUI_QUALITY_MEDIUM: return 2;
    case SLUI_QUALITY_HIGH: return 3;
    case SLUI_QUALITY_ULTRA: return 4;
    default: return 2;
    }
}
static int quality_angles(SLUIFontQuality q) {
    switch (q) {
    case SLUI_QUALITY_DRAFT: return 4;
    case SLUI_QUALITY_LOW: return 6;
    case SLUI_QUALITY_MEDIUM: return 8;
    case SLUI_QUALITY_HIGH: return 16;
    case SLUI_QUALITY_ULTRA: return 32;
    default: return 8;
    }
}

/* Composite a blurred float field tinted `c` at offset (dx,dy) into dc. */
static void composite_field(SLUIDrawContext* dc, const Mask& m,
                            const std::vector<double>& field, int base_x,
                            int base_y, double dx, double dy, Color c,
                            double opacity, SLUIBlendMode mode) {
    SLUIBlendMode prev = slui_draw_get_blend(dc);
    slui_draw_set_blend(dc, mode);
    for (int y = 0; y < m.h; ++y) {
        for (int x = 0; x < m.w; ++x) {
            double a = field[static_cast<size_t>(y) * m.w + x];
            if (a <= 0.003) continue;
            int px = base_x + (x - m.ox) + static_cast<int>(dx + 0.5);
            int py = base_y + (y - m.oy) + static_cast<int>(dy + 0.5);
            SLUIColor word = slui_rgba(c.r, c.g, c.b, 255);
            slui_draw_blend_pixel(dc, px, py, word,
                                  clampd(a * opacity, 0.0, 1.0));
        }
    }
    slui_draw_set_blend(dc, prev);
}

/* ==========================================================================
 * Effect builders
 * ========================================================================== */
extern "C" {

static SLUIFontEffect fx_zero(SLUIFontEffectType t) {
    SLUIFontEffect e;
    std::memset(&e, 0, sizeof(e));
    e.type = t;
    e.opacity = 1.0;
    e.intensity = 1.0;
    e.reserved_anchor = -1;
    return e;
}

SLUIFontEffect slui_fx_drop_shadow(double dx, double dy, double blur,
                                   SLUIColor color) {
    SLUIFontEffect e = fx_zero(SLUI_FX_DROP_SHADOW);
    e.dx = dx;
    e.dy = dy;
    e.blur = blur;
    e.color = color;
    e.opacity = 0.6;
    return e;
}
SLUIFontEffect slui_fx_inner_shadow(double dx, double dy, double blur,
                                    SLUIColor color) {
    SLUIFontEffect e = fx_zero(SLUI_FX_INNER_SHADOW);
    e.dx = dx;
    e.dy = dy;
    e.blur = blur;
    e.color = color;
    e.opacity = 0.7;
    return e;
}
SLUIFontEffect slui_fx_glow(double radius, double intensity, SLUIColor color) {
    SLUIFontEffect e = fx_zero(SLUI_FX_GLOW);
    e.blur = radius;
    e.intensity = intensity;
    e.color = color;
    return e;
}
SLUIFontEffect slui_fx_light(double dx, double dy, double intensity,
                             SLUIColor color) {
    SLUIFontEffect e = fx_zero(SLUI_FX_LIGHT);
    e.dx = dx;
    e.dy = dy;
    e.intensity = intensity;
    e.color = color;
    return e;
}
SLUIFontEffect slui_fx_emitter(double radius, double intensity, SLUIColor color,
                               int anchor) {
    SLUIFontEffect e = fx_zero(SLUI_FX_EMITTER);
    e.size = radius;
    e.intensity = intensity;
    e.color = color;
    e.reserved_anchor = anchor;
    return e;
}
SLUIFontEffect slui_fx_outline(double width, SLUIColor color) {
    SLUIFontEffect e = fx_zero(SLUI_FX_OUTLINE);
    e.size = width;
    e.color = color;
    return e;
}
SLUIFontEffect slui_fx_relief(SLUIFontRelief relief, double depth,
                              double intensity) {
    SLUIFontEffect e = fx_zero(SLUI_FX_RELIEF);
    e.relief = relief;
    e.size = depth;
    e.intensity = intensity;
    return e;
}
SLUIFontEffect slui_fx_gradient_fill(SLUIColor top, SLUIColor bottom) {
    SLUIFontEffect e = fx_zero(SLUI_FX_GRADIENT_FILL);
    e.color = top;
    e.color2 = bottom;
    return e;
}

/* ==========================================================================
 * Font lifecycle
 * ========================================================================== */
SLUIFont* slui_font_create(const char* family, double size_pt) {
    auto* f = new (std::nothrow) SLUIFont();
    if (!f) return nullptr;
    if (family && family[0]) f->family = family;
    if (size_pt > 0) f->size = size_pt;
    return f;
}
void slui_font_destroy(SLUIFont* f) { delete f; }
SLUIFont* slui_font_clone(const SLUIFont* f) {
    if (!f) return nullptr;
    return new (std::nothrow) SLUIFont(*f);
}

void slui_font_set_family(SLUIFont* f, const char* family) {
    if (f && family) f->family = family;
}
void slui_font_set_size(SLUIFont* f, double s) {
    if (f && s > 0) f->size = s;
}
void slui_font_set_weight(SLUIFont* f, SLUIFontWeight w) {
    if (f) f->weight = w;
}
void slui_font_set_slant(SLUIFont* f, SLUIFontSlant s) {
    if (f) f->slant = s;
}
void slui_font_set_letter_spacing(SLUIFont* f, double px) {
    if (f) f->letter_spacing = px;
}
void slui_font_set_line_height(SLUIFont* f, double mult) {
    if (f && mult > 0) f->line_height_mult = mult;
}
void slui_font_set_quality(SLUIFont* f, SLUIFontQuality q) {
    if (f) f->quality = q;
}
double slui_font_size(const SLUIFont* f) { return f ? f->size : 0; }
SLUIFontQuality slui_font_quality(const SLUIFont* f) {
    return f ? f->quality : SLUI_QUALITY_MEDIUM;
}

void slui_font_add_effect(SLUIFont* f, SLUIFontEffect e) {
    if (f) f->effects.push_back(e);
}
void slui_font_clear_effects(SLUIFont* f) {
    if (f) f->effects.clear();
}
int slui_font_effect_count(const SLUIFont* f) {
    return f ? static_cast<int>(f->effects.size()) : 0;
}

/* ==========================================================================
 * Measuring
 * ========================================================================== */
double slui_font_measure(SLUIFont* f, const char* utf8) {
    if (!f || !utf8) return 0;
    Backend* be = slui::shared_text_backend();
    if (!be) return 0;
    apply_face(be, f);
    double adv = 0;
    std::string s(utf8);
    for (size_t i = 0; i < s.size();) {
        uint32_t cp = next_scalar(s, i);
        adv += be->glyph_advance(cp) + f->letter_spacing;
    }
    return adv;
}
double slui_font_ascent(SLUIFont* f) {
    Backend* be = slui::shared_text_backend();
    if (!f || !be) return 0;
    apply_face(be, f);
    return be->font_ascent();
}
double slui_font_descent(SLUIFont* f) {
    Backend* be = slui::shared_text_backend();
    if (!f || !be) return 0;
    apply_face(be, f);
    return be->font_descent();
}
double slui_font_line_height(SLUIFont* f) {
    Backend* be = slui::shared_text_backend();
    if (!f || !be) return 0;
    apply_face(be, f);
    return be->font_line_height() * f->line_height_mult;
}

/* ==========================================================================
 * Drawing
 * ========================================================================== */
void slui_font_draw(SLUIDrawContext* dc, SLUIFont* f, const char* utf8, int x,
                    int baseline, SLUIColor color) {
    if (!dc || !f || !utf8) return;
    Backend* be = slui::shared_text_backend();
    if (!be) return;

    /* Pad enough for the largest shadow/glow/outline. */
    int pad = 2;
    for (const auto& e : f->effects) {
        int need = static_cast<int>(std::ceil(
            std::fabs(e.dx) + std::fabs(e.dy) + e.blur * 3 + e.size + 2));
        pad = std::max(pad, need);
    }
    Mask m = build_mask(be, f, std::string(utf8), pad);
    int taps = quality_taps(f->quality);

    /* ---- back layer: drop shadows (under the glyph) ---- */
    for (const auto& e : f->effects) {
        if (e.type != SLUI_FX_DROP_SHADOW) continue;
        auto field = blur_mask(m, std::max(0.5, e.blur), taps);
        composite_field(dc, m, field, x, baseline, e.dx, e.dy, decode(e.color),
                        e.opacity, SLUI_BLEND_OVER);
    }
    /* ---- glow (soft additive halo, under/around the glyph) ---- */
    for (const auto& e : f->effects) {
        if (e.type != SLUI_FX_GLOW) continue;
        auto field = blur_mask(m, std::max(1.0, e.blur), taps + 1);
        for (auto& v : field) v = clampd(v * (1.0 + e.intensity), 0.0, 1.0);
        composite_field(dc, m, field, x, baseline, 0, 0, decode(e.color),
                        e.opacity, SLUI_BLEND_ADD);
    }
    /* ---- outline (dilate the mask by the stroke width) ---- */
    for (const auto& e : f->effects) {
        if (e.type != SLUI_FX_OUTLINE) continue;
        int r = std::max(1, static_cast<int>(e.size + 0.5));
        int angles = quality_angles(f->quality);
        std::vector<double> ring(static_cast<size_t>(m.w) * m.h, 0.0);
        for (int ai = 0; ai < angles; ++ai) {
            double a = ai * 2.0 * 3.14159265358979323846 / angles;
            int ox = static_cast<int>(std::cos(a) * r + 0.5);
            int oy = static_cast<int>(std::sin(a) * r + 0.5);
            for (int y = 0; y < m.h; ++y)
                for (int xx = 0; xx < m.w; ++xx) {
                    double v = m.at(xx - ox, y - oy) / 255.0;
                    double& d = ring[static_cast<size_t>(y) * m.w + xx];
                    if (v > d) d = v;
                }
        }
        composite_field(dc, m, ring, x, baseline, 0, 0, decode(e.color),
                        e.opacity, SLUI_BLEND_OVER);
    }

    /* ---- the glyph fill (flat, gradient, or relieved) ---- */
    bool have_gradient = false, have_relief = false;
    SLUIFontEffect grad{}, relief{};
    for (const auto& e : f->effects) {
        if (e.type == SLUI_FX_GRADIENT_FILL) { have_gradient = true; grad = e; }
        if (e.type == SLUI_FX_RELIEF) { have_relief = true; relief = e; }
    }
    Color fillc = decode(color);
    for (int y = 0; y < m.h; ++y) {
        for (int xx = 0; xx < m.w; ++xx) {
            double cov = m.a[static_cast<size_t>(y) * m.w + xx] / 255.0;
            if (cov <= 0.003) continue;
            Color c = fillc;
            if (have_gradient) {
                /* vertical gradient across the glyph band */
                double t = m.h > 1 ? (y - (m.oy - slui_font_ascent(f)))
                                         / std::max(1.0, slui_font_ascent(f) +
                                                             slui_font_descent(f))
                                   : 0.0;
                c = slui::lerp(decode(grad.color), decode(grad.color2),
                               clampd(t, 0.0, 1.0));
            }
            if (have_relief) {
                /* normal from the coverage gradient; light from top-left */
                double hl = m.at(xx - 1, y) / 255.0;
                double hr = m.at(xx + 1, y) / 255.0;
                double hu = m.at(xx, y - 1) / 255.0;
                double hd = m.at(xx, y + 1) / 255.0;
                double nx = (hl - hr) * relief.size;
                double ny = (hu - hd) * relief.size;
                double nz = 1.0;
                double L = std::sqrt(nx * nx + ny * ny + nz * nz);
                /* light direction top-left (engraved flips it) */
                double lx = (relief.relief == SLUI_FONT_ENGRAVED) ? 0.5 : -0.5;
                double ly = (relief.relief == SLUI_FONT_ENGRAVED) ? 0.5 : -0.5;
                double lz = 0.8;
                double ndotl = (nx * lx + ny * ly + nz * lz) / (L * 1.07);
                double shade = clampd(0.6 + relief.intensity * ndotl, 0.2, 1.6);
                c = Color{static_cast<uint8_t>(clampd(c.r * shade, 0, 255)),
                          static_cast<uint8_t>(clampd(c.g * shade, 0, 255)),
                          static_cast<uint8_t>(clampd(c.b * shade, 0, 255)), 255};
            }
            int px = x + (xx - m.ox);
            int py = baseline + (y - m.oy);
            slui_draw_blend_pixel(dc, px, py, slui_rgb(c.r, c.g, c.b), cov);
        }
    }

    /* ---- inner shadow (darken the inside toward one edge) ---- */
    for (const auto& e : f->effects) {
        if (e.type != SLUI_FX_INNER_SHADOW) continue;
        auto field = blur_mask(m, std::max(0.5, e.blur), taps);
        for (int y = 0; y < m.h; ++y)
            for (int xx = 0; xx < m.w; ++xx) {
                double cov = m.a[static_cast<size_t>(y) * m.w + xx] / 255.0;
                if (cov <= 0.003) continue;
                /* shadow = coverage minus the offset-blurred coverage */
                int sx = xx - static_cast<int>(e.dx + 0.5);
                int sy = y - static_cast<int>(e.dy + 0.5);
                double inner = (sx >= 0 && sy >= 0 && sx < m.w && sy < m.h)
                                   ? field[static_cast<size_t>(sy) * m.w + sx]
                                   : 0.0;
                double dark = clampd(cov - inner, 0.0, 1.0) * e.opacity;
                if (dark <= 0.003) continue;
                int px = x + (xx - m.ox);
                int py = baseline + (y - m.oy);
                Color sc = decode(e.color);
                slui_draw_blend_pixel(dc, px, py, slui_rgb(sc.r, sc.g, sc.b),
                                      dark);
            }
    }

    /* ---- directional light sheen on the glyph face (on top) ---- */
    for (const auto& e : f->effects) {
        if (e.type != SLUI_FX_LIGHT) continue;
        double lx = e.dx, ly = e.dy;
        double Ln = std::sqrt(lx * lx + ly * ly);
        if (Ln < 1e-6) { lx = -1; ly = -1; Ln = std::sqrt(2.0); }
        lx /= Ln;
        ly /= Ln;
        for (int y = 0; y < m.h; ++y)
            for (int xx = 0; xx < m.w; ++xx) {
                double cov = m.a[static_cast<size_t>(y) * m.w + xx] / 255.0;
                if (cov <= 0.02) continue;
                double hl = m.at(xx - 1, y) / 255.0;
                double hr = m.at(xx + 1, y) / 255.0;
                double hu = m.at(xx, y - 1) / 255.0;
                double hd = m.at(xx, y + 1) / 255.0;
                double gx = hr - hl, gy = hd - hu;
                double sheen = clampd(-(gx * lx + gy * ly), 0.0, 1.0);
                sheen = std::pow(sheen, 1.5) * e.intensity * cov;
                if (sheen <= 0.01) continue;
                int px = x + (xx - m.ox);
                int py = baseline + (y - m.oy);
                Color lc = decode(e.color);
                SLUIBlendMode prev = slui_draw_get_blend(dc);
                slui_draw_set_blend(dc, SLUI_BLEND_ADD);
                slui_draw_blend_pixel(dc, px, py, slui_rgb(lc.r, lc.g, lc.b),
                                      clampd(sheen, 0.0, 1.0));
                slui_draw_set_blend(dc, prev);
            }
    }
}

void slui_font_draw_in(SLUIDrawContext* dc, SLUIFont* f, const char* utf8,
                       SLUIRect area, int halign, SLUIColor color) {
    if (!dc || !f || !utf8) return;
    double tw = slui_font_measure(f, utf8);
    double asc = slui_font_ascent(f), desc = slui_font_descent(f);
    int x = area.x;
    if (halign == 1) x = area.x + static_cast<int>((area.w - tw) / 2.0);
    else if (halign == 2) x = area.x + static_cast<int>(area.w - tw);
    int baseline =
        area.y + static_cast<int>((area.h - (asc + desc)) / 2.0 + asc + 0.5);
    slui_font_draw(dc, f, utf8, x, baseline, color);
}

int slui_font_emit_into_scene(SLUIFont* f, void* light_scene, int x,
                              int baseline, const char* utf8) {
    if (!f || !light_scene) return SLUI_ERR_INVALID;
    auto* scene = static_cast<SLUILightScene*>(light_scene);
    const SLUIFontEffect* em = nullptr;
    for (const auto& e : f->effects)
        if (e.type == SLUI_FX_EMITTER) { em = &e; break; }
    if (!em) return 1; /* no emitter effect */
    double tw = slui_font_measure(f, utf8);
    double asc = slui_font_ascent(f);
    double cx = x + tw / 2.0;
    double cy = baseline - asc / 2.0;
    SLUILight light = slui_light_point(cx, cy, em->size > 0 ? em->size : tw,
                                       em->intensity, em->color);
    if (em->reserved_anchor >= 0) {
        /* bind as a SOURCE reserving the anchor */
        return slui_light_scene_add_source(scene, em->reserved_anchor, light);
    }
    /* a pure emitter: reserves nothing */
    return slui_light_scene_add_emitter(scene, light);
}

} /* extern "C" */

/* ==========================================================================
 * Widget integration (needs the C++ widget type).
 * ========================================================================== */
#include "slui_widget.hpp"

extern "C" void slui_widget_set_font(SLUIWidget* widget, SLUIFont* font) {
    if (!widget) return;
    auto* w = reinterpret_cast<slui::Widget*>(widget);
    w->set_styled_font(font);
}

namespace slui {

/* Render a styled run into a scratch draw context, then composite it into the
 * window canvas. Falls back to plain draw_text when no font is set. */
void draw_text_styled(PaintContext& ctx, void* font, const std::string& utf8,
                      int x, int baseline, const Color& color) {
    SLUIFont* f = static_cast<SLUIFont*>(font);
    if (!f || f->effects.empty()) {
        draw_text(ctx, utf8, x, baseline, color);
        return;
    }
    Backend* be = slui::shared_text_backend();
    if (!be || !ctx.canvas) {
        draw_text(ctx, utf8, x, baseline, color);
        return;
    }
    /* Size a scratch context around the run + effect padding. */
    double tw = slui_font_measure(f, utf8.c_str());
    double asc = slui_font_ascent(f), desc = slui_font_descent(f);
    int pad = 4;
    for (const auto& e : f->effects) {
        int need = static_cast<int>(std::ceil(std::fabs(e.dx) + std::fabs(e.dy) +
                                              e.blur * 3 + e.size + 2));
        pad = std::max(pad, need);
    }
    int cw = static_cast<int>(tw + 0.5) + pad * 2;
    int ch = static_cast<int>(asc + desc + 0.5) + pad * 2;
    if (cw < 1 || ch < 1) return;

    SLUIDrawContext* dc = slui_draw_create(cw, ch, SLUI_BUFFER_SINGLE);
    if (!dc) {
        draw_text(ctx, utf8, x, baseline, color);
        return;
    }
    /* transparent scratch */
    slui_draw_clear(dc, 0x00000000u);
    slui_font_draw(dc, f, utf8.c_str(), pad, pad + static_cast<int>(asc + 0.5),
                   color.to_abi());

    /* Composite the scratch (which is opaque where it cleared black) by reading
     * pixels and blending only the non-background ones. Because clear made it
     * black-opaque, we instead track coverage by comparing to the cleared
     * colour would be lossy -- so draw into a transparent-friendly path: the
     * Canvas is opaque, so we blend every pixel that differs from pure black
     * weighted by its luminance distance. For crisp text we simply OVER-blend
     * the scratch's RGB using per-pixel alpha derived from how far it is from
     * the transparent clear. */
    int dstx = x - pad;
    int dsty = baseline - (pad + static_cast<int>(asc + 0.5));
    for (int yy = 0; yy < ch; ++yy) {
        for (int xx = 0; xx < cw; ++xx) {
            SLUIColor word = slui_draw_get_pixel(dc, xx, yy);
            Color px = Color::from_abi(word);
            /* the scratch was cleared to transparent-black; any drawn pixel has
             * non-zero rgb. Use max channel as coverage so AA edges carry. */
            double cov = std::max({px.r, px.g, px.b}) / 255.0;
            if (cov <= 0.004) continue;
            ctx.canvas->blend(dstx + xx, dsty + yy, Color{px.r, px.g, px.b, 255},
                              cov);
        }
    }
    slui_draw_destroy(dc);
}

} // namespace slui
