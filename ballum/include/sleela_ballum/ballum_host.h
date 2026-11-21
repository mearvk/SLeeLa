/*
 * ballum/include/sleela_ballum/ballum_host.h
 * Ballum Program Host -- run a real program inside the Ballum environment.
 *
 * The Ballum fullscreen surface (the white background) becomes the screen of a
 * hosted program such as a terminal or an admin interface. Ballum receives the
 * program's pixels and acts as its monitor: every keyboard and pointer event
 * arriving at Ballum is routed to the program 1:1, as if the program ran on a
 * native X11 or Windows GUI stack.
 *
 *   program (own display) --pixels--> source --frame--> HOST --blit--> Ballum surface
 *   program <--1:1 input-- source <-- router <-- HOST <--events-- Ballum surface
 *
 * Screen layout (all rectangles are exact integer pixels):
 *
 *   +--------------------------------------------------------------+
 *   | APP NAME slot                          | DATE TIME slot      |  header
 *   +--------+-----------------------------------------+-----------+
 *   | LEFT   |                                         | RIGHT     |
 *   | sibling|            PROGRAM VIEWPORT             | sibling   |
 *   |        |      (the program's exact pixels)       |           |
 *   +--------+-----------------------------------------+-----------+
 *   |                      BOTTOM sibling                          |
 *   +--------------------------------------------------------------+
 *
 * The APP NAME and DATE TIME slots sit above the program. They are component
 * slots: the host reserves and clears the space, and a registered component
 * draws into it. With nothing registered the slot stays an empty header band,
 * so the layout is already final when the components arrive. Sibling components
 * (status panels, tool strips) attach to the left, right or bottom edge the same
 * way, before the program starts, so the program's viewport size is fixed.
 *
 * Hosting is delegated to a SOURCE (ballum_host_source_t): a producer of
 * program frames and a sink for input. ballum_host_x11.h provides the real
 * source: it runs the program on a private, authenticated X display and
 * captures that display's pixels, so a terminal looks pixel-for-pixel as it
 * does on a native desktop. Tests use an in-memory source.
 *
 * Safety properties (fail closed):
 *   - The host never blocks on the program: input is polled and the escape
 *     chord honoured every step, so a hung program cannot trap the user.
 *   - Starting requires pointer AND full keyboard input from the backend;
 *     without them the user could not leave, so start() refuses.
 *   - The escape chord (default Ctrl+Alt+Esc) is never forwarded to the program.
 *   - The program is ended and the surface and input grab released on every
 *     exit path, including errors; ballum_host_stop is idempotent.
 *   - All sizes and indices are validated; the host copies program frames into
 *     memory it owns and never exposes a pointer into the compositor.
 */
#ifndef SLEELA_BALLUM_HOST_H
#define SLEELA_BALLUM_HOST_H

#include "ballum_manager.h"
#include "ballum_transition.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BALLUM_HOST_MAX_SIBLINGS      8u
#define BALLUM_HOST_MAX_KEY_REMAPS    64u
#define BALLUM_HOST_MAX_BUTTON_REMAPS 8u
#define BALLUM_HOST_MIN_VIEWPORT      64u     /* smallest program viewport, px */
#define BALLUM_HOST_MAX_PIXELS        (36u * 1000u * 1000u)   /* ~8K display */

/* X11 keysym for Escape; the default chord key. Backends that deliver full key
 * events report symbols in this vocabulary. */
#define BALLUM_KEYSYM_ESCAPE 0xFF1Bu

/* ---- drawing surface handed to components -------------------------------- */

/* A writable window into the composed frame, covering exactly the component's
 * rectangle. (0,0) is the component's top-left; `stride` is in pixels. */
typedef struct {
    ballum_color_t *px;
    uint32_t        w;
    uint32_t        h;
    uint32_t        stride;
} ballum_surface_view_t;

/* Bounds-checked helpers for component code. Out-of-range writes are ignored. */
void ballum_view_put(ballum_surface_view_t *v, int32_t x, int32_t y, ballum_color_t c);
void ballum_view_fill(ballum_surface_view_t *v, ballum_rect_t r, ballum_color_t c);

/* ---- components ----------------------------------------------------------- */

typedef void (*ballum_component_draw_fn)(void *user, ballum_surface_view_t *view);
/* Pointer event in component-local coordinates. Return nonzero if consumed. */
typedef int  (*ballum_component_input_fn)(void *user, const ballum_pointer_event_t *ev);

typedef struct {
    ballum_component_draw_fn  draw;        /* required */
    ballum_component_input_fn input;       /* optional */
    void                     *user;
    uint32_t                  refresh_ms;  /* 0 = draw on layout change only; else
                                              redraw at this interval (e.g. a clock) */
} ballum_component_t;

