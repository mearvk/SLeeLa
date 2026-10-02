#ifndef SLEELA_SLVM_CAPABILITY_H
#define SLEELA_SLVM_CAPABILITY_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t slvm_capability_id_t;

typedef enum {
    SLVM_CAP_FILES = 1,
    SLVM_CAP_DIRECTORIES,
    SLVM_CAP_PROCESSES,
    SLVM_CAP_THREADS,
    SLVM_CAP_NETWORK,
    SLVM_CAP_DNS,
    SLVM_CAP_TIME,
    SLVM_CAP_IPC,
    SLVM_CAP_SIGNALS,
    SLVM_CAP_MEMORY,
    SLVM_CAP_DYNAMIC_LIBRARIES,
    SLVM_CAP_DEVICES,
    SLVM_CAP_SECURITY,
    SLVM_CAP_SYSTEM,
    SLVM_CAP_TERMINAL,
    SLVM_CAP_RANDOM,
    SLVM_CAP_CRYPTO,
    SLVM_CAP_GUI_MEDIA
} slvm_capability_domain_t;

typedef struct {
    slvm_capability_id_t id;
    slvm_capability_domain_t domain;
    uint64_t rights;
    uint64_t resource_limit;
} slvm_capability_t;

#ifdef __cplusplus
}
#endif

#endif
