/* Portable tests for the program host: layout, lifecycle, transitions, 1:1 input
 * routing, remapping, components and fail-closed behavior. A mock backend
 * stands in for the display and a mock source stands in for the program. */
#include "sleela_ballum/ballum_host.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DW 640u
#define DH 480u

/* ---- mock backend --------------------------------------------------------- */
static ballum_color_t g_fb[DW * DH];
static int g_claims, g_releases, g_presents, g_grab_calls, g_grab_on, g_surface_up;
static ballum_rect_t g_last_blit;
static int g_blit_fail, g_has_keys, g_has_surface;
static ballum_pointer_event_t g_pq[64]; static int g_pq_n, g_pq_r;
static ballum_key_event_t g_kq[64];     static int g_kq_n, g_kq_r;

static ballum_status_t m_enum(void *c, ballum_display_t *o, size_t cap, size_t *n) {
    (void)c;
    if (cap < 1) return BALLUM_STATUS_BUFFER_TOO_SMALL;
    memset(&o[0], 0, sizeof(o[0]));
    o[0].id = 7; strcpy(o[0].name, "mock"); o[0].width = DW; o[0].height = DH; o[0].is_primary = 1;
    o[0].capabilities = BALLUM_CAP_PER_PIXEL | BALLUM_CAP_REGION_BLIT | BALLUM_CAP_POINTER_INPUT |
                        (g_has_surface ? BALLUM_CAP_FULLSCREEN_SURFACE : 0);
    *n = 1; return BALLUM_STATUS_OK;
}
static ballum_status_t m_blit(void *c, uint32_t id, ballum_rect_t r, const ballum_color_t *px, size_t n) {
    (void)c; (void)id;
    if (g_blit_fail) return BALLUM_STATUS_BACKEND_ERROR;
    assert(n == (size_t)r.width * r.height);
    for (uint32_t y = 0; y < r.height; ++y)
        memcpy(&g_fb[(size_t)(r.y + y) * DW + r.x], &px[(size_t)y * r.width], r.width * sizeof(ballum_color_t));
    g_last_blit = r;
    return BALLUM_STATUS_OK;
}
static ballum_status_t m_poll_event(void *c, ballum_pointer_event_t *o) {
    (void)c;
    if (g_pq_r < g_pq_n) *o = g_pq[g_pq_r++]; else o->type = BALLUM_EVENT_NONE;
    return BALLUM_STATUS_OK;
}
static ballum_status_t m_poll_key_event(void *c, ballum_key_event_t *o) {
    (void)c;
    if (g_kq_r < g_kq_n) *o = g_kq[g_kq_r++]; else memset(o, 0, sizeof(*o));
    return BALLUM_STATUS_OK;
}
static ballum_status_t m_claim(void *c, uint32_t id) { (void)c; (void)id; g_claims++; g_surface_up = 1; return BALLUM_STATUS_OK; }
static ballum_status_t m_present(void *c, uint32_t id) { (void)c; (void)id; g_presents++; return BALLUM_STATUS_OK; }
static ballum_status_t m_release(void *c, uint32_t id) { (void)c; (void)id; g_releases++; g_surface_up = 0; return BALLUM_STATUS_OK; }
static ballum_status_t m_grab(void *c, int on) { (void)c; g_grab_calls++; g_grab_on = on; return BALLUM_STATUS_OK; }

static ballum_backend_t g_backend = {
    "mock", m_enum, NULL, NULL, m_blit, NULL, NULL, NULL, m_poll_event, NULL,
    m_claim, m_present, m_release, m_poll_key_event, m_grab
};

static void push_ptr(ballum_event_type_t t, int x, int y, int button, unsigned mods) {
    ballum_pointer_event_t e; memset(&e, 0, sizeof(e));
    e.type = t; e.x = x; e.y = y; e.button = (ballum_pointer_button_t)button; e.modifiers = mods;
    g_pq[g_pq_n++] = e;
}
static void push_key(unsigned sym, unsigned code, int pressed, unsigned mods) {
    ballum_key_event_t k = { sym, code, pressed, mods };
    g_kq[g_kq_n++] = k;
}

/* ---- mock source ----------------------------------------------------------- */
typedef struct {
    uint32_t w, h; int started, stopped, alive, ready, changed, start_fail, poll_fail;
    ballum_color_t *frame;
    int n_ptr, n_btn, n_key;
    int32_t px, py;
    int btn[32], btn_down[32]; int bi;
    struct { unsigned sym, code; int down; } keys[32]; int ki;
} msrc_t;
static msrc_t g_src;

