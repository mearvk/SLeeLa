/* ==========================================================================
 * sleela_core.c -- Sleela execution core.
 *
 * Stack VM with threads, TCP sockets, Linux file descriptors, anonymous
 * pipes, and filesystem FIFOs. File descriptors are represented by VM-local
 * bounded handles so Sleela code never receives raw host descriptor numbers.
 * ========================================================================== */
#define _POSIX_C_SOURCE 200809L
#include "sleela_core.h"
#include "sleela_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

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
    int fd;
} SLSocket;

typedef struct {
    pthread_mutex_t mtx;
    int active;
    int fd;
    int peer;
} SLFile;

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
    SLSocket sockets[SL_MAX_SOCKETS];
    SLFile files[SL_MAX_FILES];

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
    pthread_mutex_init(&vm->thr_mtx, NULL);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_init(&vm->sockets[i].mtx, NULL);
        vm->sockets[i].active = 0; vm->sockets[i].fd = -1;
    }
    for (int i = 0; i < SL_MAX_FILES; i++) {
        pthread_mutex_init(&vm->files[i].mtx, NULL);
        vm->files[i].active = 0; vm->files[i].fd = -1; vm->files[i].peer = -1;
    }
    for (int i = 0; i < SL_MAX_LOCKS; i++) {
        pthread_mutex_init(&vm->locks[i], NULL);
        pthread_mutex_init(&vm->mailbox[i].mtx, NULL);
        pthread_cond_init(&vm->mailbox[i].cond, NULL);
    }
    /* A broken FIFO/pipe writer must return EPIPE rather than terminate the VM. */
    signal(SIGPIPE, SIG_IGN);
    return vm;
}

static void close_file_slot(SLFile* f) {
    if (f->active) { close(f->fd); f->fd = -1; f->active = 0; f->peer = -1; }
}

void slvm_free(SLVM* vm) {
    if (!vm) return;
    slvm_joinall(vm);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_lock(&vm->sockets[i].mtx);
        if (vm->sockets[i].active) { close(vm->sockets[i].fd); vm->sockets[i].fd = -1; vm->sockets[i].active = 0; }
        pthread_mutex_unlock(&vm->sockets[i].mtx);
        pthread_mutex_destroy(&vm->sockets[i].mtx);
    }
    pthread_mutex_destroy(&vm->sock_mtx);
    for (int i = 0; i < SL_MAX_FILES; i++) {
        pthread_mutex_lock(&vm->files[i].mtx);
        close_file_slot(&vm->files[i]);
        pthread_mutex_unlock(&vm->files[i].mtx);
        pthread_mutex_destroy(&vm->files[i].mtx);
    }
    pthread_mutex_destroy(&vm->file_mtx);
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

static int socket_alloc(SLVM* vm, int fd) {
    pthread_mutex_lock(&vm->sock_mtx);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) if (!vm->sockets[i].active) {
        pthread_mutex_lock(&vm->sockets[i].mtx); vm->sockets[i].fd = fd; vm->sockets[i].active = 1;
        pthread_mutex_unlock(&vm->sockets[i].mtx); pthread_mutex_unlock(&vm->sock_mtx); return i;
    }
    pthread_mutex_unlock(&vm->sock_mtx); return -1;
}
static int socket_valid_handle(int h) { return h >= 0 && h < SL_MAX_SOCKETS; }
static int socket_fd_locked(SLSocket* s) { return s->active ? s->fd : -1; }

static int file_alloc(SLVM* vm, int fd) {
    pthread_mutex_lock(&vm->file_mtx);
    for (int i = 0; i < SL_MAX_FILES; i++) if (!vm->files[i].active) {
        pthread_mutex_lock(&vm->files[i].mtx); vm->files[i].fd = fd; vm->files[i].active = 1; vm->files[i].peer = -1;
        pthread_mutex_unlock(&vm->files[i].mtx); pthread_mutex_unlock(&vm->file_mtx); return i;
    }
    pthread_mutex_unlock(&vm->file_mtx); return -1;
}
static int file_valid_handle(int h) { return h >= 0 && h < SL_MAX_FILES; }
static int file_fd_locked(SLFile* f) { return f->active ? f->fd : -1; }

