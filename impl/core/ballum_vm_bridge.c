#include "ballum_vm_bridge.h"
#include "../../ballum/include/sleela_ballum/ballum_manager.h"
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
