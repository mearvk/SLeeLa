#ifndef SLEELA_BALLUM_MANAGER_H
#define SLEELA_BALLUM_MANAGER_H

/*
 * Ballum(TM) -- pixel-level control over the screen/monitor of the native OS.
 *
 * Ballum gives a SLeeLa program direct, bounded control of the display: set an
 * individual pixel's color, read/blit a region, and adjust whole-display
 * brightness, warmth (color temperature), and gamma. It targets macOS, Linux,
 * and Windows 10+ through one portable manager over a per-OS backend vtable,
 * exactly like wifi-common/ abstracts the radio.
 *
 * Design rules (carried from the repo's established native subsystems):
 *   - Open standards only on each OS (X11/XRANDR + DDC/CI on Linux, Core
 *     Graphics / IOKit on macOS, DXGI + the Win32 gamma ramp on Windows 10+).
 *   - Exactly one backend is compiled in per build; a graceful "unavailable"
 *     backend is always available so calls fail closed, never crash.
 *   - The manager copies results into caller/VM-owned memory and never hands
 *     out internal pointers.
 *   - Every setter is bounded and validated before it touches the device.
 *
 * This header is the single C ABI the SLeeLa bridge and the standalone tests
 * build against.
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- limits -------------------------------------------------------------- */

#define BALLUM_MAX_DISPLAYS        16u
#define BALLUM_DISPLAY_NAME_BYTES  64u
#define BALLUM_BACKEND_NAME_BYTES  32u

/* Percent controls (brightness) are 0..100. */
#define BALLUM_BRIGHTNESS_MIN      0
#define BALLUM_BRIGHTNESS_MAX      100

/* Color temperature / warmth, in kelvin. 6500K is the neutral D65 white point;
 * lower is warmer (toward amber), higher is cooler (toward blue). */
#define BALLUM_WARMTH_MIN_KELVIN   1000u
#define BALLUM_WARMTH_NEUTRAL_K    6500u
#define BALLUM_WARMTH_MAX_KELVIN   10000u

/* Gamma is a fixed-point value scaled by 100 (so 2.20 gamma == 220). */
#define BALLUM_GAMMA_SCALE         100u
#define BALLUM_GAMMA_MIN           10u    /* 0.10 */
#define BALLUM_GAMMA_MAX           1000u  /* 10.00 */

/* ---- status -------------------------------------------------------------- */

typedef enum {
    BALLUM_STATUS_OK = 0,
    BALLUM_STATUS_INVALID_ARGUMENT,
    BALLUM_STATUS_BACKEND_UNAVAILABLE,
    BALLUM_STATUS_BACKEND_ERROR,
    BALLUM_STATUS_DISPLAY_NOT_FOUND,
    BALLUM_STATUS_OUT_OF_BOUNDS,         /* pixel/region outside the display   */
    BALLUM_STATUS_BUFFER_TOO_SMALL,
    BALLUM_STATUS_UNSUPPORTED,           /* backend lacks this capability       */
    BALLUM_STATUS_PERMISSION_DENIED,     /* OS refused the display operation    */
    BALLUM_STATUS_NOT_INITIALIZED
} ballum_status_t;

/* Capability flags a backend advertises per display. A SLeeLa program checks
 * these before enabling a control, so a missing capability fails cleanly
 * instead of silently doing nothing. */
typedef enum {
    BALLUM_CAP_NONE              = 0,
    BALLUM_CAP_PER_PIXEL         = 1 << 0, /* set/get individual pixels         */
    BALLUM_CAP_REGION_BLIT       = 1 << 1, /* blit a rectangle of pixels        */
    BALLUM_CAP_BRIGHTNESS        = 1 << 2, /* whole-display brightness          */
    BALLUM_CAP_COLOR_TEMPERATURE = 1 << 3, /* warmth / color temperature        */
    BALLUM_CAP_GAMMA             = 1 << 4, /* gamma ramp                        */
    BALLUM_CAP_POINTER_INPUT     = 1 << 5, /* mouse/touch pointer events        */
    BALLUM_CAP_TOUCH_INPUT       = 1 << 6, /* multi-touch is reported           */
    BALLUM_CAP_FULLSCREEN_SURFACE = 1 << 7 /* claim a fullscreen framebuffer    */
} ballum_capability_t;

/* ---- pointer input ------------------------------------------------------- */

