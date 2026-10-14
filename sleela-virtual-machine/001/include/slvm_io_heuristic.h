#ifndef SLEELA_SLVM_IO_HEURISTIC_H
#define SLEELA_SLVM_IO_HEURISTIC_H
#include <stddef.h>
#include <stdint.h>
#include "slvm_security.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    SLVM_IO_HEURISTIC_ALLOW=0,
    SLVM_IO_HEURISTIC_MONITOR=1,
    SLVM_IO_HEURISTIC_THROTTLE=2,
    SLVM_IO_HEURISTIC_BLOCK=3
} slvm_io_decision_t;
typedef struct { uint64_t requests; uint64_t bytes; uint64_t bursts; } slvm_io_heuristic_t;
void slvm_io_heuristic_init(slvm_io_heuristic_t *);
slvm_io_decision_t slvm_io_heuristic_observe(slvm_io_heuristic_t *, slvm_io_kind_t, size_t, uint32_t, slvm_security_state_t);
#ifdef __cplusplus
}
#endif
#endif
