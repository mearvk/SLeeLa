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
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* MIT-SHM is optional: when the headers and server support it, present() uses
 * a shared-memory XImage so the whole framebuffer crosses to the X server
 * without a socket copy (the fastest portable X11 path). Falls back to a plain
 * XPutImage otherwise. Guarded so a build without the extension still compiles. */
#if defined(BALLUM_X11_USE_SHM)
#  include <sys/ipc.h>
#  include <sys/shm.h>
#  include <X11/extensions/XShm.h>
#endif

typedef struct {
    Display *dpy;
    Window   root;
    int      screen;

    /* Fullscreen surface state (one at a time; keyed by the owning output). */
    int      surface_active;
    uint32_t surface_output;      /* RROutput id that owns the surface   */
    Window   surface_win;         /* override-redirect fullscreen window */
    GC       surface_gc;
    int      surface_x, surface_y;/* surface origin in root coords       */
    uint32_t surface_w, surface_h;/* surface size in pixels              */
    uint32_t *fb;                 /* framebuffer: surface_w*surface_h px, 0xAARRGGBB */
    XImage  *ximg;                /* reusable image over fb (or SHM)     */
    int      depth;
#if defined(BALLUM_X11_USE_SHM)
    XShmSegmentInfo shm;
    int      shm_active;
#endif
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

/* Is the given point inside the active surface, and where in its framebuffer? */
static int surface_index(int32_t x, int32_t y, size_t *idx_out) {
    if (!g_ctx.surface_active || !g_ctx.fb) return 0;
    int lx = x - g_ctx.surface_x;
    int ly = y - g_ctx.surface_y;
    if (lx < 0 || ly < 0 || (uint32_t)lx >= g_ctx.surface_w ||
        (uint32_t)ly >= g_ctx.surface_h) return 0;
    if (idx_out) *idx_out = (size_t)ly * g_ctx.surface_w + (size_t)lx;
    return 1;
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
                                 BALLUM_CAP_GAMMA | BALLUM_CAP_POINTER_INPUT |
                                 BALLUM_CAP_FULLSCREEN_SURFACE;
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

/* Pack an RGBA color into the framebuffer's 0xAARRGGBB word. */
static uint32_t pack_argb(ballum_color_t c) {
    return ((uint32_t)c.a << 24) | ((uint32_t)c.r << 16) |
           ((uint32_t)c.g << 8)  |  (uint32_t)c.b;
}

static ballum_status_t x11_set_pixel(void *ctx, uint32_t id,
                                     ballum_point_t at, ballum_color_t color) {
    (void)ctx; (void)id;
    /* Fast path: a claimed surface writes to the in-memory framebuffer in O(1)
     * with no X round-trip; present() pushes it later in one shot. */
    if (g_ctx.surface_active) {
        size_t idx;
        if (!surface_index((int32_t)at.x, (int32_t)at.y, &idx))
            return BALLUM_STATUS_OUT_OF_BOUNDS;
        g_ctx.fb[idx] = pack_argb(color);
        return BALLUM_STATUS_OK;
    }
    /* Fallback: draw on the live desktop (transient; a compositor may repaint). */
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
    if (g_ctx.surface_active) {
        size_t idx;
        if (!surface_index((int32_t)at.x, (int32_t)at.y, &idx))
            return BALLUM_STATUS_OUT_OF_BOUNDS;
        uint32_t px = g_ctx.fb[idx];
        out->a = (uint8_t)((px >> 24) & 0xFF);
        out->r = (uint8_t)((px >> 16) & 0xFF);
        out->g = (uint8_t)((px >> 8) & 0xFF);
        out->b = (uint8_t)(px & 0xFF);
        return BALLUM_STATUS_OK;
    }
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
    if (!pixels || n != (size_t)rect.width * (size_t)rect.height)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    /* World-Class path: with a claimed surface, a blit is a row-wise memcpy
     * into the framebuffer -- no X calls at all until present(). A full-screen
     * frame is O(pixels) memory writes, not millions of XDrawPoint round-trips. */
    if (g_ctx.surface_active) {
        for (uint32_t row = 0; row < rect.height; ++row) {
            int ly = (int)rect.y + (int)row - g_ctx.surface_y;
            if (ly < 0 || (uint32_t)ly >= g_ctx.surface_h) continue;
            for (uint32_t col = 0; col < rect.width; ++col) {
                int lx = (int)rect.x + (int)col - g_ctx.surface_x;
                if (lx < 0 || (uint32_t)lx >= g_ctx.surface_w) continue;
                g_ctx.fb[(size_t)ly * g_ctx.surface_w + (size_t)lx] =
                    pack_argb(pixels[(size_t)row * rect.width + col]);
            }
        }
        return BALLUM_STATUS_OK;
    }
    /* Fallback: build ONE XImage and push it with a single XPutImage, instead
     * of the old per-pixel XDrawPoint loop (the README's intended behavior). */
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    int depth = DefaultDepth(dpy, g_ctx.screen);
    Visual *vis = DefaultVisual(dpy, g_ctx.screen);
    uint32_t *buf = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!buf) return BALLUM_STATUS_BACKEND_ERROR;
    for (size_t i = 0; i < n; ++i) buf[i] = pack_argb(pixels[i]);
    XImage *img = XCreateImage(dpy, vis, (unsigned)depth, ZPixmap, 0,
                               (char *)buf, rect.width, rect.height, 32, 0);
    if (!img) { free(buf); return BALLUM_STATUS_BACKEND_ERROR; }
    GC gc = DefaultGC(dpy, g_ctx.screen);
    XPutImage(dpy, g_ctx.root, gc, img, 0, 0,
              (int)rect.x, (int)rect.y, rect.width, rect.height);
    XFlush(dpy);
    img->data = NULL;        /* we free buf ourselves */
    XDestroyImage(img);
    free(buf);
    return BALLUM_STATUS_OK;
}

/* ---- fullscreen surface (total per-pixel control) ------------------------ */

/* Look up the CRTC geometry (origin + size) that drives output `id`. */
static int output_geometry(Display *dpy, uint32_t id,
                           int *ox, int *oy, uint32_t *ow, uint32_t *oh) {
    XRRScreenResources *res = XRRGetScreenResources(dpy, g_ctx.root);
    if (!res) return 0;
    int found = 0;
    for (int i = 0; i < res->noutput; ++i) {
        if ((uint32_t)res->outputs[i] != id) continue;
        XRROutputInfo *oi = XRRGetOutputInfo(dpy, res, res->outputs[i]);
        if (oi && oi->crtc) {
            XRRCrtcInfo *ci = XRRGetCrtcInfo(dpy, res, oi->crtc);
            if (ci) {
                *ox = ci->x; *oy = ci->y; *ow = ci->width; *oh = ci->height;
                found = 1;
                XRRFreeCrtcInfo(ci);
            }
        }
        if (oi) XRRFreeOutputInfo(oi);
        break;
    }
    XRRFreeScreenResources(res);
    return found;
}

static void surface_teardown(Display *dpy) {
#if defined(BALLUM_X11_USE_SHM)
    if (g_ctx.shm_active) {
        XShmDetach(dpy, &g_ctx.shm);
        if (g_ctx.ximg) { g_ctx.ximg->data = NULL; XDestroyImage(g_ctx.ximg); g_ctx.ximg = NULL; }
        if (g_ctx.shm.shmaddr) shmdt(g_ctx.shm.shmaddr);
        if (g_ctx.shm.shmid != -1) shmctl(g_ctx.shm.shmid, IPC_RMID, NULL);
        g_ctx.shm_active = 0;
        g_ctx.fb = NULL;  /* fb was the SHM segment */
    }
#endif
    if (g_ctx.ximg) { g_ctx.ximg->data = NULL; XDestroyImage(g_ctx.ximg); g_ctx.ximg = NULL; }
    if (g_ctx.fb) { free(g_ctx.fb); g_ctx.fb = NULL; }
    if (g_ctx.surface_gc) { XFreeGC(dpy, g_ctx.surface_gc); g_ctx.surface_gc = 0; }
    if (g_ctx.surface_win) { XDestroyWindow(dpy, g_ctx.surface_win); g_ctx.surface_win = 0; }
    g_ctx.surface_active = 0;
    g_ctx.surface_w = g_ctx.surface_h = 0;
}

static ballum_status_t x11_claim_surface(void *ctx, uint32_t id) {
    (void)ctx;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    if (g_ctx.surface_active) surface_teardown(dpy);

    int ox = 0, oy = 0; uint32_t ow = 0, oh = 0;
    if (!output_geometry(dpy, id, &ox, &oy, &ow, &oh) || ow == 0 || oh == 0)
        return BALLUM_STATUS_DISPLAY_NOT_FOUND;

    int depth = DefaultDepth(dpy, g_ctx.screen);
    Visual *vis = DefaultVisual(dpy, g_ctx.screen);

    /* Override-redirect window: the window manager/compositor leaves it alone,
     * so the program truly owns every pixel of the surface. */
    XSetWindowAttributes swa;
    memset(&swa, 0, sizeof(swa));
    swa.override_redirect = True;
    swa.background_pixel = BlackPixel(dpy, g_ctx.screen);
    swa.event_mask = ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask;
    Window win = XCreateWindow(dpy, g_ctx.root, ox, oy, ow, oh, 0,
                               depth, InputOutput, vis,
                               CWOverrideRedirect | CWBackPixel | CWEventMask, &swa);
    if (!win) return BALLUM_STATUS_BACKEND_ERROR;
    XMapRaised(dpy, win);

    g_ctx.surface_win = win;
    g_ctx.surface_gc = XCreateGC(dpy, win, 0, NULL);
    g_ctx.surface_x = ox; g_ctx.surface_y = oy;
    g_ctx.surface_w = ow; g_ctx.surface_h = oh;
    g_ctx.surface_output = id;
    g_ctx.depth = depth;
    size_t px = (size_t)ow * (size_t)oh;

#if defined(BALLUM_X11_USE_SHM)
    if (XShmQueryExtension(dpy)) {
        memset(&g_ctx.shm, 0, sizeof(g_ctx.shm));
        g_ctx.shm.shmid = -1;
        g_ctx.ximg = XShmCreateImage(dpy, vis, (unsigned)depth, ZPixmap, NULL,
                                     &g_ctx.shm, ow, oh);
        if (g_ctx.ximg) {
            g_ctx.shm.shmid = shmget(IPC_PRIVATE,
                                     (size_t)g_ctx.ximg->bytes_per_line * oh, IPC_CREAT | 0600);
            if (g_ctx.shm.shmid != -1) {
                g_ctx.shm.shmaddr = (char *)shmat(g_ctx.shm.shmid, NULL, 0);
                if (g_ctx.shm.shmaddr != (char *)-1) {
                    g_ctx.ximg->data = g_ctx.shm.shmaddr;
                    g_ctx.shm.readOnly = False;
                    if (XShmAttach(dpy, &g_ctx.shm)) {
                        g_ctx.fb = (uint32_t *)g_ctx.shm.shmaddr;
                        g_ctx.shm_active = 1;
                    }
                }
            }
        }
        if (!g_ctx.shm_active && g_ctx.ximg) { XDestroyImage(g_ctx.ximg); g_ctx.ximg = NULL; }
    }
#endif
    if (!g_ctx.fb) {
        /* Plain-XImage path: our own heap buffer, pushed with XPutImage. */
        g_ctx.fb = (uint32_t *)calloc(px, sizeof(uint32_t));
        if (!g_ctx.fb) { surface_teardown(dpy); return BALLUM_STATUS_BACKEND_ERROR; }
        g_ctx.ximg = XCreateImage(dpy, vis, (unsigned)depth, ZPixmap, 0,
                                  (char *)g_ctx.fb, ow, oh, 32, 0);
        if (!g_ctx.ximg) { surface_teardown(dpy); return BALLUM_STATUS_BACKEND_ERROR; }
    }

    g_ctx.surface_active = 1;
    XFlush(dpy);
    return BALLUM_STATUS_OK;
}

static ballum_status_t x11_present(void *ctx, uint32_t id) {
    (void)ctx; (void)id;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    if (!g_ctx.surface_active || !g_ctx.ximg) return BALLUM_STATUS_UNSUPPORTED;
#if defined(BALLUM_X11_USE_SHM)
    if (g_ctx.shm_active) {
        XShmPutImage(dpy, g_ctx.surface_win, g_ctx.surface_gc, g_ctx.ximg,
                     0, 0, 0, 0, g_ctx.surface_w, g_ctx.surface_h, False);
        XFlush(dpy);
        return BALLUM_STATUS_OK;
    }
#endif
    XPutImage(dpy, g_ctx.surface_win, g_ctx.surface_gc, g_ctx.ximg,
              0, 0, 0, 0, g_ctx.surface_w, g_ctx.surface_h);
    XFlush(dpy);
    return BALLUM_STATUS_OK;
}

static ballum_status_t x11_release_surface(void *ctx, uint32_t id) {
    (void)ctx; (void)id;
    Display *dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    if (!g_ctx.surface_active) return BALLUM_STATUS_OK;
    surface_teardown(dpy);
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

/* Non-blocking global key-state polling. XQueryKeymap reports physical key
 * state for the active X server even when the application has no focused
 * window. Return only key-down edges so holding ESC does not skip both stages
 * of SLBallumSession's RUNNING -> FRAME -> EXIT transition. */
static ballum_status_t x11_poll_key(void *ctx, int *out) {
    static int was_escape_down;
    static int was_enter_down;
    static int was_space_down;
    char keys[32];
    Display *dpy;
    KeyCode esc, enter, space;
    int esc_down, enter_down, space_down;
    (void)ctx;
    if (!out) return BALLUM_STATUS_INVALID_ARGUMENT;
    *out = BALLUM_KEY_NONE;
    dpy = ensure_display();
    if (!dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    esc = XKeysymToKeycode(dpy, XK_Escape);
    enter = XKeysymToKeycode(dpy, XK_Return);
    space = XKeysymToKeycode(dpy, XK_space);
    if (!esc) return BALLUM_STATUS_UNSUPPORTED;
    XQueryKeymap(dpy, keys);
    esc_down = (((unsigned char)keys[esc >> 3] & (1u << (esc & 7))) != 0);
    enter_down = enter && (((unsigned char)keys[enter >> 3] & (1u << (enter & 7))) != 0);
    space_down = space && (((unsigned char)keys[space >> 3] & (1u << (space & 7))) != 0);
    if (esc_down && !was_escape_down) *out = BALLUM_KEY_ESC;
    else if (enter_down && !was_enter_down) *out = BALLUM_KEY_ENTER;
    else if (space_down && !was_space_down) *out = BALLUM_KEY_SPACE;
    was_escape_down = esc_down;
    was_enter_down = enter_down;
    was_space_down = space_down;
    return BALLUM_STATUS_OK;
}

static const ballum_backend_t k_x11 = {
    "x11-xrandr",
    x11_enumerate, x11_set_pixel, x11_get_pixel, x11_blit,
    x11_set_brightness, x11_set_warmth, x11_set_gamma, x11_poll_event,
    x11_poll_key,
    x11_claim_surface, x11_present, x11_release_surface
};

const ballum_backend_t *ballum_backend_x11(void) { return &k_x11; }

#else  /* !BALLUM_BACKEND_X11 -- empty TU so the file is always compilable */

typedef int ballum_x11_translation_unit_not_selected;

#endif /* BALLUM_BACKEND_X11 */
