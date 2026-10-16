#ifndef SLEELA_SLVM5_TRANSACTION_H
#define SLEELA_SLVM5_TRANSACTION_H
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
 uint64_t transaction_id;
 uint64_t parent_id;
 uint64_t sequence;
 uint64_t started_at_ns;
 uint64_t deadline_ns;
 uint32_t state;
 uint32_t flags;
} slvm5_transaction_t;
enum { SLVM5_TX_OPEN=1, SLVM5_TX_COMMITTING=2, SLVM5_TX_COMMITTED=3, SLVM5_TX_ABORTING=4, SLVM5_TX_ABORTED=5 };
int slvm5_transaction_validate(const slvm5_transaction_t*, uint64_t now_ns);
#ifdef __cplusplus
}
#endif
#endif
