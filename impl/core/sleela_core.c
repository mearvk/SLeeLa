/* ==========================================================================
 * sleela_core.c -- Sleela execution core.
 *
 * Stack VM with threads, TCP sockets, files, anonymous pipes, and named
 * pipes/FIFOs. All OS facilities go through the OS-aware abstraction layer
 * (sleela_thread/pthread shim, sleela_net, sleela_io), so the core builds and
 * runs on both Linux/POSIX and Windows 10+ without raw platform calls. Host
 * resources are exposed only as VM-local bounded handles; Sleela code never
 * receives a raw descriptor, SOCKET, or HANDLE.
 * ========================================================================== */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "sleela_core.h"
#include "sleela_net.h"
#include "sleela_io.h"
#include "sleela_time.h"
#include "sleela_synchro.h"
#include "sleela_munction.h"
#include "sleela_bestof.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Threads come from the OS-aware layer: on Windows this shim remaps the
 * pthread_* names to the Win32-backed slthread_* API; on POSIX it pulls in the
 * native <pthread.h>. Sockets and files are handled entirely through
 * sleela_net.h / sleela_io.h, so no raw BSD-socket or <unistd.h> includes are
 * needed here anymore. The angle-bracket form resolves to core/pthread.h
 * (the build compiles the core with -Icore); that shim then pulls in the
 * native <pthread.h> on POSIX via #include_next. */
#include <pthread.h>
#ifndef _WIN32
#include <signal.h>
#endif

#define STACK_MAX 4096
#define FRAMES_MAX 1024

typedef struct { uint8_t op; int32_t a; } SLInstr;
typedef struct { char* name; int32_t entry; int32_t nargs; int32_t nlocals; } SLFunc;
typedef struct { int32_t ret_ip; int32_t base; int32_t nargs; } SLFrame;

typedef struct SLThread {
    SLVM* vm;
    SLValue stack[STACK_MAX]; int sp;
    SLFrame frames[FRAMES_MAX]; int fp;
    int entry_func;
    SLValue result;
    char err[256];
    pthread_t handle;
    int started;
} SLThread;

typedef struct {
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    int has;
    int32_t tag;
    SLValue value;
} SLMailbox;

typedef struct {
    pthread_mutex_t mtx;
    int active;
    SLNetHandle handle;
} SLSocket;

typedef struct {
    pthread_mutex_t mtx;
    int active;
    SLIOHandle fd;
    int peer;
} SLFile;

/* A registered struct type: a name and an ordered list of field names. Field
 * order defines the offsets used by OP_GETFIELD / OP_SETFIELD. */
typedef struct {
    char* name;
    int nfields;
    char* fields[SL_MAX_STRUCT_FIELDS];
} SLStructType;

/* A live struct instance: its type index plus one SLValue per declared field. */
typedef struct {
    int active;
    int type;
    SLValue fields[SL_MAX_STRUCT_FIELDS];
} SLStructInstance;

struct SLVM {
    SLInstr* code; int codelen, codecap;
    SLValue* consts; int nconst, constcap;
    char** strs; int nstr, strcap;
    SLValue* globals; char** gnames; int nglobal, globalcap;
    SLFunc* funcs; int nfunc, funccap;
    int cur_func;
    int entry;
    SLValue last_result;
    char err[256];

    pthread_mutex_t intern_mtx;
    pthread_mutex_t global_mtx;
    pthread_mutex_t print_mtx;
    pthread_mutex_t sock_mtx;
    pthread_mutex_t file_mtx;
    pthread_mutex_t struct_mtx;
    SLSocket sockets[SL_MAX_SOCKETS];
    SLFile files[SL_MAX_FILES];
    /* Synchro probes and Munction reaches: VM-owned bounded handle tables,
     * guarded by their own mutex (allocated lazily; NULL = free slot). */
    pthread_mutex_t synchro_mtx;
    SLSynchro* synchro[SL_SYNCHRO_MAX];
    pthread_mutex_t munction_mtx;
    SLMunction* munction[SL_MUNCTION_MAX];
    pthread_mutex_t bestof_mtx;
    SLBestOf* bestof[SL_BESTOF_MAX];
    SLStructType struct_types[SL_MAX_STRUCT_TYPES];
    int nstruct_types;
    SLStructInstance* structs;   /* SL_MAX_STRUCTS instances, lazily allocated */
    int nstruct_live;

    pthread_mutex_t locks[SL_MAX_LOCKS];
    SLMailbox mailbox[SL_MAX_LOCKS];
    pthread_mutex_t thr_mtx;
    SLThread* threads[SL_MAX_THREADS];
    int nthreads;
};

static void set_err(SLVM* vm, const char* msg) {
    if (!vm) return;
    strncpy(vm->err, msg, sizeof(vm->err) - 1);
    vm->err[sizeof(vm->err) - 1] = 0;
}

SLValue slval_null(void) { SLValue v; v.type = SL_NULL; v.as.i = 0; return v; }
SLValue slval_int(int64_t x) { SLValue v; v.type = SL_INT; v.as.i = x; return v; }
SLValue slval_double(double x) { SLValue v; v.type = SL_DOUBLE; v.as.d = x; return v; }
SLValue slval_bool(int x) { SLValue v; v.type = SL_BOOL; v.as.b = x ? 1 : 0; return v; }

const char* slvm_error(SLVM* vm) { return (vm && vm->err[0]) ? vm->err : NULL; }
const char* slvm_str(SLVM* vm, int32_t id) {
    if (!vm) return "";
    pthread_mutex_lock(&vm->intern_mtx);
    const char* r = (id >= 0 && id < vm->nstr) ? vm->strs[id] : "";
    pthread_mutex_unlock(&vm->intern_mtx);
    return r;
}

#define ENSURE(arr, len, cap, type) do { \
    if ((len) >= (cap)) { \
        (cap) = (cap) ? (cap) * 2 : 8; \
        (arr) = (type*)realloc((arr), (size_t)(cap) * sizeof(type)); \
    } \
} while (0)

static int intern(SLVM* vm, const char* s) {
    pthread_mutex_lock(&vm->intern_mtx);
    for (int i = 0; i < vm->nstr; i++) {
        if (strcmp(vm->strs[i], s) == 0) {
            pthread_mutex_unlock(&vm->intern_mtx);
            return i;
        }
    }
    ENSURE(vm->strs, vm->nstr, vm->strcap, char*);
    vm->strs[vm->nstr] = strdup(s);
    int id = vm->nstr++;
    pthread_mutex_unlock(&vm->intern_mtx);
    return id;
}

static int add_const(SLVM* vm, SLValue v) {
    for (int i = 0; i < vm->nconst; i++) {
        SLValue c = vm->consts[i];
        if (c.type != v.type) continue;
        if (v.type == SL_INT && c.as.i == v.as.i) return i;
        if (v.type == SL_DOUBLE && c.as.d == v.as.d) return i;
        if (v.type == SL_BOOL && c.as.b == v.as.b) return i;
        if (v.type == SL_STR && c.as.s == v.as.s) return i;
        if (v.type == SL_NULL) return i;
    }
    ENSURE(vm->consts, vm->nconst, vm->constcap, SLValue);
    vm->consts[vm->nconst] = v;
    return vm->nconst++;
}

int slvm_add_const_int(SLVM* vm, int64_t v) { return add_const(vm, slval_int(v)); }
int slvm_add_const_double(SLVM* vm, double v) { return add_const(vm, slval_double(v)); }
int slvm_add_const_bool(SLVM* vm, int v) { return add_const(vm, slval_bool(v)); }
int slvm_add_const_str(SLVM* vm, const char* s) {
    SLValue v; v.type = SL_STR; v.as.s = intern(vm, s); return add_const(vm, v);
}

int slvm_declare_global(SLVM* vm, const char* name) {
    for (int i = 0; i < vm->nglobal; i++)
        if (strcmp(vm->gnames[i], name) == 0) return i;
    ENSURE(vm->globals, vm->nglobal, vm->globalcap, SLValue);
    vm->gnames = (char**)realloc(vm->gnames, (size_t)vm->globalcap * sizeof(char*));
    vm->globals[vm->nglobal] = slval_null();
    vm->gnames[vm->nglobal] = strdup(name);
    return vm->nglobal++;
}

