/*
 * Ballum Windows 10+ backend -- Win32 GDI + the device gamma ramp.
 *
 * - enumerate       : EnumDisplayMonitors -> displays + GetMonitorInfo bounds.
 * - set/get_pixel   : SetPixel / GetPixel on the screen DC.
 * - blit_region     : per-pixel SetPixel of a caller RGBA buffer.
 * - set_brightness  : scales the 256-entry GDI gamma ramp (SetDeviceGammaRamp),
 *                     the portable Win32 brightness path.
 * - set_warmth      : builds a per-channel gamma ramp for a white point.
 * - set_gamma       : exponent gamma ramp via SetDeviceGammaRamp.
 *
 * Only meaningful when built with BALLUM_BACKEND_WINDOWS (the Makefile adds
 * -lgdi32 -luser32 on Windows). Without the define this is an empty TU so
 * non-Windows builds never need the Win32 SDK.
 */

#include "sleela_ballum/ballum_manager.h"

#if defined(BALLUM_BACKEND_WINDOWS)

#include <windows.h>
#include <math.h>
#include <string.h>

/* EnumDisplayMonitors callback accumulator. */
typedef struct {
    ballum_display_t *out;
    size_t capacity;
    size_t count;
    uint32_t next_id;
} enum_ctx_t;

static BOOL CALLBACK monitor_cb(HMONITOR mon, HDC hdc, LPRECT rect, LPARAM lp) {
    (void)hdc; (void)rect;
    enum_ctx_t *e = (enum_ctx_t *)lp;
    if (e->count >= e->capacity || e->count >= BALLUM_MAX_DISPLAYS) return FALSE;
    MONITORINFOEXA mi; memset(&mi, 0, sizeof(mi)); mi.cbSize = sizeof(mi);
    if (!GetMonitorInfoA(mon, (MONITORINFO *)&mi)) return TRUE;
    ballum_display_t d; memset(&d, 0, sizeof(d));
    d.id = e->next_id++;
    strncpy(d.name, mi.szDevice, sizeof(d.name) - 1);
    d.width = (uint32_t)(mi.rcMonitor.right - mi.rcMonitor.left);
    d.height = (uint32_t)(mi.rcMonitor.bottom - mi.rcMonitor.top);
    d.refresh_mhz = 0;
    d.is_primary = (mi.dwFlags & MONITORINFOF_PRIMARY) ? 1 : 0;
    d.capabilities = BALLUM_CAP_PER_PIXEL | BALLUM_CAP_REGION_BLIT |
                     BALLUM_CAP_BRIGHTNESS | BALLUM_CAP_COLOR_TEMPERATURE |
                     BALLUM_CAP_GAMMA;
    e->out[e->count++] = d;
    return TRUE;
}

static ballum_status_t win_enumerate(void *ctx, ballum_display_t *out,
                                     size_t capacity, size_t *out_count) {
    (void)ctx;
    enum_ctx_t e; e.out = out; e.capacity = capacity; e.count = 0; e.next_id = 1;
    EnumDisplayMonitors(NULL, NULL, monitor_cb, (LPARAM)&e);
    if (out_count) *out_count = e.count;
    return BALLUM_STATUS_OK;
}

static ballum_status_t win_set_pixel(void *ctx, uint32_t id,
                                     ballum_point_t at, ballum_color_t color) {
    (void)ctx; (void)id;
    HDC dc = GetDC(NULL);
    if (!dc) return BALLUM_STATUS_BACKEND_ERROR;
    SetPixel(dc, (int)at.x, (int)at.y, RGB(color.r, color.g, color.b));
    ReleaseDC(NULL, dc);
    return BALLUM_STATUS_OK;
}

static ballum_status_t win_get_pixel(void *ctx, uint32_t id,
                                     ballum_point_t at, ballum_color_t *out) {
    (void)ctx; (void)id;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    HDC dc = GetDC(NULL);
    if (!dc) return BALLUM_STATUS_BACKEND_ERROR;
    COLORREF c = GetPixel(dc, (int)at.x, (int)at.y);
    ReleaseDC(NULL, dc);
    if (c == CLR_INVALID) return BALLUM_STATUS_OUT_OF_BOUNDS;
    out->r = GetRValue(c); out->g = GetGValue(c); out->b = GetBValue(c); out->a = 255;
    return BALLUM_STATUS_OK;
}

