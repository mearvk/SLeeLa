/* ==========================================================================
 * sleela_core.h  -- Sleela execution core: the Turing-complete C/C++ engine.
 * ========================================================================== */
#ifndef SLEELA_CORE_H
#define SLEELA_CORE_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SL_NULL = 0, SL_INT, SL_DOUBLE, SL_BOOL, SL_STR, SL_STRUCT } SLType;
/* SL_STRUCT is a VM-local handle (index into the struct-instance store), the
 * same bounded-handle discipline used for sockets/files/threads. Sleela code
 * never sees a raw pointer; a struct value carries only its instance handle. */
typedef struct { SLType type; union { int64_t i; double d; int b; int32_t s; int32_t h; } as; } SLValue;
typedef enum {
    OP_NOP = 0, OP_CONST, OP_POP, OP_DUP, OP_LOADG, OP_STOREG, OP_LOADL, OP_STOREL,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_NEG,
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE, OP_AND, OP_OR, OP_NOT,
    OP_JMP, OP_JMPF, OP_CALL, OP_RET, OP_PRINT, OP_SPAWN, OP_JOINALL,
    OP_LOCK, OP_UNLOCK, OP_SEND, OP_RECV,
    OP_LISTEN, OP_ACCEPT, OP_CONNECT, OP_SOCKREAD, OP_SOCKWRITE, OP_SOCKCLOSE,
    OP_PIPE, OP_PIPEPEER, OP_FIFO_MK, OP_FILEOPEN, OP_FILEREAD, OP_FILEWRITE,
    OP_FILECLOSE, OP_FILEUNLINK,
    OP_HALT,
    OP_TIME_UTC_MS, OP_TIME_UTC_NS, OP_TIME_MONO_NS, OP_TIME_PRECISION_MS, OP_TIME_LOCATION,
    OP_TIME_HTTP_DATE, OP_TIME_JSON, OP_TIME_NTP, OP_TIME_SET_LOCATION,
    /* struct support: a=type index / field offset as noted per op */
    OP_NEWSTRUCT,   /* a = struct-type index; pushes a fresh instance handle */
    OP_GETFIELD,    /* a = field offset; pops instance, pushes field value    */
    OP_SETFIELD,    /* a = field offset; pops value then instance             */
    OP_STRUCTPACK,  /* pops instance, pushes a JSON String of its fields      */
    OP_STRUCTUNPACK,/* a = struct-type index; pops JSON String, pushes handle */
    /* Synchro (syntax 1.3): honest packet dispatch + measurement. Handles are
     * VM-local integers into a bounded probe table, like sockets. */
    OP_SYN_OPEN,    /* pops port,host; pushes probe handle (-1 on failure)     */
    OP_SYN_DISPATCH,/* pops timeout_ms,len,handle; pushes measured RTT us / -1 */
    OP_SYN_STAT,    /* a = stat selector; pops handle; pushes int stat         */
    OP_SYN_REPORT,  /* pops handle; pushes a String honest report             */
    OP_SYN_CLOSE,   /* pops handle; releases the probe; pushes null            */
    /* Munction (syntax 1.3): the reach-composition sentence. A reach is a
     * VM-local handle into a bounded reach table. */
    OP_MUN_START,   /* pops name String; pushes reach handle (-1 on failure)   */
    OP_MUN_CONNECT, /* pops uri,handle; pushes handle (passthrough) / -1       */
    OP_MUN_ENABLE,  /* pops policy,handle; pushes handle                       */
    OP_MUN_SEND,    /* pops datum,handle; pushes handle                        */
    OP_MUN_THATCH,  /* pops spec,handle; pushes handle                         */
    OP_MUN_CONSUME, /* pops handle; pushes handle (reception stored on reach)  */
    OP_MUN_LATCH,   /* pops handle; pushes handle                              */
    OP_MUN_RECEPTION,/* pops handle; pushes last reception String             */
    OP_MUN_CLOSE,   /* pops handle; pushes receipt String; releases the reach  */
    /* Best-of (syntax 1.3): configurable route/accuracy selection for Synchro
     * and Munction/RMI packets. A selector is a VM-local handle. */
    OP_BEST_NEW,       /* pushes best-of handle (-1 on failure)                */
    OP_BEST_WEIGHT,    /* pops weight,axis,handle; pushes handle (passthrough) */
    OP_BEST_MINVER,    /* pops minVersion,handle; pushes handle                */
    OP_BEST_BUDGET,    /* pops costBudget,handle; pushes handle                */
    OP_BEST_CAND,      /* pops replays,cost,version,flags,gap,payload,timeout,route,name,handle; pushes candidate index */
    OP_BEST_RECORD,    /* pops rttUs,idx,handle; pushes handle                 */
    OP_BEST_SCORE,     /* pops idx,handle; pushes score int                    */
    OP_BEST_BEST,      /* pops handle; pushes best candidate index (-1 none)   */
    OP_BEST_STAT,      /* a=selector; pops idx,handle; pushes measured int     */
    OP_BEST_CHOICE,    /* pops handle; pushes winning-choice String            */
    OP_BEST_REPORT,    /* pops handle; pushes multi-line report String         */
    OP_BEST_ARCH,      /* pops realized,param,arch,idx,handle; pushes handle   */
    OP_BEST_ARCH_STATE,/* pops realized,idx,handle; pushes handle              */
    OP_BEST_CLOSE      /* pops handle; releases the selector; pushes null      */
} SLOp;
/* Synchro stat selectors for OP_SYN_STAT (operand a). */
#define SL_SYN_STAT_SENT 0
#define SL_SYN_STAT_RECV 1
#define SL_SYN_STAT_MEAN 2
#define SL_SYN_STAT_MIN  3
#define SL_SYN_STAT_MAX  4
#define SL_SYN_STAT_P95  5
#define SL_SYN_STAT_LOSS 6
/* Best-of per-candidate stat selectors for OP_BEST_STAT (operand a). */
#define SL_BEST_STAT_MEAN      0
#define SL_BEST_STAT_LOSS      1
#define SL_BEST_STAT_JITTER    2
#define SL_BEST_STAT_CERTAINTY 3
#define SL_BEST_STAT_ARCH      4  /* candidate architecture (SL_BESTOF_ARCH_*) */
#define SL_BEST_STAT_ARCHPARAM 5  /* architecture parameter (DSCP/kbps/label)  */
#define SL_BEST_STAT_ARCHSTATE 6  /* realization state (requested/realized/denied) */
#define SL_MAX_THREADS 128
#define SL_MAX_LOCKS 32
#define SL_MAX_SOCKETS 128
#define SL_MAX_FILES 256
#define SL_MAX_STRUCT_TYPES 256   /* distinct struct declarations per program  */
#define SL_MAX_STRUCT_FIELDS 64   /* named fields per struct type              */
#define SL_MAX_STRUCTS 4096       /* live struct instances per VM              */
typedef enum { SLR_OK = 0, SLR_ERROR, SLR_HALT } SLResult;
typedef struct SLVM SLVM;
typedef enum { SLX_RESET = 0, SLX_ADD_CONST, SLX_DECLARE_GLOBAL, SLX_BEGIN_FUNC, SLX_END_FUNC, SLX_EMIT, SLX_PATCH, SLX_HERE, SLX_SET_ENTRY, SLX_RUN, SLX_GET_RESULT, SLX_DECLARE_STRUCT } SLExchangeOp;
/* For SLX_DECLARE_STRUCT: name = struct type name, names = ordered field names,
 * i0 = field count; out receives the type index. */
