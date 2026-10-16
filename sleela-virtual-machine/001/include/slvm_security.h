#ifndef SLEELA_SLVM_SECURITY_H
#define SLEELA_SLVM_SECURITY_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    SLVM_SECURITY_NORMAL=0,
    SLVM_SECURITY_ELEVATED=1,
    SLVM_SECURITY_RESTRICTED=2,
    SLVM_SECURITY_DENIED=3
} slvm_security_state_t;
typedef enum {
    SLVM_IO_FILE=1, SLVM_IO_NETWORK, SLVM_IO_DEVICE, SLVM_IO_IPC, SLVM_IO_DYNAMIC
} slvm_io_kind_t;
typedef struct {
    uint64_t window_requests;
    uint64_t window_bytes;
    uint64_t instruction_count;
    uint64_t resource_requests;
    uint64_t object_requests;
    uint64_t io_requests;
    uint64_t io_bytes;
    uint64_t file_loads;
    uint64_t network_requests;
    uint64_t dynamic_instantiations;
    uint32_t anomaly_score;
    uint32_t request_limit;
    uint32_t object_limit;
    uint32_t io_limit;
    uint32_t anomaly_limit;
    slvm_security_state_t state;
} slvm_security_t;
void slvm_security_init(slvm_security_t *);
int slvm_security_observe_instruction(slvm_security_t *);
int slvm_security_observe_resource(slvm_security_t *, uint32_t);
int slvm_security_observe_io(slvm_security_t *, slvm_io_kind_t, size_t);
slvm_security_state_t slvm_security_state(const slvm_security_t *);
void slvm_security_reset_window(slvm_security_t *);
#ifdef __cplusplus
}
#endif
#endif