/* Raw pointer event kinds the backend reports. Mouse and touch are unified: a
 * touch contact maps to DOWN/MOVE/UP with source BALLUM_POINTER_TOUCH. Higher
 * gestures (click, drag) are synthesized above this layer, not reported here. */
typedef enum {
    BALLUM_EVENT_NONE = 0,
    BALLUM_EVENT_POINTER_DOWN,   /* button pressed / finger touched down */
    BALLUM_EVENT_POINTER_MOVE,   /* pointer/finger moved                 */
    BALLUM_EVENT_POINTER_UP      /* button released / finger lifted      */
} ballum_event_type_t;

/* Backend-neutral key codes a session acts on. The set is intentionally small:
 * a Ballum session's one hard control is ESC (exit). Per-OS backends map their
 * native keysyms (X11 keysym, Win32 VK, macOS key code) onto these. Printable
 * ASCII may be reported as its own code (e.g. 'a'); 0 means no key. */
typedef enum {
    BALLUM_KEY_NONE  = 0,
    BALLUM_KEY_ESC   = 27,  /* ASCII ESC -- exit a full-screen session        */
    BALLUM_KEY_ENTER = 13,
    BALLUM_KEY_SPACE = 32
} ballum_key_t;

/* Which physical button, matching the SLUI pointer-button vocabulary. For a
 * touch contact the button is reported as LEFT. */
typedef enum {
    BALLUM_BUTTON_NONE = 0,
    BALLUM_BUTTON_LEFT = 1,
    BALLUM_BUTTON_MIDDLE = 2,
    BALLUM_BUTTON_RIGHT = 3
} ballum_pointer_button_t;

/* Where the event came from. */
typedef enum {
    BALLUM_POINTER_MOUSE = 0,
    BALLUM_POINTER_TOUCH = 1
} ballum_pointer_source_t;

/* Modifier bitmask, matching SLUI_MOD_*. */
enum {
    BALLUM_MOD_SHIFT = 1u << 0,
    BALLUM_MOD_CTRL  = 1u << 1,
    BALLUM_MOD_ALT   = 1u << 2,
    BALLUM_MOD_SUPER = 1u << 3
};

/* One raw pointer event in display pixel coordinates (top-left origin). */
typedef struct {
    ballum_event_type_t     type;
    uint32_t                display_id;   /* display the event occurred on   */
    int32_t                 x;            /* pixel x (may be <0 if off-edge)  */
    int32_t                 y;            /* pixel y                          */
    ballum_pointer_button_t button;       /* for DOWN/UP; LEFT for touch      */
    ballum_pointer_source_t source;       /* mouse or touch                   */
    uint32_t                contact_id;   /* touch contact id (0 for mouse)   */
    uint32_t                modifiers;    /* BALLUM_MOD_* bitmask             */
} ballum_pointer_event_t;

/* ---- value types --------------------------------------------------------- */

/* A single pixel: 8-bit RGBA, straight (non-premultiplied) alpha. */
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} ballum_color_t;

/* A pixel coordinate on a display (top-left origin). */
typedef struct {
    uint32_t x;
    uint32_t y;
} ballum_point_t;

/* A rectangle of pixels. */
typedef struct {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} ballum_rect_t;

/* Static description of one connected display. */
typedef struct {
    uint32_t id;                                /* backend-stable display id    */
    char     name[BALLUM_DISPLAY_NAME_BYTES];   /* human-readable, NUL-terminated */
    uint32_t width;                             /* pixels                        */
    uint32_t height;                            /* pixels                        */
    uint32_t refresh_mhz;                       /* refresh rate in milli-Hz      */
    int      is_primary;                        /* 1 if the primary display      */
    uint32_t capabilities;                      /* OR of ballum_capability_t     */
} ballum_display_t;

/* ---- backend vtable ------------------------------------------------------ */

/*
 * One backend is compiled in per build (per-OS). The portable manager calls
 * through this vtable; any entry may be NULL, in which case the manager returns
 * BALLUM_STATUS_UNSUPPORTED for that operation rather than dereferencing NULL.
 */
