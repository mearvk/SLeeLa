#ifndef SLEELA_VM_CONFIG_H
#define SLEELA_VM_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_VM_MIN_VERSION 1
#define SLEELA_VM_MAX_VERSION 11
#define SLEELA_VM_DEFAULT_VERSION 11
/* locked: when set, the selected `version` is pinned -- launchers/CLI must use
 * it and refuse any other requested generation. The user opts in (e.g. via
 * `sleela vm lock`); off by default so selection stays free. */
typedef struct { int version; int strict; int locked; char profile[16]; } SLEELA_VM_CONFIG;
void sleela_vm_config_defaults(SLEELA_VM_CONFIG *);
int sleela_vm_config_load(const char *, SLEELA_VM_CONFIG *);
int sleela_vm_config_apply_environment(SLEELA_VM_CONFIG *);
int sleela_vm_config_set_version(SLEELA_VM_CONFIG *, int);
int sleela_vm_config_validate(const SLEELA_VM_CONFIG *);
/* Persist the canonical vm.* keys (version/profile/strict/locked) to `path`,
 * preserving any other lines (e.g. the java.* block) already present. Returns
 * 0 on success, negative on error. Used to pin/unpin the VM generation. */
int sleela_vm_config_save(const char *, const SLEELA_VM_CONFIG *);
#ifdef __cplusplus
}
#endif
#endif
