/*
 * Ballum manager test. Exercises enumeration, capability gating, bounds
 * checking, value clamping, and clean failure through a mock backend (so the
 * test runs with no display hardware). The mock records the last values the
 * manager dispatched, proving the portable manager validates before it touches
 * the device and never exceeds the documented ranges.
 */

#include "sleela_ballum/ballum_manager.h"

#include <assert.h>
#include <string.h>

/* The standalone test build links the unavailable backend for the fallback
 * check (defined in ballum_backend_unavailable.c). */
const ballum_backend_t *ballum_backend_unavailable(void);

/* ---- mock backend -------------------------------------------------------- */

static int      g_last_brightness = -1;
static uint32_t g_last_warmth = 0;
static uint32_t g_last_gamma = 0;
static ballum_color_t g_last_pixel;
static ballum_point_t g_last_point;
static int      g_set_pixel_calls = 0;

static ballum_status_t mock_enumerate(void *ctx, ballum_display_t *out,
                                      size_t capacity, size_t *out_count) {
    (void)ctx;
    if (capacity < 2) { *out_count = 2; return BALLUM_STATUS_BUFFER_TOO_SMALL; }
    memset(out, 0, 2 * sizeof(*out));

    out[0].id = 100;
    strncpy(out[0].name, "primary", sizeof(out[0].name) - 1);
    out[0].width = 1920; out[0].height = 1080; out[0].is_primary = 1;
    out[0].capabilities = BALLUM_CAP_PER_PIXEL | BALLUM_CAP_REGION_BLIT |
                          BALLUM_CAP_BRIGHTNESS | BALLUM_CAP_COLOR_TEMPERATURE |
                          BALLUM_CAP_GAMMA | BALLUM_CAP_FULLSCREEN_SURFACE;

    /* A second display that supports brightness ONLY (no per-pixel). */
    out[1].id = 200;
    strncpy(out[1].name, "projector", sizeof(out[1].name) - 1);
    out[1].width = 1280; out[1].height = 720; out[1].is_primary = 0;
    out[1].capabilities = BALLUM_CAP_BRIGHTNESS;

    *out_count = 2;
    return BALLUM_STATUS_OK;
}

static ballum_status_t mock_set_pixel(void *ctx, uint32_t id,
                                      ballum_point_t at, ballum_color_t color) {
    (void)ctx; (void)id;
    g_last_point = at; g_last_pixel = color; g_set_pixel_calls++;
    return BALLUM_STATUS_OK;
}

static ballum_status_t mock_get_pixel(void *ctx, uint32_t id,
                                      ballum_point_t at, ballum_color_t *out) {
    (void)ctx; (void)id; (void)at;
    *out = ballum_color_rgb(10, 20, 30);
    return BALLUM_STATUS_OK;
}

static ballum_status_t mock_blit(void *ctx, uint32_t id, ballum_rect_t rect,
                                 const ballum_color_t *pixels, size_t n) {
    (void)ctx; (void)id; (void)rect; (void)pixels; (void)n;
    return BALLUM_STATUS_OK;
}

static ballum_status_t mock_set_brightness(void *ctx, uint32_t id, int percent) {
    (void)ctx; (void)id; g_last_brightness = percent; return BALLUM_STATUS_OK;
}
static ballum_status_t mock_set_warmth(void *ctx, uint32_t id, uint32_t kelvin) {
    (void)ctx; (void)id; g_last_warmth = kelvin; return BALLUM_STATUS_OK;
}
static ballum_status_t mock_set_gamma(void *ctx, uint32_t id, uint32_t gamma) {
    (void)ctx; (void)id; g_last_gamma = gamma; return BALLUM_STATUS_OK;
}

