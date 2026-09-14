#include "sleela_thread.h"

#include <stdlib.h>

SLThreadPlatform slthread_platform(void) {
#ifdef _WIN32
    return SL_THREAD_WINDOWS;
#else
    return SL_THREAD_LINUX;
#endif
}

const char* slthread_platform_name(void) {
    return slthread_platform() == SL_THREAD_WINDOWS ? "windows" : "linux";
}

int slthread_platform_is_available(SLThreadPlatform platform) {
    if (platform == SL_THREAD_AUTO) return 1;
#ifdef _WIN32
    return platform == SL_THREAD_WINDOWS;
#else
    return platform == SL_THREAD_LINUX;
#endif
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

int slthread_mutex_init(SLThreadMutex* mutex) { return pthread_mutex_init(mutex, NULL); }
int slthread_mutex_destroy(SLThreadMutex* mutex) { return pthread_mutex_destroy(mutex); }
int slthread_mutex_lock(SLThreadMutex* mutex) { return pthread_mutex_lock(mutex); }
int slthread_mutex_unlock(SLThreadMutex* mutex) { return pthread_mutex_unlock(mutex); }

int slthread_cond_init(SLThreadCond* cond) { return pthread_cond_init(cond, NULL); }
int slthread_cond_destroy(SLThreadCond* cond) { return pthread_cond_destroy(cond); }
int slthread_cond_wait(SLThreadCond* cond, SLThreadMutex* mutex) { return pthread_cond_wait(cond, mutex); }
int slthread_cond_broadcast(SLThreadCond* cond) { return pthread_cond_broadcast(cond); }

int slthread_create(SLThreadHandle* thread, SLThreadStart start, void* arg) {
    return pthread_create(thread, NULL, start, arg);
}
int slthread_join(SLThreadHandle thread) { return pthread_join(thread, NULL); }

#endif
