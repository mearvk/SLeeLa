#ifndef SLEELA_SLVM_OBSERVER_H
#define SLEELA_SLVM_OBSERVER_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLVM_OBSERVER_FUNCTION_ENTER=1, SLVM_OBSERVER_PARAMETER=2, SLVM_OBSERVER_FUNCTION_RETURN=3, SLVM_OBSERVER_OBJECT=4, SLVM_OBSERVER_MEMORY=5, SLVM_OBSERVER_IO=6, SLVM_OBSERVER_BROKER=7, SLVM_OBSERVER_CERTIFICATE=8 } slvm_observer_event_t;
typedef struct { slvm_observer_event_t event; uint64_t execution_id; uint64_t function_id; uint32_t parameter_index; uint64_t object_id; int64_t status; const void *value; size_t value_size; uint8_t value_is_secret; uint64_t timestamp_ns; } slvm_observer_record_t;
typedef int (*slvm_observer_hook_fn)(const slvm_observer_record_t *, void *);
typedef struct { slvm_observer_hook_fn hook; void *context; uint8_t enabled; uint8_t certified_only; uint8_t redact_values; } slvm_observer_t;
void slvm_observer_init(slvm_observer_t *);
int slvm_observer_attach(slvm_observer_t *, slvm_observer_hook_fn, void *, int);
void slvm_observer_detach(slvm_observer_t *);
int slvm_observer_emit(slvm_observer_t *, const slvm_observer_record_t *);
#ifdef __cplusplus
}
#endif
#endif