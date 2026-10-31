/*
 * Ballum portable manager -- OS-independent logic over a backend vtable.
 *
 * All validation (display bounds, capability presence, value ranges) lives here
 * so every per-OS backend stays thin and only performs the device I/O. The
 * manager caches the enumerated display list and validates each pixel / region
 * / control request against it before dispatching through the vtable.
 */

#include "sleela_ballum/ballum_manager.h"

#include <string.h>

/* ---- helpers ------------------------------------------------------------- */

const char *ballum_event_type_name(ballum_event_type_t type) {
    switch (type) {
        case BALLUM_EVENT_NONE:         return "none";
        case BALLUM_EVENT_POINTER_DOWN: return "down";
        case BALLUM_EVENT_POINTER_MOVE: return "move";
        case BALLUM_EVENT_POINTER_UP:   return "up";
    }
    return "unknown";
}

const char *ballum_status_name(ballum_status_t status) {
    switch (status) {
        case BALLUM_STATUS_OK:                  return "ok";
        case BALLUM_STATUS_INVALID_ARGUMENT:    return "invalid argument";
        case BALLUM_STATUS_BACKEND_UNAVAILABLE: return "backend unavailable";
        case BALLUM_STATUS_BACKEND_ERROR:       return "backend error";
        case BALLUM_STATUS_DISPLAY_NOT_FOUND:   return "display not found";
        case BALLUM_STATUS_OUT_OF_BOUNDS:       return "coordinate out of bounds";
        case BALLUM_STATUS_BUFFER_TOO_SMALL:    return "buffer too small";
        case BALLUM_STATUS_UNSUPPORTED:         return "unsupported by backend";
        case BALLUM_STATUS_PERMISSION_DENIED:   return "permission denied";
        case BALLUM_STATUS_NOT_INITIALIZED:     return "not initialized";
    }
    return "unknown";
}

ballum_color_t ballum_color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    ballum_color_t c;
    c.r = r; c.g = g; c.b = b; c.a = 255;
    return c;
}

ballum_color_t ballum_color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    ballum_color_t c;
    c.r = r; c.g = g; c.b = b; c.a = a;
    return c;
}

int ballum_clamp_brightness(int percent) {
    if (percent < BALLUM_BRIGHTNESS_MIN) return BALLUM_BRIGHTNESS_MIN;
    if (percent > BALLUM_BRIGHTNESS_MAX) return BALLUM_BRIGHTNESS_MAX;
    return percent;
}

uint32_t ballum_clamp_warmth(uint32_t kelvin) {
    if (kelvin < BALLUM_WARMTH_MIN_KELVIN) return BALLUM_WARMTH_MIN_KELVIN;
    if (kelvin > BALLUM_WARMTH_MAX_KELVIN) return BALLUM_WARMTH_MAX_KELVIN;
    return kelvin;
}

uint32_t ballum_clamp_gamma(uint32_t gamma_scaled) {
    if (gamma_scaled < BALLUM_GAMMA_MIN) return BALLUM_GAMMA_MIN;
    if (gamma_scaled > BALLUM_GAMMA_MAX) return BALLUM_GAMMA_MAX;
    return gamma_scaled;
}

/* ---- internal ------------------------------------------------------------ */

/* Find the cached display with id `id`, or NULL. */
static const ballum_display_t *find_display(const ballum_manager_t *manager, uint32_t id) {
    if (!manager) return NULL;
    for (size_t i = 0; i < manager->display_count; ++i)
        if (manager->displays[i].id == id) return &manager->displays[i];
    return NULL;
}

static int point_in_display(const ballum_display_t *d, ballum_point_t at) {
    return at.x < d->width && at.y < d->height;
}

static int rect_in_display(const ballum_display_t *d, ballum_rect_t r) {
    /* Guard against overflow on x+width / y+height. */
    if (r.width == 0 || r.height == 0) return 0;
    if (r.x >= d->width || r.y >= d->height) return 0;
    if (r.width > d->width - r.x) return 0;
    if (r.height > d->height - r.y) return 0;
    return 1;
}

