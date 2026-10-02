#ifndef SLEELA_SLVM3_OBSERVER_H
#define SLEELA_SLVM3_OBSERVER_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
 SLVM3_OBS_VM=1, SLVM3_OBS_SECURITY, SLVM3_OBS_CRYPTO,
 SLVM3_OBS_LINK, SLVM3_OBS_CERTIFICATE, SLVM3_OBS_RUNTIME,
 SLVM3_OBS_ISOLATION, SLVM3_OBS_ATTESTATION
} slvm3_observer_event_t;

typedef struct {
 uint64_t sequence;
 uint64_t timestamp_ns;
 const char *previous_record_hash;
 slvm3_observer_event_t type;
 const char *subject;
 const void *data;
 size_t data_size;
 int secret;
} slvm3_observer_record_t;

typedef int (*slvm3_observer_fn)(const slvm3_observer_record_t*, void*);
#ifdef __cplusplus
}
#endif
#endif
