#ifndef OSLS_RUNTIME_H
#define OSLS_RUNTIME_H

/* In-process extension hooks and cooperative callback queue. This API does not
 * intercept OS/system calls, create threads/processes, perform I/O, or change
 * platform policy. The caller owns callback argument lifetimes. */
#include <stddef.h>
#include <stdint.h>
#include "../osls.h"
#include "../osls.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct osls_runtime osls_runtime;
typedef uint64_t osls_runtime_token;

typedef enum osls_runtime_status {
    OSLS_RUNTIME_OK = 0,
    OSLS_RUNTIME_INVALID_ARGUMENT = 1,
    OSLS_RUNTIME_OUT_OF_MEMORY = 2,
    OSLS_RUNTIME_CAPACITY = 3,
    OSLS_RUNTIME_NOT_FOUND = 4,
    OSLS_RUNTIME_BUSY = 5,
    OSLS_RUNTIME_CALLBACK_ERROR = 6
} osls_runtime_status;

typedef enum osls_event_kind {
    OSLS_EVENT_USER = 1,
    OSLS_EVENT_TIMER_TICK = 2,
    OSLS_EVENT_IO_READY = 3
} osls_event_kind;

typedef struct osls_event {
    osls_event_kind kind;
    uint64_t sequence;
    const void *payload; /* borrowed for the duration of dispatch only */
    size_t payload_size;
} osls_event;

typedef osls_runtime_status (*osls_event_callback)(
    osls_runtime *runtime, const osls_event *event, void *user_data);

typedef osls_runtime_status (*osls_task_callback)(
    osls_runtime *runtime, void *user_data);

#define OSLS_RUNTIME_MAX_HOOKS 64u
#define OSLS_RUNTIME_MAX_TASKS 128u

OSLS_API osls_runtime_status osls_runtime_create(osls_runtime **out_runtime);
OSLS_API void osls_runtime_destroy(osls_runtime *runtime);

/* Hooks are callbacks within this runtime only; they are not OS syscall hooks. */
OSLS_API osls_runtime_status osls_runtime_subscribe(
    osls_runtime *runtime, osls_event_kind kind, osls_event_callback callback,
    void *user_data, osls_runtime_token *out_token);
OSLS_API osls_runtime_status osls_runtime_unsubscribe(
    osls_runtime *runtime, osls_runtime_token token);
OSLS_API osls_runtime_status osls_runtime_emit(
    osls_runtime *runtime, osls_event_kind kind,
    const void *payload, size_t payload_size);

/* Cooperative queue: tasks execute only when the caller pumps the queue.
 * post() never runs the callback inline. run_one() executes at most one task. */
OSLS_API osls_runtime_status osls_runtime_post(
    osls_runtime *runtime, osls_task_callback callback, void *user_data);
OSLS_API osls_runtime_status osls_runtime_run_one(
    osls_runtime *runtime, int *out_ran);
OSLS_API size_t osls_runtime_pending(const osls_runtime *runtime);

#ifdef __cplusplus
}
#endif
#endif
