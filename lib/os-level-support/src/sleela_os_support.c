#include "sleela_os_support.h"

#include <stdlib.h>
#include <string.h>

typedef struct hook_slot {
    slosh_token token;
    slosh_event_kind kind;
    slosh_event_fn callback;
    void *user_data;
    int active;
} hook_slot;

typedef struct task_slot {
    slosh_task_fn callback;
    void *user_data;
} task_slot;

struct slosh_runtime {
    hook_slot hooks[SLOSH_MAX_HOOKS];
    task_slot tasks[SLOSH_MAX_TASKS];
    size_t task_head;
    size_t task_count;
    uint64_t next_token;
    uint64_t event_sequence;
    unsigned dispatch_depth;
    int task_running;
};

slosh_status slosh_runtime_create(slosh_runtime **out) {
    slosh_runtime *r;
    if (!out) return SLOSH_INVALID_ARGUMENT;
    *out = NULL;
    r = (slosh_runtime *)calloc(1, sizeof(*r));
    if (!r) return SLOSH_OUT_OF_MEMORY;
    r->next_token = 1;
    *out = r;
    return SLOSH_OK;
}

void slosh_runtime_destroy(slosh_runtime *r) {
    if (!r) return;
    memset(r, 0, sizeof(*r));
    free(r);
}

slosh_status slosh_subscribe(slosh_runtime *r, slosh_event_kind kind,
    slosh_event_fn callback, void *user, slosh_token *out_token) {
    size_t i;
    if (!r || !callback || !out_token || kind < SLOSH_EVENT_USER || kind > SLOSH_EVENT_TASK_FINISHED)
        return SLOSH_INVALID_ARGUMENT;
    if (r->dispatch_depth) return SLOSH_BUSY;
    for (i = 0; i < SLOSH_MAX_HOOKS; ++i) {
        if (!r->hooks[i].active) {
            slosh_token t = r->next_token++;
            if (!t) t = r->next_token++;
            r->hooks[i].token = t;
            r->hooks[i].kind = kind;
            r->hooks[i].callback = callback;
            r->hooks[i].user_data = user;
            r->hooks[i].active = 1;
            *out_token = t;
            return SLOSH_OK;
        }
    }
    return SLOSH_CAPACITY;
}

slosh_status slosh_unsubscribe(slosh_runtime *r, slosh_token token) {
    size_t i;
    if (!r || !token) return SLOSH_INVALID_ARGUMENT;
    if (r->dispatch_depth) return SLOSH_BUSY;
    for (i = 0; i < SLOSH_MAX_HOOKS; ++i) {
        if (r->hooks[i].active && r->hooks[i].token == token) {
            memset(&r->hooks[i], 0, sizeof(r->hooks[i]));
            return SLOSH_OK;
        }
    }
    return SLOSH_NOT_FOUND;
}

static slosh_status emit_internal(slosh_runtime *r, slosh_event_kind kind,
                                  const void *payload, size_t payload_size) {
    slosh_event e;
    size_t i;
    slosh_status result = SLOSH_OK;
    e.kind = kind;
    e.sequence = ++r->event_sequence;
    e.payload = payload;
    e.payload_size = payload_size;
    ++r->dispatch_depth;
    for (i = 0; i < SLOSH_MAX_HOOKS; ++i) {
        hook_slot *h = &r->hooks[i];
        if (h->active && h->kind == kind) {
            result = h->callback(r, &e, h->user_data);
            if (result != SLOSH_OK) break;
        }
    }
    --r->dispatch_depth;
    return result;
}

slosh_status slosh_emit(slosh_runtime *r, slosh_event_kind kind,
    const void *payload, size_t payload_size) {
    if (!r || kind < SLOSH_EVENT_USER || kind > SLOSH_EVENT_TASK_FINISHED ||
        (!payload && payload_size)) return SLOSH_INVALID_ARGUMENT;
    return emit_internal(r, kind, payload, payload_size);
}

slosh_status slosh_post(slosh_runtime *r, slosh_task_fn callback, void *user) {
    size_t tail;
    if (!r || !callback) return SLOSH_INVALID_ARGUMENT;
    if (r->task_count == SLOSH_MAX_TASKS) return SLOSH_CAPACITY;
    tail = (r->task_head + r->task_count) % SLOSH_MAX_TASKS;
    r->tasks[tail].callback = callback;
    r->tasks[tail].user_data = user;
    ++r->task_count;
    return emit_internal(r, SLOSH_EVENT_TASK_QUEUED, NULL, 0);
}

slosh_status slosh_run_one(slosh_runtime *r, int *out_ran) {
    task_slot task;
    slosh_status result;
    if (!r || !out_ran) return SLOSH_INVALID_ARGUMENT;
    *out_ran = 0;
    if (r->task_running || r->dispatch_depth) return SLOSH_BUSY;
    if (!r->task_count) return SLOSH_OK;
    task = r->tasks[r->task_head];
    memset(&r->tasks[r->task_head], 0, sizeof(r->tasks[0]));
    r->task_head = (r->task_head + 1) % SLOSH_MAX_TASKS;
    --r->task_count;
    r->task_running = 1;
    *out_ran = 1;
    result = task.callback(r, task.user_data);
    r->task_running = 0;
    if (result != SLOSH_OK) return result;
    return emit_internal(r, SLOSH_EVENT_TASK_FINISHED, NULL, 0);
}

size_t slosh_pending(const slosh_runtime *r) { return r ? r->task_count : 0; }