/* ---- lifecycle ----------------------------------------------------------- */

ballum_status_t ballum_manager_init(ballum_manager_t *manager,
                                    const ballum_backend_t *backend,
                                    void *backend_context) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    memset(manager, 0, sizeof(*manager));
    manager->backend = backend ? backend : ballum_backend_default();
    manager->backend_context = backend_context;
    manager->display_count = 0;
    manager->enumerate_generation = 0;
    manager->initialized = 1;
    if (!manager->backend) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    return BALLUM_STATUS_OK;
}

/* ---- enumeration --------------------------------------------------------- */

ballum_status_t ballum_manager_enumerate(ballum_manager_t *manager,
                                         ballum_display_t *out, size_t capacity,
                                         size_t *out_count) {
    if (!manager || !out_count) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    if (!manager->backend || !manager->backend->enumerate)
        return BALLUM_STATUS_BACKEND_UNAVAILABLE;

    size_t count = 0;
    ballum_status_t r = manager->backend->enumerate(manager->backend_context,
                                                    manager->displays,
                                                    BALLUM_MAX_DISPLAYS, &count);
    if (r != BALLUM_STATUS_OK) {
        manager->display_count = 0;
        *out_count = 0;
        return r;
    }
    if (count > BALLUM_MAX_DISPLAYS) count = BALLUM_MAX_DISPLAYS;
    manager->display_count = count;
    manager->enumerate_generation++;

    /* Copy into the caller's buffer (VM-owned), never expose internal storage. */
    size_t n = count < capacity ? count : capacity;
    if (out && n) memcpy(out, manager->displays, n * sizeof(ballum_display_t));
    *out_count = count;
    return (count > capacity) ? BALLUM_STATUS_BUFFER_TOO_SMALL : BALLUM_STATUS_OK;
}

ballum_status_t ballum_manager_describe(const ballum_manager_t *manager,
                                        uint32_t id, ballum_display_t *out) {
    if (!manager || !out) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    *out = *d;
    return BALLUM_STATUS_OK;
}

int ballum_manager_has_capability(const ballum_manager_t *manager,
                                  uint32_t id, ballum_capability_t cap) {
    if (!manager) return -1;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return -1;
    return (d->capabilities & (uint32_t)cap) ? 1 : 0;
}

/* ---- pixel control ------------------------------------------------------- */

ballum_status_t ballum_manager_set_pixel(ballum_manager_t *manager, uint32_t id,
                                         ballum_point_t at, ballum_color_t color) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_PER_PIXEL)) return BALLUM_STATUS_UNSUPPORTED;
    if (!point_in_display(d, at)) return BALLUM_STATUS_OUT_OF_BOUNDS;
    if (!manager->backend->set_pixel) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->set_pixel(manager->backend_context, id, at, color);
}

ballum_status_t ballum_manager_get_pixel(ballum_manager_t *manager, uint32_t id,
                                         ballum_point_t at, ballum_color_t *out) {
    if (!manager || !out) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_PER_PIXEL)) return BALLUM_STATUS_UNSUPPORTED;
    if (!point_in_display(d, at)) return BALLUM_STATUS_OUT_OF_BOUNDS;
    if (!manager->backend->get_pixel) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->get_pixel(manager->backend_context, id, at, out);
}

ballum_status_t ballum_manager_blit_region(ballum_manager_t *manager, uint32_t id,
                                           ballum_rect_t rect,
                                           const ballum_color_t *pixels,
                                           size_t pixel_count) {
    if (!manager || !pixels) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_REGION_BLIT)) return BALLUM_STATUS_UNSUPPORTED;
    if (!rect_in_display(d, rect)) return BALLUM_STATUS_OUT_OF_BOUNDS;
    if (pixel_count != (size_t)rect.width * (size_t)rect.height)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->backend->blit_region) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->blit_region(manager->backend_context, id, rect,
                                         pixels, pixel_count);
}

