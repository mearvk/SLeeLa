/*
 * lib/web-server-technology/native/src/slweb.c
 *
 * SLeeLa Web Server Technology -- native C11 library core.
 * Dependency-free (libc only); no sockets, no HTTP wire handling.
 */
#include "slweb.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>   /* GetSystemInfo for the CPU count on Windows */
#else
#include <unistd.h>    /* sysconf(_SC_NPROCESSORS_ONLN) on POSIX/macOS */
#endif

const char *slweb_version(void) { return "1.0.0"; }

/* ================================================================== */
/*  Envelopment                                                        */
/* ================================================================== */
typedef struct {
  char name[SLWEB_MAX_NAME];
  slweb_program_kind kind;
  char mount[SLWEB_MAX_PATH];
  char resource[SLWEB_MAX_PATH];
  int writable;
} slweb_program;

typedef struct {
  char cause[SLWEB_MAX_PATH];
  char subject[SLWEB_MAX_NAME];
  char path_prefix[SLWEB_MAX_PATH];
  char program[SLWEB_MAX_NAME];
  int enabled;
} slweb_warrant;

struct slweb_envelopment {
  char domain[SLWEB_MAX_NAME];
  int enabled;
  int scientist;
  slweb_program *programs;
  size_t n_programs, cap_programs;
  slweb_warrant *warrants;
  size_t n_warrants, cap_warrants;
};

static void copy_bounded(char *dst, size_t cap, const char *src) {
  if (!src) { dst[0] = 0; return; }
  size_t n = strlen(src);
  if (n >= cap) n = cap - 1;
  memcpy(dst, src, n);
  dst[n] = 0;
}

slweb_envelopment *slweb_env_new(const char *domain) {
  slweb_envelopment *e = (slweb_envelopment *)calloc(1, sizeof *e);
  if (!e) return NULL;
  copy_bounded(e->domain, sizeof e->domain, domain ? domain : "");
  return e;
}

void slweb_env_free(slweb_envelopment *env) {
  if (!env) return;
  free(env->programs);
  free(env->warrants);
  free(env);
}

void slweb_env_set_enabled(slweb_envelopment *env, int enabled) {
  if (env) env->enabled = enabled ? 1 : 0;
}
void slweb_env_set_scientist(slweb_envelopment *env, int scientist) {
  if (env) env->scientist = scientist ? 1 : 0;
}
int slweb_env_enabled(const slweb_envelopment *env) { return env ? env->enabled : 0; }
int slweb_env_scientist(const slweb_envelopment *env) { return env ? env->scientist : 0; }

static int ensure_programs(slweb_envelopment *e) {
  if (e->n_programs < e->cap_programs) return 1;
  size_t ncap = e->cap_programs ? e->cap_programs * 2 : 4;
  slweb_program *p = (slweb_program *)realloc(e->programs, ncap * sizeof *p);
  if (!p) return 0;
  e->programs = p;
  e->cap_programs = ncap;
  return 1;
}
static int ensure_warrants(slweb_envelopment *e) {
  if (e->n_warrants < e->cap_warrants) return 1;
  size_t ncap = e->cap_warrants ? e->cap_warrants * 2 : 4;
  slweb_warrant *w = (slweb_warrant *)realloc(e->warrants, ncap * sizeof *w);
  if (!w) return 0;
  e->warrants = w;
  e->cap_warrants = ncap;
  return 1;
}

static const slweb_program *find_program(const slweb_envelopment *e,
                                         const char *name) {
  for (size_t i = 0; i < e->n_programs; ++i)
    if (strcmp(e->programs[i].name, name) == 0) return &e->programs[i];
  return NULL;
}

slweb_status slweb_env_add_program(slweb_envelopment *env, const char *name,
                                   slweb_program_kind kind, const char *mount,
                                   const char *resource, int writable) {
  if (!env || !name || !*name) return SLWEB_ERR_ARG;
  if (strlen(name) >= SLWEB_MAX_NAME) return SLWEB_ERR_RANGE;
  if (find_program(env, name)) return SLWEB_ERR_ARG; /* duplicate */
  if (!ensure_programs(env)) return SLWEB_ERR_OOM;
  slweb_program *p = &env->programs[env->n_programs++];
  memset(p, 0, sizeof *p);
  copy_bounded(p->name, sizeof p->name, name);
  p->kind = kind;
  copy_bounded(p->mount, sizeof p->mount, mount ? mount : "/");
  copy_bounded(p->resource, sizeof p->resource, resource ? resource : "");
  p->writable = writable ? 1 : 0;
  return SLWEB_OK;
}

