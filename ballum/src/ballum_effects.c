/*
 * ballum/src/ballum_effects.c
 * Ballum transition effects (see ballum_effects.h). Pure, deterministic,
 * integer-only per-pixel compositing. Keep in sync with
 * lib/ballum/SLBallumEffect.sleela.
 */
#include "sleela_ballum/ballum_effects.h"
#include <string.h>

/* ---- small helpers ------------------------------------------------------- */

static int clampi(int v, int lo, int hi) { return v < lo ? lo : (v > hi ? hi : v); }

/* Alpha-blend straight-alpha `fg` over `bg` scaled by a reveal weight a(0..255):
 * effective fg alpha = fg.a * a / 255. */
static ballum_color_t over(ballum_color_t fg, ballum_color_t bg, int a /*0..255*/) {
    int fa = fg.a * clampi(a, 0, 255) / 255;
    ballum_color_t o;
    o.r = (uint8_t)((fg.r * fa + bg.r * (255 - fa)) / 255);
    o.g = (uint8_t)((fg.g * fa + bg.g * (255 - fa)) / 255);
    o.b = (uint8_t)((fg.b * fa + bg.b * (255 - fa)) / 255);
    o.a = 255;
    return o;
}

/* integer isqrt */
static int isqrt_i(long v) {
    if (v <= 0) return 0;
    long x = v, y = (x + 1) / 2;
    while (y < x) { x = y; y = (x + v / x) / 2; }
    return (int)x;
}

/* A cheap, stable per-pixel hash -> 0..99, for dissolve/sparkle ordering. */
static int hash100(int x, int y) {
    unsigned h = (unsigned)(x * 73856093) ^ (unsigned)(y * 19349663);
    h ^= h >> 13; h *= 0x5bd1e995u; h ^= h >> 15;
    return (int)(h % 100u);
}

/* Bayer 4x4 ordered-dither threshold 0..99 for a smooth dissolve. */
static int bayer100(int x, int y) {
    static const int m[16] = { 0, 8, 2,10, 12, 4,14, 6, 3,11, 1, 9, 15, 7,13, 5 };
    return m[(y & 3) * 4 + (x & 3)] * 100 / 16;
}

/* ---- catalog names ------------------------------------------------------- */

static const char *k_names[BALLUM_FX__COUNT] = {
    "fade","wipe-left","wipe-right","wipe-up","wipe-down",
    "iris-in","iris-out","bars","blinds","dissolve","pixelate",
    "slide-left","slide-up","zoom","spotlight","ripple",
    "checker","diamond","clock","sparkle"
};

const char *ballum_fx_name(ballum_fx_t fx) {
    if (fx < 0 || fx >= BALLUM_FX__COUNT) return "fade";
    return k_names[fx];
}

int ballum_fx_count(void) { return (int)BALLUM_FX__COUNT; }

ballum_fx_t ballum_fx_by_name(const char *name) {
    if (name) for (int i = 0; i < (int)BALLUM_FX__COUNT; ++i)
        if (strcmp(name, k_names[i]) == 0) return (ballum_fx_t)i;
    return BALLUM_FX_FADE;
}

/* ---- the effects --------------------------------------------------------- */
/* Each returns a "reveal weight" r in 0..255 (how much fg shows at this pixel
 * for this t). A soft edge uses an intermediate weight; a hard mask is 0 or 255.
 * The final composite is over(fg,bg,r). Some effects also modulate fg itself
 * (zoom/pixelate/sparkle) before compositing. */