typedef struct ballum_backend {
    const char *name;

    /* Enumerate displays into `out` (capacity entries); sets *out_count. */
    ballum_status_t (*enumerate)(void *context, ballum_display_t *out,
                                 size_t capacity, size_t *out_count);

    /* Per-pixel. */
    ballum_status_t (*set_pixel)(void *context, uint32_t display_id,
                                 ballum_point_t at, ballum_color_t color);
    ballum_status_t (*get_pixel)(void *context, uint32_t display_id,
                                 ballum_point_t at, ballum_color_t *out);

    /* Blit a tightly-packed RGBA buffer of rect.width*rect.height pixels. */
    ballum_status_t (*blit_region)(void *context, uint32_t display_id,
                                   ballum_rect_t rect, const ballum_color_t *pixels,
                                   size_t pixel_count);

    /* Whole-display controls. brightness is 0..100; warmth is kelvin;
     * gamma is scaled by BALLUM_GAMMA_SCALE. */
    ballum_status_t (*set_brightness)(void *context, uint32_t display_id, int percent);
    ballum_status_t (*set_warmth)(void *context, uint32_t display_id, uint32_t kelvin);
    ballum_status_t (*set_gamma)(void *context, uint32_t display_id, uint32_t gamma_scaled);

    /* Poll one pending pointer (mouse/touch) event without blocking. Writes the
     * event through `out` and returns BALLUM_STATUS_OK when one was dequeued, or
     * BALLUM_STATUS_UNSUPPORTED when the backend has no input queue. When no
     * event is pending it returns BALLUM_STATUS_OK with out->type ==
     * BALLUM_EVENT_NONE. May be NULL (input not provided). */
    ballum_status_t (*poll_event)(void *context, ballum_pointer_event_t *out);

    /* Poll one pending key event without blocking. Writes the key through `out`
     * and returns BALLUM_STATUS_OK when one was dequeued; *out == BALLUM_KEY_NONE
     * means no key was pending. Returns BALLUM_STATUS_UNSUPPORTED when the
     * backend has no keyboard queue. May be NULL (keyboard not provided), in
     * which case the manager reports no keys -- fail closed, never a false key.
     * Key codes are the backend-neutral ballum_key_t set (ESC is the one a
     * session needs to exit); per-OS backends map their native keysyms onto it. */
    ballum_status_t (*poll_key)(void *context, int *out);

    /* ---- Fullscreen surface (the World-Class per-pixel path) -------------
     * These three turn Ballum from "draw on the live desktop (transient,
     * compositor may overpaint)" into TOTAL per-pixel control: the backend
     * creates an override-redirect, always-on-top fullscreen surface backed by
     * an in-memory RGBA framebuffer, so the program owns every pixel and the
     * window manager cannot paint over it.
     *
     * claim_surface : create and map the fullscreen surface for `display_id`.
     *                 While a surface is claimed, set_pixel/get_pixel/blit
     *                 operate on its framebuffer (O(1) / memcpy -- no X calls),
     *                 and `present` pushes the whole framebuffer to the screen
     *                 in ONE XPutImage (MIT-SHM when available), not per-pixel.
     * present       : push the framebuffer to the surface in one shot.
     * release_surface: unmap and destroy the surface; drawing reverts to the
     *                 live-desktop fallback (if the backend offers one).
     *
     * Any of these may be NULL: the manager then reports
     * BALLUM_STATUS_UNSUPPORTED and BALLUM_CAP_FULLSCREEN_SURFACE is absent. */
    ballum_status_t (*claim_surface)(void *context, uint32_t display_id);
    ballum_status_t (*present)(void *context, uint32_t display_id);
    ballum_status_t (*release_surface)(void *context, uint32_t display_id);
} ballum_backend_t;

/* The backend compiled in for this build (never NULL; may be the unavailable
 * backend when no OS backend is selected). */
const ballum_backend_t *ballum_backend_default(void);

/* 1 if the compiled-in backend is a real OS backend, 0 if it is the
 * unavailable fallback. */
int ballum_backend_supported(void);

/* ---- manager ------------------------------------------------------------- */

typedef struct ballum_manager {
    const ballum_backend_t *backend;
    void                   *backend_context;
    ballum_display_t        displays[BALLUM_MAX_DISPLAYS];
    size_t                  display_count;
    uint64_t                enumerate_generation;
    int                     initialized;
} ballum_manager_t;

/* Initialize a manager over a backend (NULL selects ballum_backend_default()). */
ballum_status_t ballum_manager_init(ballum_manager_t *manager,
                                    const ballum_backend_t *backend,
                                    void *backend_context);

/* Refresh the manager's display list; fills `out` (capacity entries) and
 * *out_count with the enumerated displays, copied into caller memory. */
ballum_status_t ballum_manager_enumerate(ballum_manager_t *manager,
                                         ballum_display_t *out, size_t capacity,
                                         size_t *out_count);

