#ifndef SLEELA_SLVM5_RECOVERY_H
#define SLEELA_SLVM5_RECOVERY_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint64_t checkpoint_id;
 uint64_t transaction_id;
 uint64_t sequence;
 uint64_t created_at_ns;
 const char *state_hash;
 const void *state;
 size_t state_size;
 uint32_t integrity_valid;
} slvm5_checkpoint_t;
int slvm5_checkpoint_validate(const slvm5_checkpoint_t*);
#ifdef __cplusplus
}
#endif
#endif