slweb_status slweb_env_add_warrant(slweb_envelopment *env, const char *cause,
                                   const char *subject, const char *path_prefix,
                                   const char *program, int enabled) {
  if (!env || !cause || !*cause) return SLWEB_ERR_ARG;
  if (!ensure_warrants(env)) return SLWEB_ERR_OOM;
  slweb_warrant *w = &env->warrants[env->n_warrants++];
  memset(w, 0, sizeof *w);
  copy_bounded(w->cause, sizeof w->cause, cause);
  copy_bounded(w->subject, sizeof w->subject, subject ? subject : "*");
  copy_bounded(w->path_prefix, sizeof w->path_prefix, path_prefix ? path_prefix : "");
  copy_bounded(w->program, sizeof w->program, program ? program : "");
  w->enabled = enabled ? 1 : 0;
  return SLWEB_OK;
}

size_t slweb_env_program_count(const slweb_envelopment *env) {
  return env ? env->n_programs : 0;
}
size_t slweb_env_warrant_count(const slweb_envelopment *env) {
  return env ? env->n_warrants : 0;
}

int slweb_env_active(const slweb_envelopment *env) {
  if (!env) return 0;
  return env->enabled && env->scientist && env->n_warrants > 0;
}

static int starts_with(const char *s, const char *prefix) {
  size_t n = strlen(prefix);
  return strncmp(s, prefix, n) == 0;
}

slweb_status slweb_env_resolve(const slweb_envelopment *env, const char *host,
                               const char *path, char *out_program,
                               char *out_cause, slweb_program_kind *out_kind) {
  if (!env) return SLWEB_ERR_ARG;
  if (!slweb_env_active(env)) return SLWEB_ERR_DENIED;
  if (!path) path = "/";
  if (!host) host = "";
  const slweb_warrant *best = NULL;
  const slweb_program *best_prog = NULL;
  size_t best_len = 0;
  int have = 0;
  for (size_t i = 0; i < env->n_warrants; ++i) {
    const slweb_warrant *w = &env->warrants[i];
    if (!w->enabled) continue;
    if (!(strcmp(w->subject, "*") == 0 || strcmp(w->subject, host) == 0)) continue;
    if (w->path_prefix[0] && !starts_with(path, w->path_prefix)) continue;
    const slweb_program *p = find_program(env, w->program);
    if (!p) continue;
    size_t len = strlen(w->path_prefix);
    if (!have || len > best_len) {
      have = 1; best = w; best_prog = p; best_len = len;
    }
  }
  if (!have) return SLWEB_ERR_NOTFOUND;
  if (out_program) copy_bounded(out_program, SLWEB_MAX_NAME, best_prog->name);
  if (out_cause) copy_bounded(out_cause, SLWEB_MAX_PATH, best->cause);
  if (out_kind) *out_kind = best_prog->kind;
  return SLWEB_OK;
}

const char *slweb_program_kind_name(slweb_program_kind k) {
  switch (k) {
    case SLWEB_PROGRAM_STATIC: return "static";
    case SLWEB_PROGRAM_SQL: return "sql";
    case SLWEB_PROGRAM_MEME: return "meme";
  }
  return "meme";
}

/* ================================================================== */
/*  Rate limiter                                                       */
/* ================================================================== */
typedef struct {
  char peer[64];
  unsigned long long *stamps; /* ring of request times (ms)      */
  size_t n, cap, head;        /* count, capacity, ring head      */
} slweb_peer_window;

struct slweb_rate {
  unsigned per_peer_max, global_max, window_ms;
  unsigned long long *global_stamps;
  size_t global_n, global_cap, global_head;
  slweb_peer_window *peers;
  size_t n_peers, cap_peers;
};

slweb_rate *slweb_rate_new(unsigned per_peer_max, unsigned global_max,
                           unsigned window_ms) {
  slweb_rate *r = (slweb_rate *)calloc(1, sizeof *r);
  if (!r) return NULL;
  r->per_peer_max = per_peer_max;
  r->global_max = global_max;
  r->window_ms = window_ms ? window_ms : 1000;
  return r;
}

void slweb_rate_free(slweb_rate *r) {
  if (!r) return;
  free(r->global_stamps);
  for (size_t i = 0; i < r->n_peers; ++i) free(r->peers[i].stamps);
  free(r->peers);
  free(r);
}

