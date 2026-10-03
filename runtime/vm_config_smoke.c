#include "vm_config.h"
#include <stdio.h>
#include <stdlib.h>
int main(void){SLEELA_VM_CONFIG c;sleela_vm_config_defaults(&c);if(c.version!=11||!c.strict)return 1;if(sleela_vm_config_set_version(&c,1)||c.version!=1)return 2;if(!sleela_vm_config_set_version(&c,12))return 3;if(sleela_vm_config_load("config/vm.conf",&c)||c.version!=11)return 4;if(setenv("SLEELA_VM_VERSION","7",1))return 5;if(sleela_vm_config_apply_environment(&c)||c.version!=7)return 6;unsetenv("SLEELA_VM_VERSION");puts("VM CONFIG: PASS");return 0;}
