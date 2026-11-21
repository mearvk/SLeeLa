/*
 * ballum/src/ballum_host_x11.c -- program source over a private X display.
 *
 * Runs a program (a terminal, an admin UI, any X11 client) on its OWN X server
 * (Xvfb), captures that server's root window as the program's pixels, and
 * replays Ballum's input into it with the XTEST extension. The program draws
 * itself with its real toolkit and fonts, so a terminal is pixel-for-pixel what
 * it is on a native desktop; Ballum then owns those pixels.
 *
 * Why a private server:
 *   - Isolation: the program cannot see, capture or inject into the user's real
 *     desktop, and XTEST injection reaches ONLY the private display.
 *   - Authentication: the display requires a random MIT-MAGIC-COOKIE-1 held in
 *     a 0600 file inside a 0700 directory, and does not listen on TCP.
 *   - Exact pixels: no compositor or window manager of the real desktop is in
 *     the path. This source plays the window manager's one job: it sizes the
 *     program's first window to the viewport.
 *
 * Needs libX11 at build time, and at run time an Xvfb binary on the trusted
 * search path plus libXtst.so.6. If either is missing start() fails and nothing
 * runs: input could not be forwarded 1:1, so the source refuses.
 *
 * Contract note: the program must stay in the foreground (as xterm and GTK
 * terminals do). A launcher that forks and exits is treated as having ended.
 *
 * Without BALLUM_BACKEND_X11 this file builds to stubs returning UNSUPPORTED.
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include "sleela_ballum/ballum_host.h"
#include "sleela_ballum/ballum_sandbox.h"

#include <string.h>

#if defined(BALLUM_BACKEND_X11) && defined(__linux__)

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define DISPLAY_FIRST 90
#define DISPLAY_LAST  139
#define MAX_ARGS      64
#define CAPTURE_MIN_INTERVAL_MS 8

typedef Bool (*xt_query_fn)(Display *, int *, int *, int *, int *);
typedef int  (*xt_key_fn)(Display *, unsigned int, Bool, unsigned long);
typedef int  (*xt_button_fn)(Display *, unsigned int, Bool, unsigned long);
typedef int  (*xt_motion_fn)(Display *, int, int, int, unsigned long);

typedef struct {
    /* program description (owned copies) */
    char     *argv[MAX_ARGS + 1];
    char    **extra_env;
    size_t    n_extra;
    int       sandbox, log_fd;

    uint32_t  w, h;
    int       display_no;
    char      dir[PATH_MAX];
    char      auth[PATH_MAX + 32];
    int       xvfb_pid, prog_pid;
    int       prog_alive, started;

    Display  *dpy;
    Window    root;
    int       screen;
    Window    toplevel;
    int       saw_toplevel;

    void        *xtst;
    xt_key_fn    fake_key;
    xt_button_fn fake_button;
    xt_motion_fn fake_motion;

    XImage         *img;
    unsigned char  *raw_prev;
    size_t          raw_bytes;
    ballum_color_t *cur;
    uint64_t        last_capture_ms;
    int             have_cur;
} x11src_t;

/* ---- helpers --------------------------------------------------------------- */

static uint64_t now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}

static void sleep_ms(unsigned ms) {
    struct timespec ts = { (time_t)(ms / 1000u), (long)(ms % 1000u) * 1000000L };
    nanosleep(&ts, NULL);
}

/* Xlib's default error handler exits the process. Errors on a client window
 * that vanished mid-request are normal for a window manager, so they are
 * tolerated, but ONLY while this guard is held: the handler is process-wide and
 * must not hide errors from the Ballum backend's own display connection. */
static int tolerant_handler(Display *d, XErrorEvent *e) { (void)d; (void)e; return 0; }

typedef int (*xerr_fn)(Display *, XErrorEvent *);
static xerr_fn guard_begin(void) { return XSetErrorHandler(tolerant_handler); }
static void guard_end(Display *dpy, xerr_fn prev) { XSync(dpy, False); XSetErrorHandler(prev); }

static int put16(FILE *f, unsigned v) { return fputc((int)(v >> 8) & 0xFF, f) == EOF || fputc((int)v & 0xFF, f) == EOF; }
static int put_str(FILE *f, const void *s, size_t n) {
    return put16(f, (unsigned)n) || (n && fwrite(s, 1, n, f) != n);
}

