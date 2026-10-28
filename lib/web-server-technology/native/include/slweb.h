/*
 * lib/web-server-technology/native/include/slweb.h
 *
 * SLeeLa Web Server Technology -- native C11 library (public C ABI).
 *
 * This is the reusable, dependency-free C core of the web-server-technology
 * subsystem. It is the library form of the model the standalone server builds
 * on (see the web-server-technology native sources): the two-sided interface
 *
 *   Side 1 -- a program goes to a domain and then a meme (one response unit);
 *   Side 2 -- a domain envelops many programs via subjects/warrants for causes,
 *             Scientist-gated,
 *
 * plus the production-hardening and CPU keep-warm primitives. It ships no
 * sockets and no HTTP wire handling: it is the decision/model layer that a
 * transport (the server, a test, or another embedder) drives. The adjacent
 * C++17 facade in ../cpp wraps this same ABI with RAII types.
 *
 * Mirrors the design of lib/sleela-sql (native C core + cpp facade).
 */
#ifndef SLEELA_SLWEB_H
#define SLEELA_SLWEB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLWEB_MAX_NAME 128
#define SLWEB_MAX_PATH 1024

/* ---- status codes ---------------------------------------------------- */
typedef enum {
  SLWEB_OK = 0,
  SLWEB_ERR_ARG = -1,       /* bad argument                         */
  SLWEB_ERR_OOM = -2,       /* allocation failed                    */
  SLWEB_ERR_NOTFOUND = -3,  /* no match / no such program           */
  SLWEB_ERR_DENIED = -4,    /* a gate refused (scientist/auth/rate) */
  SLWEB_ERR_RANGE = -5      /* value outside the allowed range      */
} slweb_status;

/* ================================================================== */
/*  Side 2 -- envelopment                                              */
/* ================================================================== */

typedef enum {
  SLWEB_PROGRAM_MEME = 0,   /* a single fixed response (the "meme")   */
  SLWEB_PROGRAM_STATIC = 1, /* a document-root subtree                */
  SLWEB_PROGRAM_SQL = 2     /* a /db-style SQL surface over a table dir */
} slweb_program_kind;

/* Opaque envelopment handle: a domain plus its programs and warrants. */
typedef struct slweb_envelopment slweb_envelopment;

/* Create an empty envelopment for a domain (NULL domain => ""). */
slweb_envelopment *slweb_env_new(const char *domain);
/* Release an envelopment. Safe on NULL. */
void slweb_env_free(slweb_envelopment *env);

/* The two gates. Side 2 only ACTIVATES with both set and >=1 warrant. */
void slweb_env_set_enabled(slweb_envelopment *env, int enabled);
void slweb_env_set_scientist(slweb_envelopment *env, int scientist);
int  slweb_env_enabled(const slweb_envelopment *env);
int  slweb_env_scientist(const slweb_envelopment *env);

/* Register a program. `mount` is the owned path prefix (e.g. "/lab"). For
 * SQL, `resource` is the table directory and `writable` gates mutations; for
 * static, `resource` is the document root; for meme, `resource` is the body.
 * Returns SLWEB_OK or an error (duplicate name, bad arg). */
slweb_status slweb_env_add_program(slweb_envelopment *env, const char *name,
                                   slweb_program_kind kind, const char *mount,
                                   const char *resource, int writable);

/* Register a warrant: a cause served for a subject (host, "*" = any) with an
 * optional path gate, activating a named program. `enabled` nonzero to arm. */
slweb_status slweb_env_add_warrant(slweb_envelopment *env, const char *cause,
                                   const char *subject, const char *path_prefix,
                                   const char *program, int enabled);

size_t slweb_env_program_count(const slweb_envelopment *env);
size_t slweb_env_warrant_count(const slweb_envelopment *env);

/* Is envelopment usable? (enabled AND scientist AND >=1 warrant). */
int slweb_env_active(const slweb_envelopment *env);

