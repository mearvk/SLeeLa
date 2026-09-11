/* ==========================================================================
 * sleela_core.c  --  Implementation of the Sleela execution core.
 *
 * A stack-based bytecode VM (Turing-complete: unbounded conditional jumps +
 * a growable operand stack + addressable globals/locals + recursion). All
 * embedder access is routed through slcore_exchange(); the builder helpers
 * are thin wrappers over it.
 * ========================================================================== */
#define _POSIX_C_SOURCE 200809L   /* strdup */
#include "sleela_core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

/* ---- tunables ---------------------------------------------------------- */
#define STACK_MAX   4096
#define FRAMES_MAX  1024

typedef struct { uint8_t op; int32_t a; } SLInstr;

typedef struct {
    char*   name;
    int32_t entry;    /* ip of first instruction     */
    int32_t nargs;
    int32_t nlocals;
} SLFunc;

typedef struct {
    int32_t ret_ip;   /* where to resume in caller    */
    int32_t base;     /* index in stack[] of local 0  */
    int32_t nargs;
} SLFrame;

/* ---- per-thread execution state --------------------------------------- *
 * Each line of execution owns its own operand stack, call frames, and error
 * slot, so the interpreter loop is fully re-entrant across threads. Only the
 * shared SLVM state (code/consts/funcs/globals/strings/locks/mailbox) is
 * touched under locks.                                                     */
typedef struct SLThread {
    SLVM*    vm;                    /* the shared VM                        */
    SLValue  stack[STACK_MAX]; int sp;
    SLFrame  frames[FRAMES_MAX]; int fp;
    int      entry_func;            /* function this thread runs            */
    SLValue  result;                /* value returned by entry_func         */
    char     err[256];
    /* backing for spawned threads */
    pthread_t handle;
    int       started;
} SLThread;

/* A single mailbox slot: a bounded 2-tuple channel ("burble line"). Holds
 * one pending (tag, value) tuple; a receiver blocks until one is present. */
typedef struct {
    pthread_mutex_t mtx;
    pthread_cond_t  cond;
    int             has;      /* 1 if a tuple is waiting                    */
    int32_t         tag;      /* first element of the 2-tuple               */
    SLValue         value;    /* second element                            */
} SLMailbox;

/* A VM-local socket handle. The public Sleela value is the bounded slot
 * index, not the operating-system descriptor, so the VM owns its lifecycle. */
typedef struct {
    pthread_mutex_t mtx;
    int             active;
    int             fd;
} SLSocket;

struct SLVM {
    /* ---- shared program (immutable during run) ---- */
    SLInstr* code; int codelen, codecap;
    SLValue* consts; int nconst, constcap;
    char**   strs;  int nstr, strcap;
    SLValue* globals; char** gnames; int nglobal, globalcap;
    SLFunc*  funcs; int nfunc, funccap;
    int      cur_func;
    int      entry;

    /* ---- shared result of the entry (main) thread ---- */
    SLValue  last_result;
    char     err[256];

    /* ---- concurrency primitives ---- */
    pthread_mutex_t intern_mtx;   /* guards the string table               */
    pthread_mutex_t global_mtx;   /* guards global slot reads/writes       */
    pthread_mutex_t print_mtx;    /* keeps printed lines atomic            */
    pthread_mutex_t sock_mtx;     /* allocates/reclaims socket slots       */

    SLSocket        sockets[SL_MAX_SOCKETS];

    pthread_mutex_t locks[SL_MAX_LOCKS];   /* the bounded lock table        */
    SLMailbox       mailbox[SL_MAX_LOCKS]; /* the bounded 2-tuple channels  */

    pthread_mutex_t thr_mtx;               /* guards the spawned-thread set */
    SLThread*       threads[SL_MAX_THREADS];
    int             nthreads;
};

/* ---- small helpers ----------------------------------------------------- */
static void set_err(SLVM* vm, const char* msg) {
    if (!vm) return;
    strncpy(vm->err, msg, sizeof(vm->err) - 1);
    vm->err[sizeof(vm->err) - 1] = 0;
}

SLValue slval_null(void)          { SLValue v; v.type = SL_NULL;   v.as.i = 0; return v; }
SLValue slval_int(int64_t x)      { SLValue v; v.type = SL_INT;    v.as.i = x; return v; }
SLValue slval_double(double x)    { SLValue v; v.type = SL_DOUBLE; v.as.d = x; return v; }
SLValue slval_bool(int x)         { SLValue v; v.type = SL_BOOL;   v.as.b = x ? 1 : 0; return v; }

