#include "synchro_integration.h"

#include <string.h>

int synchro_integration_init(synchro_integration *integration, size_t window) {
    if (!integration) return -1;
    memset(integration, 0, sizeof(*integration));
    return synchro_stats_init(&integration->stats, window);
}

void synchro_integration_free(synchro_integration *integration) {
    if (!integration) return;
    synchro_stats_free(&integration->stats);
    integration->next_sequence = 0;
}

int synchro_integration_prepare(synchro_integration *integration,
                                uint8_t packet[16],
                                uint64_t sent_ns,
                                uint32_t *sequence) {
    uint32_t seq;
    if (!integration || !packet || !sequence) return -1;
    seq = ++integration->next_sequence;
    if (synchro_packet_encode(packet, 16, seq, sent_ns) != 16) return -1;
    *sequence = seq;
    return 16;
}

int synchro_integration_ack(synchro_integration *integration,
                            const uint8_t packet[16],
                            uint32_t expected_sequence,
                            uint64_t now_ns,
                            uint64_t sent_ns) {
    uint32_t sequence = 0;
    uint64_t wire_sent_ns = 0;
    uint64_t origin_ns;

    if (!integration || !packet) return -1;
    if (synchro_packet_decode(packet, 16, &sequence, &wire_sent_ns) != 0)
        return -2;
    if (sequence != expected_sequence)
        return -3;

    /* Prefer the caller's local send timestamp; otherwise use the timestamp
     * carried by the validated wire packet. */
    origin_ns = sent_ns ? sent_ns : wire_sent_ns;
    if (origin_ns == 0 || now_ns < origin_ns)
        return -4;

    return synchro_stats_record(&integration->stats, 1,
                                (double)(now_ns - origin_ns) / 1000000.0);
}

int synchro_integration_timeout(synchro_integration *integration) {
    if (!integration) return -1;
    return synchro_stats_record(&integration->stats, 0, 0.0);
}

const synchro_stats *synchro_integration_stats(
    const synchro_integration *integration) {
    return integration ? &integration->stats : NULL;
}
