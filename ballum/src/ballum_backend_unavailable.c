/*
 * Ballum unavailable backend -- the always-present fallback.
 *
 * Selected when no OS backend is compiled in (unknown OS, or a headless build).
 * Every operation fails closed with BALLUM_STATUS_BACKEND_UNAVAILABLE, and
 * enumerate reports zero displays, so a SLeeLa program degrades gracefully
 * instead of crashing. This is what keeps ballum_backend_default() non-NULL on
 * every platform.
 */

#include "sleela_ballum/ballum_manager.h"

static ballum_status_t unavail_enumerate(void *ctx, ballum_display_t *out,
                                         size_t capacity, size_t *out_count) {
    (void)ctx; (void)out; (void)capacity;
    if (out_count) *out_count = 0;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static ballum_status_t unavail_set_pixel(void *ctx, uint32_t id,
                                         ballum_point_t at, ballum_color_t color) {
    (void)ctx; (void)id; (void)at; (void)color;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static ballum_status_t unavail_get_pixel(void *ctx, uint32_t id,
                                         ballum_point_t at, ballum_color_t *out) {
    (void)ctx; (void)id; (void)at; (void)out;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static ballum_status_t unavail_blit(void *ctx, uint32_t id, ballum_rect_t rect,
                                    const ballum_color_t *pixels, size_t n) {
    (void)ctx; (void)id; (void)rect; (void)pixels; (void)n;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static ballum_status_t unavail_set_brightness(void *ctx, uint32_t id, int percent) {
    (void)ctx; (void)id; (void)percent;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static ballum_status_t unavail_set_warmth(void *ctx, uint32_t id, uint32_t kelvin) {
    (void)ctx; (void)id; (void)kelvin;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static ballum_status_t unavail_set_gamma(void *ctx, uint32_t id, uint32_t gamma) {
    (void)ctx; (void)id; (void)gamma;
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

static const ballum_backend_t k_unavailable = {
    "unavailable",
    unavail_enumerate,
    unavail_set_pixel,
    unavail_get_pixel,
    unavail_blit,
    unavail_set_brightness,
    unavail_set_warmth,
    unavail_set_gamma,
    /* poll_event = NULL: the fallback has no input queue, so the manager
     * reports has_input()==0 and poll returns UNSUPPORTED (never a fake event). */
    0,
    /* poll_key = NULL: the fallback has no keyboard queue either, so
     * has_keyboard()==0 and poll_key returns UNSUPPORTED (never a fake key). */
    0,
    /* claim_surface / present / release_surface = NULL: no fullscreen surface,
     * so has_surface()==0 and the three surface calls return UNSUPPORTED. */
    0, 0, 0
};

const ballum_backend_t *ballum_backend_unavailable(void) { return &k_unavailable; }