const char* slvm_error(SLVM* vm)  { return (vm && vm->err[0]) ? vm->err : NULL; }
const char* slvm_str(SLVM* vm, int32_t id) {
    if (!vm) return "";
    pthread_mutex_lock(&vm->intern_mtx);
    const char* r = (id >= 0 && id < vm->nstr) ? vm->strs[id] : "";
    pthread_mutex_unlock(&vm->intern_mtx);
    return r;
}

/* growable-array push macros keep the plumbing terse */
#define ENSURE(arr, len, cap, type)                                   \
    do {                                                              \
        if ((len) >= (cap)) {                                         \
            (cap) = (cap) ? (cap) * 2 : 8;                            \
            (arr) = (type*)realloc((arr), (size_t)(cap) * sizeof(type)); \
        }                                                             \
    } while (0)

/* ---- interning (thread-safe) ------------------------------------------- */
static int intern(SLVM* vm, const char* s) {
    pthread_mutex_lock(&vm->intern_mtx);
    for (int i = 0; i < vm->nstr; i++)
        if (strcmp(vm->strs[i], s) == 0) { pthread_mutex_unlock(&vm->intern_mtx); return i; }
    ENSURE(vm->strs, vm->nstr, vm->strcap, char*);
    vm->strs[vm->nstr] = strdup(s);
    int id = vm->nstr++;
    pthread_mutex_unlock(&vm->intern_mtx);
    return id;
}

/* ---- lifecycle --------------------------------------------------------- */
SLVM* slvm_new(void) {
    SLVM* vm = (SLVM*)calloc(1, sizeof(SLVM));
    vm->cur_func = -1;
    vm->entry    = -1;
    vm->last_result = slval_null();

    pthread_mutex_init(&vm->intern_mtx, NULL);
    pthread_mutex_init(&vm->global_mtx, NULL);
    pthread_mutex_init(&vm->print_mtx, NULL);
    pthread_mutex_init(&vm->sock_mtx, NULL);
    pthread_mutex_init(&vm->thr_mtx, NULL);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_init(&vm->sockets[i].mtx, NULL);
        vm->sockets[i].active = 0;
        vm->sockets[i].fd = -1;
    }
    for (int i = 0; i < SL_MAX_LOCKS; i++) {
        pthread_mutex_init(&vm->locks[i], NULL);
        pthread_mutex_init(&vm->mailbox[i].mtx, NULL);
        pthread_cond_init(&vm->mailbox[i].cond, NULL);
        vm->mailbox[i].has = 0;
    }
    return vm;
}

void slvm_free(SLVM* vm) {
    if (!vm) return;
    /* join any threads still outstanding, then reap their handles */
    slvm_joinall(vm);

    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_lock(&vm->sockets[i].mtx);
        if (vm->sockets[i].active) {
            close(vm->sockets[i].fd);
            vm->sockets[i].fd = -1;
            vm->sockets[i].active = 0;
        }
        pthread_mutex_unlock(&vm->sockets[i].mtx);
        pthread_mutex_destroy(&vm->sockets[i].mtx);
    }
    pthread_mutex_destroy(&vm->sock_mtx);

    for (int i = 0; i < SL_MAX_LOCKS; i++) {
        pthread_mutex_destroy(&vm->locks[i]);
        pthread_mutex_destroy(&vm->mailbox[i].mtx);
        pthread_cond_destroy(&vm->mailbox[i].cond);
    }
    pthread_mutex_destroy(&vm->intern_mtx);
    pthread_mutex_destroy(&vm->global_mtx);
    pthread_mutex_destroy(&vm->print_mtx);
    pthread_mutex_destroy(&vm->thr_mtx);

    free(vm->code);
    free(vm->consts);
    for (int i = 0; i < vm->nstr; i++) free(vm->strs[i]);
    free(vm->strs);
    free(vm->globals);
    for (int i = 0; i < vm->nglobal; i++) free(vm->gnames[i]);
    free(vm->gnames);
    for (int i = 0; i < vm->nfunc; i++) free(vm->funcs[i].name);
    free(vm->funcs);
    free(vm);
}

