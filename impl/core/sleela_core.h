/* ==========================================================================
 * sleela_core.h  --  Sleela execution core: the Turing-complete C/C++ engine.
 *
 * This is the "underwriting C/C++" beneath the Sleela language. It is a small
 * stack-based bytecode virtual machine with a stable C ABI. Everything above
 * it (the Sleela front end, or any other language) drives it by:
 *   1. assembling a program with the builder helpers, then
 *   2. running it,
 * all of which is multiplexed through the single dispatch entry point
 * slcore_exchange() (the "exchange function").
 *
 * The core is language-agnostic: it knows opcodes and values, not Sleela.
 * ========================================================================== */
#ifndef SLEELA_CORE_H
#define SLEELA_CORE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Value model. Values are POD tagged unions so they cross the C ABI freely.
 * Strings are interned per-VM and referenced by id.
 * ------------------------------------------------------------------------- */
typedef enum {
    SL_NULL = 0,
    SL_INT,
    SL_DOUBLE,
    SL_BOOL,
    SL_STR      /* payload .s is an interned string id */
} SLType;

typedef struct {
    SLType type;
    union {
        int64_t i;
        double  d;
        int     b;
        int32_t s;   /* interned string id */
    } as;
} SLValue;

/* -------------------------------------------------------------------------
 * Opcodes for the stack machine.
 * ------------------------------------------------------------------------- */
typedef enum {
    OP_NOP = 0,
    OP_CONST,     /* a: const index                  push constants[a]       */
    OP_POP,       /*                                  pop, discard            */
    OP_DUP,       /*                                  duplicate top           */

    OP_LOADG,     /* a: global slot                  push globals[a]         */
    OP_STOREG,    /* a: global slot                  globals[a] = pop        */
    OP_LOADL,     /* a: local slot (frame relative)  push locals[a]          */
    OP_STOREL,    /* a: local slot                   locals[a] = pop         */

    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_NEG,
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE,
    OP_AND, OP_OR, OP_NOT,

    OP_JMP,       /* a: target ip                    ip = a                  */
    OP_JMPF,      /* a: target ip                    if(!pop) ip = a         */

    OP_CALL,      /* a: function index               call; args on stack     */
    OP_RET,       /*                                  return top to caller    */

    OP_PRINT,     /*                                  pop and println         */

    /* -----------------------------------------------------------------
     * Threading (bounded, clean). A "thread" is one line of execution with
     * its own stack and frames; the shared VM owns code/consts/globals plus
     * a small fixed lock table and a 2-tuple mailbox for coordination.
     * ----------------------------------------------------------------- */
    OP_SPAWN,     /* a: function index               start fn on a new thread;
                     top `nargs` stack slots are the thread's initial locals;
                     pushes a thread handle (SL_INT id, or -1 if at capacity) */
    OP_JOINALL,   /*                                  wait for all spawned threads */
    OP_LOCK,      /* a: lock id [0..SL_MAX_LOCKS)     acquire lock a          */
    OP_UNLOCK,    /* a: lock id                       release lock a          */
    OP_SEND,      /* a: mailbox slot [0..SL_MAX_LOCKS) send 2-tuple (a, pop) onto the burble line */
    OP_RECV,      /* a: mailbox slot                  block until a tuple for slot a; push its value */

    OP_HALT
} SLOp;

/* Bounds. Threading here is deliberately simple and bounded: at most 128
 * concurrent lines of execution, and a modest fixed set of locks / mailbox
 * slots (comfortably covering "~24 concurrent locks on a 2-tuple line"). */
#define SL_MAX_THREADS 128
#define SL_MAX_LOCKS   32

/* Result codes returned across the ABI. */
typedef enum {
    SLR_OK = 0,
    SLR_ERROR,
    SLR_HALT
} SLResult;

/* Opaque VM handle. */
typedef struct SLVM SLVM;