ballum_color_t ballum_fx_pixel(ballum_fx_t fx, int t, int w, int h,
                               int x, int y, ballum_color_t fg, ballum_color_t bg) {
    t = clampi(t, 0, 100);
    if (w <= 0 || h <= 0) return bg;
    const int cx = w / 2, cy = h / 2;

    switch (fx) {
    case BALLUM_FX_FADE:
        return over(fg, bg, t * 255 / 100);

    case BALLUM_FX_WIPE_LEFT:
        return over(fg, bg, (x * 100 / w) <= t ? 255 : 0);
    case BALLUM_FX_WIPE_RIGHT:
        return over(fg, bg, ((w - 1 - x) * 100 / w) <= t ? 255 : 0);
    case BALLUM_FX_WIPE_UP:
        return over(fg, bg, ((h - 1 - y) * 100 / h) <= t ? 255 : 0);
    case BALLUM_FX_WIPE_DOWN:
        return over(fg, bg, (y * 100 / h) <= t ? 255 : 0);

    case BALLUM_FX_IRIS_IN: {
        long dx = x - cx, dy = y - cy;
        int dist = isqrt_i(dx * dx + dy * dy);
        int maxr = isqrt_i((long)cx * cx + (long)cy * cy);
        int r = maxr * t / 100;
        /* soft 8px edge */
        int soft = r - dist;
        return over(fg, bg, clampi(soft * 32, 0, 255));
    }
    case BALLUM_FX_IRIS_OUT: {
        /* Reveal that closes inward: the revealed band is everything OUTSIDE a
         * shrinking hole of radius r. As t->100 the hole closes to nothing, so
         * the whole frame (center included) is revealed. */
        long dx = x - cx, dy = y - cy;
        int dist = isqrt_i(dx * dx + dy * dy);
        int maxr = isqrt_i((long)cx * cx + (long)cy * cy);
        int r = maxr * (100 - t) / 100;       /* hole radius: maxr -> 0 */
        int soft = dist - r;                  /* >0 outside the hole (revealed) */
        if (t >= 100) return over(fg, bg, 255);  /* fully revealed at the end */
        return over(fg, bg, clampi(soft * 32, 0, 255));
    }

    case BALLUM_FX_BARS: {           /* vertical blinds: 16 columns open */
        int bars = 16, bw = (w + bars - 1) / bars;
        int local = (x % bw) * 100 / (bw > 0 ? bw : 1);
        return over(fg, bg, local <= t ? 255 : 0);
    }
    case BALLUM_FX_BLINDS: {         /* horizontal blinds: 12 rows open */
        int rows = 12, bh = (h + rows - 1) / rows;
        int local = (y % bh) * 100 / (bh > 0 ? bh : 1);
        return over(fg, bg, local <= t ? 255 : 0);
    }

    case BALLUM_FX_DISSOLVE:
        return over(fg, bg, bayer100(x, y) < t ? 255 : 0);

    case BALLUM_FX_PIXELATE: {
        /* coarse mosaic that sharpens: block size shrinks big->1 as t rises.
         * We snap the pixel to its block's top-left-ish center via a weight,
         * and ramp opacity with t so it emerges from the background. */
        int blk = 1 + (40 * (100 - t) / 100);   /* 41 -> 1 */
        int bxp = (x / blk) * blk + blk / 2;
        int byp = (y / blk) * blk + blk / 2;
        (void)bxp; (void)byp;                    /* mosaic sampling is caller-side;
                                                    here we ramp reveal + a block
                                                    checker to suggest the grid. */
        int grid = ((x / blk) + (y / blk)) & 1 ? 255 : 220;
        int r = t * 255 / 100;
        ballum_color_t m = fg; m.r = (uint8_t)(fg.r * grid / 255);
        m.g = (uint8_t)(fg.g * grid / 255); m.b = (uint8_t)(fg.b * grid / 255);
        return over(m, bg, r);
    }

    case BALLUM_FX_SLIDE_LEFT: {     /* fg content shifted; reveal from right */
        int edge = w - (w * t / 100);
        return over(fg, bg, x >= edge ? 255 : 0);
    }
    case BALLUM_FX_SLIDE_UP: {
        int edge = h - (h * t / 100);
        return over(fg, bg, y >= edge ? 255 : 0);
    }

    case BALLUM_FX_ZOOM: {           /* grows from center + fades in */
        long dx = x - cx, dy = y - cy;
        int dist = isqrt_i(dx * dx + dy * dy);
        int maxr = isqrt_i((long)cx * cx + (long)cy * cy);
        int r = maxr * t / 100;
        int inside = dist <= r ? 255 : 0;
        return over(fg, bg, inside * (t * 255 / 100) / 255);
    }

    case BALLUM_FX_SPOTLIGHT: {      /* soft growing radial spotlight */
        long dx = x - cx, dy = y - cy;
        int dist = isqrt_i(dx * dx + dy * dy);
        int maxr = isqrt_i((long)cx * cx + (long)cy * cy);
        int r = maxr * t / 100;
        int w8 = clampi((r - dist) * 255 / (maxr / 6 + 1), 0, 255);
        return over(fg, bg, w8);
    }

    case BALLUM_FX_RIPPLE: {         /* concentric rings sweep outward */
        long dx = x - cx, dy = y - cy;
        int dist = isqrt_i(dx * dx + dy * dy);
        int maxr = isqrt_i((long)cx * cx + (long)cy * cy);
        int front = maxr * t / 100;
        /* revealed inside the front; a ring shimmer near the front edge */
        if (dist > front) return bg;
        int ring = ((front - dist) % 24) < 12 ? 255 : 210;
        ballum_color_t m = fg; m.r = (uint8_t)(fg.r * ring / 255);
        m.g = (uint8_t)(fg.g * ring / 255); m.b = (uint8_t)(fg.b * ring / 255);
        return over(m, bg, 255);
    }

    case BALLUM_FX_CHECKER: {        /* 10x10 checkerboard squares fill in */
        int cxs = 10, cys = 10;
        int sw = (w + cxs - 1) / cxs, sh = (h + cys - 1) / cys;
        int gx = x / (sw > 0 ? sw : 1), gy = y / (sh > 0 ? sh : 1);
        int order = ((gx + gy) & 1) ? (gx + gy) : (gx + gy);
        int thresh = order * 100 / (cxs + cys);
        return over(fg, bg, thresh <= t ? 255 : 0);
    }

    case BALLUM_FX_DIAMOND: {        /* expanding diamond (Manhattan iris) */
        int dist = (x > cx ? x - cx : cx - x) + (y > cy ? y - cy : cy - y);
        int maxd = cx + cy;
        int r = maxd * t / 100;
        return over(fg, bg, clampi((r - dist) * 32, 0, 255));
    }

    case BALLUM_FX_CLOCK: {          /* radial clock wipe (12 o'clock, CW) */
        /* angle of the pixel, 0..100 going clockwise from straight up, via a
         * cheap octant/ratio approximation (no float). */
        long dx = x - cx, dy = cy - y;           /* dy up-positive */
        long adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
        long ratio = (adx + ady) ? (adx * 25) / (adx + ady) : 0; /* 0..25 per octant */
        long ang;                                  /* 0..100 clockwise from up */
        if (dy >= 0 && dx >= 0)      ang = ratio;              /* up -> right   */
        else if (dy < 0 && dx >= 0)  ang = 50 - ratio;         /* right -> down */
        else if (dy < 0 && dx < 0)   ang = 50 + ratio;         /* down -> left  */
        else                         ang = 100 - ratio;        /* left -> up    */
        return over(fg, bg, ang <= t ? 255 : 0);
    }

    case BALLUM_FX_SPARKLE: {        /* dissolve + a twinkle shimmer over fg */
        int reveal = hash100(x, y) < t ? 255 : 0;
        int tw = (hash100(x + t, y - t) < 8) ? 255 : 230;   /* occasional sparkle */
        ballum_color_t m = fg;
        m.r = (uint8_t)clampi(fg.r * tw / 230, 0, 255);
        m.g = (uint8_t)clampi(fg.g * tw / 230, 0, 255);
        m.b = (uint8_t)clampi(fg.b * tw / 230, 0, 255);
        return over(m, bg, reveal);
    }

    default:
        return over(fg, bg, t * 255 / 100);
    }
}