static int make_listener(int port) {
    if (port < 0 || port > 65535) return -1;
    int fd = socket(AF_INET, SOCK_STREAM, 0); if (fd < 0) return -1;
    int yes = 1; (void)setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    struct sockaddr_in addr; memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; addr.sin_addr.s_addr = htonl(INADDR_ANY); addr.sin_port = htons((uint16_t)port);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 || listen(fd, 16) < 0) { close(fd); return -1; }
    return fd;
}
static int make_connection(const char* host, int port) {
    if (!host || port < 0 || port > 65535) return -1;
    char service[16]; snprintf(service, sizeof(service), "%d", port);
    struct addrinfo hints; memset(&hints, 0, sizeof(hints)); hints.ai_socktype = SOCK_STREAM; hints.ai_family = AF_UNSPEC;
    struct addrinfo* list = NULL; if (getaddrinfo(host, service, &hints, &list) != 0) return -1;
    int fd = -1;
    for (struct addrinfo* p = list; p; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); if (fd < 0) continue;
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) break;
        close(fd); fd = -1;
    }
    freeaddrinfo(list); return fd;
}

static int make_pipe_handles(SLVM* vm) {
    int pfd[2];
    if (pipe(pfd) < 0) return -1;
    int rh = file_alloc(vm, pfd[0]);
    if (rh < 0) { close(pfd[0]); close(pfd[1]); return -1; }
    int wh = file_alloc(vm, pfd[1]);
    if (wh < 0) {
        pthread_mutex_lock(&vm->files[rh].mtx); close_file_slot(&vm->files[rh]); pthread_mutex_unlock(&vm->files[rh].mtx);
        close(pfd[1]); return -1;
    }
    pthread_mutex_lock(&vm->files[rh].mtx); vm->files[rh].peer = wh; pthread_mutex_unlock(&vm->files[rh].mtx);
    pthread_mutex_lock(&vm->files[wh].mtx); vm->files[wh].peer = rh; pthread_mutex_unlock(&vm->files[wh].mtx);
    return rh;
}

static int make_fifo(const char* path, mode_t mode) {
    if (!path || !*path) return -1;
    if (mkfifo(path, mode) == 0) return 0;
    if (errno == EEXIST) return 0;
    return -1;
}

static int parse_open_mode(const char* mode, int* flags) {
    if (!mode || !*mode || !flags) return -1;
    int nonblock = strchr(mode, 'n') != NULL;
    char base[4] = {0}; int j = 0;
    for (const char* p = mode; *p && j < 3; ++p) if (*p != 'n' && *p != 'b') base[j++] = *p;
    base[j] = 0;
    if (strcmp(base, "r") == 0) *flags = O_RDONLY;
    else if (strcmp(base, "w") == 0) *flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (strcmp(base, "a") == 0) *flags = O_WRONLY | O_CREAT | O_APPEND;
    else if (strcmp(base, "rw") == 0 || strcmp(base, "r+") == 0) *flags = O_RDWR;
    else if (strcmp(base, "w+") == 0) *flags = O_RDWR | O_CREAT | O_TRUNC;
    else if (strcmp(base, "a+") == 0) *flags = O_RDWR | O_CREAT | O_APPEND;
    else return -1;
    *flags |= O_CLOEXEC;
    if (nonblock) *flags |= O_NONBLOCK;
    return 0;
}

static int open_file_handle(SLVM* vm, const char* path, const char* mode) {
    int flags = 0;
    if (parse_open_mode(mode, &flags) < 0 || !path) return -1;
    int fd = open(path, flags, (mode_t)0666);
    if (fd < 0) return -1;
    int h = file_alloc(vm, fd);
    if (h < 0) close(fd);
    return h;
}