/* A tiny scripted pointer-event queue: DOWN(100,100,left) then MOVE(140,100). */
static int g_poll_step = 0;
static ballum_status_t mock_poll_event(void *ctx, ballum_pointer_event_t *out) {
    (void)ctx;
    out->type = BALLUM_EVENT_NONE;
    if (g_poll_step == 0) {
        out->type = BALLUM_EVENT_POINTER_DOWN; out->x = 100; out->y = 100;
        out->button = BALLUM_BUTTON_LEFT; out->source = BALLUM_POINTER_MOUSE;
        g_poll_step = 1;
    } else if (g_poll_step == 1) {
        out->type = BALLUM_EVENT_POINTER_MOVE; out->x = 140; out->y = 100;
        out->source = BALLUM_POINTER_TOUCH; out->contact_id = 7;
        g_poll_step = 2;
    }
    return BALLUM_STATUS_OK;
}

static int g_key_poll_step = 0;
static ballum_status_t mock_poll_key(void *ctx, int *out) {
    (void)ctx;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    *out = BALLUM_KEY_NONE;
    if (g_key_poll_step == 0) *out = BALLUM_KEY_ESC;
    else if (g_key_poll_step == 2) *out = BALLUM_KEY_ENTER;
    ++g_key_poll_step;
    return BALLUM_STATUS_OK;
}

/* Mock fullscreen surface: record claim/present/release so the manager's new
 * surface path is exercised without any display hardware. */
static int g_surface_claimed = 0;
static int g_present_calls = 0;
static int g_release_calls = 0;
static ballum_status_t mock_claim_surface(void *ctx, uint32_t id) {
    (void)ctx; (void)id; g_surface_claimed = 1; return BALLUM_STATUS_OK;
}
static ballum_status_t mock_present(void *ctx, uint32_t id) {
    (void)ctx; (void)id; g_present_calls++; return BALLUM_STATUS_OK;
}
static ballum_status_t mock_release_surface(void *ctx, uint32_t id) {
    (void)ctx; (void)id; g_release_calls++; g_surface_claimed = 0; return BALLUM_STATUS_OK;
}

static const ballum_backend_t g_mock = {
    "mock", mock_enumerate, mock_set_pixel, mock_get_pixel, mock_blit,
    mock_set_brightness, mock_set_warmth, mock_set_gamma, mock_poll_event,
    mock_poll_key,
    mock_claim_surface, mock_present, mock_release_surface
};

