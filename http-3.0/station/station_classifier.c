/* ==========================================================================
 * station_classifier.c -- implementation of the shared Station classifier.
 * Pure logic; safe for both kernel and userland (no libc/kernel-only calls).
 * ========================================================================== */
#include "station_classifier.h"

#ifdef __KERNEL__
#include <linux/stddef.h>   /* NULL */
#else
#include <stddef.h>         /* NULL */
#endif

sl_hop_verb_t sl_classify_hop(int8_t hop_delta)
{
    if (hop_delta < 0) {
        return SL_HOP_CANCEL;   /* DIST-1 (or beyond) */
    }
    if (hop_delta > 0) {
        return SL_HOP_ADVANCE;  /* DIST+1 (or beyond) */
    }
    return SL_HOP_HOLD;         /* DIST+0 */
}

sl_condition_t sl_classify_condition(uint8_t tcp_flags, uint8_t urg_units)
{
    /* No URG bit, or zero magnitude: not a premise-bearing signal. */
    if ((tcp_flags & SL_TCP_URG) == 0u || urg_units == 0u) {
        return SL_COND_NONE;
    }
    /* EMERGENCY: URG+PSH+ACK at full scale (checked before LEGAL/WARNING). */
    if ((tcp_flags & (SL_TCP_URG | SL_TCP_PSH | SL_TCP_ACK)) ==
            (SL_TCP_URG | SL_TCP_PSH | SL_TCP_ACK) &&
        urg_units == SL_STATION_URG_MAX) {
        return SL_COND_EMERGENCY;
    }
    /* WARNING: URG+PSH (a caution that must be acknowledged). */
    if ((tcp_flags & (SL_TCP_URG | SL_TCP_PSH)) == (SL_TCP_URG | SL_TCP_PSH)) {
        return SL_COND_WARNING;
    }
    /* LEGAL: URG+ACK at full scale (a legally-framed assertion). */
    if ((tcp_flags & (SL_TCP_URG | SL_TCP_ACK)) == (SL_TCP_URG | SL_TCP_ACK) &&
        urg_units == SL_STATION_URG_MAX) {
        return SL_COND_LEGAL;
    }
    /* Otherwise a deliberate premise step. */
    return SL_COND_INTENTIONAL;
}

sl_classification_t sl_classify_signal(const sl_signal_t *sig, int current_open_premise)
{
    sl_classification_t out;
    out.condition = SL_COND_NONE;
    out.premise = SL_PREMISE_START;
    out.hop_verb = SL_HOP_HOLD;
    out.valid = 0;

    if (sig == NULL) {
        return out;
    }

    out.hop_verb = sl_classify_hop(sig->hop_delta);
    out.condition = sl_classify_condition(sig->tcp_flags, sig->urg_units);

    /* A bare ACK / non-URG signal carries no premise; still a valid event but
     * not a premise-bearing one. */
    if (out.condition == SL_COND_NONE) {
        out.valid = 0;
        return out;
    }

    /* Premise selection (§4). */
    switch (out.hop_verb) {
        case SL_HOP_CANCEL:
            out.premise = SL_PREMISE_CANCELLING;
            break;
        case SL_HOP_ADVANCE:
            /* First advance opens START; a subsequent advance yields
             * INTERMEDIARY. current_open_premise < 0 means "none open". */
            if (current_open_premise < 0 ||
                current_open_premise == (int)SL_PREMISE_CANCELLING) {
                out.premise = SL_PREMISE_START;
            } else if (current_open_premise == (int)SL_PREMISE_START) {
                out.premise = SL_PREMISE_INTERMEDIARY;
            } else {
                out.premise = SL_PREMISE_INTERMEDIARY; /* stays mediated */
            }
            break;
        case SL_HOP_HOLD:
        default:
            /* Re-assert the current premise (witnessed, not advanced). If none
             * is open, a HOLD opens START. */
            out.premise = (current_open_premise < 0)
                              ? SL_PREMISE_START
                              : (sl_premise_t)current_open_premise;
            break;
    }
    out.valid = 1;
    return out;
}
