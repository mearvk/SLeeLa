/* Portable tests for the program entry/exit transitions. */
#include "sleela_ballum/ballum_transition.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W 64u
#define H 48u

static ballum_color_t g_bg;

/* Every pixel unique, so equality with the source means "settled at home". */
static void make_src(ballum_color_t *p) {
    for (uint32_t y = 0; y < H; ++y)
        for (uint32_t x = 0; x < W; ++x) {
            ballum_color_t c = { (uint8_t)(x * 4), (uint8_t)(y * 5),
                                 (uint8_t)((x * 7 + y * 3) & 0xFF), 255 };
            if (c.r == 255 && c.g == 255 && c.b == 255) c.b = 254;   /* never == white bg */
            p[y * W + x] = c;
        }
}
static int same(ballum_color_t a, ballum_color_t b) {
    return a.r == b.r && a.g == b.g && a.b == b.b;
}
static size_t count_equal(const ballum_color_t *a, const ballum_color_t *b) {
    size_t n = 0;
    for (size_t i = 0; i < (size_t)W * H; ++i) n += same(a[i], b[i]);
    return n;
}
static int all_bg(const ballum_color_t *a) {
    for (size_t i = 0; i < (size_t)W * H; ++i) if (!same(a[i], g_bg)) return 0;
    return 1;
}