/* Write a one-entry Xauthority file for display `no` and return the cookie. */
static int write_authority(const char *path, int no, unsigned char cookie[16]) {
    int rfd = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
    if (rfd < 0) return -1;
    size_t got = 0;
    while (got < 16) {
        ssize_t r = read(rfd, cookie + got, 16 - got);
        if (r <= 0) { close(rfd); return -1; }
        got += (size_t)r;
    }
    close(rfd);
    char host[256];
    if (gethostname(host, sizeof(host) - 1) != 0) return -1;
    host[sizeof(host) - 1] = '\0';
    char num[16];
    snprintf(num, sizeof(num), "%d", no);
    int fd = open(path, O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (fd < 0) return -1;
    FILE *f = fdopen(fd, "wb");
    if (!f) { close(fd); return -1; }
    int bad = put16(f, 256 /* FamilyLocal */) || put_str(f, host, strlen(host)) ||
              put_str(f, num, strlen(num)) || put_str(f, "MIT-MAGIC-COOKIE-1", 18) ||
              put_str(f, cookie, 16);
    if (fclose(f) != 0) bad = 1;
    return bad ? -1 : 0;
}

static int socket_exists(int no) {
    char p[64];
    snprintf(p, sizeof(p), "/tmp/.X11-unix/X%d", no);
    struct stat st;
    return stat(p, &st) == 0;
}
static int lock_exists(int no) {
    char p[64];
    snprintf(p, sizeof(p), "/tmp/.X%d-lock", no);
    return access(p, F_OK) == 0;
}

static void free_owned(x11src_t *s) {
    for (int i = 0; s->argv[i]; ++i) { free(s->argv[i]); s->argv[i] = NULL; }
    for (size_t i = 0; i < s->n_extra; ++i) free(s->extra_env[i]);
    free(s->extra_env); s->extra_env = NULL; s->n_extra = 0;
}

/* ---- teardown ---------------------------------------------------------------- */

static void src_stop(void *ctx) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s) return;
    if (s->prog_pid > 0) { ballum_sandbox_terminate(s->prog_pid, 500); s->prog_pid = 0; }
    s->prog_alive = 0;
    if (s->dpy) { XCloseDisplay(s->dpy); s->dpy = NULL; }
    if (s->xvfb_pid > 0) { ballum_sandbox_terminate(s->xvfb_pid, 500); s->xvfb_pid = 0; }
    if (s->img) { XDestroyImage(s->img); s->img = NULL; }
    free(s->raw_prev); s->raw_prev = NULL;
    free(s->cur); s->cur = NULL;
    if (s->xtst) { dlclose(s->xtst); s->xtst = NULL; }
    if (s->auth[0]) { unlink(s->auth); s->auth[0] = '\0'; }
    if (s->dir[0]) { rmdir(s->dir); s->dir[0] = '\0'; }
    if (s->display_no) {                                   /* Xvfb removes its own; be thorough */
        char p[64];
        snprintf(p, sizeof(p), "/tmp/.X%d-lock", s->display_no);
        unlink(p);
        s->display_no = 0;
    }
    s->started = 0;
}

void ballum_host_source_x11_destroy(void *ctx) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s) return;
    src_stop(s);
    free_owned(s);
    free(s);
}

/* ---- start ---------------------------------------------------------------------- */

static int load_xtest(x11src_t *s) {
    s->xtst = dlopen("libXtst.so.6", RTLD_NOW | RTLD_LOCAL);
    if (!s->xtst) return -1;
    void *k = dlsym(s->xtst, "XTestFakeKeyEvent");
    void *b = dlsym(s->xtst, "XTestFakeButtonEvent");
    void *m = dlsym(s->xtst, "XTestFakeMotionEvent");
    if (!k || !b || !m) return -1;
    memcpy(&s->fake_key, &k, sizeof(k));
    memcpy(&s->fake_button, &b, sizeof(b));
    memcpy(&s->fake_motion, &m, sizeof(m));
    return 0;
}