int slweb_rate_enabled(const slweb_rate *r) {
  return r && (r->per_peer_max != 0 || r->global_max != 0);
}

/* Keep stamps still within the window: stamp is live iff stamp + window > now.
 * Written overflow-safe (no now-window subtraction that could underflow), so a
 * stamp recorded at t=0 is correctly still live while now < window. Compacts in
 * place and returns the kept count. */
static size_t window_count(unsigned long long *stamps, size_t n,
                           unsigned long long now_ms, unsigned window_ms) {
  size_t keep = 0;
  for (size_t i = 0; i < n; ++i)
    if (stamps[i] + (unsigned long long)window_ms > now_ms)
      stamps[keep++] = stamps[i];
  return keep;
}

static int push_stamp(unsigned long long **arr, size_t *n, size_t *cap,
                      unsigned long long v) {
  if (*n >= *cap) {
    size_t ncap = *cap ? *cap * 2 : 8;
    unsigned long long *p =
        (unsigned long long *)realloc(*arr, ncap * sizeof(unsigned long long));
    if (!p) return 0;
    *arr = p;
    *cap = ncap;
  }
  (*arr)[(*n)++] = v;
  return 1;
}

static slweb_peer_window *peer_of(slweb_rate *r, const char *peer) {
  for (size_t i = 0; i < r->n_peers; ++i)
    if (strcmp(r->peers[i].peer, peer) == 0) return &r->peers[i];
  if (r->n_peers >= r->cap_peers) {
    size_t ncap = r->cap_peers ? r->cap_peers * 2 : 8;
    slweb_peer_window *p =
        (slweb_peer_window *)realloc(r->peers, ncap * sizeof *p);
    if (!p) return NULL;
    r->peers = p;
    r->cap_peers = ncap;
  }
  slweb_peer_window *w = &r->peers[r->n_peers++];
  memset(w, 0, sizeof *w);
  copy_bounded(w->peer, sizeof w->peer, peer ? peer : "");
  (void)w->head;
  return w;
}

int slweb_rate_allow(slweb_rate *r, const char *peer, unsigned long long now_ms) {
  if (!slweb_rate_enabled(r)) return 1;
  if (!peer) peer = "";

  if (r->global_max != 0) {
    r->global_n = window_count(r->global_stamps, r->global_n, now_ms, r->window_ms);
    if (r->global_n >= r->global_max) return 0;
  }
  slweb_peer_window *w = peer_of(r, peer);
  if (!w) return 0; /* OOM: fail closed */
  if (r->per_peer_max != 0) {
    w->n = window_count(w->stamps, w->n, now_ms, r->window_ms);
    if (w->n >= r->per_peer_max) return 0;
  }
  if (r->global_max != 0)
    if (!push_stamp(&r->global_stamps, &r->global_n, &r->global_cap, now_ms))
      return 0;
  if (!push_stamp(&w->stamps, &w->n, &w->cap, now_ms)) return 0;
  return 1;
}

void slweb_rate_sweep(slweb_rate *r, unsigned long long now_ms) {
  if (!r) return;
  r->global_n = window_count(r->global_stamps, r->global_n, now_ms, r->window_ms);
  size_t keep = 0;
  for (size_t i = 0; i < r->n_peers; ++i) {
    r->peers[i].n = window_count(r->peers[i].stamps, r->peers[i].n, now_ms, r->window_ms);
    if (r->peers[i].n == 0) {
      free(r->peers[i].stamps);
    } else {
      r->peers[keep++] = r->peers[i];
    }
  }
  r->n_peers = keep;
}

/* ================================================================== */
/*  Write auth + client IP                                             */
/* ================================================================== */
int slweb_constant_time_equals(const char *a, const char *b) {
  if (!a) a = "";
  if (!b) b = "";
  size_t la = strlen(a), lb = strlen(b);
  size_t n = la > lb ? la : lb;
  unsigned diff = (la == lb) ? 0u : 1u;
  for (size_t i = 0; i < n; ++i) {
    unsigned char ca = i < la ? (unsigned char)a[i] : 0;
    unsigned char cb = i < lb ? (unsigned char)b[i] : 0;
    diff |= (unsigned)(ca ^ cb);
  }
  return diff == 0 ? 1 : 0;
}

