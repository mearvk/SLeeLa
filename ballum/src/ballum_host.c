/*
 * ballum/src/ballum_host.c -- the Ballum program host (see ballum_host.h).
 *
 * Portable: it talks only to the ballum_manager (surface, blit, input) and to a
 * ballum_host_source_t. All device I/O stays in the backend and the source.
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include "sleela_ballum/ballum_host.h"

#include <stdlib.h>
#include <string.h>

#if defined(__unix__) || defined(__APPLE__)
#  include <time.h>
#  define BALLUM_HOST_HAVE_RUN 1
#endif

#define INPUT_BURST 64u     /* events drained per queue per step */

typedef struct { uint32_t from, mods, to; } key_remap_t;

typedef struct {
    ballum_component_t c;
    int                set;
    uint64_t           last_ms;
    int                drawn;
} slot_t;

typedef struct {
    slot_t        s;
    ballum_edge_t edge;
    uint32_t      size;
} sibling_t;

struct ballum_host {
    ballum_manager_t        *mgr;
    ballum_host_config_t     cfg;
    ballum_host_source_t     src;
    void                    *src_ctx;
    uint32_t                 dw, dh;

    ballum_host_state_t      state;
    ballum_exit_reason_t     reason;

    slot_t                   slot[BALLUM_SLOT__COUNT];
    sibling_t                sib[BALLUM_HOST_MAX_SIBLINGS];
    uint32_t                 nsib;

    key_remap_t              kremap[BALLUM_HOST_MAX_KEY_REMAPS];
    uint32_t                 nkremap;
    int                      bremap_from[BALLUM_HOST_MAX_BUTTON_REMAPS];
    int                      bremap_to[BALLUM_HOST_MAX_BUTTON_REMAPS];
    uint32_t                 nbremap;

    ballum_host_layout_t     layout;
    ballum_color_t          *fb;        /* composed display frame        */
    ballum_color_t          *prog;      /* latest program frame (owned)  */
    ballum_color_t          *scratch;   /* transition output             */
    ballum_color_t          *blitbuf;   /* contiguous rect for the blit  */
    size_t                   fb_n, vp_n;

    int                      have_frame;
    int                      prog_dirty;
    int                      composed_once;
    int                      last_t;    /* last transition progress drawn, -1 = none */
    uint64_t                 t_start, t_state;

    int                      surface_claimed, grabbed, source_started;
    uint32_t                 buttons_down;     /* bit n set: button n is held in the program */
    uint32_t                 chord_code;
    int                      chord_down;

    int                      dirty_any;
    uint32_t                 dx0, dy0, dx1, dy1;   /* dirty rect, [x0,x1) [y0,y1) */
    uint64_t                 presented;
};

/* ---- small helpers -------------------------------------------------------- */

void ballum_view_put(ballum_surface_view_t *v, int32_t x, int32_t y, ballum_color_t c) {
    if (!v || !v->px || x < 0 || y < 0 || (uint32_t)x >= v->w || (uint32_t)y >= v->h) return;
    v->px[(size_t)y * v->stride + (size_t)x] = c;
}

void ballum_view_fill(ballum_surface_view_t *v, ballum_rect_t r, ballum_color_t c) {
    if (!v || !v->px || r.x >= v->w || r.y >= v->h) return;
    uint32_t w = r.width > v->w - r.x ? v->w - r.x : r.width;
    uint32_t h = r.height > v->h - r.y ? v->h - r.y : r.height;
    for (uint32_t y = 0; y < h; ++y) {
        ballum_color_t *row = v->px + (size_t)(r.y + y) * v->stride + r.x;
        for (uint32_t x = 0; x < w; ++x) row[x] = c;
    }
}

static ballum_color_t rgb(uint8_t r, uint8_t g, uint8_t b) { return ballum_color_rgb(r, g, b); }

const char *ballum_host_state_name(ballum_host_state_t s) {
    switch (s) {
    case BALLUM_HOST_IDLE:     return "idle";
    case BALLUM_HOST_STARTING: return "starting";
    case BALLUM_HOST_ENTERING: return "entering";
    case BALLUM_HOST_RUNNING:  return "running";
    case BALLUM_HOST_LEAVING:  return "leaving";
    case BALLUM_HOST_DONE:     return "done";
    }
    return "unknown";
}

