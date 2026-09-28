#include "debugger_line.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct line_point_copy {
    uint64_t id;
    char *file;
    uint32_t line;
    uint32_t column;
    sleela_debugger_line_action_t action;
} line_point_copy_t;

typedef struct line_registry {
    sleela_debugger_session_t *session;
    line_point_copy_t *points;
    size_t count;
    size_t capacity;
} line_registry_t;

/*
 * The registry is intentionally process-local and session-scoped through a
 * private side table. It keeps line instrumentation separate from native
 * ptrace/LLDB/Windows debugger backends.
 */
static line_registry_t g_registry;

static char *dup_s(const char *s) {
    size_t n;
    char *p;
    if (!s) return NULL;
    n = strlen(s) + 1u;
    p = (char *)malloc(n);
    if (!p) return NULL;
    memcpy(p, s, n);
    return p;
}

static int grow_points(void) {
    size_t next;
    line_point_copy_t *p;
    if (g_registry.count < g_registry.capacity) return 1;
    next = g_registry.capacity ? g_registry.capacity * 2u : 64u;
    p = (line_point_copy_t *)realloc(g_registry.points, next * sizeof(*p));
    if (!p) return 0;
    g_registry.points = p;
    g_registry.capacity = next;
    return 1;
}

uint64_t sleela_debugger_add_line_point(
    sleela_debugger_session_t *session,
    const sleela_debugger_line_point_t *point) {
    line_point_copy_t *p;
    if (!session || !point || !point->file || point->line == 0u) return 0u;
    if (g_registry.session != session) {
        free(g_registry.points);
        memset(&g_registry, 0, sizeof(g_registry));
        g_registry.session = session;
    }
    if (!grow_points()) return 0u;
    p = &g_registry.points[g_registry.count++];
    memset(p, 0, sizeof(*p));
    p->file = dup_s(point->file);
    if (!p->file) { --g_registry.count; return 0u; }
    p->line = point->line;
    p->column = point->column;
    p->action = point->action;
    p->id = (uint64_t)g_registry.count;
    return p->id;
}

sleela_debugger_result_t sleela_debugger_hit_line(
    sleela_debugger_session_t *session,
    const sleela_debugger_line_point_t *point,
    const char *thread,
    const char *function) {
    sleela_debugger_event_t event;
    const char *message;
    if (!session || !point || !point->file || point->line == 0u)
        return SLEELA_DEBUGGER_INVALID_ARGUMENT;

    memset(&event, 0, sizeof(event));
    event.type = (point->action == SLEELA_DEBUGGER_LINE_EXCEPTION)
        ? SLEELA_DEBUGGER_EVENT_EXCEPTION
        : (point->action == SLEELA_DEBUGGER_LINE_STOP
            ? SLEELA_DEBUGGER_EVENT_BREAKPOINT
            : SLEELA_DEBUGGER_EVENT_INFO);
    event.severity = (point->action == SLEELA_DEBUGGER_LINE_EXCEPTION) ? "exception"
                    : (point->action == SLEELA_DEBUGGER_LINE_STOP) ? "stop"
                    : "trace";
    message = function ? function : "source-line";
    event.message = message;
    event.thread = thread ? thread : "";
    event.location.file = point->file;
    event.location.line = point->line;
    event.location.column = point->column;
    return sleela_debugger_emit(session, &event);
}

sleela_debugger_result_t sleela_debugger_voice_command(
    sleela_debugger_session_t *session,
    const char *command,
    char *response,
    uint32_t response_size) {
    if (!session || !command || !response || response_size == 0u)
        return SLEELA_DEBUGGER_INVALID_ARGUMENT;

    if (strcmp(command, "continue") == 0 || strcmp(command, "resume") == 0) {
        snprintf(response, response_size, "continue");
        return SLEELA_DEBUGGER_OK;
    }
    if (strcmp(command, "step") == 0 || strcmp(command, "step into") == 0) {
        snprintf(response, response_size, "step");
        return SLEELA_DEBUGGER_OK;
    }
    if (strcmp(command, "next") == 0 || strcmp(command, "step over") == 0) {
        snprintf(response, response_size, "next");
        return SLEELA_DEBUGGER_OK;
    }
    if (strcmp(command, "stop here") == 0 ||
        strcmp(command, "break here") == 0) {
        snprintf(response, response_size, "stop-here");
        return SLEELA_DEBUGGER_OK;
    }
    if (strcmp(command, "trace line") == 0 ||
        strcmp(command, "trace lines") == 0) {
        snprintf(response, response_size, "trace-line");
        return SLEELA_DEBUGGER_OK;
    }

    snprintf(response, response_size, "unknown-command");
    return SLEELA_DEBUGGER_NOT_FOUND;
}
