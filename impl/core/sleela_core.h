/* ==========================================================================
 * sleela_core.h  --  Sleela execution core: the Turing-complete C/C++ engine.
 * ========================================================================== */
#ifndef SLEELA_CORE_H
#define SLEELA_CORE_H
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SL_NULL = 0, SL_INT, SL_DOUBLE, SL_BOOL, SL_STR } SLType;
typedef struct { SLType type; union { int64_t i; double d; int b; int32_t s; } as; } SLValue;
typedef enum {
    OP_NOP = 0, OP_CONST, OP_POP, OP_DUP, OP_LOADG, OP_STOREG, OP_LOADL, OP_STOREL,
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, OP_NEG,
    OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE, OP_AND, OP_OR, OP_NOT,
    OP_JMP, OP_JMPF, OP_CALL, OP_RET, OP_PRINT, OP_SPAWN, OP_JOINALL,
    OP_LOCK, OP_UNLOCK, OP_SEND, OP_RECV,
    OP_LISTEN, OP_ACCEPT, OP_CONNECT, OP_SOCKREAD, OP_SOCKWRITE, OP_SOCKCLOSE,
    OP_HALT
} SLOp;
#define SL_MAX_THREADS 128
#define SL_MAX_LOCKS 32
#define SL_MAX_SOCKETS 128
typedef enum { SLR_OK = 0, SLR_ERROR, SLR_HALT } SLResult;
typedef struct SLVM SLVM;
typedef enum { SLX_RESET = 0, SLX_ADD_CONST, SLX_DECLARE_GLOBAL, SLX_BEGIN_FUNC, SLX_END_FUNC, SLX_EMIT, SLX_PATCH, SLX_HERE, SLX_SET_ENTRY, SLX_RUN, SLX_GET_RESULT } SLExchangeOp;
typedef struct { SLValue value; const char* name; int32_t op; int32_t a; int32_t i0; int32_t i1; int32_t i2; int32_t out; } SLExchangeArg;
SLVM* slvm_new(void);
void slvm_free(SLVM* vm);
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg);
int slvm_add_const_int(SLVM* vm, int64_t v);
int slvm_add_const_double(SLVM* vm, double v);
int slvm_add_const_bool(SLVM* vm, int v);
int slvm_add_const_str(SLVM* vm, const char* s);
int slvm_declare_global(SLVM* vm, const char* name);
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
/* Persistent runnable .sleela artifacts: Core bytecode, directly loadable by runtime. */
int slvm_save_file(SLVM* vm, const char* path);
SLVM* slvm_load_file(const char* path);
int slvm_is_artifact_file(const char* path);
#ifdef __cplusplus
}
#endif
#endif /* SLEELA_CORE_H */
