/* ==========================================================================
 * station_session.c -- Station witnessed session + state machine.
 * Pure logic; no libc/kernel-only calls beyond memset via a tiny local zero.
 * ========================================================================== */
#include "station_session.h"

#ifdef __KERNEL__
#include <linux/string.h>   /* memset */
#include <linux/stddef.h>   /* NULL   */
#else
#include <string.h>
#include <stddef.h>
#endif

void sl_session_open(sl_session_t *s, uint64_t session_id, uint64_t now_ns)
{
    if (s == NULL) {
        return;
    }
    memset(s, 0, sizeof(*s));
    s->session_id = session_id;
    s->state = SL_STATE_OPEN;
    s->party[0].open_premise = -1;
    s->party[1].open_premise = -1;
    s->opened_ns = now_ns;
    s->updated_ns = now_ns;
}

static int append_record(sl_session_t *s, const sl_signal_t *sig,
                         const sl_classification_t *cls)
{
    sl_record_t *r;
    if (s->record_count >= SL_SESSION_MAX_RECORDS) {
        return -1; /* store full; caller keeps state but stops recording */
    }
    r = &s->records[s->record_count];
    r->session_id = s->session_id;
    r->party = sig->party;
    r->condition = (uint8_t)cls->condition;
    r->premise = (uint8_t)cls->premise;
    r->hop_verb = (int8_t)cls->hop_verb;
    r->timestamp_ns = sig->timestamp_ns;
    r->sequence = s->sequence++;
    ++s->record_count;
    return 0;
}

/* Both parties agreed: each opened INTERMEDIARY and each ACKed the peer. */
static int both_agreed(const sl_session_t *s)
{
    return s->party[0].open_premise == (int)SL_PREMISE_INTERMEDIARY &&
           s->party[1].open_premise == (int)SL_PREMISE_INTERMEDIARY &&
           s->party[0].acked_peer && s->party[1].acked_peer;
}

int sl_session_signal(sl_session_t *s, const sl_signal_t *sig, sl_record_t *out_record)
{
    sl_classification_t cls;
    sl_party_state_t *ps;
    if (s == NULL || sig == NULL || s->sealed) {
        return -1;
    }
    if (sig->party > (uint8_t)SL_PARTY_B) {
        return -1;
    }
    if (s->state == SL_STATE_CLEAR || s->state == SL_STATE_DISASTROUS) {
        return -1; /* terminal; no further folding */
    }
    ps = &s->party[sig->party];

    /* A hard abort ends the negotiation disastrously, regardless of URG. */
    if (sig->tcp_flags & SL_TCP_RST) {
        cls.condition = SL_COND_WARNING;
        cls.premise = SL_PREMISE_CANCELLING;
        cls.hop_verb = SL_HOP_CANCEL;
        cls.valid = 1;
        (void)append_record(s, sig, &cls);
        s->state = SL_STATE_DISASTROUS;
        s->updated_ns = sig->timestamp_ns;
        if (out_record != NULL) {
            *out_record = s->records[s->record_count ? s->record_count - 1u : 0u];
        }
        return 0;
    }

    /* An orderly FIN marks this party finished. */
    if (sig->tcp_flags & SL_TCP_FIN) {
        ps->finished = 1;
    }

    /* An ACK (without URG premise) records acknowledgement of the peer. */
    if ((sig->tcp_flags & SL_TCP_ACK) && (sig->tcp_flags & SL_TCP_URG) == 0u) {
        ps->acked_peer = 1;
    }

    cls = sl_classify_signal(sig, ps->open_premise);
    ++ps->signal_count;
    s->updated_ns = sig->timestamp_ns;

    if (cls.valid) {
        (void)append_record(s, sig, &cls);
        if (cls.condition == SL_COND_LEGAL) {
            s->legal_asserted = 1;
        }
        /* A premise that both is CANCELLING while a LEGAL premise stands, or a
         * LEGAL premise arriving after a CANCELLING from the same party, is a
         * contradiction the Station flags as DISASTROUS. */
        if (s->legal_asserted && cls.premise == SL_PREMISE_CANCELLING &&
            cls.condition == SL_COND_LEGAL) {
            s->state = SL_STATE_DISASTROUS;
            if (out_record != NULL && s->record_count) {
                *out_record = s->records[s->record_count - 1u];
            }
            return 0;
        }
        ps->open_premise = (int)cls.premise;
        /* A premise signal that also carries ACK acknowledges the peer. */
        if (sig->tcp_flags & SL_TCP_ACK) {
            ps->acked_peer = 1;
        }
    }

    /* Recompute joint state. */
    if (both_agreed(s)) {
        s->state = SL_STATE_CLEAR;
    } else if (s->party[0].finished || s->party[1].finished) {
        s->state = SL_STATE_HUNG_UP;
    } else if ((s->party[0].open_premise == (int)SL_PREMISE_CANCELLING ||
                s->party[1].open_premise == (int)SL_PREMISE_CANCELLING)) {
        /* A standing cancellation with no re-advance leaves things UNCLEAR,
         * but the negotiation may still continue, so this is not terminal. */
        s->state = SL_STATE_UNCLEAR;
    } else {
        s->state = SL_STATE_OPEN;
    }

    if (out_record != NULL && s->record_count) {
        *out_record = s->records[s->record_count - 1u];
    }
    return 0;
}