const char *ballum_exit_reason_name(ballum_exit_reason_t r) {
    switch (r) {
    case BALLUM_EXIT_NONE:           return "none";
    case BALLUM_EXIT_USER_CHORD:     return "user-chord";
    case BALLUM_EXIT_PROGRAM_ENDED:  return "program-ended";
    case BALLUM_EXIT_REQUESTED:      return "requested";
    case BALLUM_EXIT_TIMEOUT:        return "timeout";
    case BALLUM_EXIT_STARTUP_FAILED: return "startup-failed";
    case BALLUM_EXIT_ERROR:          return "error";
    }
    return "unknown";
}

void ballum_host_config_default(ballum_host_config_t *cfg, uint32_t display_id) {
    if (!cfg) return;
    memset(cfg, 0, sizeof(*cfg));
    cfg->display_id = display_id;
    cfg->background = rgb(255, 255, 255);
    cfg->header_color = rgb(0xF2, 0xF2, 0xF2);
    cfg->panel_color = rgb(0xE6, 0xE6, 0xE6);
    cfg->header_height = 32;
    cfg->datetime_width = 280;
    ballum_transition_by_name("disintegrate", &cfg->enter);
    ballum_transition_by_name("fade", &cfg->leave);
    cfg->enter_ms = 700;
    cfg->leave_ms = 450;
    cfg->startup_timeout_ms = 15000;
    cfg->escape_chord.modifiers = BALLUM_MOD_CTRL | BALLUM_MOD_ALT;
    cfg->escape_chord.symbol = BALLUM_KEYSYM_ESCAPE;
    cfg->grab_input = 1;
}

/* ---- layout ---------------------------------------------------------------- */

static ballum_rect_t mkrect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    ballum_rect_t r; r.x = x; r.y = y; r.width = w; r.height = h; return r;
}

static ballum_status_t compute_layout(const ballum_host_config_t *cfg, uint32_t dw, uint32_t dh,
                                      const sibling_t *sib, uint32_t nsib,
                                      ballum_host_layout_t *out) {
    memset(out, 0, sizeof(*out));
    uint32_t hh = cfg->header_height;
    if (hh > dh / 2u) return BALLUM_STATUS_INVALID_ARGUMENT;
    out->header = mkrect(0, 0, dw, hh);
    if (hh > 0) {
        uint32_t dtw = cfg->datetime_width;
        if (dtw > dw / 2u) dtw = dw / 2u;
        out->app_name = mkrect(0, 0, dw - dtw, hh);
        out->date_time = mkrect(dw - dtw, 0, dtw, hh);
    }
    ballum_rect_t rem = mkrect(0, hh, dw, dh - hh);
    for (uint32_t i = 0; i < nsib; ++i) {
        uint32_t sz = sib[i].size;
        ballum_rect_t r;
        switch (sib[i].edge) {
        case BALLUM_EDGE_LEFT:
            if (sz == 0 || sz >= rem.width) return BALLUM_STATUS_INVALID_ARGUMENT;
            r = mkrect(rem.x, rem.y, sz, rem.height);
            rem.x += sz; rem.width -= sz;
            break;
        case BALLUM_EDGE_RIGHT:
            if (sz == 0 || sz >= rem.width) return BALLUM_STATUS_INVALID_ARGUMENT;
            r = mkrect(rem.x + rem.width - sz, rem.y, sz, rem.height);
            rem.width -= sz;
            break;
        case BALLUM_EDGE_BOTTOM:
            if (sz == 0 || sz >= rem.height) return BALLUM_STATUS_INVALID_ARGUMENT;
            r = mkrect(rem.x, rem.y + rem.height - sz, rem.width, sz);
            rem.height -= sz;
            break;
        default:
            return BALLUM_STATUS_INVALID_ARGUMENT;
        }
        out->siblings[out->sibling_count++] = r;
    }
    if (rem.width < BALLUM_HOST_MIN_VIEWPORT || rem.height < BALLUM_HOST_MIN_VIEWPORT)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    out->viewport = rem;
    return BALLUM_STATUS_OK;
}

ballum_status_t ballum_host_layout(const ballum_host_t *h, ballum_host_layout_t *out) {
    if (!h || !out) return BALLUM_STATUS_INVALID_ARGUMENT;
    return compute_layout(&h->cfg, h->dw, h->dh, h->sib, h->nsib, out);
}

/* ---- lifecycle ------------------------------------------------------------- */

