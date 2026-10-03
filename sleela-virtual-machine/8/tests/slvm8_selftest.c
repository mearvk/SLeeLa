#include "slvm8.h"
#include "slvm8_policy.h"
#include "slvm8_transaction.h"
#include "slvm8_supervisor.h"
#include "slvm8_audit.h"
#include "slvm8_admission.h"
#include <assert.h>
#include <stdio.h>
int main(void){
 slvm8_config_t c; assert(slvm8_config_defaults(&c)==SLVM8_OK); assert(slvm8_config_validate(&c)==SLVM8_OK);
 slvm8_policy_t p={1,1024,100,100,100,2,1,1,"policy",1,1}; assert(slvm8_policy_can_admit(&p)==SLVM8_OK);
 slvm8_admission_t a={1,1,1,1,1,1,1,0}; assert(slvm8_admission_evaluate(&a)==SLVM8_OK);
 slvm8_supervisor_t s={SLVM8_NORMAL,1,0,0,0,1,1,1,1,1}; assert(slvm8_supervisor_admit(&s)==SLVM8_OK); assert(slvm8_supervisor_start(&s)==SLVM8_OK); assert(slvm8_supervisor_checkpoint(&s)==SLVM8_OK);
 slvm8_transaction_t t; assert(slvm8_transaction_begin(&t,1,1)==SLVM8_OK); assert(slvm8_transaction_record(&t)==SLVM8_OK); assert(slvm8_transaction_commit(&t)==SLVM8_OK);
 slvm8_audit_record_t r={1,1,2,0,0,1}; r.record_hash=slvm8_audit_hash(r.sequence,r.epoch,r.event,r.previous_hash); assert(slvm8_audit_validate(&r)==SLVM8_OK);
 assert(slvm8_supervisor_fault(&s,1)==SLVM8_QUARANTINED); assert(slvm8_supervisor_stop(&s)==SLVM8_OK);
 puts("SLVM/8 self-test: PASS"); return 0;
}
