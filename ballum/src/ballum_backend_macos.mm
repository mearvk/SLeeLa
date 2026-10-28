/*
 * Ballum macOS backend -- Core Graphics / Quartz (open system frameworks).
 *
 * - enumerate       : CGGetActiveDisplayList -> displays + CGDisplayBounds.
 * - set/get_pixel   : a CGDisplay capture/draw path (per-pixel via a CGContext).
 * - blit_region     : draws a caller RGBA buffer into a display context.
 * - set_brightness  : scales the display gamma transfer (CGSetDisplayTransfer
 *                     ByFormula min/max), the portable Quartz brightness path.
 * - set_warmth      : per-channel gamma min offsets approximating a white point.
 * - set_gamma       : CGSetDisplayTransferByFormula gamma exponent.
 *
 * Only meaningful when built with BALLUM_BACKEND_MACOS (the Makefile adds
 * -framework CoreGraphics -framework ApplicationServices on macOS). Without the
 * define this compiles to an empty TU so non-mac builds never need the SDK.
 */

#include "sleela_ballum/ballum_manager.h"

#if defined(BALLUM_BACKEND_MACOS)

#import <ApplicationServices/ApplicationServices.h>
#include <math.h>
#include <string.h>

static ballum_status_t macos_enumerate(void *ctx, ballum_display_t *out,
                                        size_t capacity, size_t *out_count) {
    (void)ctx;
    if (out_count) *out_count = 0;
    CGDirectDisplayID ids[BALLUM_MAX_DISPLAYS];
    uint32_t n = 0;
    if (CGGetActiveDisplayList(BALLUM_MAX_DISPLAYS, ids, &n) != kCGErrorSuccess)
        return BALLUM_STATUS_BACKEND_ERROR;

    size_t count = 0;
    for (uint32_t i = 0; i < n && count < capacity && count < BALLUM_MAX_DISPLAYS; ++i) {
        CGRect b = CGDisplayBounds(ids[i]);
        ballum_display_t d;
        memset(&d, 0, sizeof(d));
        d.id = (uint32_t)ids[i];
        snprintf(d.name, sizeof(d.name), "display-%u", (unsigned)ids[i]);
        d.width = (uint32_t)b.size.width;
        d.height = (uint32_t)b.size.height;
        d.refresh_mhz = 0;
        d.is_primary = CGDisplayIsMain(ids[i]) ? 1 : 0;
        d.capabilities = BALLUM_CAP_PER_PIXEL | BALLUM_CAP_REGION_BLIT |
                         BALLUM_CAP_BRIGHTNESS | BALLUM_CAP_COLOR_TEMPERATURE |
                         BALLUM_CAP_GAMMA;
        out[count++] = d;
    }
    if (out_count) *out_count = count;
    return BALLUM_STATUS_OK;
}

/* Per-pixel drawing uses a one-pixel fill rect on the display's context. */
static ballum_status_t macos_set_pixel(void *ctx, uint32_t id,
                                       ballum_point_t at, ballum_color_t color) {
    (void)ctx;
    CGDirectDisplayID disp = (CGDirectDisplayID)id;
    CGContextRef c = CGDisplayGetDrawingContext(disp);
    if (!c) return BALLUM_STATUS_UNSUPPORTED;
    CGContextSetRGBFillColor(c, color.r / 255.0, color.g / 255.0,
                             color.b / 255.0, color.a / 255.0);
    CGContextFillRect(c, CGRectMake(at.x, at.y, 1, 1));
    CGContextFlush(c);
    return BALLUM_STATUS_OK;
}

static ballum_status_t macos_get_pixel(void *ctx, uint32_t id,
                                       ballum_point_t at, ballum_color_t *out) {
    (void)ctx; (void)id; (void)at;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    /* macOS 15 marks CGDisplayCreateImageForRect unavailable. ScreenCaptureKit
       is the supported replacement, but it requires an asynchronous capture
       session and user-granted screen-recording permission. Do not call the
       removed API or claim synchronous readback; drawing/blitting remain
       available through the Core Graphics backend. */
    return BALLUM_STATUS_UNSUPPORTED;
}

static ballum_status_t macos_blit(void *ctx, uint32_t id, ballum_rect_t rect,
                                  const ballum_color_t *pixels, size_t n) {
    (void)ctx;
    if (!pixels || n != (size_t)rect.width * rect.height) return BALLUM_STATUS_INVALID_ARGUMENT;
    CGDirectDisplayID disp = (CGDirectDisplayID)id;
    CGContextRef c = CGDisplayGetDrawingContext(disp);
    if (!c) return BALLUM_STATUS_UNSUPPORTED;
    for (uint32_t row = 0; row < rect.height; ++row) {
        for (uint32_t col = 0; col < rect.width; ++col) {
            const ballum_color_t px = pixels[(size_t)row * rect.width + col];
            CGContextSetRGBFillColor(c, px.r / 255.0, px.g / 255.0, px.b / 255.0, px.a / 255.0);
            CGContextFillRect(c, CGRectMake(rect.x + col, rect.y + row, 1, 1));
        }
    }
    CGContextFlush(c);
    return BALLUM_STATUS_OK;
}

