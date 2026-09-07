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

struct SLVM {
    /* code */
    SLInstr* code; int codelen, codecap;

    /* constants */
    SLValue* consts; int nconst, constcap;

    /* interned strings */
    char**   strs;  int nstr, strcap;

    /* globals */
    SLValue* globals; char** gnames; int nglobal, globalcap;

    /* functions */
    SLFunc*  funcs; int nfunc, funccap;
    int      cur_func;   /* function currently being emitted, or -1 */
    int      entry;      /* entry function index */

    /* runtime */
    SLValue  stack[STACK_MAX]; int sp;
    SLFrame  frames[FRAMES_MAX]; int fp;
    SLValue  last_result;

    char     err[256];
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
    if (!vm || id < 0 || id >= vm->nstr) return "";
    return vm->strs[id];
}

/* growable-array push macros keep the plumbing terse */
#define ENSURE(arr, len, cap, type)                                   \
    do {                                                              \
        if ((len) >= (cap)) {                                         \
            (cap) = (cap) ? (cap) * 2 : 8;                            \
            (arr) = (type*)realloc((arr), (size_t)(cap) * sizeof(type)); \
        }                                                             \
    } while (0)

/* ---- interning --------------------------------------------------------- */
static int intern(SLVM* vm, const char* s) {
    for (int i = 0; i < vm->nstr; i++)
        if (strcmp(vm->strs[i], s) == 0) return i;
    ENSURE(vm->strs, vm->nstr, vm->strcap, char*);
    vm->strs[vm->nstr] = strdup(s);
    return vm->nstr++;
}

/* ---- lifecycle --------------------------------------------------------- */
SLVM* slvm_new(void) {
    SLVM* vm = (SLVM*)calloc(1, sizeof(SLVM));
    vm->cur_func = -1;
    vm->entry    = -1;
    vm->last_result = slval_null();
    return vm;
}

void slvm_free(SLVM* vm) {
    if (!vm) return;
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

/* ---- the interpreter loop ---------------------------------------------- */
#define PUSH(v) do { if (vm->sp >= STACK_MAX) { set_err(vm,"stack overflow"); return SLR_ERROR; } vm->stack[vm->sp++] = (v); } while(0)
#define POP()   (vm->stack[--vm->sp])

SLResult slvm_run(SLVM* vm) {
    if (vm->entry < 0 || vm->entry >= vm->nfunc) {
        set_err(vm, "no entry function set");
        return SLR_ERROR;
    }
    vm->sp = 0;
    vm->fp = 0;
    vm->err[0] = 0;

    SLFunc* ef = &vm->funcs[vm->entry];
    /* entry frame: reserve locals region (args=0 for main) */
    SLFrame* fr = &vm->frames[vm->fp++];
    fr->ret_ip = -1;
    fr->base   = vm->sp;
    fr->nargs  = ef->nargs;
    for (int i = 0; i < ef->nlocals; i++) PUSH(slval_null());

    int ip = ef->entry;

    for (;;) {
        if (ip < 0 || ip >= vm->codelen) { set_err(vm, "ip out of range"); return SLR_ERROR; }
        SLInstr in = vm->code[ip++];
        switch (in.op) {
        case OP_NOP: break;
        case OP_HALT: return SLR_HALT;

        case OP_CONST: PUSH(vm->consts[in.a]); break;
        case OP_POP:   (void)POP(); break;
        case OP_DUP:   { SLValue t = vm->stack[vm->sp-1]; PUSH(t); } break;

        case OP_LOADG:  PUSH(vm->globals[in.a]); break;
        case OP_STOREG: vm->globals[in.a] = POP(); break;

        case OP_LOADL:  { SLFrame* f = &vm->frames[vm->fp-1]; PUSH(vm->stack[f->base + in.a]); } break;
        case OP_STOREL: { SLFrame* f = &vm->frames[vm->fp-1]; vm->stack[f->base + in.a] = POP(); } break;

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
            if (both_int(a,b)) { if(b.as.i==0){set_err(vm,"integer divide by zero");return SLR_ERROR;} PUSH(slval_int(a.as.i/b.as.i)); }
            else PUSH(slval_double(as_num(a)/as_num(b))); } break;
        case OP_MOD: { SLValue b=POP(),a=POP();
            if (both_int(a,b)) { if(b.as.i==0){set_err(vm,"integer modulo by zero");return SLR_ERROR;} PUSH(slval_int(a.as.i%b.as.i)); }
            else { set_err(vm,"modulo requires integers"); return SLR_ERROR; } } break;
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
            int fi = in.a;
            if (fi < 0 || fi >= vm->nfunc) { set_err(vm,"call to unknown function"); return SLR_ERROR; }
            SLFunc* callee = &vm->funcs[fi];
            if (vm->fp >= FRAMES_MAX) { set_err(vm,"call depth exceeded"); return SLR_ERROR; }
            /* args are the top `nargs` stack slots; they become locals 0..nargs-1 */
            int base = vm->sp - callee->nargs;
            SLFrame* nf = &vm->frames[vm->fp++];
            nf->ret_ip = ip;
            nf->base   = base;
            nf->nargs  = callee->nargs;
            /* reserve the remaining locals */
            for (int i = callee->nargs; i < callee->nlocals; i++) PUSH(slval_null());
            ip = callee->entry;
        } break;

        case OP_RET: {
            SLValue rv = (vm->sp > vm->frames[vm->fp-1].base) ? POP() : slval_null();
            SLFrame f = vm->frames[--vm->fp];
            vm->sp = f.base;          /* discard the frame's locals */
            if (f.ret_ip < 0) {       /* returned from entry: done  */
                vm->last_result = rv;
                return SLR_OK;
            }
            PUSH(rv);                 /* deliver return value to caller */
            ip = f.ret_ip;
        } break;

        case OP_PRINT: { SLValue v = POP(); print_value(vm, v); printf("\n"); } break;

        default: set_err(vm, "illegal opcode"); return SLR_ERROR;
        }
    }
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
