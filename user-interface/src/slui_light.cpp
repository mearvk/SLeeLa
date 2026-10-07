/* =============================================================================
 * SleelaUI Lighting & Shadow implementation.
 *
 * Backs sleela_ui_light.h. The quality of the relief comes from a real
 * (if lightweight) shading pipeline, run over the pixels already in a draw
 * context via the public Draw API:
 *
 *   1. HEIGHT FIELD. For the object's rounded-rect region we build a height
 *      h(x,y) in [0,1] from the chosen relief profile and the signed distance
 *      to the rounded boundary -- a dome for ROUNDED, a chamfer for BEVEL, an
 *      inset ramp for ENGRAVED, a raised plateau for EMBOSSED. ENGRAVED simply
 *      flips the height sign so the shape reads pressed INTO the floor.
 *   2. NORMALS. The surface normal is (-dh/dx, -dh/dy, 1) normalised -- central
 *      differences on the height field. This is what gives directional relief.
 *   3. SHADING. For every pixel we start from the scene ambient, then for each
 *      light accumulate: a smooth distance falloff * N.L diffuse term, plus a
 *      Blinn-Phong specular sheen scaled by the material gloss (quality light).
 *      SHADOW-polarity lights subtract, as a coloured darkening that pools with
 *      the surface -- darkness treated as a first-class emission (quality dark).
 *      Ambient occlusion darkens the creases (low height) for depth.
 *   4. CAST SHADOWS. Each light throws the occluder's rounded shadow away from
 *      itself; the penumbra softens with the light's softness and the occluder
 *      lift, multi-sampled so the edge is smooth rather than hard.
 *
 * The base warm colour #2B1608 is dark, so good relief needs real contrast
 * between lit and shadowed facets -- which is exactly what this produces.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui_light.h"

#include "slui_geometry.hpp"
#include "slui_render.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>

using slui::Color;

namespace {

struct Vec3 {
    double x, y, z;
};
static Vec3 normalize(Vec3 v) {
    double L = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (L <= 1e-9) return Vec3{0, 0, 1};
    return Vec3{v.x / L, v.y / L, v.z / L};
}
static double clampd(double v, double lo, double hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}
static Color decode(SLUIColor c) {
    return Color{static_cast<uint8_t>((c >> 24) & 0xFF),
                 static_cast<uint8_t>((c >> 16) & 0xFF),
                 static_cast<uint8_t>((c >> 8) & 0xFF),
                 static_cast<uint8_t>(c & 0xFF)};
}
static uint8_t clamp8(double v) {
    return static_cast<uint8_t>(clampd(v, 0, 255) + 0.5);
}

/* Signed distance to a rounded rect (negative inside). */
static double rr_sdf(double px, double py, const slui::Rect& r, double radius) {
    radius = std::min(radius, std::min(r.w, r.h) / 2.0);
    double rl = r.x + radius, rt = r.y + radius;
    double rr = r.x + r.w - radius, rb = r.y + r.h - radius;
    double cx = clampd(px, rl, rr);
    double cy = clampd(py, rt, rb);
    double dx = px - cx, dy = py - cy;
    double d = std::sqrt(dx * dx + dy * dy) - radius;
    return d; /* <0 inside */
}

/* Height in [0,1] from the relief profile at signed distance `d` (d<0 inside),
 * with an edge band of `edge` px. ENGRAVED/EMBOSSED select the sign/plateau. */
