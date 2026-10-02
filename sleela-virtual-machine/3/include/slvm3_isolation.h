#ifndef SLEELA_SLVM3_ISOLATION_H
#define SLEELA_SLVM3_ISOLATION_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint64_t max_memory;
 uint64_t max_open_resources;
 uint64_t max_network_bytes;
 uint64_t max_file_bytes;
 uint64_t max_execution_steps;
 uint64_t max_wall_time_ms;
 int allow_network;
 int allow_files;
 int allow_processes;
} slvm3_isolation_policy_t;

int slvm3_isolation_validate(const slvm3_isolation_policy_t*);
int slvm3_isolation_charge(slvm3_isolation_policy_t*, uint64_t memory,
                           uint64_t network_bytes, uint64_t file_bytes);
#ifdef __cplusplus
}
#endif
#endif
