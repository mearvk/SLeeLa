#ifndef SLEELA_SLVM3_ISOLATION_H
#define SLEELA_SLVM3_ISOLATION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct { uint64_t max_memory,max_open_resources,max_network_bytes,max_file_bytes,max_execution_steps,max_wall_time_ms; int allow_network,allow_files,allow_processes; } slvm3_isolation_policy_t;
int slvm3_isolation_validate(const slvm3_isolation_policy_t*);
int slvm3_isolation_charge(slvm3_isolation_policy_t*,uint64_t,uint64_t,uint64_t);
#ifdef __cplusplus
}
#endif
#endif
