#include "sleela_events.h"

#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#endif

#define SL_EVENT_QUEUE_CAPACITY 64

static volatile sig_atomic_t sl_signal_flags[NSIG > 64 ? 64 : NSIG];
static SLEvent sl_queue[SL_EVENT_QUEUE_CAPACITY];
static size_t sl_queue_head;
static size_t sl_queue_tail;
static size_t sl_queue_count;
#ifdef _WIN32
static CRITICAL_SECTION sl_queue_lock;
static INIT_ONCE sl_queue_once = INIT_ONCE_STATIC_INIT;
static BOOL CALLBACK sl_queue_init(PINIT_ONCE once, PVOID param, PVOID* context) {
    (void)once; (void)param; (void)context;
    InitializeCriticalSection(&sl_queue_lock);
    return TRUE;
}
static void sl_lock(void) { InitOnceExecuteOnce(&sl_queue_once, sl_queue_init, NULL, NULL); EnterCriticalSection(&sl_queue_lock); }
static void sl_unlock(void) { LeaveCriticalSection(&sl_queue_lock); }
#else
static pthread_mutex_t sl_queue_lock = PTHREAD_MUTEX_INITIALIZER;
static void sl_lock(void) { pthread_mutex_lock(&sl_queue_lock); }
static void sl_unlock(void) { pthread_mutex_unlock(&sl_queue_lock); }
#endif

static void sl_signal_handler(int signal_number) {
    if (signal_number >= 0 && signal_number < (int)(sizeof(sl_signal_flags) / sizeof(sl_signal_flags[0]))) {
        sl_signal_flags[signal_number] = 1;
    }
}

SLEventPlatform slevent_platform(void) {
#ifdef _WIN32
    return SL_EVENT_WINDOWS;
#else
    return SL_EVENT_LINUX;
#endif
}

const char* slevent_platform_name(void) {
#ifdef _WIN32
    return "windows-events";
#else
    return "linux-signals-events";
#endif
}

int slevent_platform_is_available(SLEventPlatform platform) {
    if (platform == SL_EVENT_AUTO) return 1;
    return platform == slevent_platform();
}

int slevent_install_signal(int signal_number) {
    if (signal_number <= 0 || signal_number >= (int)(sizeof(sl_signal_flags) / sizeof(sl_signal_flags[0]))) return EINVAL;
    return signal(signal_number, sl_signal_handler) == SIG_ERR ? errno : 0;
}

int slevent_signal_pending(int signal_number) {
    if (signal_number <= 0 || signal_number >= (int)(sizeof(sl_signal_flags) / sizeof(sl_signal_flags[0]))) return 0;
    return sl_signal_flags[signal_number] != 0;
}

int slevent_clear_signal(int signal_number) {
    if (signal_number <= 0 || signal_number >= (int)(sizeof(sl_signal_flags) / sizeof(sl_signal_flags[0]))) return EINVAL;
    sl_signal_flags[signal_number] = 0;
    return 0;
}

SLEventHandle slevent_create(int initial_state) {
#ifdef _WIN32
    HANDLE handle = CreateEventA(NULL, TRUE, initial_state ? TRUE : FALSE, NULL);
    return handle ? (SLEventHandle)(intptr_t)handle : SL_EVENT_INVALID;
#else
    /* Linux event handles are represented by an internal event object. */
    int* state = (int*)malloc(sizeof(*state));
    if (!state) return SL_EVENT_INVALID;
    *state = initial_state ? 1 : 0;
    return (SLEventHandle)(intptr_t)state;
#endif
}

int slevent_set(SLEventHandle event) {
#ifdef _WIN32
    return SetEvent((HANDLE)(intptr_t)event) ? 0 : (int)GetLastError();
#else
    if (!event) return EINVAL;
    *(int*)(intptr_t)event = 1;
    return 0;
#endif
}

int slevent_reset(SLEventHandle event) {
#ifdef _WIN32
    return ResetEvent((HANDLE)(intptr_t)event) ? 0 : (int)GetLastError();
#else
    if (!event) return EINVAL;
    *(int*)(intptr_t)event = 0;
    return 0;
#endif
}

int slevent_wait(SLEventHandle event, uint32_t timeout_ms) {
#ifdef _WIN32
    DWORD result = WaitForSingleObject((HANDLE)(intptr_t)event, timeout_ms == UINT32_MAX ? INFINITE : timeout_ms);
    return result == WAIT_OBJECT_0 ? 0 : (result == WAIT_TIMEOUT ? ETIMEDOUT : (int)GetLastError());
#else
    if (!event) return EINVAL;
    if (*(int*)(intptr_t)event) return 0;
    if (timeout_ms == 0) return ETIMEDOUT;
    return ETIMEDOUT;
#endif
}

int slevent_close(SLEventHandle event) {
#ifdef _WIN32
    return CloseHandle((HANDLE)(intptr_t)event) ? 0 : (int)GetLastError();
#else
    if (!event) return EINVAL;
    free((void*)(intptr_t)event);
    return 0;
#endif
}

int slevent_valid(SLEventHandle event) {
#ifdef _WIN32
    return event != SL_EVENT_INVALID && event != 0;
#else
    return event != SL_EVENT_INVALID && event != 0;
#endif
}

int slevent_post(uint32_t type, uint32_t code, intptr_t value) {
    sl_lock();
    if (sl_queue_count == SL_EVENT_QUEUE_CAPACITY) {
        sl_unlock();
        return EAGAIN;
    }
    sl_queue[sl_queue_tail].type = type;
    sl_queue[sl_queue_tail].code = code;
    sl_queue[sl_queue_tail].value = value;
    sl_queue_tail = (sl_queue_tail + 1U) % SL_EVENT_QUEUE_CAPACITY;
    ++sl_queue_count;
    sl_unlock();
    return 0;
}

int slevent_poll(SLEvent* event) {
    if (!event) return EINVAL;
    sl_lock();
    if (sl_queue_count == 0) {
        sl_unlock();
        return 0;
    }
    *event = sl_queue[sl_queue_head];
    sl_queue_head = (sl_queue_head + 1U) % SL_EVENT_QUEUE_CAPACITY;
    --sl_queue_count;
    sl_unlock();
    return 1;
}