static ballum_status_t start_xvfb(x11src_t *s) {
    char xvfb[PATH_MAX];
    ballum_status_t st = ballum_sandbox_resolve("Xvfb", xvfb, sizeof(xvfb));
    if (st != BALLUM_STATUS_OK) return st == BALLUM_STATUS_PERMISSION_DENIED ? st : BALLUM_STATUS_BACKEND_UNAVAILABLE;

    const char *allow[] = { "PATH", "LANG", NULL };
    const char *extra[] = { "HOME=/nonexistent", NULL };
    char **env = ballum_sandbox_env_new(allow, extra);
    if (!env) return BALLUM_STATUS_BACKEND_ERROR;
    int devnull = open("/dev/null", O_WRONLY | O_CLOEXEC);

    ballum_sandbox_policy_t pol;
    ballum_sandbox_policy_default(&pol);
    st = BALLUM_STATUS_BACKEND_UNAVAILABLE;
    for (int no = DISPLAY_FIRST; no <= DISPLAY_LAST; ++no) {
        if (socket_exists(no) || lock_exists(no)) continue;
        unsigned char cookie[16];
        snprintf(s->auth, sizeof(s->auth), "%s/Xauthority", s->dir);
        unlink(s->auth);
        if (write_authority(s->auth, no, cookie) != 0) { st = BALLUM_STATUS_PERMISSION_DENIED; break; }

        char dname[16], geom[48];
        snprintf(dname, sizeof(dname), ":%d", no);
        snprintf(geom, sizeof(geom), "%ux%ux24", s->w, s->h);
        char *argv[] = { "Xvfb", dname, "-screen", "0", geom, "-nolisten", "tcp",
                         "-auth", s->auth, "-noreset", "-dpi", "96", NULL };
        int pid = -1;
        st = ballum_sandbox_spawn(&pol, xvfb, argv, env, devnull, &pid);
        if (st != BALLUM_STATUS_OK) break;
        /* wait for the server socket, watching for an early exit (lost the race) */
        int up = 0, code = 0;
        for (int waited = 0; waited < 5000; waited += 20) {
            if (ballum_sandbox_wait(pid, 0, &code) == 1) break;
            if (socket_exists(no)) { up = 1; break; }
            sleep_ms(20);
        }
        if (up) { s->xvfb_pid = pid; s->display_no = no; st = BALLUM_STATUS_OK; break; }
        ballum_sandbox_terminate(pid, 200);
        st = BALLUM_STATUS_BACKEND_UNAVAILABLE;
    }
    if (devnull >= 0) close(devnull);
    ballum_sandbox_env_free(env);
    return st;
}

static ballum_status_t connect_display(x11src_t *s) {
    char name[16];
    snprintf(name, sizeof(name), ":%d", s->display_no);
    const char *old = getenv("XAUTHORITY");
    char *saved = old ? strdup(old) : NULL;
    setenv("XAUTHORITY", s->auth, 1);
    for (int waited = 0; waited < 5000 && !s->dpy; waited += 20) {
        s->dpy = XOpenDisplay(name);
        if (!s->dpy) sleep_ms(20);
    }
    if (saved) { setenv("XAUTHORITY", saved, 1); free(saved); } else unsetenv("XAUTHORITY");
    if (!s->dpy) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    s->screen = DefaultScreen(s->dpy);
    s->root = RootWindow(s->dpy, s->screen);

    int a, b, c, d;
    xt_query_fn q = NULL;
    void *qp = dlsym(s->xtst, "XTestQueryExtension");
    if (!qp) return BALLUM_STATUS_UNSUPPORTED;
    memcpy(&q, &qp, sizeof(qp));
    if (!q(s->dpy, &a, &b, &c, &d)) return BALLUM_STATUS_UNSUPPORTED;

    /* The one window-manager duty: be the redirect target so we size the window. */
    xerr_fn prev = guard_begin();
    XSelectInput(s->dpy, s->root, SubstructureRedirectMask | SubstructureNotifyMask);
    XSetWindowBackground(s->dpy, s->root, WhitePixel(s->dpy, s->screen));
    XClearWindow(s->dpy, s->root);
    guard_end(s->dpy, prev);

    XImage *probe = XCreateImage(s->dpy, DefaultVisual(s->dpy, s->screen), (unsigned)DefaultDepth(s->dpy, s->screen),
                                 ZPixmap, 0, NULL, s->w, s->h, 32, 0);
    if (!probe) return BALLUM_STATUS_BACKEND_ERROR;
    int ok = probe->bits_per_pixel == 32 && probe->byte_order == LSBFirst &&
             probe->red_mask == 0xFF0000u && probe->green_mask == 0x00FF00u && probe->blue_mask == 0x0000FFu;
    s->raw_bytes = (size_t)probe->bytes_per_line * s->h;
    probe->data = malloc(s->raw_bytes);
    if (!probe->data) { XDestroyImage(probe); return BALLUM_STATUS_BACKEND_ERROR; }
    s->img = probe;
    if (!ok) return BALLUM_STATUS_UNSUPPORTED;               /* only 32bpp BGRX is converted */
    s->raw_prev = (unsigned char *)calloc(1, s->raw_bytes);
    s->cur = (ballum_color_t *)calloc((size_t)s->w * s->h, sizeof(ballum_color_t));
    if (!s->raw_prev || !s->cur) return BALLUM_STATUS_BACKEND_ERROR;
    return BALLUM_STATUS_OK;
}

