/* ==========================================================================
 * sleela_synchro.h -- Synchro: honest packet dispatch + latency measurement.
 *
 * The C/C++ re-authoring of the Synchro layer for the direct Sleela engine
 * build. Synchro sends timestamped probes to a destination, matches replies,
 * and reports the MEASURED latency distribution. It makes no delivery-time
 * guarantee (none is physically possible for arbitrary destinations); it
 * measures what actually happened and reports it honestly, including loss.
 *
 * A Synchro probe is a Sleela-owned, bounded resource (a handle into a fixed
 * table), the same discipline used for sockets/files. Handles are VM-local
 * integers, never raw OS descriptors.
 * ========================================================================== */
#ifndef SLEELA_SYNCHRO_H
#define SLEELA_SYNCHRO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum concurrent Synchro probes owned by one VM. */
#define SL_SYNCHRO_MAX 64
/* Bound on samples retained per probe for the measured distribution. */
#define SL_SYNCHRO_MAX_SAMPLES 4096

/* An opaque Synchro probe. Callers never see its fields; they hold an index. */
typedef struct SLSynchro SLSynchro;

/* Create a probe bound to a destination "host:port" (UDP echo/measurement).
 * Returns a heap probe or NULL on error. The probe owns no thread; it is driven
 * synchronously by dispatch/measure so it composes with the VM thread model. */
SLSynchro* slsynchro_open(const char* host, uint16_t port);

/* One dispatch cycle: send a timestamped probe of `payload_len` bytes and wait
 * up to `timeout_ms` for the echoed reply. Records the measured round-trip when
 * the reply matches, or a loss when it does not. Returns the measured RTT in
 * microseconds (>= 0) on a matched reply, or -1 on loss/timeout. Never blocks
 * longer than timeout_ms. */
int64_t slsynchro_dispatch(SLSynchro* s, size_t payload_len, int timeout_ms);

/* Number of dispatches so far and how many were matched (received). */
int64_t slsynchro_sent(const SLSynchro* s);
int64_t slsynchro_received(const SLSynchro* s);

/* Measured latency statistics in microseconds over retained samples. When no
 * sample has been received these report -1 (mean/min/max) / 0 (loss uses sent).
 * loss_permille is losses per 1000 dispatches (0..1000). */
int64_t slsynchro_mean_us(const SLSynchro* s);
int64_t slsynchro_min_us(const SLSynchro* s);
int64_t slsynchro_max_us(const SLSynchro* s);
/* p in [0,100]; returns the measured percentile RTT in us, or -1 if no data. */
int64_t slsynchro_percentile_us(const SLSynchro* s, int p);
int64_t slsynchro_loss_permille(const SLSynchro* s);

/* A one-line honest report the language surface can print, e.g.
 * "synchro host:port sent=N recv=M loss=..permille mean=..us p95=..us".
 * Writes at most cap-1 bytes + NUL into out; returns bytes written. */
int slsynchro_report(const SLSynchro* s, char* out, size_t cap);

/* Release the probe and its socket. Safe on NULL. */
void slsynchro_close(SLSynchro* s);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_SYNCHRO_H */
