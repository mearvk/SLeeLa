/*
 * rate_budget.h - Stay inside a server's published rate limit.
 *
 * Reads the numbers a server tells you (X-RateLimit-Limit, -Remaining, -Reset,
 * and Retry-After) and says how long to wait before the next request, so you
 * take the data you are entitled to at the fastest pace the server allows and
 * no faster. The point is to RESPECT the limit: the budget is spread evenly
 * over the window, a reserve is kept back, and a server's explicit
 * "Retry-After" is always obeyed.
 *
 * Pure: no sockets, no clock, no sleeping. The caller makes the request, feeds
 * the header values in, and does the waiting.
 *
 * Header conventions differ between servers. This module takes the reset as
 * SECONDS FROM NOW. Some APIs send an absolute epoch time instead; convert it
 * first (the C++ layer does, when asked). Retry-After in HTTP-date form is not
 * supported and is reported as a parse error, never guessed.
 *
 * Fail closed: any header value that is present but malformed returns an
 * error and leaves the state unchanged.
 */
#ifndef ADVANTAGE_RATE_BUDGET_H
#define ADVANTAGE_RATE_BUDGET_H

#ifdef __cplusplus
extern "C" {
#endif

#define RB_UNKNOWN (-1.0) /* a value the server has not told us */

typedef enum {
    RB_OK = 0,
    RB_ERR_ARG = -1,   /* NULL pointer or invalid option                  */
    RB_ERR_PARSE = -2, /* a header value is not a plain non-negative number */
    RB_ERR_WAIT = -3   /* required wait exceeds the caller's cap; *out_ms is
                          still set to the wait the server requires        */
} rb_status;

typedef struct {
    double limit;               /* requests allowed per window, or RB_UNKNOWN */
    double remaining;           /* requests left in this window               */
    double reset_seconds;       /* seconds until the window resets            */
    double retry_after_seconds; /* explicit server back-off, or RB_UNKNOWN    */
} rb_state;

void rb_init(rb_state *st);

/*
 * Strict parse of a non-negative decimal number ("120", "0.5"). No sign, no
 * exponent, no hex, no whitespace beyond optional surrounding spaces, and at
 * most 1e12.
 */
rb_status rb_parse_nonneg(const char *text, double *out);

/*
 * Record the header values from one response. A NULL argument means that
 * header was absent: the stored value is kept (except retry_after, which is
 * cleared, because a response without Retry-After lifts the back-off). If any
 * PRESENT value is malformed the call fails and the state is not modified.
 */
rb_status rb_observe(rb_state *st, const char *limit, const char *remaining,
                     const char *reset_seconds, const char *retry_after);

/*
 * Delay before the next request, in milliseconds.
 *   - an explicit Retry-After wins over everything;
 *   - otherwise, if remaining <= reserve, wait out the window (reset);
 *   - otherwise spread the usable budget (remaining - reserve) evenly over the
 *     time left in the window;
 *   - never less than min_interval_ms; unknown headers fall back to it.
 * If the delay exceeds max_wait_ms the call returns RB_ERR_WAIT with *out_ms
 * set to the required wait, so the caller can stop rather than sleep forever.
 */
rb_status rb_next_delay_ms(const rb_state *st, double reserve,
                           double min_interval_ms, double max_wait_ms,
                           double *out_ms);

const char *rb_strerror(rb_status st);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADVANTAGE_RATE_BUDGET_H */
