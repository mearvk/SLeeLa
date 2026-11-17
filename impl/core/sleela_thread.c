#include "sleela_thread.h"

#include <stdlib.h>

SLThreadPlatform slthread_platform(void) {
#if defined(_WIN32)
    return SL_THREAD_WINDOWS;
#elif defined(__APPLE__)
    return SL_THREAD_MACOS;
#else
    return SL_THREAD_LINUX;
#endif
}

const char* slthread_platform_name(void) {
    switch (slthread_platform()) {
        case SL_THREAD_WINDOWS: return "windows";
        case SL_THREAD_MACOS:   return "macos";
        default:                return "linux";
    }
}

int slthread_platform_is_available(SLThreadPlatform platform) {
    if (platform == SL_THREAD_AUTO) return 1;
    return platform == slthread_platform();
}

#ifdef _WIN32

static DWORD WINAPI slthread_entry(LPVOID raw) {
    struct SLThreadStartBlock { SLThreadStart start; void* arg; } *block = raw;
    SLThreadStart start = block->start;
    void* arg = block->arg;
    free(block);
    (void)start(arg);
    return 0;
}

int slthread_mutex_init(SLThreadMutex* mutex) {
    if (!mutex) return -1;
    InitializeCriticalSection(mutex);
    return 0;
}
int slthread_mutex_destroy(SLThreadMutex* mutex) {
    if (!mutex) return -1;
    DeleteCriticalSection(mutex);
    return 0;
}
int slthread_mutex_lock(SLThreadMutex* mutex) {
    if (!mutex) return -1;
    EnterCriticalSection(mutex);
    return 0;
}
int slthread_mutex_unlock(SLThreadMutex* mutex) {
    if (!mutex) return -1;
    LeaveCriticalSection(mutex);
    return 0;
}

int slthread_cond_init(SLThreadCond* cond) {
    if (!cond) return -1;
    InitializeConditionVariable(cond);
    return 0;
}
int slthread_cond_destroy(SLThreadCond* cond) {
    (void)cond;
    return 0;
}
int slthread_cond_wait(SLThreadCond* cond, SLThreadMutex* mutex) {
    if (!cond || !mutex) return -1;
    return SleepConditionVariableCS(cond, mutex, INFINITE) ? 0 : -1;
}
int slthread_cond_broadcast(SLThreadCond* cond) {
    if (!cond) return -1;
    WakeAllConditionVariable(cond);
    return 0;
}

int slthread_create(SLThreadHandle* thread, SLThreadStart start, void* arg) {
    if (!thread || !start) return -1;
    struct SLThreadStartBlock { SLThreadStart start; void* arg; } *block = malloc(sizeof(*block));
    if (!block) return -1;
    block->start = start;
    block->arg = arg;
    HANDLE handle = CreateThread(NULL, 0, slthread_entry, block, 0, NULL);
    if (!handle) {
        free(block);
        return -1;
    }
    *thread = handle;
    return 0;
}
int slthread_join(SLThreadHandle thread) {
    if (!thread) return -1;
    DWORD r = WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    return r == WAIT_OBJECT_0 ? 0 : -1;
}

#else

/* A NULL argument is rejected with -1 on every platform (as the Windows branch
 * does) instead of being passed to pthreads, which would dereference it. */
int slthread_mutex_init(SLThreadMutex* mutex) { return mutex ? pthread_mutex_init(mutex, NULL) : -1; }
int slthread_mutex_destroy(SLThreadMutex* mutex) { return mutex ? pthread_mutex_destroy(mutex) : -1; }
int slthread_mutex_lock(SLThreadMutex* mutex) { return mutex ? pthread_mutex_lock(mutex) : -1; }
int slthread_mutex_unlock(SLThreadMutex* mutex) { return mutex ? pthread_mutex_unlock(mutex) : -1; }

int slthread_cond_init(SLThreadCond* cond) { return cond ? pthread_cond_init(cond, NULL) : -1; }
int slthread_cond_destroy(SLThreadCond* cond) { return cond ? pthread_cond_destroy(cond) : -1; }
int slthread_cond_wait(SLThreadCond* cond, SLThreadMutex* mutex) {
    return (cond && mutex) ? pthread_cond_wait(cond, mutex) : -1;
}
int slthread_cond_broadcast(SLThreadCond* cond) { return cond ? pthread_cond_broadcast(cond) : -1; }

int slthread_create(SLThreadHandle* thread, SLThreadStart start, void* arg) {
    if (!thread || !start) return -1;
    return pthread_create(thread, NULL, start, arg);
}
int slthread_join(SLThreadHandle thread) { return pthread_join(thread, NULL); }

#endif
