#ifndef SLEELA_DEBUGGER_C_H
#define SLEELA_DEBUGGER_C_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SLEELA_DEBUGGER_C_ABI_VERSION 2u
typedef struct sleela_debugger_session sleela_debugger_session_t;
typedef enum sleela_debugger_result { SLEELA_DEBUGGER_OK=0, SLEELA_DEBUGGER_INVALID_ARGUMENT=1, SLEELA_DEBUGGER_NOT_FOUND=2, SLEELA_DEBUGGER_IO_ERROR=3 } sleela_debugger_result_t;
typedef enum sleela_debugger_event_type { SLEELA_DEBUGGER_EVENT_INFO=0,SLEELA_DEBUGGER_EVENT_BREAKPOINT,SLEELA_DEBUGGER_EVENT_WATCHPOINT,SLEELA_DEBUGGER_EVENT_EXCEPTION,SLEELA_DEBUGGER_EVENT_ASSERTION,SLEELA_DEBUGGER_EVENT_THREAD,SLEELA_DEBUGGER_EVENT_LOG,SLEELA_DEBUGGER_EVENT_COVERAGE,SLEELA_DEBUGGER_EVENT_SANITIZER,SLEELA_DEBUGGER_EVENT_TEST,SLEELA_DEBUGGER_EVENT_REGRESSION,SLEELA_DEBUGGER_EVENT_CRASH } sleela_debugger_event_type_t;
typedef enum sleela_debugger_stop_reason { SLEELA_DEBUGGER_STOP_NONE=0,SLEELA_DEBUGGER_STOP_BREAKPOINT,SLEELA_DEBUGGER_STOP_WATCHPOINT,SLEELA_DEBUGGER_STOP_EXCEPTION,SLEELA_DEBUGGER_STOP_SIGNAL,SLEELA_DEBUGGER_STOP_ASSERTION,SLEELA_DEBUGGER_STOP_SANITIZER,SLEELA_DEBUGGER_STOP_CRASH,SLEELA_DEBUGGER_STOP_USER_PAUSE,SLEELA_DEBUGGER_STOP_TEST_FAILURE } sleela_debugger_stop_reason_t;
typedef struct sleela_debugger_location { const char *file; uint32_t line; uint32_t column; } sleela_debugger_location_t;
typedef struct sleela_debugger_stop_record { sleela_debugger_stop_reason_t reason; uint64_t breakpoint_id; uint64_t watchpoint_id; const char *thread; sleela_debugger_location_t location; const char *detail; } sleela_debugger_stop_record_t;
typedef struct sleela_debugger_event { sleela_debugger_event_type_t type; const char *severity; const char *message; const char *thread; sleela_debugger_location_t location; } sleela_debugger_event_t;
typedef struct sleela_debugger_evidence { uint64_t sequence; const char *observation; const char *event; sleela_debugger_location_t source; const char *thread; const char *stack_summary; const char *action; const char *backend_result; const char *diagnostic; } sleela_debugger_evidence_t;
sleela_debugger_session_t *sleela_debugger_create(void);
void sleela_debugger_destroy(sleela_debugger_session_t *);
uint64_t sleela_debugger_add_breakpoint(sleela_debugger_session_t *,const char *,const sleela_debugger_location_t *);
uint64_t sleela_debugger_add_watchpoint(sleela_debugger_session_t *,const char *);
sleela_debugger_result_t sleela_debugger_remove_breakpoint(sleela_debugger_session_t *,uint64_t);
sleela_debugger_result_t sleela_debugger_remove_watchpoint(sleela_debugger_session_t *,uint64_t);
sleela_debugger_result_t sleela_debugger_emit(sleela_debugger_session_t *,const sleela_debugger_event_t *);
sleela_debugger_result_t sleela_debugger_add_evidence(sleela_debugger_session_t *, const sleela_debugger_evidence_t *, uint64_t *sequence_out);
size_t sleela_debugger_evidence_count(const sleela_debugger_session_t *);
size_t sleela_debugger_event_count(const sleela_debugger_session_t *);
sleela_debugger_result_t sleela_debugger_set_stop_record(sleela_debugger_session_t *,const sleela_debugger_stop_record_t *);
sleela_debugger_result_t sleela_debugger_write_report(const sleela_debugger_session_t *,const char *);
const char *sleela_debugger_result_string(sleela_debugger_result_t);
#ifdef __cplusplus
}
#endif
#endif
