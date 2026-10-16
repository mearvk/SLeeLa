#include "slvm11_filesystem_module.h"
#include <stdio.h>
int main(void){const slvm11_filesystem_module_t m={"tac3",SLVM11_FS_MODULE_SCHEMA,"TAC3",1,0,4096,1,1,0,1,1};if(slvm11_filesystem_module_validate(&m)||!slvm11_filesystem_module_is_tac3(&m))return 1;puts("SLVM/11 filesystem module self-test: PASS");return 0;}