static ballum_status_t launch_program(x11src_t *s) {
    char path[PATH_MAX];
    ballum_status_t st = ballum_sandbox_resolve(s->argv[0], path, sizeof(path));
    if (st != BALLUM_STATUS_OK) return st;

    char disp[16], auth[PATH_MAX + 64];
    snprintf(disp, sizeof(disp), "DISPLAY=:%d", s->display_no);
    snprintf(auth, sizeof(auth), "XAUTHORITY=%s", s->auth);
    const char *allow[] = { "PATH", "HOME", "USER", "LOGNAME", "LANG", "LC_ALL", "SHELL",
                            "XDG_RUNTIME_DIR", NULL };
    size_t n = 4 + s->n_extra;
    const char **extra = (const char **)calloc(n + 1, sizeof(char *));
    if (!extra) return BALLUM_STATUS_BACKEND_ERROR;
    size_t k = 0;
    /* First match wins in getenv(): the private display can never be overridden,
     * caller variables beat the defaults placed last. */
    extra[k++] = disp; extra[k++] = auth;
    for (size_t i = 0; i < s->n_extra; ++i) extra[k++] = s->extra_env[i];
    extra[k++] = "TERM=xterm-256color"; extra[k++] = "GDK_BACKEND=x11";
    char **env = ballum_sandbox_env_new(allow, extra);
    free((void *)extra);
    if (!env) return BALLUM_STATUS_INVALID_ARGUMENT;

    ballum_sandbox_policy_t pol;
    ballum_sandbox_policy_default(&pol);
    if (!s->sandbox) { pol.no_new_privs = 0; pol.seccomp_denylist = 0; }
    int pid = -1;
    st = ballum_sandbox_spawn(&pol, path, s->argv, env, s->log_fd, &pid);
    ballum_sandbox_env_free(env);
    if (st != BALLUM_STATUS_OK) return st;
    s->prog_pid = pid;
    s->prog_alive = 1;
    return BALLUM_STATUS_OK;
}

static ballum_status_t src_start(void *ctx, uint32_t w, uint32_t h) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s || s->started || w < 16 || h < 16 || w > 16384 || h > 16384) return BALLUM_STATUS_INVALID_ARGUMENT;
    s->w = w; s->h = h;
    s->started = 1;

    ballum_status_t st = BALLUM_STATUS_OK;
    if (load_xtest(s) != 0) { st = BALLUM_STATUS_UNSUPPORTED; goto fail; }   /* no 1:1 input possible */

    const char *base = getenv("XDG_RUNTIME_DIR");
    char tmpl[PATH_MAX];
    if (base && *base && access(base, W_OK) == 0) snprintf(tmpl, sizeof(tmpl), "%s/ballum-XXXXXX", base);
    else snprintf(tmpl, sizeof(tmpl), "/tmp/ballum-XXXXXX");
    if (!mkdtemp(tmpl)) { st = BALLUM_STATUS_PERMISSION_DENIED; goto fail; }   /* mode 0700 */
    snprintf(s->dir, sizeof(s->dir), "%s", tmpl);

    if ((st = start_xvfb(s)) != BALLUM_STATUS_OK) goto fail;
    if ((st = connect_display(s)) != BALLUM_STATUS_OK) goto fail;
    if ((st = launch_program(s)) != BALLUM_STATUS_OK) goto fail;
    return BALLUM_STATUS_OK;
fail:
    src_stop(s);
    return st;
}

/* ---- window management, capture, input ------------------------------------------ */

