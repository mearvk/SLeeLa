/*
 * Ballum Linux backend -- X11 + XRANDR (open standards only).
 *
 * - enumerate       : XRANDR active outputs/CRTCs -> displays + modes.
 * - set/get_pixel   : X11 XPutImage / XGetImage against the root window.
 * - blit_region     : XPutImage of a caller RGBA buffer.
 * - set_brightness  : scales the CRTC gamma ramp (a linear multiplier), the
 *                     portable X11 path; a DDC/CI backlight path can refine it.
 * - set_warmth      : builds a per-channel gamma ramp approximating a black-body
 *                     white point at the requested kelvin (XRRSetCrtcGamma).
 * - set_gamma       : sets an exponent gamma ramp on the CRTC.
 *
 * This file is only meaningful when built with BALLUM_BACKEND_X11 (the Makefile
 * adds -lX11 -lXrandr on Linux). Without that define it compiles to an empty
 * translation unit, so the standalone unit-test build (which links the
 * unavailable backend) does not need X11 development headers.
 */

#include "sleela_ballum/ballum_manager.h"

#if defined(BALLUM_BACKEND_X11)

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <math.h>
#include <string.h>

typedef struct {
    Display *dpy;
    Window   root;
    int      screen;
} x11_ctx_t;

static x11_ctx_t g_ctx;

static Display *ensure_display(void) {
    if (!g_ctx.dpy) {
        g_ctx.dpy = XOpenDisplay(NULL);
        if (g_ctx.dpy) {
            g_ctx.screen = DefaultScreen(g_ctx.dpy);
            g_ctx.root = RootWindow(g_ctx.dpy, g_ctx.screen);
        }
    }
    return g_ctx.dpy;
}

static ballum_status_t x11_enumerate(void *ctx, ballum_display_t *out,
                                     size_t capacity, size_t *out_count) {
    (void)ctx;
    if (out_count) *out_count = 0;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;

    XRRScreenResources *res = XRRGetScreenResources(dpy, g_ctx.root);
    if (!res) return BALLUM_STATUS_BACKEND_ERROR;

    RROutput primary = XRRGetOutputPrimary(dpy, g_ctx.root);
    size_t count = 0;
    for (int i = 0; i < res->noutput && count < capacity && count < BALLUM_MAX_DISPLAYS; ++i) {
        XRROutputInfo *oi = XRRGetOutputInfo(dpy, res, res->outputs[i]);
        if (oi && oi->connection == RR_Connected && oi->crtc) {
            XRRCrtcInfo *ci = XRRGetCrtcInfo(dpy, res, oi->crtc);
            if (ci) {
                ballum_display_t d;
                memset(&d, 0, sizeof(d));
                d.id = (uint32_t)res->outputs[i];
                strncpy(d.name, oi->name ? oi->name : "output", sizeof(d.name) - 1);
                d.width = ci->width;
                d.height = ci->height;
                d.refresh_mhz = 0;
                d.is_primary = (res->outputs[i] == primary) ? 1 : 0;
                d.capabilities = BALLUM_CAP_PER_PIXEL | BALLUM_CAP_REGION_BLIT |
                                 BALLUM_CAP_BRIGHTNESS | BALLUM_CAP_COLOR_TEMPERATURE |
                                 BALLUM_CAP_GAMMA | BALLUM_CAP_POINTER_INPUT;
                out[count++] = d;
                XRRFreeCrtcInfo(ci);
            }
        }
        if (oi) XRRFreeOutputInfo(oi);
    }
    XRRFreeScreenResources(res);
    if (out_count) *out_count = count;
    return BALLUM_STATUS_OK;
}

static ballum_status_t x11_set_pixel(void *ctx, uint32_t id,
                                     ballum_point_t at, ballum_color_t color) {
    (void)ctx; (void)id;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    GC gc = DefaultGC(dpy, g_ctx.screen);
    unsigned long px = ((unsigned long)color.r << 16) |
                       ((unsigned long)color.g << 8) | (unsigned long)color.b;
    XSetForeground(dpy, gc, px);
    XDrawPoint(dpy, g_ctx.root, gc, (int)at.x, (int)at.y);
    XFlush(dpy);
    return BALLUM_STATUS_OK;
}