static ballum_status_t win_blit(void *ctx, uint32_t id, ballum_rect_t rect,
                                const ballum_color_t *pixels, size_t n) {
    (void)ctx; (void)id;
    if (!pixels || n != (size_t)rect.width * rect.height) return BALLUM_STATUS_INVALID_ARGUMENT;
    HDC dc = GetDC(NULL);
    if (!dc) return BALLUM_STATUS_BACKEND_ERROR;
    for (uint32_t row = 0; row < rect.height; ++row)
        for (uint32_t col = 0; col < rect.width; ++col) {
            const ballum_color_t p = pixels[(size_t)row * rect.width + col];
            SetPixel(dc, (int)(rect.x + col), (int)(rect.y + row), RGB(p.r, p.g, p.b));
        }
    ReleaseDC(NULL, dc);
    return BALLUM_STATUS_OK;
}

/* Build and set a 256x3 WORD gamma ramp from per-channel multipliers + exponent. */
static ballum_status_t apply_ramp(double rmul, double gmul, double bmul, double exponent) {
    HDC dc = GetDC(NULL);
    if (!dc) return BALLUM_STATUS_BACKEND_ERROR;
    WORD ramp[3][256];
    for (int i = 0; i < 256; ++i) {
        double t = (double)i / 255.0;
        double v = pow(t, exponent);
        double rv = v * rmul; if (rv > 1.0) rv = 1.0;
        double gv = v * gmul; if (gv > 1.0) gv = 1.0;
        double bv = v * bmul; if (bv > 1.0) bv = 1.0;
        ramp[0][i] = (WORD)(rv * 65535.0);
        ramp[1][i] = (WORD)(gv * 65535.0);
        ramp[2][i] = (WORD)(bv * 65535.0);
    }
    BOOL ok = SetDeviceGammaRamp(dc, ramp);
    ReleaseDC(NULL, dc);
    return ok ? BALLUM_STATUS_OK : BALLUM_STATUS_UNSUPPORTED;
}

static ballum_status_t win_set_brightness(void *ctx, uint32_t id, int percent) {
    (void)ctx; (void)id;
    double m = (double)percent / 100.0;
    return apply_ramp(m, m, m, 1.0);
}

static void warmth_multipliers(uint32_t kelvin, double *rm, double *gm, double *bm) {
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
    *rm = r / 255.0; *gm = g / 255.0; *bm = b / 255.0;
}

static ballum_status_t win_set_warmth(void *ctx, uint32_t id, uint32_t kelvin) {
    (void)ctx; (void)id;
    double rm, gm, bm; warmth_multipliers(kelvin, &rm, &gm, &bm);
    return apply_ramp(rm, gm, bm, 1.0);
}

static ballum_status_t win_set_gamma(void *ctx, uint32_t id, uint32_t gamma_scaled) {
    (void)ctx; (void)id;
    double gamma = (double)gamma_scaled / (double)BALLUM_GAMMA_SCALE;
    if (gamma <= 0.0) return BALLUM_STATUS_INVALID_ARGUMENT;
    return apply_ramp(1.0, 1.0, 1.0, 1.0 / gamma);
}

/* Whole-screen global pointer capture on Windows uses a low-level mouse hook
 * with its own message-pump lifecycle; it remains intentionally unavailable.
 * Keyboard exit is independent and uses non-blocking Win32 key-state polling. */
static ballum_status_t win_poll_key(void *ctx, int *out) {
    static int was_escape_down;
    static int was_enter_down;
    static int was_space_down;
    int escape_down, enter_down, space_down;
    (void)ctx;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    *out = BALLUM_KEY_NONE;
    escape_down = (GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0;
    enter_down = (GetAsyncKeyState(VK_RETURN) & 0x8000) != 0;
    space_down = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
    if (escape_down && !was_escape_down) *out = BALLUM_KEY_ESC;
    else if (enter_down && !was_enter_down) *out = BALLUM_KEY_ENTER;
    else if (space_down && !was_space_down) *out = BALLUM_KEY_SPACE;
    was_escape_down = escape_down;
    was_enter_down = enter_down;
    was_space_down = space_down;
    return BALLUM_STATUS_OK;
}

static const ballum_backend_t k_windows = {
    "windows-gdi",
    win_enumerate, win_set_pixel, win_get_pixel, win_blit,
    win_set_brightness, win_set_warmth, win_set_gamma, /* poll_event */ 0,
    win_poll_key
};

const ballum_backend_t *ballum_backend_windows(void) { return &k_windows; }

#else  /* !BALLUM_BACKEND_WINDOWS */

typedef int ballum_windows_translation_unit_not_selected;

#endif /* BALLUM_BACKEND_WINDOWS */
