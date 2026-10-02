#ifndef SLEELA_SLVM_OS_H
#define SLEELA_SLVM_OS_H

#include "slvm_capability.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
 SLVM_OS_OK=0, SLVM_OS_DENIED=-100, SLVM_OS_INVALID_ARGUMENT=-101,
 SLVM_OS_NOT_FOUND=-102, SLVM_OS_RESOURCE_EXHAUSTED=-103,
 SLVM_OS_UNSUPPORTED=-104, SLVM_OS_PLATFORM_ERROR=-105
} slvm_os_status_t;
typedef struct slvm_os_adapter slvm_os_adapter_t;
typedef struct { slvm_os_status_t status; int32_t native_error; const char *operation; } slvm_os_result_t;
struct slvm_os_adapter { const char *name; slvm_os_result_t (*probe)(void); };
#ifdef __cplusplus
}
#endif
#endif