int slweb_write_authorize(const char *token, const char *authorization_header) {
  if (!token || !*token) return 1; /* no gate */
  const char *h = authorization_header ? authorization_header : "";
  size_t i = 0;
  while (h[i] && isspace((unsigned char)h[i])) ++i;
  size_t s = i;
  while (h[s] && !isspace((unsigned char)h[s])) ++s;
  /* scheme = h[i..s) */
  char scheme[16];
  size_t slen = s - i;
  if (slen >= sizeof scheme) return 0;
  for (size_t k = 0; k < slen; ++k) scheme[k] = (char)tolower((unsigned char)h[i + k]);
  scheme[slen] = 0;
  if (strcmp(scheme, "bearer") != 0) return 0;
  while (h[s] && isspace((unsigned char)h[s])) ++s;
  /* credential = h[s..end), trim trailing ws */
  size_t e = strlen(h);
  while (e > s && isspace((unsigned char)h[e - 1])) --e;
  char cred[SLWEB_MAX_PATH];
  size_t clen = e - s;
  if (clen >= sizeof cred) return 0;
  memcpy(cred, h + s, clen);
  cred[clen] = 0;
  return slweb_constant_time_equals(cred, token);
}

slweb_status slweb_client_ip(const char *peer_ip, const char *forwarded_for,
                             const char *const *trusted_proxies, size_t n_proxies,
                             char *out, size_t out_cap) {
  if (!out || out_cap == 0) return SLWEB_ERR_ARG;
  if (!peer_ip) peer_ip = "";
  if (!forwarded_for || !*forwarded_for) {
    copy_bounded(out, out_cap, peer_ip);
    return SLWEB_OK;
  }
  int trusted = 0;
  for (size_t i = 0; i < n_proxies; ++i)
    if (trusted_proxies && trusted_proxies[i] &&
        strcmp(trusted_proxies[i], peer_ip) == 0) { trusted = 1; break; }
  if (!trusted) {
    copy_bounded(out, out_cap, peer_ip);
    return SLWEB_OK;
  }
  /* left-most of a comma list is the origin client */
  const char *comma = strchr(forwarded_for, ',');
  size_t len = comma ? (size_t)(comma - forwarded_for) : strlen(forwarded_for);
  /* trim */
  size_t a = 0;
  while (a < len && isspace((unsigned char)forwarded_for[a])) ++a;
  while (len > a && isspace((unsigned char)forwarded_for[len - 1])) --len;
  if (len <= a) { copy_bounded(out, out_cap, peer_ip); return SLWEB_OK; }
  size_t cp = len - a;
  if (cp >= out_cap) cp = out_cap - 1;
  memcpy(out, forwarded_for + a, cp);
  out[cp] = 0;
  return SLWEB_OK;
}

/* ================================================================== */
/*  Keep-warm band math                                                */
/* ================================================================== */
slweb_warm slweb_warm_default(void) {
  slweb_warm w;
  w.enabled = 1;
  w.floor_pct = 11;
  w.ceiling_pct = 14;
  w.max_cores = 16;
  w.window_ms = 200;
  return w;
}

slweb_warm slweb_warm_sanitize(slweb_warm w) {
  if (w.floor_pct < 1) w.floor_pct = 1;
  if (w.floor_pct > 90) w.floor_pct = 90;
  if (w.ceiling_pct < w.floor_pct) w.ceiling_pct = w.floor_pct;
  if (w.ceiling_pct > 95) w.ceiling_pct = 95;
  if (w.max_cores < 1) w.max_cores = 1;
  if (w.max_cores > 16) w.max_cores = 16;
  if (w.window_ms < 50) w.window_ms = 50;
  if (w.window_ms > 2000) w.window_ms = 2000;
  return w;
}

double slweb_warm_target_pct(slweb_warm w) {
  w = slweb_warm_sanitize(w);
  return ((double)w.floor_pct + (double)w.ceiling_pct) / 2.0;
}

unsigned slweb_warm_core_count(unsigned max_cores) {
  long n = 1;
#if defined(_WIN32) || defined(_WIN64)
  /* Windows (XP through 11+): GetSystemInfo is available on every version. */
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  if (si.dwNumberOfProcessors > 0) n = (long)si.dwNumberOfProcessors;
#elif defined(_SC_NPROCESSORS_ONLN)
  /* POSIX / macOS. */
  long got = sysconf(_SC_NPROCESSORS_ONLN);
  if (got > 0) n = got;
#endif
  if (max_cores < 1) max_cores = 1;
  if (max_cores > 16) max_cores = 16;
  if (n < 1) n = 1;
  if ((unsigned)n > max_cores) n = (long)max_cores;
  return (unsigned)n;
}
