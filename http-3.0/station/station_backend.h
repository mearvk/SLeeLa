/* ==========================================================================
 * station_backend.h -- userland transport to the Station (J-STA-0001 §9).
 *
 * Two interchangeable backends expose the SAME "known API" (open / signal /
 * query / close) so calling code is identical either way:
 *
 *   DEVICE     -- open /dev/sleela_station and drive the kernel driver by
 *                 ioctl (the real Router-as-Station path);
 *   SIMULATION -- an in-process Station holding sl_session_t directly, using
 *                 the same classifier/session logic, so the negotiation runs
 *                 with no privileges (and identical judgment) here.
 *
 * The simulation backend is what makes the manualog demonstrable in an
 * unprivileged environment; the device backend is what a deployed Router uses.
 * ========================================================================== */
#ifndef SLEELA_STATION_BACKEND_H
#define SLEELA_STATION_BACKEND_H

#include <stddef.h>

#include "station_abi.h"
#include "station_session.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_BACKEND_SIMULATION = 0, /* in-process; no privileges                  */
    SL_BACKEND_DEVICE     = 1  /* /dev/sleela_station via ioctl              */
} sl_backend_kind_t;

#define SL_BACKEND_MAX_SESSIONS 32u

typedef struct {
    sl_backend_kind_t kind;
    int               fd;          /* device fd (DEVICE), else -1            */
    /* simulation storage */
    sl_session_t      sessions[SL_BACKEND_MAX_SESSIONS];
    size_t            session_count;
    uint64_t          next_session_id;
    uint64_t          clock_ns;    /* simulated monotonic clock              */
} sl_station_t;

/*
 * Initialize a backend. For SL_BACKEND_DEVICE, `device_path` (or NULL for the
 * default SLEELA_STATION_DEVPATH) is opened; returns -1 if it cannot be opened.
 * For SL_BACKEND_SIMULATION the Station lives in-process. Returns 0 on success.
 */
int sl_station_init(sl_station_t *st, sl_backend_kind_t kind, const char *device_path);

/* Open a witnessed session; fills *session_id. Returns 0 on success. */
int sl_station_open(sl_station_t *st, uint64_t *session_id);

/* Feed one signaling event; returns the classified record via *out (optional). */
int sl_station_signal(sl_station_t *st, const sl_signal_t *sig, sl_record_t *out);

/* Read a session's current witnessed view. */
int sl_station_query(sl_station_t *st, uint64_t session_id, sl_session_view_t *view);

/* Seal + close a session. */
int sl_station_close_session(sl_station_t *st, uint64_t session_id);

/* Access the stored session (simulation only) for record inspection/demo. */
const sl_session_t *sl_station_peek(const sl_station_t *st, uint64_t session_id);

/* Release the backend (closes device fd if any). */
void sl_station_shutdown(sl_station_t *st);

/*
 * Convenience: build an sl_signal_t from human-level intent, so callers do not
 * hand-assemble flags. `advance` is the hop verb (+1/0/-1); `urg_units` is the
 * 0..8 magnitude; `flags_extra` OR's in ACK/PSH/FIN/RST as needed.
 */
sl_signal_t sl_make_signal(uint64_t session_id, sl_party_t party,
                           uint8_t urg_units, int hop_delta,
                           uint8_t flags_extra, uint64_t timestamp_ns);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_STATION_BACKEND_H */
