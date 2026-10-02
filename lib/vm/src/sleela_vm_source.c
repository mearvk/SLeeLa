#include "../include/sleela_vm_source.h"
int sleela_vm_requirements_validate(const sleela_vm_requirements_t *r){ if(!r) return -1; if(r->kind!=SLEELA_VM_SLVM && r->kind!=SLEELA_VM_SLJVM) return -2; if(!r->pointer_bits) return -3; if(!r->memory_limit) return -4; return 0; }
int sleela_vm_plan_modules(const sleela_vm_requirements_t *r, uint32_t n){ return sleela_vm_requirements_validate(r)==0 && n>0 ? 0 : -1; }
