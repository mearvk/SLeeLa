#ifndef SLEELA_SLVM2_H
#define SLEELA_SLVM2_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLVM2_VERSION_MAJOR 2
#define SLVM2_VERSION_MINOR 0
typedef enum { SLVM2_RUNTIME_AUTO=0, SLVM2_RUNTIME_LINUX=1, SLVM2_RUNTIME_WINDOWS=2, SLVM2_RUNTIME_MACOS=3 } slvm2_runtime_target_t;
typedef struct {
 uint32_t instance;
 slvm2_runtime_target_t runtime_target;
 uint64_t memory_limit;
 uint32_t security_profile;
 uint32_t crypto_provider;
 uint32_t link_policy;
 uint32_t observer_mode;
 uint32_t certificate_policy;
 uint32_t compliance_profile;
} slvm2_config_t;
int slvm2_config_defaults(slvm2_config_t*);
int slvm2_config_load_file(slvm2_config_t*, const char*);
int slvm2_runtime_validate(const slvm2_config_t*);
#ifdef __cplusplus
}
#endif
#endif
