#include "../debugger_c.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    sleela_debugger_session_t *s = sleela_debugger_create();
    sleela_debugger_location_t loc = {"debugger_c_test.c", 12u, 3u};
    sleela_debugger_event_t e = {
        SLEELA_DEBUGGER_EVENT_BREAKPOINT, "info",
        "C breakpoint hit", "test-thread", loc
    };

    assert(s != NULL);
    assert(sleela_debugger_add_breakpoint(s, "main", &loc) != 0u);
    assert(sleela_debugger_add_watchpoint(s, "counter") != 0u);
    assert(sleela_debugger_emit(s, &e) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_event_count(s) == 1u);
    assert(sleela_debugger_write_report(s, "debugger-c-report.txt") == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_remove_breakpoint(s, 1u) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_remove_watchpoint(s, 2u) == SLEELA_DEBUGGER_OK);
    remove("debugger-c-report.txt");
    sleela_debugger_destroy(s);
    return 0;
}
