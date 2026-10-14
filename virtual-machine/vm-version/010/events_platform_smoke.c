#include "sleela_events.h"
#include <stdio.h>
#include <signal.h>

int main(void) {
    SLEvent event;
    SLEventHandle handle;

    if (!slevent_platform_is_available(SL_EVENT_AUTO)) return 1;
    handle = slevent_create(0);
    if (!slevent_valid(handle)) return 2;
    if (slevent_wait(handle, 0) == 0) return 3;
    if (slevent_set(handle) != 0) return 4;
    if (slevent_wait(handle, 0) != 0) return 5;
    if (slevent_reset(handle) != 0) return 6;
    if (slevent_post(7, 42, (intptr_t)99) != 0) return 7;
    if (slevent_poll(&event) != 1 || event.type != 7 || event.code != 42 || event.value != 99) return 8;
#ifdef SIGUSR1
    if (slevent_install_signal(SIGUSR1) != 0) return 9;
    raise(SIGUSR1);
    if (!slevent_signal_pending(SIGUSR1)) return 10;
    if (slevent_clear_signal(SIGUSR1) != 0 || slevent_signal_pending(SIGUSR1)) return 11;
#endif
    if (slevent_close(handle) != 0) return 12;
    printf("events platform smoke: PASS (%s)\n", slevent_platform_name());
    return 0;
}
