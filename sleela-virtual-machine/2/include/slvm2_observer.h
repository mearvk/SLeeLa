#ifndef SLEELA_SLVM2_OBSERVER_H
#define SLEELA_SLVM2_OBSERVER_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SLVM2_OBS_VM=1, SLVM2_OBS_SECURITY, SLVM2_OBS_CRYPTO, SLVM2_OBS_LINK, SLVM2_OBS_CERTIFICATE, SLVM2_OBS_RUNTIME } slvm2_observer_event_t;
typedef struct { uint64_t sequence; uint64_t timestamp_ns; slvm2_observer_event_t type; const char *subject; const void *data; size_t data_size; int secret; } slvm2_observer_record_t;
typedef int (*slvm2_observer_fn)(const slvm2_observer_record_t*, void*);
#ifdef __cplusplus
}
#endif
#endif