int slvm_declare_struct(SLVM* vm, const char* name, const char* const* field_names, int nfields) {
    if (!vm || !name || nfields < 0 || nfields > SL_MAX_STRUCT_FIELDS) return -1;
    if (vm->nstruct_types >= SL_MAX_STRUCT_TYPES) return -1;
    for (int i = 0; i < vm->nstruct_types; i++)
        if (strcmp(vm->struct_types[i].name, name) == 0) return i; /* idempotent */
    int idx = vm->nstruct_types++;
    SLStructType* st = &vm->struct_types[idx];
    st->name = strdup(name);
    st->nfields = nfields;
    for (int i = 0; i < nfields; i++) st->fields[i] = strdup(field_names[i] ? field_names[i] : "");
    return idx;
}

int slvm_begin_func(SLVM* vm, const char* name, int nargs, int nlocals) {
    ENSURE(vm->funcs, vm->nfunc, vm->funccap, SLFunc);
    int idx = vm->nfunc++;
    vm->funcs[idx].name = strdup(name);
    vm->funcs[idx].entry = vm->codelen;
    vm->funcs[idx].nargs = nargs;
    vm->funcs[idx].nlocals = nlocals;
    vm->cur_func = idx;
    return idx;
}
void slvm_end_func(SLVM* vm) { vm->cur_func = -1; }
int slvm_emit(SLVM* vm, SLOp op, int32_t a) {
    ENSURE(vm->code, vm->codelen, vm->codecap, SLInstr);
    vm->code[vm->codelen].op = (uint8_t)op;
    vm->code[vm->codelen].a = a;
    return vm->codelen++;
}
int slvm_emit0(SLVM* vm, SLOp op) { return slvm_emit(vm, op, 0); }
void slvm_patch(SLVM* vm, int at, int32_t a) { if (at >= 0 && at < vm->codelen) vm->code[at].a = a; }
int slvm_here(SLVM* vm) { return vm->codelen; }
void slvm_set_entry(SLVM* vm, int fi) { vm->entry = fi; }
SLValue slvm_result(SLVM* vm) { return vm->last_result; }

SLVM* slvm_new(void) {
    (void)sltime_init();
    SLVM* vm = (SLVM*)calloc(1, sizeof(SLVM));
    if (!vm) return NULL;
    vm->cur_func = -1;
    vm->entry = -1;
    vm->last_result = slval_null();
    pthread_mutex_init(&vm->intern_mtx, NULL);
    pthread_mutex_init(&vm->global_mtx, NULL);
    pthread_mutex_init(&vm->print_mtx, NULL);
    pthread_mutex_init(&vm->sock_mtx, NULL);
    pthread_mutex_init(&vm->file_mtx, NULL);
    pthread_mutex_init(&vm->struct_mtx, NULL);
    pthread_mutex_init(&vm->thr_mtx, NULL);
    pthread_mutex_init(&vm->synchro_mtx, NULL);
    pthread_mutex_init(&vm->munction_mtx, NULL);
    pthread_mutex_init(&vm->bestof_mtx, NULL);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_init(&vm->sockets[i].mtx, NULL);
        vm->sockets[i].active = 0; vm->sockets[i].handle = SL_NET_INVALID;
    }
    for (int i = 0; i < SL_MAX_FILES; i++) {
        pthread_mutex_init(&vm->files[i].mtx, NULL);
        vm->files[i].active = 0; vm->files[i].fd = SLIO_INVALID_HANDLE; vm->files[i].peer = -1;
    }
    for (int i = 0; i < SL_MAX_LOCKS; i++) {
        pthread_mutex_init(&vm->locks[i], NULL);
        pthread_mutex_init(&vm->mailbox[i].mtx, NULL);
        pthread_cond_init(&vm->mailbox[i].cond, NULL);
    }
    /* A broken FIFO/pipe writer must return EPIPE rather than terminate the VM. */
    if (slnet_startup() != 0) set_err(vm, "network backend startup failed");
#ifndef _WIN32
    signal(SIGPIPE, SIG_IGN);
#endif
    return vm;
}

static void close_file_slot(SLFile* f) {
    if (f->active) { slio_close(f->fd); f->fd = SLIO_INVALID_HANDLE; f->active = 0; f->peer = -1; }
}

void slvm_free(SLVM* vm) {
    if (!vm) return;
    slvm_joinall(vm);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_lock(&vm->sockets[i].mtx);
        if (vm->sockets[i].active) { slnet_close(vm->sockets[i].handle); vm->sockets[i].handle = SL_NET_INVALID; vm->sockets[i].active = 0; }
        pthread_mutex_unlock(&vm->sockets[i].mtx);
        pthread_mutex_destroy(&vm->sockets[i].mtx);
    }
    pthread_mutex_destroy(&vm->sock_mtx);
    slnet_shutdown();
    for (int i = 0; i < SL_MAX_FILES; i++) {
        pthread_mutex_lock(&vm->files[i].mtx);
        close_file_slot(&vm->files[i]);
        pthread_mutex_unlock(&vm->files[i].mtx);
        pthread_mutex_destroy(&vm->files[i].mtx);
    }
    pthread_mutex_destroy(&vm->file_mtx);
    /* Release any live Synchro probes and Munction reaches. */
    for (int i = 0; i < SL_SYNCHRO_MAX; i++) {
        if (vm->synchro[i]) { slsynchro_close(vm->synchro[i]); vm->synchro[i] = NULL; }
    }
    pthread_mutex_destroy(&vm->synchro_mtx);
    for (int i = 0; i < SL_MUNCTION_MAX; i++) {
        if (vm->munction[i]) { char sink[8]; slmunction_close(vm->munction[i], sink, sizeof(sink)); vm->munction[i] = NULL; }
    }
    pthread_mutex_destroy(&vm->munction_mtx);
    for (int i = 0; i < SL_BESTOF_MAX; i++) {
        if (vm->bestof[i]) { slbestof_close(vm->bestof[i]); vm->bestof[i] = NULL; }
    }
    pthread_mutex_destroy(&vm->bestof_mtx);
    for (int i = 0; i < vm->nstruct_types; i++) {
        free(vm->struct_types[i].name);
        for (int j = 0; j < vm->struct_types[i].nfields; j++) free(vm->struct_types[i].fields[j]);
    }
    free(vm->structs);
    pthread_mutex_destroy(&vm->struct_mtx);
    for (int i = 0; i < SL_MAX_LOCKS; i++) {
        pthread_mutex_destroy(&vm->locks[i]);
        pthread_mutex_destroy(&vm->mailbox[i].mtx);
        pthread_cond_destroy(&vm->mailbox[i].cond);
    }
    pthread_mutex_destroy(&vm->intern_mtx);
    pthread_mutex_destroy(&vm->global_mtx);
    pthread_mutex_destroy(&vm->print_mtx);
    pthread_mutex_destroy(&vm->thr_mtx);
    free(vm->code); free(vm->consts);
    for (int i = 0; i < vm->nstr; i++) free(vm->strs[i]);
    free(vm->strs); free(vm->globals);
    for (int i = 0; i < vm->nglobal; i++) free(vm->gnames[i]);
    free(vm->gnames);
    for (int i = 0; i < vm->nfunc; i++) free(vm->funcs[i].name);
    free(vm->funcs); free(vm);
}

static double as_num(SLValue v) {
    if (v.type == SL_INT) return (double)v.as.i;
    if (v.type == SL_DOUBLE) return v.as.d;
    if (v.type == SL_BOOL) return v.as.b;
    return 0.0;
}
static int is_truthy(SLValue v) {
    if (v.type == SL_NULL) return 0;
    if (v.type == SL_INT) return v.as.i != 0;
    if (v.type == SL_DOUBLE) return v.as.d != 0.0;
    if (v.type == SL_BOOL) return v.as.b != 0;
    return 1;
}
static int both_int(SLValue a, SLValue b) { return a.type == SL_INT && b.type == SL_INT; }

static int socket_alloc(SLVM* vm, SLNetHandle handle) {
    pthread_mutex_lock(&vm->sock_mtx);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) if (!vm->sockets[i].active) {
        pthread_mutex_lock(&vm->sockets[i].mtx); vm->sockets[i].handle = handle; vm->sockets[i].active = 1;
        pthread_mutex_unlock(&vm->sockets[i].mtx); pthread_mutex_unlock(&vm->sock_mtx); return i;
    }
    pthread_mutex_unlock(&vm->sock_mtx); return -1;
}
static int socket_valid_handle(int h) { return h >= 0 && h < SL_MAX_SOCKETS; }
static SLNetHandle socket_handle_locked(SLSocket* s) { return s->active ? s->handle : SL_NET_INVALID; }

