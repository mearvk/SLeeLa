/* =============================================================================
 * SleelaUI Mood / Wash / Calculus-8 / millimetre light placement.
 *
 * Backs sleela_ui_mood.h.
 *
 * CALCULUS-8. The 100 differentiable inputs x[0..99] are partitioned into eight
 * groups. Each of the eight stages forms a weighted aggregate a_s = sum(w_i*x_i)
 * over its group and applies a smooth, analytically-differentiable basis phi_s:
 *
 *     stage 0  identity      phi(a) = a
 *     stage 1  quadratic     phi(a) = a^2
 *     stage 2  smoothstep    phi(a) = 3a^2 - 2a^3
 *     stage 3  sine          phi(a) = (sin(2pi a)+1)/2
 *     stage 4  cosine        phi(a) = (cos(2pi a)+1)/2
 *     stage 5  logistic      phi(a) = 1/(1+e^-k(a-.5))
 *     stage 6  gaussian      phi(a) = e^-((a-.5)^2 / 2s^2)
 *     stage 7  tanh          phi(a) = (tanh(k(a-.5))+1)/2
 *
 * The mood scalar is M = logistic( sum_s beta_s * phi_s(a_s) ). Every function
 * is C-infinity, so dM/dx_j exists and is computed by the chain rule (no finite
 * differences). Nudging any input moves the mood smoothly; eval returns the
 * slope dM/dx[wrt] so an adjuster can walk the gradient into a mood.
 *
 * WASH. A multi-stop colour field sampled with smoothstep between stops -> a
 * soft "excellent wash" rather than hard bands.
 *
 * TANOR. A portrait glow for lightless bulbs; its refresh + a little-left bias
 * freshen the wash sample point over time.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "sleela_ui_mood.h"

#include "slui_geometry.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <vector>

using slui::Color;

namespace {
constexpr double kPi = 3.14159265358979323846;

Color decode(SLUIColor c) {
    return Color{static_cast<uint8_t>((c >> 24) & 0xFF),
                 static_cast<uint8_t>((c >> 16) & 0xFF),
                 static_cast<uint8_t>((c >> 8) & 0xFF),
                 static_cast<uint8_t>(c & 0xFF)};
}
double clampd(double v, double lo, double hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}
double logistic(double z) { return 1.0 / (1.0 + std::exp(-z)); }
} // namespace

/* ==========================================================================
 * Millimetre placement
 * ========================================================================== */
