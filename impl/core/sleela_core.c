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
#include "../subjects/math/linear_algebra.h"
#include "sleela_utf16.h"
#include "sleela_net.h"
#include "sleela_io.h"
#include "sleela_time.h"
#include "sleela_synchro.h"
#include "sleela_munction.h"
#include "sleela_bestof.h"
#include "sleela_audio_mixer.h"
#include "sleela_os.h"
#include "../../os-level-support/osls_vm_bridge.h"
#include "sleela_memmgr.h"
#include "sleela_idea_canvas.h"
#include "sleela_ui_vm.h"
#include "sleela_ui_ids.h"

#ifndef SL_IDEA_CANVAS_MAX
#define SL_IDEA_CANVAS_MAX 16   /* bounded VM-local realtime-canvas table */
#endif

#include <limits.h>
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
#define EXCEPTION_HANDLERS_MAX 1024

typedef struct {
    int32_t target_ip;
    int32_t stack_height;
    int32_t frame_depth;
    int32_t owner_frame;
    SLValue exception_value;
    int has_exception_value;
} SLExceptionHandler;

typedef struct {
    int32_t ret_ip;
    int32_t base;
    int32_t nargs;
    int32_t handler_base; /* handlers active when this frame was entered */
} SLFrame;

typedef struct SLThread {
    SLVM* vm;
    SLValue stack[STACK_MAX]; int sp;
    SLFrame frames[FRAMES_MAX]; int fp;
    SLExceptionHandler exception_handlers[EXCEPTION_HANDLERS_MAX];
    int exception_handler_count;
    SLValue pending_exception;
    int has_pending_exception;
    SLValue uncaught_exception;
    int has_uncaught_exception;
    int entry_func;
    SLValue result;
    char err[256];
    int underflow;   /* set by POP() on stack underflow; checked each instruction */
    uint32_t safepoint_tick; /* counts instructions; the GC safepoint is polled
                              * once per SL_GC_SAFEPOINT_STRIDE instructions
                              * rather than on every one, so a tight numeric loop
                              * does not pay a mutex per opcode. */
    int workers_cached;      /* worker-thread count observed at the last safepoint.
                              * When 0, this thread owns every array/struct, so the
                              * array opcodes skip the per-element array mutex (the
                              * dominant cost of a large fill/scan loop). Refreshed
                              * under thr_mtx at the batched safepoint; a worker can
                              * only be spawned by this thread (OP_SPAWN), which
                              * forces a safepoint refresh before any concurrent
                              * access. */
    pthread_t handle;
    int started;
} SLThread;

/* How often run_thread polls the GC safepoint. Collection still only *acts* at
 * the heap byte threshold; polling 1-in-N instructions removes the per-opcode
 * thread-mutex from the hot path. Between polls the heap can grow by at most
 * this many instructions' worth of allocation, which the allocator's own byte
 * limit still bounds (it fails closed). Allocation-heavy opcodes also poll
 * directly, so a burst of large allocations is never deferred by the stride. */
#define SL_GC_SAFEPOINT_STRIDE 1024u

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
    int parent_type;
    char* fields[SL_MAX_STRUCT_FIELDS];
} SLStructType;
typedef struct {
    int active;
    uint32_t sample_rate;
    char output_path[1024];
    size_t input_count;
    char input_paths[128][1024];
    double start_seconds[128];
    double gain_db[128];
    double bass_db, mid_db, treble_db, master_gain_db, pan, left_gain, right_gain;
} SLAudioJob;


/* A live struct instance: its type index plus one SLValue per declared field. */
typedef struct {
    int active;
    int type;
    SLValue fields[SL_MAX_STRUCT_FIELDS];
    SLGCObject *gc_object;
} SLStructInstance;

/* A live array instance: a dynamic, growable sequence of SLValues. Like a
 * struct, it is reached only by a VM-local handle. `elems` is heap-allocated
 * and grows geometrically; `len` is the logical length, `cap` the allocation. */
typedef struct {
    SLJavaString value;
    int handle;
    SLVM *gc_context;
    SLGCObject *gc_object;
} SLJavaStringInstance;

typedef struct {
    int active;
    int len;
    int cap;
    int has_refs;   /* 1 once a struct/array element has ever been stored. While
                     * 0 the array holds only scalars, so the GC can skip tracing
                     * its elements entirely -- turning O(n) marking of a large
                     * primitive array into O(1) and removing the O(n^2) cost of
                     * re-marking a growing int[] across a fill loop. */
    SLValue *elems;
    SLGCObject *gc_object;
} SLArrayInstance;

struct SLVM {
    SLInstr* code; int codelen, codecap;
    SLValue* consts; int nconst, constcap;
    char** strs; int nstr, strcap;
    SLValue* globals; char** gnames; int nglobal, globalcap;
    SLFunc* funcs; int nfunc, funccap;
    int cur_func;
    int entry;
    SLValue last_result;
    SLValue last_exception;
    int has_last_exception;
    char err[256];
    uint64_t osls_capabilities;

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
    pthread_mutex_t idea_canvas_mtx;
    void* idea_canvas[SL_IDEA_CANVAS_MAX];   /* Idea System(TM) realtime canvases */
    SLStructType* struct_types;  /* grown base-2 from SL_STRUCT_TYPES_INIT, capped at SL_MAX_STRUCT_TYPES */
    int nstruct_types;
    int struct_typecap;          /* current allocated capacity of struct_types */
    SLStructInstance* structs;   /* SL_MAX_STRUCTS instances, lazily allocated */
    int nstruct_live;

    pthread_mutex_t array_mtx;
    SLArrayInstance* arrays;     /* SL_MAX_ARRAYS instances, lazily allocated   */
    int narray_live;
    pthread_mutex_t java_string_mtx;
    SLJavaStringInstance* java_strings[SL_MAX_ARRAYS];

    pthread_mutex_t audio_mtx;
    GarbageCollector gc;
    SLAudioJob audio_jobs[64];
    SLAudioNativeRenderFn audio_renderer;
    void* audio_renderer_user;

    pthread_mutex_t locks[SL_MAX_LOCKS];
    SLMailbox mailbox[SL_MAX_LOCKS];
    pthread_mutex_t thr_mtx;
    SLThread* threads[SL_MAX_THREADS];
    int nthreads;

    /* OS system calls: spawned child processes as VM-local bounded handles.
     * os_proc[h] holds a live backend process, or SLOS_INVALID_PROCESS when the
     * slot is free. Guarded by os_mtx. */
    pthread_mutex_t os_mtx;
    SLOSProcess os_proc[SL_MAX_OS_PROCESSES];
    int os_proc_init;

    /* Trusted native modules are VM-owned bounded handles. The host must
     * install an authorization callback before any module can be opened. */
    pthread_mutex_t native_mtx;
    SLNativeModuleTable native_modules;
    SLNativeAuthorizePath native_authorizer;
    void* native_authorizer_context;
    SLNativeAuthorizeCallback native_callback_authorizer;
    void* native_callback_authorizer_context;
    int native_modules_init;
    SLNativeHostAPI native_host_api;
    SLThread* native_callback_caller; /* suspended OP_NATIVE_CALL stack, rooted during callback */
};

static int slvm_native_host_invoke(void* context, const char* function,
                                   uint32_t argc, const SLNativeValue* argv,
                                   SLNativeValue* out_result);

#if defined(_MSC_VER)
#  define SL_NATIVE_THREAD_LOCAL __declspec(thread)
#else
#  define SL_NATIVE_THREAD_LOCAL _Thread_local
#endif
static SL_NATIVE_THREAD_LOCAL SLVM* native_callback_vm = NULL;
static SL_NATIVE_THREAD_LOCAL SLVM* native_plugin_call_vm = NULL;

static void set_err(SLVM* vm, const char* msg) {
    if (!vm) return;
    strncpy(vm->err, msg, sizeof(vm->err) - 1);
    vm->err[sizeof(vm->err) - 1] = 0;
}

SLValue slval_null(void) { SLValue v; v.type = SL_NULL; v.as.i = 0; return v; }
SLValue slval_int(int64_t x) { SLValue v; v.type = SL_INT; v.as.i = x; return v; }
SLValue slval_int128(sl_int128_t x) { SLValue v; v.type = SL_INT128; v.as.i128 = x; return v; }
/* Read any integer value (SL_INT or SL_INT128) as a 128-bit operand. */
static sl_int128_t as_i128(SLValue v) { return v.type == SL_INT128 ? v.as.i128 : (sl_int128_t)v.as.i; }
SLValue slval_double(double x) { SLValue v; v.type = SL_DOUBLE; v.as.d = x; return v; }
SLValue slval_bool(int x) { SLValue v; v.type = SL_BOOL; v.as.b = x ? 1 : 0; return v; }

/* ---- OS process handle table (bounded, VM-local; mirrors socket_alloc) ---- */
static void os_proc_ensure_init(SLVM* vm) {
    if (vm->os_proc_init) return;
    for (int i = 0; i < SL_MAX_OS_PROCESSES; i++) vm->os_proc[i] = SLOS_INVALID_PROCESS;
    vm->os_proc_init = 1;
}
/* Store a live backend process; returns a VM-local handle or -1 if the table is
 * full (the caller must then release the backend process). */
static int os_proc_alloc(SLVM* vm, SLOSProcess p) {
    pthread_mutex_lock(&vm->os_mtx);
    os_proc_ensure_init(vm);
    int h = -1;
    for (int i = 0; i < SL_MAX_OS_PROCESSES; i++) {
        if (vm->os_proc[i] == SLOS_INVALID_PROCESS) { vm->os_proc[i] = p; h = i; break; }
    }
    pthread_mutex_unlock(&vm->os_mtx);
    return h;
}
static int os_proc_valid(int h) { return h >= 0 && h < SL_MAX_OS_PROCESSES; }
/* Take the backend process out of the slot (freeing the handle); returns it or
 * SLOS_INVALID_PROCESS. */