typedef enum {
    BALLUM_SLOT_APP_NAME = 0,   /* header, left  */
    BALLUM_SLOT_DATE_TIME,      /* header, right */
    BALLUM_SLOT__COUNT
} ballum_slot_t;

typedef enum {
    BALLUM_EDGE_LEFT = 0,
    BALLUM_EDGE_RIGHT,
    BALLUM_EDGE_BOTTOM
} ballum_edge_t;

/* ---- program source ------------------------------------------------------- */

typedef struct ballum_host_source {
    const char *name;

    /* Begin producing a program viewport of exactly width x height pixels. */
    ballum_status_t (*start)(void *context, uint32_t width, uint32_t height);

    /* Latest frame. *pixels is NULL until the program has something to show
     * (the host then waits). Otherwise it points at width*height pixels valid
     * until the next call. *changed is set when the frame differs from the one
     * returned previously. */
    ballum_status_t (*poll_frame)(void *context, const ballum_color_t **pixels,
                                  int *changed);

    /* Input, in viewport coordinates, forwarded 1:1. `button` is a
     * ballum_pointer_button_t value (1..7). `symbol` is a key symbol in the
     * backend vocabulary and `code` the physical code (0 if unknown). */
    ballum_status_t (*pointer)(void *context, int32_t x, int32_t y);
    ballum_status_t (*button)(void *context, int button, int pressed);
    ballum_status_t (*key)(void *context, uint32_t symbol, uint32_t code, int pressed);

    /* 1 while the program is running, 0 once it has ended. */
    int  (*alive)(void *context);

    /* End the program and free the source's resources. Idempotent. */
    void (*stop)(void *context);
} ballum_host_source_t;

/* ---- configuration -------------------------------------------------------- */

typedef struct {
    uint32_t modifiers;   /* BALLUM_MOD_* bits that must all be held */
    uint32_t symbol;      /* key symbol that completes the chord      */
} ballum_chord_t;

typedef struct {
    uint32_t            display_id;
    ballum_color_t      background;          /* default white               */
    ballum_color_t      header_color;        /* default #F2F2F2             */
    ballum_color_t      panel_color;         /* sibling panels, #E6E6E6     */
    uint32_t            header_height;       /* default 32; 0 = no header   */
    uint32_t            datetime_width;      /* default 280                 */
    ballum_transition_t enter;               /* default disintegrate        */
    ballum_transition_t leave;               /* default fade                */
    uint32_t            enter_ms;            /* default 700                 */
    uint32_t            leave_ms;            /* default 450                 */
    uint32_t            startup_timeout_ms;  /* default 15000               */
    uint64_t            max_runtime_ms;      /* 0 = unlimited               */
    ballum_chord_t      escape_chord;        /* default Ctrl+Alt+Esc        */
    int                 grab_input;          /* default 1                   */
    int                 require_grab;        /* default 0: a refused grab is
                                                tolerated, the chord still works
                                                through normal focus delivery */
} ballum_host_config_t;

void ballum_host_config_default(ballum_host_config_t *cfg, uint32_t display_id);

/* ---- state ---------------------------------------------------------------- */

typedef enum {
    BALLUM_HOST_IDLE = 0,     /* created, not started               */
    BALLUM_HOST_STARTING,     /* surface claimed, waiting for pixels */
    BALLUM_HOST_ENTERING,     /* entry transition playing           */
    BALLUM_HOST_RUNNING,      /* program live, input routed 1:1     */
    BALLUM_HOST_LEAVING,      /* exit transition playing            */
    BALLUM_HOST_DONE          /* everything released                */
} ballum_host_state_t;

typedef enum {
    BALLUM_EXIT_NONE = 0,
    BALLUM_EXIT_USER_CHORD,       /* the escape chord was pressed       */
    BALLUM_EXIT_PROGRAM_ENDED,    /* the program exited by itself       */
    BALLUM_EXIT_REQUESTED,        /* ballum_host_request_exit()         */
    BALLUM_EXIT_TIMEOUT,          /* max_runtime_ms elapsed             */
    BALLUM_EXIT_STARTUP_FAILED,   /* no pixels within the timeout       */
    BALLUM_EXIT_ERROR             /* a device operation failed          */
} ballum_exit_reason_t;

/* Resolved rectangles in display pixels. */
typedef struct {
    ballum_rect_t header;
    ballum_rect_t app_name;
    ballum_rect_t date_time;
    ballum_rect_t viewport;
    ballum_rect_t siblings[BALLUM_HOST_MAX_SIBLINGS];
    uint32_t      sibling_count;
} ballum_host_layout_t;

typedef struct ballum_host ballum_host_t;