void sl_session_timeout(sl_session_t *s, sl_party_t party, uint64_t now_ns)
{
    if (s == NULL || s->sealed || party > SL_PARTY_B) {
        return;
    }
    if (s->state == SL_STATE_CLEAR || s->state == SL_STATE_DISASTROUS) {
        return;
    }
    s->party[party].finished = 1;
    s->state = SL_STATE_HUNG_UP;
    s->updated_ns = now_ns;
}

void sl_session_close(sl_session_t *s, uint64_t now_ns)
{
    if (s == NULL || s->sealed) {
        return;
    }
    /* If still merely OPEN at close, the outcome was never made definite. */
    if (s->state == SL_STATE_OPEN) {
        s->state = SL_STATE_UNCLEAR;
    }
    s->sealed = 1;
    s->updated_ns = now_ns;
}

void sl_session_view(const sl_session_t *s, sl_session_view_t *view)
{
    if (s == NULL || view == NULL) {
        return;
    }
    memset(view, 0, sizeof(*view));
    view->session_id = s->session_id;
    view->state = (uint8_t)s->state;
    view->last_premise_a = (uint8_t)(s->party[0].open_premise < 0 ? 0 : s->party[0].open_premise);
    view->last_premise_b = (uint8_t)(s->party[1].open_premise < 0 ? 0 : s->party[1].open_premise);
    view->record_count = s->record_count;
    view->opened_ns = s->opened_ns;
    view->updated_ns = s->updated_ns;
}

const char *sl_state_name(sl_state_t st)
{
    switch (st) {
        case SL_STATE_IDLE:       return "IDLE";
        case SL_STATE_OPEN:       return "OPEN";
        case SL_STATE_CLEAR:      return "CLEAR";
        case SL_STATE_UNCLEAR:    return "UNCLEAR";
        case SL_STATE_HUNG_UP:    return "HUNG-UP";
        case SL_STATE_DISASTROUS: return "DISASTROUS";
        default:                  return "?";
    }
}

const char *sl_condition_name(sl_condition_t c)
{
    switch (c) {
        case SL_COND_NONE:        return "NONE";
        case SL_COND_INTENTIONAL: return "INTENTIONAL";
        case SL_COND_LEGAL:       return "LEGAL";
        case SL_COND_WARNING:     return "WARNING";
        case SL_COND_EMERGENCY:   return "EMERGENCY";
        default:                  return "?";
    }
}

const char *sl_premise_name(sl_premise_t p)
{
    switch (p) {
        case SL_PREMISE_START:        return "START";
        case SL_PREMISE_INTERMEDIARY: return "INTERMEDIARY";
        case SL_PREMISE_CANCELLING:   return "CANCELLING";
        default:                      return "?";
    }
}

const char *sl_hop_verb_name(sl_hop_verb_t v)
{
    switch (v) {
        case SL_HOP_CANCEL:  return "CANCEL(DIST-1)";
        case SL_HOP_HOLD:    return "HOLD(DIST+0)";
        case SL_HOP_ADVANCE: return "ADVANCE(DIST+1)";
        default:             return "?";
    }
}
