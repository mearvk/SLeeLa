#include "slvm9.h"
#include "slvm9_filesystem.h"
#include "slvm9_adapter.h"
#include "slvm9_mount.h"
#include "slvm9_fsop.h"
#include "slvm9_context.h"
#include "slvm9_supervisor.h"
#include <assert.h>
#include <stdio.h>
int main(void){
 slvm9_platform_t p={SLVM9_LINUX,1,0,"Linux",1,1,1,1,1}; assert(slvm9_platform_validate(&p)==SLVM9_OK);
 slvm9_filesystem_t f={1,7,100000,50000,4096,"tac3","uuid","/data",SLVM9_FS_READ|SLVM9_FS_WRITE|SLVM9_FS_RECOVERY,1,0,1,1}; assert(slvm9_filesystem_validate(&f)==SLVM9_OK); assert(slvm9_filesystem_can(&f,SLVM9_FS_RECOVERY)==SLVM9_OK); assert(slvm9_filesystem_generation_valid(&f,7)==SLVM9_OK);
 slvm9_adapter_t a={p,"tac3",1,1,1}; assert(slvm9_adapter_validate(&a)==SLVM9_OK);
 slvm9_mount_t m={1,1,7,"/data","default",1,1,0}; assert(slvm9_mount_validate(&m)==SLVM9_OK); assert(slvm9_mount_is_current(&m,1,7)==SLVM9_OK);
 slvm9_file_t file={11,2,7,100,"example",0,1,1,1}; assert(slvm9_file_validate(&file)==SLVM9_OK); assert(slvm9_file_can_write(&file)==SLVM9_OK);
 slvm9_fsop_t op={SLVM9_FSOP_WRITE,11,7,100,0,1,0,0}; assert(slvm9_fsop_validate(&op)==SLVM9_OK);
 slvm9_context_identity_t c={11,22,2,1,0,"example","tac3","/data",1}; assert(slvm9_context_validate(&c)==SLVM9_OK);
 slvm9_supervisor_t s={SLVM9_NORMAL,1,7,0,1,1,1,1,1}; assert(slvm9_supervisor_admit(&s)==SLVM9_OK); assert(slvm9_supervisor_start(&s)==SLVM9_OK); assert(slvm9_supervisor_fault(&s,1)==SLVM9_INTEGRITY); assert(slvm9_supervisor_stop(&s)==SLVM9_OK);
 puts("SLVM/9 self-test: PASS"); return 0; }