/* ---- builders (also reachable via exchange) ---------------------------- */
static int add_const(SLVM* vm, SLValue v) {
    /* dedupe simple immediates */
    for (int i = 0; i < vm->nconst; i++) {
        SLValue c = vm->consts[i];
        if (c.type != v.type) continue;
        if (v.type == SL_INT    && c.as.i == v.as.i) return i;
        if (v.type == SL_DOUBLE && c.as.d == v.as.d) return i;
        if (v.type == SL_BOOL   && c.as.b == v.as.b) return i;
        if (v.type == SL_STR    && c.as.s == v.as.s) return i;
        if (v.type == SL_NULL) return i;
    }
    ENSURE(vm->consts, vm->nconst, vm->constcap, SLValue);
    vm->consts[vm->nconst] = v;
    return vm->nconst++;
}

int slvm_add_const_int(SLVM* vm, int64_t v)    { return add_const(vm, slval_int(v)); }
int slvm_add_const_double(SLVM* vm, double v)  { return add_const(vm, slval_double(v)); }
int slvm_add_const_bool(SLVM* vm, int v)       { return add_const(vm, slval_bool(v)); }
int slvm_add_const_str(SLVM* vm, const char* s) {
    SLValue v; v.type = SL_STR; v.as.s = intern(vm, s);
    return add_const(vm, v);
}

int slvm_declare_global(SLVM* vm, const char* name) {
    for (int i = 0; i < vm->nglobal; i++)
        if (strcmp(vm->gnames[i], name) == 0) return i;
    ENSURE(vm->globals, vm->nglobal, vm->globalcap, SLValue);
    /* gnames grows in lockstep with globals */
    if (vm->nglobal >= vm->globalcap /*already ensured*/ ) {}
    vm->gnames = (char**)realloc(vm->gnames, (size_t)vm->globalcap * sizeof(char*));
    vm->globals[vm->nglobal] = slval_null();
    vm->gnames[vm->nglobal]  = strdup(name);
    return vm->nglobal++;
}

int slvm_begin_func(SLVM* vm, const char* name, int nargs, int nlocals) {
    ENSURE(vm->funcs, vm->nfunc, vm->funccap, SLFunc);
    int idx = vm->nfunc++;
    vm->funcs[idx].name    = strdup(name);
    vm->funcs[idx].entry   = vm->codelen;
    vm->funcs[idx].nargs   = nargs;
    vm->funcs[idx].nlocals = nlocals;
    vm->cur_func = idx;
    return idx;
}

void slvm_end_func(SLVM* vm) { vm->cur_func = -1; }

int slvm_emit(SLVM* vm, SLOp op, int32_t a) {
    ENSURE(vm->code, vm->codelen, vm->codecap, SLInstr);
    vm->code[vm->codelen].op = (uint8_t)op;
    vm->code[vm->codelen].a  = a;
    return vm->codelen++;
}

int slvm_emit0(SLVM* vm, SLOp op) { return slvm_emit(vm, op, 0); }

void slvm_patch(SLVM* vm, int at, int32_t a) {
    if (at >= 0 && at < vm->codelen) vm->code[at].a = a;
}

int  slvm_here(SLVM* vm)              { return vm->codelen; }
void slvm_set_entry(SLVM* vm, int fi) { vm->entry = fi; }
SLValue slvm_result(SLVM* vm)         { return vm->last_result; }

/* ---- numeric helpers --------------------------------------------------- */
static double as_num(SLValue v) {
    switch (v.type) {
        case SL_INT:    return (double)v.as.i;
        case SL_DOUBLE: return v.as.d;
        case SL_BOOL:   return v.as.b;
        default:        return 0.0;
    }
}
static int is_truthy(SLValue v) {
    switch (v.type) {
        case SL_NULL:   return 0;
        case SL_INT:    return v.as.i != 0;
        case SL_DOUBLE: return v.as.d != 0.0;
        case SL_BOOL:   return v.as.b != 0;
        case SL_STR:    return 1;
    }
    return 0;
}
static int both_int(SLValue a, SLValue b) { return a.type == SL_INT && b.type == SL_INT; }

/* ---- networking -------------------------------------------------------- */
static int socket_alloc(SLVM* vm, int fd) {
    pthread_mutex_lock(&vm->sock_mtx);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        if (!vm->sockets[i].active) {
            pthread_mutex_lock(&vm->sockets[i].mtx);
            vm->sockets[i].fd = fd;
            vm->sockets[i].active = 1;
            pthread_mutex_unlock(&vm->sockets[i].mtx);
            pthread_mutex_unlock(&vm->sock_mtx);
            return i;
        }
    }
    pthread_mutex_unlock(&vm->sock_mtx);
    return -1;
}