static ballum_status_t x11_get_pixel(void *ctx, uint32_t id,
                                     ballum_point_t at, ballum_color_t *out) {
    (void)ctx; (void)id;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    XImage *img = XGetImage(dpy, g_ctx.root, (int)at.x, (int)at.y, 1, 1, AllPlanes, ZPixmap);
    if (!img) return BALLUM_STATUS_BACKEND_ERROR;
    unsigned long px = XGetPixel(img, 0, 0);
    out->r = (uint8_t)((px >> 16) & 0xFF);
    out->g = (uint8_t)((px >> 8) & 0xFF);
    out->b = (uint8_t)(px & 0xFF);
    out->a = 255;
    XDestroyImage(img);
    return BALLUM_STATUS_OK;
}

static ballum_status_t x11_blit(void *ctx, uint32_t id, ballum_rect_t rect,
                                const ballum_color_t *pixels, size_t n) {
    (void)ctx; (void)id;
    if (!pixels || n != (size_t)rect.width * rect.height) return BALLUM_STATUS_INVALID_ARGUMENT;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    GC gc = DefaultGC(dpy, g_ctx.screen);
    for (uint32_t row = 0; row < rect.height; ++row) {
        for (uint32_t col = 0; col < rect.width; ++col) {
            const ballum_color_t c = pixels[(size_t)row * rect.width + col];
            unsigned long px = ((unsigned long)c.r << 16) |
                               ((unsigned long)c.g << 8) | (unsigned long)c.b;
            XSetForeground(dpy, gc, px);
            XDrawPoint(dpy, g_ctx.root, gc, (int)(rect.x + col), (int)(rect.y + row));
        }
    }
    XFlush(dpy);
    return BALLUM_STATUS_OK;
}

/* Find the CRTC driving output `id`, and its gamma ramp size. */
static RRCrtc crtc_for_output(Display *dpy, XRRScreenResources *res, uint32_t id) {
    for (int i = 0; i < res->noutput; ++i) {
        if ((uint32_t)res->outputs[i] != id) continue;
        XRROutputInfo *oi = XRRGetOutputInfo(dpy, res, res->outputs[i]);
        RRCrtc crtc = oi ? oi->crtc : 0;
        if (oi) XRRFreeOutputInfo(oi);
        return crtc;
    }
    return 0;
}

/* Apply a gamma ramp built by `fill` (per channel, normalized 0..1 input). */
static ballum_status_t apply_ramp(uint32_t id,
                                  double rmul, double gmul, double bmul, double exponent) {
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    XRRScreenResources *res = XRRGetScreenResources(dpy, g_ctx.root);
    if (!res) return BALLUM_STATUS_BACKEND_ERROR;
    RRCrtc crtc = crtc_for_output(dpy, res, id);
    if (!crtc) { XRRFreeScreenResources(res); return BALLUM_STATUS_DISPLAY_NOT_FOUND; }

    int size = XRRGetCrtcGammaSize(dpy, crtc);
    if (size <= 0) { XRRFreeScreenResources(res); return BALLUM_STATUS_UNSUPPORTED; }
    XRRCrtcGamma *g = XRRAllocGamma(size);
    if (!g) { XRRFreeScreenResources(res); return BALLUM_STATUS_BACKEND_ERROR; }

    for (int i = 0; i < size; ++i) {
        double t = (double)i / (double)(size - 1);
        double v = pow(t, exponent);
        unsigned short rv = (unsigned short)(fmin(1.0, v * rmul) * 65535.0);
        unsigned short gv = (unsigned short)(fmin(1.0, v * gmul) * 65535.0);
        unsigned short bv = (unsigned short)(fmin(1.0, v * bmul) * 65535.0);
        g->red[i] = rv; g->green[i] = gv; g->blue[i] = bv;
    }
    XRRSetCrtcGamma(dpy, crtc, g);
    XRRFreeGamma(g);
    XRRFreeScreenResources(res);
    XFlush(dpy);
    return BALLUM_STATUS_OK;
}

static ballum_status_t x11_set_brightness(void *ctx, uint32_t id, int percent) {
    (void)ctx;
    double m = (double)percent / 100.0;
    return apply_ramp(id, m, m, m, 1.0);  /* linear scale on all channels */
}

/* Approximate black-body channel multipliers for a color temperature (kelvin).
 * A compact approximation sufficient for a warmth control. */
static void warmth_multipliers(uint32_t kelvin, double *rm, double *gm, double *bm) {
    double t = (double)kelvin / 100.0;
    double r, g, b;
    if (t <= 66.0) { r = 255.0; }
    else { r = 329.698727446 * pow(t - 60.0, -0.1332047592); }
    if (t <= 66.0) { g = 99.4708025861 * log(t) - 161.1195681661; }
    else { g = 288.1221695283 * pow(t - 60.0, -0.0755148492); }
    if (t >= 66.0) { b = 255.0; }
    else if (t <= 19.0) { b = 0.0; }
    else { b = 138.5177312231 * log(t - 10.0) - 305.0447927307; }
    if (r < 0) { r = 0; } else if (r > 255) { r = 255; }
    if (g < 0) { g = 0; } else if (g > 255) { g = 255; }
    if (b < 0) { b = 0; } else if (b > 255) { b = 255; }
    *rm = r / 255.0; *gm = g / 255.0; *bm = b / 255.0;
}