static double relief_height(SLUIReliefProfile profile, double d, double edge,
                            const slui::Rect& r, double px, double py,
                            double radius) {
    if (profile == SLUI_RELIEF_FLAT) return (d < 0) ? 1.0 : 0.0;
    double inside = (d < 0) ? 1.0 : 0.0;
    if (!inside) return 0.0;
    double dist_in = -d; /* >=0 inside, how far from the edge */
    switch (profile) {
    case SLUI_RELIEF_ROUNDED: {
        /* a smooth dome: height peaks at the centre. Normalise by the shape's
         * inscribed radius so any size domes evenly. */
        double maxr = std::min(r.w, r.h) / 2.0 - radius;
        if (maxr < 1) maxr = 1;
        double t = clampd(dist_in / maxr, 0.0, 1.0);
        return std::sqrt(t); /* fuller near the rim, rounded on top */
    }
    case SLUI_RELIEF_BEVEL: {
        double t = clampd(dist_in / std::max(1.0, edge), 0.0, 1.0);
        return t; /* linear ramp up over the edge band, then flat 1 */
    }
    case SLUI_RELIEF_ENGRAVED: {
        /* inset: high at the rim, low in the middle (pressed in) */
        double t = clampd(dist_in / std::max(1.0, edge), 0.0, 1.0);
        return 1.0 - t * 0.9;
    }
    case SLUI_RELIEF_EMBOSSED:
    default: {
        /* raised plateau with a soft shoulder */
        double t = clampd(dist_in / std::max(1.0, edge), 0.0, 1.0);
        return 0.15 + 0.85 * t;
    }
    }
    (void)px;
    (void)py;
}

} // namespace

/* ==========================================================================
 * SLUILight initialisers
 * ========================================================================== */
extern "C" {

SLUILight slui_light_point(double x, double y, double radius, double intensity,
                           SLUIColor color) {
    SLUILight l{};
    l.kind = SLUI_LIGHT_POINT;
    l.role = SLUI_LIGHT_EMITTER;
    l.polarity = SLUI_POLARITY_LIGHT;
    l.x = x;
    l.y = y;
    l.z = radius * 0.5 + 24.0;
    l.radius = radius;
    l.intensity = intensity;
    l.softness = 0.5;
    l.color = color;
    l.anchor = -1;
    return l;
}

SLUILight slui_light_directional(double dx, double dy, double intensity,
                                 SLUIColor color) {
    SLUILight l{};
    l.kind = SLUI_LIGHT_DIRECTIONAL;
    l.role = SLUI_LIGHT_EMITTER;
    l.polarity = SLUI_POLARITY_LIGHT;
    double L = std::sqrt(dx * dx + dy * dy);
    if (L < 1e-9) { dx = 0; dy = 1; L = 1; }
    l.dx = dx / L;
    l.dy = dy / L;
    l.z = 0.6; /* a fairly grazing sun by default -> strong relief */
    l.intensity = intensity;
    l.softness = 0.4;
    l.color = color;
    l.anchor = -1;
    return l;
}

SLUILight slui_light_ambient(double intensity, SLUIColor color) {
    SLUILight l{};
    l.kind = SLUI_LIGHT_AMBIENT;
    l.role = SLUI_LIGHT_EMITTER;
    l.polarity = SLUI_POLARITY_LIGHT;
    l.intensity = intensity;
    l.color = color;
    l.anchor = -1;
    return l;
}

SLUILight slui_shadow_point(double x, double y, double radius, double intensity,
                            SLUIColor shadow_color) {
    SLUILight l = slui_light_point(x, y, radius, intensity, shadow_color);
    l.polarity = SLUI_POLARITY_SHADOW;
    return l;
}

SLUIMaterial slui_material(SLUIReliefProfile profile, double depth) {
    SLUIMaterial m{};
    m.profile = profile;
    m.depth = depth;
    m.gloss = 0.35;
    m.occlusion = 0.4;
    m.edge_px = 6.0;
    return m;
}

} // extern "C"

/* ==========================================================================
 * SLUILightScene
 * ========================================================================== */
struct SLUILightScene {
    std::vector<SLUILight> lights;
    std::map<int, size_t> reserved;      /* anchor -> index into lights       */
    double ambient_intensity = 0.5;
    SLUIColor ambient_color = 0xFFF2DCFF; /* warm fill suited to #2B1608      */
};