typedef struct { SLValue value; const char* name; const char* const* names; int32_t op; int32_t a; int32_t i0; int32_t i1; int32_t i2; int32_t out; } SLExchangeArg;
SLVM* slvm_new(void);
/* True when Sleela source values use the bounded VM-owned memory/handle model. */
int slvm_memory_safe_mode(const SLVM* vm);
void slvm_free(SLVM* vm);
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg);
int slvm_add_const_int(SLVM* vm, int64_t v);
int slvm_add_const_double(SLVM* vm, double v);
int slvm_add_const_bool(SLVM* vm, int v);
int slvm_add_const_str(SLVM* vm, const char* s);
int slvm_declare_global(SLVM* vm, const char* name);
/* Register a struct type and its ordered field names. Returns the type index
 * used by OP_NEWSTRUCT / OP_STRUCTUNPACK, or -1 on error. Field order defines
 * the offsets used by OP_GETFIELD / OP_SETFIELD. */
int slvm_declare_struct(SLVM* vm, const char* name, const char* const* field_names, int nfields);
int slvm_begin_func(SLVM* vm, const char* name, int nargs, int nlocals);
void slvm_end_func(SLVM* vm);
int slvm_emit(SLVM* vm, SLOp op, int32_t a);
int slvm_emit0(SLVM* vm, SLOp op);
void slvm_patch(SLVM* vm, int at, int32_t a);
int slvm_here(SLVM* vm);
void slvm_set_entry(SLVM* vm, int func_index);
SLResult slvm_run(SLVM* vm);
SLValue slvm_result(SLVM* vm);
void slvm_joinall(SLVM* vm);
SLValue slval_null(void);
SLValue slval_int(int64_t v);
SLValue slval_double(double v);
SLValue slval_bool(int v);
const char* slvm_str(SLVM* vm, int32_t id);
const char* slvm_error(SLVM* vm);
int slvm_save_file(SLVM* vm, const char* path);
SLVM* slvm_load_file(const char* path);
int slvm_is_artifact_file(const char* path);
#ifdef __cplusplus
}
#endif
#endif /* SLEELA_CORE_H */