/* -------------------------------------------------------------------------
 * The exchange (dispatch) API.
 *
 * slcore_exchange is the single symbol through which an embedder can drive
 * the entire core. Each op reads/writes the SLExchangeArg packet.
 * ------------------------------------------------------------------------- */
typedef enum {
    SLX_RESET = 0,       /* clear program state                              */
    SLX_ADD_CONST,       /* arg.value -> pool; arg.out = const index         */
    SLX_DECLARE_GLOBAL,  /* arg.name  -> global; arg.out = global slot       */
    SLX_BEGIN_FUNC,      /* arg.name,i0=nargs,i1=nlocals; arg.out = func idx */
    SLX_END_FUNC,        /* finalize current function                        */
    SLX_EMIT,            /* arg.op, arg.a -> code; arg.out = instr index     */
    SLX_PATCH,           /* arg.a = instr index, arg.i0 = new operand        */
    SLX_HERE,            /* arg.out = current code length                    */
    SLX_SET_ENTRY,       /* arg.a = function index of entry point            */
    SLX_RUN,             /* execute entry; arg.value = result                */
    SLX_GET_RESULT       /* arg.value = last run result                      */
} SLExchangeOp;

/* The threading opcodes (OP_SPAWN, OP_JOINALL, OP_LOCK, OP_UNLOCK, OP_SEND,
 * OP_RECV) need no new exchange ops: they are assembled like any other
 * instruction through SLX_EMIT (arg.op = the opcode, arg.a = its operand),
 * so slcore_exchange remains the single dispatch entry point. slvm_run then
 * executes the whole program -- main thread plus any spawned threads -- and
 * joins all workers before returning. */

typedef struct {
    SLValue     value;   /* const value in / run result out                  */
    const char* name;    /* symbol name                                      */
    int32_t     op;      /* opcode for SLX_EMIT                              */
    int32_t     a;       /* operand / index                                  */
    int32_t     i0;      /* nargs / patch operand                            */
    int32_t     i1;      /* nlocals                                          */
    int32_t     i2;      /* spare                                            */
    int32_t     out;     /* returned index / slot / instr                    */
} SLExchangeArg;

/* Lifecycle. */
SLVM*    slvm_new(void);
void     slvm_free(SLVM* vm);

/* The one exchange/dispatch entry point. */
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg);

/* -------------------------------------------------------------------------
 * Convenience builder helpers. These are thin, typed wrappers over
 * slcore_exchange so the compiler can assemble programs ergonomically.
 * ------------------------------------------------------------------------- */
int  slvm_add_const_int(SLVM* vm, int64_t v);
int  slvm_add_const_double(SLVM* vm, double v);
int  slvm_add_const_bool(SLVM* vm, int v);
int  slvm_add_const_str(SLVM* vm, const char* s);

int  slvm_declare_global(SLVM* vm, const char* name);

int  slvm_begin_func(SLVM* vm, const char* name, int nargs, int nlocals);
void slvm_end_func(SLVM* vm);

int  slvm_emit(SLVM* vm, SLOp op, int32_t a);   /* returns instr index */
int  slvm_emit0(SLVM* vm, SLOp op);              /* emit an opcode with operand 0 */
void slvm_patch(SLVM* vm, int at, int32_t a);   /* backpatch jump target */
int  slvm_here(SLVM* vm);                        /* current code length */

void slvm_set_entry(SLVM* vm, int func_index);
SLResult slvm_run(SLVM* vm);
SLValue  slvm_result(SLVM* vm);

/* Wait for every thread spawned via OP_SPAWN to finish. Called automatically
 * at the end of slvm_run and slvm_free, and exposed for embedders. */
void     slvm_joinall(SLVM* vm);

/* Helpers for constructing values. */
SLValue slval_null(void);
SLValue slval_int(int64_t v);
SLValue slval_double(double v);
SLValue slval_bool(int v);

/* Interned-string lookup (for a str value). */
const char* slvm_str(SLVM* vm, int32_t id);

/* Last error message, or NULL. */
const char* slvm_error(SLVM* vm);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SLEELA_CORE_H */
