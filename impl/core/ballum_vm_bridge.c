#include "ballum_vm_bridge.h"
#include "../../ballum/include/sleela_ballum/ballum_manager.h"
#include "../../ballum/include/sleela_ballum/ballum_image.h"
#include "../../ballum/include/sleela_ballum/ballum_effects.h"
#include <string.h>

/*
 * One process-global Ballum manager over the compiled-in per-OS backend, lazily
 * initialized on first use. The cached display list is owned here; names are
 * copied into a small static buffer at the boundary so the VM never holds a
 * pointer into native storage.
 */
static ballum_manager_t g_manager;
static ballum_display_t g_displays[BALLUM_MAX_DISPLAYS];
static size_t           g_count;
static int              g_initialized;
static char             g_name[BALLUM_DISPLAY_NAME_BYTES];

static int ensure_manager(void) {
    ballum_status_t status;
    if (g_initialized) return BALLUM_STATUS_OK;
    status = ballum_manager_init(&g_manager, ballum_backend_default(), NULL);
    if (status == BALLUM_STATUS_OK) g_initialized = 1;
    return (int)status;
}

static const ballum_display_t *display_at(int index) {
    return index >= 0 && (size_t)index < g_count ? &g_displays[index] : NULL;
}

int sleela_ballum_vm_supported(void) { return ballum_backend_supported(); }

const char *sleela_ballum_vm_backend_name(void) {
    const ballum_backend_t *b = ballum_backend_default();
    return (b && b->name) ? b->name : "unavailable";
}

int sleela_ballum_vm_enumerate(void) {
    size_t count = 0u;
    int init = ensure_manager();
    ballum_status_t status;
    if (init != BALLUM_STATUS_OK) return init;
    status = ballum_manager_enumerate(&g_manager, g_displays, BALLUM_MAX_DISPLAYS, &count);
    if (status == BALLUM_STATUS_OK || status == BALLUM_STATUS_BUFFER_TOO_SMALL)
        g_count = count > BALLUM_MAX_DISPLAYS ? BALLUM_MAX_DISPLAYS : count;
    else
        g_count = 0u;
    return (int)status;
}

int sleela_ballum_vm_display_count(void) { return (int)g_count; }

int sleela_ballum_vm_display_id(int index) {
    const ballum_display_t *d = display_at(index);
    return d ? (int)d->id : -1;
}

const char *sleela_ballum_vm_display_name(int index) {
    const ballum_display_t *d = display_at(index);
    if (!d) return "";
    memcpy(g_name, d->name, sizeof(g_name));
    g_name[sizeof(g_name) - 1u] = '\0';
    return g_name;
}

int sleela_ballum_vm_display_width(int index) {
    const ballum_display_t *d = display_at(index);
    return d ? (int)d->width : 0;
}

int sleela_ballum_vm_display_height(int index) {
    const ballum_display_t *d = display_at(index);
    return d ? (int)d->height : 0;
}

int sleela_ballum_vm_display_is_primary(int index) {
    const ballum_display_t *d = display_at(index);
    return d ? (d->is_primary ? 1 : 0) : -1;
}

int sleela_ballum_vm_display_caps(int index) {
    const ballum_display_t *d = display_at(index);
    return d ? (int)d->capabilities : 0;
}

static int clamp_byte(int v) { return v < 0 ? 0 : (v > 255 ? 255 : v); }

int sleela_ballum_vm_set_pixel(int display_id, int x, int y,
                               int r, int g, int b, int a) {
    ballum_point_t at;
    ballum_color_t color;
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    if (x < 0 || y < 0) return BALLUM_STATUS_OUT_OF_BOUNDS;
    at.x = (uint32_t)x;
    at.y = (uint32_t)y;
    color = ballum_color_rgba((uint8_t)clamp_byte(r), (uint8_t)clamp_byte(g),
                              (uint8_t)clamp_byte(b), (uint8_t)clamp_byte(a));
    return (int)ballum_manager_set_pixel(&g_manager, (uint32_t)display_id, at, color);
}