/* ---- Synchro probe table (VM-owned, bounded) ----------------------------- */
static int synchro_alloc(SLVM* vm, SLSynchro* s) {
    pthread_mutex_lock(&vm->synchro_mtx);
    for (int i = 0; i < SL_SYNCHRO_MAX; i++) if (!vm->synchro[i]) {
        vm->synchro[i] = s; pthread_mutex_unlock(&vm->synchro_mtx); return i;
    }
    pthread_mutex_unlock(&vm->synchro_mtx); return -1;
}
static SLSynchro* synchro_get(SLVM* vm, int h) {
    if (h < 0 || h >= SL_SYNCHRO_MAX) return NULL;
    pthread_mutex_lock(&vm->synchro_mtx); SLSynchro* s = vm->synchro[h]; pthread_mutex_unlock(&vm->synchro_mtx); return s;
}
static void synchro_release(SLVM* vm, int h) {
    if (h < 0 || h >= SL_SYNCHRO_MAX) return;
    pthread_mutex_lock(&vm->synchro_mtx); SLSynchro* s = vm->synchro[h]; vm->synchro[h] = NULL; pthread_mutex_unlock(&vm->synchro_mtx);
    if (s) slsynchro_close(s);
}

/* ---- Munction reach table (VM-owned, bounded) ---------------------------- */
static int munction_alloc(SLVM* vm, SLMunction* m) {
    pthread_mutex_lock(&vm->munction_mtx);
    for (int i = 0; i < SL_MUNCTION_MAX; i++) if (!vm->munction[i]) {
        vm->munction[i] = m; pthread_mutex_unlock(&vm->munction_mtx); return i;
    }
    pthread_mutex_unlock(&vm->munction_mtx); return -1;
}
static SLMunction* munction_get(SLVM* vm, int h) {
    if (h < 0 || h >= SL_MUNCTION_MAX) return NULL;
    pthread_mutex_lock(&vm->munction_mtx); SLMunction* m = vm->munction[h]; pthread_mutex_unlock(&vm->munction_mtx); return m;
}
static void munction_clear_slot(SLVM* vm, int h) {
    if (h < 0 || h >= SL_MUNCTION_MAX) return;
    pthread_mutex_lock(&vm->munction_mtx); vm->munction[h] = NULL; pthread_mutex_unlock(&vm->munction_mtx);
}

/* ---- Best-of selector table (VM-owned, bounded) -------------------------- */
static int bestof_alloc(SLVM* vm, SLBestOf* b) {
    pthread_mutex_lock(&vm->bestof_mtx);
    for (int i = 0; i < SL_BESTOF_MAX; i++) if (!vm->bestof[i]) {
        vm->bestof[i] = b; pthread_mutex_unlock(&vm->bestof_mtx); return i;
    }
    pthread_mutex_unlock(&vm->bestof_mtx); return -1;
}
static SLBestOf* bestof_get(SLVM* vm, int h) {
    if (h < 0 || h >= SL_BESTOF_MAX) return NULL;
    pthread_mutex_lock(&vm->bestof_mtx); SLBestOf* b = vm->bestof[h]; pthread_mutex_unlock(&vm->bestof_mtx); return b;
}
static void bestof_release(SLVM* vm, int h) {
    if (h < 0 || h >= SL_BESTOF_MAX) return;
    pthread_mutex_lock(&vm->bestof_mtx); SLBestOf* b = vm->bestof[h]; vm->bestof[h] = NULL; pthread_mutex_unlock(&vm->bestof_mtx);
    if (b) slbestof_close(b);
}

static int file_alloc(SLVM* vm, SLIOHandle fd) {
    pthread_mutex_lock(&vm->file_mtx);
    for (int i = 0; i < SL_MAX_FILES; i++) if (!vm->files[i].active) {
        pthread_mutex_lock(&vm->files[i].mtx); vm->files[i].fd = fd; vm->files[i].active = 1; vm->files[i].peer = -1;
        pthread_mutex_unlock(&vm->files[i].mtx); pthread_mutex_unlock(&vm->file_mtx); return i;
    }
    pthread_mutex_unlock(&vm->file_mtx); return -1;
}
static int file_valid_handle(int h) { return h >= 0 && h < SL_MAX_FILES; }
static SLIOHandle file_fd_locked(SLFile* f) { return f->active ? f->fd : SLIO_INVALID_HANDLE; }

/* ---- struct instances: the same bounded-handle discipline as sockets/files -- */
static SLValue slval_struct(int32_t h) { SLValue v; v.type = SL_STRUCT; v.as.h = h; return v; }

/* Allocate a zero-initialised instance of struct type `type`. Returns a VM-local
 * handle, or -1 on exhaustion / bad type. The instance store is grown lazily. */
static int struct_alloc(SLVM* vm, int type) {
    if (type < 0 || type >= vm->nstruct_types) return -1;
    pthread_mutex_lock(&vm->struct_mtx);
    if (!vm->structs) {
        vm->structs = (SLStructInstance*)calloc(SL_MAX_STRUCTS, sizeof(SLStructInstance));
        if (!vm->structs) { pthread_mutex_unlock(&vm->struct_mtx); return -1; }
    }
    int nf = vm->struct_types[type].nfields;
    for (int i = 0; i < SL_MAX_STRUCTS; i++) {
        if (!vm->structs[i].active) {
            vm->structs[i].active = 1;
            vm->structs[i].type = type;
            for (int j = 0; j < nf; j++) vm->structs[i].fields[j] = slval_null();
            vm->nstruct_live++;
            pthread_mutex_unlock(&vm->struct_mtx);
            return i;
        }
    }
    pthread_mutex_unlock(&vm->struct_mtx);
    return -1;
}
static int struct_valid_handle(SLVM* vm, int h) {
    return vm->structs && h >= 0 && h < SL_MAX_STRUCTS && vm->structs[h].active;
}

static SLNetHandle make_listener(int port) {
    if (port < 0 || port > 65535) return SL_NET_INVALID;
    return slnet_listen((uint16_t)port, 16);
}
static SLNetHandle make_connection(const char* host, int port) {
    if (!host || port < 0 || port > 65535) return SL_NET_INVALID;
    return slnet_connect(host, (uint16_t)port);
}

static int make_pipe_handles(SLVM* vm) {
    SLIOHandle pr = SLIO_INVALID_HANDLE, pw = SLIO_INVALID_HANDLE;
    if (slio_pipe(&pr, &pw) != 0) return -1;
    int rh = file_alloc(vm, pr);
    if (rh < 0) { slio_close(pr); slio_close(pw); return -1; }
    int wh = file_alloc(vm, pw);
    if (wh < 0) {
        pthread_mutex_lock(&vm->files[rh].mtx); close_file_slot(&vm->files[rh]); pthread_mutex_unlock(&vm->files[rh].mtx);
        slio_close(pw); return -1;
    }
    pthread_mutex_lock(&vm->files[rh].mtx); vm->files[rh].peer = wh; pthread_mutex_unlock(&vm->files[rh].mtx);
    pthread_mutex_lock(&vm->files[wh].mtx); vm->files[wh].peer = rh; pthread_mutex_unlock(&vm->files[wh].mtx);
    return rh;
}

static int make_fifo(const char* path, unsigned mode) {
    if (!path || !*path) return -1;
    /* slio maps this to a Linux FIFO or a Windows named pipe; it treats an
     * already-existing endpoint as success. */
    return slio_named_pipe_create(path, mode) == 0 ? 0 : -1;
}

static int open_file_handle(SLVM* vm, const char* path, const char* mode) {
    if (!path || !mode) return -1;
    /* slio_open takes the same r/w/a/rw/r+/w+/a+ (+optional n) mode strings
     * the language exposes, and returns an OS-neutral handle. */
    SLIOHandle fd = slio_open(path, mode);
    if (fd == SLIO_INVALID_HANDLE) return -1;
    int h = file_alloc(vm, fd);
    if (h < 0) slio_close(fd);
    return h;
}

/* Render one scalar SLValue into buf (used by print, string concat, and JSON
 * packing). Struct handles render as "TypeName#handle". */
