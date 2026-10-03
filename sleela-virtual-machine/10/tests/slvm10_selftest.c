#include "slvm10.h"
#include "slvm10_storage.h"
#include "slvm10_operation.h"
#include "slvm10_identity.h"
#include "slvm10_adapter.h"
#include <assert.h>
#include <stdio.h>
int main(void){slvm10_state_t s={SLVM10_LINUX,"Linux",0,1,1,1,1,SLVM10_NEW};assert(slvm10_admit(&s)==SLVM10_OK);assert(slvm10_start(&s)==SLVM10_OK);slvm10_storage_t st={1,9,100000,50000,4096,"native-linux","/",SLVM10_READ|SLVM10_WRITE|SLVM10_SYNC|SLVM10_RECOVERY|SLVM10_NATIVE,1,0,1,1};assert(slvm10_storage_validate(&st)==SLVM10_OK);slvm10_identity_t i={1,9,42,7,1,1,"native-linux","example","/",1};assert(slvm10_identity_validate(&i)==SLVM10_OK);slvm10_operation_t o={SLVM10_REVALIDATE_OP,42,9,0,SLVM10_READ,1,1,0,0};assert(slvm10_operation_validate(&o)==SLVM10_OK);slvm10_adapter_t a={SLVM10_LINUX,"Linux native","native-linux",1,SLVM10_READ|SLVM10_WRITE|SLVM10_NATIVE,1,1};assert(slvm10_adapter_validate(&a)==SLVM10_OK);assert(slvm10_quiesce(&s)==SLVM10_OK);assert(slvm10_recover(&s)==SLVM10_OK);assert(slvm10_quarantine(&s)==SLVM10_OK);assert(slvm10_stop(&s)==SLVM10_OK);puts("SLVM/10 self-test: PASS");return 0;}