static SLOSProcess os_proc_take(SLVM* vm, int h) {
    SLOSProcess p = SLOS_INVALID_PROCESS;
    if (!os_proc_valid(h)) return p;
    pthread_mutex_lock(&vm->os_mtx);
    os_proc_ensure_init(vm);
    p = vm->os_proc[h];
    vm->os_proc[h] = SLOS_INVALID_PROCESS;
    pthread_mutex_unlock(&vm->os_mtx);
    return p;
}
/* Peek the backend process without releasing the slot. */
static SLOSProcess os_proc_peek(SLVM* vm, int h) {
    SLOSProcess p = SLOS_INVALID_PROCESS;
    if (!os_proc_valid(h)) return p;
    pthread_mutex_lock(&vm->os_mtx);
    os_proc_ensure_init(vm);
    p = vm->os_proc[h];
    pthread_mutex_unlock(&vm->os_mtx);
    return p;
}

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
int slvm_add_const_utf16(SLVM* vm, const uint16_t* units, size_t length) {
    SLValue value = slvm_java_string_new(vm, units, length);
    return value.type == SL_UTF16 ? add_const(vm, value) : -1;
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

/* Group a non-negative long with thousands separators into `out` (1234567 ->
 * "1,234,567") for the VM's own diagnostic lines. System-side presentation
 * only; the user print(...) path (value_to_text) does NOT use this. */
static const char* sl_group_ll(long long v, char* out, size_t cap) {
    char tmp[32]; long long a = v < 0 ? -v : v;
    int n = snprintf(tmp, sizeof tmp, "%lld", a);
    size_t o = 0;
    if (v < 0 && o + 1 < cap) out[o++] = '-';
    for (int i = 0; i < n; ++i) {
        if (i > 0 && (n - i) % 3 == 0 && o + 1 < cap) out[o++] = ',';
        if (o + 1 < cap) out[o++] = tmp[i];
    }
    out[o < cap ? o : cap - 1] = 0;
    return out;
}

int slvm_declare_struct(SLVM* vm, const char* name, const char* const* field_names, int nfields) {
    if (!vm || !name || nfields < 0 || nfields > SL_MAX_STRUCT_FIELDS) return -1;
    /* Diagnostic trace (env SLEELA_TRACE_LAYOUTS=1): report the live table state
     * on every registration. Catches a corrupt nstruct_types/struct_typecap --
     * e.g. a stale sleela_core.o whose slvm_new left them uninitialized -- by
     * showing the actual counts at the exact call that fails. Written to BOTH
     * stderr and stdout and flushed, so it cannot be swallowed by redirection. */
    int sl_trace = (getenv("SLEELA_TRACE_LAYOUTS") != NULL);
    if (sl_trace) {
        char gnt[32], gtc[32], gmx[32];
        fprintf(stderr, "[trace-declare_struct] name='%s' nstruct_types=%s struct_typecap=%s MAX=%s\n",
                name, sl_group_ll(vm->nstruct_types, gnt, sizeof gnt),
                sl_group_ll(vm->struct_typecap, gtc, sizeof gtc),
                sl_group_ll(SL_MAX_STRUCT_TYPES, gmx, sizeof gmx));
        fflush(stderr);
    }
    for (int i = 0; i < vm->nstruct_types; i++)
        if (strcmp(vm->struct_types[i].name, name) == 0) return i; /* idempotent */
    /* Grow the layout table on demand: 256 -> 512 -> ... -> 65536 (base-2),
     * never past the SL_MAX_STRUCT_TYPES hard ceiling. New slots are zeroed so
     * a partially-filled table never carries stale name/field pointers. */
    if (vm->nstruct_types >= vm->struct_typecap) {
        if (vm->struct_typecap >= SL_MAX_STRUCT_TYPES) {
            /* ALWAYS report the refusal (not gated): a layout-table overflow
             * must never be silent, so this prints even without the trace env. */
            char gnt[32], gtc[32], gmx[32];
            fprintf(stderr, "[sleela-vm] layout-table REFUSED '%s': nstruct_types=%s struct_typecap=%s MAX=%s "
                            "(if this fires at a tiny count, the running VM core is stale/mismatched)\n",
                    name, sl_group_ll(vm->nstruct_types, gnt, sizeof gnt),
                    sl_group_ll(vm->struct_typecap, gtc, sizeof gtc),
                    sl_group_ll(SL_MAX_STRUCT_TYPES, gmx, sizeof gmx));
            fflush(stderr);
            return -1; /* at the ceiling */
        }
        int newcap = vm->struct_typecap ? vm->struct_typecap * 2 : SL_STRUCT_TYPES_INIT;
        if (newcap > SL_MAX_STRUCT_TYPES) newcap = SL_MAX_STRUCT_TYPES;
        SLStructType* grown = (SLStructType*)realloc(vm->struct_types, (size_t)newcap * sizeof(SLStructType));
        if (!grown) return -1; /* out of memory: fail closed, keep the old table */
        memset(grown + vm->struct_typecap, 0, (size_t)(newcap - vm->struct_typecap) * sizeof(SLStructType));
        vm->struct_types = grown;
        vm->struct_typecap = newcap;
    }
    int idx = vm->nstruct_types++;
    SLStructType* st = &vm->struct_types[idx];
    st->name = strdup(name);
    st->nfields = nfields;
    st->parent_type = -1;
    for (int i = 0; i < nfields; i++) st->fields[i] = strdup(field_names[i] ? field_names[i] : "");
    return idx;
}

int slvm_set_struct_parent(SLVM* vm, int type_index, int parent_type_index) {
    if (!vm || type_index < 0 || type_index >= vm->nstruct_types) return -1;
    if (parent_type_index < -1 || parent_type_index >= vm->nstruct_types || parent_type_index == type_index) return -1;
    vm->struct_types[type_index].parent_type = parent_type_index;
    return 0;
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
int slvm_uncaught_exception(SLVM* vm, SLValue* out_exception) {
    if (!vm || !out_exception || !vm->has_last_exception) return 0;
    *out_exception = vm->last_exception;
    return 1;
}

void slvm_set_audio_native_renderer(SLVM* vm, SLAudioNativeRenderFn fn, void* user) {
    if (!vm) return;
    pthread_mutex_lock((pthread_mutex_t*)&vm->audio_mtx);
    vm->audio_renderer = fn;
    vm->audio_renderer_user = user;
    pthread_mutex_unlock((pthread_mutex_t*)&vm->audio_mtx);
}

int slvm_set_osls_capabilities(SLVM* vm, uint64_t capabilities) {
    if (!vm || (capabilities & ~OSLS_VM_CAP_HOST_READ) != 0) return -1;
    vm->osls_capabilities = capabilities;
    return 0;
}

static int slvm_native_authorize_bridge(const char* path, void* context) {
    SLVM* vm = (SLVM*)context;
    return vm && vm->native_authorizer
        ? vm->native_authorizer(path, vm->native_authorizer_context) : 0;
}

int slvm_set_native_authorizer(SLVM* vm, SLNativeAuthorizePath authorize, void* context) {
    if (!vm || !authorize || native_callback_vm == vm) return -1;
    pthread_mutex_lock(&vm->native_mtx);
    if (vm->native_authorizer) {
        pthread_mutex_unlock(&vm->native_mtx);
        return -1; /* immutable after first installation */
    }
    vm->native_authorizer = authorize;
    vm->native_authorizer_context = context;
    pthread_mutex_unlock(&vm->native_mtx);
    return 0;
}

int slvm_set_native_callback_authorizer(SLVM* vm,
                                        SLNativeAuthorizeCallback authorize,
                                        void* context) {
    if (!vm || !authorize || native_callback_vm == vm) return -1;
    pthread_mutex_lock(&vm->native_mtx);
    if (vm->native_callback_authorizer) {
        pthread_mutex_unlock(&vm->native_mtx);
        return -1; /* immutable after first installation */
    }
    vm->native_callback_authorizer = authorize;
    vm->native_callback_authorizer_context = context;
    pthread_mutex_unlock(&vm->native_mtx);
    return 0;
}

SLNativeStatus slvm_native_module_open(SLVM* vm, const char* path, SLNativeHandle* out_handle) {
    SLNativeStatus status;
    if (!vm || !out_handle) return SL_NATIVE_INVALID_ARGUMENT;
    if (native_callback_vm == vm || native_plugin_call_vm == vm) return SL_NATIVE_REENTRANT_CALL;
    *out_handle = SL_NATIVE_HANDLE_INVALID;
    pthread_mutex_lock(&vm->native_mtx);
    status = sl_native_table_open(&vm->native_modules, path, out_handle);
    if (status == SL_NATIVE_OK) {
        SLNativeStatus bind_status = sl_native_table_bind_host(
            &vm->native_modules, *out_handle, &vm->native_host_api);
        /* Callback support is optional for legacy typed and run-only modules.
         * A plugin that advertises the binder but rejects the host table fails
         * closed and is unloaded rather than left partially initialized. */
        if (bind_status != SL_NATIVE_OK &&
            bind_status != SL_NATIVE_HOST_CALLBACK_NOT_FOUND) {
            (void)sl_native_table_close(&vm->native_modules, *out_handle);
            *out_handle = SL_NATIVE_HANDLE_INVALID;
            status = bind_status;
        }
    }
    pthread_mutex_unlock(&vm->native_mtx);
    return status;
}

/* Rebind immediately before each plugin entrypoint. A shared library may
 * be loaded by multiple VM instances in one process; plugins must copy this
 * table into thread-local storage so concurrent VM calls cannot overwrite one
 * another's callback context. Caller holds native_mtx. */
static SLNativeStatus slvm_native_bind_module_host_locked(
    SLVM* vm, SLNativeHandle handle) {
    SLNativeStatus status = sl_native_table_bind_host(
        &vm->native_modules, handle, &vm->native_host_api);
    return status == SL_NATIVE_HOST_CALLBACK_NOT_FOUND ? SL_NATIVE_OK : status;
}

SLNativeStatus slvm_native_module_run(SLVM* vm, SLNativeHandle handle,
                                      int argc, const char* const argv[], int* out_result) {
    SLNativeStatus status;
    if (!vm) return SL_NATIVE_INVALID_ARGUMENT;
    if (native_callback_vm == vm || native_plugin_call_vm == vm) return SL_NATIVE_REENTRANT_CALL;
    /* Serialize calls against close/free so code cannot be unloaded while it
     * is executing. A plugin entry point must not re-enter this VM's native
     * module lifecycle API. */
    pthread_mutex_lock(&vm->native_mtx);
    status = slvm_native_bind_module_host_locked(vm, handle);
    if (status == SL_NATIVE_OK) {
        SLVM* previous_plugin_vm = native_plugin_call_vm;
        native_plugin_call_vm = vm;
        status = sl_native_table_run(&vm->native_modules, handle, argc, argv, out_result);
        native_plugin_call_vm = previous_plugin_vm;
    }
    pthread_mutex_unlock(&vm->native_mtx);
    return status;
}

SLNativeStatus slvm_native_module_call(SLVM* vm, SLNativeHandle handle,
                                       const char* method, uint32_t argc,
                                       const SLNativeValue* argv, SLNativeValue* out_result) {
    SLNativeStatus status;
    if (!vm) return SL_NATIVE_INVALID_ARGUMENT;
    if (native_callback_vm == vm || native_plugin_call_vm == vm) return SL_NATIVE_REENTRANT_CALL;
    /* The VM lock prevents close/unload during a typed call. Plugins must not
     * re-enter this VM's native-module API. Result UTF-8 spans are borrowed
     * from the plugin and must be copied before the next call or close. */
    pthread_mutex_lock(&vm->native_mtx);
    status = slvm_native_bind_module_host_locked(vm, handle);
    if (status == SL_NATIVE_OK) {
        SLVM* previous_plugin_vm = native_plugin_call_vm;
        native_plugin_call_vm = vm;
        status = sl_native_table_call(&vm->native_modules, handle, method, argc, argv, out_result);
        native_plugin_call_vm = previous_plugin_vm;
    }
    pthread_mutex_unlock(&vm->native_mtx);
    return status;
}

SLNativeStatus slvm_native_module_close(SLVM* vm, SLNativeHandle handle) {
    SLNativeStatus status;
    if (!vm) return SL_NATIVE_INVALID_ARGUMENT;
    if (native_callback_vm == vm || native_plugin_call_vm == vm) return SL_NATIVE_REENTRANT_CALL;
    pthread_mutex_lock(&vm->native_mtx);
    status = sl_native_table_close(&vm->native_modules, handle);
    pthread_mutex_unlock(&vm->native_mtx);
    return status;
}

int slvm_memory_safe_mode(const SLVM* vm) {
    /* Sleela source never receives native pointers. Globals, locals, files,
     * sockets, threads, reaches, probes, and struct instances are VM-owned
     * values or bounded handles. A live VM therefore satisfies the memory
     * boundary required by protected source. */
    return vm != NULL ? 1 : 0;
}

SLVM* slvm_new(void) {
    (void)sltime_init();
    SLVM* vm = (SLVM*)calloc(1, sizeof(SLVM));
    if (!vm) return NULL;
    vm->cur_func = -1;
    vm->entry = -1;
    vm->last_result = slval_null();
    vm->osls_capabilities = OSLS_VM_CAP_HOST_READ;
    pthread_mutex_init(&vm->native_mtx, NULL);
    if (sl_native_table_init(&vm->native_modules, slvm_native_authorize_bridge, vm) != SL_NATIVE_OK) {
        pthread_mutex_destroy(&vm->native_mtx);
        free(vm);
        return NULL;
    }
    vm->native_modules_init = 1;
    vm->native_host_api.abi_version = SL_NATIVE_HOST_CALLBACK_ABI_VERSION;
    vm->native_host_api.struct_size = (uint32_t)sizeof(vm->native_host_api);
    vm->native_host_api.context = vm;
    vm->native_host_api.invoke = slvm_native_host_invoke;
    pthread_mutex_init(&vm->intern_mtx, NULL);
    pthread_mutex_init(&vm->global_mtx, NULL);
    pthread_mutex_init(&vm->print_mtx, NULL);
    pthread_mutex_init(&vm->sock_mtx, NULL);
    pthread_mutex_init(&vm->file_mtx, NULL);
    pthread_mutex_init(&vm->struct_mtx, NULL);
    pthread_mutex_init(&vm->array_mtx, NULL);
    pthread_mutex_init(&vm->java_string_mtx, NULL);
    pthread_mutex_init(&vm->thr_mtx, NULL);
    pthread_mutex_init(&vm->synchro_mtx, NULL);
    pthread_mutex_init(&vm->munction_mtx, NULL);
    pthread_mutex_init(&vm->bestof_mtx, NULL);
    pthread_mutex_init(&vm->idea_canvas_mtx, NULL);
    pthread_mutex_init(&vm->audio_mtx, NULL);
    pthread_mutex_init(&vm->os_mtx, NULL);
    for (int i = 0; i < SL_MAX_OS_PROCESSES; i++) vm->os_proc[i] = SLOS_INVALID_PROCESS;
    vm->os_proc_init = 1;
    gc_init(&vm->gc, 1024u * 1024u);
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


/* ---- Java String values: lossless UTF-16 objects managed by the VM GC ---- */
static void gc_java_string_destroy(void *payload);
static SLValue slval_utf16(int32_t handle) { SLValue v; v.type = SL_UTF16; v.as.h = handle; return v; }
static SLJavaStringInstance *java_string_get(SLVM *vm, SLValue value) {
    if (!vm || value.type != SL_UTF16 || value.as.h < 0 || value.as.h >= SL_MAX_ARRAYS) return NULL;
    pthread_mutex_lock(&vm->java_string_mtx);
    SLJavaStringInstance *instance = vm->java_strings[value.as.h];
    pthread_mutex_unlock(&vm->java_string_mtx);
    return instance;
}
SLValue slvm_java_string_from_utf8(SLVM *vm, const char *utf8) {
    if (!vm || !utf8) return slval_null();
    uint16_t *units = NULL;
    size_t length = 0;
    int status = sleela_utf8_to_utf16(utf8, strlen(utf8), &units, &length);
    if (status != 0) return slval_null();
    SLValue value = slvm_java_string_new(vm, units, length);
    free(units);
    return value;
}
SLValue slvm_java_string_new(SLVM *vm, const uint16_t *units, size_t length) {
    if (!vm || (!units && length) || length > (SIZE_MAX - sizeof(SLJavaStringInstance)) / sizeof(uint16_t)) return slval_null();
    SLJavaStringInstance *instance = (SLJavaStringInstance *)calloc(1, sizeof(*instance));
    if (!instance) return slval_null();
    sljava_string_init_empty(&instance->value);
    if (sljava_string_from_utf16(&instance->value, units, length) != SL_JAVA_STRING_OK) { free(instance); return slval_null(); }
    pthread_mutex_lock(&vm->java_string_mtx);
    int handle = -1;
    for (int i = 0; i < SL_MAX_ARRAYS; ++i) if (!vm->java_strings[i]) { handle = i; break; }
    if (handle < 0) { pthread_mutex_unlock(&vm->java_string_mtx); sljava_string_destroy(&instance->value); free(instance); return slval_null(); }
    instance->handle = handle; instance->gc_context = vm;
    instance->gc_object = gc_allocate_external(&vm->gc, instance, sizeof(*instance) + length * sizeof(uint16_t), NULL, gc_java_string_destroy, vm);
    if (!instance->gc_object) { pthread_mutex_unlock(&vm->java_string_mtx); sljava_string_destroy(&instance->value); free(instance); return slval_null(); }
    vm->java_strings[handle] = instance;
    pthread_mutex_unlock(&vm->java_string_mtx);
    return slval_utf16(handle);
}
int slvm_java_string_root(SLVM *vm, SLValue value) {
    SLJavaStringInstance *instance = java_string_get(vm, value);
    return instance && gc_add_root(&vm->gc, instance->gc_object) ? 0 : -1;
}
int slvm_java_string_unroot(SLVM *vm, SLValue value) {
    SLJavaStringInstance *instance = java_string_get(vm, value);
    return instance && gc_remove_root(&vm->gc, instance->gc_object) ? 0 : -1;
}
int slvm_java_string_copy(SLVM *vm, SLValue value, SLJavaString *out) {
    SLJavaStringInstance *instance = java_string_get(vm, value);
    return instance ? sljava_string_clone(out, &instance->value) : SL_JAVA_STRING_INVALID_ARGUMENT;
}
#define SLJAVA_DELEGATE(name, args, callargs) \
int slvm_java_string_##name args { SLJavaStringInstance *instance = java_string_get(vm, value); \
    return instance ? sljava_string_##name callargs : SL_JAVA_STRING_INVALID_ARGUMENT; }
SLJAVA_DELEGATE(length, (SLVM *vm, SLValue value, int32_t *out_length), (&instance->value, out_length))
SLJAVA_DELEGATE(char_at, (SLVM *vm, SLValue value, int32_t index, uint16_t *out_unit), (&instance->value, index, out_unit))
SLJAVA_DELEGATE(code_point_at, (SLVM *vm, SLValue value, int32_t index, int32_t *out_code_point), (&instance->value, index, out_code_point))
SLJAVA_DELEGATE(code_point_count, (SLVM *vm, SLValue value, int32_t begin_index, int32_t end_index, int32_t *out_count), (&instance->value, begin_index, end_index, out_count))
SLJAVA_DELEGATE(offset_by_code_points, (SLVM *vm, SLValue value, int32_t index, int32_t offset, int32_t *out_index), (&instance->value, index, offset, out_index))
SLJAVA_DELEGATE(hash_code, (SLVM *vm, SLValue value, int32_t *out_hash), (&instance->value, out_hash))
#undef SLJAVA_DELEGATE
SLValue slvm_java_string_substring(SLVM *vm, SLValue value, int32_t begin_index, int32_t end_index) {
    SLJavaStringInstance *instance = java_string_get(vm, value); SLJavaString result; sljava_string_init_empty(&result);
    if (!instance || sljava_string_substring(&result, &instance->value, begin_index, end_index) != SL_JAVA_STRING_OK) return slval_null();
    SLValue out = slvm_java_string_new(vm, result.units, result.length); sljava_string_destroy(&result); return out;
}
SLValue slvm_java_string_concat(SLVM *vm, SLValue left, SLValue right) {
    SLJavaStringInstance *a = java_string_get(vm, left), *b = java_string_get(vm, right); SLJavaString result; sljava_string_init_empty(&result);
    if (!a || !b || sljava_string_concat(&result, &a->value, &b->value) != SL_JAVA_STRING_OK) return slval_null();
    SLValue out = slvm_java_string_new(vm, result.units, result.length); sljava_string_destroy(&result); return out;
}
int slvm_java_string_equals(SLVM *vm, SLValue left, SLValue right, int *out_equal) {
    SLJavaStringInstance *a = java_string_get(vm, left), *b = java_string_get(vm, right);
    return a && b ? sljava_string_equals(&a->value, &b->value, out_equal) : SL_JAVA_STRING_INVALID_ARGUMENT;
}
int slvm_java_string_compare_to(SLVM *vm, SLValue left, SLValue right, int32_t *out_order) {
    SLJavaStringInstance *a = java_string_get(vm, left), *b = java_string_get(vm, right);
    return a && b ? sljava_string_compare_to(&a->value, &b->value, out_order) : SL_JAVA_STRING_INVALID_ARGUMENT;
}

static void close_file_slot(SLFile* f) {
    if (f->active) { slio_close(f->fd); f->fd = SLIO_INVALID_HANDLE; f->active = 0; f->peer = -1; }
}

void slvm_free(SLVM* vm) {
    if (!vm) return;
    slvm_joinall(vm);
    if (vm->native_modules_init) {
        pthread_mutex_lock(&vm->native_mtx);
        (void)sl_native_table_close_all(&vm->native_modules);
        pthread_mutex_unlock(&vm->native_mtx);
        vm->native_modules_init = 0;
    }
    pthread_mutex_destroy(&vm->native_mtx);
    for (int i = 0; i < SL_MAX_SOCKETS; i++) {
        pthread_mutex_lock(&vm->sockets[i].mtx);
        if (vm->sockets[i].active) { slnet_close(vm->sockets[i].handle); vm->sockets[i].handle = SL_NET_INVALID; vm->sockets[i].active = 0; }
        pthread_mutex_unlock(&vm->sockets[i].mtx);
        pthread_mutex_destroy(&vm->sockets[i].mtx);
    }
    pthread_mutex_destroy(&vm->sock_mtx);
    gc_collect_with_roots(&vm->gc, NULL, NULL, 0);
    gc_free(&vm->gc);
    pthread_mutex_destroy(&vm->java_string_mtx);
    pthread_mutex_destroy(&vm->audio_mtx);
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
    for (int i = 0; i < SL_IDEA_CANVAS_MAX; i++) {
        if (vm->idea_canvas[i]) { sl_idea_canvas_destroy(vm->idea_canvas[i]); vm->idea_canvas[i] = NULL; }
    }
    pthread_mutex_destroy(&vm->idea_canvas_mtx);
    /* Release any still-live spawned child processes (reap without blocking). */
    for (int i = 0; i < SL_MAX_OS_PROCESSES; i++) {
        if (vm->os_proc[i] != SLOS_INVALID_PROCESS) { slos_release(vm->os_proc[i]); vm->os_proc[i] = SLOS_INVALID_PROCESS; }
    }
    pthread_mutex_destroy(&vm->os_mtx);
    for (int i = 0; i < vm->nstruct_types; i++) {
        free(vm->struct_types[i].name);
        for (int j = 0; j < vm->struct_types[i].nfields; j++) free(vm->struct_types[i].fields[j]);
    }
    free(vm->struct_types);   /* the growable layout table itself */
    free(vm->structs);
    pthread_mutex_destroy(&vm->struct_mtx);
    if (vm->arrays) {
        for (int i = 0; i < SL_MAX_ARRAYS; i++) free(vm->arrays[i].elems);
        free(vm->arrays);
    }
    pthread_mutex_destroy(&vm->array_mtx);
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
    if (v.type == SL_INT128) return (double)v.as.i128;
    if (v.type == SL_DOUBLE) return v.as.d;
    if (v.type == SL_BOOL) return v.as.b;
    return 0.0;
}
static int is_truthy(SLValue v) {
    if (v.type == SL_NULL) return 0;
    if (v.type == SL_INT) return v.as.i != 0;
    if (v.type == SL_INT128) return v.as.i128 != 0;
    if (v.type == SL_DOUBLE) return v.as.d != 0.0;
    if (v.type == SL_BOOL) return v.as.b != 0;
    return 1;
}
static int both_int(SLValue a, SLValue b) { return a.type == SL_INT && b.type == SL_INT; }
/* True when both operands are some integer (SL_INT or SL_INT128). */
static int both_any_int(SLValue a, SLValue b) {
    return (a.type == SL_INT || a.type == SL_INT128) && (b.type == SL_INT || b.type == SL_INT128);
}
static int any_i128(SLValue a, SLValue b) { return a.type == SL_INT128 || b.type == SL_INT128; }

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

/* ---- Idea System(TM) realtime canvas table (VM-owned, bounded) ----------- */
static int ic_alloc(SLVM* vm, void* c) {
    pthread_mutex_lock(&vm->idea_canvas_mtx);
    for (int i = 0; i < SL_IDEA_CANVAS_MAX; i++) if (!vm->idea_canvas[i]) {
        vm->idea_canvas[i] = c; pthread_mutex_unlock(&vm->idea_canvas_mtx); return i;
    }
    pthread_mutex_unlock(&vm->idea_canvas_mtx); return -1;
}
static void* ic_get(SLVM* vm, int h) {
    if (h < 0 || h >= SL_IDEA_CANVAS_MAX) return NULL;
    pthread_mutex_lock(&vm->idea_canvas_mtx); void* c = vm->idea_canvas[h]; pthread_mutex_unlock(&vm->idea_canvas_mtx); return c;
}
static void ic_release(SLVM* vm, int h) {
    if (h < 0 || h >= SL_IDEA_CANVAS_MAX) return;
    pthread_mutex_lock(&vm->idea_canvas_mtx); void* c = vm->idea_canvas[h]; vm->idea_canvas[h] = NULL; pthread_mutex_unlock(&vm->idea_canvas_mtx);
    if (c) sl_idea_canvas_destroy(c);
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

/* ---- GC integration --------------------------------------------------- */
static int struct_valid_handle(SLVM *vm, int h);
static SLGCObject *struct_gc_object(SLVM *vm, int h) {
    if (!vm || !vm->structs || h < 0 || h >= SL_MAX_STRUCTS) return NULL;
    return vm->structs[h].gc_object;
}
static int array_valid_handle(SLVM *vm, int h);
static SLGCObject *array_gc_object(SLVM *vm, int h) {
    if (!vm || !vm->arrays || h < 0 || h >= SL_MAX_ARRAYS) return NULL;
    return vm->arrays[h].gc_object;
}
static void gc_mark_value(SLVM *vm, GarbageCollector *gc, SLValue v);

static SLGCObject *gc_object_for_value(SLVM *vm, SLValue v) {
    if (!vm) return NULL;
    if (v.type == SL_STRUCT && struct_valid_handle(vm, v.as.h))
        return struct_gc_object(vm, v.as.h);
    if (v.type == SL_ARRAY && array_valid_handle(vm, v.as.h))
        return array_gc_object(vm, v.as.h);
    if (v.type == SL_UTF16 && v.as.h >= 0 && v.as.h < SL_MAX_ARRAYS && vm->java_strings[v.as.h])
        return vm->java_strings[v.as.h]->gc_object;
    return NULL;
}

/* Child traversal is delegated to the collector's mark worklist rather than
 * recursively traversing the graph on the C call stack. This keeps deep and
 * cyclic object graphs safe and lets the collector detect worklist failures. */
static void gc_trace_struct(SLGCObject *object, void *context) {
    SLVM *vm = (SLVM *)context;
    SLStructInstance *si = object ? (SLStructInstance *)object->payload : NULL;
    if (!vm || !si || !si->active) return;
    for (int i = 0; i < SL_MAX_STRUCT_FIELDS; ++i)
        if (si->fields[i].type == SL_STRUCT || si->fields[i].type == SL_ARRAY || si->fields[i].type == SL_UTF16)
            gc_mark_value(vm, &vm->gc, si->fields[i]);
}

static void gc_trace_array(SLGCObject *object, void *context) {
    SLVM *vm = (SLVM *)context;
    SLArrayInstance *ai = object ? (SLArrayInstance *)object->payload : NULL;
    if (!vm || !ai || !ai->active) return;
    /* A scalars-only array can hold no references, so there is nothing to trace.
     * This is what keeps marking a large primitive int[]/double[] O(1). */
    if (!ai->has_refs) return;
    for (int i = 0; i < ai->len; ++i)
        if (ai->elems && (ai->elems[i].type == SL_STRUCT || ai->elems[i].type == SL_ARRAY || ai->elems[i].type == SL_UTF16))
            gc_mark_value(vm, &vm->gc, ai->elems[i]);
}

static void gc_mark_value(SLVM *vm, GarbageCollector *gc, SLValue v) {
    SLGCObject *object = gc_object_for_value(vm, v);
    if (gc && object) gc_mark(gc, object);
}
typedef struct { SLVM *vm; SLThread *current; SLThread *suspended; } SLGCRootContext;
static void gc_mark_vm_roots(GarbageCollector *gc, void *context) {
    SLGCRootContext *roots = (SLGCRootContext *)context;
    SLVM *vm = roots ? roots->vm : NULL;
    if (!vm) return;
    pthread_mutex_lock(&vm->global_mtx);
    for (int i = 0; i < vm->nglobal; ++i) gc_mark_value(vm, gc, vm->globals[i]);
    for (int i = 0; i < vm->nconst; ++i) gc_mark_value(vm, gc, vm->consts[i]);
    pthread_mutex_unlock(&vm->global_mtx);
    gc_mark_value(vm, gc, vm->last_result);
    if (vm->has_last_exception) gc_mark_value(vm, gc, vm->last_exception);
    for (int i = 0; i < SL_MAX_LOCKS; ++i) {
        pthread_mutex_lock(&vm->mailbox[i].mtx);
        if (vm->mailbox[i].has) gc_mark_value(vm, gc, vm->mailbox[i].value);
        pthread_mutex_unlock(&vm->mailbox[i].mtx);
    }
    if (roots && roots->current) {
        SLThread *thread = roots->current;
        for (int j = 0; j < thread->sp; ++j) gc_mark_value(vm, gc, thread->stack[j]);
        if (thread->has_pending_exception) gc_mark_value(vm, gc, thread->pending_exception);
        if (thread->has_uncaught_exception) gc_mark_value(vm, gc, thread->uncaught_exception);
        for (int j = 0; j < thread->exception_handler_count; ++j) {
            SLExceptionHandler *handler = &thread->exception_handlers[j];
            if (handler->has_exception_value)
                gc_mark_value(vm, gc, handler->exception_value);
        }
    }
    if (roots && roots->suspended && roots->suspended != roots->current) {
        SLThread *thread = roots->suspended;
        for (int j = 0; j < thread->sp; ++j)
            gc_mark_value(vm, gc, thread->stack[j]);
        if (thread->has_pending_exception) gc_mark_value(vm, gc, thread->pending_exception);
        if (thread->has_uncaught_exception) gc_mark_value(vm, gc, thread->uncaught_exception);
        for (int j = 0; j < thread->exception_handler_count; ++j) {
            SLExceptionHandler *handler = &thread->exception_handlers[j];
            if (handler->has_exception_value)
                gc_mark_value(vm, gc, handler->exception_value);
        }
    }
}
static void gc_java_string_destroy(void *payload) {
    SLJavaStringInstance *instance = (SLJavaStringInstance *)payload;
    if (!instance) return;
    SLVM *vm = (SLVM *)instance->gc_context;
    if (vm && instance->handle >= 0 && instance->handle < SL_MAX_ARRAYS) {
        pthread_mutex_lock(&vm->java_string_mtx);
        if (vm->java_strings[instance->handle] == instance) vm->java_strings[instance->handle] = NULL;
        pthread_mutex_unlock(&vm->java_string_mtx);
    }
    sljava_string_destroy(&instance->value);
    free(instance);
}
static void gc_struct_destroy(void *payload) {
    SLStructInstance *si = (SLStructInstance *)payload;
    if (si) { si->active = 0; si->gc_object = NULL; }
}
static void gc_array_destroy(void *payload) {
    SLArrayInstance *ai = (SLArrayInstance *)payload;
    if (ai) { free(ai->elems); ai->elems = NULL; ai->len = 0; ai->cap = 0; ai->has_refs = 0; ai->active = 0; ai->gc_object = NULL; }
}
static void slvm_gc_safepoint(SLVM *vm, SLThread *current, size_t budget) {
    if (!vm) return;
    pthread_mutex_lock(&vm->thr_mtx);
    int workers = vm->nthreads;
    pthread_mutex_unlock(&vm->thr_mtx);
    /* Cache the worker count so the array opcodes can take a lock-free fast path
     * when single-threaded. Refreshed every batched safepoint. */
    if (current) current->workers_cached = workers;
    /* Collection is a VM safepoint. With worker threads active, defer heap
     * reclamation until structured-concurrency quiescence so all stacks are
     * observed consistently. */
    if (workers == 0) {
        SLGCRootContext roots = { vm, current, vm->native_callback_caller };
        gc_safepoint_with_roots(&vm->gc, gc_mark_vm_roots, &roots, 1, budget);
    }
}

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
            vm->structs[i].gc_object = gc_allocate_external(&vm->gc, &vm->structs[i], sizeof(vm->structs[i]), gc_trace_struct, gc_struct_destroy, vm);
            if (!vm->structs[i].gc_object) { vm->structs[i].active = 0; pthread_mutex_unlock(&vm->struct_mtx); return -1; }
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
static int struct_type_is_a(SLVM* vm, int actual, int expected) {
    if (!vm || expected < 0 || expected >= vm->nstruct_types) return 0;
    for (int depth = 0; actual >= 0 && actual < vm->nstruct_types && depth < SL_MAX_STRUCT_TYPES; ++depth) {
        if (actual == expected) return 1;
        actual = vm->struct_types[actual].parent_type;
    }
    return 0;
}

/* ---- array instances: the same bounded-handle discipline as structs ------- */
static SLValue slval_array(int32_t h) { SLValue v; v.type = SL_ARRAY; v.as.h = h; return v; }

static int array_valid_handle(SLVM* vm, int h) {
    return vm->arrays && h >= 0 && h < SL_MAX_ARRAYS && vm->arrays[h].active;
}

/* Convert VM-owned scalar arrays to contiguous native double buffers. The
 * array mutex is held by the caller for the entire conversion/dispatch/writeback
 * sequence, so no VM array can be resized or mutated during the native call. */
static int sl_math_array_to_double(SLVM *vm, int handle,
                                   double **data, size_t *length) {
    if (!array_valid_handle(vm, handle) || !data || !length) return SLMATH_INVALID_ARGUMENT;
    SLArrayInstance *array = &vm->arrays[handle];
    *length = (size_t)array->len;
    *data = (double*)malloc((*length ? *length : 1) * sizeof(double));
    if (!*data) return SLMATH_ALLOCATION_FAILURE;
    for (size_t i = 0; i < *length; ++i) {
        SLValue value = array->elems[i];
        if (value.type == SL_INT) (*data)[i] = (double)value.as.i;
        else if (value.type == SL_INT128) (*data)[i] = (double)value.as.i128;
        else if (value.type == SL_DOUBLE) (*data)[i] = value.as.d;
        else if (value.type == SL_BOOL) (*data)[i] = value.as.b ? 1.0 : 0.0;
        else { free(*data); *data = NULL; return SLMATH_INVALID_ARGUMENT; }
    }
    return SLMATH_OK;
}

static int sl_math_array_output_buffer(SLVM *vm, int handle,
                                          double **data, size_t *length) {
    if (!array_valid_handle(vm, handle) || !data || !length)
        return SLMATH_INVALID_ARGUMENT;
    *length = (size_t)vm->arrays[handle].len;
    *data = (double*)calloc((*length ? *length : 1), sizeof(double));
    if (!*data) return SLMATH_ALLOCATION_FAILURE;
    return SLMATH_OK;
}

static int sl_math_double_to_array(SLVM *vm, int handle,
                                   const double *data, size_t length) {
    if (!array_valid_handle(vm, handle) || !data) return SLMATH_INVALID_ARGUMENT;
    SLArrayInstance *array = &vm->arrays[handle];
    if ((size_t)array->len < length) return SLMATH_DIMENSION_MISMATCH;
    for (size_t i = 0; i < length; ++i) array->elems[i] = slval_double(data[i]);
    return SLMATH_OK;
}

int slvm_math_dispatch_arrays(SLVM* vm, int operation,
                              int a_handle, int b_handle,
                              int out_handle, int aux_handle,
                              size_t a_rows, size_t a_cols,
                              size_t b_rows, size_t b_cols,
                              size_t out_rows, size_t out_cols,
                              size_t aux_rows, size_t aux_cols,
                              double scalar, size_t flags,
                              double* scalar_result) {
    if (!vm || (out_handle >= 0 && aux_handle >= 0 && out_handle == aux_handle)) return SLMATH_INVALID_ARGUMENT;
    double *a_data=NULL,*b_data=NULL,*out_data=NULL,*aux_data=NULL;
    size_t a_len=0,b_len=0,out_len=0,aux_len=0;
    double scalar_out=0.0;
    size_t out_write_len=0, aux_write_len=0;
    int status=SLMATH_INVALID_ARGUMENT;
    pthread_mutex_lock(&vm->array_mtx);
    if (!array_valid_handle(vm,a_handle)) { status=SLMATH_INVALID_ARGUMENT; goto done; }
    status=sl_math_array_to_double(vm,a_handle,&a_data,&a_len);
    if(status!=SLMATH_OK) goto done;
    if(b_handle>=0) {
        status=sl_math_array_to_double(vm,b_handle,&b_data,&b_len);
        if(status!=SLMATH_OK) goto done;
    }
    if(out_handle>=0) {
        /* Output arrays are capacity-checked but not read: fresh arrayNew()
         * elements are null until the native kernel writes numeric results. */
        status=sl_math_array_output_buffer(vm,out_handle,&out_data,&out_len);
        if(status!=SLMATH_OK) goto done;
    } else {
        out_data=&scalar_out; out_len=1;
        if(!scalar_result) { status=SLMATH_INVALID_ARGUMENT; goto done; }
    }
    if(aux_handle>=0) {
        status=sl_math_array_output_buffer(vm,aux_handle,&aux_data,&aux_len);
        if(status!=SLMATH_OK) goto done;
    }
    {
        slmath_buffer a={a_data,a_len,a_rows,a_cols};
        slmath_buffer b={b_data,b_len,b_rows,b_cols};
        slmath_buffer out={out_data,out_len,out_rows,out_cols};
        slmath_buffer aux={aux_data,aux_len,aux_rows,aux_cols};
        status=(int)slmath_vm_dispatch((slmath_operation)operation,&a,
              b_handle>=0?&b:NULL,&out,aux_handle>=0?&aux:NULL,scalar,flags);
    }
    if(status==SLMATH_OK) {
        if(out_handle>=0) {
            if(out_rows && out_cols > SIZE_MAX/out_rows) status=SLMATH_SIZE_OVERFLOW;
            else {
                out_write_len=out_rows*out_cols;
                if(out_write_len>out_len) status=SLMATH_DIMENSION_MISMATCH;
                else status=sl_math_double_to_array(vm,out_handle,out_data,out_write_len);
            }
        } else *scalar_result=scalar_out;
        if(status==SLMATH_OK && aux_handle>=0) {
            if(aux_rows && aux_cols > SIZE_MAX/aux_rows) status=SLMATH_SIZE_OVERFLOW;
            else {
                aux_write_len=aux_rows*aux_cols;
                if(aux_write_len>aux_len) status=SLMATH_DIMENSION_MISMATCH;
                else status=sl_math_double_to_array(vm,aux_handle,aux_data,aux_write_len);
            }
        }
    }
done:
    free(a_data); free(b_data);
    if(out_handle>=0) free(out_data);
    free(aux_data);
    pthread_mutex_unlock(&vm->array_mtx);
    return status;
}

/* Allocate an array of `n` null elements. Returns a VM-local handle, or -1 on
 * exhaustion / bad length. The instance store is grown lazily. */
static int array_alloc(SLVM* vm, int n) {
    if (n < 0 || n > SL_MAX_ARRAY_LEN) return -1;
    pthread_mutex_lock(&vm->array_mtx);
    if (!vm->arrays) {
        vm->arrays = (SLArrayInstance*)calloc(SL_MAX_ARRAYS, sizeof(SLArrayInstance));
        if (!vm->arrays) { pthread_mutex_unlock(&vm->array_mtx); return -1; }
    }
    for (int i = 0; i < SL_MAX_ARRAYS; i++) {
        if (!vm->arrays[i].active) {
            int cap = n > 0 ? n : 8;
            SLValue* elems = (SLValue*)calloc((size_t)cap, sizeof(SLValue));
            if (!elems) { pthread_mutex_unlock(&vm->array_mtx); return -1; }
            vm->arrays[i].active = 1;
            vm->arrays[i].len = n;
            vm->arrays[i].cap = cap;
            vm->arrays[i].elems = elems;
            vm->arrays[i].gc_object = gc_allocate_external(&vm->gc, &vm->arrays[i], sizeof(vm->arrays[i]) + (size_t)cap * sizeof(*elems), gc_trace_array, gc_array_destroy, vm);
            if (!vm->arrays[i].gc_object) { free(elems); vm->arrays[i].active = 0; vm->arrays[i].elems = NULL; pthread_mutex_unlock(&vm->array_mtx); return -1; }
            for (int j = 0; j < n; j++) vm->arrays[i].elems[j] = slval_null();
            vm->narray_live++;
            pthread_mutex_unlock(&vm->array_mtx);
            return i;
        }
    }
    pthread_mutex_unlock(&vm->array_mtx);
    return -1;
}

/* Append one element, growing capacity geometrically. Returns new length, -1 on failure. */
static int array_push(SLVM* vm, int h, SLValue val) {
    if (!array_valid_handle(vm, h)) return -1;
    pthread_mutex_lock(&vm->array_mtx);
    SLArrayInstance* ai = &vm->arrays[h];
    if (ai->len >= ai->cap) {
        int ncap = ai->cap > 0 ? ai->cap * 2 : 8;
        if (ncap > SL_MAX_ARRAY_LEN) ncap = SL_MAX_ARRAY_LEN;
        if (ai->len >= ncap) { pthread_mutex_unlock(&vm->array_mtx); return -1; }
        size_t old_bytes = sizeof(*ai) + (size_t)ai->cap * sizeof(SLValue);
        size_t new_bytes = sizeof(*ai) + (size_t)ncap * sizeof(SLValue);
        if (!gc_resize_object(&vm->gc, ai->gc_object, new_bytes)) {
            pthread_mutex_unlock(&vm->array_mtx);
            return -1;
        }
        SLValue* ne = (SLValue*)realloc(ai->elems, (size_t)ncap * sizeof(SLValue));
        if (!ne) {
            (void)gc_resize_object(&vm->gc, ai->gc_object, old_bytes);
            pthread_mutex_unlock(&vm->array_mtx);
            return -1;
        }
        ai->elems = ne; ai->cap = ncap;
    }
    if (val.type == SL_STRUCT || val.type == SL_ARRAY || val.type == SL_UTF16) ai->has_refs = 1;
    ai->elems[ai->len++] = val;
    int len = ai->len;
    pthread_mutex_unlock(&vm->array_mtx);
    return len;
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
/* Render a signed 128-bit integer in base 10. %lld cannot print 128-bit, so we
 * build the digits by repeated division on the unsigned magnitude. Works both
 * for a real __int128 and for the 64-bit fallback typedef. */
static void format_int128(sl_int128_t v, char* buf, size_t cap) {
    if (cap == 0) return;
    char tmp[48];
    int neg = (v < 0);
    /* Unsigned magnitude; the standard negate-min-value guard is handled by
     * doing the math in the unsigned domain. */
#if SLEELA_HAVE_INT128
    unsigned __int128 mag = neg ? (unsigned __int128)(-(v + 1)) + 1u : (unsigned __int128)v;
#else
    uint64_t mag = neg ? (uint64_t)(-(v + 1)) + 1u : (uint64_t)v;
#endif
    int n = 0;
    if (mag == 0) tmp[n++] = '0';
    while (mag > 0 && n < (int)sizeof(tmp)) { tmp[n++] = (char)('0' + (int)(mag % 10)); mag /= 10; }
    size_t pos = 0;
    if (neg && pos < cap - 1) buf[pos++] = '-';
    while (n > 0 && pos < cap - 1) buf[pos++] = tmp[--n];
    buf[pos] = '\0';
}
static void value_to_text(SLVM* vm, SLValue v, char* buf, size_t cap) {
    switch (v.type) {
        case SL_NULL:   snprintf(buf, cap, "null"); break;
        case SL_INT:    snprintf(buf, cap, "%lld", (long long)v.as.i); break;
        case SL_INT128: format_int128(v.as.i128, buf, cap); break;
        case SL_DOUBLE: snprintf(buf, cap, "%g", v.as.d); break;
        case SL_BOOL:   snprintf(buf, cap, "%s", v.as.b ? "true" : "false"); break;
        case SL_STR:    snprintf(buf, cap, "%s", slvm_str(vm, v.as.s)); break;
        case SL_UTF16: { SLJavaStringInstance *js = java_string_get(vm, v); snprintf(buf, cap, "[JavaString UTF-16 length=%zu]", js ? js->value.length : 0u); } break;
        case SL_STRUCT: {
            const char* tn = (vm->structs && struct_valid_handle(vm, v.as.h))
                ? vm->struct_types[vm->structs[v.as.h].type].name : "struct";
            snprintf(buf, cap, "%s#%d", tn, v.as.h);
        } break;
        case SL_ARRAY: {
            /* Render as [e0, e1, ...] so print()/String concat on an array is
             * legible. Bounded by cap; elements are rendered recursively. */
            if (!array_valid_handle(vm, v.as.h)) { snprintf(buf, cap, "[]"); break; }
            pthread_mutex_lock(&vm->array_mtx);
            SLArrayInstance* ai = &vm->arrays[v.as.h];
            size_t pos = 0;
            if (pos + 1 < cap) buf[pos++] = '[';
            for (int i = 0; i < ai->len && pos + 2 < cap; ++i) {
                if (i > 0 && pos + 2 < cap) { buf[pos++] = ','; buf[pos++] = ' '; }
                char ebuf[256];
                value_to_text(vm, ai->elems[i], ebuf, sizeof(ebuf));
                size_t elen = strlen(ebuf);
                for (size_t k = 0; k < elen && pos + 2 < cap; ++k) buf[pos++] = ebuf[k];
            }
            if (pos + 1 < cap) buf[pos++] = ']';
            buf[pos] = '\0';
            pthread_mutex_unlock(&vm->array_mtx);
        } break;
    }
}

static void print_java_string(SLVM *vm, SLValue value) {
    SLJavaStringInstance *instance = java_string_get(vm, value);
    if (!instance) { fputs("[invalid Java String]", stdout); return; }
    for (size_t i = 0; i < instance->value.length; ++i) {
        uint32_t cp = instance->value.units[i];
        if (cp >= 0xd800u && cp <= 0xdbffu && i + 1 < instance->value.length) {
            uint32_t low = instance->value.units[i + 1];
            if (low >= 0xdc00u && low <= 0xdfffu) {
                cp = 0x10000u + ((cp - 0xd800u) << 10) + (low - 0xdc00u);
                ++i;
            } else cp = 0xfffdu;
        } else if (cp >= 0xd800u && cp <= 0xdfffu) cp = 0xfffdu;
        unsigned char out[4]; size_t n;
        if (cp <= 0x7fu) { out[0]=(unsigned char)cp; n=1; }
        else if (cp <= 0x7ffu) { out[0]=(unsigned char)(0xc0u|(cp>>6)); out[1]=(unsigned char)(0x80u|(cp&0x3fu)); n=2; }
        else if (cp <= 0xffffu) { out[0]=(unsigned char)(0xe0u|(cp>>12)); out[1]=(unsigned char)(0x80u|((cp>>6)&0x3fu)); out[2]=(unsigned char)(0x80u|(cp&0x3fu)); n=3; }
        else { out[0]=(unsigned char)(0xf0u|(cp>>18)); out[1]=(unsigned char)(0x80u|((cp>>12)&0x3fu)); out[2]=(unsigned char)(0x80u|((cp>>6)&0x3fu)); out[3]=(unsigned char)(0x80u|(cp&0x3fu)); n=4; }
        (void)fwrite(out, 1, n, stdout);
    }
}
static void print_value(SLVM* vm, SLValue v) {
    /* A printed String may be a large generated document; size the buffer from
     * SL_MAX_STRTEXT (heap-allocated) so output is not truncated. */
    if (v.type == SL_STR) {
        printf("%s", slvm_str(vm, v.as.s));
        return;
    }
    if (v.type == SL_UTF16) { print_java_string(vm, v); return; }
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

static void log_unsupported_opcode(SLVM* vm, uint8_t opcode) {
    const char* configured = getenv("SLEELA_OPCODE_LOG");
    FILE* out = (configured && *configured) ? fopen(configured, "a") : stderr;
    if (!out) out = stderr;
    fprintf(out, "SLeeLa VM: unsupported opcode %u (0x%02X) in function %d encountered; execution rejected\\n",
            (unsigned)opcode, (unsigned)opcode, vm ? vm->cur_func : -1);
    fflush(out);
    if (out != stderr) fclose(out);
}

/* ------------------------------------------------------------------------
 * Native big-integer multiply kernel (base 1,000,000,000 limbs).
 *
 * Limbs are uint32_t in [0, 1e9), little-endian (index 0 least significant).
 * sl_bigmul computes r = a * b. It uses schoolbook multiply below a threshold
 * and Karatsuba recursion above it, so the cost grows as O(n^1.585) rather
 * than O(n^2) -- the difference between a million-digit multiply taking
 * seconds versus minutes. Intermediate products are accumulated in uint64_t
 * (max limb product 1e18, well within 2^64) and carries are propagated in a
 * final normalization pass. Returns the number of significant result limbs.
 *
 * This is deliberately buffer-only: it never touches the VM stack, the array
 * mutex per element, or the per-instruction GC safepoint -- the whole multiply
 * is one native call, which is the entire point of the OP_BIGMUL bridge.
 * ---------------------------------------------------------------------- */
#define SL_BIG_BASE 1000000000ULL
#define SL_BIG_KARATSUBA_THRESHOLD 40

/* Schoolbook: r[0..na+nb-1] = a[0..na-1] * b[0..nb-1], base 1e9. r must be
 * zeroed by the caller and have room for na+nb limbs. */
static void sl_bigmul_school(const uint32_t* a, int na,
                             const uint32_t* b, int nb, uint32_t* r) {
    for (int i = 0; i < na; i++) {
        uint64_t carry = 0;
        uint64_t ai = a[i];
        for (int j = 0; j < nb; j++) {
            uint64_t cur = (uint64_t)r[i + j] + ai * (uint64_t)b[j] + carry;
            r[i + j] = (uint32_t)(cur % SL_BIG_BASE);
            carry = cur / SL_BIG_BASE;
        }
        int k = i + nb;
        while (carry) {
            uint64_t cur = (uint64_t)r[k] + carry;
            r[k] = (uint32_t)(cur % SL_BIG_BASE);
            carry = cur / SL_BIG_BASE;
            k++;
        }
    }
}

/* Add src[0..n-1] into dst[0..] at limb offset `off`, propagating carry. */
static void sl_big_addto(uint32_t* dst, const uint32_t* src, int n, int off) {
    uint64_t carry = 0;
    int i = 0;
    for (; i < n; i++) {
        uint64_t cur = (uint64_t)dst[off + i] + src[i] + carry;
        dst[off + i] = (uint32_t)(cur % SL_BIG_BASE);
        carry = cur / SL_BIG_BASE;
    }
    int k = off + n;
    while (carry) {
        uint64_t cur = (uint64_t)dst[k] + carry;
        dst[k] = (uint32_t)(cur % SL_BIG_BASE);
        carry = cur / SL_BIG_BASE;
        k++;
    }
}

/* Karatsuba: r = a * b. r must be zeroed and have room for na+nb limbs.
 * Allocates scratch; returns 0 on success, -1 on allocation failure. */
static int sl_bigmul_kara(const uint32_t* a, int na,
                          const uint32_t* b, int nb, uint32_t* r) {
    if (na <= SL_BIG_KARATSUBA_THRESHOLD || nb <= SL_BIG_KARATSUBA_THRESHOLD) {
        sl_bigmul_school(a, na, b, nb, r);
        return 0;
    }
    int half = (na > nb ? na : nb) / 2;
    if (half >= na) half = na - 1;
    if (half >= nb) half = nb - 1;
    if (half < 1) { sl_bigmul_school(a, na, b, nb, r); return 0; }

    const uint32_t* a0 = a;         int n0 = half;
    const uint32_t* a1 = a + half;  int n1 = na - half;
    const uint32_t* b0 = b;         int m0 = (half < nb ? half : nb);
    const uint32_t* b1 = b + half;  int m1 = nb - half;
    if (m1 < 0) { m1 = 0; }

    int z0len = n0 + m0;
    int z2len = n1 + m1;
    int rc = 0;
    uint32_t* z0 = (uint32_t*)calloc((size_t)(z0len > 0 ? z0len : 1), sizeof(uint32_t));
    uint32_t* z2 = (uint32_t*)calloc((size_t)(z2len > 0 ? z2len : 1), sizeof(uint32_t));
    /* sums a0+a1 and b0+b1 (each at most half+1 limbs) */
    int sa_len = (n0 > n1 ? n0 : n1) + 1;
    int sb_len = (m0 > m1 ? m0 : m1) + 1;
    uint32_t* sa = (uint32_t*)calloc((size_t)sa_len, sizeof(uint32_t));
    uint32_t* sb = (uint32_t*)calloc((size_t)sb_len, sizeof(uint32_t));
    int z1len = sa_len + sb_len;
    uint32_t* z1 = (uint32_t*)calloc((size_t)z1len, sizeof(uint32_t));
    if (!z0 || !z2 || !sa || !sb || !z1) { rc = -1; goto done; }

    if (m0 > 0) { if (sl_bigmul_kara(a0, n0, b0, m0, z0)) { rc = -1; goto done; } }
    if (m1 > 0) { if (sl_bigmul_kara(a1, n1, b1, m1, z2)) { rc = -1; goto done; } }

    /* sa = a0 + a1 */
    { uint64_t c = 0; for (int i = 0; i < sa_len; i++) {
        uint64_t x = c; if (i < n0) x += a0[i]; if (i < n1) x += a1[i];
        sa[i] = (uint32_t)(x % SL_BIG_BASE); c = x / SL_BIG_BASE; } }
    /* sb = b0 + b1 */
    { uint64_t c = 0; for (int i = 0; i < sb_len; i++) {
        uint64_t x = c; if (i < m0) x += b0[i]; if (i < m1) x += b1[i];
        sb[i] = (uint32_t)(x % SL_BIG_BASE); c = x / SL_BIG_BASE; } }

    if (sl_bigmul_kara(sa, sa_len, sb, sb_len, z1)) { rc = -1; goto done; }
    /* z1 = z1 - z0 - z2 (borrow subtraction; z1 >= z0 + z2 mathematically).
     * A single limb subtracts BOTH z0[i] and z2[i], so the running borrow can
     * exceed 1 (up to 2 per limb). Carry it as an integer count of bases, not a
     * 0/1 flag, and normalize cur up by whole bases until it is nonnegative. */
    { int64_t borrow = 0;
      for (int i = 0; i < z1len; i++) {
        int64_t cur = (int64_t)z1[i] - borrow;
        if (i < z0len) cur -= (int64_t)z0[i];
        if (i < z2len) cur -= (int64_t)z2[i];
        borrow = 0;
        while (cur < 0) { cur += (int64_t)SL_BIG_BASE; borrow++; }
        z1[i] = (uint32_t)cur;
      }
    }
    /* r = z0 + z1<<half + z2<<(2*half) */
    sl_big_addto(r, z0, z0len, 0);
    sl_big_addto(r, z1, z1len, half);
    sl_big_addto(r, z2, z2len, 2 * half);
done:
    free(z0); free(z2); free(sa); free(sb); free(z1);
    return rc;
}

/* Trim trailing zero limbs; always report at least 1 limb. */
static int sl_big_significant(const uint32_t* r, int n) {
    while (n > 1 && r[n - 1] == 0) n--;
    return n;
}

/* ------------------------------------------------------------------------
 * NTT (number-theoretic transform) big-integer multiply.
 *
 * For large operands, schoolbook is O(n^2) and Karatsuba O(n^1.585); an NTT
 * convolution is O(n log n). We transform exactly in the integers modulo a
 * single NTT-friendly 62-bit prime, so there is no floating-point rounding and
 * no CRT recombination to get wrong.
 *
 * Each base-1e9 limb is split into THREE base-1000 sub-limbs (a base-1e9 number
 * is a base-1000 number with 3x the digits). The acyclic convolution of two
 * base-1000 digit sequences of combined length L has coefficients bounded by
 * L * (999)^2 < L * 1e6. For the sizes this is used at (L up to ~2^24) that is
 * < 2^54, comfortably below the prime, so every coefficient is recovered
 * exactly. Carries are then propagated in base 1000 and the result repacked
 * into base-1e9 limbs.
 *
 * Prime: p = 4179340454199820289 = 29 * 2^57 + 1 (a known NTT prime with a
 * 2^57 power-of-two factor, so transforms up to length 2^57 exist) with
 * primitive root 3.
 * ---------------------------------------------------------------------- */
#define SL_NTT_PRIME 4179340454199820289ULL
#define SL_NTT_ROOT  3ULL
/* Use NTT only when the schoolbook/Karatsuba product would be large. Below this
 * (in base-1e9 limbs) Karatsuba is faster due to NTT's setup/transform constant. */
#define SL_NTT_THRESHOLD 512

static uint64_t sl_ntt_mulmod(uint64_t a, uint64_t b, uint64_t m) {
#if defined(__SIZEOF_INT128__)
    return (uint64_t)(((unsigned __int128)a * b) % m);
#else
    /* Portable fallback: binary mulmod (slower, only if no __int128). */
    uint64_t r = 0; a %= m;
    while (b) { if (b & 1) { r = (r + a) % m; } a = (a << 1) % m; b >>= 1; }
    return r;
#endif
}
static uint64_t sl_ntt_powmod(uint64_t a, uint64_t e, uint64_t m) {
    uint64_t r = 1 % m; a %= m;
    while (e) { if (e & 1) r = sl_ntt_mulmod(r, a, m); a = sl_ntt_mulmod(a, a, m); e >>= 1; }
    return r;
}
/* In-place iterative NTT of length n (a power of two). invert!=0 does the
 * inverse transform (without the final 1/n scaling, applied by the caller). */
static void sl_ntt_transform(uint64_t* v, size_t n, int invert) {
    for (size_t i = 1, j = 0; i < n; ++i) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint64_t t = v[i]; v[i] = v[j]; v[j] = t; }
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        uint64_t w = sl_ntt_powmod(SL_NTT_ROOT, (SL_NTT_PRIME - 1) / len, SL_NTT_PRIME);
        if (invert) w = sl_ntt_powmod(w, SL_NTT_PRIME - 2, SL_NTT_PRIME);
        for (size_t i = 0; i < n; i += len) {
            uint64_t wn = 1;
            for (size_t k = 0; k < len / 2; ++k) {
                uint64_t u = v[i + k];
                uint64_t t = sl_ntt_mulmod(v[i + k + len / 2], wn, SL_NTT_PRIME);
                v[i + k] = u + t < SL_NTT_PRIME ? u + t : u + t - SL_NTT_PRIME;
                v[i + k + len / 2] = u >= t ? u - t : u + SL_NTT_PRIME - t;
                wn = sl_ntt_mulmod(wn, w, SL_NTT_PRIME);
            }
        }
    }
}
/* r = a * b via NTT. r must be zeroed, room for na+nb base-1e9 limbs.
 * Returns 0 on success, -1 on allocation failure. */
static int sl_bigmul_ntt(const uint32_t* a, int na, const uint32_t* b, int nb, uint32_t* r) {
    /* split each base-1e9 limb into 3 base-1000 sub-limbs */
    size_t la = (size_t)na * 3, lb = (size_t)nb * 3;
    size_t need = la + lb; /* acyclic convolution length */
    size_t n = 1;
    while (n < need) n <<= 1;
    uint64_t* fa = (uint64_t*)calloc(n, sizeof(uint64_t));
    uint64_t* fb = (uint64_t*)calloc(n, sizeof(uint64_t));
    if (!fa || !fb) { free(fa); free(fb); return -1; }
    for (int i = 0; i < na; ++i) {
        uint32_t x = a[i];
        fa[(size_t)i * 3 + 0] = x % 1000; x /= 1000;
        fa[(size_t)i * 3 + 1] = x % 1000; x /= 1000;
        fa[(size_t)i * 3 + 2] = x % 1000;
    }
    for (int i = 0; i < nb; ++i) {
        uint32_t x = b[i];
        fb[(size_t)i * 3 + 0] = x % 1000; x /= 1000;
        fb[(size_t)i * 3 + 1] = x % 1000; x /= 1000;
        fb[(size_t)i * 3 + 2] = x % 1000;
    }
    sl_ntt_transform(fa, n, 0);
    sl_ntt_transform(fb, n, 0);
    for (size_t i = 0; i < n; ++i) fa[i] = sl_ntt_mulmod(fa[i], fb[i], SL_NTT_PRIME);
    sl_ntt_transform(fa, n, 1);
    uint64_t ninv = sl_ntt_powmod(n % SL_NTT_PRIME, SL_NTT_PRIME - 2, SL_NTT_PRIME);
    /* propagate base-1000 carries over the convolution coefficients */
    uint64_t carry = 0;
    size_t outlen = la + lb;
    /* accumulate into base-1000 sub-limbs, then repack 3 at a time into base-1e9 */
    uint64_t subcarry = 0;
    size_t ri = 0;
    uint32_t packLow = 0, packMid = 0, packHigh = 0; (void)packHigh;
    (void)carry;
    /* First pass: coefficient -> base-1000 digit stream with carry. */
    for (size_t i = 0; i < outlen; ++i) {
        uint64_t coeff = sl_ntt_mulmod(fa[i], ninv, SL_NTT_PRIME) + subcarry;
        uint64_t digit = coeff % 1000;
        subcarry = coeff / 1000;
        /* repack: every 3 base-1000 digits form one base-1e9 limb */
        size_t phase = i % 3;
        if (phase == 0) packLow = (uint32_t)digit;
        else if (phase == 1) packMid = (uint32_t)digit;
        else {
            uint32_t limb = packLow + packMid * 1000u + (uint32_t)digit * 1000000u;
            r[ri++] = limb;
        }
    }
    /* flush any tail digits and remaining subcarry into further base-1e9 limbs */
    {
        size_t phase = outlen % 3;
        uint64_t limb = 0;
        if (phase == 1) limb = packLow;
        else if (phase == 2) limb = packLow + (uint64_t)packMid * 1000u;
        /* fold remaining subcarry (in base 1000 at position outlen) into limb */
        /* position within current limb for the next base-1000 digit = phase */
        uint64_t mul = (phase == 0) ? 1 : (phase == 1) ? 1000 : 1000000;
        if (phase == 0) { mul = 1; }
        while (subcarry > 0 || phase != 0) {
            uint64_t d = subcarry % 1000; subcarry /= 1000;
            if (phase == 0) { limb = d; mul = 1000; phase = 1; }
            else if (phase == 1) { limb += d * 1000u; mul = 1000000; phase = 2; }
            else { limb += d * 1000000u; r[ri++] = (uint32_t)limb; limb = 0; mul = 1; phase = 0; }
            if (subcarry == 0 && phase == 0) break;
        }
        if (phase != 0) r[ri++] = (uint32_t)limb;
    }
    free(fa); free(fb);
    return 0;
}

/* Dispatcher: choose NTT for large products, Karatsuba otherwise. */
static int sl_bigmul(const uint32_t* a, int na, const uint32_t* b, int nb, uint32_t* r) {
    if (na >= SL_NTT_THRESHOLD && nb >= SL_NTT_THRESHOLD) {
        if (sl_bigmul_ntt(a, na, b, nb, r) == 0) return 0;
        /* fall through to Karatsuba on NTT allocation failure */
    }
    return sl_bigmul_kara(a, na, b, nb, r);
}

/* r = a + b, base 1e9. r must be zeroed with room for max(na,nb)+1 limbs.
 * Returns the significant result-limb count. */
static int sl_bigadd(const uint32_t* a, int na, const uint32_t* b, int nb, uint32_t* r) {
    int n = na > nb ? na : nb;
    uint64_t carry = 0;
    for (int i = 0; i < n; ++i) {
        uint64_t s = carry;
        if (i < na) s += a[i];
        if (i < nb) s += b[i];
        r[i] = (uint32_t)(s % SL_BIG_BASE);
        carry = s / SL_BIG_BASE;
    }
    int len = n;
    if (carry) { r[n] = (uint32_t)carry; len = n + 1; }
    return sl_big_significant(r, len);
}

/* quotient[0..n-1] = dividend[0..n-1] / divisor (small, 1..1e9-1), base 1e9,
 * most-significant-first long division. Returns the remainder (0..divisor-1).
 * quotient may alias dividend. */
static uint32_t sl_bigdivmod_small(const uint32_t* dividend, int n, uint32_t divisor, uint32_t* quotient) {
    uint64_t rem = 0;
    for (int i = n - 1; i >= 0; --i) {
        uint64_t cur = rem * SL_BIG_BASE + dividend[i];
        quotient[i] = (uint32_t)(cur / divisor);
        rem = cur % divisor;
    }
    return (uint32_t)rem;
}

/* ------------------------------------------------------------------------
 * A1 primitives: big subtract, big compare, big/big divide, big isqrt.
 * All operate on base-1e9 little-endian limb arrays (limb 0 least significant),
 * the same representation bigmul/bigadd/bigdivmod use. These back a binary-
 * splitting Chudnovsky Pi generator (see pi-generator/NATIVE.ADD.md): the
 * recursion needs only +, -, x, and the single final scaling needs one big/big
 * divide and one big isqrt.
 * ------------------------------------------------------------------------ */

/* Compare magnitudes a (na limbs) and b (nb limbs). Returns -1/0/1. */
static int sl_bigcmp(const uint32_t* a, int na, const uint32_t* b, int nb) {
    while (na > 1 && a[na - 1] == 0) na--;
    while (nb > 1 && b[nb - 1] == 0) nb--;
    if (na != nb) return na < nb ? -1 : 1;
    for (int i = na - 1; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

/* r = a - b, base 1e9, assuming a >= b (caller guarantees). r must have room
 * for na limbs. Returns the significant result-limb count. r may alias a. */
static int sl_bigsub(const uint32_t* a, int na, const uint32_t* b, int nb, uint32_t* r) {
    int64_t borrow = 0;
    for (int i = 0; i < na; ++i) {
        int64_t cur = (int64_t)a[i] - borrow - (i < nb ? (int64_t)b[i] : 0);
        if (cur < 0) { cur += (int64_t)SL_BIG_BASE; borrow = 1; } else { borrow = 0; }
        r[i] = (uint32_t)cur;
    }
    return sl_big_significant(r, na);
}

/* q = num / den, rem = num % den, for big/big. Base-1e9 schoolbook long
 * division: build the quotient most-significant limb first, binary-searching
 * each quotient limb q in [0,BASE) so that den*q <= R < den*(q+1) over the
 * running remainder R. num has nn limbs, den nd limbs (den != 0). q holds >= nn
 * limbs, rem holds >= nd limbs; *rem_len receives the remainder length. Returns
 * 0 on success, -1 on allocation failure. O(nn*nd). */
static int sl_bigdiv_school(const uint32_t* num, int nn, const uint32_t* den, int nd,
                            uint32_t* q, uint32_t* rem, int* rem_len) {
    while (nd > 1 && den[nd - 1] == 0) nd--;
    for (int i = 0; i < nn; ++i) q[i] = 0;
    if (nd == 1) {
        uint64_t r = 0;
        for (int i = nn - 1; i >= 0; --i) {
            uint64_t cur = r * SL_BIG_BASE + num[i];
            q[i] = (uint32_t)(cur / den[0]);
            r = cur % den[0];
        }
        rem[0] = (uint32_t)r; *rem_len = 1;
        return 0;
    }
    uint32_t* R = (uint32_t*)calloc((size_t)(nd + 2), sizeof(uint32_t));
    uint32_t* T = (uint32_t*)calloc((size_t)(nd + 2), sizeof(uint32_t));
    if (!R || !T) { free(R); free(T); return -1; }
    int rn = 1;                       /* R starts at 0 */
    for (int i = nn - 1; i >= 0; --i) {
        /* R = R*BASE + num[i] : shift up one limb, insert new low limb */
        for (int j = rn; j >= 1; --j) R[j] = R[j - 1];
        R[0] = num[i];
        rn++;
        while (rn > 1 && R[rn - 1] == 0) rn--;
        /* binary search quotient limb */
        uint32_t lo = 0, hi = (uint32_t)(SL_BIG_BASE - 1), qd = 0;
        while (lo <= hi) {
            uint32_t mid = lo + (hi - lo) / 2;
            uint64_t carry = 0; int tn;
            for (int j = 0; j < nd; ++j) {
                uint64_t p = (uint64_t)den[j] * mid + carry;
                T[j] = (uint32_t)(p % SL_BIG_BASE);
                carry = p / SL_BIG_BASE;
            }
            tn = nd;
            if (carry) { T[nd] = (uint32_t)carry; tn = nd + 1; } else { while (tn > 1 && T[tn-1]==0) tn--; }
            if (sl_bigcmp(T, tn, R, rn) <= 0) { qd = mid; if (mid == (uint32_t)(SL_BIG_BASE-1)) break; lo = mid + 1; }
            else { if (mid == 0) break; hi = mid - 1; }
        }
        q[i] = qd;
        if (qd) {
            uint64_t carry = 0; int tn;
            for (int j = 0; j < nd; ++j) {
                uint64_t p = (uint64_t)den[j] * qd + carry;
                T[j] = (uint32_t)(p % SL_BIG_BASE);
                carry = p / SL_BIG_BASE;
            }
            tn = nd; if (carry) { T[nd] = (uint32_t)carry; tn = nd + 1; }
            int64_t borrow = 0;
            for (int j = 0; j < rn; ++j) {
                int64_t cur = (int64_t)R[j] - borrow - (j < tn ? (int64_t)T[j] : 0);
                if (cur < 0) { cur += (int64_t)SL_BIG_BASE; borrow = 1; } else borrow = 0;
                R[j] = (uint32_t)cur;
            }
            while (rn > 1 && R[rn - 1] == 0) rn--;
        }
    }
    for (int j = 0; j < rn; ++j) rem[j] = R[j];
    *rem_len = rn;
    free(R); free(T);
    return 0;
}

/* (R2) Quasi-linear big/big divide via a Newton-reciprocal computed with the
 * NTT multiply. Computes q = floor(num/den), rem = num - den*q.
 *   num: nn limbs, den: nd limbs (den != 0). q holds >= nn limbs, rem >= nd+1.
 * Returns 0 on success, -1 on allocation failure, and falls back to the
 * schoolbook divide for small operands (where Newton's constant overhead loses).
 *
 * Method: let L = significant limbs of den. Compute the scaled reciprocal
 *   inv = floor(B^(2*ndig) / den)   where ndig = nn (enough guard limbs),
 * by Newton iteration  inv <- inv + inv*(B^k - den*inv)/B^k  doubling precision
 * each step (every multiply is sl_bigmul -> NTT at scale). Then
 *   q ~= floor(num * inv / B^(2*ndig))
 * and a bounded correction (while den*q > num: q--; while den*(q+1) <= num: q++;
 * at most a few steps) makes q exact. rem = num - den*q. */
static int sl_bigshr_limbs(const uint32_t* a, int na, int k, uint32_t* r) {
    /* r = floor(a / B^k): drop the low k limbs. Returns significant count. */
    if (k >= na) { r[0] = 0; return 1; }
    int n = na - k;
    for (int i = 0; i < n; ++i) r[i] = a[i + k];
    return sl_big_significant(r, n);
}

static int sl_bigdiv(const uint32_t* num, int nn, const uint32_t* den, int nd,
                     uint32_t* q, uint32_t* rem, int* rem_len) {
    int ndz = nd; while (ndz > 1 && den[ndz - 1] == 0) ndz--;
    int nnz = nn; while (nnz > 1 && num[nnz - 1] == 0) nnz--;
    /* The Newton-reciprocal path below is retained but GATED OFF: it does not
     * yet converge to an exact quotient in base 1e9, so it always fell back to
     * schoolbook anyway (wasting the Newton attempt). Until the reciprocal is
     * fixed, dispatch straight to the exact schoolbook divide. Correctness is
     * unaffected; the speed win for the big final divide is deferred (see
     * pi-generator/NATIVE.ADD.md). The isqrt speedup (R1) comes from a tight
     * seed that cuts its iteration count, which is independent of this path. */
    if (1 || ndz <= 2 || nnz <= 4 || nnz - ndz <= 2 || nnz < SL_NTT_THRESHOLD) {
        return sl_bigdiv_school(num, nn, den, nd, q, rem, rem_len);
    }
    if (sl_bigcmp(num, nnz, den, ndz) < 0) {
        for (int i = 0; i < nn; ++i) q[i] = 0;
        for (int i = 0; i < nnz; ++i) rem[i] = num[i];
        *rem_len = nnz; return 0;
    }
    /* Precision in limbs: we need the reciprocal to ndig = nnz - ndz + 2 guard
     * limbs so q is accurate to the whole quotient width. */
    int ndig = nnz - ndz + 2;
    int kcap = ndig + ndz + 4;
    uint32_t* inv  = (uint32_t*)calloc((size_t)(kcap + 4), sizeof(uint32_t));
    uint32_t* prod = (uint32_t*)calloc((size_t)(2 * kcap + 8), sizeof(uint32_t));
    uint32_t* two  = (uint32_t*)calloc((size_t)(kcap + 4), sizeof(uint32_t));
    uint32_t* t    = (uint32_t*)calloc((size_t)(2 * kcap + 8), sizeof(uint32_t));
    if (!inv || !prod || !two || !t) { free(inv); free(prod); free(two); free(t); return -1; }
    int rc = 0;
    /* Seed inv = floor(B^(2) ... ) from the top of den: inv0 ~= B^(ndig) / den,
     * approximated from den's top two limbs. Start low-precision and double. */
    {
        /* initial reciprocal of the top ~2 limbs of den, scaled. */
        unsigned long long dtop = (unsigned long long)den[ndz - 1] * SL_BIG_BASE + (ndz >= 2 ? den[ndz - 2] : 0);
        if (dtop == 0) dtop = 1;
        /* inv ~= floor(B^3 / (dtop))  placed so inv*den ~ B^(ndig). We build a
         * low-precision seed then refine with Newton doubling. */
        unsigned long long seed = (unsigned long long)(( (long double)SL_BIG_BASE * SL_BIG_BASE * SL_BIG_BASE ) / (long double)dtop);
        /* place seed at limb position (ndig - ndz) */
        int pos = ndig - ndz;
        if (pos < 0) pos = 0;
        inv[pos] = (uint32_t)(seed % SL_BIG_BASE);
        inv[pos + 1] = (uint32_t)((seed / SL_BIG_BASE) % SL_BIG_BASE);
        inv[pos + 2] = (uint32_t)((seed / SL_BIG_BASE / SL_BIG_BASE) % SL_BIG_BASE);
    }
    int invn = sl_big_significant(inv, kcap + 4);
    /* Newton: inv <- inv + inv*(B^ndig - den*inv)/B^ndig, a few iterations. The
     * fixpoint test is that q stops changing; we simply run enough iterations
     * (log2(ndig)+4) then correct exactly, which is cheap and robust. */
    int iters = 4; { int w = ndig; while (w > 1) { w >>= 1; iters++; } }
    for (int it = 0; it < iters; ++it) {
        /* prod = den * inv */
        for (int i = 0; i < 2*kcap+8; ++i) prod[i] = 0;
        if (sl_bigmul(den, ndz, inv, invn, prod) != 0) { rc = -1; break; }
        int pn = sl_big_significant(prod, ndz + invn);
        /* two = 2*B^ndig  (we want B^ndig - den*inv, but to stay non-negative we
         * compute e = B^ndig - den*inv via sign handling). Build B^ndig: */
        for (int i = 0; i < kcap + 4; ++i) two[i] = 0;
        two[ndig] = 1; int twon = ndig + 1;
        /* e = B^ndig - prod  (prod should be <= B^ndig near convergence; if it
         * exceeds, the seed was high -> clamp to 0 so inv only grows upward). */
        int en; uint32_t* e = t;
        for (int i = 0; i < 2*kcap+8; ++i) t[i] = 0;
        if (sl_bigcmp(two, twon, prod, pn) >= 0) {
            en = sl_bigsub(two, twon, prod, pn, e);
        } else {
            e[0] = 0; en = 1; /* already converged at this precision */
        }
        /* delta = floor(inv * e / B^ndig) */
        for (int i = 0; i < 2*kcap+8; ++i) prod[i] = 0;
        if (sl_bigmul(inv, invn, e, en, prod) != 0) { rc = -1; break; }
        int dn = sl_big_significant(prod, invn + en);
        uint32_t* delta = two; /* reuse */
        int deltan = sl_bigshr_limbs(prod, dn, ndig, delta);
        /* inv = inv + delta */
        for (int i = 0; i < 2*kcap+8; ++i) t[i] = 0;
        int newn = sl_bigadd(inv, invn, delta, deltan, t);
        if (newn > kcap + 3) newn = kcap + 3;
        for (int i = 0; i < newn; ++i) inv[i] = t[i];
        for (int i = newn; i < kcap + 4; ++i) inv[i] = 0;
        invn = sl_big_significant(inv, kcap + 4);
    }
    if (rc == 0) {
        /* q ~= floor(num * inv / B^ndig) */
        for (int i = 0; i < 2*kcap+8; ++i) prod[i] = 0;
        if (sl_bigmul(num, nnz, inv, invn, prod) != 0) rc = -1;
        if (rc == 0) {
            int pn = sl_big_significant(prod, nnz + invn);
            for (int i = 0; i < nn; ++i) q[i] = 0;
            int qn = sl_bigshr_limbs(prod, pn, ndig, q);
            if (qn > nn) qn = nn;
            qn = sl_big_significant(q, qn);
            /* Exact correction. Compute den*q, compare to num, adjust q by +-1
             * a bounded number of times (reciprocal error is < a few ulp). */
            uint32_t* dq = prod; /* reuse */
            for (int guard = 0; guard < 8; ++guard) {
                for (int i = 0; i < 2*kcap+8; ++i) dq[i] = 0;
                if (sl_bigmul(den, ndz, q, qn, dq) != 0) { rc = -1; break; }
                int dqn = sl_big_significant(dq, ndz + qn);
                int c = sl_bigcmp(dq, dqn, num, nnz);
                if (c > 0) {
                    /* q too big: q-- */
                    uint32_t one = 1; uint32_t* qm = t;
                    for (int i=0;i<2*kcap+8;++i) t[i]=0;
                    int qmn = sl_bigsub(q, qn, &one, 1, qm);
                    for (int i=0;i<qmn;++i) q[i]=qm[i]; for (int i=qmn;i<qn;++i) q[i]=0;
                    qn = sl_big_significant(q, qn);
                } else {
                    /* check if (q+1)*den <= num -> q too small */
                    uint32_t one = 1; uint32_t* qp = t;
                    for (int i=0;i<2*kcap+8;++i) t[i]=0;
                    int qpn = sl_bigadd(q, qn, &one, 1, qp);
                    for (int i=0;i<2*kcap+8;++i) dq[i]=0;
                    if (sl_bigmul(den, ndz, qp, qpn, dq) != 0) { rc = -1; break; }
                    int dqn2 = sl_big_significant(dq, ndz + qpn);
                    if (sl_bigcmp(dq, dqn2, num, nnz) <= 0) {
                        for (int i=0;i<qpn;++i) q[i]=qp[i]; qn = sl_big_significant(q, qpn);
                    } else break; /* q is exact */
                }
            }
            if (rc == 0) {
                /* rem = num - den*q, then VERIFY exactness: a correct quotient
                 * leaves 0 <= rem < den. If the Newton reciprocal did not fully
                 * settle within the guard budget (rem still >= den, or den*q >
                 * num so the subtract would underflow), fall back to the
                 * schoolbook divide for a guaranteed-exact result rather than
                 * writing a too-wide / wrong remainder. */
                for (int i = 0; i < 2*kcap+8; ++i) dq[i] = 0;
                if (sl_bigmul(den, ndz, q, qn, dq) != 0) rc = -1;
                if (rc == 0) {
                    int dqn = sl_big_significant(dq, ndz + qn);
                    if (sl_bigcmp(dq, dqn, num, nnz) > 0) {
                        rc = 2;  /* den*q > num: q overshot -> fall back */
                    } else {
                        for (int i = 0; i < 2*kcap+8; ++i) t[i] = 0;
                        int rn = sl_bigsub(num, nnz, dq, dqn, t);
                        if (sl_bigcmp(t, rn, den, ndz) >= 0) {
                            rc = 2;  /* rem >= den: q undershot -> fall back */
                        } else {
                            for (int i = 0; i < rn && i < nnz; ++i) rem[i] = t[i];
                            *rem_len = rn;
                        }
                    }
                }
            }
        }
    }
    free(inv); free(prod); free(two); free(t);
    if (rc == 2) {
        /* Newton did not converge to an exact quotient within budget: use the
         * schoolbook divide, which is always exact (and still correct, just
         * O(n^2) for this one call). */
        return sl_bigdiv_school(num, nn, den, nd, q, rem, rem_len);
    }
    return rc;
}

/* r = floor(sqrt(a)), base 1e9. a has na limbs; r must hold >= (na+1)/2 + 1
 * limbs. Returns significant limb count of r, or -1 on failure. a >= 0.
 *
 * (R1) Newton's method `x = (x + a/x)/2`, but the SEED is seeded in double
 * precision and only ONE a/x divide runs at full precision: the previous
 * schoolbook isqrt ran ~log2(limbs) full-width schoolbook divides (O(n^2 log n),
 * ~94% of a Chudnovsky run). Here the Newton root converges quadratically, so we
 * iterate to the fixpoint but the correction loop runs few times near the root;
 * combined with the recursive quasi-linear divide (sl_bigdiv, used for the inner
 * a/x once operands are large) the dominant cost drops from O(n^2 log n) toward
 * the divide's own complexity. A final adjust guarantees floor() exactly. */
static int sl_bigisqrt(const uint32_t* a, int na, uint32_t* r) {
    while (na > 1 && a[na - 1] == 0) na--;
    if (na <= 2) {
        /* a fits in <= 18 digits -> exact 64-bit integer Newton. */
        unsigned long long v = a[0];
        if (na == 2) v += (unsigned long long)a[1] * SL_BIG_BASE;
        unsigned long long x = v;
        if (v >= 2) {
            unsigned long long y = (x + 1) / 2;
            while (y < x) { x = y; y = (x + v / x) / 2; }
        }
        r[0] = (uint32_t)(x % SL_BIG_BASE);
        if (x >= SL_BIG_BASE) { r[1] = (uint32_t)(x / SL_BIG_BASE); return 2; }
        return 1;
    }
    int cap = (na + 1) / 2 + 2;
    uint32_t* x = (uint32_t*)calloc((size_t)(cap + 4), sizeof(uint32_t));
    uint32_t* q = (uint32_t*)calloc((size_t)(na + 2), sizeof(uint32_t));
    uint32_t* rem = (uint32_t*)calloc((size_t)(na + 4), sizeof(uint32_t));
    uint32_t* s = (uint32_t*)calloc((size_t)(na + 4), sizeof(uint32_t));
    if (!x || !q || !rem || !s) { free(x); free(q); free(rem); free(s); return -1; }
    /* (R1) TIGHT seed that is a guaranteed overestimate, so Newton's
     * `x = (x + a/x)/2` descends monotonically to floor(sqrt(a)) in only ~2-3
     * steps instead of the ~log2(BASE^(na/2)) steps a BASE^ceil(na/2) seed
     * needs. This is the win: the previous isqrt ran ~37 full-width schoolbook
     * divides for a 10k-digit sqrt (~94% of a Chudnovsky run); a tight seed cuts
     * that to a handful.
     *
     * Construction: take the top 1-2 limbs of a as `top` with an EVEN number of
     * trailing limbs `rest` (so sqrt scales by BASE^(rest/2)). s0 = isqrt(top)+1
     * (64-bit, a tiny overestimate of sqrt(top)), placed at limb rest/2. Because
     * (s0)^2 * BASE^rest >= top * BASE^rest >= a, the seed is >= sqrt(a). */
    {
        /* Use the top 2 limbs (18 digits, fits in 64-bit) as `top`, with `rest`
         * = the remaining low limbs. We need `rest` EVEN so sqrt scales by the
         * whole-limb factor BASE^(rest/2). na>=3 here, so rest = na-2 >= 1; if
         * that is odd, fold one more limb into `top`... but 3 limbs overflow
         * 64-bit, so instead we DROP to rest being even by using topn=2 and
         * accepting an odd rest via a half-limb (sqrt(BASE)) correction. */
        int topn = 2;
        int rest = na - topn;                  /* low limbs below the top two */
        unsigned long long top = (unsigned long long)a[na - 1] * SL_BIG_BASE + a[na - 2];
        /* 64-bit floor-sqrt(top), +1 for a strict overestimate. */
        unsigned long long st = top;
        if (top >= 2) { unsigned long long y = (st + 1) / 2; while (y < st) { st = y; y = (st + top / st) / 2; } }
        st += 1;
        int half = rest / 2;                   /* whole-limb up-shift */
        if (rest & 1) {
            /* odd rest: one leftover BASE factor under the sqrt -> multiply the
             * seed by ceil(sqrt(BASE)) = 31623 (31623^2 = 1000013129 > 1e9) to
             * keep the seed an overestimate, and shift by floor(rest/2). */
            st *= 31623ULL;
        }
        x[half]     = (uint32_t)(st % SL_BIG_BASE);
        x[half + 1] = (uint32_t)((st / SL_BIG_BASE) % SL_BIG_BASE);
        x[half + 2] = (uint32_t)((st / SL_BIG_BASE / SL_BIG_BASE) % SL_BIG_BASE);
    }
    int xn = sl_big_significant(x, cap + 4);
    if (xn < 1) { x[0] = 1; xn = 1; }
    for (int iter = 0; iter < 10000; ++iter) {
        int reml = 0;
        if (sl_bigdiv(a, na, x, xn, q, rem, &reml) != 0) { free(x); free(q); free(rem); free(s); return -1; }
        int ql = sl_big_significant(q, na);
        int sn = sl_bigadd(x, xn, q, ql, s);        /* s = x + a/x */
        sl_bigdivmod_small(s, sn, 2, s);            /* s = (x + a/x)/2 */
        sn = sl_big_significant(s, sn);
        if (sl_bigcmp(s, sn, x, xn) >= 0) break;    /* floor-sqrt fixpoint */
        for (int i = 0; i < sn; ++i) x[i] = s[i];
        for (int i = sn; i < xn; ++i) x[i] = 0;
        xn = sn;
    }
    int out = sl_big_significant(x, xn);
    for (int i = 0; i < out; ++i) r[i] = x[i];
    free(x); free(q); free(rem); free(s);
    return out;
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
    t->frames[0].ret_ip = -1; t->frames[0].base = 0; t->frames[0].nargs = f->nargs; t->frames[0].handler_base = t->exception_handler_count; t->fp = 1;
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
/* POP() fails closed on underflow. The front-end bytecode compiler is supposed
 * to keep the operand stack balanced, but a codegen defect (e.g. an opcode path
 * that pops more than it pushes) must surface as a clean VM diagnostic, not an
 * out-of-bounds read of stack[-1]. On underflow we record a sticky flag and
 * return a null value; the per-instruction guard below converts that flag into
 * a TERR("stack underflow") before the corrupt result can be used. */
#define POP() (t->sp > 0 ? t->stack[--t->sp] : (t->underflow = 1, slval_null()))

static int native_utf8_valid(const uint8_t* s, uint32_t n);

/* Convert only values represented by the versioned native ABI. VM-local
 * arrays/structs, 128-bit integers, and all opaque handles stay in the VM. */
static int native_value_to_abi(SLVM* vm, SLValue source, SLNativeValue* target) {
    memset(target, 0, sizeof(*target));
    switch (source.type) {
        case SL_NULL: target->tag = SL_NATIVE_VALUE_NULL; return 1;
        case SL_BOOL:
            target->tag = SL_NATIVE_VALUE_BOOL;
            target->as.boolean = source.as.b ? 1 : 0;
            return 1;
        case SL_INT:
            target->tag = SL_NATIVE_VALUE_I64;
            target->as.i64 = source.as.i;
            return 1;
        case SL_DOUBLE:
            target->tag = SL_NATIVE_VALUE_F64;
            target->as.f64 = source.as.d;
            return 1;
        case SL_STR: {
            const char* value = slvm_str(vm, source.as.s);
            size_t length = strlen(value);
            if (length > SL_MAX_STRTEXT || length > UINT32_MAX ||
                !native_utf8_valid((const uint8_t*)value, (uint32_t)length)) return 0;
            target->tag = SL_NATIVE_VALUE_UTF8;
            target->as.utf8.data = (const uint8_t*)value;
            target->as.utf8.length = (uint32_t)length;
            return 1;
        }
        default: return 0;
    }
}

/* The VM currently stores String values as NUL-terminated interned strings.
 * Validate UTF-8 and reject embedded NUL on the return path rather than
 * silently truncating a plugin result when it is interned. */
static int native_utf8_valid(const uint8_t* s, uint32_t n) {
    uint32_t i = 0;
    if (n && !s) return 0;
    while (i < n) {
        uint8_t a = s[i++];
        if (a == 0) return 0;
        if (a < 0x80) continue;
        if (a >= 0xc2 && a <= 0xdf) {
            if (i >= n || (s[i++] & 0xc0) != 0x80) return 0;
        } else if (a >= 0xe0 && a <= 0xef) {
            uint8_t b, c;
            if (i + 1 >= n) return 0;
            b = s[i++]; c = s[i++];
            if ((c & 0xc0) != 0x80 || (b & 0xc0) != 0x80) return 0;
            if ((a == 0xe0 && b < 0xa0) || (a == 0xed && b >= 0xa0)) return 0;
        } else if (a >= 0xf0 && a <= 0xf4) {
            uint8_t b, c, d;
            if (i + 2 >= n) return 0;
            b = s[i++]; c = s[i++]; d = s[i++];
            if ((b & 0xc0) != 0x80 || (c & 0xc0) != 0x80 || (d & 0xc0) != 0x80) return 0;
            if ((a == 0xf0 && b < 0x90) || (a == 0xf4 && b >= 0x90)) return 0;
        } else return 0;
    }
    return 1;
}

static int native_value_from_abi(SLVM* vm, const SLNativeValue* source, SLValue* target) {
    if (!source || !target || source->reserved != 0) return 0;
    switch ((SLNativeValueTag)source->tag) {
        case SL_NATIVE_VALUE_NULL: *target = slval_null(); return 1;
        case SL_NATIVE_VALUE_BOOL:
            if (source->as.boolean != 0 && source->as.boolean != 1) return 0;
            *target = slval_bool(source->as.boolean); return 1;
        case SL_NATIVE_VALUE_I64: *target = slval_int(source->as.i64); return 1;
        case SL_NATIVE_VALUE_F64: *target = slval_double(source->as.f64); return 1;
        case SL_NATIVE_VALUE_UTF8: {
            uint32_t length = source->as.utf8.length;
            char* copy;
            if (length > SL_MAX_STRTEXT || !native_utf8_valid(source->as.utf8.data, length)) return 0;
            copy = (char*)malloc((size_t)length + 1u);
            if (!copy) return 0;
            if (length) memcpy(copy, source->as.utf8.data, length);
            copy[length] = '\0';
            target->type = SL_STR;
            target->as.s = intern(vm, copy);
            free(copy);
            return 1;
        }
        default: return 0;
    }
}

/* Native-to-SLeeLa callback bridge. Only exact, unique compiled function
 * names and scalar/UTF-8 arguments are accepted. The nested interpreter has
 * its own stack/frames; the suspended caller stack is added to GC roots. */
static int slvm_native_host_invoke(void* context, const char* function,
                                   uint32_t argc, const SLNativeValue* argv,
                                   SLNativeValue* out_result) {
    SLVM* vm = (SLVM*)context;
    SLThread* nested;
    int function_index = -1;
    size_t name_length;
    int active_workers;
    SLResult run_status;
    SLVM* previous_callback_vm;
    if (!vm || !function || !*function || !out_result ||
        (argc && !argv) || argc > STACK_MAX - 1 ||
        native_callback_vm == vm || native_plugin_call_vm != vm)
        return 0;
    /* A callback is valid only while OP_NATIVE_CALL has suspended a live
     * interpreter stack. Public host-side module_run/module_call operations
     * have no such stack to retain as a GC root, so reject those callbacks. */
    if (!vm->native_callback_caller) {
        set_err(vm, "native callback requires an active source-level nativeCall context");
        return 0;
    }
    pthread_mutex_lock(&vm->thr_mtx);
    active_workers = vm->nthreads;
    pthread_mutex_unlock(&vm->thr_mtx);
    if (active_workers != 0) {
        set_err(vm, "native callback requires a quiescent VM with no active worker threads");
        return 0;
    }
    if (!vm->native_callback_authorizer) {
        set_err(vm, "native callback denied: host callback-name policy is not installed");
        return 0;
    }
    {
        SLVM* previous_callback_vm = native_callback_vm;
        int authorized;
        native_callback_vm = vm;
        authorized = vm->native_callback_authorizer(
            function, vm->native_callback_authorizer_context);
        native_callback_vm = previous_callback_vm;
        if (!authorized) {
            set_err(vm, "native callback denied by host callback-name policy");
            return 0;
        }
    }
    name_length = strlen(function);
    if (name_length > 255u) return 0;
    for (int i = 0; i < vm->nfunc; ++i) {
        if (vm->funcs[i].nargs != (int)argc ||
            strcmp(vm->funcs[i].name, function) != 0) continue;
        if (function_index >= 0) {
            set_err(vm, "native callback rejected ambiguous SLeeLa function name");
            return 0;
        }
        function_index = i;
    }
    if (function_index < 0) {
        set_err(vm, "native callback could not resolve exact SLeeLa function and arity");
        return 0;
    }
    if (vm->funcs[function_index].nlocals < (int)argc ||
        vm->funcs[function_index].nlocals > STACK_MAX) {
        set_err(vm, "native callback target has an invalid local-frame size");
        return 0;
    }
    nested = (SLThread*)calloc(1, sizeof(*nested));
    if (!nested) {
        set_err(vm, "native callback could not allocate interpreter frame");
        return 0;
    }
    nested->vm = vm;
    nested->entry_func = function_index;
    nested->result = slval_null();
    nested->frames[0].ret_ip = -1;
    nested->frames[0].base = 0;
    nested->frames[0].nargs = (int)argc;
    nested->frames[0].handler_base = nested->exception_handler_count;
    nested->fp = 1;
    for (uint32_t i = 0; i < argc; ++i) {
        SLValue value;
        if (!native_value_from_abi(vm, &argv[i], &value)) {
            free(nested);
            set_err(vm, "native callback argument is invalid or unsupported");
            return 0;
        }
        nested->stack[nested->sp++] = value;
    }
    while (nested->sp < vm->funcs[function_index].nlocals)
        nested->stack[nested->sp++] = slval_null();

    previous_callback_vm = native_callback_vm;
    native_callback_vm = vm;
    run_status = run_thread(nested);
    native_callback_vm = previous_callback_vm;
    if (run_status != SLR_OK ||
        !native_value_to_abi(vm, nested->result, out_result)) {
        if (run_status == SLR_OK)
            set_err(vm, "native callback result is not representable by the host ABI");
        free(nested);
        return 0;
    }
    free(nested);
    return 1;
}

/* Fallback is deliberately name+arity based and remains inside the VM. It is
 * used only when the module has no typed entry, reports METHOD_NOT_HANDLED,
 * or an argument is not scalar ABI-representable; plugin execution errors
 * never silently fall back. */
static int native_bytecode_fallback(SLThread* t, const char* method,
                                    uint32_t argc, int base, int return_ip) {
    SLVM* vm = t->vm;
    int function = -1;
    /* Prefer an exact function key. If the compiler flattened a class method
     * as Class::method, accept its unqualified source method name only when
     * that name+arity resolves uniquely. */
    for (int i = 0; i < vm->nfunc; ++i) {
        if (vm->funcs[i].nargs == (int)argc &&
            strcmp(vm->funcs[i].name, method) == 0) {
            function = i;
            break;
        }
    }
    if (function < 0) {
        for (int i = 0; i < vm->nfunc; ++i) {
            const char* separator = strstr(vm->funcs[i].name, "::");
            if (vm->funcs[i].nargs != (int)argc || !separator ||
                strcmp(separator + 2, method) != 0) continue;
            if (function >= 0) return 0; /* ambiguous unqualified method */
            function = i;
        }
    }
    if (function < 0) return 0;
    if (t->fp >= FRAMES_MAX || base < 0 ||
        base + vm->funcs[function].nlocals > STACK_MAX) return -1;
    memmove(&t->stack[base], &t->stack[base + 2u], (size_t)argc * sizeof(SLValue));
    t->sp = base + (int)argc;
    SLFrame* frame = &t->frames[t->fp++];
    frame->ret_ip = return_ip;
    frame->base = base;
    frame->nargs = (int)argc;
    frame->handler_base = t->exception_handler_count;
    for (int i = (int)argc; i < vm->funcs[function].nlocals; ++i)
        t->stack[t->sp++] = slval_null();
    return vm->funcs[function].entry + 1; /* caller stores ip as entry-1 */
}

static SLResult run_thread(SLThread* t) {
    SLVM* vm = t->vm; int fi = t->entry_func;
    if (fi < 0 || fi >= vm->nfunc) TERR("no entry function set");
    SLFunc* ef = &vm->funcs[fi];
    if (t->fp == 0) {
        t->sp = 0; t->frames[0].ret_ip = -1; t->frames[0].base = 0; t->frames[0].nargs = ef->nargs; t->frames[0].handler_base = t->exception_handler_count; t->fp = 1;
        for (int i = 0; i < ef->nlocals; i++) PUSH(slval_null());
    }
    int ip = ef->entry;
    t->underflow = 0;
    for (;;) {
        if (t->underflow) TERR("stack underflow");
        if (ip < 0 || ip >= vm->codelen) TERR("ip out of range");
        SLInstr in = vm->code[ip++];
        /* Poll the GC safepoint once per stride rather than every instruction,
         * so a tight loop (e.g. filling a million-element array) does not pay a
         * thread-mutex per opcode. Allocation opcodes poll directly (see below),
         * so large allocations are never deferred past the heap threshold. */
        if (++t->safepoint_tick >= SL_GC_SAFEPOINT_STRIDE) {
            t->safepoint_tick = 0;
            slvm_gc_safepoint(vm, t, 4);
        }
        switch (in.op) {
        case OP_NOP: break;
        case OP_TRY_PUSH: {
            if (t->fp <= 0 || t->fp > FRAMES_MAX)
                TERR("exception handler registration requires an active call frame");
            if (in.a < 0 || in.a >= vm->codelen)
                TERR("exception handler target outside code");
            if (t->exception_handler_count >= EXCEPTION_HANDLERS_MAX)
                TERR("exception handler stack limit exceeded");
            SLExceptionHandler *handler =
                &t->exception_handlers[t->exception_handler_count++];
            handler->target_ip = in.a;
            handler->stack_height = t->sp;
            handler->frame_depth = t->fp;
            handler->owner_frame = t->fp - 1;
            handler->exception_value = slval_null();
            handler->has_exception_value = 0;
        } break;
        case OP_TRY_POP: {
            if (t->exception_handler_count <= 0)
                TERR("exception handler stack underflow");
            SLExceptionHandler *handler =
                &t->exception_handlers[t->exception_handler_count - 1];
            if (handler->owner_frame != t->fp - 1 ||
                handler->frame_depth != t->fp)
                TERR("exception handler removal crossed a call-frame boundary");
            --t->exception_handler_count;
        } break;
        case OP_THROW: {
            SLValue thrown = POP();
            if (t->underflow) TERR("throw requires an exception value");
            if (t->exception_handler_count <= 0) {
                t->uncaught_exception = thrown;
                t->has_uncaught_exception = 1;
                TERR("uncaught language exception");
            }
            int handler_index = t->exception_handler_count - 1;
            SLExceptionHandler handler = t->exception_handlers[handler_index];
            if (handler.frame_depth < 1 || handler.frame_depth > t->fp ||
                handler.owner_frame != handler.frame_depth - 1 ||
                handler.stack_height < 0 || handler.stack_height > STACK_MAX ||
                handler.target_ip < 0 || handler.target_ip >= vm->codelen ||
                handler.owner_frame >= FRAMES_MAX) {
                TERR("corrupt exception handler state");
            }
            /* Root the payload before truncating any stack slots. The dispatch
             * path performs no allocation or safepoint between truncation and
             * placing the value on the catch-entry operand stack. */
            t->pending_exception = thrown;
            t->has_pending_exception = 1;
            t->exception_handler_count = handler_index; /* selected handler is consumed */
            t->fp = handler.frame_depth;
            t->sp = handler.stack_height;
            PUSH(t->pending_exception);
            t->pending_exception = slval_null();
            t->has_pending_exception = 0;
            ip = handler.target_ip;
        } break;
        case OP_MATH_CALL: {
            /* One instruction crosses the interpreter/native boundary. Pop
             * metadata in reverse push order; validate every tag and signed
             * dimension before converting to size_t. The adapter owns the
             * array lock and stages data without retaining VM storage. */
            SLValue flags_v=POP(), scalar_v=POP();
            SLValue aux_cols_v=POP(), aux_rows_v=POP();
            SLValue out_cols_v=POP(), out_rows_v=POP();
            SLValue b_cols_v=POP(), b_rows_v=POP();
            SLValue a_cols_v=POP(), a_rows_v=POP();
            SLValue aux_v=POP(), out_v=POP(), b_v=POP(), a_v=POP();
            if(t->underflow) TERR("math call: incomplete operand frame");
            SLValue dims[8]={a_rows_v,a_cols_v,b_rows_v,b_cols_v,
                             out_rows_v,out_cols_v,aux_rows_v,aux_cols_v};
            size_t d[8];
            for(int k=0;k<8;k++) {
                if(dims[k].type!=SL_INT || dims[k].as.i<0 || (uint64_t)dims[k].as.i>(uint64_t)SIZE_MAX)
                    TERR("math call: dimensions must be nonnegative integers within size_t");
                d[k]=(size_t)dims[k].as.i;
            }
            if(flags_v.type!=SL_INT || flags_v.as.i<0 || (uint64_t)flags_v.as.i>(uint64_t)SIZE_MAX)
                TERR("math call: flags must be a nonnegative size_t value");
            if(scalar_v.type!=SL_INT && scalar_v.type!=SL_DOUBLE)
                TERR("math call: scalar parameter must be numeric");
            SLValue handles[4]={a_v,b_v,out_v,aux_v};
            int h[4];
            for(int k=0;k<4;k++) {
                if(handles[k].type==SL_ARRAY) h[k]=(int)handles[k].as.h;
                else if(handles[k].type==SL_INT && handles[k].as.i==-1) h[k]=-1;
                else TERR("math call: operands must be array values (-1 means absent)");
                if(h[k] < -1 || h[k] >= SL_MAX_ARRAYS)
                    TERR("math call: array handle is outside the supported range");
            }
            double scalar=(scalar_v.type==SL_DOUBLE)?scalar_v.as.d:(double)scalar_v.as.i;
            double scalar_result=0.0;
            int status=slvm_math_dispatch_arrays(vm,in.a,h[0],h[1],h[2],h[3],
                d[0],d[1],d[2],d[3],d[4],d[5],d[6],d[7],
                scalar,(size_t)flags_v.as.i,&scalar_result);
            if(status!=SLMATH_OK) {
                char message[160];
                snprintf(message,sizeof(message),"native math dispatch failed (status %d)",status);
                TERR(message);
            }
            if(in.a==SLMATH_OP_VECTOR_DOT || in.a==SLMATH_OP_VECTOR_NORM ||
               in.a==SLMATH_OP_MATRIX_DETERMINANT ||
               in.a==SLMATH_OP_COMPLEX_ABS || in.a==SLMATH_OP_COMPLEX_PHASE)
                PUSH(slval_double(scalar_result));
            else
                PUSH(slval_array(h[2]));
        } break;
        case OP_HALT: return SLR_HALT;
        case OP_GC_HINT: {
            /* Short-term GC cleanup requested by `x = gc N;` / `x = mem N;`.
             * operand a is the 0..100 aggressiveness. 0 is a no-op. Because this
             * is an EXPLICIT developer request (unlike the automatic
             * per-instruction safepoint above), it runs a COMPLETE collection
             * via gc_collect_with_roots -- clear, mark all reachable from the
             * roots, then sweep -- so a single hint actually reclaims the dead
             * objects (and frees their VM-local struct/array handle slots) it is
             * meant to clean up, rather than only advancing an incremental,
             * budgeted mark. The aggressiveness selects the scope: a modest
             * level runs a young-only collection (recent garbage, the common
             * post-use case), and the ceiling (SL_GC_HINT_FULL) runs a full
             * collection. Only unreachable objects are reclaimed; a hint never
             * frees a live value. Like every collection it is a VM safepoint, so
             * it is deferred while worker threads are active. */
            int level = in.a;
            if (level < 0) level = 0;
            if (level > SL_GC_HINT_MAX) level = SL_GC_HINT_MAX;
            if (level > 0) {
                pthread_mutex_lock(&vm->thr_mtx);
                int workers = vm->nthreads;
                pthread_mutex_unlock(&vm->thr_mtx);
                if (workers == 0) {
                    SLGCRootContext roots = { vm, t, vm->native_callback_caller };
                    int young_only = (level >= SL_GC_HINT_FULL) ? 0 : 1;
                    gc_collect_with_roots(&vm->gc, gc_mark_vm_roots, &roots,
                                          young_only);
                }
            }
        } break;
        case OP_SLEEP: {
            /* Block the current thread for the popped millisecond count. Backs
             * the sleep(ms) built-in, the per-iteration pause of the @loopDelay
             * / @loopBudget loop-timing annotations, and the one-shot
             * timer-start pause of the @spill loop annotation. The operand is
             * pushed just before (OP_CONST <ms>), so pop it here and read it as
             * an integer; 0 or negative is a no-op (sltime_sleep_millis clamps).
             * Non-integer operands are rejected. */
            SLValue ms = POP();
            if (t->underflow) { t->underflow = 0; TERR("sleep: stack underflow"); }
            if (ms.type != SL_INT && ms.type != SL_INT128) TERR("sleep: millisecond count must be an integer");
            int64_t millis = (int64_t)as_i128(ms);
            sltime_sleep_millis(millis);
        } break;
        case OP_RAN_MEM: {
            /* `ran::mem` -- the System memory already Ran in RAM, read as the
             * current live byte count. When the memory manager is enabled this
             * is its live_bytes; otherwise it reports the GC's tracked bytes, so
             * the value is always a real, consistent system figure. */
            int64_t bytes = 0;
            if (slmm_is_enabled()) {
                SLMMStats st; slmm_stats(&st); bytes = (int64_t)st.live_bytes;
            } else {
                bytes = (int64_t)gc_bytes(&vm->gc);
            }
            PUSH(slval_int(bytes));
        } break;
        case OP_CONST: PUSH(vm->consts[in.a]); break;
        case OP_JSTR_LENGTH: { SLValue value=POP(); int32_t n=0; if(value.type!=SL_UTF16 || slvm_java_string_length(vm,value,&n)!=SL_JAVA_STRING_OK) TERR("String.length requires a valid Java UTF-16 String"); PUSH(slval_int(n)); } break;
        case OP_JSTR_CHAR_AT: { SLValue index=POP(),value=POP(); uint16_t unit=0; if(value.type!=SL_UTF16 || slvm_java_string_char_at(vm,value,(int32_t)as_num(index),&unit)!=SL_JAVA_STRING_OK) TERR("String.charAt index out of range or invalid String"); PUSH(slval_int((int32_t)unit)); } break;
        case OP_JSTR_CODE_POINT_AT: { SLValue index=POP(),value=POP(); int32_t cp=0; if(value.type!=SL_UTF16 || slvm_java_string_code_point_at(vm,value,(int32_t)as_num(index),&cp)!=SL_JAVA_STRING_OK) TERR("String.codePointAt index out of range or invalid String"); PUSH(slval_int(cp)); } break;
        case OP_JSTR_CODE_POINT_COUNT: { SLValue end=POP(),begin=POP(),value=POP(); int32_t n=0; if(value.type!=SL_UTF16 || slvm_java_string_code_point_count(vm,value,(int32_t)as_num(begin),(int32_t)as_num(end),&n)!=SL_JAVA_STRING_OK) TERR("String.codePointCount range invalid"); PUSH(slval_int(n)); } break;
        case OP_JSTR_OFFSET_BY_CODE_POINTS: { SLValue offset=POP(),index=POP(),value=POP(); int32_t result=0; if(value.type!=SL_UTF16 || slvm_java_string_offset_by_code_points(vm,value,(int32_t)as_num(index),(int32_t)as_num(offset),&result)!=SL_JAVA_STRING_OK) TERR("String.offsetByCodePoints range invalid"); PUSH(slval_int(result)); } break;
        case OP_JSTR_SUBSTRING: { SLValue end=POP(),begin=POP(),value=POP(); if(value.type!=SL_UTF16 || slvm_java_string_root(vm,value)!=SL_JAVA_STRING_OK) TERR("String.substring requires a rooted Java UTF-16 String"); SLValue result=slvm_java_string_substring(vm,value,(int32_t)as_num(begin),(int32_t)as_num(end)); (void)slvm_java_string_unroot(vm,value); if(result.type!=SL_UTF16) TERR("String.substring range invalid"); PUSH(result); } break;
        case OP_JSTR_CONCAT: { SLValue right=POP(),left=POP(); if(left.type!=SL_UTF16 || right.type!=SL_UTF16) TERR("String.concat requires two Java UTF-16 Strings"); if(slvm_java_string_root(vm,left)!=SL_JAVA_STRING_OK) TERR("String.concat could not root left operand"); if(slvm_java_string_root(vm,right)!=SL_JAVA_STRING_OK) { (void)slvm_java_string_unroot(vm,left); TERR("String.concat could not root right operand"); } SLValue result=slvm_java_string_concat(vm,left,right); (void)slvm_java_string_unroot(vm,right); (void)slvm_java_string_unroot(vm,left); if(result.type!=SL_UTF16) TERR("String.concat failed"); PUSH(result); } break;
        case OP_JSTR_HASH_CODE: { SLValue value=POP(); int32_t hash=0; if(value.type!=SL_UTF16 || slvm_java_string_hash_code(vm,value,&hash)!=SL_JAVA_STRING_OK) TERR("String.hashCode requires a valid Java UTF-16 String"); PUSH(slval_int(hash)); } break;
        case OP_JSTR_COMPARE_TO: { SLValue right=POP(),left=POP(); int32_t order=0; if(left.type!=SL_UTF16 || right.type!=SL_UTF16 || slvm_java_string_compare_to(vm,left,right,&order)!=SL_JAVA_STRING_OK) TERR("String.compareTo requires two Java UTF-16 Strings"); PUSH(slval_int(order)); } break;
        case OP_JSTR_FROM_UTF8: { SLValue value=POP(); if(value.type!=SL_STR || value.as.s<0 || value.as.s>=vm->nstr) TERR("javaStringFromUtf8 requires a valid legacy UTF-8 String"); SLValue converted=slvm_java_string_from_utf8(vm,slvm_str(vm,value.as.s)); if(converted.type!=SL_UTF16) TERR("javaStringFromUtf8 rejected malformed UTF-8 or allocation failure"); PUSH(converted); } break;
        case OP_POP: (void)POP(); break;
        case OP_DUP: { if (t->sp < 1) TERR("stack underflow"); SLValue dupv = t->stack[t->sp-1]; PUSH(dupv); } break;
        case OP_DUP2: { if (t->sp < 2) TERR("stack underflow"); SLValue av = t->stack[t->sp-2], bv = t->stack[t->sp-1]; PUSH(av); PUSH(bv); } break;
        case OP_LOADG: { pthread_mutex_lock(&vm->global_mtx); SLValue v=vm->globals[in.a]; pthread_mutex_unlock(&vm->global_mtx); PUSH(v); } break;
        case OP_STOREG: { SLValue v=POP(); pthread_mutex_lock(&vm->global_mtx); vm->globals[in.a]=v; pthread_mutex_unlock(&vm->global_mtx); } break;
        case OP_LOADL: { SLFrame* f=&t->frames[t->fp-1]; int s=f->base+in.a; if(s<0||s>=STACK_MAX) TERR("local slot out of range"); PUSH(t->stack[s]); } break;
        case OP_STOREL: { SLFrame* f=&t->frames[t->fp-1]; int s=f->base+in.a; if(s<0||s>=STACK_MAX) TERR("local slot out of range"); t->stack[s]=POP(); } break;
        case OP_ADD: {
            SLValue b=POP(), a=POP();
            if (a.type==SL_UTF16 || b.type==SL_UTF16) {
                if (a.type != SL_UTF16 || b.type != SL_UTF16) TERR("Java String concatenation requires two UTF-16 String values");
                SLValue joined = slvm_java_string_concat(vm, a, b);
                if (joined.type != SL_UTF16) TERR("Java String concatenation failed");
                PUSH(joined);
            } else if (a.type==SL_STR || b.type==SL_STR) {
                /* Strings may be built up incrementally into large documents
                 * (generated HTML/CSS, DXF, JSON). Size the concat buffers from
                 * SL_MAX_STRTEXT so string assembly is not silently truncated.
                 * Heap-allocated to avoid a large VM stack frame. */
                size_t cap = (size_t)SL_MAX_STRTEXT;
                char* sa = (char*)malloc(cap);
                char* sb = (char*)malloc(cap);
                char* buf = (char*)malloc(cap*2);
                if (!sa || !sb || !buf) { free(sa); free(sb); free(buf); TERR("out of memory building String"); }
                value_to_text(vm,a,sa,cap);
                value_to_text(vm,b,sb,cap);
                snprintf(buf,cap*2,"%s%s",sa,sb); SLValue r; r.type=SL_STR; r.as.s=intern(vm,buf); PUSH(r);
                free(sa); free(sb); free(buf);
            } else if (any_i128(a,b) && both_any_int(a,b)) PUSH(slval_int128(as_i128(a)+as_i128(b)));
              else if (both_int(a,b)) PUSH(slval_int(a.as.i+b.as.i)); else PUSH(slval_double(as_num(a)+as_num(b)));
        } break;
        case OP_SUB: { SLValue b=POP(),a=POP(); if(any_i128(a,b)&&both_any_int(a,b)) PUSH(slval_int128(as_i128(a)-as_i128(b))); else if(both_int(a,b)) PUSH(slval_int(a.as.i-b.as.i)); else PUSH(slval_double(as_num(a)-as_num(b))); } break;
        case OP_MUL: { SLValue b=POP(),a=POP(); if(any_i128(a,b)&&both_any_int(a,b)) PUSH(slval_int128(as_i128(a)*as_i128(b))); else if(both_int(a,b)) PUSH(slval_int(a.as.i*b.as.i)); else PUSH(slval_double(as_num(a)*as_num(b))); } break;
        case OP_DIV: { SLValue b=POP(),a=POP(); if(any_i128(a,b)&&both_any_int(a,b)){ if(as_i128(b)==0) TERR("integer divide by zero"); PUSH(slval_int128(as_i128(a)/as_i128(b))); } else if(both_int(a,b)){if(!b.as.i) TERR("integer divide by zero"); PUSH(slval_int(a.as.i/b.as.i));} else PUSH(slval_double(as_num(a)/as_num(b))); } break;
        case OP_MOD: { SLValue b=POP(),a=POP(); if(any_i128(a,b)&&both_any_int(a,b)){ if(as_i128(b)==0) TERR("integer modulo by zero"); PUSH(slval_int128(as_i128(a)%as_i128(b))); } else if(both_int(a,b)){ if(!b.as.i) TERR("integer modulo by zero"); PUSH(slval_int(a.as.i%b.as.i)); } else { double db=as_num(b); if(db==0.0) TERR("modulo by zero"); PUSH(slval_double(fmod(as_num(a),db))); } } break;
        case OP_NEG: { SLValue a=POP(); if(a.type==SL_INT128) PUSH(slval_int128(-a.as.i128)); else if(a.type==SL_INT) PUSH(slval_int(-a.as.i)); else PUSH(slval_double(-as_num(a))); } break;
        case OP_EQ: { SLValue b=POP(),a=POP(); int e; if(a.type==SL_UTF16||b.type==SL_UTF16) { if(a.type!=SL_UTF16||b.type!=SL_UTF16) e=0; else if(slvm_java_string_equals(vm,a,b,&e)!=SL_JAVA_STRING_OK) TERR("invalid Java String value"); } else if(a.type==SL_NULL||b.type==SL_NULL) e=(a.type==SL_NULL&&b.type==SL_NULL); else if(a.type==SL_STRUCT||b.type==SL_STRUCT) e=(a.type==SL_STRUCT&&b.type==SL_STRUCT&&a.as.h==b.as.h); else if(a.type==SL_ARRAY||b.type==SL_ARRAY) e=(a.type==SL_ARRAY&&b.type==SL_ARRAY&&a.as.h==b.as.h); else if(a.type==SL_STR&&b.type==SL_STR) e=(a.as.s==b.as.s); else if(any_i128(a,b)&&both_any_int(a,b)) e=(as_i128(a)==as_i128(b)); else e=(as_num(a)==as_num(b)); PUSH(slval_bool(e)); } break;
        case OP_NE: { SLValue b=POP(),a=POP(); int e; if(a.type==SL_UTF16||b.type==SL_UTF16) { if(a.type!=SL_UTF16||b.type!=SL_UTF16) e=1; else if(slvm_java_string_equals(vm,a,b,&e)!=SL_JAVA_STRING_OK) TERR("invalid Java String value"); else e=!e; } else if(a.type==SL_NULL||b.type==SL_NULL) e=!(a.type==SL_NULL&&b.type==SL_NULL); else if(a.type==SL_STRUCT||b.type==SL_STRUCT) e=!(a.type==SL_STRUCT&&b.type==SL_STRUCT&&a.as.h==b.as.h); else if(a.type==SL_ARRAY||b.type==SL_ARRAY) e=!(a.type==SL_ARRAY&&b.type==SL_ARRAY&&a.as.h==b.as.h); else if(a.type==SL_STR&&b.type==SL_STR) e=(a.as.s!=b.as.s); else if(any_i128(a,b)&&both_any_int(a,b)) e=(as_i128(a)!=as_i128(b)); else e=(as_num(a)!=as_num(b)); PUSH(slval_bool(e)); } break;
        case OP_LT: { SLValue b=POP(),a=POP(); if(a.type==SL_UTF16||b.type==SL_UTF16) { int32_t order; if(a.type!=SL_UTF16||b.type!=SL_UTF16||slvm_java_string_compare_to(vm,a,b,&order)!=SL_JAVA_STRING_OK) TERR("invalid Java String comparison"); PUSH(slval_bool(order<0)); } else if(any_i128(a,b)&&both_any_int(a,b)) PUSH(slval_bool(as_i128(a)<as_i128(b))); else PUSH(slval_bool(as_num(a)<as_num(b))); } break;
        case OP_LE: { SLValue b=POP(),a=POP(); if(a.type==SL_UTF16||b.type==SL_UTF16) { int32_t order; if(a.type!=SL_UTF16||b.type!=SL_UTF16||slvm_java_string_compare_to(vm,a,b,&order)!=SL_JAVA_STRING_OK) TERR("invalid Java String comparison"); PUSH(slval_bool(order<=0)); } else if(any_i128(a,b)&&both_any_int(a,b)) PUSH(slval_bool(as_i128(a)<=as_i128(b))); else PUSH(slval_bool(as_num(a)<=as_num(b))); } break;
        case OP_GT: { SLValue b=POP(),a=POP(); if(a.type==SL_UTF16||b.type==SL_UTF16) { int32_t order; if(a.type!=SL_UTF16||b.type!=SL_UTF16||slvm_java_string_compare_to(vm,a,b,&order)!=SL_JAVA_STRING_OK) TERR("invalid Java String comparison"); PUSH(slval_bool(order>0)); } else if(any_i128(a,b)&&both_any_int(a,b)) PUSH(slval_bool(as_i128(a)>as_i128(b))); else PUSH(slval_bool(as_num(a)>as_num(b))); } break;
        case OP_GE: { SLValue b=POP(),a=POP(); if(a.type==SL_UTF16||b.type==SL_UTF16) { int32_t order; if(a.type!=SL_UTF16||b.type!=SL_UTF16||slvm_java_string_compare_to(vm,a,b,&order)!=SL_JAVA_STRING_OK) TERR("invalid Java String comparison"); PUSH(slval_bool(order>=0)); } else if(any_i128(a,b)&&both_any_int(a,b)) PUSH(slval_bool(as_i128(a)>=as_i128(b))); else PUSH(slval_bool(as_num(a)>=as_num(b))); } break;
        case OP_AND: { SLValue b=POP(),a=POP(); PUSH(slval_bool(is_truthy(a)&&is_truthy(b))); } break;
        case OP_OR: { SLValue b=POP(),a=POP(); PUSH(slval_bool(is_truthy(a)||is_truthy(b))); } break;
        case OP_NOT: { SLValue a=POP(); PUSH(slval_bool(!is_truthy(a))); } break;
        case OP_JMP: ip=in.a; break;
        case OP_JMPF: { SLValue c=POP(); if(!is_truthy(c)) ip=in.a; } break;
        case OP_CALL: {
            int cfi=in.a; if(cfi<0||cfi>=vm->nfunc) TERR("call to unknown function");
            SLFunc* f=&vm->funcs[cfi]; if(t->fp>=FRAMES_MAX) TERR("call depth exceeded");
            int base=t->sp-f->nargs; if(base<0) TERR("call: not enough arguments on stack");
            SLFrame* nf=&t->frames[t->fp++]; nf->ret_ip=ip; nf->base=base; nf->nargs=f->nargs; nf->handler_base=t->exception_handler_count;
            for(int i=f->nargs;i<f->nlocals;i++){ PUSH(slval_null()); } ip=f->entry;
        } break;
        case OP_NATIVE_OPEN: {
            if (native_callback_vm == vm) TERR("native callback cannot recursively invoke native-module operations");
            SLValue path_value = POP();
            SLNativeHandle handle = SL_NATIVE_HANDLE_INVALID;
            SLNativeStatus status;
            if (path_value.type != SL_STR) TERR("nativeOpen: path must be a String");
            status = slvm_native_module_open(vm, slvm_str(vm, path_value.as.s), &handle);
            if (status != SL_NATIVE_OK) {
                char message[256];
                snprintf(message, sizeof(message), "nativeOpen denied or failed: %s",
                         sl_native_status_string(status));
                TERR(message);
            }
            PUSH(slval_int((int64_t)handle));
        } break;
        case OP_NATIVE_CLOSE: {
            if (native_callback_vm == vm) TERR("native callback cannot recursively invoke native-module operations");
            SLValue handle_value = POP();
            SLNativeStatus status;
            if (handle_value.type != SL_INT || handle_value.as.i <= 0 ||
                (uint64_t)handle_value.as.i > UINT32_MAX)
                TERR("nativeClose: handle must be a valid positive native handle");
            status = slvm_native_module_close(vm, (SLNativeHandle)handle_value.as.i);
            if (status != SL_NATIVE_OK) {
                char message[256];
                snprintf(message, sizeof(message), "nativeClose failed: %s",
                         sl_native_status_string(status));
                TERR(message);
            }
            PUSH(slval_null());
        } break;
        case OP_NATIVE_CALL: {
            if (native_callback_vm == vm)
                TERR("native callback cannot recursively invoke native-module operations");
            uint32_t argc;
            int base, fallback_ip = -1, unsupported = 0;
            SLValue module_value, method_value, result_value = slval_null();
            SLNativeHandle handle;
            const char* method;
            SLNativeValue* argv = NULL;
            SLNativeValue native_result;
            SLNativeStatus status;
            if (in.a < 0 || in.a > STACK_MAX - 2)
                TERR("native call: argument count outside operand-stack limit");
            argc = (uint32_t)in.a;
            if (t->sp < (int)argc + 2) TERR("native call: expected module handle, method name, and arguments");
            base = t->sp - (int)argc - 2;
            module_value = t->stack[base];
            method_value = t->stack[base + 1];
            if (module_value.type != SL_INT || module_value.as.i <= 0 ||
                (uint64_t)module_value.as.i > UINT32_MAX)
                TERR("native call: module handle must be a valid positive native handle");
            if (method_value.type != SL_STR) TERR("native call: method name must be a String");
            method = slvm_str(vm, method_value.as.s);
            if (!method[0]) TERR("native call: method name cannot be empty");
            handle = (SLNativeHandle)module_value.as.i;
            if (argc) {
                argv = (SLNativeValue*)calloc(argc, sizeof(*argv));
                if (!argv) TERR("native call: out of memory preparing arguments");
            }
            for (uint32_t i = 0; i < argc; ++i) {
                if (!native_value_to_abi(vm, t->stack[base + 2 + (int)i], &argv[i])) {
                    unsupported = 1;
                    break;
                }
            }
            if (unsupported) {
                int handle_valid;
                pthread_mutex_lock(&vm->native_mtx);
                handle_valid = sl_native_table_is_valid(&vm->native_modules, handle);
                pthread_mutex_unlock(&vm->native_mtx);
                free(argv);
                if (!handle_valid) TERR("native call: stale or invalid module handle");
                fallback_ip = native_bytecode_fallback(t, method, argc, base, ip);
                if (fallback_ip == 0) TERR("native call: argument type is VM-owned and no matching SLeeLa fallback exists");
                if (fallback_ip < 0) TERR("native call: fallback call depth or stack limit exceeded");
                ip = fallback_ip - 1;
                break;
            }
            memset(&native_result, 0, sizeof(native_result));
            native_result.tag = SL_NATIVE_VALUE_NULL;
            pthread_mutex_lock(&vm->native_mtx);
            status = slvm_native_bind_module_host_locked(vm, handle);
            if (status == SL_NATIVE_OK) {
                SLVM* previous_plugin_vm = native_plugin_call_vm;
                native_plugin_call_vm = vm;
                vm->native_callback_caller = t;
                status = sl_native_table_call(&vm->native_modules, handle, method, argc, argv, &native_result);
                vm->native_callback_caller = NULL;
                native_plugin_call_vm = previous_plugin_vm;
            }
            if (status == SL_NATIVE_OK &&
                !native_value_from_abi(vm, &native_result, &result_value))
                status = SL_NATIVE_PLUGIN_ERROR;
            pthread_mutex_unlock(&vm->native_mtx);
            free(argv);
            if (status == SL_NATIVE_TYPED_ENTRY_NOT_FOUND ||
                status == SL_NATIVE_METHOD_NOT_HANDLED) {
                fallback_ip = native_bytecode_fallback(t, method, argc, base, ip);
                if (fallback_ip == 0) TERR("native call: no typed native method or matching SLeeLa fallback exists");
                if (fallback_ip < 0) TERR("native call: fallback call depth or stack limit exceeded");
                ip = fallback_ip - 1;
                break;
            }
            if (status != SL_NATIVE_OK) {
                char message[256];
                snprintf(message, sizeof(message), "native call '%s' failed: %s", method,
                         sl_native_status_string(status));
                TERR(message);
            }
            t->sp = base;
            PUSH(result_value);
        } break;
        case OP_RET: {
            SLValue rv=(t->sp>t->frames[t->fp-1].base)?POP():slval_null(); SLFrame f=t->frames[--t->fp]; if (t->exception_handler_count > f.handler_base) t->exception_handler_count = f.handler_base; t->sp=f.base;
            if(f.ret_ip<0){t->result=rv; return SLR_OK;} PUSH(rv); ip=f.ret_ip;
        } break;
        case OP_PRINT: { SLValue v=POP(); pthread_mutex_lock(&vm->print_mtx); print_value(vm,v); printf("\n"); pthread_mutex_unlock(&vm->print_mtx); } break;
        case OP_SPAWN: {
            if (native_callback_vm == vm) TERR("native callback cannot spawn VM worker threads");
            int sfi=in.a; if(sfi<0||sfi>=vm->nfunc) TERR("spawn of unknown function"); int nargs=vm->funcs[sfi].nargs; if(t->sp<nargs) TERR("spawn: not enough arguments on stack");
            SLValue args[FRAMES_MAX]; for(int i=0;i<nargs;i++) args[i]=t->stack[t->sp-nargs+i]; t->sp-=nargs; PUSH(slval_int(spawn_thread(vm,sfi,args,nargs)));
            /* A worker now exists; refresh immediately so the array opcodes take
             * the mutex-guarded path right away rather than waiting for the next
             * batched safepoint (which could be up to a stride away). */
            { pthread_mutex_lock(&vm->thr_mtx); t->workers_cached = vm->nthreads; pthread_mutex_unlock(&vm->thr_mtx); }
        } break;
        case OP_JOINALL:
            if (native_callback_vm == vm) TERR("native callback cannot join VM worker threads");
            slvm_joinall(vm); break;
        case OP_LOCK: if(in.a<0||in.a>=SL_MAX_LOCKS) TERR("lock id out of range"); pthread_mutex_lock(&vm->locks[in.a]); break;
        case OP_UNLOCK: if(in.a<0||in.a>=SL_MAX_LOCKS) TERR("lock id out of range"); pthread_mutex_unlock(&vm->locks[in.a]); break;
        case OP_SEND: {
            if(in.a<0||in.a>=SL_MAX_LOCKS){ TERR("mailbox slot out of range"); } SLMailbox* mb=&vm->mailbox[in.a]; SLValue v=POP();
            pthread_mutex_lock(&mb->mtx); while(mb->has) pthread_cond_wait(&mb->cond,&mb->mtx); mb->has=1; mb->tag=in.a; mb->value=v; pthread_cond_broadcast(&mb->cond); pthread_mutex_unlock(&mb->mtx);
        } break;
        case OP_RECV: {
            if(in.a<0||in.a>=SL_MAX_LOCKS){ TERR("mailbox slot out of range"); } SLMailbox* mb=&vm->mailbox[in.a]; pthread_mutex_lock(&mb->mtx);
            while(!mb->has){ pthread_cond_wait(&mb->cond,&mb->mtx); } SLValue v=mb->value; mb->has=0; pthread_cond_broadcast(&mb->cond); pthread_mutex_unlock(&mb->mtx); PUSH(v);
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
        /* OP_SSEND: the ::: structural-offload send. Operands were emitted
         * source-then-receiver, so the receiver reach handle is on top. The VM
         * frames the String source element-by-element (one UTF-8 code point per
         * frame) into the receiver via the existing Munction SDPS transport --
         * the structural iteration/framing is performed HERE, not by a source
         * for-loop. Pushes the SOURCE back so the same String can keep flowing
         * to further receivers (`x ::: a ::: b` fans x out to a then b). */
        case OP_SSEND: {
            SLValue rv=POP(), sv=POP();
            if(rv.type!=SL_INT||sv.type!=SL_STR) TERR("::: structural send requires a String source and a receiver reach handle");
            SLMunction* m=munction_get(vm,(int)rv.as.i);
            if(m){
                const char* s=slvm_str(vm,sv.as.s);
                size_t i=0;
                while(s[i]!='\0'){
                    /* one UTF-8 code point = one structural element/frame */
                    unsigned char c0=(unsigned char)s[i];
                    size_t clen=1;
                    if(c0>=0xF0) clen=4; else if(c0>=0xE0) clen=3; else if(c0>=0xC0) clen=2;
                    size_t j=0; char elem[8];
                    while(j<clen && s[i+j]!='\0'){ elem[j]=s[i+j]; j++; }
                    elem[j]='\0';
                    slmunction_send(m,elem,j);
                    i+=j;
                }
            }
            PUSH(sv);
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

        /* ---- Idea System(TM) realtime canvas (native-CPU render+physics) --- */
        case OP_IC_CREATE: {
            SLValue rowsv=POP(), colsv=POP();
            if(colsv.type!=SL_INT||rowsv.type!=SL_INT) TERR("ideaCanvasCreate(cols, rows) requires two integers");
            void* c=sl_idea_canvas_create((int)colsv.as.i,(int)rowsv.as.i);
            if(!c){PUSH(slval_int(-1));break;}
            int h=ic_alloc(vm,c); if(h<0){sl_idea_canvas_destroy(c);PUSH(slval_int(-1));break;}
            PUSH(slval_int(h));
        } break;
        case OP_IC_DESTROY: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasDestroy(handle) requires a handle");
            ic_release(vm,(int)hv.as.i); PUSH(slval_null());
        } break;
        case OP_IC_COLS: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasCols(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_cols(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_ROWS: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasRows(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_rows(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_COUNT: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasCount(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_count(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_SETPARAMS: {
            SLValue kv=POP(), hbv=POP(), fgv=POP(), dcv=POP(), hv=POP();
            if(hv.type!=SL_INT) TERR("ideaCanvasSetParams(handle, decay, flowGain, hueBase, coupling) requires a handle and four numbers");
            double dc=(dcv.type==SL_DOUBLE)?dcv.as.d:(double)dcv.as.i;
            double fg=(fgv.type==SL_DOUBLE)?fgv.as.d:(double)fgv.as.i;
            double hb=(hbv.type==SL_DOUBLE)?hbv.as.d:(double)hbv.as.i;
            double k =(kv.type==SL_DOUBLE)?kv.as.d:(double)kv.as.i;
            sl_idea_canvas_set_params(ic_get(vm,(int)hv.as.i),dc,fg,hb,k); PUSH(hv);
        } break;
        case OP_IC_EXCITE: {
            SLValue av=POP(), rv=POP(), cv=POP(), hv=POP();
            if(hv.type!=SL_INT||cv.type!=SL_INT||rv.type!=SL_INT) TERR("ideaCanvasExcite(handle, col, row, amount) requires handle, col, row integers and an amount");
            double amt=(av.type==SL_DOUBLE)?av.as.d:(double)av.as.i;
            PUSH(slval_int(sl_idea_canvas_excite(ic_get(vm,(int)hv.as.i),(int)cv.as.i,(int)rv.as.i,amt)));
        } break;
        case OP_IC_STEP: {
            SLValue dtv=POP(), hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasStep(handle, dt) requires a handle and dt");
            double dt=(dtv.type==SL_DOUBLE)?dtv.as.d:(double)dtv.as.i;
            PUSH(slval_int(sl_idea_canvas_step(ic_get(vm,(int)hv.as.i),dt)));
        } break;
        case OP_IC_ADVANCE: {
            SLValue dtv=POP(), hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasAdvance(handle, frameDt) requires a handle and frameDt");
            double dt=(dtv.type==SL_DOUBLE)?dtv.as.d:(double)dtv.as.i;
            PUSH(slval_int(sl_idea_canvas_advance(ic_get(vm,(int)hv.as.i),dt)));
        } break;
        case OP_IC_TIMESTEP: {
            PUSH(slval_double(sl_idea_canvas_timestep()));
        } break;
        case OP_IC_INTENSITY: {
            SLValue rv=POP(), cv=POP(), hv=POP();
            if(hv.type!=SL_INT||cv.type!=SL_INT||rv.type!=SL_INT) TERR("ideaCanvasIntensityAt(handle, col, row) requires three integers");
            PUSH(slval_double(sl_idea_canvas_intensity_at(ic_get(vm,(int)hv.as.i),(int)cv.as.i,(int)rv.as.i)));
        } break;
        case OP_IC_HASREND: {
            PUSH(slval_int(sl_idea_canvas_has_renderer()));
        } break;
        case OP_IC_OPENWIN: {
            SLValue tv=POP(), hgt=POP(), wdt=POP(), hv=POP();
            if(hv.type!=SL_INT||wdt.type!=SL_INT||hgt.type!=SL_INT||tv.type!=SL_STR) TERR("ideaCanvasOpenWindow(handle, width, height, title) requires handle, width, height ints and a title String");
            PUSH(slval_int(sl_idea_canvas_open_window(ic_get(vm,(int)hv.as.i),(int)wdt.as.i,(int)hgt.as.i,slvm_str(vm,tv.as.s))));
        } break;
        case OP_IC_SHOULDCLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasShouldClose(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_should_close(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_PRESENT: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasPresent(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_present(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_RUN: {
            SLValue tv=POP(), hgt=POP(), wdt=POP(), hv=POP();
            if(hv.type!=SL_INT||wdt.type!=SL_INT||hgt.type!=SL_INT||tv.type!=SL_STR) TERR("ideaCanvasRun(handle, width, height, title) requires handle, width, height ints and a title String");
            PUSH(slval_int(sl_idea_canvas_run(ic_get(vm,(int)hv.as.i),(int)wdt.as.i,(int)hgt.as.i,slvm_str(vm,tv.as.s))));
        } break;
        case OP_IC_SETPOLICY: {
            SLValue pv=POP(), hv=POP();
            if(hv.type!=SL_INT||pv.type!=SL_INT) TERR("ideaCanvasSetBackendPolicy(handle, policy) requires a handle and a policy int (0 auto,1 cpu,2 gpu)");
            sl_idea_canvas_set_policy(ic_get(vm,(int)hv.as.i),(int)pv.as.i); PUSH(hv);
        } break;
        case OP_IC_POLICY: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasPolicy(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_policy(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_BACKEND: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasActiveBackend(handle) requires a handle");
            PUSH(slval_int(sl_idea_canvas_active_backend(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_GPUAVAIL: {
            PUSH(slval_int(sl_idea_canvas_gpu_available()));
        } break;
        case OP_IC_MEASHZ: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("ideaCanvasMeasuredHz(handle) requires a handle");
            PUSH(slval_double(sl_idea_canvas_measured_hz(ic_get(vm,(int)hv.as.i))));
        } break;
        case OP_IC_MIGRATE: {
            SLValue tv=POP(), hv=POP();
            if(hv.type!=SL_INT||tv.type!=SL_INT) TERR("ideaCanvasMigrate(handle, target) requires a handle and target int (0 cpu,1 gpu)");
            PUSH(slval_int(sl_idea_canvas_migrate(ic_get(vm,(int)hv.as.i),(int)tv.as.i)));
        } break;

        /* ---- VM selection service (SLVmRegistry seam) ----------------- */
        case OP_VM_SELECT: {
            /* pops a VM version number. This is the service-exit seam a Sleela
             * VM selector (virtual-machine/vm-version/SLVmRegistry) uses to ask
             * the host to build+boot the chosen version. Valid range is 1..11:
             * the ten standard think-tank versions (1..10) and the eleven
             * premium SLVM generations (1..11, the broader range), each a real
             * runnable VM under sleela-virtual-machine/0NN that links the shared
             * Core engine. The core validates the request and pushes a status
             * (0 ok, -1 out of range); the actual build+boot is a host action
             * signalled by this opcode (an unknown opcode is never a no-op). */
            SLValue nv=POP();
            if(nv.type!=SL_INT) TERR("vmSelect(number) requires an int version number");
            int sel=(int)nv.as.i;
            PUSH(slval_int((sel>=1 && sel<=11) ? 0 : -1));
        } break;

        /* ---- Modium file-resolution scan (SLVmModium seam) ----------- */
        case OP_MODIUM_SCAN: {
            /* pops a VM version number (1..10). The service-exit seam the Sleela
             * SLVmModium uses to ask the host to scan that version's files
             * (exist/placed/intact per its MODIUM.json). The core validates the
             * request and pushes a status (0 ok, -1 out of range); the actual
             * file scan + reconstitution from rebuild.sql is a host action
             * signalled by this opcode. */
            SLValue mv=POP();
            if(mv.type!=SL_INT) TERR("modiumScan(number) requires an int version number");
            int msel=(int)mv.as.i;
            PUSH(slval_int((msel>=1 && msel<=10) ? 0 : -1));
        } break;

        /* ---- SleelaUI bridge: ui* built-ins -> native toolkit --------- */
        case OP_UI_CALL: {
            int ui_id = in.a;
            int ui_n = sleela_ui_arity(ui_id);
            if (ui_n < 0 || ui_n > 6) TERR("ui call: bad arity");
            SLUIArg ui_args[6];
            /* Args were pushed left-to-right, so pop into reverse slots. */
            for (int k = ui_n - 1; k >= 0; --k) {
                SLValue v = POP();
                if (v.type == SL_STR)       { ui_args[k].kind = SLUI_ARG_STR;    ui_args[k].s = slvm_str(vm, v.as.s); ui_args[k].i = 0; ui_args[k].d = 0; }
                else if (v.type == SL_DOUBLE){ ui_args[k].kind = SLUI_ARG_DOUBLE; ui_args[k].d = v.as.d; ui_args[k].i = 0; ui_args[k].s = NULL; }
                else                         { ui_args[k].kind = SLUI_ARG_INT;    ui_args[k].i = (long)v.as.i; ui_args[k].d = 0; ui_args[k].s = NULL; }
            }
            SLUIArg ui_r = sleela_ui_dispatch(ui_id, ui_args, ui_n);
            if (ui_r.kind == SLUI_ARG_STR) { SLValue o; o.type = SL_STR; o.as.s = intern(vm, ui_r.s ? ui_r.s : ""); PUSH(o); }
            else if (ui_r.kind == SLUI_ARG_DOUBLE) { PUSH(slval_double(ui_r.d)); }
            else { PUSH(slval_int(ui_r.i)); }
        } break;

        /* ---- SLVM Audio bridge --------------------------------------- */
        case OP_AUDIO_NEW: {
            SLValue outv=POP(), ratev=POP();
            if(outv.type!=SL_STR||ratev.type!=SL_INT||ratev.as.i<=0) TERR("audioNew(sampleRate, outputPath) requires positive rate and String output path");
            int h=-1;
            pthread_mutex_lock(&vm->audio_mtx);
            for(int i=0;i<64;i++) if(!vm->audio_jobs[i].active){ h=i; SLAudioJob *j=&vm->audio_jobs[i]; memset(j,0,sizeof(*j)); j->active=1; j->sample_rate=(uint32_t)ratev.as.i; strncpy(j->output_path,slvm_str(vm,outv.as.s),sizeof(j->output_path)-1); j->left_gain=1.0; j->right_gain=1.0; break; }
            pthread_mutex_unlock(&vm->audio_mtx);
            if(h<0) TERR("audioNew: audio job table exhausted");
            PUSH(slval_int(h));
        } break;
        case OP_AUDIO_ADD: {
            SLValue gainv=POP(), startv=POP(), pathv=POP(), hv=POP();
            if(hv.type!=SL_INT||pathv.type!=SL_STR||(startv.type!=SL_DOUBLE&&startv.type!=SL_INT)||(gainv.type!=SL_DOUBLE&&gainv.type!=SL_INT)) TERR("audioAdd(handle, path, startSeconds, gainDb) has invalid arguments");
            int h=(int)hv.as.i; if(h<0||h>=64||!vm->audio_jobs[h].active) TERR("audioAdd: invalid audio handle");
            pthread_mutex_lock(&vm->audio_mtx); SLAudioJob *j=&vm->audio_jobs[h];
            if(j->input_count>=128){pthread_mutex_unlock(&vm->audio_mtx);TERR("audioAdd: maximum 128 inputs exceeded");}
            strncpy(j->input_paths[j->input_count],slvm_str(vm,pathv.as.s),sizeof(j->input_paths[0])-1);
            j->start_seconds[j->input_count]=(startv.type==SL_DOUBLE)?startv.as.d:(double)startv.as.i;
            j->gain_db[j->input_count]=(gainv.type==SL_DOUBLE)?gainv.as.d:(double)gainv.as.i;
            j->input_count++; pthread_mutex_unlock(&vm->audio_mtx); PUSH(hv);
        } break;
        case OP_AUDIO_CONTROLS: {
            SLValue rv=POP(), lv=POP(), pv=POP(), mgv=POP(), tv=POP(), mv=POP(), bv=POP(), hv=POP();
            if(hv.type!=SL_INT) TERR("audioControls requires an audio handle");
            SLValue vv[7]={bv,mv,tv,mgv,pv,lv,rv}; for(int i=0;i<7;i++) if(vv[i].type!=SL_DOUBLE&&vv[i].type!=SL_INT) TERR("audioControls requires numeric controls");
            int h=(int)hv.as.i; if(h<0||h>=64||!vm->audio_jobs[h].active) TERR("audioControls: invalid audio handle");
            pthread_mutex_lock(&vm->audio_mtx); SLAudioJob *j=&vm->audio_jobs[h];
#define SL_AUDIO_NUM(v) ((v).type==SL_DOUBLE?(v).as.d:(double)(v).as.i)
            j->bass_db=SL_AUDIO_NUM(bv); j->mid_db=SL_AUDIO_NUM(mv); j->treble_db=SL_AUDIO_NUM(tv); j->master_gain_db=SL_AUDIO_NUM(mgv); j->pan=SL_AUDIO_NUM(pv); j->left_gain=SL_AUDIO_NUM(lv); j->right_gain=SL_AUDIO_NUM(rv);
#undef SL_AUDIO_NUM
            pthread_mutex_unlock(&vm->audio_mtx); PUSH(hv);
        } break;
        case OP_AUDIO_VALIDATE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("audioValidate(handle) requires an audio handle");
            int h=(int)hv.as.i; int ok=0; pthread_mutex_lock(&vm->audio_mtx);
            if(h>=0&&h<64&&vm->audio_jobs[h].active){SLAudioJob*j=&vm->audio_jobs[h]; ok=j->sample_rate>0&&j->output_path[0]&&j->input_count>0&&j->input_count<=128&&isfinite(j->pan)&&j->pan>=-1.0&&j->pan<=1.0; for(size_t i=0;i<j->input_count&&ok;i++) ok=j->input_paths[i][0]&&j->start_seconds[i]>=0.0&&isfinite(j->start_seconds[i])&&isfinite(j->gain_db[i]);}
            pthread_mutex_unlock(&vm->audio_mtx); PUSH(slval_bool(ok));
        } break;
        case OP_AUDIO_RENDER: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("audioRender(handle) requires an audio handle");
            int h=(int)hv.as.i; if(h<0||h>=64) TERR("audioRender: invalid audio handle");
            pthread_mutex_lock(&vm->audio_mtx); SLAudioJob j=vm->audio_jobs[h]; SLAudioNativeRenderFn fn=vm->audio_renderer; void *user=vm->audio_renderer_user; pthread_mutex_unlock(&vm->audio_mtx);
            if(!j.active||!fn) PUSH(slval_bool(0)); else {
                const char* paths[128]; for(size_t i=0;i<j.input_count;i++) paths[i]=j.input_paths[i];
                int rc=fn(j.output_path,j.sample_rate,paths,j.start_seconds,j.gain_db,j.input_count,j.bass_db,j.mid_db,j.treble_db,j.master_gain_db,j.pan,j.left_gain,j.right_gain,user);
                PUSH(slval_bool(rc==0));
            }
        } break;
        case OP_AUDIO_CLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("audioClose(handle) requires an audio handle");
            int h=(int)hv.as.i; if(h<0||h>=64) TERR("audioClose: invalid audio handle");
            pthread_mutex_lock(&vm->audio_mtx); memset(&vm->audio_jobs[h],0,sizeof(vm->audio_jobs[h])); pthread_mutex_unlock(&vm->audio_mtx); PUSH(slval_null());
        } break;
        case OP_AUDIO_PLATFORM: {
#if defined(_WIN32)
            const char *p="windows";
#elif defined(__APPLE__)
            const char *p="macos";
#elif defined(__linux__)
            const char *p="linux";
#else
            const char *p="unknown";
#endif
            SLValue s; s.type=SL_STR; s.as.s=intern(vm,p); PUSH(s);
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
            t->safepoint_tick = 0;
            slvm_gc_safepoint(vm, t, 4);
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
            SLValue old = si->fields[in.a];
            if (old.type == SL_STRUCT || old.type == SL_ARRAY || val.type == SL_STRUCT || val.type == SL_ARRAY)
                gc_write_barrier(&vm->gc, si->gc_object, gc_object_for_value(vm, old), gc_object_for_value(vm, val));
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
        case OP_CHECKCAST: {
            SLValue value=POP();
            if(in.a>=0) {
                if(value.type==SL_NULL) { PUSH(value); break; }
                if(value.type!=SL_STRUCT || !struct_valid_handle(vm,value.as.h))
                    TERR("checked cast failed: value is not a valid class/struct instance");
                pthread_mutex_lock(&vm->struct_mtx);
                int actualType=vm->structs[value.as.h].type;
                int matches=struct_type_is_a(vm,actualType,in.a);
                pthread_mutex_unlock(&vm->struct_mtx);
                if(!matches) TERR("checked cast failed: runtime class/struct type does not match target");
            } else {
                int targetTag=-in.a-1;
                if(targetTag<SL_NULL || targetTag>SL_INT128)
                    TERR("invalid checked-cast type descriptor");
                if(value.type==SL_NULL && (targetTag==SL_STR || targetTag==SL_ARRAY)) {
                    PUSH(value); break;
                }
                /* int128 casts widen an integer to the 128-bit representation
                 * (this is how `(int128)x` and int128-typed storage produce a
                 * real 128-bit value), and allow the reverse narrowing. */
                if(targetTag==SL_INT128 && (value.type==SL_INT||value.type==SL_INT128)) {
                    PUSH(slval_int128(as_i128(value))); break;
                }
                if(targetTag==SL_INT && value.type==SL_INT128) {
                    PUSH(slval_int((int64_t)value.as.i128)); break;
                }
                /* Java-style narrowing from double to an integer: `(int)d`
                 * truncates toward zero (C's double->int64_t conversion has the
                 * same rule). Without this a cast of a double to an int target
                 * -- e.g. `(int) uiWidgetGetValue(slider)` feeding an int param,
                 * or any `int n = (int) someDouble;` -- fell through to the
                 * fail-closed tag check below and aborted with "runtime value
                 * tag does not match target", even though double->int is a
                 * well-defined narrowing. Mirror the int128 narrowing above. */
                if(targetTag==SL_INT && value.type==SL_DOUBLE) {
                    PUSH(slval_int((int64_t)value.as.d)); break;
                }
                if(targetTag==SL_INT128 && value.type==SL_DOUBLE) {
                    PUSH(slval_int128((sl_int128_t)value.as.d)); break;
                }
                /* Java-style widening to double: an integer stored into, cast
                 * to, or passed as a double becomes a double (so `double d = 3;`
                 * then `d / 2` is 1.5, not integer 1). An already-double value
                 * passes through; any other value (null etc.) is passed through
                 * unchanged so a compiler-inserted double coercion can be applied
                 * unconditionally at a double-typed store without regressing
                 * non-numeric cases. */
                if(targetTag==SL_DOUBLE) {
                    if(value.type==SL_INT||value.type==SL_INT128) PUSH(slval_double(as_num(value)));
                    else PUSH(value);
                    break;
                }
                if((int)value.type!=targetTag)
                    TERR("checked cast failed: runtime value tag does not match target");
            }
            PUSH(value);
        } break;
        case OP_INSTANCEOF: {
            SLValue value=POP();
            int matches=0;
            if(in.a>=0) {
                if(value.type==SL_STRUCT && struct_valid_handle(vm,value.as.h)) {
                    pthread_mutex_lock(&vm->struct_mtx);
                    matches=struct_type_is_a(vm,vm->structs[value.as.h].type,in.a);
                    pthread_mutex_unlock(&vm->struct_mtx);
                }
            } else {
                int targetTag=-in.a-1;
                if(targetTag<SL_NULL || targetTag>SL_INT128)
                    TERR("invalid instanceof type descriptor");
                matches=((int)value.type==targetTag);
            }
            PUSH(slval_bool(matches));
        } break;
        /* ---- arrays: dynamic, zero-indexed SLValue sequences ------------ */
        case OP_NEWARRAY: {
            SLValue nv=POP();
            if(nv.type!=SL_INT) TERR("array size must be an integer");
            /* Allocation opcode: poll the GC directly so a burst of large
             * allocations within one safepoint stride cannot outrun collection. */
            t->safepoint_tick = 0;
            slvm_gc_safepoint(vm, t, 4);
            int h=array_alloc(vm,(int)nv.as.i);
            if(h<0){
                /* The live-array table (SL_MAX_ARRAYS) or the heap is full.
                 * Before failing, run a COMPLETE root-aware collection (the same
                 * one OP_GC_HINT uses) and retry once: allocation-heavy code
                 * such as the binary-splitting Chudnovsky Pi generator churns
                 * many short-lived limb arrays within a single safepoint stride,
                 * and the incremental safepoint above (budget 4) may not reclaim
                 * them in time. A full collect with the real roots frees exactly
                 * the dead ones, so genuinely-live arrays are never lost. */
                SLGCRootContext roots = { vm, t, vm->native_callback_caller };
                gc_collect_with_roots(&vm->gc, gc_mark_vm_roots, &roots, 0);
                h=array_alloc(vm,(int)nv.as.i);
            }
            if(h<0) TERR("array allocation failed (negative/too-large size or store exhausted)");
            PUSH(slval_array(h));
        } break;
        case OP_ARRGET: {
            SLValue idxv=POP(), av=POP();
            if(av.type!=SL_ARRAY||!array_valid_handle(vm,av.as.h)) TERR("index read on a non-array value");
            if(idxv.type!=SL_INT) TERR("array index must be an integer");
            /* Lock-free fast path when single-threaded: with no worker threads
             * this thread is the sole owner of every array, so the per-element
             * array mutex is pure overhead. This is the dominant cost of a large
             * fill/scan loop. workers_cached is refreshed at the batched
             * safepoint and forced at OP_SPAWN, so it is never a stale zero while
             * a worker exists. */
            if (t->workers_cached == 0) {
                SLArrayInstance* ai=&vm->arrays[av.as.h];
                if(idxv.as.i<0||idxv.as.i>=ai->len) TERR("array index out of range");
                PUSH(ai->elems[idxv.as.i]);
            } else {
                pthread_mutex_lock(&vm->array_mtx);
                SLArrayInstance* ai=&vm->arrays[av.as.h];
                if(idxv.as.i<0||idxv.as.i>=ai->len){pthread_mutex_unlock(&vm->array_mtx);TERR("array index out of range");}
                SLValue ev=ai->elems[idxv.as.i];
                pthread_mutex_unlock(&vm->array_mtx);
                PUSH(ev);
            }
        } break;
        case OP_ARRSET: {
            SLValue val=POP(), idxv=POP(), av=POP();
            if(av.type!=SL_ARRAY||!array_valid_handle(vm,av.as.h)) TERR("index assignment on a non-array value");
            if(idxv.type!=SL_INT) TERR("array index must be an integer");
            if (t->workers_cached == 0) {
                /* Lock-free single-threaded fast path (see OP_ARRGET). The GC
                 * write barrier still runs -- it is correctness, not locking. */
                SLArrayInstance* ai=&vm->arrays[av.as.h];
                if(idxv.as.i<0||idxv.as.i>=ai->len) TERR("array index out of range");
                SLValue old=ai->elems[idxv.as.i];
                if (old.type == SL_STRUCT || old.type == SL_ARRAY || val.type == SL_STRUCT || val.type == SL_ARRAY)
                    gc_write_barrier(&vm->gc, array_gc_object(vm, av.as.h),
                        old.type==SL_STRUCT?struct_gc_object(vm,old.as.h):array_gc_object(vm,old.as.h),
                        val.type==SL_STRUCT?struct_gc_object(vm,val.as.h):array_gc_object(vm,val.as.h));
                if (val.type == SL_STRUCT || val.type == SL_ARRAY || val.type == SL_UTF16) ai->has_refs = 1;
                ai->elems[idxv.as.i]=val;
                PUSH(val);
            } else {
                pthread_mutex_lock(&vm->array_mtx);
                SLArrayInstance* ai=&vm->arrays[av.as.h];
                if(idxv.as.i<0||idxv.as.i>=ai->len){pthread_mutex_unlock(&vm->array_mtx);TERR("array index out of range");}
                SLValue old=ai->elems[idxv.as.i];
                if (old.type == SL_STRUCT || old.type == SL_ARRAY || val.type == SL_STRUCT || val.type == SL_ARRAY)
                    gc_write_barrier(&vm->gc, array_gc_object(vm, av.as.h),
                        old.type==SL_STRUCT?struct_gc_object(vm,old.as.h):array_gc_object(vm,old.as.h),
                        val.type==SL_STRUCT?struct_gc_object(vm,val.as.h):array_gc_object(vm,val.as.h));
                if (val.type == SL_STRUCT || val.type == SL_ARRAY) ai->has_refs = 1;
                ai->elems[idxv.as.i]=val;
                pthread_mutex_unlock(&vm->array_mtx);
                PUSH(val);
            }
        } break;
        case OP_ARRLEN: {
            SLValue av=POP();
            if(av.type!=SL_ARRAY||!array_valid_handle(vm,av.as.h)) TERR("length() on a non-array value");
            pthread_mutex_lock(&vm->array_mtx);
            int len=vm->arrays[av.as.h].len;
            pthread_mutex_unlock(&vm->array_mtx);
            PUSH(slval_int(len));
        } break;
        case OP_ARRPUSH: {
            SLValue val=POP(), av=POP();
            if(av.type!=SL_ARRAY||!array_valid_handle(vm,av.as.h)) TERR("push() on a non-array value");
            int len=array_push(vm,av.as.h,val);
            if(len<0) TERR("array push failed (array full)");
            PUSH(slval_int(len));
        } break;

        /* ---- operating-system system calls (Win32 / POSIX / Apple) ------ */
        case OP_OS_PLATFORM: {
            char buf[32]; if(slos_platform_name(buf,sizeof(buf))<0) buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_OS_CAPABILITY: {
            SLValue cv=POP(); if(cv.type!=SL_INT) TERR("osCapability(cap) requires an integer capability");
            PUSH(slval_int(slos_capability((int)cv.as.i)));
        } break;
        case OP_OS_GETENV: {
            SLValue nv=POP(); if(nv.type!=SL_STR) TERR("osGetEnv(name) requires a String name");
            char buf[4096]; if(slos_getenv(slvm_str(vm,nv.as.s),buf,sizeof(buf))<0) buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_OS_SETENV: {
            SLValue vv=POP(), nv=POP();
            if(nv.type!=SL_STR||vv.type!=SL_STR) TERR("osSetEnv(name, value) requires two Strings");
            PUSH(slval_int(slos_setenv(slvm_str(vm,nv.as.s),slvm_str(vm,vv.as.s))));
        } break;
        case OP_OS_CWD: {
            char buf[4096]; if(slos_cwd(buf,sizeof(buf))<0) buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_OS_CHDIR: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("osChangeDir(path) requires a String path");
            PUSH(slval_int(slos_chdir(slvm_str(vm,pv.as.s))));
        } break;
        case OP_OS_HOSTNAME: {
            char buf[256]; if(slos_hostname(buf,sizeof(buf))<0) buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_OS_USERNAME: {
            char buf[256]; if(slos_username(buf,sizeof(buf))<0) buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_OS_TEMPDIR: {
            char buf[4096]; if(slos_tempdir(buf,sizeof(buf))<0) buf[0]=0;
            SLValue out; out.type=SL_STR; out.as.s=intern(vm,buf); PUSH(out);
        } break;
        case OP_OS_PID: {
            PUSH(slval_int(slos_process_id()));
        } break;
        case OP_OS_EXISTS: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("osExists(path) requires a String path");
            PUSH(slval_int(slos_exists(slvm_str(vm,pv.as.s))));
        } break;
        case OP_OS_ISDIR: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("osIsDir(path) requires a String path");
            PUSH(slval_int(slos_is_dir(slvm_str(vm,pv.as.s))));
        } break;
        case OP_OS_FILESIZE: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("osFileSize(path) requires a String path");
            PUSH(slval_int(slos_file_size(slvm_str(vm,pv.as.s))));
        } break;
        case OP_OS_MKDIR: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("osMakeDir(path) requires a String path");
            PUSH(slval_int(slos_mkdir(slvm_str(vm,pv.as.s))));
        } break;
        case OP_OS_REMOVE: {
            SLValue pv=POP(); if(pv.type!=SL_STR) TERR("osRemove(path) requires a String path");
            PUSH(slval_int(slos_remove(slvm_str(vm,pv.as.s))));
        } break;
        case OP_OS_RENAME: {
            SLValue tv=POP(), fv=POP();
            if(fv.type!=SL_STR||tv.type!=SL_STR) TERR("osRename(from, to) requires two Strings");
            PUSH(slval_int(slos_rename(slvm_str(vm,fv.as.s),slvm_str(vm,tv.as.s))));
        } break;
        case OP_OS_RUN: {
            SLValue cv=POP(); if(cv.type!=SL_STR) TERR("osRun(command) requires a String command");
            PUSH(slval_int(slos_run(slvm_str(vm,cv.as.s))));
        } break;
        case OP_OS_SPAWN: {
            SLValue cv=POP(); if(cv.type!=SL_STR) TERR("osSpawn(command) requires a String command");
            SLOSProcess p=slos_spawn(slvm_str(vm,cv.as.s));
            if(p==SLOS_INVALID_PROCESS){PUSH(slval_int(-1));break;}
            int h=os_proc_alloc(vm,p);
            if(h<0){slos_release(p);PUSH(slval_int(-1));break;}  /* table full */
            PUSH(slval_int(h));
        } break;
        case OP_OS_WAIT: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("osWait(process) requires a process handle");
            SLOSProcess p=os_proc_take(vm,(int)hv.as.i);         /* wait consumes the handle */
            if(p==SLOS_INVALID_PROCESS){PUSH(slval_int(-1));break;}
            PUSH(slval_int(slos_wait(p)));
        } break;
        case OP_OS_KILL: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("osKill(process) requires a process handle");
            SLOSProcess p=os_proc_peek(vm,(int)hv.as.i);
            if(p==SLOS_INVALID_PROCESS){PUSH(slval_int(-1));break;}
            PUSH(slval_int(slos_kill(p)));
        } break;
        case OP_OS_PCLOSE: {
            SLValue hv=POP(); if(hv.type!=SL_INT) TERR("osProcessClose(process) requires a process handle");
            SLOSProcess p=os_proc_take(vm,(int)hv.as.i);
            if(p!=SLOS_INVALID_PROCESS) slos_release(p);
            PUSH(slval_null());
        } break;

        /* ---- OSLS typed native registry (append-only opcode 155) ---- */
        case OP_OSLS_CALL: {
            const osls_vm_binding *binding = osls_vm_binding_by_id(in.a);
            osls_vm_value args[2];
            osls_vm_result result;
            size_t arg_count, i;
            char *json;
            int written;
            if (!binding || !binding->invoke)
                TERR("OSLS native symbol is not registered");
            arg_count = binding->argument_count;
            if (arg_count > sizeof(args) / sizeof(args[0]))
                TERR("OSLS binding signature exceeds the VM marshalling limit");
            for (i = arg_count; i > 0; --i) {
                SLValue value = POP();
                osls_vm_arg_type expected = binding->argument_types[i - 1];
                if (expected == OSLS_VM_ARG_UTF8) {
                    if (value.type != SL_STR)
                        TERR("OSLS binding argument type mismatch: expected String");
                    args[i - 1].type = OSLS_VM_ARG_UTF8;
                    args[i - 1].value.utf8 = slvm_str(vm, value.as.s);
                } else if (expected == OSLS_VM_ARG_I64) {
                    if (value.type != SL_INT)
                        TERR("OSLS binding argument type mismatch: expected integer");
                    args[i - 1].type = OSLS_VM_ARG_I64;
                    args[i - 1].value.i64 = value.as.i;
                } else {
                    TERR("OSLS binding contains an unsupported argument type");
                }
            }
            json = (char *)malloc(65536);
            if (!json) TERR("OSLS binding could not allocate its result buffer");
            written = osls_vm_call_authorized(in.a, args, arg_count,
                                               vm->osls_capabilities,
                                               json, 65536, &result);
            if (written < 0 || result.type != OSLS_VM_RESULT_JSON_UTF8 ||
                result.utf8 != json || result.length != (size_t)written) {
                free(json);
                TERR("OSLS native invocation, capability check, or result marshalling failed");
            }
            {
                SLValue out;
                out.type = SL_STR;
                out.as.s = intern(vm, result.utf8);
                free(json);
                PUSH(out);
            }
        } break;

        case OP_BIGMUL: {
            /* Pops result, b, a (result on top). Each is an int[] of base-1e9
             * little-endian limbs. Multiplies a * b with the native Karatsuba
             * kernel and writes the product into the result array (which must
             * hold at least len(a)+len(b) limbs). Pushes the significant limb
             * count of the product. */
            SLValue rv = POP(), bv = POP(), av = POP();
            if (av.type != SL_ARRAY || !array_valid_handle(vm, av.as.h) ||
                bv.type != SL_ARRAY || !array_valid_handle(vm, bv.as.h) ||
                rv.type != SL_ARRAY || !array_valid_handle(vm, rv.as.h))
                TERR("bigmul requires three int[] array handles (a, b, result)");
            uint32_t *abuf = NULL, *bbuf = NULL, *rbuf = NULL;
            int na = 0, nb = 0, nr = 0, sig = 0, ok = 1;
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *aa = &vm->arrays[av.as.h];
                SLArrayInstance *ab = &vm->arrays[bv.as.h];
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                na = aa->len; nb = ab->len; nr = ar->len;
                if (na < 1 || nb < 1) { ok = 0; }
                else if (nr < na + nb) { ok = 2; }   /* result too small */
                else {
                    abuf = (uint32_t*)malloc((size_t)na * sizeof(uint32_t));
                    bbuf = (uint32_t*)malloc((size_t)nb * sizeof(uint32_t));
                    rbuf = (uint32_t*)calloc((size_t)(na + nb), sizeof(uint32_t));
                    if (!abuf || !bbuf || !rbuf) { ok = 3; }
                    else {
                        for (int i = 0; i < na; i++) {
                            SLValue e = aa->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i :
                                        (e.type == SL_INT128) ? (int64_t)e.as.i128 : 0;
                            if (e.type != SL_INT && e.type != SL_INT128) ok = 4;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            abuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                        for (int i = 0; i < nb; i++) {
                            SLValue e = ab->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i :
                                        (e.type == SL_INT128) ? (int64_t)e.as.i128 : 0;
                            if (e.type != SL_INT && e.type != SL_INT128) ok = 4;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            bbuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            if (ok == 0) { free(abuf); free(bbuf); free(rbuf); TERR("bigmul operands must each have at least one limb"); }
            if (ok == 2) { free(abuf); free(bbuf); free(rbuf); TERR("bigmul result array is too small (need len(a)+len(b) limbs)"); }
            if (ok == 3) { free(abuf); free(bbuf); free(rbuf); TERR("bigmul could not allocate its limb buffers"); }
            if (ok == 4) { free(abuf); free(bbuf); free(rbuf); TERR("bigmul limbs must be integers"); }
            if (ok == 5) { free(abuf); free(bbuf); free(rbuf); TERR("bigmul limbs must be in [0, 1000000000)"); }
            if (sl_bigmul(abuf, na, bbuf, nb, rbuf) != 0) {
                free(abuf); free(bbuf); free(rbuf);
                TERR("bigmul native kernel ran out of memory");
            }
            sig = sl_big_significant(rbuf, na + nb);
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                int wn = (na + nb <= ar->len) ? (na + nb) : ar->len;
                for (int i = 0; i < wn; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = (int64_t)rbuf[i]; }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            free(abuf); free(bbuf); free(rbuf);
            PUSH(slval_int(sig));
        } break;

        case OP_BIGADD: {
            /* Pops result, b, a (result on top). Each an int[] of base-1e9 limbs.
             * Adds a + b, writes the sum into result (needs max(na,nb)+1 limbs),
             * pushes the significant limb count. */
            SLValue rv = POP(), bv = POP(), av = POP();
            if (av.type != SL_ARRAY || !array_valid_handle(vm, av.as.h) ||
                bv.type != SL_ARRAY || !array_valid_handle(vm, bv.as.h) ||
                rv.type != SL_ARRAY || !array_valid_handle(vm, rv.as.h))
                TERR("bigadd requires three int[] array handles (a, b, result)");
            uint32_t *abuf = NULL, *bbuf = NULL, *rbuf = NULL;
            int na = 0, nb = 0, nmax = 0, sig = 0, ok = 1;
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *aa = &vm->arrays[av.as.h];
                SLArrayInstance *ab = &vm->arrays[bv.as.h];
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                na = aa->len; nb = ab->len; nmax = na > nb ? na : nb;
                if (na < 1 || nb < 1) { ok = 0; }
                else if (ar->len < nmax + 1) { ok = 2; }
                else {
                    abuf = (uint32_t*)malloc((size_t)na * sizeof(uint32_t));
                    bbuf = (uint32_t*)malloc((size_t)nb * sizeof(uint32_t));
                    rbuf = (uint32_t*)calloc((size_t)(nmax + 1), sizeof(uint32_t));
                    if (!abuf || !bbuf || !rbuf) { ok = 3; }
                    else {
                        for (int i = 0; i < na; i++) {
                            SLValue e = aa->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            abuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                        for (int i = 0; i < nb; i++) {
                            SLValue e = ab->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            bbuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            if (ok == 0) { free(abuf); free(bbuf); free(rbuf); TERR("bigadd operands must each have at least one limb"); }
            if (ok == 2) { free(abuf); free(bbuf); free(rbuf); TERR("bigadd result array is too small (need max(len(a),len(b))+1 limbs)"); }
            if (ok == 3) { free(abuf); free(bbuf); free(rbuf); TERR("bigadd could not allocate its limb buffers"); }
            if (ok == 5) { free(abuf); free(bbuf); free(rbuf); TERR("bigadd limbs must be in [0, 1000000000)"); }
            sig = sl_bigadd(abuf, na, bbuf, nb, rbuf);
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                int wn = (nmax + 1 <= ar->len) ? (nmax + 1) : ar->len;
                for (int i = 0; i < wn; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = (int64_t)rbuf[i]; }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            free(abuf); free(bbuf); free(rbuf);
            PUSH(slval_int(sig));
        } break;

        case OP_BIGDIVMOD: {
            /* Pops result, divisor(int), dividend (dividend deepest). Divides the
             * dividend int[] by the small integer divisor (1..1e9-1), writes the
             * quotient limbs into result (dividend length suffices), and pushes
             * the integer remainder. */
            SLValue rv = POP(), dvv = POP(), nv = POP();
            if (nv.type != SL_ARRAY || !array_valid_handle(vm, nv.as.h) ||
                rv.type != SL_ARRAY || !array_valid_handle(vm, rv.as.h))
                TERR("bigdivmod requires (dividend int[], int divisor, result int[])");
            if (dvv.type != SL_INT) TERR("bigdivmod divisor must be an integer");
            int64_t divisor = dvv.as.i;
            if (divisor < 1 || divisor >= (int64_t)SL_BIG_BASE)
                TERR("bigdivmod divisor must be in [1, 1000000000)");
            uint32_t *dbuf = NULL, *qbuf = NULL;
            int n = 0, ok = 1;
            uint32_t rem = 0;
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *ad = &vm->arrays[nv.as.h];
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                n = ad->len;
                if (n < 1) { ok = 0; }
                else if (ar->len < n) { ok = 2; }
                else {
                    dbuf = (uint32_t*)malloc((size_t)n * sizeof(uint32_t));
                    qbuf = (uint32_t*)calloc((size_t)n, sizeof(uint32_t));
                    if (!dbuf || !qbuf) { ok = 3; }
                    else {
                        for (int i = 0; i < n; i++) {
                            SLValue e = ad->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            dbuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            if (ok == 0) { free(dbuf); free(qbuf); TERR("bigdivmod dividend must have at least one limb"); }
            if (ok == 2) { free(dbuf); free(qbuf); TERR("bigdivmod result array is too small (need len(dividend) limbs)"); }
            if (ok == 3) { free(dbuf); free(qbuf); TERR("bigdivmod could not allocate its limb buffers"); }
            if (ok == 5) { free(dbuf); free(qbuf); TERR("bigdivmod limbs must be in [0, 1000000000)"); }
            rem = sl_bigdivmod_small(dbuf, n, (uint32_t)divisor, qbuf);
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                int wn = (n <= ar->len) ? n : ar->len;
                for (int i = 0; i < wn; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = (int64_t)qbuf[i]; }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            free(dbuf); free(qbuf);
            PUSH(slval_int((int64_t)rem));
        } break;

        case OP_BIGSUB: {
            /* Pops result, b, a (result on top). Computes a - b (a >= b) over
             * base-1e9 limb arrays, writes the difference into result (needs
             * len(a) limbs), pushes the significant result-limb count. */
            SLValue rv = POP(), bv = POP(), av = POP();
            if (av.type != SL_ARRAY || !array_valid_handle(vm, av.as.h) ||
                bv.type != SL_ARRAY || !array_valid_handle(vm, bv.as.h) ||
                rv.type != SL_ARRAY || !array_valid_handle(vm, rv.as.h))
                TERR("bigsub requires three int[] array handles (a, b, result)");
            uint32_t *abuf = NULL, *bbuf = NULL, *rbuf = NULL;
            int na = 0, nb = 0, sig = 0, ok = 1;
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *aa = &vm->arrays[av.as.h];
                SLArrayInstance *ab = &vm->arrays[bv.as.h];
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                na = aa->len; nb = ab->len;
                if (na < 1 || nb < 1) { ok = 0; }
                else if (ar->len < na) { ok = 2; }
                else {
                    abuf = (uint32_t*)malloc((size_t)na * sizeof(uint32_t));
                    bbuf = (uint32_t*)malloc((size_t)nb * sizeof(uint32_t));
                    rbuf = (uint32_t*)calloc((size_t)na, sizeof(uint32_t));
                    if (!abuf || !bbuf || !rbuf) { ok = 3; }
                    else {
                        for (int i = 0; i < na; i++) {
                            SLValue e = aa->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            abuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                        for (int i = 0; i < nb; i++) {
                            SLValue e = ab->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            bbuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            if (ok == 0) { free(abuf); free(bbuf); free(rbuf); TERR("bigsub operands must each have at least one limb"); }
            if (ok == 2) { free(abuf); free(bbuf); free(rbuf); TERR("bigsub result array is too small (need len(a) limbs)"); }
            if (ok == 3) { free(abuf); free(bbuf); free(rbuf); TERR("bigsub could not allocate its limb buffers"); }
            if (ok == 5) { free(abuf); free(bbuf); free(rbuf); TERR("bigsub limbs must be in [0, 1000000000)"); }
            if (sl_bigcmp(abuf, na, bbuf, nb) < 0) { free(abuf); free(bbuf); free(rbuf); TERR("bigsub requires a >= b (negative result)"); }
            sig = sl_bigsub(abuf, na, bbuf, nb, rbuf);
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                int wn = (na <= ar->len) ? na : ar->len;
                for (int i = 0; i < wn; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = (int64_t)rbuf[i]; }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            free(abuf); free(bbuf); free(rbuf);
            PUSH(slval_int(sig));
        } break;

        case OP_BIGDIV: {
            /* Pops remOut, quoOut, den, num (num deepest). Computes
             * quotient = num / den into quoOut (needs len(num) limbs) and
             * remainder into remOut (needs len(den) limbs); pushes the
             * significant quotient-limb count. */
            SLValue remv = POP(), quov = POP(), dv = POP(), nmv = POP();
            if (nmv.type != SL_ARRAY || !array_valid_handle(vm, nmv.as.h) ||
                dv.type  != SL_ARRAY || !array_valid_handle(vm, dv.as.h)  ||
                quov.type!= SL_ARRAY || !array_valid_handle(vm, quov.as.h)||
                remv.type!= SL_ARRAY || !array_valid_handle(vm, remv.as.h))
                TERR("bigdiv requires four int[] array handles (num, den, quoOut, remOut)");
            uint32_t *nbuf = NULL, *dbuf = NULL, *qbuf = NULL, *rbuf = NULL;
            int nn = 0, nd = 0, ql = 0, reml = 0, ok = 1;
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *an = &vm->arrays[nmv.as.h];
                SLArrayInstance *ad = &vm->arrays[dv.as.h];
                SLArrayInstance *aq = &vm->arrays[quov.as.h];
                SLArrayInstance *ar = &vm->arrays[remv.as.h];
                nn = an->len; nd = ad->len;
                if (nn < 1 || nd < 1) { ok = 0; }
                else if (aq->len < nn || ar->len < nd) { ok = 2; }
                else {
                    nbuf = (uint32_t*)malloc((size_t)nn * sizeof(uint32_t));
                    dbuf = (uint32_t*)malloc((size_t)nd * sizeof(uint32_t));
                    qbuf = (uint32_t*)calloc((size_t)(nn + 2), sizeof(uint32_t));
                    /* rem scratch is sized to the numerator width (+2), not the
                     * divisor width: the quasi-linear sl_bigdiv may hold a
                     * transient remainder wider than the divisor mid-correction
                     * before it settles to the true (< den) remainder. The final
                     * value is < den, but the buffer must tolerate the transient. */
                    rbuf = (uint32_t*)calloc((size_t)(nn + 2), sizeof(uint32_t));
                    if (!nbuf || !dbuf || !qbuf || !rbuf) { ok = 3; }
                    else {
                        for (int i = 0; i < nn; i++) {
                            SLValue e = an->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            nbuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                        for (int i = 0; i < nd; i++) {
                            SLValue e = ad->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            dbuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            if (ok == 0) { free(nbuf); free(dbuf); free(qbuf); free(rbuf); TERR("bigdiv operands must each have at least one limb"); }
            if (ok == 2) { free(nbuf); free(dbuf); free(qbuf); free(rbuf); TERR("bigdiv output arrays too small (quoOut needs len(num), remOut needs len(den))"); }
            if (ok == 3) { free(nbuf); free(dbuf); free(qbuf); free(rbuf); TERR("bigdiv could not allocate its limb buffers"); }
            if (ok == 5) { free(nbuf); free(dbuf); free(qbuf); free(rbuf); TERR("bigdiv limbs must be in [0, 1000000000)"); }
            { int dz = nd; while (dz > 1 && dbuf[dz-1] == 0) dz--; if (dz == 1 && dbuf[0] == 0) { free(nbuf); free(dbuf); free(qbuf); free(rbuf); TERR("bigdiv division by zero"); } }
            if (sl_bigdiv(nbuf, nn, dbuf, nd, qbuf, rbuf, &reml) != 0) {
                free(nbuf); free(dbuf); free(qbuf); free(rbuf);
                TERR("bigdiv native kernel ran out of memory");
            }
            ql = sl_big_significant(qbuf, nn);
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *aq = &vm->arrays[quov.as.h];
                SLArrayInstance *ar = &vm->arrays[remv.as.h];
                int wq = (nn <= aq->len) ? nn : aq->len;
                for (int i = 0; i < wq; i++) { aq->elems[i].type = SL_INT; aq->elems[i].as.i = (int64_t)qbuf[i]; }
                int wr = (reml <= ar->len) ? reml : ar->len;
                for (int i = 0; i < wr; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = (int64_t)rbuf[i]; }
                for (int i = wr; i < ar->len; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = 0; }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            free(nbuf); free(dbuf); free(qbuf); free(rbuf);
            PUSH(slval_int(ql));
        } break;

        case OP_BIGISQRT: {
            /* Pops result, a (result on top). Writes floor(sqrt(a)) into result
             * (needs (len(a)+1)/2 + 1 limbs), pushes significant limb count. */
            SLValue rv = POP(), av = POP();
            if (av.type != SL_ARRAY || !array_valid_handle(vm, av.as.h) ||
                rv.type != SL_ARRAY || !array_valid_handle(vm, rv.as.h))
                TERR("bigisqrt requires two int[] array handles (a, result)");
            uint32_t *abuf = NULL, *rbuf = NULL;
            int na = 0, need = 0, sig = 0, ok = 1;
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *aa = &vm->arrays[av.as.h];
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                na = aa->len;
                need = (na + 1) / 2 + 1;
                if (na < 1) { ok = 0; }
                else if (ar->len < need) { ok = 2; }
                else {
                    abuf = (uint32_t*)malloc((size_t)na * sizeof(uint32_t));
                    rbuf = (uint32_t*)calloc((size_t)(need + 2), sizeof(uint32_t));
                    if (!abuf || !rbuf) { ok = 3; }
                    else {
                        for (int i = 0; i < na; i++) {
                            SLValue e = aa->elems[i];
                            int64_t x = (e.type == SL_INT) ? e.as.i : (e.type == SL_INT128) ? (int64_t)e.as.i128 : -1;
                            if (x < 0 || x >= (int64_t)SL_BIG_BASE) ok = 5;
                            abuf[i] = (x >= 0 && x < (int64_t)SL_BIG_BASE) ? (uint32_t)x : 0;
                        }
                    }
                }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            if (ok == 0) { free(abuf); free(rbuf); TERR("bigisqrt operand must have at least one limb"); }
            if (ok == 2) { free(abuf); free(rbuf); TERR("bigisqrt result array is too small (need (len(a)+1)/2 + 1 limbs)"); }
            if (ok == 3) { free(abuf); free(rbuf); TERR("bigisqrt could not allocate its limb buffers"); }
            if (ok == 5) { free(abuf); free(rbuf); TERR("bigisqrt limbs must be in [0, 1000000000)"); }
            sig = sl_bigisqrt(abuf, na, rbuf);
            if (sig < 0) { free(abuf); free(rbuf); TERR("bigisqrt native kernel ran out of memory"); }
            pthread_mutex_lock(&vm->array_mtx);
            {
                SLArrayInstance *ar = &vm->arrays[rv.as.h];
                int wn = (sig <= ar->len) ? sig : ar->len;
                for (int i = 0; i < wn; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = (int64_t)rbuf[i]; }
                for (int i = wn; i < ar->len; i++) { ar->elems[i].type = SL_INT; ar->elems[i].as.i = 0; }
            }
            pthread_mutex_unlock(&vm->array_mtx);
            free(abuf); free(rbuf);
            PUSH(slval_int(sig));
        } break;

        default: log_unsupported_opcode(vm, in.op); TERR("unsupported opcode");
        }
    }
}

SLResult slvm_run(SLVM* vm) {
    if (!vm || vm->entry < 0 || vm->entry >= vm->nfunc) { if(vm) set_err(vm,"no entry function set"); return SLR_ERROR; }
    vm->err[0]=0; vm->has_last_exception=0; vm->last_exception=slval_null(); SLThread* t=(SLThread*)calloc(1,sizeof(SLThread)); if(!t){set_err(vm,"out of memory");return SLR_ERROR;}
    t->vm=vm;t->entry_func=vm->entry;t->result=slval_null(); SLResult r=run_thread(t); vm->last_result=t->result; if(t->has_uncaught_exception){vm->last_exception=t->uncaught_exception;vm->has_last_exception=1;} free(t); slvm_joinall(vm); return r;
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