int sleela_ballum_vm_get_pixel(int display_id, int x, int y) {
    ballum_point_t at;
    ballum_color_t out;
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return -1;
    if (x < 0 || y < 0) return -1;
    at.x = (uint32_t)x;
    at.y = (uint32_t)y;
    if (ballum_manager_get_pixel(&g_manager, (uint32_t)display_id, at, &out) != BALLUM_STATUS_OK)
        return -1;
    return ((int)out.a << 24) | ((int)out.r << 16) | ((int)out.g << 8) | (int)out.b;
}

int sleela_ballum_vm_set_brightness(int display_id, int percent) {
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    return (int)ballum_manager_set_brightness(&g_manager, (uint32_t)display_id, percent);
}

int sleela_ballum_vm_set_warmth(int display_id, int kelvin) {
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    if (kelvin < 0) kelvin = 0;
    return (int)ballum_manager_set_warmth(&g_manager, (uint32_t)display_id, (uint32_t)kelvin);
}

int sleela_ballum_vm_set_gamma(int display_id, int gamma_scaled) {
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    if (gamma_scaled < 0) gamma_scaled = 0;
    return (int)ballum_manager_set_gamma(&g_manager, (uint32_t)display_id, (uint32_t)gamma_scaled);
}

const char *sleela_ballum_vm_status_name(int status) {
    return ballum_status_name((ballum_status_t)status);
}

/* ---- fullscreen surface (total per-pixel control) ------------------------ */
/* Claim an override-redirect fullscreen framebuffer over the display; while
 * claimed, set_pixel/get_pixel and logo_blend write to the framebuffer (fast,
 * no X round-trips) and present() pushes the whole frame in one shot. */
int sleela_ballum_vm_has_surface(int display_id) {
    if (ensure_manager() != BALLUM_STATUS_OK) return 0;
    return ballum_manager_has_surface(&g_manager, (uint32_t)display_id) == 1 ? 1 : 0;
}

int sleela_ballum_vm_claim_surface(int display_id) {
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    return (int)ballum_manager_claim_surface(&g_manager, (uint32_t)display_id);
}

int sleela_ballum_vm_present(int display_id) {
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    return (int)ballum_manager_present(&g_manager, (uint32_t)display_id);
}

int sleela_ballum_vm_release_surface(int display_id) {
    int init = ensure_manager();
    if (init != BALLUM_STATUS_OK) return init;
    return (int)ballum_manager_release_surface(&g_manager, (uint32_t)display_id);
}

/* ---- pointer input ------------------------------------------------------- */
/* The most recently polled event, latched so the SLeeLa side can read its
 * fields after poll() reports the type. */
static ballum_pointer_event_t g_event;

int sleela_ballum_vm_has_input(void) {
    if (ensure_manager() != BALLUM_STATUS_OK) return 0;
    return ballum_manager_has_input(&g_manager) == 1 ? 1 : 0;
}

int sleela_ballum_vm_poll(void) {
    ballum_pointer_event_t ev;
    if (ensure_manager() != BALLUM_STATUS_OK) { g_event.type = BALLUM_EVENT_NONE; return BALLUM_EVENT_NONE; }
    if (ballum_manager_poll_event(&g_manager, &ev) != BALLUM_STATUS_OK) {
        g_event.type = BALLUM_EVENT_NONE;
        return BALLUM_EVENT_NONE;
    }
    g_event = ev;
    return (int)g_event.type;
}

int sleela_ballum_vm_event_x(void)         { return (int)g_event.x; }
int sleela_ballum_vm_event_y(void)         { return (int)g_event.y; }
int sleela_ballum_vm_event_button(void)    { return (int)g_event.button; }
int sleela_ballum_vm_event_source(void)    { return (int)g_event.source; }
int sleela_ballum_vm_event_contact(void)   { return (int)g_event.contact_id; }
int sleela_ballum_vm_event_modifiers(void) { return (int)g_event.modifiers; }
const char *sleela_ballum_vm_event_type_name(int type) {
    return ballum_event_type_name((ballum_event_type_t)type);
}