static ballum_status_t macos_set_brightness(void *ctx, uint32_t id, int percent) {
    (void)ctx;
    CGDirectDisplayID disp = (CGDirectDisplayID)id;
    CGGammaValue m = (CGGammaValue)percent / 100.0f;
    /* Linear ramp scaled by brightness: min 0, max = m, gamma 1.0. */
    if (CGSetDisplayTransferByFormula(disp, 0, m, 1.0f, 0, m, 1.0f, 0, m, 1.0f) != kCGErrorSuccess)
        return BALLUM_STATUS_BACKEND_ERROR;
    return BALLUM_STATUS_OK;
}

static void warmth_multipliers(uint32_t kelvin, float *rm, float *gm, float *bm) {
    double t = (double)kelvin / 100.0;
    double r, g, b;
    if (t <= 66.0) r = 255.0; else r = 329.698727446 * pow(t - 60.0, -0.1332047592);
    if (t <= 66.0) g = 99.4708025861 * log(t) - 161.1195681661;
    else           g = 288.1221695283 * pow(t - 60.0, -0.0755148492);
    if (t >= 66.0) b = 255.0; else if (t <= 19.0) b = 0.0;
    else           b = 138.5177312231 * log(t - 10.0) - 305.0447927307;
    if (r < 0) { r = 0; } else if (r > 255) { r = 255; }
    if (g < 0) { g = 0; } else if (g > 255) { g = 255; }
    if (b < 0) { b = 0; } else if (b > 255) { b = 255; }
    *rm = (float)(r / 255.0); *gm = (float)(g / 255.0); *bm = (float)(b / 255.0);
}

static ballum_status_t macos_set_warmth(void *ctx, uint32_t id, uint32_t kelvin) {
    (void)ctx;
    CGDirectDisplayID disp = (CGDirectDisplayID)id;
    float rm, gm, bm; warmth_multipliers(kelvin, &rm, &gm, &bm);
    if (CGSetDisplayTransferByFormula(disp, 0, rm, 1.0f, 0, gm, 1.0f, 0, bm, 1.0f) != kCGErrorSuccess)
        return BALLUM_STATUS_BACKEND_ERROR;
    return BALLUM_STATUS_OK;
}

static ballum_status_t macos_set_gamma(void *ctx, uint32_t id, uint32_t gamma_scaled) {
    (void)ctx;
    CGDirectDisplayID disp = (CGDirectDisplayID)id;
    float gamma = (float)gamma_scaled / (float)BALLUM_GAMMA_SCALE;
    if (gamma <= 0.0f) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (CGSetDisplayTransferByFormula(disp, 0, 1.0f, gamma, 0, 1.0f, gamma, 0, 1.0f, gamma) != kCGErrorSuccess)
        return BALLUM_STATUS_BACKEND_ERROR;
    return BALLUM_STATUS_OK;
}

/* Whole-screen pointer capture requires a CGEventTap and Accessibility
 * permission, so pointer events remain unavailable here. Escape polling is a
 * separate, non-blocking Core Graphics key-state query. Key-down edges prevent
 * one held key from triggering both stages of SLBallumSession's exit flow. */
static ballum_status_t macos_poll_key(void *ctx, int *out) {
    static int was_escape_down;
    static int was_enter_down;
    static int was_space_down;
    int escape_down, enter_down, space_down;
    (void)ctx;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    *out = BALLUM_KEY_NONE;
    /* Virtual key codes: Escape=53, Return=36, Space=49. */
    escape_down = CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, 53);
    enter_down = CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, 36);
    space_down = CGEventSourceKeyState(kCGEventSourceStateCombinedSessionState, 49);
    if (escape_down && !was_escape_down) *out = BALLUM_KEY_ESC;
    else if (enter_down && !was_enter_down) *out = BALLUM_KEY_ENTER;
    else if (space_down && !was_space_down) *out = BALLUM_KEY_SPACE;
    was_escape_down = escape_down;
    was_enter_down = enter_down;
    was_space_down = space_down;
    return BALLUM_STATUS_OK;
}

static const ballum_backend_t k_macos = {
    "macos-coregraphics",
    macos_enumerate, macos_set_pixel, macos_get_pixel, macos_blit,
    macos_set_brightness, macos_set_warmth, macos_set_gamma, /* poll_event */ 0,
    macos_poll_key
};

const ballum_backend_t *ballum_backend_macos(void) { return &k_macos; }

#else  /* !BALLUM_BACKEND_MACOS */

typedef int ballum_macos_translation_unit_not_selected;

#endif /* BALLUM_BACKEND_MACOS */
