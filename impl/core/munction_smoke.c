/* munction_smoke.c -- C-level smoke test for the Munction reach engine.
 *
 * Exercises the module directly (no VM): the verb ladder, coherent send,
 * receivable receipt, the SDPS framing round-trip, the crypto seal/open
 * envelope, and the 4..16 sanity bound. The structure of Reach is what we
 * protect: a send is coherent or the bump is recorded; a reception is real or
 * absent, never fabricated.
 */
#include "sleela_munction.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;
static void check(int cond, const char* what) {
    if (!cond) { printf("FAIL: %s\n", what); failures++; }
}

/* A full sane reach over the pipe channel; returns the outcome. */
static SLMunOutcome reach_pipe(void) {
    SLMunction* m = slmunction_start("smoke-pipe");
    check(m != NULL, "start opens a reach");
    check(slmunction_connect(m, "pipe:/tmp/mun-smoke") == 0, "connect pipe");
    check(slmunction_send(m, "hello", 5) == 5, "coherent send acks all bytes");
    check(slmunction_thatch(m, "frame,checksum") == 0, "thatch interims");
    int64_t got = slmunction_consume(m);           /* pulls the framed "hello" */
    check(got == 5, "consume returns the sent unit");
    char rec[64];
    slmunction_last_reception(m, rec, sizeof(rec));
    check(strcmp(rec, "hello") == 0, "reception round-trips the datum");
    check(slmunction_latch(m) == 0, "latch once");
    check(slmunction_coherent(m), "reach is coherent");
    char receipt[512];
    SLMunOutcome oc = slmunction_close(m, receipt, sizeof(receipt));
    printf("pipe %s\n", receipt);
    return oc;
}

/* SDPS private-packet framing must round-trip and stay coherent. */
static void reach_sdps(void) {
    SLMunction* m = slmunction_start("smoke-sdps");
    check(slmunction_connect(m, "sdps://tmcf:19866") == 0, "connect sdps");
    check(slmunction_send(m, "packet-datum", 12) == 12, "sdps coherent send");
    int64_t got = slmunction_consume(m);
    check(got == 12, "sdps deframes to the original length");
    char rec[64];
    slmunction_last_reception(m, rec, sizeof(rec));
    check(strcmp(rec, "packet-datum") == 0, "sdps payload round-trips through the frame+MAC");
    slmunction_latch(m);
    char receipt[512];
    SLMunOutcome oc = slmunction_close(m, receipt, sizeof(receipt));
    check(oc == SL_MUN_REACHED, "sdps reach REACHED");
    printf("sdps %s\n", receipt);
}

/* Crypto envelope: seal on send, open on consume, byte-exact plaintext. */
static void reach_crypto(void) {
    SLMunction* m = slmunction_start("smoke-crypto");
    check(slmunction_connect(m, "crypto:pipe:/tmp/mun-crypto") == 0, "connect crypto:pipe");
    check(slmunction_enable(m, "crypto:kref-2026") == 0, "enable key reference");
    check(slmunction_send(m, "topsecret", 9) == 9, "crypto coherent send");
    int64_t got = slmunction_consume(m);
    check(got == 9, "crypto consume opens to plaintext length");
    char rec[64];
    slmunction_last_reception(m, rec, sizeof(rec));
    check(strcmp(rec, "topsecret") == 0, "crypto seal->open round-trips plaintext byte-exact");
    slmunction_latch(m);
    char receipt[512];
    slmunction_close(m, receipt, sizeof(receipt));
    printf("crypto %s\n", receipt);
}

/* The 4..16 sanity bound: an under-length sentence closes CONTAINED. */
static void reach_bound(void) {
    SLMunction* m = slmunction_start("smoke-short");
    slmunction_connect(m, "pipe:/tmp/mun-short");
    /* start + connect + close = 3 verbs, below the floor of 4. */
    char receipt[512];
    SLMunOutcome oc = slmunction_close(m, receipt, sizeof(receipt));
    check(oc == SL_MUN_CONTAINED, "under-4-verb reach is CONTAINED at the boundary");
    printf("bound %s\n", receipt);
}

int main(void) {
    SLMunOutcome pipe_oc = reach_pipe();
    check(pipe_oc == SL_MUN_REACHED, "pipe reach REACHED");
    reach_sdps();
    reach_crypto();
    reach_bound();

    if (failures) { printf("munction smoke: FAILED (%d)\n", failures); return 1; }
    printf("munction smoke: PASS\n");
    return 0;
}
