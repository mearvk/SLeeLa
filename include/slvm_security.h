#ifndef SLEELA_SLVM_SECURITY_H
#define SLEELA_SLVM_SECURITY_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLVM_SECURITY_NORMAL=0, SLVM_SECURITY_ELEVATED=1, SLVM_SECURITY_RESTRICTED=2, SLVM_SECURITY_DENIED=3 } slvm_security_state_t;
typedef enum { SLVM_IO_FILE_READ=0, SLVM_IO_FILE_WRITE, SLVM_IO_NETWORK_READ, SLVM_IO_NETWORK_WRITE, SLVM_IO_FILE_LOAD, SLVM_IO_DYNAMIC_INSTANTIATION, SLVM_IO_RESOURCE_CONTROL } slvm_io_kind_t;
typedef struct {
 uint64_t instructions, resource_requests, object_requests, io_requests, io_bytes;
 uint64_t file_loads, dynamic_instantiations, network_requests, security_events;
 uint64_t window_requests, window_objects, window_io, window_instructions;
 uint32_t anomaly_score, object_rate_limit, io_rate_limit, request_rate_limit, anomaly_threshold;
 slvm_security_state_t state;
} slvm_security_t;
void slvm_security_init(slvm_security_t*);
void slvm_security_reset_window(slvm_security_t*);
void slvm_security_set_limits(slvm_security_t*,uint32_t,uint32_t,uint32_t,uint32_t);
int slvm_security_observe_instruction(slvm_security_t*);
int slvm_security_observe_resource(slvm_security_t*,uint32_t);
int slvm_security_observe_io(slvm_security_t*,slvm_io_kind_t,size_t);
slvm_security_state_t slvm_security_state(const slvm_security_t*);
uint32_t slvm_security_score(const slvm_security_t*);
#ifdef __cplusplus
}
#endif
#endif
