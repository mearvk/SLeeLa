/* Native IPC VM handle-table lifecycle and stale-handle regression. */
#include "../../core/sleela_core.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct { int closed; } FakeWorker;
typedef struct { SLVM *vm; int32_t handle; } CloseArgs;

static void fake_close(void *opaque, void *context) {
    FakeWorker *worker=(FakeWorker*)opaque;
    int *count=(int*)context;
    if (worker) { worker->closed=1; ++*count; }
}
static void *close_thread(void *opaque) {
    CloseArgs *args=(CloseArgs*)opaque;
    (void)slvm_ipc_worker_close(args->vm,args->handle);
    return NULL;
}
#define CHECK(x,msg) do { if (!(x)) { fprintf(stderr,"FAIL: %s\n",msg); return 1; } } while (0)

int main(void) {
    SLVM *vm=slvm_new();
    FakeWorker workers[SL_MAX_IPC_WORKERS+1];
    int closed=0;
    int32_t handles[SL_MAX_IPC_WORKERS];
    pthread_t thread;
    CloseArgs args;
    struct timespec pause={0,50000000};
    memset(workers,0,sizeof(workers));
    CHECK(vm!=NULL,"VM allocation");
    CHECK(slvm_ipc_worker_acquire(vm,-1)==NULL,"negative handle rejected");
    CHECK(slvm_ipc_worker_close(vm,-1)==-1,"negative close rejected");
    handles[0]=slvm_ipc_worker_register(vm,&workers[0],fake_close,&closed);
    CHECK(handles[0]>=0,"first worker registered");
    CHECK(slvm_ipc_worker_acquire(vm,handles[0])==&workers[0],"valid handle acquired");
    slvm_ipc_worker_release(vm,handles[0]);
    CHECK(slvm_ipc_worker_close(vm,handles[0])==0,"valid handle closed");
    CHECK(workers[0].closed && closed==1,"close callback exactly once");
    CHECK(slvm_ipc_worker_acquire(vm,handles[0])==NULL,"stale handle rejected after close");
    handles[0]=slvm_ipc_worker_register(vm,&workers[0],fake_close,&closed);
    CHECK(handles[0]>=0,"slot reused");
    CHECK(handles[0]!=(int32_t)0,"generation-tagged handle");
    CHECK(slvm_ipc_worker_acquire(vm,handles[0])==&workers[0],"reused handle acquired");
    /* Hold a lease while close runs on another thread: callback must wait. */
    args.vm=vm; args.handle=handles[0];
    CHECK(pthread_create(&thread,NULL,close_thread,&args)==0,"close thread starts");
    nanosleep(&pause,NULL);
    CHECK(closed==1,"close waits for outstanding lease");
    slvm_ipc_worker_release(vm,handles[0]);
    pthread_join(thread,NULL);
    CHECK(closed==2,"close callback runs after final release");
    CHECK(slvm_ipc_worker_acquire(vm,handles[0])==NULL,"closed generation is invalid");
    for (int i=0;i<SL_MAX_IPC_WORKERS;i++) {
        handles[i]=slvm_ipc_worker_register(vm,&workers[i],fake_close,&closed);
        CHECK(handles[i]>=0,"bounded table slot allocated");
    }
    CHECK(slvm_ipc_worker_register(vm,&workers[SL_MAX_IPC_WORKERS],fake_close,&closed)==-1,
          "table exhaustion fails closed");
    slvm_free(vm);
    CHECK(closed==2+SL_MAX_IPC_WORKERS,"VM shutdown closes every remaining worker");
    puts("Native IPC VM handle-table smoke: PASS");
    return 0;
}