static void value_to_text(SLVM* vm, SLValue v, char* buf, size_t cap) {
    switch (v.type) {
        case SL_NULL:   snprintf(buf, cap, "null"); break;
        case SL_INT:    snprintf(buf, cap, "%lld", (long long)v.as.i); break;
        case SL_DOUBLE: snprintf(buf, cap, "%g", v.as.d); break;
        case SL_BOOL:   snprintf(buf, cap, "%s", v.as.b ? "true" : "false"); break;
        case SL_STR:    snprintf(buf, cap, "%s", slvm_str(vm, v.as.s)); break;
        case SL_STRUCT: {
            const char* tn = (vm->structs && struct_valid_handle(vm, v.as.h))
                ? vm->struct_types[vm->structs[v.as.h].type].name : "struct";
            snprintf(buf, cap, "%s#%d", tn, v.as.h);
        } break;
    }
}

static void print_value(SLVM* vm, SLValue v) {
    char buf[1024];
    value_to_text(vm, v, buf, sizeof(buf));
    printf("%s", buf);
}

/* Append a JSON-escaped copy of `s` (without surrounding quotes) to out/cap. */
static void json_escape_into(char* out, size_t cap, const char* s) {
    size_t n = strlen(out);
    for (; *s && n + 2 < cap; s++) {
        char c = *s;
        if (c == '"' || c == '\\') { out[n++] = '\\'; out[n++] = c; }
        else if (c == '\n') { out[n++] = '\\'; if (n + 1 < cap) out[n++] = 'n'; }
        else if (c == '\t') { out[n++] = '\\'; if (n + 1 < cap) out[n++] = 't'; }
        else out[n++] = c;
    }
    out[n] = 0;
}

/* Serialize a struct instance to a compact JSON object:
 * {"__type":"Name","field":<value>,...}. Scalar fields become JSON scalars;
 * string fields are quoted+escaped; nested struct fields become their handle
 * integer (transport of nested graphs is left to the caller). */
static void struct_to_json(SLVM* vm, int h, char* out, size_t cap) {
    out[0] = 0;
    if (!struct_valid_handle(vm, h)) { snprintf(out, cap, "null"); return; }
    SLStructInstance* si = &vm->structs[h];
    SLStructType* st = &vm->struct_types[si->type];
    strncat(out, "{\"__type\":\"", cap - strlen(out) - 1);
    json_escape_into(out, cap, st->name);
    strncat(out, "\"", cap - strlen(out) - 1);
    for (int i = 0; i < st->nfields; i++) {
        strncat(out, ",\"", cap - strlen(out) - 1);
        json_escape_into(out, cap, st->fields[i]);
        strncat(out, "\":", cap - strlen(out) - 1);
        SLValue fv = si->fields[i];
        if (fv.type == SL_STR) {
            strncat(out, "\"", cap - strlen(out) - 1);
            json_escape_into(out, cap, slvm_str(vm, fv.as.s));
            strncat(out, "\"", cap - strlen(out) - 1);
        } else {
            char tmp[256]; value_to_text(vm, fv, tmp, sizeof(tmp));
            strncat(out, tmp, cap - strlen(out) - 1);
        }
    }
    strncat(out, "}", cap - strlen(out) - 1);
}

/* Minimal, forgiving JSON reader for struct_from_json: locate the value token
 * for "key" in a flat object and store it into *out as int/double/bool/string.
 * Returns 1 on success. Not a general JSON parser -- it matches the shape that
 * struct_to_json emits (flat, ordered, no nested objects). */
static int json_find_scalar(SLVM* vm, const char* json, const char* key, SLValue* out) {
    char pat[SL_MAX_STRUCT_FIELDS + 8];
    snprintf(pat, sizeof(pat), "\"%s\"", key);
    const char* p = strstr(json, pat);
    if (!p) return 0;
    p = strchr(p + strlen(pat), ':');
    if (!p) return 0;
    p++;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '"') {
        p++;
        char buf[1024]; size_t n = 0;
        while (*p && *p != '"' && n + 1 < sizeof(buf)) {
            if (*p == '\\' && p[1]) {
                p++;
                char c = *p;
                buf[n++] = (c == 'n') ? '\n' : (c == 't') ? '\t' : c;
            } else buf[n++] = *p;
            p++;
        }
        buf[n] = 0;
        SLValue v; v.type = SL_STR; v.as.s = intern(vm, buf); *out = v; return 1;
    }
    if (strncmp(p, "true", 4) == 0) { *out = slval_bool(1); return 1; }
    if (strncmp(p, "false", 5) == 0) { *out = slval_bool(0); return 1; }
    if (strncmp(p, "null", 4) == 0) { *out = slval_null(); return 1; }
    /* number: integer unless it contains '.', 'e', or 'E' */
    int is_double = 0;
    for (const char* q = p; *q && *q != ',' && *q != '}'; q++)
        if (*q == '.' || *q == 'e' || *q == 'E') { is_double = 1; break; }
    if (is_double) *out = slval_double(strtod(p, NULL));
    else *out = slval_int((int64_t)strtoll(p, NULL, 10));
    return 1;
}

static SLResult run_thread(SLThread* t);
static void* thread_trampoline(void* arg) { run_thread((SLThread*)arg); return NULL; }

static int spawn_thread(SLVM* vm, int fi, const SLValue* args, int nargs) {
    if (fi < 0 || fi >= vm->nfunc) return -1;
    pthread_mutex_lock(&vm->thr_mtx);
    if (vm->nthreads >= SL_MAX_THREADS) { pthread_mutex_unlock(&vm->thr_mtx); return -1; }
    SLThread* t = (SLThread*)calloc(1, sizeof(SLThread)); if (!t) { pthread_mutex_unlock(&vm->thr_mtx); return -1; }
    t->vm = vm; t->entry_func = fi; t->result = slval_null();
    SLFunc* f = &vm->funcs[fi]; int n = nargs; if (n > f->nlocals) n = f->nlocals;
    for (int i = 0; i < n; i++) t->stack[t->sp++] = args[i];
    for (int i = n; i < f->nlocals; i++) t->stack[t->sp++] = slval_null();
    t->frames[0].ret_ip = -1; t->frames[0].base = 0; t->frames[0].nargs = f->nargs; t->fp = 1;
    int id = vm->nthreads; vm->threads[id] = t; vm->nthreads++;
    pthread_mutex_unlock(&vm->thr_mtx);
    if (pthread_create(&t->handle, NULL, thread_trampoline, t) != 0) {
        pthread_mutex_lock(&vm->thr_mtx); vm->threads[id] = NULL; vm->nthreads--; pthread_mutex_unlock(&vm->thr_mtx);
        free(t); return -1;
    }
    t->started = 1; return id;
}

void slvm_joinall(SLVM* vm) {
    for (;;) {
        pthread_mutex_lock(&vm->thr_mtx);
        SLThread* t = NULL; int idx = -1;
        for (int i = 0; i < vm->nthreads; i++) if (vm->threads[i]) { t = vm->threads[i]; idx = i; break; }
        pthread_mutex_unlock(&vm->thr_mtx);
        if (!t) break;
        if (t->started) pthread_join(t->handle, NULL);
        pthread_mutex_lock(&vm->thr_mtx); vm->threads[idx] = NULL; pthread_mutex_unlock(&vm->thr_mtx);
        free(t);
    }
    pthread_mutex_lock(&vm->thr_mtx); vm->nthreads = 0; pthread_mutex_unlock(&vm->thr_mtx);
}

#define TERR(msg) do { strncpy(t->err, (msg), sizeof(t->err)-1); t->err[sizeof(t->err)-1]=0; set_err(vm,(msg)); return SLR_ERROR; } while (0)
#define PUSH(v) do { if (t->sp >= STACK_MAX) TERR("stack overflow"); t->stack[t->sp++] = (v); } while (0)
#define POP() (t->stack[--t->sp])

