/* =============================================================================
 * SleelaUI Throbber + CanvasView.
 *
 * CanvasView is the general animated drawing widget: it owns a double-buffered
 * SLUIDrawContext sized to its bounds and, each frame, lets a developer draw
 * callback paint into it with the comprehensive Draw API, then blits the result
 * into the window. Throbber is a CanvasView with a built-in draw callback that
 * is a WATER-LIKE FLOW FIELD.
 *
 * The flow model (why it reads as water, not a mechanical scroll):
 *
 *   * A 1-D field across the throbber's width carries a surface HEIGHT h[i] and
 *     a VELOCITY u[i]. Each step we advect height by velocity and apply a light
 *     diffusion + restoring force -- a shallow-water-style relaxation -- so
 *     disturbances propagate as travelling crests that disperse and recombine
 *     the way ripples do, instead of a rigid marquee.
 *   * Motion is integrated so the field SPEEDS UP AND SLOWS DOWN smoothly (the
 *     drive is a slow sinusoidal "amplification", bounded in its rate of
 *     change), echoing the project's existing throbber jerk-bounded feel but
 *     made fluid.
 *   * COLOUR IS PREDICTIVE: a crest is coloured by where it is ABOUT TO BE --
 *     the hue is advanced along the base hue by the local velocity, so faster
 *     water leads warmer/forward in hue and the colour arrives slightly ahead
 *     of the crest. Under strong amplification the hue spread widens (latent
 *     colour surfaces on the brightest crests) then recedes as the field calms.
 *   * WIDTH IS ADJUSTABLE: the field resamples to any pixel width while keeping
 *     the same physical wavelength feel, so a 40px and a 400px throbber look
 *     like the same water at different crops.
 *
 * It is "more natural than Water? -- no": the model is tuned to approach, not
 * exceed, the ease of real flowing water: bounded acceleration, energy that
 * only ever dissipates between impulses, and no instantaneous jumps.
 *
 * Max Rupplin -- MEARVK LLC -- 2026
 * ===========================================================================*/
#include "slui_widget.hpp"

#include "sleela_ui_draw.h"
#include "slui_backend.hpp"
#include "slui_render.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace slui {

/* ---- CanvasView --------------------------------------------------------- */
CanvasView::CanvasView(int min_w, int min_h)
    : Widget(WidgetKind::CanvasView), req_w_(min_w), req_h_(min_h) {}

CanvasView::~CanvasView() {
    if (ctx_) slui_draw_destroy(ctx_);
}

SLUISize CanvasView::measure(const PaintContext&) {
    return SLUISize{req_w_ + margin_.left + margin_.right,
                    req_h_ + margin_.top + margin_.bottom};
}

void CanvasView::ensure_context(int w, int h) {
    if (w < 1) w = 1;
    if (h < 1) h = 1;
    if (!ctx_) {
        ctx_ = slui_draw_create(w, h, SLUI_BUFFER_DOUBLE);
        ctx_w_ = w;
        ctx_h_ = h;
    } else if (ctx_w_ != w || ctx_h_ != h) {
        slui_draw_resize(ctx_, w, h);
        ctx_w_ = w;
        ctx_h_ = h;
    }
}

void CanvasView::paint(PaintContext& ctx) {
    Rect c = content();
    if (c.empty()) return;
    ensure_context(c.w, c.h);
    if (draw_fn_ && ctx_) {
        draw_fn_(ctx_, time_, last_dt_, draw_user_);
        slui_draw_present(ctx_);
        /* Blit the context's front buffer into the window canvas. */
        const int stride = ctx_w_;
        /* read directly from the draw context's published pixels */
        uint32_t* px = slui_draw_lock(ctx_, nullptr);
        if (px) {
            /* lock returns the DRAW target; we want the FRONT (published) one.
             * Instead read via get_pixel through the public API blit path by
             * compositing word-for-word here using the ABI read. */
            slui_draw_unlock(ctx_);
        }
        /* Composite the published surface pixel-for-pixel into the window. */
        for (int y = 0; y < c.h; ++y) {
            for (int x = 0; x < c.w; ++x) {
                SLUIColor word = slui_draw_get_pixel(ctx_, x, y);
                Color k = Color::from_abi(word);
                if (k.a == 0) continue;
                ctx.canvas->blend(c.x + x, c.y + y, k, 1.0);
            }
        }
        (void)stride;
    }
}

/* ---- Throbber water field ----------------------------------------------- */
namespace {

/* Per-throbber flow state, owned via the draw user pointer. */
struct WaterField {
    int cells = 0;             /* resolution across the width               */
    std::vector<double> h;     /* surface height (relative, ~[-1,1])        */
    std::vector<double> u;     /* horizontal velocity                       */
    std::vector<double> hue_lead; /* smoothed predictive hue offset / cell  */
    double drive_phase = 0.0;  /* slow amplification oscillator phase       */
    double amp = 0.6;          /* current amplification [0..1]              */
    double amp_vel = 0.0;      /* its bounded rate of change                */
    double spawn_accum = 0.0;  /* impulse accumulator                       */
    double base_hue = 205.0;
    double intensity = 0.6;
    uint32_t rng = 0x1234567u;

