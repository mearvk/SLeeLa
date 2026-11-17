/* events_portable.c -- portable smoke test for impl/core/sleela_events.c.
 *
 * reg_events.c is the full POSIX suite (it links the instrumented allocator
 * through GNU ld --wrap). This file needs only a C11 compiler and the OS
 * thread API, so CI can compile and run the Windows and macOS branches of
 * sleela_events.c too. Build:
 *
 *   cc -std=c11 -Wall -Wextra -Wpedantic -Werror \
 *      test-suites/regression/c/events_portable.c impl/core/sleela_events.c \
 *      -o events_portable [-pthread]
 *
 * (No -Iimpl/core: that directory holds a Windows pthread.h shim that must not
 * replace the system header on POSIX.)
 *
 * Exit status 0 only when every check passes.
 */
#ifndef _WIN32
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
#endif
#include "../../../impl/core/sleela_events.h"

#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <time.h>
#endif

static int failures;
static int checks;

#define CHECK(cond) do { checks++; if (!(cond)) { failures++; fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); } } while (0)
#define CHECK_EQ(want, got) do { long long w_ = (long long)(want), g_ = (long long)(got); checks++; \
    if (w_ != g_) { failures++; fprintf(stderr, "FAIL %s:%d: %s == %s (want %lld, got %lld)\n", __FILE__, __LINE__, #want, #got, w_, g_); } } while (0)

static uint64_t now_ms(void) {
#ifdef _WIN32
    return (uint64_t)GetTickCount64();
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000u + (uint64_t)ts.tv_nsec / 1000000u;
#endif
}

static void sleep_ms(unsigned ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000u);
    ts.tv_nsec = (long)(ms % 1000u) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

static void t_platform(void) {
#if defined(_WIN32)
    CHECK_EQ(SL_EVENT_WINDOWS, slevent_platform());
    CHECK(strcmp(slevent_platform_name(), "windows-events") == 0);
#elif defined(__APPLE__)
    CHECK_EQ(SL_EVENT_MACOS, slevent_platform());
    CHECK(strcmp(slevent_platform_name(), "macos-signals-events") == 0);
#else
    CHECK_EQ(SL_EVENT_LINUX, slevent_platform());
    CHECK(strcmp(slevent_platform_name(), "linux-signals-events") == 0);
#endif
    CHECK_EQ(1, slevent_platform_is_available(SL_EVENT_AUTO));
    CHECK_EQ(1, slevent_platform_is_available(slevent_platform()));
}

static void t_manual_reset_and_timeout(void) {
    SLEventHandle h = slevent_create(0), s = slevent_create(1);
    uint64_t t0, waited;
    CHECK(slevent_valid(h));
    CHECK(slevent_valid(s));
    CHECK_EQ(ETIMEDOUT, slevent_wait(h, 0));
    CHECK_EQ(0, slevent_wait(s, 0));
    t0 = now_ms();
    CHECK_EQ(ETIMEDOUT, slevent_wait(h, 80));
    waited = now_ms() - t0;
    CHECK(waited >= 60);           /* a timed wait must block, not return at once */
    CHECK(waited < 5000);
    CHECK_EQ(0, slevent_set(h));
    CHECK_EQ(0, slevent_wait(h, 0));
    CHECK_EQ(0, slevent_wait(h, 0));  /* manual reset: stays set */
    CHECK_EQ(0, slevent_reset(h));
    CHECK_EQ(ETIMEDOUT, slevent_wait(h, 0));
    CHECK_EQ(0, slevent_close(h));
    CHECK_EQ(0, slevent_close(s));
}

static void t_invalid_handles(void) {
    SLEventHandle bad[2] = { SL_EVENT_INVALID, 0 };
    int i;
    for (i = 0; i < 2; i++) {
        CHECK(!slevent_valid(bad[i]));
        CHECK_EQ(EINVAL, slevent_set(bad[i]));
        CHECK_EQ(EINVAL, slevent_reset(bad[i]));
        CHECK_EQ(EINVAL, slevent_wait(bad[i], 0));
        /* must be rejected, not passed to the OS (on Windows -1 is the
         * current-process pseudo-handle, so this used to block forever) */
        CHECK_EQ(EINVAL, slevent_wait(bad[i], UINT32_MAX));
        CHECK_EQ(EINVAL, slevent_close(bad[i]));
    }
}

static SLEventHandle g_event;
static int g_wait_result = -1;

#ifdef _WIN32
static DWORD WINAPI waiter(LPVOID arg) { (void)arg; g_wait_result = slevent_wait(g_event, UINT32_MAX); return 0; }
#else
static void* waiter(void* arg) { (void)arg; g_wait_result = slevent_wait(g_event, UINT32_MAX); return NULL; }
#endif

static void t_set_wakes_a_blocked_waiter(void) {
    g_event = slevent_create(0);
    CHECK(slevent_valid(g_event));
#ifdef _WIN32
    {
        HANDLE th = CreateThread(NULL, 0, waiter, NULL, 0, NULL);
        CHECK(th != NULL);
        sleep_ms(50);
        CHECK_EQ(-1, g_wait_result);  /* still blocked */
        CHECK_EQ(0, slevent_set(g_event));
        CHECK_EQ(WAIT_OBJECT_0, WaitForSingleObject(th, 5000));
        CloseHandle(th);
    }
#else
    {
        pthread_t th;
        CHECK_EQ(0, pthread_create(&th, NULL, waiter, NULL));
        sleep_ms(50);
        CHECK_EQ(-1, g_wait_result);
        CHECK_EQ(0, slevent_set(g_event));
        CHECK_EQ(0, pthread_join(th, NULL));
    }
#endif
    CHECK_EQ(0, g_wait_result);
    CHECK_EQ(0, slevent_close(g_event));
}

static void t_signal_handler_survives_repeat_delivery(void) {
    int round;
    CHECK_EQ(0, slevent_install_signal(SIGINT));
    for (round = 0; round < 3; round++) {
        CHECK_EQ(0, slevent_signal_pending(SIGINT));
        CHECK_EQ(0, raise(SIGINT));   /* a one-shot handler kills the process on round 2 */
        CHECK_EQ(1, slevent_signal_pending(SIGINT));
        CHECK_EQ(0, slevent_clear_signal(SIGINT));
    }
    CHECK_EQ(EINVAL, slevent_install_signal(0));
}

static void t_queue_is_bounded_fifo(void) {
    SLEvent e;
    uint32_t k;
    while (slevent_poll(&e) == 1) {}
    for (k = 0; k < 64; k++) CHECK_EQ(0, slevent_post(1, k, (intptr_t)k * 10));
    CHECK_EQ(EAGAIN, slevent_post(1, 99, 0));
    for (k = 0; k < 64; k++) {
        CHECK_EQ(1, slevent_poll(&e));
        CHECK_EQ(k, e.code);
        CHECK_EQ((intptr_t)k * 10, e.value);
    }
    CHECK_EQ(0, slevent_poll(&e));
    CHECK_EQ(EINVAL, slevent_poll(NULL));
}

int main(void) {
    t_platform();
    t_manual_reset_and_timeout();
    t_invalid_handles();
    t_set_wakes_a_blocked_waiter();
    t_signal_handler_survives_repeat_delivery();
    t_queue_is_bounded_fifo();
    printf("SUITE events_portable (%s): checks=%d failures=%d\n", slevent_platform_name(), checks, failures);
    return failures == 0 ? 0 : 1;
}
