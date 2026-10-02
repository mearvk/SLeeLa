#ifndef SLEELA_SLVM2_LINK_H
#define SLEELA_SLVM2_LINK_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    uint32_t schema;
    uint32_t vm_major;
    const char *module_id;
    const char *version;
    const char *hash;
    const char *signature;
    const char *requested_capabilities;
} slvm2_link_manifest_t;
int slvm2_link_validate(const slvm2_link_manifest_t*, int require_signature);
#ifdef __cplusplus
}
#endif
#endif