static void print_value(SLVM* vm, SLValue v) {
    switch (v.type) {
        case SL_NULL: printf("null"); break;
        case SL_INT: printf("%lld", (long long)v.as.i); break;
        case SL_DOUBLE: printf("%g", v.as.d); break;
        case SL_BOOL: printf(v.as.b ? "true" : "false"); break;
        case SL_STR: printf("%s", slvm_str(vm, v.as.s)); break;
    }
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
        case OP_DUP: PUSH(t->stack[t->sp-1]); break;
        case OP_LOADG: { pthread_mutex_lock(&vm->global_mtx); SLValue v=vm->globals[in.a]; pthread_mutex_unlock(&vm->global_mtx); PUSH(v); } break;
        case OP_STOREG: { SLValue v=POP(); pthread_mutex_lock(&vm->global_mtx); vm->globals[in.a]=v; pthread_mutex_unlock(&vm->global_mtx); } break;
        case OP_LOADL: { SLFrame* f=&t->frames[t->fp-1]; PUSH(t->stack[f->base+in.a]); } break;
        case OP_STOREL: { SLFrame* f=&t->frames[t->fp-1]; t->stack[f->base+in.a]=POP(); } break;
        case OP_ADD: {
            SLValue b=POP(), a=POP();
            if (a.type==SL_STR || b.type==SL_STR) {
                char buf[1024], sa[512], sb[512];
                if(a.type==SL_STR) snprintf(sa,sizeof(sa),"%s",slvm_str(vm,a.as.s)); else if(a.type==SL_INT) snprintf(sa,sizeof(sa),"%lld",(long long)a.as.i); else if(a.type==SL_DOUBLE) snprintf(sa,sizeof(sa),"%g",a.as.d); else if(a.type==SL_BOOL) snprintf(sa,sizeof(sa),"%s",a.as.b?"true":"false"); else snprintf(sa,sizeof(sa),"null");
                if(b.type==SL_STR) snprintf(sb,sizeof(sb),"%s",slvm_str(vm,b.as.s)); else if(b.type==SL_INT) snprintf(sb,sizeof(sb),"%lld",(long long)b.as.i); else if(b.type==SL_DOUBLE) snprintf(sb,sizeof(sb),"%g",b.as.d); else if(b.type==SL_BOOL) snprintf(sb,sizeof(sb),"%s",b.as.b?"true":"false"); else snprintf(sb,sizeof(sb),"null");
                snprintf(buf,sizeof(buf),"%s%s",sa,sb); SLValue r; r.type=SL_STR; r.as.s=intern(vm,buf); PUSH(r);
            } else if (both_int(a,b)) PUSH(slval_int(a.as.i+b.as.i)); else PUSH(slval_double(as_num(a)+as_num(b)));
        } break;
        case OP_SUB: { SLValue b=POP(),a=POP(); if(both_int(a,b)) PUSH(slval_int(a.as.i-b.as.i)); else PUSH(slval_double(as_num(a)-as_num(b))); } break;
        case OP_MUL: { SLValue b=POP(),a=POP(); if(both_int(a,b)) PUSH(slval_int(a.as.i*b.as.i)); else PUSH(slval_double(as_num(a)*as_num(b))); } break;
        case OP_DIV: { SLValue b=POP(),a=POP(); if(both_int(a,b)){if(!b.as.i) TERR("integer divide by zero"); PUSH(slval_int(a.as.i/b.as.i));} else PUSH(slval_double(as_num(a)/as_num(b))); } break;
        case OP_MOD: { SLValue b=POP(),a=POP(); if(!both_int(a,b)) TERR("modulo requires integers"); if(!b.as.i) TERR("integer modulo by zero"); PUSH(slval_int(a.as.i%b.as.i)); } break;
        case OP_NEG: { SLValue a=POP(); if(a.type==SL_INT) PUSH(slval_int(-a.as.i)); else PUSH(slval_double(-as_num(a))); } break;
        case OP_EQ: { SLValue b=POP(),a=POP(); int e=(a.type==SL_STR&&b.type==SL_STR)?(a.as.s==b.as.s):(as_num(a)==as_num(b)); PUSH(slval_bool(e)); } break;
        case OP_NE: { SLValue b=POP(),a=POP(); int e=(a.type==SL_STR&&b.type==SL_STR)?(a.as.s!=b.as.s):(as_num(a)!=as_num(b)); PUSH(slval_bool(e)); } break;
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
            SLValue pv=POP(); if(pv.type!=SL_INT) TERR("listen(port) requires an integer port"); int fd=make_listener((int)pv.as.i); if(fd<0){PUSH(slval_int(-1));break;} int h=socket_alloc(vm,fd); if(h<0){close(fd);PUSH(slval_int(-1));break;} PUSH(slval_int(h));
        } break;
        case OP_ACCEPT: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!socket_valid_handle((int)hv.as.i)) TERR("accept(socket) requires a valid socket handle"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); int fd=socket_fd_locked(s); if(fd<0){pthread_mutex_unlock(&s->mtx);PUSH(slval_int(-1));break;} int cfd=accept(fd,NULL,NULL); pthread_mutex_unlock(&s->mtx); if(cfd<0){PUSH(slval_int(-1));break;} int h=socket_alloc(vm,cfd); if(h<0){close(cfd);PUSH(slval_int(-1));break;} PUSH(slval_int(h));
        } break;
        case OP_CONNECT: {
            SLValue portv=POP(),hostv=POP(); if(hostv.type!=SL_STR||portv.type!=SL_INT) TERR("connect(host, port) requires a String and integer port"); int fd=make_connection(slvm_str(vm,hostv.as.s),(int)portv.as.i); if(fd<0){PUSH(slval_int(-1));break;} int h=socket_alloc(vm,fd); if(h<0){close(fd);PUSH(slval_int(-1));break;} PUSH(slval_int(h));
        } break;
        case OP_SOCKREAD: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!socket_valid_handle((int)hv.as.i)) TERR("sockread(socket) requires a valid socket handle"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); int fd=socket_fd_locked(s); if(fd<0){pthread_mutex_unlock(&s->mtx);PUSH(slval_int(-1));break;} char buf[4097]; ssize_t n=recv(fd,buf,sizeof(buf)-1,0); pthread_mutex_unlock(&s->mtx); if(n<=0){SLValue e; e.type=SL_STR;e.as.s=intern(vm,"");PUSH(e);break;} buf[n]=0; SLValue out;out.type=SL_STR;out.as.s=intern(vm,buf);PUSH(out);
        } break;
        case OP_SOCKWRITE: {
            SLValue sv=POP(),hv=POP(); if(hv.type!=SL_INT||sv.type!=SL_STR||!socket_valid_handle((int)hv.as.i)) TERR("sockwrite(socket, string) requires a socket handle and String"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); int fd=socket_fd_locked(s); if(fd<0){pthread_mutex_unlock(&s->mtx);PUSH(slval_int(-1));break;} const char* data=slvm_str(vm,sv.as.s); ssize_t n=send(fd,data,strlen(data),0); pthread_mutex_unlock(&s->mtx); PUSH(slval_int(n<0?-1:(int64_t)n));
        } break;
        case OP_SOCKCLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!socket_valid_handle((int)hv.as.i)) TERR("sockclose(socket) requires a valid socket handle"); SLSocket* s=&vm->sockets[(int)hv.as.i]; pthread_mutex_lock(&s->mtx); if(s->active){close(s->fd);s->fd=-1;s->active=0;} pthread_mutex_unlock(&s->mtx); PUSH(slval_null());
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
            SLValue mv=POP(),pv=POP(); if(pv.type!=SL_STR||mv.type!=SL_INT) TERR("fifoCreate(path, mode) requires a String path and integer mode"); PUSH(slval_int(make_fifo(slvm_str(vm,pv.as.s),(mode_t)mv.as.i)==0?0:-1));
        } break;
        case OP_FILEOPEN: {
            SLValue mv=POP(),pv=POP(); if(pv.type!=SL_STR||mv.type!=SL_STR) TERR("openFile(path, mode) requires String arguments"); PUSH(slval_int(open_file_handle(vm,slvm_str(vm,pv.as.s),slvm_str(vm,mv.as.s))));
        } break;
        case OP_FILEREAD: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!file_valid_handle((int)hv.as.i)) TERR("read(handle) requires a valid file handle"); int h=(int)hv.as.i; SLFile* f=&vm->files[h]; pthread_mutex_lock(&f->mtx); int fd=file_fd_locked(f); if(fd<0){pthread_mutex_unlock(&f->mtx);SLValue e;e.type=SL_STR;e.as.s=intern(vm,"");PUSH(e);break;} char buf[4097]; ssize_t n=read(fd,buf,sizeof(buf)-1); pthread_mutex_unlock(&f->mtx); if(n<=0){SLValue e;e.type=SL_STR;e.as.s=intern(vm,"");PUSH(e);break;} buf[n]=0; SLValue out;out.type=SL_STR;out.as.s=intern(vm,buf);PUSH(out);
        } break;
        case OP_FILEWRITE: {
            SLValue sv=POP(),hv=POP(); if(hv.type!=SL_INT||sv.type!=SL_STR||!file_valid_handle((int)hv.as.i)) TERR("write(handle, data) requires a file handle and String"); int h=(int)hv.as.i; SLFile* f=&vm->files[h]; pthread_mutex_lock(&f->mtx); int fd=file_fd_locked(f); if(fd<0){pthread_mutex_unlock(&f->mtx);PUSH(slval_int(-1));break;} const char* data=slvm_str(vm,sv.as.s); ssize_t n=write(fd,data,strlen(data)); pthread_mutex_unlock(&f->mtx); PUSH(slval_int(n<0?-1:(int64_t)n));
        } break;
        case OP_FILECLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT||!file_valid_handle((int)hv.as.i)) TERR("close(handle) requires a valid file handle"); int h=(int)hv.as.i; pthread_mutex_lock(&vm->files[h].mtx); close_file_slot(&vm->files[h]); pthread_mutex_unlock(&vm->files[h].mtx); PUSH(slval_null());
        } break;
        case OP_FILEUNLINK: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("unlinkFile(path) requires a String path"); PUSH(slval_int(unlink(slvm_str(vm,pv.as.s))==0?0:-1));
        } break;
        case OP_TIME_UTC_MS: {
            PUSH(slval_int(sltime_utc_millis()));
        } break;
        case OP_TIME_MONO_NS: {
            PUSH(slval_int((int64_t)sltime_monotonic_nanos()));
        } break;
        case OP_TIME_PRECISION_MS: {
            SLTimeSample ts; if(sltime_sample(&ts)!=0){PUSH(slval_int(-1));break;}
            PUSH(slval_int((int64_t)((ts.uncertainty_us+999ULL)/1000ULL)));
        } break;
        case OP_TIME_LOCATION: {
            PUSH(slval_int(intern(vm,sltime_location_timezone())));
        } break;
        case OP_TIME_HTTP_DATE: {
            char out[64]; if(sltime_http_date(sltime_utc_millis(),out,sizeof(out))!=0) out[0]=0;
            PUSH(slval_int(intern(vm,out)));
        } break;
        case OP_TIME_JSON: {
            char out[2048]; SLTimeSample ts;
            if(sltime_sample(&ts)!=0 || sltime_json(&ts,out,sizeof(out))!=0) out[0]=0;
            PUSH(slval_int(intern(vm,out)));
        } break;
        case OP_TIME_NTP: {
            SLValue hv=POP(); if(hv.type!=SL_STR) TERR("timeNtp(host) requires a String host");
            char out[2048]; SLTimeSample ts;
            if(sltime_query_ntp(slvm_str(vm,hv.as.s),1500,&ts)!=0 || sltime_json(&ts,out,sizeof(out))!=0) out[0]=0;
            PUSH(slval_int(intern(vm,out)));
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