int main(void) {
    g_bg = ballum_color_rgb(255, 255, 255);
    ballum_color_t *src = malloc(sizeof(ballum_color_t) * W * H);
    ballum_color_t *a = malloc(sizeof(ballum_color_t) * W * H);
    ballum_color_t *b = malloc(sizeof(ballum_color_t) * W * H);
    assert(src && a && b);
    make_src(src);

    /* ---- names ----------------------------------------------------------- */
    ballum_transition_t tr;
    assert(ballum_transition_by_name("disintegrate", &tr) && tr.kind == BALLUM_TRANSITION_DISINTEGRATE);
    assert(strcmp(ballum_transition_name(&tr), "disintegrate") == 0);
    assert(ballum_transition_by_name("pixelize", &tr) && tr.kind == BALLUM_TRANSITION_PIXELIZE);
    assert(ballum_transition_by_name("slide-left", &tr) && tr.kind == BALLUM_TRANSITION_SLIDE_LEFT);
    assert(ballum_transition_by_name("iris-in", &tr) && tr.kind == BALLUM_TRANSITION_FX && tr.fx == BALLUM_FX_IRIS_IN);
    assert(strcmp(ballum_transition_name(&tr), "iris-in") == 0);
    assert(!ballum_transition_by_name("no-such-effect", &tr) && tr.kind == BALLUM_TRANSITION_NONE);
    assert(!ballum_transition_by_name("fx", &tr));
    assert(!ballum_transition_by_name(NULL, &tr));
    assert(ballum_transition_count() == (int)BALLUM_TRANSITION__COUNT - 1 + ballum_fx_count());

    /* every nameable transition: t=100 reproduces the source exactly */
    int names = 0;
    static const char *host_names[] = { "none", "slide-left", "slide-right", "slide-up",
                                        "slide-down", "pixelize", "disintegrate" };
    for (size_t i = 0; i < sizeof(host_names) / sizeof(host_names[0]); ++i) {
        assert(ballum_transition_by_name(host_names[i], &tr));
        assert(ballum_transition_frame(&tr, 100, W, H, src, g_bg, a));
        assert(count_equal(a, src) == (size_t)W * H);
        ++names;
    }
    for (int i = 0; i < ballum_fx_count(); ++i) {
        tr.kind = BALLUM_TRANSITION_FX; tr.fx = (ballum_fx_t)i;
        assert(ballum_transition_frame(&tr, 100, W, H, src, g_bg, a));
        assert(count_equal(a, src) == (size_t)W * H);   /* opaque src fully revealed */
        ++names;
    }
    assert(names == ballum_transition_count());

    /* ---- t = 0 is the background ---------------------------------------- */
    static const char *zero_names[] = { "none", "fade", "slide-left", "slide-right",
                                        "slide-up", "slide-down", "pixelize", "disintegrate" };
    for (size_t i = 0; i < sizeof(zero_names) / sizeof(zero_names[0]); ++i) {
        assert(ballum_transition_by_name(zero_names[i], &tr));
        assert(ballum_transition_frame(&tr, 0, W, H, src, g_bg, a));
        if (strcmp(zero_names[i], "none") == 0) assert(count_equal(a, src) == (size_t)W * H);
        else assert(all_bg(a));
    }

    /* ---- slide-left geometry: at t=50 the frame is shifted right by W/2 --- */
    ballum_transition_by_name("slide-left", &tr);
    assert(ballum_transition_frame(&tr, 50, W, H, src, g_bg, a));
    for (uint32_t y = 0; y < H; ++y)
        for (uint32_t x = 0; x < W; ++x) {
            if (x < W / 2) assert(same(a[y * W + x], g_bg));
            else           assert(same(a[y * W + x], src[y * W + (x - W / 2)]));
        }
    ballum_transition_by_name("slide-right", &tr);
    assert(ballum_transition_frame(&tr, 50, W, H, src, g_bg, a));
    for (uint32_t y = 0; y < H; ++y)
        for (uint32_t x = 0; x < W; ++x) {
            if (x >= W / 2) assert(same(a[y * W + x], g_bg));
            else            assert(same(a[y * W + x], src[y * W + (x + W / 2)]));
        }
    ballum_transition_by_name("slide-up", &tr);
    assert(ballum_transition_frame(&tr, 50, W, H, src, g_bg, a));
    for (uint32_t y = 0; y < H; ++y)
        for (uint32_t x = 0; x < W; ++x) {
            if (y < H / 2) assert(same(a[y * W + x], g_bg));
            else           assert(same(a[y * W + x], src[(y - H / 2) * W + x]));
        }

    /* ---- pixelize: mid-progress is blocky (neighbors share a color) ------- */
    ballum_transition_by_name("pixelize", &tr);
    assert(ballum_transition_frame(&tr, 50, W, H, src, g_bg, a));
    assert(count_equal(a, src) < (size_t)W * H / 4);              /* not the sharp frame */
    assert(same(a[0], a[1]) && same(a[0], a[W]));                  /* one 25px block */

    /* ---- disintegrate: settled pixels grow monotonically, then exact ------ */
    ballum_transition_by_name("disintegrate", &tr);
    size_t prev = 0;
    for (int t = 0; t <= 100; t += 10) {
        assert(ballum_transition_frame(&tr, t, W, H, src, g_bg, a));
        size_t eq = count_equal(a, src);
        assert(eq >= prev);
        prev = eq;
    }
    assert(prev == (size_t)W * H);
    assert(ballum_transition_frame(&tr, 45, W, H, src, g_bg, a));
    size_t mid = count_equal(a, src);
    assert(mid > 0 && mid < (size_t)W * H);                         /* genuinely in flight */
    assert(!all_bg(a));
    /* deterministic */
    assert(ballum_transition_frame(&tr, 45, W, H, src, g_bg, b));
    assert(memcmp(a, b, sizeof(ballum_color_t) * W * H) == 0);
    /* exit is the entry played backwards: 100-t of an entry == leaving at t */
    assert(ballum_transition_frame(&tr, 100 - 55, W, H, src, g_bg, b));
    assert(memcmp(a, b, sizeof(ballum_color_t) * W * H) == 0);

    /* ---- transparency: alpha 0 source pixels stay background at t=100 ----- */
    src[5].a = 0;
    ballum_transition_by_name("fade", &tr);
    assert(ballum_transition_frame(&tr, 100, W, H, src, g_bg, a));
    assert(same(a[5], g_bg));
    src[5].a = 255;

    /* ---- argument validation (fail closed) -------------------------------- */
    assert(!ballum_transition_frame(NULL, 50, W, H, src, g_bg, a));
    assert(!ballum_transition_frame(&tr, 50, 0, H, src, g_bg, a));
    assert(!ballum_transition_frame(&tr, 50, W, 0, src, g_bg, a));
    assert(!ballum_transition_frame(&tr, 50, W, H, NULL, g_bg, a));
    assert(!ballum_transition_frame(&tr, 50, W, H, src, g_bg, NULL));
    assert(!ballum_transition_frame(&tr, 50, W, H, src, g_bg, src));          /* alias */
    assert(!ballum_transition_frame(&tr, 50, W, H, src, g_bg, src + 3));      /* overlap */
    assert(!ballum_transition_frame(&tr, 50, 0xFFFFFFFFu, 0xFFFFFFFFu, src, g_bg, a)); /* overflow guard */
    /* out-of-range progress clamps */
    ballum_transition_by_name("slide-left", &tr);
    assert(ballum_transition_frame(&tr, 500, W, H, src, g_bg, a) && count_equal(a, src) == (size_t)W * H);
    assert(ballum_transition_frame(&tr, -9, W, H, src, g_bg, a) && all_bg(a));
    /* corrupt kind degrades to "appear at once" rather than crashing */
    tr.kind = (ballum_transition_kind_t)99;
    assert(ballum_transition_frame(&tr, 10, W, H, src, g_bg, a) && count_equal(a, src) == (size_t)W * H);
    assert(strcmp(ballum_transition_name(&tr), "none") == 0);

    free(src); free(a); free(b);
    puts("test_ballum_transition: ok");
    return 0;
}