static int socket_valid_handle(int h) {
    return h >= 0 && h < SL_MAX_SOCKETS;
}

static int socket_fd_locked(SLSocket* s) {
    return s->active ? s->fd : -1;
}

static int make_listener(int port) {
    if (port < 0 || port > 65535) return -1;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    int yes = 1;
    (void)setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((uint16_t)port);
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0 || listen(fd, 16) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

static int make_connection(const char* host, int port) {
    if (!host || port < 0 || port > 65535) return -1;
    char service[16];
    snprintf(service, sizeof(service), "%d", port);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    struct addrinfo* list = NULL;
    if (getaddrinfo(host, service, &hints, &list) != 0) return -1;
    int fd = -1;
    for (struct addrinfo* p = list; p; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) break;
        close(fd);
        fd = -1;
    }
    freeaddrinfo(list);
    return fd;
}

/* ---- printing ---------------------------------------------------------- */
static void print_value(SLVM* vm, SLValue v) {
    switch (v.type) {
        case SL_NULL:   printf("null"); break;
        case SL_INT:    printf("%lld", (long long)v.as.i); break;
        case SL_DOUBLE: printf("%g", v.as.d); break;
        case SL_BOOL:   printf(v.as.b ? "true" : "false"); break;
        case SL_STR:    printf("%s", slvm_str(vm, v.as.s)); break;
    }
}

/* ---- thread bookkeeping ------------------------------------------------ */
static SLResult run_thread(SLThread* t);   /* fwd */

/* Spawn a function on a fresh thread. `args` are `nargs` values that become
 * the new thread's initial locals. Returns the thread id, or -1 at capacity. */
static void* thread_trampoline(void* arg) {
    SLThread* t = (SLThread*)arg;
    run_thread(t);
    return NULL;
}

static int spawn_thread(SLVM* vm, int fi, const SLValue* args, int nargs) {
    if (fi < 0 || fi >= vm->nfunc) return -1;
    pthread_mutex_lock(&vm->thr_mtx);
    if (vm->nthreads >= SL_MAX_THREADS) { pthread_mutex_unlock(&vm->thr_mtx); return -1; }

    SLThread* t = (SLThread*)calloc(1, sizeof(SLThread));
    t->vm = vm;
    t->entry_func = fi;
    t->result = slval_null();
    /* seed the thread's stack with the arguments as locals 0..nargs-1 */
    SLFunc* f = &vm->funcs[fi];
    int n = nargs; if (n > f->nlocals) n = f->nlocals;
    for (int i = 0; i < n; i++) t->stack[t->sp++] = args[i];
    for (int i = n; i < f->nlocals; i++) t->stack[t->sp++] = slval_null();
    /* first frame */
    t->frames[t->fp].ret_ip = -1;
    t->frames[t->fp].base   = 0;
    t->frames[t->fp].nargs  = f->nargs;
    t->fp = 1;

    int id = vm->nthreads;
    vm->threads[id] = t;
    vm->nthreads++;
    pthread_mutex_unlock(&vm->thr_mtx);

    if (pthread_create(&t->handle, NULL, thread_trampoline, t) != 0) {
        /* roll back on failure */
        pthread_mutex_lock(&vm->thr_mtx);
        vm->threads[id] = NULL;
        vm->nthreads--;
        pthread_mutex_unlock(&vm->thr_mtx);
        free(t);
        return -1;
    }
    t->started = 1;
    return id;
}

void slvm_joinall(SLVM* vm) {
    for (;;) {
        pthread_mutex_lock(&vm->thr_mtx);
        int n = vm->nthreads;
        SLThread* t = NULL;
        int idx = -1;
        for (int i = 0; i < n; i++) {
            if (vm->threads[i]) { t = vm->threads[i]; idx = i; break; }
        }
        pthread_mutex_unlock(&vm->thr_mtx);
        if (!t) break;
        if (t->started) pthread_join(t->handle, NULL);
        pthread_mutex_lock(&vm->thr_mtx);
        vm->threads[idx] = NULL;
        pthread_mutex_unlock(&vm->thr_mtx);
        free(t);
    }
    pthread_mutex_lock(&vm->thr_mtx);
    vm->nthreads = 0;
    pthread_mutex_unlock(&vm->thr_mtx);
}

