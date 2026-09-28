#ifndef SLEELA_EVENTS_H
#define SLEELA_EVENTS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_EVENT_AUTO = 0,
    SL_EVENT_LINUX = 1,
    SL_EVENT_WINDOWS = 2,
    SL_EVENT_MACOS = 3
} SLEventPlatform;

typedef intptr_t SLEventHandle;
#define SL_EVENT_INVALID ((SLEventHandle)-1)

typedef struct {
    uint32_t type;
    uint32_t code;
    intptr_t value;
} SLEvent;

SLEventPlatform slevent_platform(void);
const char* slevent_platform_name(void);
int slevent_platform_is_available(SLEventPlatform platform);

/* Explicit process/runtime signal notification. */
int slevent_install_signal(int signal_number);
int slevent_signal_pending(int signal_number);
int slevent_clear_signal(int signal_number);

/* Cross-platform manual-reset event. */
SLEventHandle slevent_create(int initial_state);
int slevent_set(SLEventHandle event);
int slevent_reset(SLEventHandle event);
int slevent_wait(SLEventHandle event, uint32_t timeout_ms);
int slevent_close(SLEventHandle event);
int slevent_valid(SLEventHandle event);

/* Runtime event queue: one producer/consumer-safe event at a time. */
int slevent_post(uint32_t type, uint32_t code, intptr_t value);
int slevent_poll(SLEvent* event);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_EVENTS_H */
