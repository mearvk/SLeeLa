#ifndef SLEELA_SLVM7_CHECKPOINT_H
#define SLEELA_SLVM7_CHECKPOINT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t checkpoint_id;
    uint64_t epoch;
    uint64_t created_at_ns;
    uint64_t artifact_sequence;
    const char *artifact_hash;
    const char *policy_hash;
    const char *lineage_hash;
    const char *integrity_hash;
    uint32_t complete;
    uint32_t replay_safe;
} slvm7_checkpoint_t;

int slvm7_checkpoint_validate(const slvm7_checkpoint_t *);

#ifdef __cplusplus
}
#endif

#endif
