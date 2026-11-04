#ifndef SLEELA_OS_SUPPORT_H
#define SLEELA_OS_SUPPORT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLOSH_MAX_HOOKS 32u
#define SLOSH_MAX_TASKS 128u

typedef struct slosh_runtime slosh_runtime;
typedef uint64_t slosh_token;

typedef enum slosh_status {
    SLOSH_OK = 0,
    SLOSH_INVALID_ARGUMENT = 1,
    SLOSH_OUT_OF_MEMORY = 2,
    SLOSH_CAPACITY = 3,
    SLOSH_NOT_FOUND = 4,
    SLOSH_BUSY = 5,
    SLOSH_CALLBACK_FAILED = 6
} slosh_status;

typedef enum slosh_event_kind {
    SLOSH_EVENT_USER = 1,
    SLOSH_EVENT_TASK_QUEUED = 2,
    SLOSH_EVENT_TASK_FINISHED = 3
} slosh_event_kind;

typedef struct slosh_event {
    slosh_event_kind kind;
    uint64_t sequence;
    const void *payload; /* borrowed for the callback duration only */
    size_t payload_size;
} slosh_event;

typedef slosh_status (*slosh_event_fn)(slosh_runtime *, const slosh_event *, void *);
typedef slosh_status (*slosh_task_fn)(slosh_runtime *, void *);

/* Allocates a runtime. One runtime is single-threaded; callers must serialize
 * access and must not destroy it from within its callback. */
slosh_status slosh_runtime_create(slosh_runtime **out_runtime);
void slosh_runtime_destroy(slosh_runtime *runtime);

/* Runtime-local callbacks only. No OS/system-call interception. */
slosh_status slosh_subscribe(slosh_runtime *runtime, slosh_event_kind kind,
                             slosh_event_fn callback, void *user_data,
                             slosh_token *out_token);
slosh_status slosh_unsubscribe(slosh_runtime *runtime, slosh_token token);
slosh_status slosh_emit(slosh_runtime *runtime, slosh_event_kind kind,
                        const void *payload, size_t payload_size);

/* Cooperative queue: post never runs inline; run_one runs at most one task. */
slosh_status slosh_post(slosh_runtime *runtime, slosh_task_fn callback,
                        void *user_data);
slosh_status slosh_run_one(slosh_runtime *runtime, int *out_ran);
size_t slosh_pending(const slosh_runtime *runtime);

#ifdef __cplusplus
}
#endif
#endif
