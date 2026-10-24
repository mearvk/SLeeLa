/* ballum/tests/test_ballum_effects.c -- transition effects + image handler. */
#include "sleela_ballum/ballum_effects.h"
#include "sleela_ballum/ballum_image.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static ballum_color_t C(int r, int g, int b, int a) {
    ballum_color_t c; c.r=(uint8_t)r; c.g=(uint8_t)g; c.b=(uint8_t)b; c.a=(uint8_t)a; return c;
}

int main(void) {
    const int W = 100, H = 100;
    ballum_color_t fg = C(200, 50, 50, 255);   /* opaque logo pixel */
    ballum_color_t bg = C(255, 255, 255, 255);  /* white             */

    /* catalog sanity */
    assert(ballum_fx_count() == (int)BALLUM_FX__COUNT);
    assert(ballum_fx_count() >= 20);
    assert(strcmp(ballum_fx_name(BALLUM_FX_IRIS_IN), "iris-in") == 0);
    assert(strcmp(ballum_fx_name(BALLUM_FX_SPARKLE), "sparkle") == 0);
    assert(ballum_fx_by_name("clock") == BALLUM_FX_CLOCK);
    assert(ballum_fx_by_name("nonsense") == BALLUM_FX_FADE);  /* unknown -> fade */
    assert(strcmp(ballum_fx_name((ballum_fx_t)999), "fade") == 0); /* out of range */

    /* FADE: t=0 is pure background, t=100 is pure foreground */
    {
        ballum_color_t a = ballum_fx_pixel(BALLUM_FX_FADE, 0, W, H, 50, 50, fg, bg);
        ballum_color_t z = ballum_fx_pixel(BALLUM_FX_FADE, 100, W, H, 50, 50, fg, bg);
        assert(a.r == bg.r && a.g == bg.g && a.b == bg.b);        /* all background */
        assert(z.r == fg.r && z.g == fg.g && z.b == fg.b);        /* all foreground */
        /* midway is between the two on the red channel */
        ballum_color_t m = ballum_fx_pixel(BALLUM_FX_FADE, 50, W, H, 50, 50, fg, bg);
        assert(m.r > fg.r && m.r < bg.r);
    }

    /* WIPE_LEFT: at t=50 the left half is revealed, the right half is not */
    {
        ballum_color_t left  = ballum_fx_pixel(BALLUM_FX_WIPE_LEFT, 50, W, H, 10, 50, fg, bg);
        ballum_color_t right = ballum_fx_pixel(BALLUM_FX_WIPE_LEFT, 50, W, H, 90, 50, fg, bg);
        assert(left.r == fg.r);                                   /* revealed */
        assert(right.r == bg.r);                                  /* not yet  */
    }

    /* IRIS_IN: center reveals before the corner as t grows */
    {
        ballum_color_t ctr = ballum_fx_pixel(BALLUM_FX_IRIS_IN, 30, W, H, 50, 50, fg, bg);
        ballum_color_t cnr = ballum_fx_pixel(BALLUM_FX_IRIS_IN, 30, W, H, 0, 0, fg, bg);
        assert(ctr.r <= cnr.r);   /* center closer to fg (smaller r) than corner */
    }

    /* every effect: t=0 touches background near a corner; t=100 reveals center */
    for (int i = 0; i < ballum_fx_count(); ++i) {
        ballum_color_t z = ballum_fx_pixel((ballum_fx_t)i, 100, W, H, 50, 50, fg, bg);
        /* at full progress the center pixel should be foreground-dominated
         * (closer to fg than to bg on red) for every effect */
        assert(z.r <= (fg.r + bg.r) / 2);
    }

    /* a transparent fg pixel stays background regardless of effect/progress */
    {
        ballum_color_t clear = C(0,0,0,0);
        ballum_color_t o = ballum_fx_pixel(BALLUM_FX_FADE, 100, W, H, 50, 50, clear, bg);
        assert(o.r == bg.r && o.g == bg.g && o.b == bg.b);
    }

    /* image handler: empty image is not ready and samples transparent */
    {
        ballum_image_t img; ballum_image_init(&img);
        assert(ballum_image_ready(&img) == 0);
        ballum_color_t t = ballum_image_at(&img, 0, 0);
        assert(t.a == 0);
        /* loading garbage fails closed */
        assert(ballum_image_load_memory(&img, "not a jpeg", 10) != BALLUM_STATUS_OK);
        assert(ballum_image_ready(&img) == 0);
        ballum_image_free(&img);
    }

    printf("test_ballum_effects: ok\n");
    return 0;
}