extern "C" {

SLUIMm slui_mm(double right_mm, double down_mm, double height_mm) {
    SLUIMm m;
    m.right_mm = right_mm;
    m.down_mm = down_mm;
    m.height_mm = height_mm;
    return m;
}
SLUIMm slui_mm_left(double mm, double height_mm) {
    return slui_mm(-mm, 0, height_mm);
}
SLUIMm slui_mm_right(double mm, double height_mm) {
    return slui_mm(mm, 0, height_mm);
}
SLUIMm slui_mm_up(double mm, double height_mm) {
    return slui_mm(0, -mm, height_mm);
}
SLUIMm slui_mm_down(double mm, double height_mm) {
    return slui_mm(0, mm, height_mm);
}

double slui_mm_to_px(double mm, double dpi) {
    if (dpi <= 0) dpi = 96.0;
    return mm * dpi / 25.4; /* 25.4 mm per inch */
}

SLUILight slui_light_place_mm(SLUILight* light, double ref_x, double ref_y,
                              SLUIMm mm, double dpi) {
    SLUILight l = light ? *light : SLUILight{};
    double px = slui_mm_to_px(mm.right_mm, dpi);
    double py = slui_mm_to_px(mm.down_mm, dpi);
    double pz = slui_mm_to_px(mm.height_mm, dpi);
    /* The ground point directly under the light is the font origin shifted by
     * the horizontal/vertical mm; the light shines down from height pz. */
    l.x = ref_x + px;
    l.y = ref_y + py;
    l.z = pz > 0 ? pz : l.z;
    if (light) *light = l;
    return l;
}

/* ==========================================================================
 * Excellent Wash
 * ========================================================================== */
SLUIWash slui_wash_begin(void) {
    SLUIWash w;
    std::memset(&w, 0, sizeof(w));
    w.softness = 0.5;
    return w;
}
SLUIWash slui_wash_stop(SLUIWash w, double position, SLUIColor color) {
    if (w.count < SLUI_WASH_MAX_STOPS) {
        w.pos[w.count] = clampd(position, 0.0, 1.0);
        w.color[w.count] = color;
        ++w.count;
    }
    return w;
}
SLUIWash slui_wash_linear(SLUIColor a, SLUIColor b) {
    SLUIWash w = slui_wash_begin();
    w = slui_wash_stop(w, 0.0, a);
    w = slui_wash_stop(w, 1.0, b);
    return w;
}
SLUIWash slui_wash_triad(SLUIColor a, SLUIColor b, SLUIColor c) {
    SLUIWash w = slui_wash_begin();
    w = slui_wash_stop(w, 0.0, a);
    w = slui_wash_stop(w, 0.5, b);
    w = slui_wash_stop(w, 1.0, c);
    return w;
}
SLUIColor slui_wash_sample(const SLUIWash* wash, double t) {
    if (!wash || wash->count == 0) return 0x000000FF;
    if (wash->count == 1) return wash->color[0];
    t = clampd(t, 0.0, 1.0);
    /* copy + sort stops by position */
    int n = wash->count;
    int idx[SLUI_WASH_MAX_STOPS];
    for (int i = 0; i < n; ++i) idx[i] = i;
    std::sort(idx, idx + n,
              [&](int a, int b) { return wash->pos[a] < wash->pos[b]; });
    if (t <= wash->pos[idx[0]]) return wash->color[idx[0]];
    if (t >= wash->pos[idx[n - 1]]) return wash->color[idx[n - 1]];
    for (int i = 1; i < n; ++i) {
        double p0 = wash->pos[idx[i - 1]], p1 = wash->pos[idx[i]];
        if (t <= p1) {
            double u = (p1 > p0) ? (t - p0) / (p1 - p0) : 0.0;
            /* smoothstep for a soft wash, extra-softened by `softness` */
            double s = u * u * (3.0 - 2.0 * u);
            s = u + (s - u) * clampd(wash->softness, 0.0, 1.0);
            Color a = decode(wash->color[idx[i - 1]]);
            Color b = decode(wash->color[idx[i]]);
            return slui::lerp(a, b, s).to_abi();
        }
    }
    return wash->color[idx[n - 1]];
}

/* ==========================================================================
 * Natural Tanor
 * ========================================================================== */
SLUITanor slui_tanor_default(void) {
    SLUITanor t;
    t.warmth = 0.72;      /* warm */
    t.diffusion = 0.8;    /* soft, portrait */
    t.refresh_hz = 0.5;   /* a gentle freshen twice a second */
    t.left_bias = 0.2;    /* a little left */
    t.depth = 0.6;
    t.tone = slui_rgb(0xFF, 0xE6, 0xC6); /* warm portrait tone */
    return t;
}

} // extern "C"

/* ==========================================================================
 * Calculus-8
 * ========================================================================== */
struct SLUICalculus8 {
    double x[SLUI_CALC8_INPUTS];
    /* per-stage group weights and the stage mixing weights beta. Fixed, chosen
     * for a balanced "careful" feel; deterministic so moods are reproducible. */
    double beta[SLUI_CALC8_STAGES];
    SLUICalculus8() {
        for (int i = 0; i < SLUI_CALC8_INPUTS; ++i) x[i] = 0.5;
        /* stages weighted so no single basis dominates the mood */
        const double b[SLUI_CALC8_STAGES] = {0.9, 0.7, 1.1, 0.8,
                                             0.8, 1.2, 1.0, 1.0};
        for (int s = 0; s < SLUI_CALC8_STAGES; ++s) beta[s] = b[s];
    }
    /* input i belongs to stage i%8; its weight within the stage is a smooth
     * 1/(1+rank) taper so early inputs in a group matter a little more. */
    static int stage_of(int i) { return i % SLUI_CALC8_STAGES; }
    static double in_weight(int i) {
        int rank = i / SLUI_CALC8_STAGES; /* 0..12 */
        return 1.0 / (1.0 + 0.25 * rank);
    }
};

