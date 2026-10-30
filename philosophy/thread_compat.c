#include "thread_compat.h"
#include <stdlib.h> /* malloc/free for the Win32 & pthreads trampolines */

/* ---- C11 threads ------------------------------------------------------- */
#if SL_THREAD_BACKEND == 1

int sl_thread_start(sl_thread *t, sl_thread_fn fn, void *arg) {
    return thrd_create(t, fn, arg) == thrd_success ? 0 : 1;
}
int sl_thread_join(sl_thread *t) {
    int result = 0;
    return thrd_join(*t, &result) == thrd_success ? 0 : 1;
}
int sl_mutex_init(sl_mutex *m) { return mtx_init(m, mtx_plain) == thrd_success ? 0 : 1; }
void sl_mutex_lock(sl_mutex *m) { (void)mtx_lock(m); }
void sl_mutex_unlock(sl_mutex *m) { (void)mtx_unlock(m); }
void sl_mutex_destroy(sl_mutex *m) { mtx_destroy(m); }
void sl_sleep_ms(uint32_t ms) {
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    thrd_sleep(&ts, NULL);
}
uint64_t sl_monotonic_ms(void) {
    struct timespec ts;
    if (timespec_get(&ts, TIME_UTC) != TIME_UTC) return 0u;
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)(ts.tv_nsec / 1000000);
}

/* ---- Win32 (covers Windows XP and newer) ------------------------------- */
#elif SL_THREAD_BACKEND == 2

typedef struct sl_win_trampoline {
    sl_thread_fn fn;
    void *arg;
} sl_win_trampoline;

static DWORD WINAPI sl_win_entry(LPVOID raw) {
    sl_win_trampoline local = *(sl_win_trampoline *)raw;
    free(raw);
    return (DWORD)local.fn(local.arg);
}
int sl_thread_start(sl_thread *t, sl_thread_fn fn, void *arg) {
    sl_win_trampoline *tramp = (sl_win_trampoline *)malloc(sizeof(*tramp));
    if (!tramp) return 1;
    tramp->fn = fn;
    tramp->arg = arg;
    *t = CreateThread(NULL, 0, sl_win_entry, tramp, 0, NULL);
    if (!*t) { free(tramp); return 1; }
    return 0;
}
int sl_thread_join(sl_thread *t) {
    if (WaitForSingleObject(*t, INFINITE) != WAIT_OBJECT_0) return 1;
    CloseHandle(*t);
    return 0;
}
int sl_mutex_init(sl_mutex *m) { InitializeCriticalSection(m); return 0; }
void sl_mutex_lock(sl_mutex *m) { EnterCriticalSection(m); }
void sl_mutex_unlock(sl_mutex *m) { LeaveCriticalSection(m); }
void sl_mutex_destroy(sl_mutex *m) { DeleteCriticalSection(m); }
void sl_sleep_ms(uint32_t ms) { Sleep(ms); }
uint64_t sl_monotonic_ms(void) { return (uint64_t)GetTickCount(); }

/* ---- POSIX pthreads (Linux, macOS, BSD) -------------------------------- */
#else

typedef struct sl_pth_trampoline {
    sl_thread_fn fn;
    void *arg;
} sl_pth_trampoline;

static void *sl_pth_entry(void *raw) {
    sl_pth_trampoline local = *(sl_pth_trampoline *)raw;
    free(raw);
    (void)local.fn(local.arg);
    return NULL;
}
int sl_thread_start(sl_thread *t, sl_thread_fn fn, void *arg) {
    sl_pth_trampoline *tramp = (sl_pth_trampoline *)malloc(sizeof(*tramp));
    if (!tramp) return 1;
    tramp->fn = fn;
    tramp->arg = arg;
    if (pthread_create(t, NULL, sl_pth_entry, tramp) != 0) { free(tramp); return 1; }
    return 0;
}
int sl_thread_join(sl_thread *t) { return pthread_join(*t, NULL) == 0 ? 0 : 1; }
int sl_mutex_init(sl_mutex *m) { return pthread_mutex_init(m, NULL) == 0 ? 0 : 1; }
void sl_mutex_lock(sl_mutex *m) { (void)pthread_mutex_lock(m); }
void sl_mutex_unlock(sl_mutex *m) { (void)pthread_mutex_unlock(m); }
void sl_mutex_destroy(sl_mutex *m) { (void)pthread_mutex_destroy(m); }
void sl_sleep_ms(uint32_t ms) {
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)((ms % 1000u) * 1000000u);
    nanosleep(&ts, NULL);
}
uint64_t sl_monotonic_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)(ts.tv_nsec / 1000000);
}

#endif