ballum_status_t ballum_host_create(ballum_manager_t *mgr, const ballum_host_config_t *cfg_in,
                                   const ballum_host_source_t *source, void *source_ctx,
                                   ballum_host_t **out) {
    if (!mgr || !source || !out) return BALLUM_STATUS_INVALID_ARGUMENT;
    *out = NULL;
    if (!source->start || !source->poll_frame || !source->pointer || !source->button ||
        !source->key || !source->alive || !source->stop)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!mgr->initialized) return BALLUM_STATUS_NOT_INITIALIZED;

    ballum_host_config_t cfg;
    if (cfg_in) cfg = *cfg_in; else ballum_host_config_default(&cfg, 0);

    ballum_display_t d;
    ballum_status_t st = ballum_manager_describe(mgr, cfg.display_id, &d);
    if (st != BALLUM_STATUS_OK) return st;
    if (!(d.capabilities & BALLUM_CAP_FULLSCREEN_SURFACE) ||
        !(d.capabilities & BALLUM_CAP_REGION_BLIT) ||
        ballum_manager_has_surface(mgr, cfg.display_id) != 1)
        return BALLUM_STATUS_UNSUPPORTED;
    /* Without pointer and full key input the user could not leave: refuse. */
    if (ballum_manager_has_input(mgr) != 1 || ballum_manager_has_key_events(mgr) != 1)
        return BALLUM_STATUS_UNSUPPORTED;
    if (d.width == 0 || d.height == 0 || (uint64_t)d.width * d.height > BALLUM_HOST_MAX_PIXELS)
        return BALLUM_STATUS_OUT_OF_BOUNDS;
    if (cfg.escape_chord.symbol == 0) return BALLUM_STATUS_INVALID_ARGUMENT;   /* never allow no way out */

    ballum_host_t *h = (ballum_host_t *)calloc(1, sizeof(*h));
    if (!h) return BALLUM_STATUS_BACKEND_ERROR;
    h->mgr = mgr; h->cfg = cfg; h->src = *source; h->src_ctx = source_ctx;
    h->dw = d.width; h->dh = d.height;
    h->state = BALLUM_HOST_IDLE; h->last_t = -1;
    st = compute_layout(&h->cfg, h->dw, h->dh, h->sib, 0, &h->layout);
    if (st != BALLUM_STATUS_OK) { free(h); return st; }
    *out = h;
    return BALLUM_STATUS_OK;
}

void ballum_host_stop(ballum_host_t *h) {
    if (!h) return;
    if (h->source_started) { h->src.stop(h->src_ctx); h->source_started = 0; }
    if (h->grabbed) { ballum_manager_set_input_grab(h->mgr, 0); h->grabbed = 0; }
    if (h->surface_claimed) {
        ballum_manager_release_surface(h->mgr, h->cfg.display_id);
        h->surface_claimed = 0;
    }
    if (h->state != BALLUM_HOST_IDLE) h->state = BALLUM_HOST_DONE;
}

void ballum_host_destroy(ballum_host_t *h) {
    if (!h) return;
    ballum_host_stop(h);
    free(h->fb); free(h->prog); free(h->scratch); free(h->blitbuf);
    free(h);
}

ballum_status_t ballum_host_set_slot(ballum_host_t *h, ballum_slot_t slot,
                                     const ballum_component_t *c) {
    if (!h || (unsigned)slot >= (unsigned)BALLUM_SLOT__COUNT) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (!c) { memset(&h->slot[slot], 0, sizeof(h->slot[slot])); return BALLUM_STATUS_OK; }
    if (!c->draw) return BALLUM_STATUS_INVALID_ARGUMENT;
    h->slot[slot].c = *c; h->slot[slot].set = 1; h->slot[slot].drawn = 0; h->slot[slot].last_ms = 0;
    return BALLUM_STATUS_OK;
}

int ballum_host_slot_registered(const ballum_host_t *h, ballum_slot_t slot) {
    if (!h || (unsigned)slot >= (unsigned)BALLUM_SLOT__COUNT) return -1;
    return h->slot[slot].set ? 1 : 0;
}

ballum_status_t ballum_host_add_sibling(ballum_host_t *h, ballum_edge_t edge, uint32_t size,
                                        const ballum_component_t *c) {
    if (!h || !c || !c->draw) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (h->state != BALLUM_HOST_IDLE || h->nsib >= BALLUM_HOST_MAX_SIBLINGS)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    sibling_t trial[BALLUM_HOST_MAX_SIBLINGS];
    memcpy(trial, h->sib, sizeof(h->sib));
    memset(&trial[h->nsib], 0, sizeof(trial[0]));
    trial[h->nsib].edge = edge; trial[h->nsib].size = size;
    ballum_host_layout_t lay;
    ballum_status_t st = compute_layout(&h->cfg, h->dw, h->dh, trial, h->nsib + 1u, &lay);
    if (st != BALLUM_STATUS_OK) return st;
    h->sib[h->nsib].s.c = *c; h->sib[h->nsib].s.set = 1;
    h->sib[h->nsib].edge = edge; h->sib[h->nsib].size = size;
    h->nsib++;
    h->layout = lay;
    return BALLUM_STATUS_OK;
}