static void manage(x11src_t *s) {
    xerr_fn prev = guard_begin();
    while (XPending(s->dpy) > 0) {
        XEvent ev;
        XNextEvent(s->dpy, &ev);
        switch (ev.type) {
        case MapRequest: {
            Window win = ev.xmaprequest.window;
            if (!s->toplevel) {                                   /* the program's main window fills the viewport */
                s->toplevel = win;
                XMoveResizeWindow(s->dpy, win, 0, 0, s->w, s->h);
                XMapWindow(s->dpy, win);
                XSync(s->dpy, False);
                XSetInputFocus(s->dpy, win, RevertToPointerRoot, CurrentTime);
            } else {                                              /* dialogs keep their size, centered */
                XWindowAttributes wa;
                if (XGetWindowAttributes(s->dpy, win, &wa)) {
                    int x = ((int)s->w - wa.width) / 2, y = ((int)s->h - wa.height) / 2;
                    XMoveWindow(s->dpy, win, x < 0 ? 0 : x, y < 0 ? 0 : y);
                }
                XMapRaised(s->dpy, win);
            }
            break;
        }
        case ConfigureRequest: {
            XConfigureRequestEvent *r = &ev.xconfigurerequest;
            XWindowChanges wc;
            memset(&wc, 0, sizeof(wc));
            if (r->window == s->toplevel) {                       /* pinned to the viewport */
                wc.x = 0; wc.y = 0; wc.width = (int)s->w; wc.height = (int)s->h;
                XConfigureWindow(s->dpy, r->window, CWX | CWY | CWWidth | CWHeight, &wc);
            } else {
                wc.x = r->x; wc.y = r->y; wc.width = r->width; wc.height = r->height;
                wc.border_width = r->border_width; wc.sibling = r->above; wc.stack_mode = r->detail;
                XConfigureWindow(s->dpy, r->window, (unsigned)r->value_mask, &wc);
            }
            break;
        }
        case MapNotify:
            if (ev.xmap.window == s->toplevel) s->saw_toplevel = 1;
            break;
        case UnmapNotify:
        case DestroyNotify:
            if ((ev.type == UnmapNotify ? ev.xunmap.window : ev.xdestroywindow.window) == s->toplevel) {
                s->toplevel = 0; s->saw_toplevel = 0;
            }
            break;
        default:
            break;
        }
    }
    guard_end(s->dpy, prev);
}

static ballum_status_t src_poll_frame(void *ctx, const ballum_color_t **pixels, int *changed) {
    x11src_t *s = (x11src_t *)ctx;
    *pixels = NULL; *changed = 0;
    if (!s || !s->dpy) return BALLUM_STATUS_NOT_INITIALIZED;
    manage(s);
    if (!s->saw_toplevel) return BALLUM_STATUS_OK;               /* nothing to show yet */

    uint64_t t = now_ms();
    if (s->have_cur && t - s->last_capture_ms < CAPTURE_MIN_INTERVAL_MS) { *pixels = s->cur; return BALLUM_STATUS_OK; }
    s->last_capture_ms = t;

    xerr_fn prev = guard_begin();
    XImage *got = XGetSubImage(s->dpy, s->root, 0, 0, s->w, s->h, AllPlanes, ZPixmap, s->img, 0, 0);
    guard_end(s->dpy, prev);
    if (!got) return BALLUM_STATUS_BACKEND_ERROR;

    if (s->have_cur && memcmp(s->raw_prev, s->img->data, s->raw_bytes) == 0) {
        *pixels = s->cur;                                         /* identical: skip the conversion */
        return BALLUM_STATUS_OK;
    }
    memcpy(s->raw_prev, s->img->data, s->raw_bytes);
    for (uint32_t y = 0; y < s->h; ++y) {
        const uint32_t *row = (const uint32_t *)(const void *)(s->img->data + (size_t)y * (size_t)s->img->bytes_per_line);
        ballum_color_t *out = s->cur + (size_t)y * s->w;
        for (uint32_t x = 0; x < s->w; ++x) {
            uint32_t v = row[x];
            out[x].r = (uint8_t)(v >> 16); out[x].g = (uint8_t)(v >> 8); out[x].b = (uint8_t)v; out[x].a = 255;
        }
    }
    s->have_cur = 1;
    *pixels = s->cur; *changed = 1;
    return BALLUM_STATUS_OK;
}

