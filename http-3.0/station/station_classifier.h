/* ==========================================================================
 * station_classifier.h -- shared Station judgment (spec J-STA-0001 §4/§5).
 *
 * The SAME classifier runs in the kernel Netfilter hook and in userland, so a
 * Router's judgment is identical wherever the Station executes. It is pure:
 * given one signal, it yields the condition, premise, and hop verb. It uses
 * only <station_abi.h> types and no libc/kernel-only calls, so it compiles in
 * both __KERNEL__ and userland translation units.
 * ========================================================================== */
#ifndef SLEELA_STATION_CLASSIFIER_H
#define SLEELA_STATION_CLASSIFIER_H

#include "station_abi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Result of classifying one signal (before it is folded into session state). */
typedef struct {
    sl_condition_t condition;
    sl_premise_t   premise;   /* premise this signal selects, given current    */
    sl_hop_verb_t  hop_verb;
    int            valid;     /* 0 if the signal is not a well-formed premise  */
} sl_classification_t;

/* Map the raw hop delta to a verb (clamps to CANCEL/HOLD/ADVANCE). */
sl_hop_verb_t sl_classify_hop(int8_t hop_delta);

/* Classify the condition from the flag combination + urg magnitude (§5). */
sl_condition_t sl_classify_condition(uint8_t tcp_flags, uint8_t urg_units);

/*
 * Full classification of one signal, given the premise currently open for the
 * signaling party (or SL_PREMISE_START-1 == -1 sentinel meaning "none open").
 * `current_open` should be -1 when the party has no premise open yet, else the
 * last premise value. Returns condition/premise/hop_verb and validity.
 */
sl_classification_t sl_classify_signal(const sl_signal_t *sig, int current_open_premise);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_STATION_CLASSIFIER_H */