/* ---- lifecycle ------------------------------------------------------------ */

/* Create a host over `manager` (already initialized and enumerated) and a
 * program source. The host borrows both; neither is freed by the host.
 * Fails with DISPLAY_NOT_FOUND, UNSUPPORTED (no fullscreen surface, region
 * blit, pointer input or full key events), or INVALID_ARGUMENT. */
ballum_status_t ballum_host_create(ballum_manager_t *manager,
                                   const ballum_host_config_t *cfg,
                                   const ballum_host_source_t *source,
                                   void *source_context,
                                   ballum_host_t **out);

/* Stops the host if still running, then frees it. NULL is allowed. */
void ballum_host_destroy(ballum_host_t *host);

/* Register or clear (component == NULL) a header slot. Allowed in any state. */
ballum_status_t ballum_host_set_slot(ballum_host_t *host, ballum_slot_t slot,
                                     const ballum_component_t *component);
int ballum_host_slot_registered(const ballum_host_t *host, ballum_slot_t slot);

/* Attach a sibling component of `size` pixels to an edge. Only before start,
 * because it fixes the program viewport. INVALID_ARGUMENT if the viewport would
 * fall below BALLUM_HOST_MIN_VIEWPORT or the sibling table is full. */
ballum_status_t ballum_host_add_sibling(ballum_host_t *host, ballum_edge_t edge,
                                        uint32_t size,
                                        const ballum_component_t *component);

/* Application remapping (applied before routing). A key rule matches when the
 * event's symbol equals `from` and every bit of `required_modifiers` is held;
 * `to` == 0 drops the key. A button rule maps button `from` to `to`
 * (0 drops it). First match wins. Allowed in any state. */
ballum_status_t ballum_host_remap_key(ballum_host_t *host, uint32_t from,
                                      uint32_t required_modifiers, uint32_t to);
ballum_status_t ballum_host_remap_button(ballum_host_t *host, int from, int to);

/* Compute the layout the host will use. */
ballum_status_t ballum_host_layout(const ballum_host_t *host, ballum_host_layout_t *out);

/* Claim the surface, start the program source at the viewport size and begin
 * waiting for its first frame. */
ballum_status_t ballum_host_start(ballum_host_t *host, uint64_t now_ms);

/* Begin leaving: plays the exit transition, then releases everything. */
void ballum_host_request_exit(ballum_host_t *host, ballum_exit_reason_t reason,
                              uint64_t now_ms);

/* Advance the host one frame: poll input and route it, pull a program frame,
 * advance transitions, redraw what changed and present it. `now_ms` is a
 * monotonic clock in milliseconds supplied by the caller. Returns OK normally;
 * check ballum_host_state() for DONE. */
ballum_status_t ballum_host_step(ballum_host_t *host, uint64_t now_ms);

/* Release the program, input grab and surface immediately (no transition).
 * Idempotent. */
void ballum_host_stop(ballum_host_t *host);

/* Convenience loop for command-line use: start, then step at about `fps` until
 * DONE. Uses CLOCK_MONOTONIC. */
ballum_status_t ballum_host_run(ballum_host_t *host, unsigned fps);

/* ---- queries -------------------------------------------------------------- */

ballum_host_state_t  ballum_host_state(const ballum_host_t *host);
ballum_exit_reason_t ballum_host_exit_reason(const ballum_host_t *host);
uint64_t             ballum_host_presented_frames(const ballum_host_t *host);
const char *ballum_host_state_name(ballum_host_state_t state);
const char *ballum_exit_reason_name(ballum_exit_reason_t reason);

/* ---- real sources --------------------------------------------------------- */

/* Description of the program a real source launches. */
typedef struct {
    const char *const *argv;          /* argv[0] is resolved against the trusted
                                         search path; NULL-terminated        */
    const char *const *extra_env;     /* optional "NAME=value" strings       */
    const char        *title;         /* informational, e.g. the app name    */
    int                sandbox;       /* 1 (default): confine the program    */
    int                log_fd;        /* program stdout/stderr, -1 = discard */
} ballum_program_t;

/* Create a source that runs `program` on a private X display (Xvfb) and
 * captures it. Needs Linux/X11 at build time and an Xvfb binary at run time;
 * otherwise returns UNSUPPORTED / BACKEND_UNAVAILABLE and nothing is started.
 * *source is filled for ballum_host_create; free *ctx_out with
 * ballum_host_source_x11_destroy after the host is destroyed. */
ballum_status_t ballum_host_source_x11_create(const ballum_program_t *program,
                                              ballum_host_source_t *source,
                                              void **ctx_out);
void ballum_host_source_x11_destroy(void *context);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_HOST_H */