int main(void) {
    /* ---- helpers & clamps ------------------------------------------------ */
    assert(ballum_clamp_brightness(-5) == BALLUM_BRIGHTNESS_MIN);
    assert(ballum_clamp_brightness(250) == BALLUM_BRIGHTNESS_MAX);
    assert(ballum_clamp_brightness(42) == 42);
    assert(ballum_clamp_warmth(500) == BALLUM_WARMTH_MIN_KELVIN);
    assert(ballum_clamp_warmth(99999) == BALLUM_WARMTH_MAX_KELVIN);
    assert(ballum_clamp_gamma(5) == BALLUM_GAMMA_MIN);
    assert(ballum_clamp_gamma(9999) == BALLUM_GAMMA_MAX);
    ballum_color_t c = ballum_color_rgba(1, 2, 3, 4);
    assert(c.r == 1 && c.g == 2 && c.b == 3 && c.a == 4);

    /* ---- init over the mock backend -------------------------------------- */
    ballum_manager_t m;
    assert(ballum_manager_init(&m, &g_mock, NULL) == BALLUM_STATUS_OK);

    /* Operating before enumerate: display is unknown. */
    assert(ballum_manager_set_pixel(&m, 100, (ballum_point_t){0, 0},
                                    ballum_color_rgb(255, 0, 0)) == BALLUM_STATUS_DISPLAY_NOT_FOUND);

    /* ---- enumerate (buffer-too-small then correct) ----------------------- */
    ballum_display_t one[1];
    size_t count = 0;
    assert(ballum_manager_enumerate(&m, one, 1, &count) == BALLUM_STATUS_BUFFER_TOO_SMALL);
    assert(count == 2);  /* reports the true count even when truncated */

    ballum_display_t ds[4];
    assert(ballum_manager_enumerate(&m, ds, 4, &count) == BALLUM_STATUS_OK);
    assert(count == 2);
    assert(ds[0].id == 100 && ds[0].is_primary == 1);

    ballum_display_t d;
    assert(ballum_manager_describe(&m, 100, &d) == BALLUM_STATUS_OK);
    assert(d.width == 1920 && d.height == 1080);
    assert(ballum_manager_describe(&m, 999, &d) == BALLUM_STATUS_DISPLAY_NOT_FOUND);

    /* ---- capability gating ---------------------------------------------- */
    assert(ballum_manager_has_capability(&m, 100, BALLUM_CAP_PER_PIXEL) == 1);
    assert(ballum_manager_has_capability(&m, 200, BALLUM_CAP_PER_PIXEL) == 0);
    assert(ballum_manager_has_capability(&m, 200, BALLUM_CAP_BRIGHTNESS) == 1);

    /* ---- per-pixel: bounds + dispatch ----------------------------------- */
    assert(ballum_manager_set_pixel(&m, 100, (ballum_point_t){10, 20},
                                    ballum_color_rgb(5, 6, 7)) == BALLUM_STATUS_OK);
    assert(g_set_pixel_calls == 1);
    assert(g_last_point.x == 10 && g_last_point.y == 20);
    assert(g_last_pixel.r == 5 && g_last_pixel.g == 6 && g_last_pixel.b == 7);

    /* Out of bounds is rejected before touching the backend. */
    assert(ballum_manager_set_pixel(&m, 100, (ballum_point_t){1920, 0},
                                    ballum_color_rgb(0, 0, 0)) == BALLUM_STATUS_OUT_OF_BOUNDS);
    assert(g_set_pixel_calls == 1);  /* backend NOT called */

    /* A display without the capability is rejected cleanly. */
    assert(ballum_manager_set_pixel(&m, 200, (ballum_point_t){0, 0},
                                    ballum_color_rgb(0, 0, 0)) == BALLUM_STATUS_UNSUPPORTED);

    ballum_color_t got;
    assert(ballum_manager_get_pixel(&m, 100, (ballum_point_t){1, 1}, &got) == BALLUM_STATUS_OK);
    assert(got.r == 10 && got.g == 20 && got.b == 30);

    /* ---- blit: size must match the rectangle ----------------------------- */
    ballum_color_t pix[4] = { {1,0,0,255}, {0,1,0,255}, {0,0,1,255}, {1,1,1,255} };
    assert(ballum_manager_blit_region(&m, 100, (ballum_rect_t){0, 0, 2, 2}, pix, 4) == BALLUM_STATUS_OK);
    assert(ballum_manager_blit_region(&m, 100, (ballum_rect_t){0, 0, 2, 2}, pix, 3) == BALLUM_STATUS_INVALID_ARGUMENT);
    assert(ballum_manager_blit_region(&m, 100, (ballum_rect_t){1919, 0, 2, 2}, pix, 4) == BALLUM_STATUS_OUT_OF_BOUNDS);

    /* ---- whole-display controls: clamped to the documented ranges -------- */
    assert(ballum_manager_set_brightness(&m, 100, 250) == BALLUM_STATUS_OK);
    assert(g_last_brightness == BALLUM_BRIGHTNESS_MAX);          /* clamped 100 */
    assert(ballum_manager_set_brightness(&m, 100, -10) == BALLUM_STATUS_OK);
    assert(g_last_brightness == BALLUM_BRIGHTNESS_MIN);          /* clamped 0 */

    assert(ballum_manager_set_warmth(&m, 100, 999999) == BALLUM_STATUS_OK);
    assert(g_last_warmth == BALLUM_WARMTH_MAX_KELVIN);
    assert(ballum_manager_set_warmth(&m, 100, 1) == BALLUM_STATUS_OK);
    assert(g_last_warmth == BALLUM_WARMTH_MIN_KELVIN);

    assert(ballum_manager_set_gamma(&m, 100, 220) == BALLUM_STATUS_OK);
    assert(g_last_gamma == 220);

    /* A brightness-only display rejects warmth/gamma it cannot do. */
    assert(ballum_manager_set_warmth(&m, 200, 5000) == BALLUM_STATUS_UNSUPPORTED);
    assert(ballum_manager_set_brightness(&m, 200, 50) == BALLUM_STATUS_OK);

    /* ---- pointer input: the manager polls and surfaces events ------------ */
    assert(ballum_manager_has_input(&m) == 1);
    ballum_pointer_event_t ev;
    assert(ballum_manager_poll_event(&m, &ev) == BALLUM_STATUS_OK);
    assert(ev.type == BALLUM_EVENT_POINTER_DOWN);
    assert(ev.x == 100 && ev.y == 100 && ev.button == BALLUM_BUTTON_LEFT);
    assert(ev.source == BALLUM_POINTER_MOUSE);
    assert(ballum_manager_poll_event(&m, &ev) == BALLUM_STATUS_OK);
    assert(ev.type == BALLUM_EVENT_POINTER_MOVE);
    assert(ev.x == 140 && ev.y == 100 && ev.source == BALLUM_POINTER_TOUCH && ev.contact_id == 7);
    assert(ballum_manager_poll_event(&m, &ev) == BALLUM_STATUS_OK);
    assert(ev.type == BALLUM_EVENT_NONE);          /* queue drained */
    assert(ballum_event_type_name(BALLUM_EVENT_POINTER_DOWN) != NULL);

    /* ---- keyboard bridge: Escape and subsequent key polling --------------- */
    assert(ballum_manager_has_keyboard(&m) == 1);
    int key = -1;
    assert(ballum_manager_poll_key(&m, &key) == BALLUM_STATUS_OK);
    assert(key == BALLUM_KEY_ESC);
    assert(ballum_manager_poll_key(&m, &key) == BALLUM_STATUS_OK);
    assert(key == BALLUM_KEY_NONE);
    assert(ballum_manager_poll_key(&m, &key) == BALLUM_STATUS_OK);
    assert(key == BALLUM_KEY_ENTER);

    /* ---- fullscreen surface (total per-pixel path) ----------------------- */
    /* Display 100 advertises the surface capability; 200 does not. */
    assert(ballum_manager_has_surface(&m, 100) == 1);
    assert(ballum_manager_has_surface(&m, 200) == 0);
    assert(ballum_manager_claim_surface(&m, 100) == BALLUM_STATUS_OK);
    assert(g_surface_claimed == 1);
    assert(ballum_manager_present(&m, 100) == BALLUM_STATUS_OK);
    assert(ballum_manager_present(&m, 100) == BALLUM_STATUS_OK);
    assert(g_present_calls == 2);
    assert(ballum_manager_release_surface(&m, 100) == BALLUM_STATUS_OK);
    assert(g_release_calls == 1 && g_surface_claimed == 0);
    /* A display without the capability is refused before the backend is hit. */
    assert(ballum_manager_claim_surface(&m, 200) == BALLUM_STATUS_UNSUPPORTED);

    /* ---- the unavailable fallback fails closed --------------------------- */
    ballum_manager_t u;
    assert(ballum_manager_init(&u, ballum_backend_unavailable(), NULL) == BALLUM_STATUS_OK);
    size_t uc = 0;
    assert(ballum_manager_enumerate(&u, ds, 4, &uc) == BALLUM_STATUS_BACKEND_UNAVAILABLE);
    assert(uc == 0);
    /* No-input path: the fallback has no poll, so has_input is 0 and poll
     * reports UNSUPPORTED while leaving out well-defined. */
    assert(ballum_manager_has_input(&u) == 0);
    ballum_pointer_event_t uev;
    assert(ballum_manager_poll_event(&u, &uev) == BALLUM_STATUS_UNSUPPORTED);
    assert(uev.type == BALLUM_EVENT_NONE);
    assert(ballum_manager_has_keyboard(&u) == 0);
    int no_key = -1;
    assert(ballum_manager_poll_key(&u, &no_key) == BALLUM_STATUS_UNSUPPORTED);
    assert(no_key == BALLUM_KEY_NONE);

    /* ---- status text is never NULL --------------------------------------- */
    assert(ballum_status_name(BALLUM_STATUS_OK) != NULL);
    assert(ballum_status_name(BALLUM_STATUS_UNSUPPORTED) != NULL);

    return 0;
}