namespace {
/* basis value and derivative wrt its aggregate a, for stage s. */
void basis(int s, double a, double& val, double& dval) {
    switch (s) {
    case 0: /* identity */
        val = a;
        dval = 1.0;
        break;
    case 1: /* quadratic */
        val = a * a;
        dval = 2.0 * a;
        break;
    case 2: /* smoothstep */
        val = a * a * (3.0 - 2.0 * a);
        dval = 6.0 * a - 6.0 * a * a;
        break;
    case 3: /* sine, mapped to [0,1] */
        val = (std::sin(2.0 * kPi * a) + 1.0) * 0.5;
        dval = kPi * std::cos(2.0 * kPi * a);
        break;
    case 4: /* cosine, mapped to [0,1] */
        val = (std::cos(2.0 * kPi * a) + 1.0) * 0.5;
        dval = -kPi * std::sin(2.0 * kPi * a);
        break;
    case 5: { /* logistic around 0.5, steepness k */
        double k = 6.0;
        double z = k * (a - 0.5);
        double L = logistic(z);
        val = L;
        dval = k * L * (1.0 - L);
        break;
    }
    case 6: { /* gaussian around 0.5 */
        double s2 = 0.12; /* variance-ish */
        double d = a - 0.5;
        double g = std::exp(-(d * d) / (2.0 * s2));
        val = g;
        dval = g * (-d / s2);
        break;
    }
    case 7: { /* tanh around 0.5 */
        double k = 5.0;
        double th = std::tanh(k * (a - 0.5));
        val = (th + 1.0) * 0.5;
        dval = 0.5 * k * (1.0 - th * th);
        break;
    }
    default:
        val = a;
        dval = 1.0;
        break;
    }
}
} // namespace

