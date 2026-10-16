#ifndef SLVM8_TRANSACTION_H
#define SLVM8_TRANSACTION_H
#include "slvm8.h"
typedef enum { SLVM8_TXN_NEW=0,SLVM8_TXN_ACTIVE=1,SLVM8_TXN_COMMITTED=2,SLVM8_TXN_ABORTED=3 } slvm8_txn_state_t;
typedef struct { uint64_t id,execution_epoch,action_count,committed_actions; slvm8_txn_state_t state; uint8_t integrity_valid,admission_valid; } slvm8_transaction_t;
int slvm8_transaction_begin(slvm8_transaction_t *t,uint64_t id,uint64_t epoch);
int slvm8_transaction_record(slvm8_transaction_t *t);
int slvm8_transaction_commit(slvm8_transaction_t *t);
int slvm8_transaction_abort(slvm8_transaction_t *t);
#endif