ballum_status_t ballum_host_remap_key(ballum_host_t *h, uint32_t from, uint32_t mods, uint32_t to) {
    if (!h || from == 0 || h->nkremap >= BALLUM_HOST_MAX_KEY_REMAPS) return BALLUM_STATUS_INVALID_ARGUMENT;
    /* The escape chord must stay reachable: never allow it to be remapped away. */
    if (from == h->cfg.escape_chord.symbol && (mods & h->cfg.escape_chord.modifiers) == h->cfg.escape_chord.modifiers)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    h->kremap[h->nkremap].from = from; h->kremap[h->nkremap].mods = mods; h->kremap[h->nkremap].to = to;
    h->nkremap++;
    return BALLUM_STATUS_OK;
}

ballum_status_t ballum_host_remap_button(ballum_host_t *h, int from, int to) {
    if (!h || from < 1 || from > 7 || to < 0 || to > 7 || h->nbremap >= BALLUM_HOST_MAX_BUTTON_REMAPS)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    h->bremap_from[h->nbremap] = from; h->bremap_to[h->nbremap] = to;
    h->nbremap++;
    return BALLUM_STATUS_OK;
}

/* ---- dirty tracking & drawing -------------------------------------------- */

static void mark_dirty(ballum_host_t *h, ballum_rect_t r) {
    if (r.width == 0 || r.height == 0) return;
    uint32_t x1 = r.x + r.width, y1 = r.y + r.height;
    if (!h->dirty_any) { h->dx0 = r.x; h->dy0 = r.y; h->dx1 = x1; h->dy1 = y1; h->dirty_any = 1; return; }
    if (r.x < h->dx0) h->dx0 = r.x;
    if (r.y < h->dy0) h->dy0 = r.y;
    if (x1 > h->dx1) h->dx1 = x1;
    if (y1 > h->dy1) h->dy1 = y1;
}

static void fill_rect(ballum_host_t *h, ballum_rect_t r, ballum_color_t c) {
    for (uint32_t y = 0; y < r.height; ++y) {
        ballum_color_t *row = h->fb + (size_t)(r.y + y) * h->dw + r.x;
        for (uint32_t x = 0; x < r.width; ++x) row[x] = c;
    }
}

static void draw_slot(ballum_host_t *h, slot_t *s, ballum_rect_t r, ballum_color_t base, uint64_t now) {
    if (r.width == 0 || r.height == 0) return;
    fill_rect(h, r, base);
    if (s && s->set) {
        ballum_surface_view_t v;
        v.px = h->fb + (size_t)r.y * h->dw + r.x; v.w = r.width; v.h = r.height; v.stride = h->dw;
        s->c.draw(s->c.user, &v);
        s->last_ms = now; s->drawn = 1;
    }
    mark_dirty(h, r);
}

static void draw_chrome(ballum_host_t *h, uint64_t now) {
    fill_rect(h, mkrect(0, 0, h->dw, h->dh), h->cfg.background);
    if (h->cfg.header_height > 0) {
        draw_slot(h, NULL, h->layout.header, h->cfg.header_color, now);
        draw_slot(h, &h->slot[BALLUM_SLOT_APP_NAME], h->layout.app_name, h->cfg.header_color, now);
        draw_slot(h, &h->slot[BALLUM_SLOT_DATE_TIME], h->layout.date_time, h->cfg.header_color, now);
    }
    for (uint32_t i = 0; i < h->nsib; ++i)
        draw_slot(h, &h->sib[i].s, h->layout.siblings[i], h->cfg.panel_color, now);
    mark_dirty(h, mkrect(0, 0, h->dw, h->dh));
}