extern "C" {

SLUICalculus8* slui_calc8_create(void) { return new SLUICalculus8(); }
void slui_calc8_destroy(SLUICalculus8* c) { delete c; }

void slui_calc8_set(SLUICalculus8* c, int i, double v) {
    if (c && i >= 0 && i < SLUI_CALC8_INPUTS) c->x[i] = clampd(v, 0.0, 1.0);
}
double slui_calc8_get(const SLUICalculus8* c, int i) {
    if (!c || i < 0 || i >= SLUI_CALC8_INPUTS) return 0.0;
    return c->x[i];
}
void slui_calc8_set_all(SLUICalculus8* c, const double* v, int n) {
    if (!c || !v) return;
    int m = std::min(n, SLUI_CALC8_INPUTS);
    for (int i = 0; i < m; ++i) c->x[i] = clampd(v[i], 0.0, 1.0);
}

/* Shared evaluation: aggregates a_s, basis values g_s, and the pre-squash sum.
 * Fills optional per-stage g[] and the aggregates and their basis derivatives
 * so the derivative wrt any input is a chain-rule away. */
static double calc8_core(const SLUICalculus8* c, double g[SLUI_CALC8_STAGES],
                         double dbasis[SLUI_CALC8_STAGES],
                         double agg[SLUI_CALC8_STAGES]) {
    double a[SLUI_CALC8_STAGES] = {0, 0, 0, 0, 0, 0, 0, 0};
    double wsum[SLUI_CALC8_STAGES] = {0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < SLUI_CALC8_INPUTS; ++i) {
        int s = SLUICalculus8::stage_of(i);
        double w = SLUICalculus8::in_weight(i);
        a[s] += w * c->x[i];
        wsum[s] += w;
    }
    double pre = 0.0;
    for (int s = 0; s < SLUI_CALC8_STAGES; ++s) {
        double as = wsum[s] > 0 ? a[s] / wsum[s] : 0.0; /* normalise to [0,1] */
        agg[s] = as;
        double val, dval;
        basis(s, as, val, dval);
        g[s] = val;
        dbasis[s] = dval / (wsum[s] > 0 ? wsum[s] : 1.0); /* d a_s / d x within */
        pre += c->beta[s] * val;
    }
    return pre;
}

double slui_calc8_eval(const SLUICalculus8* c, int wrt, double* derivative) {
    if (!c) {
        if (derivative) *derivative = 0.0;
        return 0.5;
    }
    double g[SLUI_CALC8_STAGES], db[SLUI_CALC8_STAGES], agg[SLUI_CALC8_STAGES];
    double pre = calc8_core(c, g, db, agg);
    double M = logistic(pre - static_cast<double>(SLUI_CALC8_STAGES) * 0.5);
    if (derivative) {
        double dM = 0.0;
        if (wrt >= 0 && wrt < SLUI_CALC8_INPUTS) {
            int s = SLUICalculus8::stage_of(wrt);
            double w = SLUICalculus8::in_weight(wrt);
            /* d pre / d x_wrt = beta_s * phi'_s(a_s) * (w / wsum_s). db[s]
             * already folded (1/wsum_s); multiply by this input's weight. */
            double dpre = c->beta[s] * db[s] * w;
            dM = M * (1.0 - M) * dpre; /* logistic chain rule */
        }
        *derivative = dM;
    }
    return M;
}

int slui_calc8_stages(const SLUICalculus8* c, double* out, int cap) {
    if (!c || !out) return 0;
    double g[SLUI_CALC8_STAGES], db[SLUI_CALC8_STAGES], agg[SLUI_CALC8_STAGES];
    calc8_core(c, g, db, agg);
    int n = std::min(cap, SLUI_CALC8_STAGES);
    for (int s = 0; s < n; ++s) out[s] = c->beta[s] * g[s];
    return n;
}

} // extern "C"

/* ==========================================================================
 * Mood
 * ========================================================================== */
struct SLUIMood {
    SLUIMoodKind kind = SLUI_MOOD_CALM;
    SLUIWash wash;
    SLUITanor tanor;
    const SLUICalculus8* calc = nullptr; /* borrowed */
};

/* Build the tasteful default wash for a built-in mood kind. */
static SLUIWash default_wash(SLUIMoodKind k) {
    switch (k) {
    case SLUI_MOOD_WARM:
        return slui_wash_triad(slui_rgb(0xFF, 0xD9, 0x9A), slui_rgb(0xFF, 0xA8, 0x4C),
                               slui_rgb(0xC8, 0x5A, 0x22));
    case SLUI_MOOD_COOL:
        return slui_wash_triad(slui_rgb(0xBF, 0xE6, 0xFF), slui_rgb(0x6E, 0xA8, 0xE8),
                               slui_rgb(0x3A, 0x5C, 0xA8));
    case SLUI_MOOD_VIVID:
        return slui_wash_triad(slui_rgb(0xFF, 0xE0, 0x66), slui_rgb(0xFF, 0x5E, 0x8A),
                               slui_rgb(0x7A, 0x4C, 0xFF));
    case SLUI_MOOD_SOMBER:
        return slui_wash_linear(slui_rgb(0x3A, 0x2E, 0x2A), slui_rgb(0x15, 0x10, 0x0E));
    case SLUI_MOOD_TENDER:
        return slui_wash_triad(slui_rgb(0xFF, 0xE4, 0xEC), slui_rgb(0xF2, 0xB8, 0xC6),
                               slui_rgb(0xD6, 0x8F, 0xA6));
    case SLUI_MOOD_RADIANT:
        return slui_wash_triad(slui_rgb(0xFF, 0xFB, 0xE8), slui_rgb(0xFF, 0xD9, 0x7A),
                               slui_rgb(0xFF, 0xAE, 0x3C));
    case SLUI_MOOD_CALM:
    default:
        return slui_wash_triad(slui_rgb(0xEF, 0xE6, 0xD8), slui_rgb(0xCF, 0xB8, 0x9C),
                               slui_rgb(0x9A, 0x7E, 0x62));
    }
}

