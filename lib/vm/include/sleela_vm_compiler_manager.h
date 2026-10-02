#ifndef SLEELA_VM_COMPILER_MANAGER_H
#define SLEELA_VM_COMPILER_MANAGER_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLEELA_CM_BASIC_COMPLETE=1, SLEELA_CM_ADVANCED_TOTAL=2 } sleela_cm_profile_t;
typedef enum { SLEELA_CM_FINE=1, SLEELA_CM_MISSING=2, SLEELA_CM_EXCESS=3, SLEELA_CM_DEPENDENCY=4, SLEELA_CM_INVALID=5 } sleela_cm_status_t;
typedef struct { uint32_t profile_code, declared_object_count, required_object_count, minimum_object_count, maximum_object_count, category_count; uint32_t architecture, execution, memory, security, io, runtime, management, linkage, observability, build; } sleela_vm_compile_manifest_t;
typedef struct { uint32_t status_code, object_code, category_code, requirement_code, dependency_code; } sleela_vm_compile_finding_t;
int sleela_vm_cm_manifest_validate(const sleela_vm_compile_manifest_t*);
int sleela_vm_cm_object_count_status(const sleela_vm_compile_manifest_t*);
int sleela_vm_cm_category_status(const sleela_vm_compile_manifest_t*, uint32_t category, uint32_t actual);
int sleela_vm_cm_dependency_required(uint32_t object_code, uint32_t requirement_code);
const char* sleela_vm_cm_status_name(uint32_t status_code);
#ifdef __cplusplus
}
#endif
#endif