static SLResult run_thread(SLThread* t) {
    SLVM* vm = t->vm; int fi = t->entry_func;
    if (fi < 0 || fi >= vm->nfunc) TERR("no entry function set");
    SLFunc* ef = &vm->funcs[fi];
    if (t->fp == 0) {
        t->sp = 0; t->frames[0].ret_ip = -1; t->frames[0].base = 0; t->frames[0].nargs = ef->nargs; t->fp = 1;
        for (int i = 0; i < ef->nlocals; i++) PUSH(slval_null());
    }
    int ip = ef->entry;
    for (;;) {
        if (ip < 0 || ip >= vm->codelen) TERR("ip out of range");
        SLInstr in = vm->code[ip++];
        switch (in.op) {
        case OP_NOP: break;
        case OP_HALT: return SLR_HALT;
        case OP_CONST: PUSH(vm->consts[in.a]); break;
        case OP_POP: (void)POP(); break;
        case OP_DUP: { SLValue dupv = t->stack[t->sp-1]; PUSH(dupv); } break;
        case OP_LOADG: { pthread_mutex_lock(&vm->global_mtx); SLValue v=vm->globals[in.a]; pthread_mutex_unlock(&vm->global_mtx); PUSH(v); } break;
        case OP_STOREG: { SLValue v=POP(); pthread_mutex_lock(&vm->global_mtx); vm->globals[in.a]=v; pthread_mutex_unlock(&vm->global_mtx); } break;
        case OP_LOADL: { SLFrame* f=&t->frames[t->fp-1]; PUSH(t->stack[f->base+in.a]); } break;
        case OP_STOREL: { SLFrame* f=&t->frames[t->fp-1]; t->stack[f->base+in.a]=POP(); } break;
        case OP_ADD: {
            SLValue b=POP(), a=POP();
            if (a.type==SL_STR || b.type==SL_STR) {
                char buf[1024], sa[512], sb[512];
                value_to_text(vm,a,sa,sizeof(sa));
                value_to_text(vm,b,sb,sizeof(sb));
                snprintf(buf,sizeof(buf),"%s%s",sa,sb); SLValue r; r.type=SL_STR; r.as.s=intern(vm,buf); PUSH(r);
            } else if (both_int(a,b)) PUSH(slval_int(a.as.i+b.as.i)); else PUSH(slval_double(as_num(a)+as_num(b)));
        } break;
        case OP_SUB: { SLValue b=POP(),a=POP(); if(both_int(a,b)) PUSH(slval_int(a.as.i-b.as.i)); else PUSH(slval_double(as_num(a)-as_num(b))); } break;
        case OP_MUL: { SLValue b=POP(),a=POP(); if(both_int(a,b)) PUSH(slval_int(a.as.i*b.as.i)); else PUSH(slval_double(as_num(a)*as_num(b))); } break;
        case OP_DIV: { SLValue b=POP(),a=POP(); if(both_int(a,b)){if(!b.as.i) TERR("integer divide by zero"); PUSH(slval_int(a.as.i/b.as.i));} else PUSH(slval_double(as_num(a)/as_num(b))); } break;
        case OP_MOD: { SLValue b=POP(),a=POP(); if(both_int(a,b)){ if(!b.as.i) TERR("integer modulo by zero"); PUSH(slval_int(a.as.i%b.as.i)); } else { double db=as_num(b); if(db==0.0) TERR("modulo by zero"); PUSH(slval_double(fmod(as_num(a),db))); } } break;
        case OP_NEG: { SLValue a=POP(); if(a.type==SL_INT) PUSH(slval_int(-a.as.i)); else PUSH(slval_double(-as_num(a))); } break;
        case OP_EQ: { SLValue b=POP(),a=POP(); int e; if(a.type==SL_STRUCT||b.type==SL_STRUCT) e=(a.type==SL_STRUCT&&b.type==SL_STRUCT&&a.as.h==b.as.h); else if(a.type==SL_STR&&b.type==SL_STR) e=(a.as.s==b.as.s); else e=(as_num(a)==as_num(b)); PUSH(slval_bool(e)); } break;
        case OP_NE: { SLValue b=POP(),a=POP(); int e; if(a.type==SL_STRUCT||b.type==SL_STRUCT) e=!(a.type==SL_STRUCT&&b.type==SL_STRUCT&&a.as.h==b.as.h); else if(a.type==SL_STR&&b.type==SL_STR) e=(a.as.s!=b.as.s); else e=(as_num(a)!=as_num(b)); PUSH(slval_bool(e)); } break;
        case OP_LT: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a)<as_num(b))); } break;
        case OP_LE: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a)<=as_num(b))); } break;
        case OP_GT: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a)>as_num(b))); } break;
        case OP_GE: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a)>=as_num(b))); } break;
        case OP_AND: { SLValue b=POP(),a=POP(); PUSH(slval_bool(is_truthy(a)&&is_truthy(b))); } break;
        case OP_OR: { SLValue b=POP(),a=POP(); PUSH(slval_bool(is_truthy(a)||is_truthy(b))); } break;
        case OP_NOT: { SLValue a=POP(); PUSH(slval_bool(!is_truthy(a))); } break;
        case OP_JMP: ip=in.a; break;
        case OP_JMPF: { SLValue c=POP(); if(!is_truthy(c)) ip=in.a; } break;
        case OP_CALL: {
            int cfi=in.a; if(cfi<0||cfi>=vm->nfunc) TERR("call to unknown function");
            SLFunc* f=&vm->funcs[cfi]; if(t->fp>=FRAMES_MAX) TERR("call depth exceeded");
            int base=t->sp-f->nargs; SLFrame* nf=&t->frames[t->fp++]; nf->ret_ip=ip; nf->base=base; nf->nargs=f->nargs;
            for(int i=f->nargs;i<f->nlocals;i++) PUSH(slval_null()); ip=f->entry;
        } break;
        case OP_RET: {
            SLValue rv=(t->sp>t->frames[t->fp-1].base)?POP():slval_null(); SLFrame f=t->frames[--t->fp]; t->sp=f.base;
            if(f.ret_ip<0){t->result=rv; return SLR_OK;} PUSH(rv); ip=f.ret_ip;
        } break;
        case OP_PRINT: { SLValue v=POP(); pthread_mutex_lock(&vm->print_mtx); print_value(vm,v); printf("\n"); pthread_mutex_unlock(&vm->print_mtx); } break;
        case OP_SPAWN: {
            int sfi=in.a; if(sfi<0||sfi>=vm->nfunc) TERR("spawn of unknown function"); int nargs=vm->funcs[sfi].nargs; if(t->sp<nargs) TERR("spawn: not enough arguments on stack");
            SLValue args[FRAMES_MAX]; for(int i=0;i<nargs;i++) args[i]=t->stack[t->sp-nargs+i]; t->sp-=nargs; PUSH(slval_int(spawn_thread(vm,sfi,args,nargs)));
        } break;
        case OP_JOINALL: slvm_joinall(vm); break;
        case OP_LOCK: if(in.a<0||in.a>=SL_MAX_LOCKS) TERR("lock id out of range"); pthread_mutex_lock(&vm->locks[in.a]); break;
        case OP_UNLOCK: if(in.a<0||in.a>=SL_MAX_LOCKS) TERR("lock id out of range"); pthread_mutex_unlock(&vm->locks[in.a]); break;
        case OP_SEND: {
            if(in.a<0||in.a>=SL_MAX_LOCKS) TERR("mailbox slot out of range"); SLMailbox* mb=&vm->mailbox[in.a]; SLValue v=POP();
            pthread_mutex_lock(&mb->mtx); while(mb->has) pthread_cond_wait(&mb->cond,&mb->mtx); mb->has=1; mb->tag=in.a; mb->value=v; pthread_cond_broadcast(&mb->cond); pthread_mutex_unlock(&mb->mtx);
        } break;
        case OP_RECV: {
            if(in.a<0||in.a>=SL_MAX_LOCKS) TERR("mailbox slot out of range"); SLMailbox* mb=&vm->mailbox[in.a]; pthread_mutex_lock(&mb->mtx);
            while(!mb->has) pthread_cond_wait(&mb->cond,&mb->mtx); SLValue v=mb->value; mb->has=0; pthread_cond_broadcast(&mb->cond); pthread_mutex_unlock(&mb->mtx); PUSH(v);
        } break;

        case OP_LISTEN: {
            SLValue pv=POP(); if(pv.type!=SL_INT) TERR("listen(port) requires an integer port"); SLNetHandle handle=make_listener((int)pv.as.i); if(handle==SL_NET_INVALID){PUSH(slval_int(-1));break;} int h=socket_alloc(vm,handle); if(h<0){slnet_close(handle);PUSH(slval_int(-1));break;} PUSH(slval_int(h));
        } break;
        case OP_ACCEPT: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!socket_valid_handle((int)hv.as.i)) TERR("accept(socket) requires a valid socket handle"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); SLNetHandle handle=socket_handle_locked(s); if(handle==SL_NET_INVALID){pthread_mutex_unlock(&s->mtx);PUSH(slval_int(-1));break;} SLNetHandle child=slnet_accept(handle); pthread_mutex_unlock(&s->mtx); if(child==SL_NET_INVALID){PUSH(slval_int(-1));break;} int h=socket_alloc(vm,child); if(h<0){slnet_close(child);PUSH(slval_int(-1));break;} PUSH(slval_int(h));
        } break;
        case OP_CONNECT: {
            SLValue portv=POP(),hostv=POP(); if(hostv.type!=SL_STR||portv.type!=SL_INT) TERR("connect(host, port) requires a String and integer port"); SLNetHandle handle=make_connection(slvm_str(vm,hostv.as.s),(int)portv.as.i); if(handle==SL_NET_INVALID){PUSH(slval_int(-1));break;} int h=socket_alloc(vm,handle); if(h<0){slnet_close(handle);PUSH(slval_int(-1));break;} PUSH(slval_int(h));
        } break;
        case OP_SOCKREAD: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!socket_valid_handle((int)hv.as.i)) TERR("sockread(socket) requires a valid socket handle"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); SLNetHandle handle=socket_handle_locked(s); if(handle==SL_NET_INVALID){pthread_mutex_unlock(&s->mtx);PUSH(slval_int(-1));break;} char buf[4097]; SLNetCount n=slnet_read(handle,buf,sizeof(buf)-1); pthread_mutex_unlock(&s->mtx); if(n<=0){SLValue e; e.type=SL_STR;e.as.s=intern(vm,"");PUSH(e);break;} buf[n]=0; SLValue out;out.type=SL_STR;out.as.s=intern(vm,buf);PUSH(out);
        } break;
        case OP_SOCKWRITE: {
            SLValue sv=POP(),hv=POP(); if(hv.type!=SL_INT||sv.type!=SL_STR||!socket_valid_handle((int)hv.as.i)) TERR("sockwrite(socket, string) requires a socket handle and String"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); SLNetHandle handle=socket_handle_locked(s); if(handle==SL_NET_INVALID){pthread_mutex_unlock(&s->mtx);PUSH(slval_int(-1));break;} const char* data=slvm_str(vm,sv.as.s); SLNetCount n=slnet_write(handle,data,strlen(data)); pthread_mutex_unlock(&s->mtx); PUSH(slval_int(n<0?-1:(int64_t)n));
        } break;
        case OP_SOCKCLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!socket_valid_handle((int)hv.as.i)) TERR("sockclose(socket) requires a valid socket handle"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); if(s->active){slnet_close(s->handle);s->handle=SL_NET_INVALID;s->active=0;} pthread_mutex_unlock(&s->mtx); PUSH(slval_null());
        } break;

        /* ---- Synchro: honest packet dispatch + latency measurement ---- */
        case OP_SYN_OPEN: {
            SLValue portv=POP(), hostv=POP();
            if(hostv.type!=SL_STR||portv.type!=SL_INT) TERR("synchroOpen(host, port) requires a String and integer port");
            SLSynchro* s=slsynchro_open(slvm_str(vm,hostv.as.s),(uint16_t)portv.as.i);
            if(!s){PUSH(slval_int(-1));break;}
            int h=synchro_alloc(vm,s); if(h<0){slsynchro_close(s);PUSH(slval_int(-1));break;}
            PUSH(slval_int(h));
        } break;
        case OP_SYN_DISPATCH: {
            SLValue tov=POP(), lenv=POP(), hv=POP();
            if(hv.type!=SL_INT||lenv.type!=SL_INT||tov.type!=SL_INT) TERR("synchroDispatch(handle, len, timeoutMs) requires three integers");
            SLSynchro* s=synchro_get(vm,(int)hv.as.i); if(!s){PUSH(slval_int(-1));break;}
            PUSH(slval_int(slsynchro_dispatch(s,(size_t)(lenv.as.i<0?0:lenv.as.i),(int)tov.as.i)));
        } break;
        case OP_SYN_STAT: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("synchro stat requires a handle");
            SLSynchro* s=synchro_get(vm,(int)hv.as.i); if(!s){PUSH(slval_int(-1));break;}
            int64_t r=-1;
            switch(in.a){
                case SL_SYN_STAT_SENT: r=slsynchro_sent(s); break;
                case SL_SYN_STAT_RECV: r=slsynchro_received(s); break;
                case SL_SYN_STAT_MEAN: r=slsynchro_mean_us(s); break;
                case SL_SYN_STAT_MIN:  r=slsynchro_min_us(s); break;
                case SL_SYN_STAT_MAX:  r=slsynchro_max_us(s); break;
                case SL_SYN_STAT_P95:  r=slsynchro_percentile_us(s,95); break;
                case SL_SYN_STAT_LOSS: r=slsynchro_loss_permille(s); break;
                default: r=-1; break;
            }
            PUSH(slval_int(r));
        } break;
        case OP_SYN_REPORT: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("synchroReport(handle) requires a handle");
            SLSynchro* s=synchro_get(vm,(int)hv.as.i);
            char buf[256]; if(s) slsynchro_report(s,buf,sizeof(buf)); else buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_SYN_CLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("synchroClose(handle) requires a handle");
            synchro_release(vm,(int)hv.as.i); PUSH(slval_null());
        } break;

        /* ---- Munction: reach-composition sentence --------------------- */
        case OP_MUN_START: {
            SLValue nv=POP(); if(nv.type!=SL_STR) TERR("Munction.start(name) requires a String name");
            SLMunction* m=slmunction_start(slvm_str(vm,nv.as.s));
            if(!m){PUSH(slval_int(-1));break;}
            int h=munction_alloc(vm,m); if(h<0){char sink[8];slmunction_close(m,sink,sizeof(sink));PUSH(slval_int(-1));break;}
            PUSH(slval_int(h));
        } break;
        case OP_MUN_CONNECT: {
            SLValue uv=POP(), hv=POP(); if(hv.type!=SL_INT||uv.type!=SL_STR) TERR("Munction connect(uri) requires a reach handle and a String URI");
            SLMunction* m=munction_get(vm,(int)hv.as.i); if(m) slmunction_connect(m,slvm_str(vm,uv.as.s)); PUSH(hv);
        } break;
        case OP_MUN_ENABLE: {
            SLValue pv=POP(), hv=POP(); if(hv.type!=SL_INT||pv.type!=SL_STR) TERR("Munction enable(policy) requires a reach handle and a String policy");
            SLMunction* m=munction_get(vm,(int)hv.as.i); if(m) slmunction_enable(m,slvm_str(vm,pv.as.s)); PUSH(hv);
        } break;
        case OP_MUN_SEND: {
            SLValue dv=POP(), hv=POP(); if(hv.type!=SL_INT||dv.type!=SL_STR) TERR("Munction send(datum) requires a reach handle and a String datum");
            SLMunction* m=munction_get(vm,(int)hv.as.i); if(m){const char* d=slvm_str(vm,dv.as.s); slmunction_send(m,d,strlen(d));} PUSH(hv);
        } break;
        case OP_MUN_THATCH: {
            SLValue sv=POP(), hv=POP(); if(hv.type!=SL_INT||sv.type!=SL_STR) TERR("Munction thatch(interims) requires a reach handle and a String spec");
            SLMunction* m=munction_get(vm,(int)hv.as.i); if(m) slmunction_thatch(m,slvm_str(vm,sv.as.s)); PUSH(hv);
        } break;
        case OP_MUN_CONSUME: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("Munction consume() requires a reach handle");
            SLMunction* m=munction_get(vm,(int)hv.as.i); if(m) slmunction_consume(m); PUSH(hv);
        } break;
        case OP_MUN_LATCH: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("Munction latch() requires a reach handle");
            SLMunction* m=munction_get(vm,(int)hv.as.i); if(m) slmunction_latch(m); PUSH(hv);
        } break;
        case OP_MUN_RECEPTION: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("Munction reception() requires a reach handle");
            SLMunction* m=munction_get(vm,(int)hv.as.i);
            char buf[2048]; if(m) slmunction_last_reception(m,buf,sizeof(buf)); else buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_MUN_CLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("Munction closeWithReceipt() requires a reach handle");
            int h=(int)hv.as.i; SLMunction* m=munction_get(vm,h);
            char receipt[512];
            if(m){ slmunction_close(m,receipt,sizeof(receipt)); munction_clear_slot(vm,h); }
            else receipt[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,receipt); PUSH(out);
        } break;

        /* ---- Best-of: configurable route/accuracy selection ---------- */
        case OP_BEST_NEW: {
            SLBestOf* b=slbestof_new(); if(!b){PUSH(slval_int(-1));break;}
            int h=bestof_alloc(vm,b); if(h<0){slbestof_close(b);PUSH(slval_int(-1));break;}
            PUSH(slval_int(h));
        } break;
        case OP_BEST_WEIGHT: {
            SLValue wv=POP(), av=POP(), hv=POP();
            if(hv.type!=SL_INT||av.type!=SL_INT||wv.type!=SL_INT) TERR("bestOfWeight(handle, axis, weight) requires three integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); if(b) slbestof_weight(b,(int)av.as.i,(int)wv.as.i); PUSH(hv);
        } break;
        case OP_BEST_MINVER: {
            SLValue vv=POP(), hv=POP(); if(hv.type!=SL_INT||vv.type!=SL_INT) TERR("bestOfMinVersion(handle, minVersion) requires two integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); if(b) slbestof_min_version(b,(int)vv.as.i); PUSH(hv);
        } break;
        case OP_BEST_BUDGET: {
            SLValue cv=POP(), hv=POP(); if(hv.type!=SL_INT||cv.type!=SL_INT) TERR("bestOfCostBudget(handle, budget) requires two integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); if(b) slbestof_cost_budget(b,(int)cv.as.i); PUSH(hv);
        } break;
        case OP_BEST_CAND: {
            /* stack (top last): name, route, timeout, payload, gap, flags, version, cost, replays over handle */
            SLValue replaysv=POP(), costv=POP(), versionv=POP(), flagsv=POP(), gapv=POP(),
                    payloadv=POP(), timeoutv=POP(), routev=POP(), namev=POP(), hv=POP();
            if(hv.type!=SL_INT||namev.type!=SL_STR||routev.type!=SL_STR||timeoutv.type!=SL_INT||
               payloadv.type!=SL_INT||gapv.type!=SL_INT||flagsv.type!=SL_INT||versionv.type!=SL_INT||
               costv.type!=SL_INT||replaysv.type!=SL_INT)
                TERR("bestOfCandidate requires (handle, nameStr, routeStr, timeout, payload, gap, flags, version, cost, replays)");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i);
            int idx=b? slbestof_add_candidate(b, slvm_str(vm,namev.as.s), slvm_str(vm,routev.as.s),
                    (int)timeoutv.as.i,(int)payloadv.as.i,(int)gapv.as.i,(int)flagsv.as.i,
                    (int)versionv.as.i,(int)costv.as.i,(int)replaysv.as.i) : -1;
            PUSH(slval_int(idx));
        } break;
        case OP_BEST_RECORD: {
            SLValue rttv=POP(), idxv=POP(), hv=POP();
            if(hv.type!=SL_INT||idxv.type!=SL_INT||rttv.type!=SL_INT) TERR("bestOfRecord(handle, idx, rttUs) requires three integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); if(b) slbestof_record(b,(int)idxv.as.i,rttv.as.i); PUSH(hv);
        } break;
        case OP_BEST_SCORE: {
            SLValue idxv=POP(), hv=POP(); if(hv.type!=SL_INT||idxv.type!=SL_INT) TERR("bestOfScore(handle, idx) requires two integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); PUSH(slval_int(b? slbestof_score(b,(int)idxv.as.i):0));
        } break;
        case OP_BEST_BEST: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("bestOfBest(handle) requires a handle");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); PUSH(slval_int(b? slbestof_best(b):-1));
        } break;
        case OP_BEST_STAT: {
            SLValue idxv=POP(), hv=POP(); if(hv.type!=SL_INT||idxv.type!=SL_INT) TERR("best-of stat requires (handle, idx)");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); int idx=(int)idxv.as.i; int64_t r=-1;
            if(b){ switch(in.a){
                case SL_BEST_STAT_MEAN: r=slbestof_mean_us(b,idx); break;
                case SL_BEST_STAT_LOSS: r=slbestof_loss_permille(b,idx); break;
                case SL_BEST_STAT_JITTER: r=slbestof_jitter_us(b,idx); break;
                case SL_BEST_STAT_CERTAINTY: r=slbestof_certainty_permille(b,idx); break;
                case SL_BEST_STAT_ARCH: r=slbestof_arch(b,idx); break;
                case SL_BEST_STAT_ARCHPARAM: r=slbestof_arch_param(b,idx); break;
                case SL_BEST_STAT_ARCHSTATE: r=slbestof_arch_state(b,idx); break;
                default: r=-1; break; } }
            PUSH(slval_int(r));
        } break;
        case OP_BEST_ARCH: {
            SLValue realv=POP(), paramv=POP(), archv=POP(), idxv=POP(), hv=POP();
            if(hv.type!=SL_INT||idxv.type!=SL_INT||archv.type!=SL_INT||paramv.type!=SL_INT||realv.type!=SL_INT)
                TERR("bestOfCandidateArch(handle, idx, architecture, param, realized) requires five integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i);
            if(b) slbestof_candidate_arch(b,(int)idxv.as.i,(int)archv.as.i,(int)paramv.as.i,(int)realv.as.i);
            PUSH(hv);
        } break;
        case OP_BEST_ARCH_STATE: {
            SLValue realv=POP(), idxv=POP(), hv=POP();
            if(hv.type!=SL_INT||idxv.type!=SL_INT||realv.type!=SL_INT) TERR("bestOfArchRealized(handle, idx, realized) requires three integers");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i);
            if(b) slbestof_arch_realized(b,(int)idxv.as.i,(int)realv.as.i);
            PUSH(hv);
        } break;
        case OP_BEST_CHOICE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("bestOfChoice(handle) requires a handle");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); char buf[512]; if(b) slbestof_choice(b,buf,sizeof(buf)); else buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_BEST_REPORT: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("bestOfReport(handle) requires a handle");
            SLBestOf* b=bestof_get(vm,(int)hv.as.i); char buf[4096]; if(b) slbestof_report(b,buf,sizeof(buf)); else buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_BEST_CLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("bestOfClose(handle) requires a handle");
            bestof_release(vm,(int)hv.as.i); PUSH(slval_null());
        } break;

        /* ---- Linux file I/O ------------------------------------------- */
        case OP_PIPE: {
            int h=make_pipe_handles(vm); PUSH(slval_int(h));
        } break;
        case OP_PIPEPEER: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!file_valid_handle((int)hv.as.i)) TERR("pipePeer(handle) requires a valid pipe handle");
            int h=(int)hv.as.i; pthread_mutex_lock(&vm->files[h].mtx); int p=vm->files[h].peer; pthread_mutex_unlock(&vm->files[h].mtx); PUSH(slval_int(p));
        } break;
        case OP_FIFO_MK: {
            SLValue mv=POP(),pv=POP(); if(pv.type!=SL_STR||mv.type!=SL_INT) TERR("fifoCreate(path, mode) requires a String path and integer mode"); PUSH(slval_int(make_fifo(slvm_str(vm,pv.as.s),(unsigned)mv.as.i)==0?0:-1));
        } break;
        case OP_FILEOPEN: {
            SLValue mv=POP(),pv=POP(); if(pv.type!=SL_STR||mv.type!=SL_STR) TERR("openFile(path, mode) requires String arguments"); PUSH(slval_int(open_file_handle(vm,slvm_str(vm,pv.as.s),slvm_str(vm,mv.as.s))));
        } break;
        case OP_FILEREAD: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!file_valid_handle((int)hv.as.i)) TERR("read(handle) requires a valid file handle"); int h=(int)hv.as.i; SLFile* f=&vm->files[h]; pthread_mutex_lock(&f->mtx); SLIOHandle fd=file_fd_locked(f); if(fd==SLIO_INVALID_HANDLE){pthread_mutex_unlock(&f->mtx);SLValue e;e.type=SL_STR;e.as.s=intern(vm,"");PUSH(e);break;} char buf[4097]; int n=slio_read(fd,buf,sizeof(buf)-1); pthread_mutex_unlock(&f->mtx); if(n<=0){SLValue e;e.type=SL_STR;e.as.s=intern(vm,"");PUSH(e);break;} buf[n]=0; SLValue out;out.type=SL_STR;out.as.s=intern(vm,buf);PUSH(out);
        } break;
        case OP_FILEWRITE: {
            SLValue sv=POP(),hv=POP(); if(hv.type!=SL_INT||sv.type!=SL_STR||!file_valid_handle((int)hv.as.i)) TERR("write(handle, data) requires a file handle and String"); int h=(int)hv.as.i; SLFile* f=&vm->files[h]; pthread_mutex_lock(&f->mtx); SLIOHandle fd=file_fd_locked(f); if(fd==SLIO_INVALID_HANDLE){pthread_mutex_unlock(&f->mtx);PUSH(slval_int(-1));break;} const char* data=slvm_str(vm,sv.as.s); int n=slio_write(fd,data,strlen(data)); pthread_mutex_unlock(&f->mtx); PUSH(slval_int(n<0?-1:(int64_t)n));
        } break;
        case OP_FILECLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!file_valid_handle((int)hv.as.i)) TERR("close(handle) requires a valid file handle"); int h=(int)hv.as.i; pthread_mutex_lock(&vm->files[h].mtx); close_file_slot(&vm->files[h]); pthread_mutex_unlock(&vm->files[h].mtx); PUSH(slval_null());
        } break;
        case OP_FILEUNLINK: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("unlinkFile(path) requires a String path"); PUSH(slval_int(slio_unlink(slvm_str(vm,pv.as.s))==0?0:-1));
        } break;
        case OP_TIME_UTC_MS: {
            PUSH(slval_int(sltime_utc_millis()));
        } break;
        case OP_TIME_UTC_NS: {
            PUSH(slval_int(sltime_utc_nanos()));
        } break;
        case OP_TIME_MONO_NS: {
            PUSH(slval_int((int64_t)sltime_monotonic_nanos()));
        } break;
        case OP_TIME_PRECISION_MS: {
            SLTimeSample ts; if(sltime_sample(&ts)!=0){PUSH(slval_int(-1));break;}
            PUSH(slval_int((int64_t)((ts.uncertainty_us+999ULL)/1000ULL)));
        } break;
        case OP_TIME_LOCATION: {
            { SLValue value; value.type=SL_STR; value.as.s=intern(vm,sltime_location_timezone()); PUSH(value); }
        } break;
        case OP_TIME_HTTP_DATE: {
            char out[64]; if(sltime_http_date(sltime_utc_millis(),out,sizeof(out))!=0) out[0]=0;
            { SLValue value; value.type=SL_STR; value.as.s=intern(vm,out); PUSH(value); }
        } break;
        case OP_TIME_JSON: {
            char out[2048]; SLTimeSample ts;
            if(sltime_sample(&ts)!=0 || sltime_json(&ts,out,sizeof(out))!=0) out[0]=0;
            { SLValue value; value.type=SL_STR; value.as.s=intern(vm,out); PUSH(value); }
        } break;
        case OP_TIME_NTP: {
            SLValue hv=POP(); if(hv.type!=SL_STR) TERR("timeNtp(host) requires a String host");
            char out[2048]; SLTimeSample ts;
            if(sltime_query_ntp(slvm_str(vm,hv.as.s),1500,&ts)!=0 || sltime_json(&ts,out,sizeof(out))!=0) out[0]=0;
            { SLValue value; value.type=SL_STR; value.as.s=intern(vm,out); PUSH(value); }
        } break;
        case OP_TIME_SET_LOCATION: {
            SLValue zv=POP(), cv=POP();
            if(cv.type!=SL_STR || zv.type!=SL_STR) TERR("timeSetLocation(country, timezone) requires two Strings");
            if(sltime_set_location(SL_TIME_LOCATION_COUNTRY,slvm_str(vm,cv.as.s),slvm_str(vm,zv.as.s))!=0) TERR("timeSetLocation failed");
            PUSH(slval_null());
        } break;

        /* ---- struct support ------------------------------------------- */
        case OP_NEWSTRUCT: {
            int h=struct_alloc(vm,in.a);
            if(h<0) TERR("struct instantiation failed (unknown type or store exhausted)");
            PUSH(slval_struct(h));
        } break;
        case OP_GETFIELD: {
            SLValue iv=POP();
            if(iv.type!=SL_STRUCT||!struct_valid_handle(vm,iv.as.h)) TERR("field access on a non-struct value");
            pthread_mutex_lock(&vm->struct_mtx);
            SLStructInstance* si=&vm->structs[iv.as.h];
            if(in.a<0||in.a>=vm->struct_types[si->type].nfields){pthread_mutex_unlock(&vm->struct_mtx);TERR("field offset out of range");}
            SLValue fv=si->fields[in.a];
            pthread_mutex_unlock(&vm->struct_mtx);
            PUSH(fv);
        } break;
        case OP_SETFIELD: {
            SLValue val=POP(), iv=POP();
            if(iv.type!=SL_STRUCT||!struct_valid_handle(vm,iv.as.h)) TERR("field assignment on a non-struct value");
            pthread_mutex_lock(&vm->struct_mtx);
            SLStructInstance* si=&vm->structs[iv.as.h];
            if(in.a<0||in.a>=vm->struct_types[si->type].nfields){pthread_mutex_unlock(&vm->struct_mtx);TERR("field offset out of range");}
            si->fields[in.a]=val;
            pthread_mutex_unlock(&vm->struct_mtx);
            PUSH(val);
        } break;
        case OP_STRUCTPACK: {
            SLValue iv=POP();
            if(iv.type!=SL_STRUCT||!struct_valid_handle(vm,iv.as.h)) TERR("structPack requires a struct value");
            char out[4096];
            pthread_mutex_lock(&vm->struct_mtx);
            struct_to_json(vm,iv.as.h,out,sizeof(out));
            pthread_mutex_unlock(&vm->struct_mtx);
            SLValue s; s.type=SL_STR; s.as.s=intern(vm,out); PUSH(s);
        } break;
        case OP_STRUCTUNPACK: {
            SLValue sv=POP();
            if(sv.type!=SL_STR) TERR("structUnpack requires a JSON String");
            int type=in.a;
            int h=struct_alloc(vm,type);
            if(h<0) TERR("structUnpack: instantiation failed");
            const char* json=slvm_str(vm,sv.as.s);
            pthread_mutex_lock(&vm->struct_mtx);
            SLStructType* st=&vm->struct_types[type];
            for(int i=0;i<st->nfields;i++){ SLValue fv; if(json_find_scalar(vm,json,st->fields[i],&fv)) vm->structs[h].fields[i]=fv; }
            pthread_mutex_unlock(&vm->struct_mtx);
            PUSH(slval_struct(h));
        } break;
        default: TERR("illegal opcode");
        }
    }
}