static ballum_status_t src_pointer(void *ctx, int32_t x, int32_t y) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s || !s->dpy || x < 0 || y < 0 || (uint32_t)x >= s->w || (uint32_t)y >= s->h) return BALLUM_STATUS_OUT_OF_BOUNDS;
    s->fake_motion(s->dpy, s->screen, x, y, 0);
    XFlush(s->dpy);
    return BALLUM_STATUS_OK;
}

static ballum_status_t src_button(void *ctx, int button, int pressed) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s || !s->dpy || button < 1 || button > 7) return BALLUM_STATUS_INVALID_ARGUMENT;
    s->fake_button(s->dpy, (unsigned)button, pressed ? True : False, 0);
    XFlush(s->dpy);
    return BALLUM_STATUS_OK;
}

static ballum_status_t src_key(void *ctx, uint32_t symbol, uint32_t code, int pressed) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s || !s->dpy) return BALLUM_STATUS_NOT_INITIALIZED;
    /* The symbol is looked up in the program display's own keymap; the physical
     * code is the fallback when the symbol is unknown there. */
    KeyCode kc = symbol ? XKeysymToKeycode(s->dpy, (KeySym)symbol) : 0;
    if (!kc && code >= 8 && code <= 255) kc = (KeyCode)code;
    if (!kc) return BALLUM_STATUS_UNSUPPORTED;
    s->fake_key(s->dpy, kc, pressed ? True : False, 0);
    XFlush(s->dpy);
    return BALLUM_STATUS_OK;
}

static int src_alive(void *ctx) {
    x11src_t *s = (x11src_t *)ctx;
    if (!s || !s->prog_alive) return 0;
    int code;
    if (ballum_sandbox_wait(s->prog_pid, 0, &code) != 0) { s->prog_alive = 0; s->prog_pid = 0; return 0; }
    if (s->xvfb_pid > 0 && ballum_sandbox_wait(s->xvfb_pid, 0, &code) != 0) {   /* the display died */
        s->xvfb_pid = 0; s->prog_alive = 0; return 0;
    }
    return 1;
}

ballum_status_t ballum_host_source_x11_create(const ballum_program_t *prog,
                                              ballum_host_source_t *source, void **ctx_out) {
    if (!prog || !prog->argv || !prog->argv[0] || !prog->argv[0][0] || !source || !ctx_out)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    *ctx_out = NULL;
    x11src_t *s = (x11src_t *)calloc(1, sizeof(*s));
    if (!s) return BALLUM_STATUS_BACKEND_ERROR;
    s->sandbox = prog->sandbox; s->log_fd = prog->log_fd;
    size_t n = 0;
    for (; prog->argv[n]; ++n) {
        if (n >= MAX_ARGS || strlen(prog->argv[n]) >= 4096u) { free_owned(s); free(s); return BALLUM_STATUS_INVALID_ARGUMENT; }
        s->argv[n] = strdup(prog->argv[n]);
        if (!s->argv[n]) { free_owned(s); free(s); return BALLUM_STATUS_BACKEND_ERROR; }
    }
    for (size_t i = 0; prog->extra_env && prog->extra_env[i]; ++i) s->n_extra++;
    if (s->n_extra) {
        s->extra_env = (char **)calloc(s->n_extra, sizeof(char *));
        if (!s->extra_env) { s->n_extra = 0; free_owned(s); free(s); return BALLUM_STATUS_BACKEND_ERROR; }
        for (size_t i = 0; i < s->n_extra; ++i) {
            s->extra_env[i] = strdup(prog->extra_env[i]);
            if (!s->extra_env[i]) { s->n_extra = i; free_owned(s); free(s); return BALLUM_STATUS_BACKEND_ERROR; }
        }
    }
    memset(source, 0, sizeof(*source));
    source->name = "x11-private-display";
    source->start = src_start; source->poll_frame = src_poll_frame;
    source->pointer = src_pointer; source->button = src_button; source->key = src_key;
    source->alive = src_alive; source->stop = src_stop;
    *ctx_out = s;
    return BALLUM_STATUS_OK;
}

#else  /* no X11 / not Linux: the source is not available, and says so */

ballum_status_t ballum_host_source_x11_create(const ballum_program_t *program,
                                              ballum_host_source_t *source, void **ctx_out) {
    (void)program;
    if (source) memset(source, 0, sizeof(*source));
    if (ctx_out) *ctx_out = NULL;
    return BALLUM_STATUS_UNSUPPORTED;
}
void ballum_host_source_x11_destroy(void *context) { (void)context; }

#endif