    double rand01() {
        /* xorshift -> [0,1) */
        rng ^= rng << 13;
        rng ^= rng >> 17;
        rng ^= rng << 5;
        return (rng & 0xFFFFFF) / static_cast<double>(0x1000000);
    }

    void ensure(int width_px) {
        /* ~1 cell per 3 px keeps the wavelength natural at any width. */
        int want = std::max(16, width_px / 3);
        if (want != cells) {
            std::vector<double> nh(want, 0.0), nu(want, 0.0), nl(want, 0.0);
            /* resample existing field so a width change keeps the water */
            for (int i = 0; i < want; ++i) {
                if (cells > 0) {
                    double s = i * (cells - 1) / static_cast<double>(want - 1 > 0 ? want - 1 : 1);
                    int si = static_cast<int>(s);
                    double f = s - si;
                    int sj = std::min(si + 1, cells - 1);
                    nh[i] = h[si] * (1 - f) + h[sj] * f;
                    nu[i] = u[si] * (1 - f) + u[sj] * f;
                    nl[i] = hue_lead[si] * (1 - f) + hue_lead[sj] * f;
                }
            }
            h.swap(nh);
            u.swap(nu);
            hue_lead.swap(nl);
            cells = want;
        }
    }
};

/* Step the shallow-water-like field forward by dt seconds. */
void step_field(WaterField& w, double dt) {
    int n = w.cells;
    if (n < 4) return;
    /* Clamp dt for stability; sub-step if the frame was long. */
    double remaining = std::min(dt, 0.1);
    const double sub = 1.0 / 240.0;
    while (remaining > 1e-6) {
        double s = std::min(sub, remaining);
        remaining -= s;

        /* Slow amplification drive: a bounded 2nd-order oscillator so the whole
         * field eases faster and slower (speeds up / slows down smoothly). */
        w.drive_phase += s * (0.25 + 0.9 * w.intensity);
        double target = 0.5 + 0.5 * std::sin(w.drive_phase);
        double accel = (target - w.amp) * 6.0 - w.amp_vel * 2.5;
        /* bound the jerk by capping |accel| */
        accel = std::clamp(accel, -4.0, 4.0);
        w.amp_vel += accel * s;
        w.amp += w.amp_vel * s;
        w.amp = std::clamp(w.amp, 0.0, 1.0);

        /* Occasional impulses feed the water, mostly from the left (flow is
         * mainly left->right). Rate scales with intensity + amplification. */
        w.spawn_accum += s * (2.0 + 10.0 * w.intensity) * (0.3 + w.amp);
        while (w.spawn_accum > 1.0) {
            w.spawn_accum -= 1.0;
            int at = (w.rand01() < 0.7)
                         ? static_cast<int>(w.rand01() * n * 0.35) /* left-biased */
                         : static_cast<int>(w.rand01() * n);
            at = std::clamp(at, 1, n - 2);
            double push = (0.4 + 0.6 * w.amp) * (0.6 + 0.8 * w.rand01());
            w.h[at] += push;
            w.u[at] += push * 1.2; /* give it rightward momentum */
        }

        /* Shallow-water update: height advected by velocity gradient, velocity
         * driven by height gradient (a wave equation) plus mild damping. */
        const double c2 = 0.9;   /* wave speed^2 (controls wavelength)       */
        const double visc = 0.6; /* viscosity -> ripples disperse like water */
        const double damp = 0.25;
        const double drift = 0.8 + 1.6 * w.intensity; /* mean rightward flow */
        std::vector<double> nh(w.h), nu(w.u);
        for (int i = 1; i < n - 1; ++i) {
            double dhdx = (w.h[i + 1] - w.h[i - 1]) * 0.5;
            double dudx = (w.u[i + 1] - w.u[i - 1]) * 0.5;
            /* velocity follows -g * dh/dx, with a steady rightward drift */
            nu[i] = w.u[i] + s * (-c2 * dhdx * 4.0) - s * damp * w.u[i];
            nu[i] += s * drift * (target - 0.5) * 0.4;
            /* height follows -d(h*u)/dx ~ -(u*dh/dx + h*du/dx) */
            nh[i] = w.h[i] - s * (w.u[i] * dhdx + w.h[i] * dudx) * 2.0;
            /* viscosity: blend toward the local average (ripple dispersion) */
            double avg = (w.h[i - 1] + w.h[i + 1]) * 0.5;
            nh[i] += s * visc * (avg - w.h[i]);
            nh[i] *= (1.0 - s * damp * 0.5); /* energy only dissipates */
        }
        /* soft reflective-ish boundaries */
        nh[0] = nh[1] * 0.6;
        nh[n - 1] = nh[n - 2] * 0.6;
        nu[0] = nu[1];
        nu[n - 1] = nu[n - 2];
        w.h.swap(nh);
        w.u.swap(nu);

        /* Predictive hue: lead the base hue by the local velocity so colour
         * arrives slightly ahead of the crest; smooth it so it flows. */
        for (int i = 0; i < n; ++i) {
            double lead = w.u[i] * (30.0 + 60.0 * w.amp); /* degrees ahead */
            w.hue_lead[i] += (lead - w.hue_lead[i]) * std::min(1.0, s * 12.0);
        }
    }
}

/* The Throbber's internal draw callback: step the water, then paint it. */
void throbber_draw(SLUIDrawContext* dc, double /*time_s*/, double dt_s,
                   void* user) {
    WaterField& w = *static_cast<WaterField*>(user);
    int W = slui_draw_width(dc), H = slui_draw_height(dc);
    w.ensure(W);
    step_field(w, dt_s > 0 ? dt_s : 1.0 / 60.0);

    /* At rest the strip is the chrome floor; crests light up over it. */
    slui_draw_clear(dc, slui_rgb(0x10, 0x10, 0x13));

    int n = w.cells;
    /* Draw additively so overlapping crests accumulate light like water. */
    slui_draw_set_blend(dc, SLUI_BLEND_ADD);
    for (int x = 0; x < W; ++x) {
        /* sample the field at this column */
        double fx = n > 1 ? x * (n - 1) / static_cast<double>(W - 1 > 0 ? W - 1 : 1) : 0.0;
        int i = static_cast<int>(fx);
        double f = fx - i;
        int j = std::min(i + 1, n - 1);
        double height = w.h[i] * (1 - f) + w.h[j] * f;
        double vel = w.u[i] * (1 - f) + w.u[j] * f;
        double lead = w.hue_lead[i] * (1 - f) + w.hue_lead[j] * f;

        /* crest brightness: positive height glows; gate by amplification */
        double crest = std::max(0.0, height);
        double bright = std::clamp(crest * (0.5 + 0.9 * w.amp), 0.0, 1.0);
        if (bright <= 0.01) continue;

        /* Predictive colour: base hue + velocity-led offset; saturation and
         * spread grow with amplification (latent colour surfaces on strong
         * crests), receding as the field calms. */
        double hue = w.base_hue + lead + vel * 20.0;
        double sat = std::clamp(0.25 + 0.55 * w.amp + std::fabs(vel) * 0.3, 0.0, 1.0);
        double val = std::clamp(0.45 + 0.55 * bright, 0.0, 1.0);
        SLUIColor top = slui_color_hsv(hue, sat, val, 1.0);

        /* The crest fills from the bottom up by its height; a thin bright cap
         * sits at the waterline so the surface reads as a moving sheet. */
        int fill_h = static_cast<int>(std::clamp(bright, 0.0, 1.0) * H + 0.5);
        if (fill_h < 1) fill_h = 1;
        int y0 = H - fill_h;
        /* body: dimmer tint beneath the waterline */
        SLUIColor body = slui_color_hsv(hue, sat * 0.8, val * 0.5, 1.0);
        for (int y = y0; y < H; ++y) {
            double ty = (y - y0) / static_cast<double>(fill_h);
            SLUIColor cc = slui_color_lerp(top, body, ty);
            slui_draw_blend_pixel(dc, x, y, cc, 0.5 + 0.5 * bright);
        }
        /* bright waterline cap */
        slui_draw_blend_pixel(dc, x, y0, top, bright);
        if (y0 - 1 >= 0)
            slui_draw_blend_pixel(dc, x, y0 - 1, top, bright * 0.4);
    }
    slui_draw_set_blend(dc, SLUI_BLEND_OVER);
}

} // namespace

Throbber::Throbber(int width_px) : CanvasView(width_px, 6) {
    kind_ = WidgetKind::Throbber;
    req_w_ = std::max(8, width_px);
    req_h_ = 6; /* a slim seam by default; grows with size_request */
    auto* field = new WaterField();
    field->base_hue = hue_;
    field->intensity = intensity_;
    set_draw_fn(&throbber_draw, field); /* draw_user_ owns the field */
}

Throbber::~Throbber() {
    delete static_cast<WaterField*>(draw_user_);
    draw_user_ = nullptr;
}

void Throbber::set_width_px(int wpx) {
    req_w_ = std::max(8, wpx);
    invalidate();
}

void Throbber::set_intensity(double v) {
    intensity_ = std::clamp(v, 0.0, 1.0);
    if (draw_user_) static_cast<WaterField*>(draw_user_)->intensity = intensity_;
    invalidate();
}

void Throbber::set_hue(double degrees) {
    hue_ = degrees;
    if (draw_user_) static_cast<WaterField*>(draw_user_)->base_hue = hue_;
    invalidate();
}

} // namespace slui