SLResult slvm_run(SLVM* vm) {
    if (!vm || vm->entry < 0 || vm->entry >= vm->nfunc) { if(vm) set_err(vm,"no entry function set"); return SLR_ERROR; }
    vm->err[0]=0; SLThread* t=(SLThread*)calloc(1,sizeof(SLThread)); if(!t){set_err(vm,"out of memory");return SLR_ERROR;}
    t->vm=vm;t->entry_func=vm->entry;t->result=slval_null(); SLResult r=run_thread(t); vm->last_result=t->result; free(t); slvm_joinall(vm); return r;
}

SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg) {
    if(!vm||!arg) return SLR_ERROR;
    switch(op){
    case SLX_RESET: vm->codelen=vm->nconst=vm->nglobal=vm->nfunc=0;vm->cur_func=-1;vm->entry=-1;vm->err[0]=0;return SLR_OK;
    case SLX_ADD_CONST: arg->out=add_const(vm,arg->value);return SLR_OK;
    case SLX_DECLARE_GLOBAL: arg->out=slvm_declare_global(vm,arg->name);return SLR_OK;
    case SLX_DECLARE_STRUCT: arg->out=slvm_declare_struct(vm,arg->name,arg->names,arg->i0);return SLR_OK;
    case SLX_BEGIN_FUNC: arg->out=slvm_begin_func(vm,arg->name,arg->i0,arg->i1);return SLR_OK;
    case SLX_END_FUNC: slvm_end_func(vm);return SLR_OK;
    case SLX_EMIT: arg->out=slvm_emit(vm,(SLOp)arg->op,arg->a);return SLR_OK;
    case SLX_PATCH: slvm_patch(vm,arg->a,arg->i0);return SLR_OK;
    case SLX_HERE: arg->out=slvm_here(vm);return SLR_OK;
    case SLX_SET_ENTRY: slvm_set_entry(vm,arg->a);return SLR_OK;
    case SLX_RUN: {SLResult r=slvm_run(vm);arg->value=vm->last_result;return r;}
    case SLX_GET_RESULT: arg->value=vm->last_result;return SLR_OK;
    default: set_err(vm,"unknown exchange op");return SLR_ERROR;
    }
}