static void refresh_components(ballum_host_t *h, uint64_t now) {
    if (h->cfg.header_height > 0) {
        for (int i = 0; i < (int)BALLUM_SLOT__COUNT; ++i) {
            slot_t *s = &h->slot[i];
            if (!s->set) continue;
            int due = !s->drawn || (s->c.refresh_ms && now - s->last_ms >= s->c.refresh_ms);
            if (due) draw_slot(h, s, i == (int)BALLUM_SLOT_APP_NAME ? h->layout.app_name : h->layout.date_time,
                               h->cfg.header_color, now);
        }
    }
    for (uint32_t i = 0; i < h->nsib; ++i) {
        slot_t *s = &h->sib[i].s;
        if (s->c.refresh_ms && now - s->last_ms >= s->c.refresh_ms)
            draw_slot(h, s, h->layout.siblings[i], h->cfg.panel_color, now);
    }
}

static void put_program(ballum_host_t *h, const ballum_color_t *src) {
    ballum_rect_t vp = h->layout.viewport;
    for (uint32_t y = 0; y < vp.height; ++y)
        memcpy(h->fb + (size_t)(vp.y + y) * h->dw + vp.x, src + (size_t)y * vp.width,
               (size_t)vp.width * sizeof(ballum_color_t));
    mark_dirty(h, vp);
}

static void draw_program(ballum_host_t *h, uint64_t now) {
    ballum_rect_t vp = h->layout.viewport;
    int t;
    switch (h->state) {
    case BALLUM_HOST_STARTING:
        return;                                       /* background only */
    case BALLUM_HOST_ENTERING: {
        uint64_t el = now - h->t_state;
        t = h->cfg.enter_ms ? (int)(el >= h->cfg.enter_ms ? 100 : el * 100u / h->cfg.enter_ms) : 100;
        break;
    }
    case BALLUM_HOST_LEAVING: {
        uint64_t el = now - h->t_state;
        int p = h->cfg.leave_ms ? (int)(el >= h->cfg.leave_ms ? 100 : el * 100u / h->cfg.leave_ms) : 100;
        t = 100 - p;
        break;
    }
    case BALLUM_HOST_RUNNING:
        if (!h->prog_dirty) return;
        put_program(h, h->prog);
        h->prog_dirty = 0;
        return;
    default:
        return;
    }
    if (!h->have_frame) return;
    if (t == h->last_t && !h->prog_dirty) return;     /* nothing new to show */
    const ballum_transition_t *tr = (h->state == BALLUM_HOST_ENTERING) ? &h->cfg.enter : &h->cfg.leave;
    if (!ballum_transition_frame(tr, t, vp.width, vp.height, h->prog, h->cfg.background, h->scratch)) return;
    put_program(h, h->scratch);
    h->last_t = t;
    h->prog_dirty = 0;
}

static ballum_status_t present_dirty(ballum_host_t *h) {
    if (!h->dirty_any) return BALLUM_STATUS_OK;
    uint32_t x0 = h->dx0, y0 = h->dy0, w = h->dx1 - h->dx0, hh = h->dy1 - h->dy0;
    h->dirty_any = 0;
    const ballum_color_t *px;
    if (x0 == 0 && w == h->dw) {
        px = h->fb + (size_t)y0 * h->dw;             /* full-width rows are already contiguous */
    } else {
        for (uint32_t y = 0; y < hh; ++y)
            memcpy(h->blitbuf + (size_t)y * w, h->fb + (size_t)(y0 + y) * h->dw + x0,
                   (size_t)w * sizeof(ballum_color_t));
        px = h->blitbuf;
    }
    ballum_status_t st = ballum_manager_blit_region(h->mgr, h->cfg.display_id,
                                                    mkrect(x0, y0, w, hh), px, (size_t)w * hh);
    if (st != BALLUM_STATUS_OK) return st;
    st = ballum_manager_present(h->mgr, h->cfg.display_id);
    if (st == BALLUM_STATUS_OK) h->presented++;
    return st;
}

/* ---- exit ------------------------------------------------------------------ */

void ballum_host_request_exit(ballum_host_t *h, ballum_exit_reason_t reason, uint64_t now) {
    if (!h) return;
    if (h->state == BALLUM_HOST_DONE || h->state == BALLUM_HOST_LEAVING) return;
    if (h->reason == BALLUM_EXIT_NONE) h->reason = reason;
    if (h->state == BALLUM_HOST_IDLE) return;
    /* No picture to animate, a failure, or no transition configured: leave now. */
    if (h->state == BALLUM_HOST_STARTING || !h->have_frame ||
        reason == BALLUM_EXIT_ERROR || reason == BALLUM_EXIT_STARTUP_FAILED ||
        h->cfg.leave.kind == BALLUM_TRANSITION_NONE || h->cfg.leave_ms == 0) {
        ballum_host_stop(h);
        return;
    }
    h->state = BALLUM_HOST_LEAVING;
    h->t_state = now;
    h->last_t = -1;
    h->prog_dirty = 1;
    h->buttons_down = 0;
}

