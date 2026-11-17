#ifndef _WIN32
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#endif
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

/* NSIG is not part of ISO C and is not exposed under a strict -std=c11 (no
 * feature-test macro). The signal-flag table is clamped to 64 entries anyway,
 * so provide a portable fallback bound when the platform does not define it. */
#ifndef NSIG
#define NSIG 64
#endif

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
#if defined(_WIN32)
    return SL_EVENT_WINDOWS;
#elif defined(__APPLE__)
    return SL_EVENT_MACOS;
#else
    return SL_EVENT_LINUX;
#endif
}

const char* slevent_platform_name(void) {
#if defined(_WIN32)
    return "windows-events";
#elif defined(__APPLE__)
    return "macos-signals-events";
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
#ifdef _WIN32
    return signal(signal_number, sl_signal_handler) == SIG_ERR ? errno : 0;
#else
    /* sigaction, not signal(): signal() resets the handler after the first
     * delivery under strict ISO feature macros (SysV semantics), which made a
     * second delivery kill the process. */
    {
        struct sigaction action;
        memset(&action, 0, sizeof action);
        action.sa_handler = sl_signal_handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = SA_RESTART;
        return sigaction(signal_number, &action, NULL) == 0 ? 0 : errno;
    }
#endif
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

#ifndef _WIN32
/* A manual-reset event: a flag guarded by a mutex, with a condition variable so
 * that waiters block instead of polling and one set releases every waiter. */
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int state;
} SLEventObject;

#if defined(__APPLE__)
#define SL_EVENT_COND_CLOCK CLOCK_REALTIME   /* no pthread_condattr_setclock on macOS */
#else
#define SL_EVENT_COND_CLOCK CLOCK_MONOTONIC
#endif

static SLEventObject* sl_event_object(SLEventHandle event) {
    return (event == 0 || event == SL_EVENT_INVALID) ? NULL : (SLEventObject*)(intptr_t)event;
}
#endif

SLEventHandle slevent_create(int initial_state) {
#ifdef _WIN32
    HANDLE handle = CreateEventA(NULL, TRUE, initial_state ? TRUE : FALSE, NULL);
    return handle ? (SLEventHandle)(intptr_t)handle : SL_EVENT_INVALID;
#else
    SLEventObject* obj = (SLEventObject*)malloc(sizeof(*obj));
    pthread_condattr_t attr;
    if (!obj) return SL_EVENT_INVALID;
    if (pthread_mutex_init(&obj->lock, NULL) != 0) { free(obj); return SL_EVENT_INVALID; }
    if (pthread_condattr_init(&attr) != 0) { pthread_mutex_destroy(&obj->lock); free(obj); return SL_EVENT_INVALID; }
#if !defined(__APPLE__)
    pthread_condattr_setclock(&attr, SL_EVENT_COND_CLOCK);
#endif
    if (pthread_cond_init(&obj->cond, &attr) != 0) {
        pthread_condattr_destroy(&attr); pthread_mutex_destroy(&obj->lock); free(obj); return SL_EVENT_INVALID;
    }
    pthread_condattr_destroy(&attr);
    obj->state = initial_state ? 1 : 0;
    return (SLEventHandle)(intptr_t)obj;
#endif
}

int slevent_set(SLEventHandle event) {
#ifdef _WIN32
    return SetEvent((HANDLE)(intptr_t)event) ? 0 : (int)GetLastError();
#else
    SLEventObject* obj = sl_event_object(event);
    if (!obj) return EINVAL;
    pthread_mutex_lock(&obj->lock);
    obj->state = 1;
    pthread_cond_broadcast(&obj->cond);
    pthread_mutex_unlock(&obj->lock);
    return 0;
#endif
}

int slevent_reset(SLEventHandle event) {
#ifdef _WIN32
    return ResetEvent((HANDLE)(intptr_t)event) ? 0 : (int)GetLastError();
#else
    SLEventObject* obj = sl_event_object(event);
    if (!obj) return EINVAL;
    pthread_mutex_lock(&obj->lock);
    obj->state = 0;
    pthread_mutex_unlock(&obj->lock);
    return 0;
#endif
}

int slevent_wait(SLEventHandle event, uint32_t timeout_ms) {
#ifdef _WIN32
    DWORD result = WaitForSingleObject((HANDLE)(intptr_t)event, timeout_ms == UINT32_MAX ? INFINITE : timeout_ms);
    return result == WAIT_OBJECT_0 ? 0 : (result == WAIT_TIMEOUT ? ETIMEDOUT : (int)GetLastError());
#else
    SLEventObject* obj = sl_event_object(event);
    int rc = 0;
    if (!obj) return EINVAL;
    pthread_mutex_lock(&obj->lock);
    if (!obj->state && timeout_ms == UINT32_MAX) {
        while (!obj->state) pthread_cond_wait(&obj->cond, &obj->lock);
    } else if (!obj->state && timeout_ms > 0) {
        struct timespec deadline;
        clock_gettime(SL_EVENT_COND_CLOCK, &deadline);
        deadline.tv_sec += (time_t)(timeout_ms / 1000u);
        deadline.tv_nsec += (long)(timeout_ms % 1000u) * 1000000L;
        if (deadline.tv_nsec >= 1000000000L) { deadline.tv_sec += 1; deadline.tv_nsec -= 1000000000L; }
        while (!obj->state && rc == 0) rc = pthread_cond_timedwait(&obj->cond, &obj->lock, &deadline);
    }
    rc = obj->state ? 0 : ETIMEDOUT;
    pthread_mutex_unlock(&obj->lock);
    return rc;
#endif
}

int slevent_close(SLEventHandle event) {
#ifdef _WIN32
    return CloseHandle((HANDLE)(intptr_t)event) ? 0 : (int)GetLastError();
#else
    SLEventObject* obj = sl_event_object(event);
    if (!obj) return EINVAL;
    pthread_cond_destroy(&obj->cond);
    pthread_mutex_destroy(&obj->lock);
    free(obj);
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