/* ---- the interpreter loop (re-entrant, per-thread) --------------------- */
#define TERR(msg) do { strncpy(t->err,(msg),sizeof(t->err)-1); t->err[sizeof(t->err)-1]=0; \
                       strncpy(vm->err,(msg),sizeof(vm->err)-1); vm->err[sizeof(vm->err)-1]=0; \
                       return SLR_ERROR; } while(0)
#define PUSH(v) do { if (t->sp >= STACK_MAX) { TERR("stack overflow"); } t->stack[t->sp++] = (v); } while(0)
#define POP()   (t->stack[--t->sp])

static SLResult run_thread(SLThread* t) {
    SLVM* vm = t->vm;
    int fi = t->entry_func;
    if (fi < 0 || fi >= vm->nfunc) { TERR("no entry function set"); }

    SLFunc* ef = &vm->funcs[fi];
    /* The spawn path pre-seeds stack/frames; the main path sets them here. */
    if (t->fp == 0) {
        t->sp = 0;
        t->frames[t->fp].ret_ip = -1;
        t->frames[t->fp].base   = t->sp;
        t->frames[t->fp].nargs  = ef->nargs;
        t->fp = 1;
        for (int i = 0; i < ef->nlocals; i++) PUSH(slval_null());
    }

    int ip = ef->entry;

    for (;;) {
        if (ip < 0 || ip >= vm->codelen) { TERR("ip out of range"); }
        SLInstr in = vm->code[ip++];
        switch (in.op) {
        case OP_NOP: break;
        case OP_HALT: return SLR_HALT;

        case OP_CONST: PUSH(vm->consts[in.a]); break;
        case OP_POP:   (void)POP(); break;
        case OP_DUP:   { SLValue tv = t->stack[t->sp-1]; PUSH(tv); } break;

        case OP_LOADG:  { pthread_mutex_lock(&vm->global_mtx); SLValue g = vm->globals[in.a]; pthread_mutex_unlock(&vm->global_mtx); PUSH(g); } break;
        case OP_STOREG: { SLValue v = POP(); pthread_mutex_lock(&vm->global_mtx); vm->globals[in.a] = v; pthread_mutex_unlock(&vm->global_mtx); } break;

        case OP_LOADL:  { SLFrame* f = &t->frames[t->fp-1]; PUSH(t->stack[f->base + in.a]); } break;
        case OP_STOREL: { SLFrame* f = &t->frames[t->fp-1]; t->stack[f->base + in.a] = POP(); } break;

        case OP_ADD: {
            SLValue b = POP(), a = POP();
            if (a.type == SL_STR || b.type == SL_STR) {
                /* string concatenation: render both, join, intern */
                char buf[512]; char sa[256]; char sb[256];
                if (a.type == SL_STR) snprintf(sa,sizeof sa,"%s", slvm_str(vm,a.as.s));
                else if (a.type == SL_INT) snprintf(sa,sizeof sa,"%lld",(long long)a.as.i);
                else if (a.type == SL_DOUBLE) snprintf(sa,sizeof sa,"%g",a.as.d);
                else if (a.type == SL_BOOL) snprintf(sa,sizeof sa,"%s",a.as.b?"true":"false");
                else snprintf(sa,sizeof sa,"null");
                if (b.type == SL_STR) snprintf(sb,sizeof sb,"%s", slvm_str(vm,b.as.s));
                else if (b.type == SL_INT) snprintf(sb,sizeof sb,"%lld",(long long)b.as.i);
                else if (b.type == SL_DOUBLE) snprintf(sb,sizeof sb,"%g",b.as.d);
                else if (b.type == SL_BOOL) snprintf(sb,sizeof sb,"%s",b.as.b?"true":"false");
                else snprintf(sb,sizeof sb,"null");
                snprintf(buf,sizeof buf,"%s%s",sa,sb);
                SLValue r; r.type = SL_STR; r.as.s = intern(vm, buf); PUSH(r);
            } else if (both_int(a,b)) PUSH(slval_int(a.as.i + b.as.i));
            else PUSH(slval_double(as_num(a) + as_num(b)));
        } break;
        case OP_SUB: { SLValue b=POP(),a=POP(); if(both_int(a,b)) PUSH(slval_int(a.as.i-b.as.i)); else PUSH(slval_double(as_num(a)-as_num(b))); } break;
        case OP_MUL: { SLValue b=POP(),a=POP(); if(both_int(a,b)) PUSH(slval_int(a.as.i*b.as.i)); else PUSH(slval_double(as_num(a)*as_num(b))); } break;
        case OP_DIV: { SLValue b=POP(),a=POP();
            if (both_int(a,b)) { if(b.as.i==0){TERR("integer divide by zero");} PUSH(slval_int(a.as.i/b.as.i)); }
            else PUSH(slval_double(as_num(a)/as_num(b))); } break;
        case OP_MOD: { SLValue b=POP(),a=POP();
            if (both_int(a,b)) { if(b.as.i==0){TERR("integer modulo by zero");} PUSH(slval_int(a.as.i%b.as.i)); }
            else { TERR("modulo requires integers"); } } break;
        case OP_NEG: { SLValue a=POP(); if(a.type==SL_INT) PUSH(slval_int(-a.as.i)); else PUSH(slval_double(-as_num(a))); } break;

        case OP_EQ: { SLValue b=POP(),a=POP();
            int e;
            if (a.type==SL_STR && b.type==SL_STR) e = (a.as.s == b.as.s);
            else e = (as_num(a) == as_num(b));
            PUSH(slval_bool(e));
        } break;
        case OP_NE: { SLValue b=POP(),a=POP();
            int e;
            if (a.type==SL_STR && b.type==SL_STR) e = (a.as.s != b.as.s);
            else e = (as_num(a) != as_num(b));
            PUSH(slval_bool(e));
        } break;
        case OP_LT: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a) <  as_num(b))); } break;
        case OP_LE: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a) <= as_num(b))); } break;
        case OP_GT: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a) >  as_num(b))); } break;
        case OP_GE: { SLValue b=POP(),a=POP(); PUSH(slval_bool(as_num(a) >= as_num(b))); } break;

        case OP_AND: { SLValue b=POP(),a=POP(); PUSH(slval_bool(is_truthy(a) && is_truthy(b))); } break;
        case OP_OR:  { SLValue b=POP(),a=POP(); PUSH(slval_bool(is_truthy(a) || is_truthy(b))); } break;
        case OP_NOT: { SLValue a=POP(); PUSH(slval_bool(!is_truthy(a))); } break;

        case OP_JMP:  ip = in.a; break;
        case OP_JMPF: { SLValue c = POP(); if (!is_truthy(c)) ip = in.a; } break;

        case OP_CALL: {
            int cfi = in.a;
            if (cfi < 0 || cfi >= vm->nfunc) { TERR("call to unknown function"); }
            SLFunc* callee = &vm->funcs[cfi];
            if (t->fp >= FRAMES_MAX) { TERR("call depth exceeded"); }
            /* args are the top `nargs` stack slots; they become locals 0..nargs-1 */
            int base = t->sp - callee->nargs;
            SLFrame* nf = &t->frames[t->fp++];
            nf->ret_ip = ip;
            nf->base   = base;
            nf->nargs  = callee->nargs;
            /* reserve the remaining locals */
            for (int i = callee->nargs; i < callee->nlocals; i++) PUSH(slval_null());
            ip = callee->entry;
        } break;

        case OP_RET: {
            SLValue rv = (t->sp > t->frames[t->fp-1].base) ? POP() : slval_null();
            SLFrame f = t->frames[--t->fp];
            t->sp = f.base;           /* discard the frame's locals */
            if (f.ret_ip < 0) {       /* returned from entry: done  */
                t->result = rv;
                return SLR_OK;
            }
            PUSH(rv);                 /* deliver return value to caller */
            ip = f.ret_ip;
        } break;

        case OP_PRINT: {
            SLValue v = POP();
            pthread_mutex_lock(&vm->print_mtx);   /* keep each printed line atomic */
            print_value(vm, v); printf("\n");
            pthread_mutex_unlock(&vm->print_mtx);
        } break;

        /* ---- threading ------------------------------------------------- */
        case OP_SPAWN: {
            /* a = function index; that function's nargs args are on top */
            int sfi = in.a;
            if (sfi < 0 || sfi >= vm->nfunc) { TERR("spawn of unknown function"); }
            int nargs = vm->funcs[sfi].nargs;
            if (t->sp < nargs) { TERR("spawn: not enough arguments on stack"); }
            SLValue argbuf[FRAMES_MAX];
            for (int i = 0; i < nargs; i++) argbuf[i] = t->stack[t->sp - nargs + i];
            t->sp -= nargs;
            int id = spawn_thread(vm, sfi, argbuf, nargs);
            PUSH(slval_int(id));      /* thread id, or -1 if at capacity */
        } break;

        case OP_JOINALL: {
            slvm_joinall(vm);
        } break;

        case OP_LOCK: {
            int L = in.a;
            if (L < 0 || L >= SL_MAX_LOCKS) { TERR("lock id out of range"); }
            pthread_mutex_lock(&vm->locks[L]);
        } break;

        case OP_UNLOCK: {
            int L = in.a;
            if (L < 0 || L >= SL_MAX_LOCKS) { TERR("lock id out of range"); }
            pthread_mutex_unlock(&vm->locks[L]);
        } break;

        case OP_SEND: {
            /* send the 2-tuple (a, top-of-stack) onto mailbox slot a */
            int slot = in.a;
            if (slot < 0 || slot >= SL_MAX_LOCKS) { TERR("mailbox slot out of range"); }
            SLValue v = POP();
            SLMailbox* mb = &vm->mailbox[slot];
            pthread_mutex_lock(&mb->mtx);
            while (mb->has) pthread_cond_wait(&mb->cond, &mb->mtx);  /* one in flight */
            mb->has = 1; mb->tag = slot; mb->value = v;
            /* broadcast: senders and the receiver share one condvar, so wake
             * all waiters and let each re-check its predicate (avoids the
             * lost-wakeup where a SEND signal wakes another blocked sender). */
            pthread_cond_broadcast(&mb->cond);
            pthread_mutex_unlock(&mb->mtx);
        } break;

        case OP_RECV: {
            /* block until a tuple is present on mailbox slot a; push its value */
            int slot = in.a;
            if (slot < 0 || slot >= SL_MAX_LOCKS) { TERR("mailbox slot out of range"); }
            SLMailbox* mb = &vm->mailbox[slot];
            pthread_mutex_lock(&mb->mtx);
            while (!mb->has) pthread_cond_wait(&mb->cond, &mb->mtx);
            SLValue v = mb->value;
            mb->has = 0;
            pthread_cond_broadcast(&mb->cond);
            pthread_mutex_unlock(&mb->mtx);
            PUSH(v);
        } break;

        /* ---- networking ------------------------------------------------ */
        case OP_LISTEN: {
            SLValue pv = POP();
            if (pv.type != SL_INT) { TERR("listen(port) requires an integer port"); }
            int fd = make_listener((int)pv.as.i);
            if (fd < 0) { PUSH(slval_int(-1)); break; }
            int h = socket_alloc(vm, fd);
            if (h < 0) { close(fd); PUSH(slval_int(-1)); break; }
            PUSH(slval_int(h));
        } break;

        case OP_ACCEPT: {
            SLValue hv = POP();
            if (hv.type != SL_INT || !socket_valid_handle((int)hv.as.i)) {
                TERR("accept(socket) requires a valid socket handle");
            }
            int h = (int)hv.as.i;
            SLSocket* listener = &vm->sockets[h];
            pthread_mutex_lock(&listener->mtx);
            int lfd = socket_fd_locked(listener);
            if (lfd < 0) {
                pthread_mutex_unlock(&listener->mtx);
                PUSH(slval_int(-1));
                break;
            }
            int cfd = accept(lfd, NULL, NULL);
            pthread_mutex_unlock(&listener->mtx);
            if (cfd < 0) { PUSH(slval_int(-1)); break; }
            int ch = socket_alloc(vm, cfd);
            if (ch < 0) { close(cfd); PUSH(slval_int(-1)); break; }
            PUSH(slval_int(ch));
        } break;

        case OP_CONNECT: {
            SLValue portv = POP();
            SLValue hostv = POP();
            if (hostv.type != SL_STR || portv.type != SL_INT) {
                TERR("connect(host, port) requires a String and integer port");
            }
            const char* host = slvm_str(vm, hostv.as.s);
            int fd = make_connection(host, (int)portv.as.i);
            if (fd < 0) { PUSH(slval_int(-1)); break; }
            int h = socket_alloc(vm, fd);
            if (h < 0) { close(fd); PUSH(slval_int(-1)); break; }
            PUSH(slval_int(h));
        } break;

        case OP_SOCKREAD: {
            SLValue hv = POP();
            if (hv.type != SL_INT || !socket_valid_handle((int)hv.as.i)) {
                TERR("sockread(socket) requires a valid socket handle");
            }
            int h = (int)hv.as.i;
            SLSocket* sock = &vm->sockets[h];
            pthread_mutex_lock(&sock->mtx);
            int fd = socket_fd_locked(sock);
            if (fd < 0) {
                pthread_mutex_unlock(&sock->mtx);
                PUSH(slval_int(-1));
                break;
            }
            char buf[4096];
            ssize_t n = recv(fd, buf, sizeof(buf), 0);
            if (n <= 0) {
                pthread_mutex_unlock(&sock->mtx);
                PUSH(slval_int(-1));
                break;
            }
            buf[n] = '\\0';
            int sid = intern(vm, buf);
            pthread_mutex_unlock(&sock->mtx);
            SLValue out; out.type = SL_STR; out.as.s = sid;
            PUSH(out);
        } break;

        case OP_SOCKWRITE: {
            SLValue sv = POP();
            SLValue hv = POP();
            if (hv.type != SL_INT || sv.type != SL_STR || !socket_valid_handle((int)hv.as.i)) {
                TERR("sockwrite(socket, string) requires a socket handle and String");
            }
            int h = (int)hv.as.i;
            SLSocket* sock = &vm->sockets[h];
            pthread_mutex_lock(&sock->mtx);
            int fd = socket_fd_locked(sock);
            if (fd < 0) {
                pthread_mutex_unlock(&sock->mtx);
                PUSH(slval_int(-1));
                break;
            }
            const char* data = slvm_str(vm, sv.as.s);
            size_t len = strlen(data);
            ssize_t n = send(fd, data, len, 0);
            pthread_mutex_unlock(&sock->mtx);
            PUSH(slval_int(n < 0 ? -1 : (int64_t)n));
        } break;

        case OP_SOCKCLOSE: {
            SLValue hv = POP();
            if (hv.type != SL_INT || !socket_valid_handle((int)hv.as.i)) {
                TERR("sockclose(socket) requires a valid socket handle");
            }
            int h = (int)hv.as.i;
            SLSocket* sock = &vm->sockets[h];
            pthread_mutex_lock(&sock->mtx);
            if (sock->active) {
                close(sock->fd);
                sock->fd = -1;
                sock->active = 0;
            }
            pthread_mutex_unlock(&sock->mtx);
            PUSH(slval_null());
        } break;

        default: TERR("illegal opcode");
        }
    }
}

