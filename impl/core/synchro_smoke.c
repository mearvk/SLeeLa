/* synchro_smoke.c -- C-level smoke test for the Synchro measurement module.
 *
 * Synchro is honest: it measures, it never promises. This test exercises the
 * module directly (no VM): open a probe to a dark loopback port, dispatch with
 * a short timeout, and assert the accounting is truthful — a lost probe is a
 * loss (never a fabricated latency), and the stats stay consistent.
 */
#include "sleela_synchro.h"

#include <stdio.h>

static int failures = 0;
static void check(int cond, const char* what) {
    if (!cond) { printf("FAIL: %s\n", what); failures++; }
}

int main(void) {
    /* Port 9 (discard) on loopback is dark for UDP echo in the sandbox, so a
     * dispatch reliably times out. That is exactly the honest-loss path. */
    SLSynchro* s = slsynchro_open("127.0.0.1", 9);
    check(s != NULL, "probe opens to a loopback endpoint");
    if (!s) { printf("synchro smoke: FAILED (%d)\n", failures + 1); return 1; }

    check(slsynchro_sent(s) == 0, "no dispatches yet");
    check(slsynchro_received(s) == 0, "nothing received yet");
    check(slsynchro_mean_us(s) == -1, "no mean before any sample (no fabrication)");
    check(slsynchro_loss_permille(s) == 0, "loss is 0 before any dispatch");

    int64_t rtt = slsynchro_dispatch(s, 32, 50); /* 50ms budget to a dark port */
    check(slsynchro_sent(s) == 1, "one dispatch recorded");
    /* Loopback discard has no UDP responder, so we expect a loss (rtt == -1).
     * If some environment does echo, a matched reply is also acceptable; the
     * invariant is that the accounting is consistent either way. */
    if (rtt < 0) {
        check(slsynchro_received(s) == 0, "lost probe is not counted as received");
        check(slsynchro_loss_permille(s) == 1000, "a single lost probe is 1000 permille loss");
        check(slsynchro_mean_us(s) == -1, "no mean when nothing was received");
        check(slsynchro_percentile_us(s, 95) == -1, "no p95 without samples");
    } else {
        check(slsynchro_received(s) == 1, "matched reply is counted");
        check(slsynchro_loss_permille(s) == 0, "no loss on a matched reply");
        check(slsynchro_mean_us(s) >= 0, "mean is a real measured value");
    }

    char report[256];
    int n = slsynchro_report(s, report, sizeof(report));
    check(n > 0, "report renders");
    printf("report: %s\n", report);

    slsynchro_close(s);

    if (failures) { printf("synchro smoke: FAILED (%d)\n", failures); return 1; }
    printf("synchro smoke: PASS\n");
    return 0;
}