extern "C" {

SLUIMood* slui_mood_create(SLUIMoodKind kind) {
    auto* m = new (std::nothrow) SLUIMood();
    if (!m) return nullptr;
    m->kind = kind;
    m->tanor = slui_tanor_default();
    if (kind != SLUI_MOOD_CUSTOM) m->wash = default_wash(kind);
    else m->wash = slui_wash_begin();
    /* temper the default tanor to the mood */
    if (kind == SLUI_MOOD_COOL) { m->tanor.warmth = 0.25; m->tanor.tone = slui_rgb(0xCF,0xE4,0xFF); }
    else if (kind == SLUI_MOOD_SOMBER) { m->tanor.depth = 0.9; m->tanor.refresh_hz = 0.2; }
    else if (kind == SLUI_MOOD_VIVID) { m->tanor.refresh_hz = 1.5; m->tanor.left_bias = 0.35; }
    return m;
}
void slui_mood_destroy(SLUIMood* m) { delete m; }

void slui_mood_set_wash(SLUIMood* m, SLUIWash w) { if (m) m->wash = w; }
void slui_mood_set_tanor(SLUIMood* m, SLUITanor t) { if (m) m->tanor = t; }
void slui_mood_set_calculus(SLUIMood* m, const SLUICalculus8* c) {
    if (m) m->calc = c;
}
SLUIWash slui_mood_wash(const SLUIMood* m) { return m ? m->wash : slui_wash_begin(); }
SLUITanor slui_mood_tanor(const SLUIMood* m) {
    return m ? m->tanor : slui_tanor_default();
}

SLUIColor slui_mood_color(const SLUIMood* m, double time_s) {
    if (!m) return 0xFFFFFFFF;
    /* where to sample the wash: the calculus's mood scalar, drifted by the
     * tanor's refresh + a little-left bias so a lightless bulb freshens. */
    double base = 0.5;
    if (m->calc) base = slui_calc8_eval(m->calc, -1, nullptr);
    double phase = (m->tanor.refresh_hz > 0)
                       ? std::sin(2.0 * kPi * m->tanor.refresh_hz * time_s)
                       : 0.0;
    /* "a little left": the drift is biased negative (toward the wash start). */
    double drift = phase * 0.08 - m->tanor.left_bias * 0.06;
    double t = clampd(base + drift, 0.0, 1.0);
    SLUIColor washed = slui_wash_sample(&m->wash, t);
    /* fold the tanor tone in by its warmth so the portrait glow tints it */
    Color w = decode(washed), tone = decode(m->tanor.tone);
    double k = clampd(m->tanor.warmth * 0.35, 0.0, 1.0);
    return slui::lerp(w, tone, k).to_abi();
}

void slui_light_apply_mood(SLUILight* light, const SLUIMood* m, double time_s) {
    if (!light || !m) return;
    light->color = slui_mood_color(m, time_s);
    /* the tanor's diffusion softens the light; its depth lifts intensity a bit
     * so a lightless-bulb glow reads as present without a hard source. */
    light->softness = clampd(0.3 + m->tanor.diffusion * 0.6, 0.0, 1.0);
    light->intensity *= (0.85 + m->tanor.depth * 0.3);
}

SLUILight slui_light_for_text(double font_x, double font_baseline, SLUIMm mm,
                              double dpi, const SLUIMood* mood, double time_s,
                              SLUILightRole role, SLUIPolarity polarity,
                              double reach_px, double intensity) {
    SLUILight l = slui_light_point(font_x, font_baseline, reach_px, intensity,
                                   0xFFFFFFFF);
    l.role = role;
    l.polarity = polarity;
    slui_light_place_mm(&l, font_x, font_baseline, mm, dpi);
    if (mood) slui_light_apply_mood(&l, mood, time_s);
    return l;
}

} // extern "C"
