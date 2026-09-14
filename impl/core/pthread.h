#ifndef SLEELA_PTHREAD_COMPAT_H
#define SLEELA_PTHREAD_COMPAT_H

/*
 * Compatibility boundary for legacy pthread names used by sleela_core.c.
 * Linux/POSIX builds continue to use the native pthread implementation.
 * Windows builds map the legacy names to the OS-aware Sleela threading API.
 */
#ifdef _WIN32

#include "sleela_thread.h"

typedef SLThreadHandle pthread_t;
typedef SLThreadMutex pthread_mutex_t;
typedef SLThreadCond pthread_cond_t;

typedef void pthread_attr_t;

typedef void pthread_mutexattr_t;
typedef void pthread_condattr_t;

#define pthread_mutex_init(mutex, attr) slthread_mutex_init((mutex))
#define pthread_mutex_destroy(mutex) slthread_mutex_destroy((mutex))
#define pthread_mutex_lock(mutex) slthread_mutex_lock((mutex))
#define pthread_mutex_unlock(mutex) slthread_mutex_unlock((mutex))

#define pthread_cond_init(cond, attr) slthread_cond_init((cond))
#define pthread_cond_destroy(cond) slthread_cond_destroy((cond))
#define pthread_cond_wait(cond, mutex) slthread_cond_wait((cond), (mutex))
#define pthread_cond_broadcast(cond) slthread_cond_broadcast((cond))

#define pthread_create(thread, attr, start, arg) slthread_create((thread), (start), (arg))
#define pthread_join(thread, retval) slthread_join((thread))

#else

/* Keep native POSIX behavior on Linux and other POSIX targets. */
#include_next <pthread.h>

#endif

#endif