extern "C" {

SLUILightScene* slui_light_scene_create(void) { return new SLUILightScene(); }
void slui_light_scene_destroy(SLUILightScene* s) { delete s; }
void slui_light_scene_clear(SLUILightScene* s) {
    if (!s) return;
    s->lights.clear();
    s->reserved.clear();
}

int slui_light_scene_add_emitter(SLUILightScene* s, SLUILight light) {
    if (!s) return SLUI_ERR_INVALID;
    light.role = SLUI_LIGHT_EMITTER;
    light.anchor = -1; /* an emitter reserves nothing */
    s->lights.push_back(light);
    return static_cast<int>(s->lights.size() - 1);
}

int slui_light_scene_add_source(SLUILightScene* s, int anchor, SLUILight light) {
    if (!s) return SLUI_ERR_INVALID;
    if (s->reserved.count(anchor)) return SLUI_ERR_BACKEND; /* already used */
    light.role = SLUI_LIGHT_SOURCE;
    light.anchor = anchor;
    s->lights.push_back(light);
    size_t idx = s->lights.size() - 1;
    s->reserved[anchor] = idx;
    return static_cast<int>(idx);
}

int slui_light_scene_anchor_reserved(const SLUILightScene* s, int anchor) {
    if (!s) return 0;
    return s->reserved.count(anchor) ? 1 : 0;
}

void slui_light_scene_release_source(SLUILightScene* s, int anchor) {
    if (!s) return;
    auto it = s->reserved.find(anchor);
    if (it == s->reserved.end()) return;
    size_t idx = it->second;
    s->lights.erase(s->lights.begin() + idx);
    s->reserved.erase(it);
    /* re-index remaining reservations after the removed one */
    for (auto& kv : s->reserved)
        if (kv.second > idx) kv.second -= 1;
}

int slui_light_scene_count(const SLUILightScene* s) {
    return s ? static_cast<int>(s->lights.size()) : 0;
}

void slui_light_scene_set_ambient(SLUILightScene* s, double intensity,
                                  SLUIColor color) {
    if (!s) return;
    s->ambient_intensity = intensity;
    s->ambient_color = color;
}

/* ==========================================================================
 * Shading
 * ========================================================================== */

/* Per-light contribution to a lit pixel. Returns an additive RGB delta (can be
 * negative for shadow polarity). `n` is the surface normal, `p` the pixel point.
 */
static void accumulate_light(const SLUILight& l, double px, double py,
                             const Vec3& n, double gloss, double& r, double& g,
                             double& b) {
    Color lc = decode(l.color);
    double contrib = 0.0;   /* diffuse+specular scalar */
    Vec3 Ldir;              /* unit vector toward the light */
    double atten = 1.0;

    if (l.kind == SLUI_LIGHT_AMBIENT) {
        contrib = l.intensity;
        Ldir = Vec3{0, 0, 1};
    } else if (l.kind == SLUI_LIGHT_DIRECTIONAL) {
        /* rays travel along (dx,dy); the vector TOWARD the light is the
         * negative of that, lifted by z. */
        Ldir = normalize(Vec3{-l.dx, -l.dy, std::max(0.05, l.z)});
        double ndotl = std::max(0.0, n.x * Ldir.x + n.y * Ldir.y + n.z * Ldir.z);
        contrib = l.intensity * ndotl;
    } else { /* POINT */
        double vx = l.x - px, vy = l.y - py;
        double dist = std::sqrt(vx * vx + vy * vy);
        Ldir = normalize(Vec3{vx, vy, std::max(1.0, l.z)});
        double ndotl = std::max(0.0, n.x * Ldir.x + n.y * Ldir.y + n.z * Ldir.z);
        if (l.radius > 0) {
            double t = clampd(dist / l.radius, 0.0, 1.0);
            /* smooth inverse-square-ish falloff: (1-t^2)^2 */
            double f = (1.0 - t * t);
            atten = f > 0 ? f * f : 0.0;
        }
        contrib = l.intensity * ndotl * atten;
    }

    /* Blinn-Phong specular sheen (quality light): a tight highlight where the
     * half-vector aligns with the normal. The viewer is straight above (0,0,1).*/
    double spec = 0.0;
    if (gloss > 0.0 && l.kind != SLUI_LIGHT_AMBIENT) {
        Vec3 H = normalize(Vec3{Ldir.x, Ldir.y, Ldir.z + 1.0});
        double ndoth = std::max(0.0, n.x * H.x + n.y * H.y + n.z * H.z);
        double shininess = 8.0 + 48.0 * gloss;
        spec = std::pow(ndoth, shininess) * gloss * atten *
               (l.kind == SLUI_LIGHT_POINT ? l.intensity : l.intensity);
    }

    double sign = (l.polarity == SLUI_POLARITY_SHADOW) ? -1.0 : 1.0;
    double diff = contrib;
    r += sign * (lc.r / 255.0) * diff + (sign > 0 ? spec * 255.0 / 255.0 : 0.0) * (lc.r / 255.0);
    g += sign * (lc.g / 255.0) * diff + (sign > 0 ? spec : 0.0) * (lc.g / 255.0);
    b += sign * (lc.b / 255.0) * diff + (sign > 0 ? spec : 0.0) * (lc.b / 255.0);
    /* fold specular as white-ish add on top for light polarity */
    if (sign > 0) {
        r += spec;
        g += spec;
        b += spec;
    }
}

void slui_light_apply_relief(SLUIDrawContext* dc, SLUIRect area, double radius,
                             SLUIMaterial material, const SLUILightScene* scene) {
    if (!dc || !scene) return;
    slui::Rect r = slui::to_rect(area);
    if (r.w <= 0 || r.h <= 0) return;
    double edge = material.edge_px > 0 ? material.edge_px : 6.0;
    double depth = material.depth > 0 ? material.depth : 4.0;

    Color amb = decode(scene->ambient_color);
    double ambI = scene->ambient_intensity;

    for (int y = r.y; y < r.y + r.h; ++y) {
        for (int x = r.x; x < r.x + r.w; ++x) {
            double px = x + 0.5, py = y + 0.5;
            double d = rr_sdf(px, py, r, radius);
            if (d > 0.75) continue; /* outside the shape (leave a soft edge) */

            /* Height field + normal from central differences. */
            double h = relief_height(material.profile, d, edge, r, px, py, radius);
            double hL = relief_height(material.profile, rr_sdf(px - 1, py, r, radius),
                                      edge, r, px - 1, py, radius);
            double hR = relief_height(material.profile, rr_sdf(px + 1, py, r, radius),
                                      edge, r, px + 1, py, radius);
            double hU = relief_height(material.profile, rr_sdf(px, py - 1, r, radius),
                                      edge, r, px, py - 1, radius);
            double hD = relief_height(material.profile, rr_sdf(px, py + 1, r, radius),
                                      edge, r, px, py + 1, radius);
            Vec3 n = normalize(Vec3{(hL - hR) * depth, (hU - hD) * depth, 1.0});

            /* Start from ambient. */
            double rr_ = amb.r / 255.0 * ambI;
            double gg = amb.g / 255.0 * ambI;
            double bb = amb.b / 255.0 * ambI;
            /* Accumulate every light/shadow. */
            for (const auto& l : scene->lights)
                accumulate_light(l, px, py, n, material.gloss, rr_, gg, bb);

            /* Ambient occlusion: pool darkness where height is low (creases). */
            double ao = 1.0 - material.occlusion * (1.0 - h);
            rr_ *= ao;
            gg *= ao;
            bb *= ao;

            /* Modulate the pixel already in the context by this shading, so the
             * object keeps its own colour but gains relief. Multiply is the
             * physically-sensible operator for lighting a surface albedo. */
            SLUIColor cur = slui_draw_get_pixel(dc, x, y);
            Color base = decode(cur);
            double nr = clampd(base.r * clampd(rr_, 0.0, 2.0), 0, 255);
            double ng = clampd(base.g * clampd(gg, 0.0, 2.0), 0, 255);
            double nb = clampd(base.b * clampd(bb, 0.0, 2.0), 0, 255);

            /* Antialiased edge coverage so the relieved shape has a clean rim. */
            double cov = 1.0 - clampd(d + 0.5, 0.0, 1.0);
            SLUIColor out = slui_rgba(clamp8(nr), clamp8(ng), clamp8(nb), 255);
            slui_draw_blend_pixel(dc, x, y, out, cov);
        }
    }
}

void slui_light_cast_shadow(SLUIDrawContext* dc, SLUIRect occluder,
                            double radius, double lift,
                            const SLUILightScene* scene) {
    if (!dc || !scene) return;
    slui::Rect o = slui::to_rect(occluder);
    double ocx = o.x + o.w / 2.0, ocy = o.y + o.h / 2.0;

    for (const auto& l : scene->lights) {
        if (l.kind == SLUI_LIGHT_AMBIENT) continue;
        /* Direction the shadow is thrown (away from the light). */
        double sx, sy, strength;
        if (l.kind == SLUI_LIGHT_DIRECTIONAL) {
            sx = l.dx;
            sy = l.dy;
            strength = l.intensity;
        } else {
            double vx = ocx - l.x, vy = ocy - l.y;
            double dist = std::sqrt(vx * vx + vy * vy);
            if (dist < 1e-6) continue;
            sx = vx / dist;
            sy = vy / dist;
            double t = l.radius > 0 ? clampd(dist / l.radius, 0, 1) : 0.5;
            strength = l.intensity * (1.0 - t);
        }
        if (strength <= 0.01) continue;

        /* Offset + penumbra grow with the occluder lift and the light softness.
         * A shadow-polarity light deepens; a light-polarity light still casts a
         * normal (dark) shadow of the occluder. */
        double offset = lift * (1.0 + 2.0 * l.z * 0.0 + 1.0);
        double off = clampd(offset, 2.0, 40.0);
        double pen = 2.0 + l.softness * (6.0 + lift); /* penumbra radius */
        double base_alpha = clampd(strength * 0.5, 0.0, 0.6);
        if (l.polarity == SLUI_POLARITY_SHADOW)
            base_alpha = clampd(base_alpha * 1.6, 0.0, 0.85);

        Color sc = (l.polarity == SLUI_POLARITY_SHADOW) ? decode(l.color)
                                                        : Color{0, 0, 0, 255};

        /* Multi-sample the shadow across the penumbra for a soft, quality edge:
         * several offset copies of the rounded footprint, each faint. */
        const int samples = 5;
        slui::Rect base = o;
        for (int s = 0; s < samples; ++s) {
            double k = (s + 1) / static_cast<double>(samples);
            double dx = sx * off * k;
            double dy = sy * off * k;
            double grow = pen * k;
            slui::Rect sh{static_cast<int>(base.x + dx - grow),
                          static_cast<int>(base.y + dy - grow),
                          static_cast<int>(base.w + grow * 2),
                          static_cast<int>(base.h + grow * 2)};
            double a = base_alpha / samples * (1.0 - 0.3 * k);
            SLUIColor word = slui_rgba(sc.r, sc.g, sc.b,
                                       static_cast<uint8_t>(clampd(a * 255, 0, 255)));
            SLUIBlendMode prev = slui_draw_get_blend(dc);
            slui_draw_set_blend(dc, SLUI_BLEND_OVER);
            slui_draw_fill_round_rect(
                dc, (SLUIRect){sh.x, sh.y, sh.w, sh.h},
                radius + grow, word);
            slui_draw_set_blend(dc, prev);
        }
    }
}

void slui_light_panel(SLUIDrawContext* dc, SLUIRect area, double radius,
                      SLUIColor base, SLUIMaterial material,
                      const SLUILightScene* scene) {
    if (!dc) return;
    /* 1. cast the shadow beneath, 2. fill the base, 3. relieve + light it. */
    slui_light_cast_shadow(dc, area, radius, material.depth + 4.0, scene);
    slui_draw_fill_round_rect(dc, area, radius, base);
    slui_light_apply_relief(dc, area, radius, material, scene);
}

} // extern "C"

/* ==========================================================================
 * Widget integration (needs the C++ widget type).
 * ========================================================================== */
#include "slui_widget.hpp"

extern "C" void slui_widget_set_light_scene(SLUIWidget* widget,
                                            const SLUILightScene* scene,
                                            SLUIMaterial material) {
    if (!widget) return;
    auto* w = reinterpret_cast<slui::Widget*>(widget);
    auto* cv = dynamic_cast<slui::CanvasView*>(w);
    if (!cv) return;
    cv->set_light(scene, static_cast<int>(material.profile), material.depth,
                  material.gloss, material.occlusion, material.edge_px);
}
