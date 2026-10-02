#ifndef SLEELA_SLVM_MEMORY_SECURITY_H
#define SLEELA_SLVM_MEMORY_SECURITY_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    SLVM_MEMORY_NORMAL=0,
    SLVM_MEMORY_PRESSURE=1,
    SLVM_MEMORY_RESTRICTED=2,
    SLVM_MEMORY_DENIED=3
} slvm_memory_security_state_t;
typedef enum {
    SLVM_MEMORY_ALLOW=0,
    SLVM_MEMORY_COLLECT=1,
    SLVM_MEMORY_THROTTLE=2,
    SLVM_MEMORY_DENY=3
} slvm_memory_decision_t;
typedef struct {
    size_t ceiling;
    size_t allocation_limit;
    size_t live_bytes;
    size_t live_objects;
    uint64_t allocation_requests;
    uint64_t denied_requests;
    uint64_t bytes_requested;
    uint64_t large_allocations;
    uint32_t burst_limit;
    uint32_t burst_count;
    slvm_memory_security_state_t state;
} slvm_memory_security_t;
void slvm_memory_security_init(slvm_memory_security_t *, size_t);
void slvm_memory_security_set_ceiling(slvm_memory_security_t *, size_t);
slvm_memory_decision_t slvm_memory_security_check(slvm_memory_security_t *, size_t, size_t);
void slvm_memory_security_record_free(slvm_memory_security_t *, size_t);
slvm_memory_security_state_t slvm_memory_security_state(const slvm_memory_security_t *);
#ifdef __cplusplus
}
#endif
#endif
