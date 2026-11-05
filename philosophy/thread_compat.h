#ifndef SLEELA_PHILOSOPHY_THREAD_COMPAT_H
#define SLEELA_PHILOSOPHY_THREAD_COMPAT_H
/*
 * Portable threading shim for the philosophy module.
 *
 * "Best available" threading, auto-selected at compile time so the same
 * source builds consistently across Windows XP+ / 7+ / 10+, Linux and macOS:
 *
 *   1. Win32 CreateThread / CRITICAL_SECTION on Windows (covers XP+).
 *   2. POSIX pthreads on macOS (the Apple SDK does not consistently ship
 *      <threads.h>) and as the portable fallback.
 *   3. C11 <threads.h> on non-Apple, non-Windows toolchains when available.
 *
 * Only the small surface the signal toss needs is wrapped: one worker
 * thread, a mutex, and a monotonic millisecond sleep. The API is C-linkage
 * so both the C and C++ translation units share it.
 */
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ---- Backend selection ------------------------------------------------- */
#if !defined(SL_THREAD_BACKEND)
   /* Prefer native platform APIs where C11 threads are absent or incomplete.
    * MinGW/MSYS2 and the Apple SDKs may advertise C11 language mode without
    * shipping a usable <threads.h>. */
#  if defined(_WIN32)
#    define SL_THREAD_BACKEND 2 /* Win32 */
#  elif defined(__APPLE__)
#    define SL_THREAD_BACKEND 3 /* pthreads */
#  elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) && \
        !defined(__STDC_NO_THREADS__)
#    define SL_THREAD_BACKEND 1 /* C11 threads */
#  else
#    define SL_THREAD_BACKEND 3 /* pthreads */
#  endif
#endif

#if SL_THREAD_BACKEND == 1
#  include <threads.h>
#  include <time.h>
typedef thrd_t sl_thread;
typedef mtx_t sl_mutex;
#elif SL_THREAD_BACKEND == 2
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN
#  endif
#  include <windows.h>
typedef HANDLE sl_thread;
typedef CRITICAL_SECTION sl_mutex;
#else
#  include <pthread.h>
#  include <time.h>
typedef pthread_t sl_thread;
typedef pthread_mutex_t sl_mutex;
#endif

typedef int (*sl_thread_fn)(void *);

/* Returns 0 on success, non-zero on failure. */
int sl_thread_start(sl_thread *thread, sl_thread_fn fn, void *arg);
int sl_thread_join(sl_thread *thread);

int sl_mutex_init(sl_mutex *mutex);
void sl_mutex_lock(sl_mutex *mutex);
void sl_mutex_unlock(sl_mutex *mutex);
void sl_mutex_destroy(sl_mutex *mutex);

/* Monotonic-ish sleep; coarse is fine, the toss re-checks the clock. */
void sl_sleep_ms(uint32_t milliseconds);
/* Milliseconds from a steady clock, for rate throttling. */
uint64_t sl_monotonic_ms(void);

#ifdef __cplusplus
}
#endif
#endif