/*
 * Resolve (host, path) to a program using longest-matching-path-prefix (ties
 * break by declaration order). On a match returns SLWEB_OK and fills:
 *   out_program : the matched program name (buffer >= SLWEB_MAX_NAME),
 *   out_cause   : the matched warrant's cause (buffer >= SLWEB_MAX_PATH),
 *   out_kind    : the program kind.
 * Returns SLWEB_ERR_NOTFOUND if nothing matches (caller falls back to Side 1),
 * or SLWEB_ERR_DENIED if envelopment is not active.
 */
slweb_status slweb_env_resolve(const slweb_envelopment *env, const char *host,
                               const char *path, char *out_program,
                               char *out_cause, slweb_program_kind *out_kind);

/* Human-readable program-kind name ("meme"/"static"/"sql"). */
const char *slweb_program_kind_name(slweb_program_kind k);

/* ================================================================== */
/*  Hardening -- rate limiting + write authorization                   */
/* ================================================================== */

/* Opaque sliding-window rate limiter (per-peer + global). */
typedef struct slweb_rate slweb_rate;

/* Create a limiter: per_peer / global caps per `window_ms` (0 = that dimension
 * disabled). Returns NULL on OOM. */
slweb_rate *slweb_rate_new(unsigned per_peer_max, unsigned global_max,
                           unsigned window_ms);
void slweb_rate_free(slweb_rate *r);
int slweb_rate_enabled(const slweb_rate *r);

/* Record a request from `peer` at monotonic time `now_ms` and report whether
 * it is admitted (1) or rejected (0). Rejected requests are not counted. */
int slweb_rate_allow(slweb_rate *r, const char *peer, unsigned long long now_ms);

/* Drop aged-out per-peer windows (bounds memory). */
void slweb_rate_sweep(slweb_rate *r, unsigned long long now_ms);

/*
 * Constant-time string equality (independent of first-mismatch position).
 * Returns 1 if equal, 0 otherwise.
 */
int slweb_constant_time_equals(const char *a, const char *b);

/*
 * Authorize a write: if `token` is NULL/empty no auth is required (returns 1).
 * Otherwise `authorization_header` must be "Bearer <token>" (scheme compared
 * case-insensitively; credential compared in constant time). Returns 1 if
 * authorized, 0 otherwise.
 */
int slweb_write_authorize(const char *token, const char *authorization_header);

/*
 * Effective client IP. If `peer_ip` is in `trusted_proxies` (an array of
 * `n_proxies` C strings) and `forwarded_for` is non-empty, the left-most
 * forwarded client is returned; otherwise `peer_ip`. Prevents spoofing by
 * non-proxy peers. Writes up to `out_cap` bytes (NUL-terminated) to `out`.
 */
slweb_status slweb_client_ip(const char *peer_ip, const char *forwarded_for,
                             const char *const *trusted_proxies, size_t n_proxies,
                             char *out, size_t out_cap);

/* ================================================================== */
/*  CPU keep-warm idle floor -- band math (no threads)                 */
/* ================================================================== */

/*
 * The keep-warm policy, as pure math so it can be reused and tested without
 * spawning threads. The live threaded controller lives in the server
 * (web-server-technology/native/cpu_warmer.*); this is the band/target model.
 */
typedef struct {
  int enabled;            /* on by default when built by slweb_warm_default */
  unsigned floor_pct;     /* lower edge of the per-core band                */
  unsigned ceiling_pct;   /* upper edge of the per-core band                */
  unsigned max_cores;     /* clamp for warmer threads (1..16)               */
  unsigned window_ms;     /* control window                                 */
} slweb_warm;

/* A default keep-warm policy: ON, 11%-14%, up to 16 cores, 200ms window. */
slweb_warm slweb_warm_default(void);

/* Clamp a policy into the in-contract range (floor 1..90, ceiling floor..95,
 * cores 1..16, window 50..2000). Returns the corrected policy. */
slweb_warm slweb_warm_sanitize(slweb_warm w);

/* The controller's target (midpoint of the band), as a percentage. */
double slweb_warm_target_pct(slweb_warm w);

/* Detected online CPU count clamped to [1, max_cores] (and to [1,16]). */
unsigned slweb_warm_core_count(unsigned max_cores);

/* Library version string, e.g. "1.0.0". */
const char *slweb_version(void);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_SLWEB_H */
