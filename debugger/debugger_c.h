#ifndef SLEELA_DEBUGGER_C_H
#define SLEELA_DEBUGGER_C_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_DEBUGGER_C_ABI_VERSION 1u

typedef struct sleela_debugger_session sleela_debugger_session_t;

typedef enum sleela_debugger_result {
    SLEELA_DEBUGGER_OK = 0,
    SLEELA_DEBUGGER_INVALID_ARGUMENT = 1,
    SLEELA_DEBUGGER_NOT_FOUND = 2,
    SLEELA_DEBUGGER_IO_ERROR = 3
} sleela_debugger_result_t;

typedef enum sleela_debugger_event_type {
    SLEELA_DEBUGGER_EVENT_INFO = 0,
    SLEELA_DEBUGGER_EVENT_BREAKPOINT,
    SLEELA_DEBUGGER_EVENT_WATCHPOINT,
    SLEELA_DEBUGGER_EVENT_EXCEPTION,
    SLEELA_DEBUGGER_EVENT_ASSERTION,
    SLEELA_DEBUGGER_EVENT_THREAD,
    SLEELA_DEBUGGER_EVENT_LOG,
    SLEELA_DEBUGGER_EVENT_COVERAGE,
    SLEELA_DEBUGGER_EVENT_SANITIZER,
    SLEELA_DEBUGGER_EVENT_TEST,
    SLEELA_DEBUGGER_EVENT_REGRESSION,
    SLEELA_DEBUGGER_EVENT_CRASH
} sleela_debugger_event_type_t;

typedef struct sleela_debugger_location {
    const char *file;
    uint32_t line;
    uint32_t column;
} sleela_debugger_location_t;

typedef struct sleela_debugger_event {
    sleela_debugger_event_type_t type;
    const char *severity;
    const char *message;
    const char *thread;
    sleela_debugger_location_t location;
} sleela_debugger_event_t;

sleela_debugger_session_t *sleela_debugger_create(void);
void sleela_debugger_destroy(sleela_debugger_session_t *session);

uint64_t sleela_debugger_add_breakpoint(
    sleela_debugger_session_t *session,
    const char *function,
    const sleela_debugger_location_t *location);

uint64_t sleela_debugger_add_watchpoint(
    sleela_debugger_session_t *session,
    const char *expression);

sleela_debugger_result_t sleela_debugger_remove_breakpoint(
    sleela_debugger_session_t *session, uint64_t id);

sleela_debugger_result_t sleela_debugger_remove_watchpoint(
    sleela_debugger_session_t *session, uint64_t id);

sleela_debugger_result_t sleela_debugger_emit(
    sleela_debugger_session_t *session,
    const sleela_debugger_event_t *event);

size_t sleela_debugger_event_count(
    const sleela_debugger_session_t *session);

sleela_debugger_result_t sleela_debugger_write_report(
    const sleela_debugger_session_t *session,
    const char *path);

const char *sleela_debugger_result_string(
    sleela_debugger_result_t result);

#ifdef __cplusplus
}
#endif

#endif
