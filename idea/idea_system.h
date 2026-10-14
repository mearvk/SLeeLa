/*
 * idea_system.h -- Idea System(TM) native core, public C ABI.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * This header is the VM-facing contract for the special native code the
 * Standard SLeeLa VM may need to implement that is not originally Sleela
 * (the Sleela "Sleela runnable" lives under /lib/idea).  It is pure C with
 * a stable, extern "C" surface so the C++ facade (idea_system.cpp) and the
 * Sleela native bridge can both link against it.
 *
 * The narrative it supports:
 *   - A signal-distributed System loads and receives its signal input from a
 *     known internet resource: a hex code the Content Author controls, served
 *     from github.com.  The default locator is the Author's wiki page.
 *   - That input drives a forward, LINEAR solution Series that may be
 *     arbitrarily deep but is sized to <= 30 hours of content with <= 9
 *     decisions that are seemly and important (design goals; law is careful,
 *     so bounded exceptions are tolerated and merely recorded).
 *   - The System is in Maintenance of System and then State.  The State IS the
 *     System; the System is prevaricated upon its State.  A user cannot really
 *     know the difference: the System presents what it presents -- its State.
 *     Not all is present/capable in a given State.
 *   - Time progresses forward and is ultimate and company to the Series.  The
 *     stream (0..30h) may SEEM to loop or retry, but it is consistent in Time.
 */
#ifndef IDEA_SYSTEM_H
#define IDEA_SYSTEM_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Versioning ------------------------------------------------------- */
#define IDEA_SYSTEM_NAME        "Idea System(TM)"
#define IDEA_SYSTEM_VERSION     "1.0.0"

/* ---- Design-goal bounds (law is careful; these are seemly, not rigid) -- */
/* The Series is bounded to <= 30 hours of content.                        */
#define IDEA_MAX_HOURS          30
#define IDEA_MAX_SECONDS        (IDEA_MAX_HOURS * 3600)
/* <= 9 decisions that are seemly, important.                              */
#define IDEA_MAX_DECISIONS      9
/* The Series may be arbitrarily deep; this is a generous reachability cap  */
/* aligned with the SLeeLa Constitution (system depth 3024).                */
#define IDEA_MAX_SERIES_DEPTH   3024
/* Complexity degree max from the Constitution (1 direct .. 4 federated).   */
#define IDEA_MAX_DEGREE         4

/* ---- Status codes ----------------------------------------------------- */
typedef enum idea_status {
    IDEA_OK                 =  0,
    IDEA_INVALID            = -1,  /* bad argument / null handle            */
    IDEA_LIMIT              = -2,  /* a design-goal bound was exceeded      */
    IDEA_NOT_FOUND          = -3,
    IDEA_INPUT_UNAVAILABLE  = -4,  /* internet resource not reachable       */
    IDEA_INPUT_MALFORMED    = -5,  /* fetched payload was not valid hex     */
    IDEA_EXHAUSTED          = -6,  /* Series reached its end of Time        */
    IDEA_CORRUPT            = -7
} idea_status;

/* The System presents exactly one State at a time.  The State IS the       */
/* System; a user cannot know which is which -- only what is presented.     */
typedef enum idea_state_kind {
    IDEA_STATE_LOADING   = 0,  /* loading the signal-distributed system     */
    IDEA_STATE_RECEIVING = 1,  /* receiving signal input from the internet  */
    IDEA_STATE_SOLVING   = 2,  /* advancing through the solution Series     */
    IDEA_STATE_DECIDING  = 3,  /* at a seemly, important decision point     */
    IDEA_STATE_LOOPING   = 4,  /* seems to loop/retry, but consistent       */
    IDEA_STATE_PRESENT   = 5,  /* presenting what is present/capable        */
    IDEA_STATE_FINAL     = 6   /* Finality: end of Time for this Series     */
} idea_state_kind;

/* Organization axes -- the Scheme on which the System is organized.        */
typedef enum idea_scheme {
    IDEA_SCHEME_CONCEPT  = 0,  /* organization based on Concepts            */
    IDEA_SCHEME_FINALITY = 1,  /* organization based on Finality            */
    IDEA_SCHEME_SIGNAL   = 2,  /* organization based on the Signal/Stream   */
    IDEA_SCHEME_SOCIETY  = 3   /* what happens in the Society               */
} idea_scheme;

/* Opaque handle to a running Idea System(TM).  Callers never see internals  */
/* (the same bounded-handle discipline the SLeeLa core uses).               */
typedef struct idea_system idea_system;

/* ---- The known internet resource (Author-controlled signal source) ----- */
/*
 * The signal input is a hex code the Content Author controls, served from
 * github.com.  This is the default locator; the Author (or the Sleela
 * runnable) may override it with idea_set_source_url before loading.  The
 * core itself performs no network I/O at construction or test time -- the
 * fetch is routed through a pluggable fetcher (see idea_fetch_fn) so the
 * System is deterministic and testable offline.
 */
#define IDEA_DEFAULT_SOURCE_URL \
    "https://github.com/mearvk/Senior.Senate.Attorney.E44Hrs/wiki/US-Title-Brands"

