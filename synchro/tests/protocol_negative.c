#include "../c/synchro.h"
#include "../c/synchro_integration.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static void packet(void) {
    uint8_t p[16], b[16];
    uint32_t q;
    uint64_t t;

    assert(synchro_packet_encode(p, 16, 7, 123456789ULL) == 16);
    assert(synchro_packet_decode(p, 16, &q, &t) == 0);
    assert(q == 7 && t == 123456789ULL);

    memcpy(b, p, 16);
    b[0] ^= 1;
    assert(synchro_packet_decode(b, 16, &q, &t) == -2);
    assert(synchro_packet_decode(p, 15, &q, &t) == -1);
    assert(synchro_packet_decode(NULL, 16, &q, &t) == -1);
}

static void reject(void) {
    synchro_integration x;
    uint8_t p[16];
    uint32_t q;

    assert(synchro_integration_init(&x, 2) == 0);
    assert(synchro_integration_prepare(&x, p, 1000000000ULL, &q) == 16);
    assert(synchro_integration_ack(&x, p, q + 1, 1005000000ULL, 0) == -3);
    assert(synchro_integration_ack(&x, p, q, 999000000ULL, 0) == -4);

    p[0] ^= 1;
    assert(synchro_integration_ack(&x, p, q, 1005000000ULL, 0) == -2);
    assert(x.stats.sent == 0 && x.stats.acked == 0);

    synchro_integration_free(&x);
}

static void stats(void) {
    synchro_stats s;

    assert(synchro_stats_init(&s, 3) == 0);
    assert(synchro_stats_record(&s, 1, 3) == 0);
    assert(synchro_stats_record(&s, 1, 1) == 0);
    assert(synchro_percentile(&s, 50) == 1);
    assert(synchro_percentile(&s, 100) == 3);

    assert(synchro_stats_record(&s, 1, 5) == 0);
    assert(s.count == 3);
    assert(synchro_percentile(&s, 50) == 3);
    assert(synchro_percentile(&s, 100) == 5);

    /* The fourth observation exercises the bounded rolling window. */
    assert(synchro_stats_record(&s, 1, 7) == 0);
    assert(s.count == 3);
    assert(synchro_percentile(&s, 50) == 5);
    assert(synchro_percentile(&s, 100) == 7);

    assert(synchro_stats_record(&s, 0, 0) == 0);
    assert(s.sent == 5 && s.acked == 4 && s.lost == 1);
    assert(synchro_loss_rate(&s) == .2 && synchro_delivery_rate(&s) == .8);

    synchro_stats_free(&s);
}

int main(void) {
    packet();
    reject();
    stats();
    puts("synchro protocol suite: PASS");
    return 0;
}