/* ---- the public run entry: runs `entry` on the main thread ------------- */
SLResult slvm_run(SLVM* vm) {
    if (vm->entry < 0 || vm->entry >= vm->nfunc) {
        set_err(vm, "no entry function set");
        return SLR_ERROR;
    }
    vm->err[0] = 0;
    SLThread* t = (SLThread*)calloc(1, sizeof(SLThread));
    t->vm = vm;
    t->entry_func = vm->entry;
    t->result = slval_null();
    SLResult r = run_thread(t);
    vm->last_result = t->result;
    free(t);
    /* ensure no orphaned worker threads outlive the run */
    slvm_joinall(vm);
    return r;
}

/* ---- the exchange dispatch --------------------------------------------- */
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg) {
    if (!vm || !arg) return SLR_ERROR;
    switch (op) {
    case SLX_RESET:
        vm->codelen = vm->nconst = vm->nglobal = vm->nfunc = 0;
        vm->cur_func = -1; vm->entry = -1; vm->err[0] = 0;
        return SLR_OK;
    case SLX_ADD_CONST:
        arg->out = add_const(vm, arg->value);
        return SLR_OK;
    case SLX_DECLARE_GLOBAL:
        arg->out = slvm_declare_global(vm, arg->name);
        return SLR_OK;
    case SLX_BEGIN_FUNC:
        arg->out = slvm_begin_func(vm, arg->name, arg->i0, arg->i1);
        return SLR_OK;
    case SLX_END_FUNC:
        slvm_end_func(vm);
        return SLR_OK;
    case SLX_EMIT:
        arg->out = slvm_emit(vm, (SLOp)arg->op, arg->a);
        return SLR_OK;
    case SLX_PATCH:
        slvm_patch(vm, arg->a, arg->i0);
        return SLR_OK;
    case SLX_HERE:
        arg->out = slvm_here(vm);
        return SLR_OK;
    case SLX_SET_ENTRY:
        slvm_set_entry(vm, arg->a);
        return SLR_OK;
    case SLX_RUN: {
        SLResult r = slvm_run(vm);
        arg->value = vm->last_result;
        return r;
    }
    case SLX_GET_RESULT:
        arg->value = vm->last_result;
        return SLR_OK;
    default:
        set_err(vm, "unknown exchange op");
        return SLR_ERROR;
    }
}
