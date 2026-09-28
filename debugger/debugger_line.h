#ifndef SLEELA_DEBUGGER_LINE_H
#define SLEELA_DEBUGGER_LINE_H

#include <stdint.h>
#include "debugger_c.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum sleela_debugger_line_action {
    SLEELA_DEBUGGER_LINE_TRACE = 0,
    SLEELA_DEBUGGER_LINE_STOP = 1,
    SLEELA_DEBUGGER_LINE_EXCEPTION = 2
} sleela_debugger_line_action_t;

typedef struct sleela_debugger_line_point {
    const char *file;
    uint32_t line;
    uint32_t column;
    sleela_debugger_line_action_t action;
} sleela_debugger_line_point_t;

/* Register a source-line control point. A line may be registered more than once. */
/* Returns zero when the point is invalid or cannot be registered. */
uint64_t sleela_debugger_add_line_point(
    sleela_debugger_session_t *session,
    const sleela_debugger_line_point_t *point);

/* Called by compiler instrumentation or a manually instrumented source line. */
sleela_debugger_result_t sleela_debugger_hit_line(
    sleela_debugger_session_t *session,
    const sleela_debugger_line_point_t *point,
    const char *thread,
    const char *function);

/*
 * Voice-control parser for safe debugger commands. It accepts commands such as:
 *   "continue"
 *   "step"
 *   "next"
 *   "break file.c 42"
 *   "stop here"
 *   "trace line"
 * It only changes debugger/session state; it never executes target-program input.
 */
sleela_debugger_result_t sleela_debugger_voice_command(
    sleela_debugger_session_t *session,
    const char *command,
    char *response,
    uint32_t response_size);

#ifdef __cplusplus
}
#endif

#endif
