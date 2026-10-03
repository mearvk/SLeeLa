#ifndef SLVM8_AUDIT_H
#define SLVM8_AUDIT_H
#include "slvm8.h"
typedef struct { uint64_t sequence,epoch,event,previous_hash,record_hash; uint8_t security_relevant; } slvm8_audit_record_t;
int slvm8_audit_validate(const slvm8_audit_record_t *r);
uint64_t slvm8_audit_hash(uint64_t sequence,uint64_t epoch,uint64_t event,uint64_t previous_hash);
#endif