/* ---- keyboard input ------------------------------------------------------ */
int sleela_ballum_vm_has_keyboard(void) {
    if (ensure_manager() != BALLUM_STATUS_OK) return 0;
    return ballum_manager_has_keyboard(&g_manager) == 1 ? 1 : 0;
}

int sleela_ballum_vm_poll_key(void) {
    int key = 0;
    if (ensure_manager() != BALLUM_STATUS_OK) return 0;
    if (ballum_manager_poll_key(&g_manager, &key) != BALLUM_STATUS_OK) return 0;
    return key;
}

/* ---- logo splash: real image handler + effects --------------------------
 * The bridge now decodes a logo (JPEG via the bundled nanojpeg, through
 * ballum_image) and composites it onto the display with a selectable transition
 * effect (ballum_effects) for a given progress. A session loads the logo once
 * (uiBallumLogoLoad), picks an effect (uiBallumLogoSetEffect), then calls
 * logo_blend each frame. Fail closed: if no logo is loaded, logo_ready()==0 and
 * logo_blend is a clean no-op, so the white fill + timeline + ESC still run and
 * nothing false is drawn. */
static ballum_image_t g_logo;
static int            g_logo_loaded;
static ballum_fx_t    g_logo_fx = BALLUM_FX_FADE;

int sleela_ballum_vm_logo_load(const char *path) {
    if (!path || path[0] == '\0') return (int)BALLUM_STATUS_INVALID_ARGUMENT;
    if (g_logo_loaded) { ballum_image_free(&g_logo); g_logo_loaded = 0; }
    ballum_image_init(&g_logo);
    ballum_status_t st = ballum_image_load_file(&g_logo, path);
    g_logo_loaded = (st == BALLUM_STATUS_OK && ballum_image_ready(&g_logo)) ? 1 : 0;
    return (int)st;
}

int sleela_ballum_vm_logo_set_effect(int fx) {
    if (fx < 0 || fx >= (int)BALLUM_FX__COUNT) return (int)BALLUM_STATUS_INVALID_ARGUMENT;
    g_logo_fx = (ballum_fx_t)fx;
    return (int)BALLUM_STATUS_OK;
}

int sleela_ballum_vm_logo_ready(void) {
    return (g_logo_loaded && ballum_image_ready(&g_logo)) ? 1 : 0;
}

int sleela_ballum_vm_logo_blend(int display_id, int opacity_percent,
                                int screen_w, int screen_h,
                                int bg_r, int bg_g, int bg_b) {
    if (!sleela_ballum_vm_logo_ready()) return (int)BALLUM_STATUS_UNSUPPORTED;
    if (ensure_manager() != BALLUM_STATUS_OK) return (int)BALLUM_STATUS_BACKEND_UNAVAILABLE;
    if (screen_w <= 0 || screen_h <= 0) return (int)BALLUM_STATUS_INVALID_ARGUMENT;

    ballum_color_t bg = ballum_color_rgb((uint8_t)bg_r, (uint8_t)bg_g, (uint8_t)bg_b);
    int t = opacity_percent < 0 ? 0 : (opacity_percent > 100 ? 100 : opacity_percent);

    /* Composite the whole frame: for each pixel, sample the logo fitted/centered
     * on the display, run the selected effect at progress t, and plot it. */
    for (int y = 0; y < screen_h; ++y) {
        for (int x = 0; x < screen_w; ++x) {
            ballum_color_t fg = ballum_image_sample_fit(&g_logo, screen_w, screen_h, x, y);
            ballum_color_t out = ballum_fx_pixel(g_logo_fx, t, screen_w, screen_h, x, y, fg, bg);
            ballum_point_t at; at.x = x; at.y = y;
            ballum_manager_set_pixel(&g_manager, (uint32_t)display_id, at, out);
        }
    }
    return (int)BALLUM_STATUS_OK;
}

/* ---- effect catalog passthroughs ---------------------------------------- */
int sleela_ballum_vm_fx_count(void) { return ballum_fx_count(); }
const char *sleela_ballum_vm_fx_name(int fx) { return ballum_fx_name((ballum_fx_t)fx); }
int sleela_ballum_vm_fx_by_name(const char *name) { return (int)ballum_fx_by_name(name); }