/* ---- input routing --------------------------------------------------------- */

static int map_button(const ballum_host_t *h, int b) {
    for (uint32_t i = 0; i < h->nbremap; ++i)
        if (h->bremap_from[i] == b) return h->bremap_to[i];
    return b;
}

static int in_rect(ballum_rect_t r, int32_t x, int32_t y) {
    return x >= (int32_t)r.x && y >= (int32_t)r.y &&
           x < (int32_t)(r.x + r.width) && y < (int32_t)(r.y + r.height);
}

static void route_to_component(ballum_host_t *h, const ballum_pointer_event_t *e) {
    struct { ballum_rect_t r; slot_t *s; } hit[2 + BALLUM_HOST_MAX_SIBLINGS];
    uint32_t n = 0;
    if (h->cfg.header_height > 0) {
        hit[n].r = h->layout.app_name;  hit[n++].s = &h->slot[BALLUM_SLOT_APP_NAME];
        hit[n].r = h->layout.date_time; hit[n++].s = &h->slot[BALLUM_SLOT_DATE_TIME];
    }
    for (uint32_t i = 0; i < h->nsib; ++i) { hit[n].r = h->layout.siblings[i]; hit[n++].s = &h->sib[i].s; }
    for (uint32_t i = 0; i < n; ++i) {
        if (!hit[i].s->set || !hit[i].s->c.input || !in_rect(hit[i].r, e->x, e->y)) continue;
        ballum_pointer_event_t local = *e;
        local.x = e->x - (int32_t)hit[i].r.x;
        local.y = e->y - (int32_t)hit[i].r.y;
        hit[i].s->c.input(hit[i].s->c.user, &local);
        return;
    }
}

static void route_pointer(ballum_host_t *h, const ballum_pointer_event_t *e) {
    ballum_rect_t vp = h->layout.viewport;
    int inside = in_rect(vp, e->x, e->y);
    int live = (h->state == BALLUM_HOST_RUNNING);
    int captured = h->buttons_down != 0;
    int32_t vx = e->x - (int32_t)vp.x, vy = e->y - (int32_t)vp.y;
    if (vx < 0) vx = 0;
    if (vy < 0) vy = 0;
    if (vx >= (int32_t)vp.width) vx = (int32_t)vp.width - 1;
    if (vy >= (int32_t)vp.height) vy = (int32_t)vp.height - 1;

    switch (e->type) {
    case BALLUM_EVENT_POINTER_MOVE:
        if (live && (inside || captured)) { h->src.pointer(h->src_ctx, vx, vy); return; }
        break;
    case BALLUM_EVENT_POINTER_DOWN: {
        int b = map_button(h, (int)e->button);
        if (live && inside) {
            if (b >= 1 && b <= 7) {
                h->src.pointer(h->src_ctx, vx, vy);
                h->src.button(h->src_ctx, b, 1);
                h->buttons_down |= 1u << b;
            }
            return;                                    /* mapped to nothing: dropped */
        }
        break;
    }
    case BALLUM_EVENT_POINTER_UP: {
        int b = map_button(h, (int)e->button);
        if (b >= 1 && b <= 7 && (h->buttons_down & (1u << b))) {
            h->buttons_down &= ~(1u << b);
            if (live) { h->src.pointer(h->src_ctx, vx, vy); h->src.button(h->src_ctx, b, 0); }
            return;
        }
        break;
    }
    default:
        return;
    }
    if (!(live && inside)) route_to_component(h, e);
}

static void route_key(ballum_host_t *h, const ballum_key_event_t *k, uint64_t now) {
    const ballum_chord_t *ch = &h->cfg.escape_chord;
    if (k->pressed && k->symbol == ch->symbol && (k->modifiers & ch->modifiers) == ch->modifiers) {
        h->chord_code = k->code; h->chord_down = 1;
        ballum_host_request_exit(h, BALLUM_EXIT_USER_CHORD, now);
        return;
    }
    if (!k->pressed && h->chord_down && k->code == h->chord_code) { h->chord_down = 0; return; }
    if (h->state != BALLUM_HOST_RUNNING) return;

    uint32_t sym = k->symbol, code = k->code;
    for (uint32_t i = 0; i < h->nkremap; ++i) {
        if (h->kremap[i].from == k->symbol && (k->modifiers & h->kremap[i].mods) == h->kremap[i].mods) {
            sym = h->kremap[i].to;
            code = (sym == k->symbol) ? k->code : 0;   /* a remapped symbol has no physical code */
            break;
        }
    }
    if (sym == 0) return;                               /* dropped by a rule */
    h->src.key(h->src_ctx, sym, code, k->pressed ? 1 : 0);
}

