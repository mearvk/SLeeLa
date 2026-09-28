#include "../debugger_line.h"
#include "../debugger_c.h"

#include <assert.h>
#include <stdio.h>

int main(void) {
    sleela_debugger_session_t *s = sleela_debugger_create();
    sleela_debugger_line_point_t trace = {"fixture.c", 10u, 1u, SLEELA_DEBUGGER_LINE_TRACE};
    sleela_debugger_line_point_t stop = {"fixture.c", 11u, 1u, SLEELA_DEBUGGER_LINE_STOP};
    sleela_debugger_line_point_t exception = {"fixture.c", 12u, 1u, SLEELA_DEBUGGER_LINE_EXCEPTION};
    char response[64];

    assert(s != NULL);
    assert(sleela_debugger_add_line_point(s, &trace) != 0u);
    assert(sleela_debugger_add_line_point(s, &stop) != 0u);
    assert(sleela_debugger_add_line_point(s, &exception) != 0u);

    assert(sleela_debugger_hit_line(s, &trace, "thread-1", "fixture") == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_hit_line(s, &stop, "thread-1", "fixture") == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_hit_line(s, &exception, "thread-1", "fixture") == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_event_count(s) == 3u);

    assert(sleela_debugger_voice_command(s, "continue", response, sizeof(response)) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_voice_command(s, "step", response, sizeof(response)) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_voice_command(s, "next", response, sizeof(response)) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_voice_command(s, "stop here", response, sizeof(response)) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_voice_command(s, "trace line", response, sizeof(response)) == SLEELA_DEBUGGER_OK);
    assert(sleela_debugger_voice_command(s, "not a debugger command", response, sizeof(response)) == SLEELA_DEBUGGER_INVALID_ARGUMENT);

    sleela_debugger_destroy(s);
    return 0;
}