/*
 * A fetcher pulls the raw body of the known internet resource into caller
 * memory.  Returns the number of bytes written to buf (<= cap), or a negative
 * idea_status on failure.  The returned bytes are expected to contain a hex
 * code (whitespace and 0x prefixes are tolerated by the decoder).
 *
 * user is the opaque pointer registered with idea_set_fetcher.
 */
typedef long (*idea_fetch_fn)(const char *url, char *buf, size_t cap, void *user);

/* ---- Lifecycle -------------------------------------------------------- */

/* Create a fresh System in state IDEA_STATE_LOADING.  Returns NULL on OOM. */
idea_system *idea_create(void);

/* Destroy a System and release all its memory.  NULL is a no-op.           */
void idea_destroy(idea_system *sys);

/* Override the known internet resource URL (defaults to IDEA_DEFAULT_...).  */
idea_status idea_set_source_url(idea_system *sys, const char *url);
const char *idea_source_url(const idea_system *sys);

/*
 * Register the fetcher used to receive signal input from the internet
 * resource, plus an opaque user pointer handed back to it.  If no fetcher is
 * registered, idea_receive_signal returns IDEA_INPUT_UNAVAILABLE.
 */
idea_status idea_set_fetcher(idea_system *sys, idea_fetch_fn fetch, void *user);

/* ---- Signal input (the hex code from github.com) ---------------------- */

/*
 * Receive the signal: fetch the Author's hex code from the known resource and
 * fold it into the System as the seed of the Series.  Advances the presented
 * State to IDEA_STATE_RECEIVING.  On success the decoded signal bytes are
 * retained inside the System and used to size/seed the Series.
 */
idea_status idea_receive_signal(idea_system *sys);

/*
 * Feed a hex code directly (bypassing the fetcher) -- used by the Author to
 * provide the URL data, and by tests.  hex may contain spaces, newlines, and
 * optional 0x prefixes; it must decode to at least one byte.
 */
idea_status idea_feed_hex(idea_system *sys, const char *hex);

/* Number of decoded signal bytes currently held, or 0.                     */
size_t idea_signal_len(const idea_system *sys);

/* ---- The solution Series (forward, linear; Time is ultimate) ---------- */

/*
 * Build the solution Series from the received signal.  The Series is linear
 * and forward: a sequence of stages across 0..30h, with <= 9 decision points.
 * Depth may be arbitrary up to IDEA_MAX_SERIES_DEPTH.  Must be called after a
 * signal has been received/fed; otherwise IDEA_INVALID.
 */
idea_status idea_build_series(idea_system *sys);

/*
 * Advance Time by delta_seconds and let the System maintain itself (System in
 * Maintenance of System and then State).  This is the single forward step:
 * Time progresses, the Series moves, and the presented State is recomputed.
 * It may SEEM to loop/retry (IDEA_STATE_LOOPING) but is consistent in Time.
 * Returns IDEA_EXHAUSTED once Time reaches the end of the Series (Finality).
 */
idea_status idea_tick(idea_system *sys, long delta_seconds);

/* ---- Presentation (the System presents its State) --------------------- */

/* The State the System currently presents.                                 */
idea_state_kind idea_present_state(const idea_system *sys);

/* Elapsed Time in seconds (0..IDEA_MAX_SECONDS).                            */
long idea_elapsed_seconds(const idea_system *sys);

/* Fraction of the Series completed, 0.0..1.0 (Time is ultimate).           */
double idea_progress(const idea_system *sys);

/* How many of the <= 9 seemly decisions have been reached so far.          */
int idea_decisions_taken(const idea_system *sys);

/* The current depth within the (possibly deep) Series.                     */
int idea_series_depth(const idea_system *sys);

/*
 * Not all is present/capable in a given State.  Reports whether a named
 * capability is present in the System's current State.  Unknown names and
 * NULL yield 0 (not present).
 */
int idea_is_capable(const idea_system *sys, const char *capability);

/*
 * Render what the System presents -- a human-readable line describing its
 * current State, Time, progress, decisions, and scheme.  Writes a
 * NUL-terminated string into buf (truncated to cap) and returns the number of
 * bytes that would be written (excluding NUL), or negative on error.
 */
long idea_present(const idea_system *sys, char *buf, size_t cap);

/* ---- Organization (Scheme / Concepts / Finality) ---------------------- */

/* Set the organizing Scheme.  Defaults to IDEA_SCHEME_CONCEPT.             */
idea_status idea_set_scheme(idea_system *sys, idea_scheme scheme);
idea_scheme idea_scheme_of(const idea_system *sys);

/*
 * Register an AI objective the System pursues as it advances (bounded; at
 * most IDEA_MAX_DECISIONS objectives, mirroring the seemly-decision budget).
 * Returns IDEA_LIMIT once the budget is full.
 */
idea_status idea_add_objective(idea_system *sys, const char *objective);
int idea_objective_count(const idea_system *sys);

/* Human-readable name for a state kind / status / scheme (never NULL).     */
const char *idea_state_name(idea_state_kind k);
const char *idea_status_name(idea_status s);
const char *idea_scheme_name(idea_scheme s);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IDEA_SYSTEM_H */