static void drain_input(ballum_host_t *h, uint64_t now) {
    for (unsigned i = 0; i < INPUT_BURST; ++i) {
        ballum_key_event_t k;
        ballum_status_t st = ballum_manager_poll_key_event(h->mgr, &k);
        if (st != BALLUM_STATUS_OK) { ballum_host_request_exit(h, BALLUM_EXIT_ERROR, now); return; }
        if (k.code == 0) break;
        if (h->state == BALLUM_HOST_LEAVING || h->state == BALLUM_HOST_DONE) continue;
        route_key(h, &k, now);
    }
    for (unsigned i = 0; i < INPUT_BURST; ++i) {
        if (h->state == BALLUM_HOST_DONE) return;
        ballum_pointer_event_t e;
        ballum_status_t st = ballum_manager_poll_event(h->mgr, &e);
        if (st != BALLUM_STATUS_OK) { ballum_host_request_exit(h, BALLUM_EXIT_ERROR, now); return; }
        if (e.type == BALLUM_EVENT_NONE) break;
        if (h->state == BALLUM_HOST_LEAVING) continue;
        route_pointer(h, &e);
    }
}

/* ---- start / step ---------------------------------------------------------- */

static void *alloc_px(size_t n) { return n ? calloc(n, sizeof(ballum_color_t)) : NULL; }

ballum_status_t ballum_host_start(ballum_host_t *h, uint64_t now) {
    if (!h) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (h->state != BALLUM_HOST_IDLE) return BALLUM_STATUS_INVALID_ARGUMENT;
    ballum_status_t st = compute_layout(&h->cfg, h->dw, h->dh, h->sib, h->nsib, &h->layout);
    if (st != BALLUM_STATUS_OK) return st;

    h->fb_n = (size_t)h->dw * h->dh;
    h->vp_n = (size_t)h->layout.viewport.width * h->layout.viewport.height;
    free(h->fb); free(h->prog); free(h->scratch); free(h->blitbuf);
    h->fb = alloc_px(h->fb_n); h->prog = alloc_px(h->vp_n);
    h->scratch = alloc_px(h->vp_n); h->blitbuf = alloc_px(h->fb_n);
    if (!h->fb || !h->prog || !h->scratch || !h->blitbuf) return BALLUM_STATUS_BACKEND_ERROR;

    st = ballum_manager_claim_surface(h->mgr, h->cfg.display_id);
    if (st != BALLUM_STATUS_OK) return st;
    h->surface_claimed = 1;
    h->state = BALLUM_HOST_STARTING;
    h->t_start = h->t_state = now;
    h->reason = BALLUM_EXIT_NONE;

    if (h->cfg.grab_input) {
        st = ballum_manager_set_input_grab(h->mgr, 1);
        if (st == BALLUM_STATUS_OK) h->grabbed = 1;
        else if (h->cfg.require_grab) { ballum_host_stop(h); return st; }
    }

    /* Show the white environment and the empty chrome before the program starts. */
    draw_chrome(h, now);
    st = present_dirty(h);
    if (st != BALLUM_STATUS_OK) { ballum_host_stop(h); return st; }

    st = h->src.start(h->src_ctx, h->layout.viewport.width, h->layout.viewport.height);
    if (st != BALLUM_STATUS_OK) { h->reason = BALLUM_EXIT_STARTUP_FAILED; ballum_host_stop(h); return st; }
    h->source_started = 1;
    return BALLUM_STATUS_OK;
}

static void pull_frame(ballum_host_t *h, uint64_t now) {
    const ballum_color_t *px = NULL;
    int changed = 0;
    ballum_status_t st = h->src.poll_frame(h->src_ctx, &px, &changed);
    if (st != BALLUM_STATUS_OK) { ballum_host_request_exit(h, BALLUM_EXIT_ERROR, now); return; }
    if (px && (changed || !h->have_frame)) {
        memcpy(h->prog, px, h->vp_n * sizeof(ballum_color_t));
        h->have_frame = 1;
        h->prog_dirty = 1;
    }
}