static ballum_status_t x11_set_warmth(void *ctx, uint32_t id, uint32_t kelvin) {
    (void)ctx;
    double rm, gm, bm;
    warmth_multipliers(kelvin, &rm, &gm, &bm);
    return apply_ramp(id, rm, gm, bm, 1.0);
}

static ballum_status_t x11_set_gamma(void *ctx, uint32_t id, uint32_t gamma_scaled) {
    (void)ctx;
    double gamma = (double)gamma_scaled / (double)BALLUM_GAMMA_SCALE;
    if (gamma <= 0.0) return BALLUM_STATUS_INVALID_ARGUMENT;
    return apply_ramp(id, 1.0, 1.0, 1.0, 1.0 / gamma);
}

/* Pointer input: select button/motion events on the root window (once), then
 * drain one queued event per poll. Mouse buttons 1/2/3 map to LEFT/MIDDLE/RIGHT;
 * X11 core input does not distinguish touch, so contacts arrive as mouse. */
static int g_input_selected;

static ballum_pointer_button_t map_button(unsigned int xbutton) {
    if (xbutton == Button1) return BALLUM_BUTTON_LEFT;
    if (xbutton == Button2) return BALLUM_BUTTON_MIDDLE;
    if (xbutton == Button3) return BALLUM_BUTTON_RIGHT;
    return BALLUM_BUTTON_NONE;
}

static uint32_t map_mods(unsigned int state) {
    uint32_t m = 0;
    if (state & ShiftMask)   m |= BALLUM_MOD_SHIFT;
    if (state & ControlMask) m |= BALLUM_MOD_CTRL;
    if (state & Mod1Mask)    m |= BALLUM_MOD_ALT;
    if (state & Mod4Mask)    m |= BALLUM_MOD_SUPER;
    return m;
}

static ballum_status_t x11_poll_event(void *ctx, ballum_pointer_event_t *out) {
    (void)ctx;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    out->type = BALLUM_EVENT_NONE;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;

    if (!g_input_selected) {
        XSelectInput(dpy, g_ctx.root,
                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
        g_input_selected = 1;
    }

    if (XPending(dpy) <= 0) return BALLUM_STATUS_OK;   /* none pending -> NONE */

    XEvent ev;
    XNextEvent(dpy, &ev);
    out->source = BALLUM_POINTER_MOUSE;
    out->contact_id = 0;
    if (ev.type == ButtonPress) {
        out->type = BALLUM_EVENT_POINTER_DOWN;
        out->x = ev.xbutton.x_root; out->y = ev.xbutton.y_root;
        out->button = map_button(ev.xbutton.button);
        out->modifiers = map_mods(ev.xbutton.state);
    } else if (ev.type == ButtonRelease) {
        out->type = BALLUM_EVENT_POINTER_UP;
        out->x = ev.xbutton.x_root; out->y = ev.xbutton.y_root;
        out->button = map_button(ev.xbutton.button);
        out->modifiers = map_mods(ev.xbutton.state);
    } else if (ev.type == MotionNotify) {
        out->type = BALLUM_EVENT_POINTER_MOVE;
        out->x = ev.xmotion.x_root; out->y = ev.xmotion.y_root;
        out->button = BALLUM_BUTTON_NONE;
        out->modifiers = map_mods(ev.xmotion.state);
    }
    return BALLUM_STATUS_OK;
}

static const ballum_backend_t k_x11 = {
    "x11-xrandr",
    x11_enumerate, x11_set_pixel, x11_get_pixel, x11_blit,
    x11_set_brightness, x11_set_warmth, x11_set_gamma, x11_poll_event,
    /* poll_key: the X11 pointer path does not yet feed keys through Ballum, so
     * NULL -> has_keyboard()==0 and poll_key returns UNSUPPORTED (fail closed).
     * Mapping XKeyEvent keysyms onto ballum_key_t is the native follow-up. */
    0
};

const ballum_backend_t *ballum_backend_x11(void) { return &k_x11; }

#else  /* !BALLUM_BACKEND_X11 -- empty TU so the file is always compilable */

typedef int ballum_x11_translation_unit_not_selected;

#endif /* BALLUM_BACKEND_X11 */