static ballum_status_t s_start(void *c, uint32_t w, uint32_t h) {
    msrc_t *s = c; if (s->start_fail) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    s->w = w; s->h = h; s->started = 1; s->alive = 1;
    s->frame = malloc((size_t)w * h * sizeof(ballum_color_t));
    return BALLUM_STATUS_OK;
}
static ballum_status_t s_poll(void *c, const ballum_color_t **px, int *chg) {
    msrc_t *s = c; if (s->poll_fail) return BALLUM_STATUS_BACKEND_ERROR;
    *px = s->ready ? s->frame : NULL; *chg = s->changed; s->changed = 0; return BALLUM_STATUS_OK;
}
static ballum_status_t s_ptr(void *c, int32_t x, int32_t y) { msrc_t *s = c; s->px = x; s->py = y; s->n_ptr++; return BALLUM_STATUS_OK; }
static ballum_status_t s_btn(void *c, int b, int down) {
    msrc_t *s = c; s->btn[s->bi] = b; s->btn_down[s->bi++] = down; s->n_btn++; return BALLUM_STATUS_OK;
}
static ballum_status_t s_key(void *c, uint32_t sym, uint32_t code, int down) {
    msrc_t *s = c; s->keys[s->ki].sym = sym; s->keys[s->ki].code = code; s->keys[s->ki++].down = down; s->n_key++;
    return BALLUM_STATUS_OK;
}
static int  s_alive(void *c) { return ((msrc_t *)c)->alive; }
static void s_stop(void *c) { msrc_t *s = c; s->stopped++; s->alive = 0; free(s->frame); s->frame = NULL; }
static const ballum_host_source_t k_src = { "mock", s_start, s_poll, s_ptr, s_btn, s_key, s_alive, s_stop };

static void set_frame(ballum_color_t c) {
    for (size_t i = 0; i < (size_t)g_src.w * g_src.h; ++i) g_src.frame[i] = c;
    g_src.ready = 1; g_src.changed = 1;
}

/* ---- components ------------------------------------------------------------ */
static int g_name_draws, g_clock_draws, g_comp_in_n; static ballum_pointer_event_t g_comp_in;
static void draw_name(void *u, ballum_surface_view_t *v) {
    (void)u; g_name_draws++;
    ballum_rect_t big = { 1, 1, 100000, 100000 };               /* must be clipped to the view */
    ballum_view_fill(v, big, ballum_color_rgb(10, 200, 30));
}
static void draw_clock(void *u, ballum_surface_view_t *v) {
    (void)u; g_clock_draws++; ballum_view_put(v, 0, 0, ballum_color_rgb(200, 10, 10));
    ballum_view_put(v, -1, 0, ballum_color_rgb(1, 1, 1)); ballum_view_put(v, 0, 99999, ballum_color_rgb(1, 1, 1));
}
static int comp_input(void *u, const ballum_pointer_event_t *e) { (void)u; g_comp_in = *e; g_comp_in_n++; return 1; }
static void draw_panel(void *u, ballum_surface_view_t *v) { (void)u; ballum_view_put(v, 0, 0, ballum_color_rgb(1, 2, 3)); }

/* ---- harness ---------------------------------------------------------------- */
static ballum_manager_t g_mgr;
static ballum_host_t *g_host;

static ballum_host_config_t quick_cfg(const char *enter) {
    ballum_host_config_t c; ballum_host_config_default(&c, 7);
    ballum_transition_by_name(enter, &c.enter);
    ballum_transition_by_name("fade", &c.leave);
    c.enter_ms = 100; c.leave_ms = 100; c.startup_timeout_ms = 1000;
    return c;
}
static void reset(const ballum_host_config_t *cfg) {
    if (g_host) { ballum_host_destroy(g_host); g_host = NULL; }
    memset(&g_src, 0, sizeof(g_src));
    memset(g_fb, 0, sizeof(g_fb));
    g_claims = g_releases = g_presents = g_grab_calls = g_grab_on = g_surface_up = 0;
    g_pq_n = g_pq_r = g_kq_n = g_kq_r = 0; g_blit_fail = 0;
    g_has_keys = 1; g_has_surface = 1;
    g_name_draws = g_clock_draws = g_comp_in_n = 0;
    g_backend.poll_key_event = m_poll_key_event;
    assert(ballum_manager_init(&g_mgr, &g_backend, NULL) == BALLUM_STATUS_OK);
    ballum_display_t d[2]; size_t n;
    assert(ballum_manager_enumerate(&g_mgr, d, 2, &n) == BALLUM_STATUS_OK && n == 1);
    assert(ballum_host_create(&g_mgr, cfg, &k_src, &g_src, &g_host) == BALLUM_STATUS_OK);
}
static int same(ballum_color_t a, ballum_color_t b) { return a.r == b.r && a.g == b.g && a.b == b.b; }
static ballum_color_t px(uint32_t x, uint32_t y) { return g_fb[(size_t)y * DW + x]; }

