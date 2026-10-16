/* ==========================================================================
 * sleela_munction.h -- Munction: the reach-composition engine (C core).
 *
 * The C/C++ re-authoring of Munction for the direct Sleela engine build. A
 * Munction is one sane sentence of reach: start an addressed reach, connect
 * over a system-method channel (pipe | file | tcp | sdps | crypto), coherently
 * send a datum, thatch interim stages, consume what returns, latch the reached
 * state, and close with a receipt.
 *
 * The structure of Reach is protected: transport may bump, but a reach is
 * receivable (always yields a receipt) and every send is coherent (offered vs.
 * acknowledged bytes accounted). The sentence is bounded to 4..16 verbs.
 *
 * A reach is a Sleela-owned bounded resource (a handle into a fixed table), the
 * same discipline used for sockets/files.
 * ========================================================================== */
#ifndef SLEELA_MUNCTION_H
#define SLEELA_MUNCTION_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Sentence sanity bound (verbs per Munction). */
#define SL_MUNCTION_MIN_VERBS 4
#define SL_MUNCTION_MAX_VERBS 16
/* Max concurrent reaches owned by one VM. */
#define SL_MUNCTION_MAX 64
/* Bounded buffer a channel can hold in flight (frames). */
#define SL_MUNCTION_QUEUE 64

/* System-method channels a reach can connect over. */
typedef enum {
    SL_MUN_NONE = 0,
    SL_MUN_PIPE,     /* pipe: OS pipe / FIFO (framed, in-process)     */
    SL_MUN_FILE,     /* file: file I/O (send appends, consume reads)  */
    SL_MUN_NET,      /* tcp:  internet TCP                            */
    SL_MUN_SDPS,     /* sdps: private packets (magic+len+MAC framing) */
    SL_MUN_CRYPTO    /* crypto: authenticated seal/open envelope       */
} SLMunChannel;

/* Reach outcome, recorded in the receipt. */
typedef enum {
    SL_MUN_REACHED = 0,   /* completed / latched or closed cleanly       */
    SL_MUN_CONTAINED,     /* a bump stopped at the boundary; partial real */
    SL_MUN_ABORTED        /* retired early; residual recorded             */
} SLMunOutcome;

typedef struct SLMunction SLMunction;

/* start(name): open a named reach. Returns a reach or NULL on error. */
SLMunction* slmunction_start(const char* name);

/* connect(uri): bind the reach to a channel resolved from the URI scheme.
 * Returns 0 on success, -1 on a boundary stop (unknown scheme / bad shape). */
int slmunction_connect(SLMunction* m, const char* uri);

/* enable(policy): permit an optional capability, e.g. "crypto:<keyref>" to key
 * the crypto envelope, or "retry". Returns 0, or -1 on a boundary stop. */
int slmunction_enable(SLMunction* m, const char* policy);

/* send(datum,len): coherently push a datum. Returns acknowledged byte count
 * (== len when coherent), or -1 on a boundary stop. */
int64_t slmunction_send(SLMunction* m, const char* datum, size_t len);

/* thatch(spec): weave interim stage names (comma/space list) over the path.
 * The stages are recorded in the receipt; interim transforms are identity in
 * the core (framing/telemetry live at higher layers). Returns 0 or -1. */
int slmunction_thatch(SLMunction* m, const char* spec);

/* consume(): pull one unit of what the channel returns into the reach's last
 * reception. Returns the received byte count (>=0) or -1 when absent/boundary. */
int64_t slmunction_consume(SLMunction* m);

/* Copy the most recent reception bytes into out (NUL-terminated); returns the
 * length (may be 0). */
int slmunction_last_reception(const SLMunction* m, char* out, size_t cap);

/* observe(): return a short channel-state string into out; returns length. */
int slmunction_observe(SLMunction* m, char* out, size_t cap);

/* latch(): hold/commit the reached state (at most once). Returns 0 or -1. */
int slmunction_latch(SLMunction* m);

/* closeWithReceipt(): orderly teardown. Fills the receipt string into out
 * (NUL-terminated) and returns the outcome. abort() is the negative closer. */
SLMunOutcome slmunction_close(SLMunction* m, char* receipt_out, size_t cap);
SLMunOutcome slmunction_abort(SLMunction* m, char* receipt_out, size_t cap);

/* Introspection for tests / the language surface. */
int slmunction_verb_count(const SLMunction* m);
int64_t slmunction_sent_bytes(const SLMunction* m);
int64_t slmunction_received_units(const SLMunction* m);
int slmunction_coherent(const SLMunction* m);   /* 1 if all sends coherent + no bumps */

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_MUNCTION_H */
