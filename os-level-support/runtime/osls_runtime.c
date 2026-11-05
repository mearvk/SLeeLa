#include "osls_runtime.h"

#include <stdlib.h>
#include <string.h>

typedef struct hook_slot {
    osls_runtime_token token;
    osls_event_kind kind;
    osls_event_callback callback;
    void *user_data;
    int active;
} hook_slot;

typedef struct task_slot {
    osls_task_callback callback;
    void *user_data;
} task_slot;

struct osls_runtime {
    hook_slot hooks[OSLS_RUNTIME_MAX_HOOKS];
    task_slot tasks[OSLS_RUNTIME_MAX_TASKS];
    size_t task_head;
    size_t task_count;
    uint64_t next_token;
    uint64_t event_sequence;
    unsigned dispatch_depth;
    int running_task;
};

osls_runtime_status osls_runtime_create(osls_runtime **out_runtime) {
    osls_runtime *runtime;
    if (!out_runtime) return OSLS_RUNTIME_INVALID_ARGUMENT;
    *out_runtime = NULL;
    runtime = (osls_runtime *)calloc(1, sizeof(*runtime));
    if (!runtime) return OSLS_RUNTIME_OUT_OF_MEMORY;
    runtime->next_token = 1;
    *out_runtime = runtime;
    return OSLS_RUNTIME_OK;
}

void osls_runtime_destroy(osls_runtime *runtime) {
    if (!runtime) return;
    /* Destroying while a callback is active is a caller contract violation. */
    memset(runtime, 0, sizeof(*runtime));
    free(runtime);
}

osls_runtime_status osls_runtime_subscribe(osls_runtime *runtime,
    osls_event_kind kind, osls_event_callback callback, void *user_data,
    osls_runtime_token *out_token) {
    size_t i;
    if (!runtime || !callback || !out_token || kind < OSLS_EVENT_USER || kind > OSLS_EVENT_IO_READY)
        return OSLS_RUNTIME_INVALID_ARGUMENT;
    if (runtime->dispatch_depth) return OSLS_RUNTIME_BUSY;
    for (i = 0; i < OSLS_RUNTIME_MAX_HOOKS; ++i) {
        if (!runtime->hooks[i].active) {
            osls_runtime_token token = runtime->next_token++;
            if (token == 0) token = runtime->next_token++;
            runtime->hooks[i].token = token;
            runtime->hooks[i].kind = kind;
            runtime->hooks[i].callback = callback;
            runtime->hooks[i].user_data = user_data;
            runtime->hooks[i].active = 1;
            *out_token = token;
            return OSLS_RUNTIME_OK;
        }
    }
    return OSLS_RUNTIME_CAPACITY;
}

osls_runtime_status osls_runtime_unsubscribe(osls_runtime *runtime, osls_runtime_token token) {
    size_t i;
    if (!runtime || token == 0) return OSLS_RUNTIME_INVALID_ARGUMENT;
    if (runtime->dispatch_depth) return OSLS_RUNTIME_BUSY;
    for (i = 0; i < OSLS_RUNTIME_MAX_HOOKS; ++i) {
        if (runtime->hooks[i].active && runtime->hooks[i].token == token) {
            memset(&runtime->hooks[i], 0, sizeof(runtime->hooks[i]));
            return OSLS_RUNTIME_OK;
        }
    }
    return OSLS_RUNTIME_NOT_FOUND;
}

osls_runtime_status osls_runtime_emit(osls_runtime *runtime, osls_event_kind kind,
    const void *payload, size_t payload_size) {
    osls_event event;
    size_t i;
    osls_runtime_status result = OSLS_RUNTIME_OK;
    if (!runtime || kind < OSLS_EVENT_USER || kind > OSLS_EVENT_IO_READY ||
        (!payload && payload_size != 0)) return OSLS_RUNTIME_INVALID_ARGUMENT;
    event.kind = kind;
    event.sequence = ++runtime->event_sequence;
    event.payload = payload;
    event.payload_size = payload_size;
    ++runtime->dispatch_depth;
    for (i = 0; i < OSLS_RUNTIME_MAX_HOOKS; ++i) {
        hook_slot *slot = &runtime->hooks[i];
        if (slot->active && slot->kind == kind) {
            result = slot->callback(runtime, &event, slot->user_data);
            if (result != OSLS_RUNTIME_OK) break;
        }
    }
    --runtime->dispatch_depth;
    return result;
}

osls_runtime_status osls_runtime_post(osls_runtime *runtime,
    osls_task_callback callback, void *user_data) {
    size_t tail;
    if (!runtime || !callback) return OSLS_RUNTIME_INVALID_ARGUMENT;
    if (runtime->task_count == OSLS_RUNTIME_MAX_TASKS) return OSLS_RUNTIME_CAPACITY;
    tail = (runtime->task_head + runtime->task_count) % OSLS_RUNTIME_MAX_TASKS;
    runtime->tasks[tail].callback = callback;
    runtime->tasks[tail].user_data = user_data;
    ++runtime->task_count;
    return OSLS_RUNTIME_OK;
}

osls_runtime_status osls_runtime_run_one(osls_runtime *runtime, int *out_ran) {
    task_slot task;
    osls_runtime_status result;
    if (!runtime || !out_ran) return OSLS_RUNTIME_INVALID_ARGUMENT;
    *out_ran = 0;
    if (runtime->running_task) return OSLS_RUNTIME_BUSY;
    if (!runtime->task_count) return OSLS_RUNTIME_OK;
    task = runtime->tasks[runtime->task_head];
    memset(&runtime->tasks[runtime->task_head], 0, sizeof(runtime->tasks[0]));
    runtime->task_head = (runtime->task_head + 1) % OSLS_RUNTIME_MAX_TASKS;
    --runtime->task_count;
    runtime->running_task = 1;
    *out_ran = 1;
    result = task.callback(runtime, task.user_data);
    runtime->running_task = 0;
    return result;
}

size_t osls_runtime_pending(const osls_runtime *runtime) {
    return runtime ? runtime->task_count : 0;
}
