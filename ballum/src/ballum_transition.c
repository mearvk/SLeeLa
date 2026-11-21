/*
 * ballum/src/ballum_transition.c -- program entry/exit transitions.
 * See ballum_transition.h. Integer-only and deterministic.
 */
#include "sleela_ballum/ballum_transition.h"

#include <stdint.h>
#include <string.h>

#define MAX_PIXELS_GUARD ((size_t)1 << 30)   /* 1G pixels: far beyond any display */

static int clampi(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

/* Straight-alpha `fg` over `bg`, with an extra reveal weight a (0..255). */
static ballum_color_t over(ballum_color_t fg, ballum_color_t bg, int a) {
    int fa = fg.a * clampi(a, 0, 255) / 255;
    ballum_color_t o;
    o.r = (uint8_t)((fg.r * fa + bg.r * (255 - fa)) / 255);
    o.g = (uint8_t)((fg.g * fa + bg.g * (255 - fa)) / 255);
    o.b = (uint8_t)((fg.b * fa + bg.b * (255 - fa)) / 255);
    o.a = 255;
    return o;
}

static uint32_t hash2(uint32_t x, uint32_t y) {
    uint32_t h = x * 0x9E3779B1u ^ (y + 0x7F4A7C15u) * 0x85EBCA6Bu;
    h ^= h >> 15; h *= 0x2C1B3C6Du;
    h ^= h >> 12; h *= 0x297A2D39u;
    h ^= h >> 15;
    return h;
}

/* ---- names --------------------------------------------------------------- */

static const char *const k_kind_names[BALLUM_TRANSITION__COUNT] = {
    "none", "fx", "slide-left", "slide-right", "slide-up", "slide-down",
    "pixelize", "disintegrate"
};

const char *ballum_transition_name(const ballum_transition_t *tr) {
    if (!tr || tr->kind < 0 || tr->kind >= BALLUM_TRANSITION__COUNT) return "none";
    if (tr->kind == BALLUM_TRANSITION_FX) return ballum_fx_name(tr->fx);
    return k_kind_names[tr->kind];
}

int ballum_transition_by_name(const char *name, ballum_transition_t *out) {
    if (!out) return 0;
    out->kind = BALLUM_TRANSITION_NONE;
    out->fx = BALLUM_FX_FADE;
    if (!name) return 0;
    for (int k = 0; k < (int)BALLUM_TRANSITION__COUNT; ++k) {
        if (k == (int)BALLUM_TRANSITION_FX) continue;          /* "fx" is not a name */
        if (strcmp(name, k_kind_names[k]) == 0) {
            out->kind = (ballum_transition_kind_t)k;
            return 1;
        }
    }
    for (int i = 0; i < ballum_fx_count(); ++i) {
        if (strcmp(name, ballum_fx_name((ballum_fx_t)i)) == 0) {
            out->kind = BALLUM_TRANSITION_FX;
            out->fx = (ballum_fx_t)i;
            return 1;
        }
    }
    return 0;
}

int ballum_transition_count(void) {
    /* host kinds except FX itself, plus every catalog effect */
    return ((int)BALLUM_TRANSITION__COUNT - 1) + ballum_fx_count();
}

/* ---- kinds --------------------------------------------------------------- */

static void frame_copy(uint32_t w, uint32_t h, const ballum_color_t *src,
                       ballum_color_t bg, ballum_color_t *dst) {
    size_t n = (size_t)w * h;
    for (size_t i = 0; i < n; ++i) dst[i] = over(src[i], bg, 255);
}

/* Content's left/top edge sits at (px, py); everything outside it is bg. */
static void frame_slide(uint32_t w, uint32_t h, const ballum_color_t *src,
                        ballum_color_t bg, ballum_color_t *dst, int64_t px, int64_t py) {
    for (uint32_t y = 0; y < h; ++y) {
        int64_t sy = (int64_t)y - py;
        for (uint32_t x = 0; x < w; ++x) {
            int64_t sx = (int64_t)x - px;
            dst[(size_t)y * w + x] =
                (sx >= 0 && sx < (int64_t)w && sy >= 0 && sy < (int64_t)h)
                    ? over(src[(size_t)sy * w + (size_t)sx], bg, 255) : bg;
        }
    }
}

static void frame_pixelize(int t, uint32_t w, uint32_t h, const ballum_color_t *src,
                           ballum_color_t bg, ballum_color_t *dst) {
    int blk = 1 + 48 * (100 - t) / 100;                 /* 49 px blocks -> 1 px */
    int weight = clampi(t * 255 / 50, 0, 255);          /* fully opaque by t=50 */
    for (uint32_t y = 0; y < h; ++y) {
        uint32_t sy = (y / (uint32_t)blk) * (uint32_t)blk + (uint32_t)blk / 2u;
        if (sy >= h) sy = h - 1u;
        for (uint32_t x = 0; x < w; ++x) {
            uint32_t sx = (x / (uint32_t)blk) * (uint32_t)blk + (uint32_t)blk / 2u;
            if (sx >= w) sx = w - 1u;
            ballum_color_t s = (blk == 1) ? src[(size_t)y * w + x]
                                          : src[(size_t)sy * w + sx];
            dst[(size_t)y * w + x] = over(s, bg, weight);
        }
    }
}

/* Per-pixel particle: arrival delay 0..79, so every particle has settled by t=100. */
typedef struct { int u; int dx; int dy; } particle_t;

static particle_t particle_at(int t, uint32_t w, uint32_t h, uint32_t x, uint32_t y) {
    uint32_t hv = hash2(x, y);
    int delay = (int)(hv % 50u) + (int)((uint64_t)x * 30u / w);   /* sweeps left->right */
    particle_t p;
    p.u = clampi((t - delay) * 5, 0, 100);
    p.dx = (((int)((hv >> 8) & 0xFFu)) - 128) * (int)(w > 8192u ? 8192u : w) / 512;
    p.dy = -((int)((hv >> 16) & 0xFFu)) * (int)(h > 8192u ? 8192u : h) / 512; /* falls from above */
    return p;
}

static void frame_disintegrate(int t, uint32_t w, uint32_t h, const ballum_color_t *src,
                               ballum_color_t bg, ballum_color_t *dst) {
    size_t n = (size_t)w * h;
    for (size_t i = 0; i < n; ++i) dst[i] = bg;
    /* pass 1: particles still in flight, blended where they land */
    for (uint32_t y = 0; y < h; ++y) {
        for (uint32_t x = 0; x < w; ++x) {
            particle_t p = particle_at(t, w, h, x, y);
            if (p.u <= 0 || p.u >= 100) continue;
            int64_t nx = (int64_t)x + (int64_t)p.dx * (100 - p.u) / 100;
            int64_t ny = (int64_t)y + (int64_t)p.dy * (100 - p.u) / 100;
            if (nx < 0 || ny < 0 || nx >= (int64_t)w || ny >= (int64_t)h) continue;
            size_t di = (size_t)ny * w + (size_t)nx;
            dst[di] = over(src[(size_t)y * w + x], dst[di], p.u * 255 / 100);
        }
    }
    /* pass 2: settled pixels last, so they always win over any flying particle */
    for (uint32_t y = 0; y < h; ++y) {
        for (uint32_t x = 0; x < w; ++x) {
            particle_t p = particle_at(t, w, h, x, y);
            if (p.u >= 100)
                dst[(size_t)y * w + x] = over(src[(size_t)y * w + x], bg, 255);
        }
    }
}

int ballum_transition_frame(const ballum_transition_t *tr, int t,
                            uint32_t w, uint32_t h,
                            const ballum_color_t *src, ballum_color_t bg,
                            ballum_color_t *dst) {
    if (!tr || !src || !dst || w == 0u || h == 0u) return 0;
    if ((size_t)w > MAX_PIXELS_GUARD / (size_t)h) return 0;
    size_t n = (size_t)w * h;
    if (dst == src || (dst < src + n && src < dst + n)) return 0;   /* no aliasing */

    t = clampi(t, 0, 100);
    if (t >= 100 || tr->kind == BALLUM_TRANSITION_NONE ||
        (unsigned)tr->kind >= (unsigned)BALLUM_TRANSITION__COUNT) {
        frame_copy(w, h, src, bg, dst);
        return 1;
    }
    switch (tr->kind) {
    case BALLUM_TRANSITION_FX:
        for (uint32_t y = 0; y < h; ++y)
            for (uint32_t x = 0; x < w; ++x)
                dst[(size_t)y * w + x] = ballum_fx_pixel(
                    tr->fx, t, (int)w, (int)h, (int)x, (int)y,
                    src[(size_t)y * w + x], bg);
        break;
    case BALLUM_TRANSITION_SLIDE_LEFT:
        frame_slide(w, h, src, bg, dst, (int64_t)w * (100 - t) / 100, 0);   break;
    case BALLUM_TRANSITION_SLIDE_RIGHT:
        frame_slide(w, h, src, bg, dst, -(int64_t)w * (100 - t) / 100, 0);  break;
    case BALLUM_TRANSITION_SLIDE_UP:
        frame_slide(w, h, src, bg, dst, 0, (int64_t)h * (100 - t) / 100);   break;
    case BALLUM_TRANSITION_SLIDE_DOWN:
        frame_slide(w, h, src, bg, dst, 0, -(int64_t)h * (100 - t) / 100);  break;
    case BALLUM_TRANSITION_PIXELIZE:
        frame_pixelize(t, w, h, src, bg, dst);                               break;
    case BALLUM_TRANSITION_DISINTEGRATE:
        frame_disintegrate(t, w, h, src, bg, dst);                           break;
    default:
        frame_copy(w, h, src, bg, dst);                                      break;
    }
    return 1;
}
