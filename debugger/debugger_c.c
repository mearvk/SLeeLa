#include "debugger_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sleela_debugger_breakpoint {
    uint64_t id;
    char *function;
    sleela_debugger_location_t location;
} sleela_debugger_breakpoint_t;

typedef struct sleela_debugger_watchpoint {
    uint64_t id;
    char *expression;
} sleela_debugger_watchpoint_t;

typedef struct sleela_debugger_event_copy {
    sleela_debugger_event_type_t type;
    char *severity;
    char *message;
    char *thread;
    sleela_debugger_location_t location;
    char *file;
} sleela_debugger_event_copy_t;

struct sleela_debugger_session {
    uint64_t next_id;
    sleela_debugger_breakpoint_t *breakpoints;
    size_t breakpoint_count;
    size_t breakpoint_capacity;
    sleela_debugger_watchpoint_t *watchpoints;
    size_t watchpoint_count;
    size_t watchpoint_capacity;
    sleela_debugger_event_copy_t *events;
    size_t event_count;
    size_t event_capacity;
};

static char *dup_string(const char *value) {
    size_t n;
    char *copy;
    if (!value) return NULL;
    n = strlen(value) + 1u;
    copy = (char *)malloc(n);
    if (!copy) return NULL;
    memcpy(copy, value, n);
    return copy;
}

static int grow(void **ptr, size_t *capacity, size_t count, size_t element_size) {
    size_t next;
    void *p;
    if (count < *capacity) return 1;
    next = (*capacity == 0u) ? 8u : (*capacity * 2u);
    if (next < count + 1u) next = count + 1u;
    p = realloc(*ptr, next * element_size);
    if (!p) return 0;
    *ptr = p;
    *capacity = next;
    return 1;
}

static void free_event(sleela_debugger_event_copy_t *event) {
    if (!event) return;
    free(event->severity);
    free(event->message);
    free(event->thread);
    free(event->file);
    memset(event, 0, sizeof(*event));
}

static const char *event_name(sleela_debugger_event_type_t type) {
    switch (type) {
        case SLEELA_DEBUGGER_EVENT_INFO: return "info";
        case SLEELA_DEBUGGER_EVENT_BREAKPOINT: return "breakpoint";
        case SLEELA_DEBUGGER_EVENT_WATCHPOINT: return "watchpoint";
        case SLEELA_DEBUGGER_EVENT_EXCEPTION: return "exception";
        case SLEELA_DEBUGGER_EVENT_ASSERTION: return "assertion";
        case SLEELA_DEBUGGER_EVENT_THREAD: return "thread";
        case SLEELA_DEBUGGER_EVENT_LOG: return "log";
        case SLEELA_DEBUGGER_EVENT_COVERAGE: return "coverage";
        case SLEELA_DEBUGGER_EVENT_SANITIZER: return "sanitizer";
        case SLEELA_DEBUGGER_EVENT_TEST: return "test";
        case SLEELA_DEBUGGER_EVENT_REGRESSION: return "regression";
        case SLEELA_DEBUGGER_EVENT_CRASH: return "crash";
    }
    return "unknown";
}

sleela_debugger_session_t *sleela_debugger_create(void) {
    sleela_debugger_session_t *s =
        (sleela_debugger_session_t *)calloc(1u, sizeof(*s));
    if (s) s->next_id = 1u;
    return s;
}

void sleela_debugger_destroy(sleela_debugger_session_t *s) {
    size_t i;
    if (!s) return;
    for (i = 0; i < s->breakpoint_count; ++i) free(s->breakpoints[i].function);
    for (i = 0; i < s->watchpoint_count; ++i) free(s->watchpoints[i].expression);
    for (i = 0; i < s->event_count; ++i) free_event(&s->events[i]);
    free(s->breakpoints);
    free(s->watchpoints);
    free(s->events);
    free(s);
}

uint64_t sleela_debugger_add_breakpoint(
    sleela_debugger_session_t *s, const char *function,
    const sleela_debugger_location_t *location) {
    sleela_debugger_breakpoint_t *b;
    if (!s || !function) return 0u;
    if (!grow((void **)&s->breakpoints, &s->breakpoint_capacity,
              s->breakpoint_count, sizeof(*s->breakpoints))) return 0u;
    b = &s->breakpoints[s->breakpoint_count++];
    memset(b, 0, sizeof(*b));
    b->function = dup_string(function);
    if (!b->function) {
        --s->breakpoint_count;
        return 0u;
    }
    if (location) b->location = *location;
    b->id = s->next_id++;
    return b->id;
}

