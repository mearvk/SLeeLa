#ifndef SLEELA_SYNCHRO_INTEGRATION_H
#define SLEELA_SYNCHRO_INTEGRATION_H

#include "synchro.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    synchro_stats stats;
    uint32_t next_sequence;
} synchro_integration;

int synchro_integration_init(synchro_integration *integration, size_t window);
void synchro_integration_free(synchro_integration *integration);

/* Prepare the next 16-byte wire packet. Returns 16 on success. */
int synchro_integration_prepare(synchro_integration *integration,
                                uint8_t packet[16],
                                uint64_t sent_ns,
                                uint32_t *sequence);

/* Validate an acknowledgement and record its locally measured RTT. */
int synchro_integration_ack(synchro_integration *integration,
                            const uint8_t packet[16],
                            uint32_t expected_sequence,
                            uint64_t now_ns,
                            uint64_t sent_ns);

/* Record a timeout or transport failure as a lost sample. */
int synchro_integration_timeout(synchro_integration *integration);

const synchro_stats *synchro_integration_stats(
    const synchro_integration *integration);

#ifdef __cplusplus
}
#endif

#endif