/* Drive a host to RUNNING with a solid frame. */
static uint64_t to_running(ballum_color_t frame) {
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_OK);
    set_frame(frame);
    assert(ballum_host_step(g_host, 10) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_ENTERING);
    assert(ballum_host_step(g_host, 200) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_RUNNING);
    return 200;
}

int main(void) {
    const ballum_color_t WHITE = ballum_color_rgb(255, 255, 255);
    const ballum_color_t HDR = ballum_color_rgb(0xF2, 0xF2, 0xF2);
    const ballum_color_t RED = ballum_color_rgb(220, 20, 30);
    ballum_host_config_t cfg;

    /* ---- create: every precondition fails closed --------------------------- */
    cfg = quick_cfg("fade");
    reset(&cfg);
    ballum_host_t *tmp = NULL;
    g_backend.poll_key_event = NULL;                                      /* no way to read keys */
    assert(ballum_manager_init(&g_mgr, &g_backend, NULL) == BALLUM_STATUS_OK);
    { ballum_display_t d[2]; size_t n; assert(ballum_manager_enumerate(&g_mgr, d, 2, &n) == BALLUM_STATUS_OK); }
    assert(ballum_host_create(&g_mgr, &cfg, &k_src, &g_src, &tmp) == BALLUM_STATUS_UNSUPPORTED && !tmp);
    g_backend.poll_key_event = m_poll_key_event;
    g_has_surface = 0;                                                     /* no fullscreen surface */
    assert(ballum_manager_init(&g_mgr, &g_backend, NULL) == BALLUM_STATUS_OK);
    { ballum_display_t d[2]; size_t n; assert(ballum_manager_enumerate(&g_mgr, d, 2, &n) == BALLUM_STATUS_OK); }
    assert(ballum_host_create(&g_mgr, &cfg, &k_src, &g_src, &tmp) == BALLUM_STATUS_UNSUPPORTED);
    g_has_surface = 1;
    assert(ballum_manager_init(&g_mgr, &g_backend, NULL) == BALLUM_STATUS_OK);
    { ballum_display_t d[2]; size_t n; assert(ballum_manager_enumerate(&g_mgr, d, 2, &n) == BALLUM_STATUS_OK); }
    ballum_host_source_t bad = k_src; bad.key = NULL;
    assert(ballum_host_create(&g_mgr, &cfg, &bad, &g_src, &tmp) == BALLUM_STATUS_INVALID_ARGUMENT);
    assert(ballum_host_create(NULL, &cfg, &k_src, &g_src, &tmp) == BALLUM_STATUS_INVALID_ARGUMENT);
    cfg.display_id = 99;
    assert(ballum_host_create(&g_mgr, &cfg, &k_src, &g_src, &tmp) == BALLUM_STATUS_DISPLAY_NOT_FOUND);
    cfg = quick_cfg("fade"); cfg.escape_chord.symbol = 0;                  /* no way out is refused */
    assert(ballum_host_create(&g_mgr, &cfg, &k_src, &g_src, &tmp) == BALLUM_STATUS_INVALID_ARGUMENT);
    cfg = quick_cfg("fade"); cfg.header_height = DH;                       /* header eats the screen */
    assert(ballum_host_create(&g_mgr, &cfg, &k_src, &g_src, &tmp) == BALLUM_STATUS_INVALID_ARGUMENT);

    /* ---- layout ------------------------------------------------------------- */
    cfg = quick_cfg("fade");
    reset(&cfg);
    ballum_host_layout_t L;
    assert(ballum_host_layout(g_host, &L) == BALLUM_STATUS_OK);
    assert(L.header.x == 0 && L.header.y == 0 && L.header.width == DW && L.header.height == 32);
    assert(L.app_name.x == 0 && L.app_name.width == DW - 280 && L.app_name.height == 32);
    assert(L.date_time.x == DW - 280 && L.date_time.width == 280 && L.date_time.height == 32);
    assert(L.viewport.x == 0 && L.viewport.y == 32 && L.viewport.width == DW && L.viewport.height == DH - 32);
    ballum_component_t panel = { draw_panel, comp_input, NULL, 0 };
    assert(ballum_host_add_sibling(g_host, BALLUM_EDGE_LEFT, 100, &panel) == BALLUM_STATUS_OK);
    assert(ballum_host_add_sibling(g_host, BALLUM_EDGE_RIGHT, 60, &panel) == BALLUM_STATUS_OK);
    assert(ballum_host_add_sibling(g_host, BALLUM_EDGE_BOTTOM, 40, &panel) == BALLUM_STATUS_OK);
    assert(ballum_host_layout(g_host, &L) == BALLUM_STATUS_OK);
    assert(L.sibling_count == 3);
    assert(L.siblings[0].x == 0 && L.siblings[0].y == 32 && L.siblings[0].width == 100 && L.siblings[0].height == DH - 32);
    assert(L.siblings[1].x == DW - 60 && L.siblings[1].width == 60 && L.siblings[1].height == DH - 32);
    assert(L.siblings[2].x == 100 && L.siblings[2].y == DH - 40 && L.siblings[2].width == DW - 160 && L.siblings[2].height == 40);
    assert(L.viewport.x == 100 && L.viewport.y == 32 && L.viewport.width == DW - 160 && L.viewport.height == DH - 32 - 40);
    assert(ballum_host_add_sibling(g_host, BALLUM_EDGE_LEFT, DW, &panel) == BALLUM_STATUS_INVALID_ARGUMENT);     /* eats the viewport */
    assert(ballum_host_add_sibling(g_host, BALLUM_EDGE_BOTTOM, 0, &panel) == BALLUM_STATUS_INVALID_ARGUMENT);
    assert(ballum_host_add_sibling(g_host, BALLUM_EDGE_BOTTOM, 10, NULL) == BALLUM_STATUS_INVALID_ARGUMENT);
    /* no overlaps, everything inside the display */
    ballum_rect_t all[6] = { L.header, L.siblings[0], L.siblings[1], L.siblings[2], L.viewport, L.app_name };
    for (int i = 0; i < 5; ++i) {
        assert(all[i].x + all[i].width <= DW && all[i].y + all[i].height <= DH);
        for (int j = i + 1; j < 5; ++j) {
            int sep = all[i].x + all[i].width <= all[j].x || all[j].x + all[j].width <= all[i].x ||
                      all[i].y + all[i].height <= all[j].y || all[j].y + all[j].height <= all[i].y;
            assert(sep);
        }
    }

    /* ---- start paints the white environment, then waits for the program -- */
    cfg = quick_cfg("slide-left");
    reset(&cfg);
    ballum_component_t name = { draw_name, comp_input, NULL, 0 };
    ballum_component_t clock = { draw_clock, comp_input, NULL, 100 };
    assert(!ballum_host_slot_registered(g_host, BALLUM_SLOT_APP_NAME));
    assert(ballum_host_set_slot(g_host, BALLUM_SLOT_APP_NAME, &name) == BALLUM_STATUS_OK);
    assert(ballum_host_set_slot(g_host, BALLUM_SLOT_DATE_TIME, &clock) == BALLUM_STATUS_OK);
    assert(ballum_host_slot_registered(g_host, BALLUM_SLOT_APP_NAME) == 1);
    assert(ballum_host_set_slot(g_host, (ballum_slot_t)9, &name) == BALLUM_STATUS_INVALID_ARGUMENT);
    ballum_component_t nodraw = { NULL, NULL, NULL, 0 };
    assert(ballum_host_set_slot(g_host, BALLUM_SLOT_APP_NAME, &nodraw) == BALLUM_STATUS_INVALID_ARGUMENT);
    assert(ballum_host_step(g_host, 0) == BALLUM_STATUS_NOT_INITIALIZED);        /* step before start */
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_OK);
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_INVALID_ARGUMENT);      /* once only */
    assert(g_claims == 1 && g_grab_on == 1 && g_src.started);
    assert(g_src.w == DW && g_src.h == DH - 32);
    assert(ballum_host_state(g_host) == BALLUM_HOST_STARTING);
    assert(same(px(300, 300), WHITE));                       /* the white Ballum background */
    assert(same(px(500, 20), HDR));                          /* header band outside the components */
    assert(same(px(1, 1), ballum_color_rgb(10, 200, 30)));   /* APP NAME component drawn    */
    assert(same(px(DW - 280, 0), ballum_color_rgb(200, 10, 10)));      /* DATE TIME component */
    assert(g_name_draws == 1 && g_clock_draws == 1);
    assert(same(px(0, 0), HDR));                                        /* name fill is clipped to its view...  */
    assert(same(px(DW - 281, 31), ballum_color_rgb(10, 200, 30)));      /* ...and fills right up to the slot edge */
    assert(same(px(DW - 279, 1), HDR));                                 /* but never spills into the next slot */

    /* program not ready: still starting, nothing but the clock refresh happens */
    assert(ballum_host_step(g_host, 50) == BALLUM_STATUS_OK && ballum_host_state(g_host) == BALLUM_HOST_STARTING);
    assert(g_clock_draws == 1);                                            /* 50 ms < refresh_ms 100 */
    assert(ballum_host_step(g_host, 100) == BALLUM_STATUS_OK);
    assert(g_clock_draws == 2 && g_name_draws == 1);                       /* only the clock refreshes */

    /* ---- entry transition then live program -------------------------------- */
    set_frame(RED);
    assert(ballum_host_step(g_host, 200) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_ENTERING);
    assert(same(px(5, 40), WHITE));                         /* t=0 slide: nothing of the program yet */
    assert(ballum_host_step(g_host, 250) == BALLUM_STATUS_OK);              /* t=50: shifted right by half */
    assert(same(px(100, 100), WHITE) && same(px(DW - 10, 100), RED));
    assert(ballum_host_step(g_host, 300) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_RUNNING);
    assert(same(px(0, 32), RED) && same(px(DW - 1, DH - 1), RED) && same(px(300, 300), RED));
    assert(same(px(0, 31), HDR));                                            /* chrome untouched */
    uint64_t shown = ballum_host_presented_frames(g_host);
    assert(ballum_host_step(g_host, 316) == BALLUM_STATUS_OK);
    assert(ballum_host_presented_frames(g_host) == shown);                  /* idle: nothing re-presented */
    set_frame(ballum_color_rgb(1, 2, 3));
    assert(ballum_host_step(g_host, 333) == BALLUM_STATUS_OK);
    assert(ballum_host_presented_frames(g_host) == shown + 1);
    assert(g_last_blit.x == 0 && g_last_blit.y == 32 && g_last_blit.width == DW && g_last_blit.height == DH - 32);
    assert(same(px(10, 100), ballum_color_rgb(1, 2, 3)));

    /* ---- 1:1 pointer routing ------------------------------------------------- */
    g_pq_n = g_pq_r = 0;
    push_ptr(BALLUM_EVENT_POINTER_MOVE, 100, 232, 0, 0);
    push_ptr(BALLUM_EVENT_POINTER_DOWN, 100, 232, BALLUM_BUTTON_LEFT, 0);
    push_ptr(BALLUM_EVENT_POINTER_MOVE, 700, 600, 0, 0);                     /* drag outside: captured, clamped */
    push_ptr(BALLUM_EVENT_POINTER_UP, 700, 600, BALLUM_BUTTON_LEFT, 0);
    push_ptr(BALLUM_EVENT_POINTER_DOWN, 50, 200, BALLUM_BUTTON_WHEEL_DOWN, 0);
    push_ptr(BALLUM_EVENT_POINTER_UP, 50, 200, BALLUM_BUTTON_WHEEL_DOWN, 0);
    assert(ballum_host_step(g_host, 350) == BALLUM_STATUS_OK);
    assert(g_src.n_ptr == 6 && g_src.n_btn == 4);   /* every button edge is preceded by a position */
    assert(g_src.btn[0] == 1 && g_src.btn_down[0] == 1 && g_src.btn[1] == 1 && g_src.btn_down[1] == 0);
    assert(g_src.btn[2] == 5 && g_src.btn_down[2] == 1 && g_src.btn[3] == 5 && g_src.btn_down[3] == 0);
    assert(g_src.px == 50 && g_src.py == 200 - 32);
    g_src.n_ptr = 0;
    g_pq_n = g_pq_r = 0;
    push_ptr(BALLUM_EVENT_POINTER_MOVE, 640 - 1, 480 - 1, 0, 0);
    assert(ballum_host_step(g_host, 351) == BALLUM_STATUS_OK);
    assert(g_src.n_ptr == 1 && g_src.px == (int)DW - 1 && g_src.py == (int)DH - 32 - 1);

    /* header events go to the component under the pointer, in local coordinates */
    g_src.n_ptr = g_src.n_btn = 0; g_pq_n = g_pq_r = 0;
    push_ptr(BALLUM_EVENT_POINTER_DOWN, DW - 270, 10, BALLUM_BUTTON_LEFT, 0);
    push_ptr(BALLUM_EVENT_POINTER_UP, DW - 270, 10, BALLUM_BUTTON_LEFT, 0);
    push_ptr(BALLUM_EVENT_POINTER_MOVE, 30, 5, 0, 0);
    assert(ballum_host_step(g_host, 352) == BALLUM_STATUS_OK);
    assert(g_src.n_ptr == 0 && g_src.n_btn == 0);                              /* not leaked to the program */
    assert(g_comp_in_n == 3 && g_comp_in.x == 30 && g_comp_in.y == 5);
    assert(ballum_host_state(g_host) == BALLUM_HOST_RUNNING);

    /* ---- 1:1 key routing ----------------------------------------------------- */
    g_kq_n = g_kq_r = 0;
    push_key(0xFFE3, 37, 1, 0);                                  /* Control_L down */
    push_key('c', 54, 1, BALLUM_MOD_CTRL);
    push_key('c', 54, 0, BALLUM_MOD_CTRL);
    push_key(0xFFE3, 37, 0, BALLUM_MOD_CTRL);
    assert(ballum_host_step(g_host, 353) == BALLUM_STATUS_OK);
    assert(g_src.n_key == 4);
    assert(g_src.keys[1].sym == 'c' && g_src.keys[1].code == 54 && g_src.keys[1].down == 1);
    assert(g_src.keys[3].sym == 0xFFE3 && g_src.keys[3].down == 0);
    /* a bare Escape (vim, shells) is the program's, not ours */
    g_src.n_key = 0; g_src.ki = 0; g_kq_n = g_kq_r = 0;
    push_key(BALLUM_KEYSYM_ESCAPE, 9, 1, 0);
    push_key(BALLUM_KEYSYM_ESCAPE, 9, 0, 0);
    push_key(BALLUM_KEYSYM_ESCAPE, 9, 1, BALLUM_MOD_CTRL);               /* Ctrl+Esc, not Ctrl+Alt+Esc */
    assert(ballum_host_step(g_host, 354) == BALLUM_STATUS_OK);
    assert(g_src.n_key == 3 && ballum_host_state(g_host) == BALLUM_HOST_RUNNING);

    /* ---- remapping ------------------------------------------------------------ */
    assert(ballum_host_remap_key(g_host, 'a', 0, 'b') == BALLUM_STATUS_OK);
    assert(ballum_host_remap_key(g_host, 'x', BALLUM_MOD_ALT, 0) == BALLUM_STATUS_OK);
    assert(ballum_host_remap_key(g_host, BALLUM_KEYSYM_ESCAPE, BALLUM_MOD_CTRL | BALLUM_MOD_ALT, 'q') == BALLUM_STATUS_INVALID_ARGUMENT); /* chord can't be remapped away */
    assert(ballum_host_remap_key(g_host, 0, 0, 'z') == BALLUM_STATUS_INVALID_ARGUMENT);
    assert(ballum_host_remap_button(g_host, 3, 0) == BALLUM_STATUS_OK);        /* drop right click   */
    assert(ballum_host_remap_button(g_host, 1, 2) == BALLUM_STATUS_OK);        /* left becomes middle */
    assert(ballum_host_remap_button(g_host, 9, 1) == BALLUM_STATUS_INVALID_ARGUMENT);
    g_src.n_key = g_src.ki = g_src.n_btn = g_src.bi = 0; g_kq_n = g_kq_r = g_pq_n = g_pq_r = 0;
    push_key('a', 38, 1, 0);          /* -> b, physical code unknown */
    push_key('x', 53, 1, BALLUM_MOD_ALT);   /* dropped */
    push_key('x', 53, 1, 0);          /* no Alt: passes */
    push_ptr(BALLUM_EVENT_POINTER_DOWN, 10, 100, BALLUM_BUTTON_RIGHT, 0);
    push_ptr(BALLUM_EVENT_POINTER_UP, 10, 100, BALLUM_BUTTON_RIGHT, 0);
    push_ptr(BALLUM_EVENT_POINTER_DOWN, 10, 100, BALLUM_BUTTON_LEFT, 0);
    push_ptr(BALLUM_EVENT_POINTER_UP, 10, 100, BALLUM_BUTTON_LEFT, 0);
    assert(ballum_host_step(g_host, 355) == BALLUM_STATUS_OK);
    assert(g_src.n_key == 2 && g_src.keys[0].sym == 'b' && g_src.keys[0].code == 0 && g_src.keys[1].sym == 'x');
    assert(g_src.n_btn == 2 && g_src.btn[0] == 2 && g_src.btn_down[0] == 1 && g_src.btn[1] == 2 && g_src.btn_down[1] == 0);

    /* ---- escape chord: swallowed, plays the exit, releases everything -------- */
    g_src.n_key = g_src.ki = 0; g_kq_n = g_kq_r = 0;
    push_key(0xFFE9, 64, 1, BALLUM_MOD_CTRL);                    /* Alt_L */
    push_key(BALLUM_KEYSYM_ESCAPE, 9, 1, BALLUM_MOD_CTRL | BALLUM_MOD_ALT);
    push_key(BALLUM_KEYSYM_ESCAPE, 9, 0, BALLUM_MOD_CTRL | BALLUM_MOD_ALT);
    push_key('z', 52, 1, 0);                                      /* arrives while leaving: dropped */
    assert(ballum_host_step(g_host, 400) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_LEAVING);
    assert(ballum_host_exit_reason(g_host) == BALLUM_EXIT_USER_CHORD);
    assert(g_src.n_key == 1);                                      /* only Alt_L went to the program */
    assert(g_releases == 0 && g_src.stopped == 0);                  /* still animating */
    assert(ballum_host_step(g_host, 450) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_LEAVING);
    assert(!same(px(300, 300), RED) && !same(px(300, 300), WHITE));  /* mid-fade */
    assert(ballum_host_step(g_host, 500) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE);
    assert(g_releases == 1 && g_src.stopped == 1 && g_grab_on == 0 && !g_surface_up);
    assert(ballum_host_step(g_host, 600) == BALLUM_STATUS_OK && g_releases == 1);   /* stays done */
    ballum_host_stop(g_host); ballum_host_stop(g_host);
    assert(g_releases == 1 && g_src.stopped == 1);                                    /* idempotent */

    /* ---- chord before the program shows anything leaves at once -------------- */
    cfg = quick_cfg("fade");
    reset(&cfg);
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_OK);
    push_key(BALLUM_KEYSYM_ESCAPE, 9, 1, BALLUM_MOD_CTRL | BALLUM_MOD_ALT);
    assert(ballum_host_step(g_host, 10) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE && ballum_host_exit_reason(g_host) == BALLUM_EXIT_USER_CHORD);
    assert(g_releases == 1 && g_src.stopped == 1);

    /* ---- startup timeout: a program that never draws cannot trap the screen -- */
    reset(&cfg);
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_OK);
    assert(ballum_host_step(g_host, 999) == BALLUM_STATUS_OK && ballum_host_state(g_host) == BALLUM_HOST_STARTING);
    assert(ballum_host_step(g_host, 1000) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE && ballum_host_exit_reason(g_host) == BALLUM_EXIT_STARTUP_FAILED);
    assert(g_releases == 1 && g_src.stopped == 1 && g_grab_on == 0);

    /* ---- program that fails to start / dies on its own ------------------------ */
    reset(&cfg);
    g_src.start_fail = 1;
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_BACKEND_UNAVAILABLE);
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE && g_releases == 1 && g_grab_on == 0);
    assert(ballum_host_exit_reason(g_host) == BALLUM_EXIT_STARTUP_FAILED);

    reset(&cfg);
    to_running(RED);
    g_src.alive = 0;                                              /* the program exited */
    assert(ballum_host_step(g_host, 300) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_LEAVING && ballum_host_exit_reason(g_host) == BALLUM_EXIT_PROGRAM_ENDED);
    assert(ballum_host_step(g_host, 400) == BALLUM_STATUS_OK && ballum_host_state(g_host) == BALLUM_HOST_DONE);
    assert(g_releases == 1);

    reset(&cfg);
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_OK);
    g_src.alive = 0;                                              /* died before ever drawing */
    assert(ballum_host_step(g_host, 5) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE && ballum_host_exit_reason(g_host) == BALLUM_EXIT_STARTUP_FAILED);

    /* ---- runtime limit, explicit request, source/device failures ------------- */
    cfg = quick_cfg("none"); cfg.max_runtime_ms = 5000;
    reset(&cfg);
    assert(ballum_host_start(g_host, 0) == BALLUM_STATUS_OK);
    set_frame(RED);
    assert(ballum_host_step(g_host, 10) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_RUNNING);                  /* "none" skips the entry */
    assert(same(px(10, 100), RED));
    assert(ballum_host_step(g_host, 5000) == BALLUM_STATUS_OK);
    assert(ballum_host_exit_reason(g_host) == BALLUM_EXIT_TIMEOUT && ballum_host_state(g_host) == BALLUM_HOST_LEAVING);
    assert(ballum_host_step(g_host, 5200) == BALLUM_STATUS_OK && ballum_host_state(g_host) == BALLUM_HOST_DONE);

    cfg = quick_cfg("fade");
    reset(&cfg);
    to_running(RED);
    ballum_host_request_exit(g_host, BALLUM_EXIT_REQUESTED, 300);
    assert(ballum_host_state(g_host) == BALLUM_HOST_LEAVING && ballum_host_exit_reason(g_host) == BALLUM_EXIT_REQUESTED);
    ballum_host_request_exit(g_host, BALLUM_EXIT_ERROR, 310);                  /* first reason wins */
    assert(ballum_host_exit_reason(g_host) == BALLUM_EXIT_REQUESTED);

    reset(&cfg);
    to_running(RED);
    g_src.poll_fail = 1;
    assert(ballum_host_step(g_host, 300) == BALLUM_STATUS_OK);
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE && ballum_host_exit_reason(g_host) == BALLUM_EXIT_ERROR);
    assert(g_releases == 1 && g_src.stopped == 1);

    reset(&cfg);
    to_running(RED);
    g_blit_fail = 1;
    set_frame(WHITE);
    assert(ballum_host_step(g_host, 300) == BALLUM_STATUS_BACKEND_ERROR);      /* device failure surfaces */
    assert(ballum_host_state(g_host) == BALLUM_HOST_DONE && g_releases == 1 && g_src.stopped == 1 && g_grab_on == 0);
    assert(ballum_host_exit_reason(g_host) == BALLUM_EXIT_ERROR);

    /* ---- no header: the program owns the top of the screen ------------------- */
    cfg = quick_cfg("none"); cfg.header_height = 0;
    reset(&cfg);
    assert(ballum_host_layout(g_host, &L) == BALLUM_STATUS_OK);
    assert(L.viewport.y == 0 && L.viewport.height == DH && L.header.height == 0);

    /* ---- destroy while running releases everything ---------------------------- */
    cfg = quick_cfg("fade");
    reset(&cfg);
    to_running(RED);
    ballum_host_destroy(g_host); g_host = NULL;
    assert(g_releases == 1 && g_src.stopped == 1 && g_grab_on == 0);
    ballum_host_destroy(NULL);

    /* ---- view helpers are bounds-safe ------------------------------------------ */
    {
        ballum_color_t buf[5 * 5]; memset(buf, 0, sizeof(buf));
        ballum_surface_view_t v = { buf + 1 * 5 + 1, 3, 3, 5 };                 /* 3x3 window inside a 5x5 */
        ballum_rect_t r = { 1, 1, 50, 50 };
        ballum_view_fill(&v, r, ballum_color_rgb(9, 9, 9));
        int n = 0;
        for (int i = 0; i < 25; ++i) n += (buf[i].r == 9);
        assert(n == 4);                                                         /* only the 2x2 inside the view */
        ballum_view_put(&v, 3, 0, ballum_color_rgb(7, 7, 7)); ballum_view_put(&v, 0, 3, ballum_color_rgb(7, 7, 7));
        ballum_view_put(NULL, 0, 0, ballum_color_rgb(7, 7, 7));
        for (int i = 0; i < 25; ++i) assert(buf[i].r != 7);
    }

    puts("test_ballum_host: ok");
    return 0;
}
