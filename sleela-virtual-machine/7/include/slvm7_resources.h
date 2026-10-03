#ifndef SLEELA_SLVM7_RESOURCES_H
#define SLEELA_SLVM7_RESOURCES_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t cpu_budget;
    uint64_t io_budget;
    uint64_t network_budget;
    uint64_t memory_budget;
    uint64_t cpu_used;
    uint64_t io_used;
    uint64_t network_used;
    uint64_t memory_used;
    uint32_t pressure;
    uint32_t throttled;
} slvm7_resource_state_t;

int slvm7_resource_validate(const slvm7_resource_state_t *);
int slvm7_resource_pressure(const slvm7_resource_state_t *);

#ifdef __cplusplus
}
#endif

#endif