uint64_t sleela_debugger_add_watchpoint(
    sleela_debugger_session_t *s, const char *expression) {
    sleela_debugger_watchpoint_t *w;
    if (!s || !expression) return 0u;
    if (!grow((void **)&s->watchpoints, &s->watchpoint_capacity,
              s->watchpoint_count, sizeof(*s->watchpoints))) return 0u;
    w = &s->watchpoints[s->watchpoint_count++];
    memset(w, 0, sizeof(*w));
    w->expression = dup_string(expression);
    if (!w->expression) {
        --s->watchpoint_count;
        return 0u;
    }
    w->id = s->next_id++;
    return w->id;
}

sleela_debugger_result_t sleela_debugger_remove_breakpoint(
    sleela_debugger_session_t *s, uint64_t id) {
    size_t i;
    if (!s || id == 0u) return SLEELA_DEBUGGER_INVALID_ARGUMENT;
    for (i = 0; i < s->breakpoint_count; ++i) {
        if (s->breakpoints[i].id == id) {
            free(s->breakpoints[i].function);
            s->breakpoints[i] = s->breakpoints[--s->breakpoint_count];
            return SLEELA_DEBUGGER_OK;
        }
    }
    return SLEELA_DEBUGGER_NOT_FOUND;
}

sleela_debugger_result_t sleela_debugger_remove_watchpoint(
    sleela_debugger_session_t *s, uint64_t id) {
    size_t i;
    if (!s || id == 0u) return SLEELA_DEBUGGER_INVALID_ARGUMENT;
    for (i = 0; i < s->watchpoint_count; ++i) {
        if (s->watchpoints[i].id == id) {
            free(s->watchpoints[i].expression);
            s->watchpoints[i] = s->watchpoints[--s->watchpoint_count];
            return SLEELA_DEBUGGER_OK;
        }
    }
    return SLEELA_DEBUGGER_NOT_FOUND;
}

sleela_debugger_result_t sleela_debugger_emit(
    sleela_debugger_session_t *s, const sleela_debugger_event_t *event) {
    sleela_debugger_event_copy_t *dst;
    if (!s || !event) return SLEELA_DEBUGGER_INVALID_ARGUMENT;
    if (!grow((void **)&s->events, &s->event_capacity,
              s->event_count, sizeof(*s->events)))
        return SLEELA_DEBUGGER_IO_ERROR;

    dst = &s->events[s->event_count++];
    memset(dst, 0, sizeof(*dst));
    dst->type = event->type;
    dst->severity = dup_string(event->severity ? event->severity : "info");
    dst->message = dup_string(event->message ? event->message : "");
    dst->thread = dup_string(event->thread ? event->thread : "");
    dst->location.line = event->location.line;
    dst->location.column = event->location.column;
    dst->file = dup_string(event->location.file ? event->location.file : "");
    dst->location.file = dst->file;

    if (!dst->severity || !dst->message || !dst->thread || !dst->file) {
        free_event(dst);
        --s->event_count;
        return SLEELA_DEBUGGER_IO_ERROR;
    }
    return SLEELA_DEBUGGER_OK;
}

size_t sleela_debugger_event_count(const sleela_debugger_session_t *s) {
    return s ? s->event_count : 0u;
}

sleela_debugger_result_t sleela_debugger_write_report(
    const sleela_debugger_session_t *s, const char *path) {
    FILE *out;
    size_t i;
    if (!s || !path) return SLEELA_DEBUGGER_INVALID_ARGUMENT;
    out = fopen(path, "w");
    if (!out) return SLEELA_DEBUGGER_IO_ERROR;
    fprintf(out, "SLeeLa Debugger C Diagnostic Report\n");
    fprintf(out, "abi=%u events=%zu\n",
            (unsigned)SLEELA_DEBUGGER_C_ABI_VERSION, s->event_count);
    for (i = 0; i < s->event_count; ++i) {
        const sleela_debugger_event_copy_t *e = &s->events[i];
        fprintf(out, "[%s] %s: %s", e->severity, event_name(e->type), e->message);
        if (e->thread && e->thread[0]) fprintf(out, " thread=%s", e->thread);
        if (e->file && e->file[0])
            fprintf(out, " %s:%u:%u", e->file,
                    (unsigned)e->location.line, (unsigned)e->location.column);
        fputc('\n', out);
    }
    if (fclose(out) != 0) return SLEELA_DEBUGGER_IO_ERROR;
    return SLEELA_DEBUGGER_OK;
}

const char *sleela_debugger_result_string(sleela_debugger_result_t result) {
    switch (result) {
        case SLEELA_DEBUGGER_OK: return "ok";
        case SLEELA_DEBUGGER_INVALID_ARGUMENT: return "invalid-argument";
        case SLEELA_DEBUGGER_NOT_FOUND: return "not-found";
        case SLEELA_DEBUGGER_IO_ERROR: return "io-error";
    }
    return "unknown";
}