/* Copy the cached description of display `id` into `out`. */
ballum_status_t ballum_manager_describe(const ballum_manager_t *manager,
                                        uint32_t id, ballum_display_t *out);

/* Does display `id` advertise capability `cap`? 1 yes, 0 no, <0 on bad args. */
int ballum_manager_has_capability(const ballum_manager_t *manager,
                                   uint32_t id, ballum_capability_t cap);

/* Pixel control (validated against the cached display bounds + capability). */
ballum_status_t ballum_manager_set_pixel(ballum_manager_t *manager, uint32_t id,
                                         ballum_point_t at, ballum_color_t color);
ballum_status_t ballum_manager_get_pixel(ballum_manager_t *manager, uint32_t id,
                                         ballum_point_t at, ballum_color_t *out);
ballum_status_t ballum_manager_blit_region(ballum_manager_t *manager, uint32_t id,
                                           ballum_rect_t rect,
                                           const ballum_color_t *pixels,
                                           size_t pixel_count);

/* ---- fullscreen surface (total per-pixel control) ------------------------ */

/* Claim an override-redirect, always-on-top fullscreen surface over display
 * `id`, backed by an in-memory RGBA framebuffer the program owns. While
 * claimed, set_pixel/get_pixel/blit operate on the framebuffer (fast, no
 * per-pixel device calls) and ballum_manager_present() pushes it to the screen
 * in one shot. Validated against the display's BALLUM_CAP_FULLSCREEN_SURFACE
 * capability. Returns BALLUM_STATUS_UNSUPPORTED when the backend cannot. */
ballum_status_t ballum_manager_claim_surface(ballum_manager_t *manager, uint32_t id);

/* Present the framebuffer of a claimed surface to the screen (one XPutImage on
 * X11, SHM when available). */
ballum_status_t ballum_manager_present(ballum_manager_t *manager, uint32_t id);

/* Release a claimed surface; drawing reverts to the backend's live-desktop
 * fallback, if any. */
ballum_status_t ballum_manager_release_surface(ballum_manager_t *manager, uint32_t id);

/* Does display `id` advertise a claimable fullscreen surface? 1/0/<0. */
int ballum_manager_has_surface(const ballum_manager_t *manager, uint32_t id);

/* Whole-display controls (validated against the documented ranges). */
ballum_status_t ballum_manager_set_brightness(ballum_manager_t *manager, uint32_t id,
                                              int percent);
ballum_status_t ballum_manager_set_warmth(ballum_manager_t *manager, uint32_t id,
                                          uint32_t kelvin);
ballum_status_t ballum_manager_set_gamma(ballum_manager_t *manager, uint32_t id,
                                         uint32_t gamma_scaled);

/* ---- pointer input ------------------------------------------------------- */

/* Poll one pending pointer (mouse/touch) event, non-blocking. On success *out
 * is filled; out->type == BALLUM_EVENT_NONE means no event was pending. Returns
 * BALLUM_STATUS_UNSUPPORTED when the backend provides no input queue. */
ballum_status_t ballum_manager_poll_event(ballum_manager_t *manager,
                                          ballum_pointer_event_t *out);

/* Does the backend provide pointer input at all? 1 yes, 0 no, <0 on bad args. */
int ballum_manager_has_input(const ballum_manager_t *manager);

/* ---- keyboard input ------------------------------------------------------ */

/* Poll one pending key, non-blocking. On success *out holds a ballum_key_t
 * (0 == BALLUM_KEY_NONE means nothing pending). Returns BALLUM_STATUS_UNSUPPORTED
 * when the backend has no keyboard queue. Fails closed: a backend without a
 * poll_key reports no keys rather than a false one. */
ballum_status_t ballum_manager_poll_key(ballum_manager_t *manager, int *out);

/* Does the backend provide keyboard input at all? 1 yes, 0 no, <0 on bad args. */
int ballum_manager_has_keyboard(const ballum_manager_t *manager);

/* ---- helpers ------------------------------------------------------------- */

const char *ballum_status_name(ballum_status_t status);
const char *ballum_event_type_name(ballum_event_type_t type);
ballum_color_t ballum_color_rgb(uint8_t r, uint8_t g, uint8_t b);
ballum_color_t ballum_color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/* Clamp helpers used by both the manager and backends. */
int      ballum_clamp_brightness(int percent);
uint32_t ballum_clamp_warmth(uint32_t kelvin);
uint32_t ballum_clamp_gamma(uint32_t gamma_scaled);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_MANAGER_H */
