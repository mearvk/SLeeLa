#ifndef SLEELA_VM_CONFIG_H
#define SLEELA_VM_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_VM_MIN_VERSION 1
#define SLEELA_VM_MAX_VERSION 11
#define SLEELA_VM_DEFAULT_VERSION 11
typedef struct { int version; int strict; char profile[16]; } SLEELA_VM_CONFIG;
void sleela_vm_config_defaults(SLEELA_VM_CONFIG *);
int sleela_vm_config_load(const char *, SLEELA_VM_CONFIG *);
int sleela_vm_config_apply_environment(SLEELA_VM_CONFIG *);
int sleela_vm_config_set_version(SLEELA_VM_CONFIG *, int);
int sleela_vm_config_validate(const SLEELA_VM_CONFIG *);
#ifdef __cplusplus
}
#endif
#endif
