#ifndef SLEELA_SLVM4_OBSERVER_H
#define SLEELA_SLVM4_OBSERVER_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
 SLVM4_OBS_VM=1, SLVM4_OBS_SECURITY, SLVM4_OBS_CRYPTO, SLVM4_OBS_LINK,
 SLVM4_OBS_CERTIFICATE, SLVM4_OBS_RUNTIME, SLVM4_OBS_ISOLATION,
 SLVM4_OBS_ATTESTATION, SLVM4_OBS_BROKER, SLVM4_OBS_OBJECT,
 SLVM4_OBS_RESOLVER
} slvm4_observer_event_t;

typedef struct {
 uint64_t sequence;
 uint64_t timestamp_ns;
 uint64_t correlation_id;
 const char *previous_record_hash;
 slvm4_observer_event_t type;
 const char *subject;
 const void *data;
 size_t data_size;
 int secret;
} slvm4_observer_record_t;

typedef int (*slvm4_observer_fn)(const slvm4_observer_record_t*, void*);
#ifdef __cplusplus
}
#endif
#endif
