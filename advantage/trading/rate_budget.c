/*
 * rate_budget.c - Implementation of the rate budget. See rate_budget.h.
 */
#include "rate_budget.h"

#include <math.h>
#include <stddef.h>

#define RB_MAX_VALUE 1e12

void rb_init(rb_state *st)
{
    if (st == NULL) return;
    st->limit = RB_UNKNOWN;
    st->remaining = RB_UNKNOWN;
    st->reset_seconds = RB_UNKNOWN;
    st->retry_after_seconds = RB_UNKNOWN;
}

rb_status rb_parse_nonneg(const char *text, double *out)
{
    double whole = 0.0, frac = 0.0, scale = 0.1;
    int digits = 0, seen_dot = 0;
    const char *p;

    if (text == NULL || out == NULL) return RB_ERR_ARG;

    p = text;
    while (*p == ' ' || *p == '\t') ++p;

    for (; *p != '\0'; ++p) {
        if (*p >= '0' && *p <= '9') {
            ++digits;
            if (!seen_dot) {
                whole = whole * 10.0 + (double)(*p - '0');
                if (whole > RB_MAX_VALUE) return RB_ERR_PARSE;
            } else {
                frac += (double)(*p - '0') * scale;
                scale *= 0.1;
            }
        } else if (*p == '.' && !seen_dot) {
            seen_dot = 1;
        } else {
            break;
        }
    }
    while (*p == ' ' || *p == '\t') ++p;
    if (*p != '\0' || digits == 0) return RB_ERR_PARSE;

    *out = whole + frac;
    return RB_OK;
}

rb_status rb_observe(rb_state *st, const char *limit, const char *remaining,
                     const char *reset_seconds, const char *retry_after)
{
    double l = 0.0, r = 0.0, s = 0.0, a = 0.0;
    rb_status rc;

    if (st == NULL) return RB_ERR_ARG;

    /* Parse everything first so a bad value changes nothing. */
    if (limit != NULL && (rc = rb_parse_nonneg(limit, &l)) != RB_OK) return rc;
    if (remaining != NULL &&
        (rc = rb_parse_nonneg(remaining, &r)) != RB_OK) return rc;
    if (reset_seconds != NULL &&
        (rc = rb_parse_nonneg(reset_seconds, &s)) != RB_OK) return rc;
    if (retry_after != NULL &&
        (rc = rb_parse_nonneg(retry_after, &a)) != RB_OK) return rc;

    if (limit != NULL) st->limit = l;
    if (remaining != NULL) st->remaining = r;
    if (reset_seconds != NULL) st->reset_seconds = s;
    st->retry_after_seconds = (retry_after != NULL) ? a : RB_UNKNOWN;
    return RB_OK;
}

rb_status rb_next_delay_ms(const rb_state *st, double reserve,
                           double min_interval_ms, double max_wait_ms,
                           double *out_ms)
{
    double delay;

    if (st == NULL || out_ms == NULL) return RB_ERR_ARG;
    if (!isfinite(reserve) || reserve < 0.0 || !isfinite(min_interval_ms) ||
        min_interval_ms < 0.0 || !isfinite(max_wait_ms) || max_wait_ms <= 0.0) {
        return RB_ERR_ARG;
    }

    if (st->retry_after_seconds >= 0.0) {
        delay = st->retry_after_seconds * 1000.0;
    } else if (st->remaining < 0.0 || st->reset_seconds < 0.0) {
        /* The server has not told us enough: stay at the floor pace. */
        delay = min_interval_ms;
    } else if (st->remaining <= reserve) {
        /* Nothing usable left: wait out the window. */
        delay = st->reset_seconds * 1000.0;
    } else {
        delay = (st->reset_seconds * 1000.0) / (st->remaining - reserve);
    }
    if (delay < min_interval_ms) delay = min_interval_ms;

    *out_ms = delay;
    return delay > max_wait_ms ? RB_ERR_WAIT : RB_OK;
}

const char *rb_strerror(rb_status st)
{
    switch (st) {
    case RB_OK:        return "ok";
    case RB_ERR_ARG:   return "invalid argument";
    case RB_ERR_PARSE: return "header value is not a plain non-negative number";
    case RB_ERR_WAIT:  return "required wait exceeds the allowed maximum";
    }
    return "unknown error";
}
