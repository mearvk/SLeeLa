/* ==========================================================================
 * station_session.h -- Station witnessed session + state machine (J-STA-0001
 * §6/§7/§8). Folds classified signals from both parties into the manualog,
 * advances the witnessed state (OPEN/CLEAR/UNCLEAR/HUNG_UP/DISASTROUS), and
 * appends an append-only record store. Pure logic: usable by kernel or
 * userland (the kernel keeps one per registered session).
 * ========================================================================== */
#ifndef SLEELA_STATION_SESSION_H
#define SLEELA_STATION_SESSION_H

#include "station_abi.h"
#include "station_classifier.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SL_SESSION_MAX_RECORDS 256u

/* Per-party negotiation state within a session. */
typedef struct {
    int      open_premise;     /* sl_premise_t, or -1 if none open           */
    int      acked_peer;       /* 1 if this party ACKed the peer's premise   */
    int      finished;         /* 1 if this party sent FIN                    */
    uint32_t signal_count;
} sl_party_state_t;

/* A witnessed session: the joint state plus the append-only record store. */
typedef struct {
    uint64_t         session_id;
    sl_state_t       state;
    sl_party_state_t party[2];
    int              legal_asserted;   /* a LEGAL condition was witnessed     */
    int              sealed;           /* 1 after CLOSE                        */
    uint64_t         opened_ns;
    uint64_t         updated_ns;
    uint32_t         sequence;         /* monotonic record index              */
    sl_record_t      records[SL_SESSION_MAX_RECORDS];
    uint32_t         record_count;
} sl_session_t;

/* Open/initialize a witnessed session. */
void sl_session_open(sl_session_t *s, uint64_t session_id, uint64_t now_ns);

/*
 * Fold one signaling event into the session (the core "witness" step).
 * Classifies the signal, appends a record, updates per-party premise state,
 * and recomputes the joint state. Writes the classified record to `out_record`
 * if non-NULL. Returns 0 on success, -1 on invalid input or a sealed session.
 */
int sl_session_signal(sl_session_t *s, const sl_signal_t *sig, sl_record_t *out_record);

/* Note that a timeout elapsed with no signal from `party` -> may go HUNG_UP. */
void sl_session_timeout(sl_session_t *s, sl_party_t party, uint64_t now_ns);

/* Seal + close the session; its stored records become immutable. */
void sl_session_close(sl_session_t *s, uint64_t now_ns);

/* Fill a caller-supplied view with the current witnessed state. */
void sl_session_view(const sl_session_t *s, sl_session_view_t *view);

/* Human-readable names (for demos, logs, §17 admin). */
const char *sl_state_name(sl_state_t st);
const char *sl_condition_name(sl_condition_t c);
const char *sl_premise_name(sl_premise_t p);
const char *sl_hop_verb_name(sl_hop_verb_t v);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_STATION_SESSION_H */
