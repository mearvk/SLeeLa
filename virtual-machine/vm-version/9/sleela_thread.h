#ifndef SLEELA_THREAD_H
#define SLEELA_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#include <windows.h>
typedef HANDLE SLThreadHandle;
typedef CRITICAL_SECTION SLThreadMutex;
typedef CONDITION_VARIABLE SLThreadCond;
#else
#include <pthread.h>
typedef pthread_t SLThreadHandle;
typedef pthread_mutex_t SLThreadMutex;
typedef pthread_cond_t SLThreadCond;
#endif

typedef void* (*SLThreadStart)(void*);

typedef enum {
    SL_THREAD_AUTO = 0,
    SL_THREAD_LINUX = 1,
    SL_THREAD_WINDOWS = 2,
    SL_THREAD_MACOS = 3
} SLThreadPlatform;

SLThreadPlatform slthread_platform(void);
const char* slthread_platform_name(void);
int slthread_platform_is_available(SLThreadPlatform platform);

int slthread_mutex_init(SLThreadMutex* mutex);
int slthread_mutex_destroy(SLThreadMutex* mutex);
int slthread_mutex_lock(SLThreadMutex* mutex);
int slthread_mutex_unlock(SLThreadMutex* mutex);

int slthread_cond_init(SLThreadCond* cond);
int slthread_cond_destroy(SLThreadCond* cond);
int slthread_cond_wait(SLThreadCond* cond, SLThreadMutex* mutex);
int slthread_cond_broadcast(SLThreadCond* cond);

int slthread_create(SLThreadHandle* thread, SLThreadStart start, void* arg);
int slthread_join(SLThreadHandle thread);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_THREAD_H */