/* ---- fullscreen surface (total per-pixel control) ------------------------ */

int ballum_manager_has_surface(const ballum_manager_t *manager, uint32_t id) {
    if (!manager) return -1;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return -1;
    if (!(d->capabilities & BALLUM_CAP_FULLSCREEN_SURFACE)) return 0;
    return (manager->backend && manager->backend->claim_surface) ? 1 : 0;
}

ballum_status_t ballum_manager_claim_surface(ballum_manager_t *manager, uint32_t id) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_FULLSCREEN_SURFACE)) return BALLUM_STATUS_UNSUPPORTED;
    if (!manager->backend->claim_surface) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->claim_surface(manager->backend_context, id);
}

ballum_status_t ballum_manager_present(ballum_manager_t *manager, uint32_t id) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!manager->backend->present) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->present(manager->backend_context, id);
}

ballum_status_t ballum_manager_release_surface(ballum_manager_t *manager, uint32_t id) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!manager->backend->release_surface) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->release_surface(manager->backend_context, id);
}

/* ---- whole-display controls ---------------------------------------------- */

ballum_status_t ballum_manager_set_brightness(ballum_manager_t *manager, uint32_t id,
                                              int percent) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_BRIGHTNESS)) return BALLUM_STATUS_UNSUPPORTED;
    if (!manager->backend->set_brightness) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->set_brightness(manager->backend_context, id,
                                            ballum_clamp_brightness(percent));
}

ballum_status_t ballum_manager_set_warmth(ballum_manager_t *manager, uint32_t id,
                                          uint32_t kelvin) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_COLOR_TEMPERATURE)) return BALLUM_STATUS_UNSUPPORTED;
    if (!manager->backend->set_warmth) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->set_warmth(manager->backend_context, id,
                                        ballum_clamp_warmth(kelvin));
}

ballum_status_t ballum_manager_set_gamma(ballum_manager_t *manager, uint32_t id,
                                         uint32_t gamma_scaled) {
    if (!manager) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    const ballum_display_t *d = find_display(manager, id);
    if (!d) return BALLUM_STATUS_DISPLAY_NOT_FOUND;
    if (!(d->capabilities & BALLUM_CAP_GAMMA)) return BALLUM_STATUS_UNSUPPORTED;
    if (!manager->backend->set_gamma) return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->set_gamma(manager->backend_context, id,
                                       ballum_clamp_gamma(gamma_scaled));
}

/* ---- pointer input ------------------------------------------------------- */

int ballum_manager_has_input(const ballum_manager_t *manager) {
    if (!manager) return -1;
    return (manager->backend && manager->backend->poll_event) ? 1 : 0;
}

ballum_status_t ballum_manager_poll_event(ballum_manager_t *manager,
                                          ballum_pointer_event_t *out) {
    if (!manager || !out) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    /* Always leave `out` well-defined, even on the no-input path. */
    memset(out, 0, sizeof(*out));
    out->type = BALLUM_EVENT_NONE;
    if (!manager->backend || !manager->backend->poll_event)
        return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->poll_event(manager->backend_context, out);
}

int ballum_manager_has_keyboard(const ballum_manager_t *manager) {
    if (!manager) return -1;
    return (manager->backend && manager->backend->poll_key) ? 1 : 0;
}

ballum_status_t ballum_manager_poll_key(ballum_manager_t *manager, int *out) {
    if (!manager || !out) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!manager->initialized) return BALLUM_STATUS_NOT_INITIALIZED;
    /* Always leave `out` well-defined, even on the no-keyboard path. */
    *out = BALLUM_KEY_NONE;
    if (!manager->backend || !manager->backend->poll_key)
        return BALLUM_STATUS_UNSUPPORTED;
    return manager->backend->poll_key(manager->backend_context, out);
}