ballum_status_t ballum_host_step(ballum_host_t *h, uint64_t now) {
    if (!h) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (h->state == BALLUM_HOST_IDLE) return BALLUM_STATUS_NOT_INITIALIZED;
    if (h->state == BALLUM_HOST_DONE) return BALLUM_STATUS_OK;

    drain_input(h, now);                               /* input and the chord come first */
    if (h->state == BALLUM_HOST_DONE) return BALLUM_STATUS_OK;

    if (h->cfg.max_runtime_ms && now - h->t_start >= h->cfg.max_runtime_ms)
        ballum_host_request_exit(h, BALLUM_EXIT_TIMEOUT, now);

    if (h->state == BALLUM_HOST_STARTING || h->state == BALLUM_HOST_ENTERING ||
        h->state == BALLUM_HOST_RUNNING) {
        pull_frame(h, now);
        if (h->state != BALLUM_HOST_DONE && !h->src.alive(h->src_ctx))
            ballum_host_request_exit(h, h->have_frame ? BALLUM_EXIT_PROGRAM_ENDED
                                                      : BALLUM_EXIT_STARTUP_FAILED, now);
    }
    if (h->state == BALLUM_HOST_DONE) return BALLUM_STATUS_OK;

    switch (h->state) {
    case BALLUM_HOST_STARTING:
        if (h->have_frame) {
            h->t_state = now; h->last_t = -1;
            h->state = (h->cfg.enter.kind == BALLUM_TRANSITION_NONE || h->cfg.enter_ms == 0)
                           ? BALLUM_HOST_RUNNING : BALLUM_HOST_ENTERING;
        } else if (now - h->t_start >= h->cfg.startup_timeout_ms) {
            ballum_host_request_exit(h, BALLUM_EXIT_STARTUP_FAILED, now);
        }
        break;
    case BALLUM_HOST_ENTERING:
        if (now - h->t_state >= h->cfg.enter_ms) { h->state = BALLUM_HOST_RUNNING; h->prog_dirty = 1; }
        break;
    case BALLUM_HOST_LEAVING:
        if (now - h->t_state >= h->cfg.leave_ms) { ballum_host_stop(h); return BALLUM_STATUS_OK; }
        break;
    default:
        break;
    }
    if (h->state == BALLUM_HOST_DONE) return BALLUM_STATUS_OK;

    refresh_components(h, now);
    draw_program(h, now);
    ballum_status_t st = present_dirty(h);
    if (st != BALLUM_STATUS_OK) {
        h->reason = h->reason == BALLUM_EXIT_NONE ? BALLUM_EXIT_ERROR : h->reason;
        ballum_host_stop(h);
        return st;
    }
    return BALLUM_STATUS_OK;
}

#if defined(BALLUM_HOST_HAVE_RUN)
static uint64_t mono_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
}

ballum_status_t ballum_host_run(ballum_host_t *h, unsigned fps) {
    if (!h) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (fps == 0 || fps > 240) fps = 60;
    ballum_status_t st = ballum_host_start(h, mono_ms());
    if (st != BALLUM_STATUS_OK) return st;
    const long frame_ns = 1000000000L / (long)fps;
    while (h->state != BALLUM_HOST_DONE) {
        struct timespec t0, t1;
        clock_gettime(CLOCK_MONOTONIC, &t0);
        st = ballum_host_step(h, mono_ms());
        if (st != BALLUM_STATUS_OK) return st;
        clock_gettime(CLOCK_MONOTONIC, &t1);
        long spent = (long)(t1.tv_sec - t0.tv_sec) * 1000000000L + (t1.tv_nsec - t0.tv_nsec);
        if (spent < frame_ns) {
            struct timespec nap = { 0, frame_ns - spent };
            nanosleep(&nap, NULL);
        }
    }
    return BALLUM_STATUS_OK;
}
#else
ballum_status_t ballum_host_run(ballum_host_t *h, unsigned fps) {
    (void)h; (void)fps;
    return BALLUM_STATUS_UNSUPPORTED;
}
#endif

ballum_host_state_t ballum_host_state(const ballum_host_t *h) { return h ? h->state : BALLUM_HOST_DONE; }
ballum_exit_reason_t ballum_host_exit_reason(const ballum_host_t *h) { return h ? h->reason : BALLUM_EXIT_NONE; }
uint64_t ballum_host_presented_frames(const ballum_host_t *h) { return h ? h->presented : 0u; }
