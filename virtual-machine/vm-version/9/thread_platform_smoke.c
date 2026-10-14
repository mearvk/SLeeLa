#include "sleela_thread.h"

#include <stdio.h>

static void* worker(void* arg) {
    int* value = (int*)arg;
    (*value)++;
    return NULL;
}

int main(void) {
    int value = 0;
    SLThreadHandle thread;
    SLThreadMutex mutex;
    SLThreadCond cond;

    if (!slthread_platform_is_available(SL_THREAD_AUTO)) return 1;
    if (slthread_mutex_init(&mutex) != 0) return 2;
    if (slthread_cond_init(&cond) != 0) return 3;
    if (slthread_create(&thread, worker, &value) != 0) return 4;
    if (slthread_join(thread) != 0) return 5;
    if (slthread_mutex_lock(&mutex) != 0) return 6;
    if (slthread_mutex_unlock(&mutex) != 0) return 7;
    if (slthread_cond_broadcast(&cond) != 0) return 8;
    slthread_cond_destroy(&cond);
    slthread_mutex_destroy(&mutex);

    if (value != 1) return 9;
    printf("native threading backend: %s\n", slthread_platform_name());
    printf("threading platform smoke: PASS\n");
    return 0;
}
