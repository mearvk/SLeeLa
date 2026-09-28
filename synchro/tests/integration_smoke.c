#include "../c/synchro_integration.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
    synchro_integration integration;
    uint8_t packet[16];
    uint32_t sequence = 0;
    const uint64_t sent_ns = 1000000000ULL;
    const uint64_t now_ns = 1005000000ULL;

    assert(synchro_integration_init(&integration, 8) == 0);
    assert(synchro_integration_prepare(&integration, packet, sent_ns, &sequence) == 16);
    assert(sequence == 1);
    assert(synchro_integration_ack(&integration, packet, sequence, now_ns, 0) == 0);
    assert(integration.stats.sent == 1);
    assert(integration.stats.acked == 1);
    assert(integration.stats.lost == 0);
    assert(integration.stats.count == 1);
    assert(integration.stats.rtts_ms[0] > 0.49 &&
           integration.stats.rtts_ms[0] < 0.51);

    assert(synchro_integration_timeout(&integration) == 0);
    assert(integration.stats.sent == 2);
    assert(integration.stats.acked == 1);
    assert(integration.stats.lost == 1);

    synchro_integration_free(&integration);
    puts("synchro integration smoke: PASS");
    return 0;
}
