/* ===========================================================================
 * sleela_artifact.c -- persistent Sleela Core runnable artifact support.
 *
 * This translation unit intentionally includes the core implementation so the
 * artifact serializer/loader can use the VM's private program representation
 * without exposing mutable execution state in the public ABI.
 *
 * A .sleela artifact is compiled Core bytecode, not Sleela source.  It can be
 * loaded directly by the Sleela runtime and therefore does not require a
 * second front-end compilation pass.
 * =========================================================================== */
#define SLEELA_ARTIFACT_EMBED_CORE 1
#include "sleela_core.c"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SLART_MAGIC "SLEELA-ART\x01"
#define SLART_MAGIC_LEN 11
/* Artifact format v2 adds struct layouts; v3 adds lossless UTF-16 constants
 * and globals. Versions 1 and 2 remain loadable by the v3 runtime. */
#define SLART_VERSION 3u

static int wr(FILE* f, const void* p, size_t n) { return fwrite(p, 1, n, f) == n; }
static int rd(FILE* f, void* p, size_t n) { return fread(p, 1, n, f) == n; }

static int wr_u32(FILE* f, uint32_t v) { return wr(f, &v, sizeof(v)); }
static int rd_u32(FILE* f, uint32_t* v) { return rd(f, v, sizeof(*v)); }
static int wr_i32(FILE* f, int32_t v) { return wr(f, &v, sizeof(v)); }
static int wr_u32_le(FILE* f, uint32_t v) { uint8_t b[4]={(uint8_t)v,(uint8_t)(v>>8),(uint8_t)(v>>16),(uint8_t)(v>>24)}; return wr(f,b,4); }
static int rd_u32_le(FILE* f, uint32_t* v) { uint8_t b[4]; if(!rd(f,b,4)) return 0; *v=(uint32_t)b[0]|((uint32_t)b[1]<<8)|((uint32_t)b[2]<<16)|((uint32_t)b[3]<<24); return 1; }
static int wr_u16_le(FILE* f, uint16_t v) { uint8_t b[2]={(uint8_t)v,(uint8_t)(v>>8)}; return wr(f,b,2); }
static int rd_u16_le(FILE* f, uint16_t* v) { uint8_t b[2]; if(!rd(f,b,2)) return 0; *v=(uint16_t)b[0]|((uint16_t)b[1]<<8); return 1; }
static int rd_i32(FILE* f, int32_t* v) { return rd(f, v, sizeof(*v)); }
static int wr_u64(FILE* f, uint64_t v) { return wr(f, &v, sizeof(v)); }
static int rd_u64(FILE* f, uint64_t* v) { return rd(f, v, sizeof(*v)); }

static int wr_string(FILE* f, const char* s) {
    uint32_t n = s ? (uint32_t)strlen(s) : 0u;
    return wr_u32(f, n) && (!n || wr(f, s, n));
}

static char* rd_string(FILE* f) {
    uint32_t n = 0;
    if (!rd_u32(f, &n) || n > (16u * 1024u * 1024u)) return NULL;
    char* s = (char*)malloc((size_t)n + 1u);
    if (!s) return NULL;
    if (n && !rd(f, s, n)) { free(s); return NULL; }
    s[n] = '\0';
    return s;
}

static int wr_value(FILE* f, SLVM* vm, SLValue v) {
    if (!wr_i32(f, (int32_t)v.type)) return 0;
    switch (v.type) {
        case SL_INT:    return wr_u64(f, (uint64_t)v.as.i);
        case SL_DOUBLE: return wr(f, &v.as.d, sizeof(v.as.d));
        case SL_BOOL:   return wr_i32(f, (int32_t)v.as.b);
        case SL_STR:    return wr_i32(f, v.as.s);
        /* A live struct handle is VM-local runtime state (like a socket/file
         * handle) and is not persisted; a global holding one saves as null. */
        case SL_STRUCT: return 1;
        case SL_UTF16: {
            SLJavaStringInstance *instance = java_string_get(vm, v);
            if (!instance || instance->value.length > INT32_MAX || !wr_u32_le(f, (uint32_t)instance->value.length)) return 0;
            for (size_t i = 0; i < instance->value.length; ++i)
                if (!wr_u16_le(f, instance->value.units[i])) return 0;
            return 1;
        }
        case SL_NULL:   return 1;
        default:        return 0;
    }
}

static int rd_value(FILE* f, SLVM* vm, SLValue* v, uint32_t artifact_version) {
    int32_t t = 0;
    if (!rd_i32(f, &t) || t < SL_NULL || t > (artifact_version >= 3u ? SL_UTF16 : SL_STRUCT)) return 0;
    memset(v, 0, sizeof(*v));
    v->type = (SLType)t;
    switch (v->type) {
        case SL_INT: {
            uint64_t x;
            if (!rd_u64(f, &x)) return 0;
            v->as.i = (int64_t)x;
            return 1;
        }
        case SL_DOUBLE: return rd(f, &v->as.d, sizeof(v->as.d));
        case SL_BOOL: {
            int32_t x;
            if (!rd_i32(f, &x)) return 0;
            v->as.b = x ? 1 : 0;
            return 1;
        }
        case SL_STR: return rd_i32(f, &v->as.s);
        /* A persisted struct-typed value is loaded as null: the handle it once
         * named is not meaningful in a freshly loaded VM. */
        case SL_STRUCT: v->type = SL_NULL; v->as.i = 0; return 1;
        case SL_UTF16: {
            uint32_t length = 0;
            uint16_t *units = NULL;
            if (!rd_u32_le(f, &length) || length > INT32_MAX ||
                (size_t)length > SIZE_MAX / sizeof(uint16_t)) return 0;
            if (length) {
                units = (uint16_t *)malloc((size_t)length * sizeof(uint16_t));
                if (!units) return 0;
            }
            for (uint32_t i = 0; i < length; ++i) {
                if (!rd_u16_le(f, &units[i])) { free(units); return 0; }
            }
            *v = slvm_java_string_new(vm, units, length);
            free(units);
            return v->type == SL_UTF16;
        }
        case SL_NULL: return 1;
        default: return 0;
    }
}

static int sane_count(uint32_t n, uint32_t max) { return n <= max; }
static void abi_error(char* error, size_t error_size, const char* msg) {
    if (error && error_size) {
        snprintf(error, error_size, "%s", msg ? msg : "artifact ABI validation failed");
    }
}

static int validate_loaded_vm(const SLVM* vm, char* error, size_t error_size) {
    if (!vm) { abi_error(error, error_size, "null VM"); return 0; }
    if (vm->entry < 0 || vm->entry >= vm->nfunc) { abi_error(error,error_size,"entry function is outside function table"); return 0; }
    for (int i=0;i<vm->nfunc;i++) {
        const SLFunc* fn=&vm->funcs[i];
        if (fn->entry < 0 || fn->entry >= vm->codelen) { abi_error(error,error_size,"function entry is outside code"); return 0; }
        /* nlocals counts the operand-STACK slots a frame lays down at entry
         * (args + body locals), so it is bounded by the stack[] buffer
         * (STACK_MAX), NOT by the frames[] buffer (FRAMES_MAX, the call-DEPTH
         * limit). Validating nlocals against FRAMES_MAX wrongly rejected an
         * otherwise-legal artifact whose entry (e.g. a local, non-/lib `main`)
         * declared 1025..STACK_MAX locals -- a file that runs fine straight from
         * source but failed to load once compiled to a Core artifact. */
        if (fn->nargs < 0 || fn->nlocals < fn->nargs || fn->nlocals > STACK_MAX) { abi_error(error,error_size,"function local count exceeds operand-stack limit"); return 0; }
    }
    for (int i=0;i<vm->codelen;i++) {
        const SLInstr* in=&vm->code[i];
        if ((unsigned)in->op >= (unsigned)OP__COUNT) { abi_error(error,error_size,"unknown opcode in artifact"); return 0; }
        switch (in->op) {
            case OP_CONST:
                if (in->a < 0 || in->a >= vm->nconst) { abi_error(error,error_size,"constant index outside constant table"); return 0; }
                break;
            case OP_LOADG: case OP_STOREG:
                if (in->a < 0 || in->a >= vm->nglobal) { abi_error(error,error_size,"global index outside global table"); return 0; }
                break;
            case OP_CALL: case OP_SPAWN:
                if (in->a < 0 || in->a >= vm->nfunc) { abi_error(error,error_size,"call target outside function table"); return 0; }
                break;
            case OP_NATIVE_CALL:
                if (in->a < 0 || in->a > STACK_MAX - 2) { abi_error(error,error_size,"native-call argument count outside operand-stack range"); return 0; }
                break;
            case OP_MATH_CALL:
                if (in->a != SLMATH_OP_VECTOR_ADD &&
                    in->a != SLMATH_OP_VECTOR_DOT &&
                    in->a != SLMATH_OP_VECTOR_NORM &&
                    in->a != SLMATH_OP_VECTOR_SCALE &&
                    in->a != SLMATH_OP_MATRIX_MULTIPLY &&
                    in->a != SLMATH_OP_MATRIX_TRANSPOSE &&
                    in->a != SLMATH_OP_MATRIX_INVERSE &&
                    in->a != SLMATH_OP_COMPLEX_MULTIPLY &&
                    in->a != SLMATH_OP_QR &&
                    in->a != SLMATH_OP_SYMMETRIC_EIGEN &&
                    in->a != SLMATH_OP_FFT) {
                    abi_error(error,error_size,"unknown native math operation in artifact"); return 0;
                }
                break;
            case OP_JMP: case OP_JMPF: case OP_TRY_PUSH:
                if (in->a < 0 || in->a >= vm->codelen) { abi_error(error,error_size,"control-flow target outside code"); return 0; }
                break;
            case OP_TRY_POP: case OP_THROW:
                if (in->a != 0) { abi_error(error,error_size,"exception opcode has an unexpected operand"); return 0; }
                break;
            case OP_NEWSTRUCT: case OP_STRUCTUNPACK:
                if (in->a < 0 || in->a >= vm->nstruct_types) { abi_error(error,error_size,"struct type index outside struct table"); return 0; }
                break;
            case OP_GETFIELD: case OP_SETFIELD:
                if (in->a < 0 || in->a >= SL_MAX_STRUCT_FIELDS) { abi_error(error,error_size,"field operand outside ABI field range"); return 0; }
                break;

            case OP_LOCK: case OP_UNLOCK:
            case OP_SEND: case OP_RECV:
                if (in->a < 0 || in->a >= SL_MAX_LOCKS) { abi_error(error,error_size,"synchronization slot outside ABI range"); return 0; }
                break;
            case OP_SYN_STAT: case OP_BEST_STAT:
                if (in->a < 0 || in->a > 6) { abi_error(error,error_size,"stat selector outside ABI range"); return 0; }
                break;
            default: break;
        }
    }
    for (int i=0;i<vm->nstruct_types;i++) {
        if (vm->struct_types[i].nfields < 0 || vm->struct_types[i].nfields > SL_MAX_STRUCT_FIELDS) {
            abi_error(error,error_size,"struct field count exceeds ABI limit"); return 0;
        }
    }
    return 1;
}


uint32_t slvm_artifact_abi_version(void) {
    return (SLEELA_VM_ABI_MAJOR << 16) | SLEELA_VM_ABI_MINOR;
}

int slvm_is_artifact_file(const char* path) {
    if (!path) return 0;
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    char magic[SLART_MAGIC_LEN];
    int ok = rd(f, magic, sizeof(magic)) && memcmp(magic, SLART_MAGIC, sizeof(magic)) == 0;
    fclose(f);
    return ok;
}

int slvm_save_file(SLVM* vm, const char* path) {
    if (!vm || !path) return -1;
    FILE* f = fopen(path, "wb");
    if (!f) return -2;

    int ok = 1;
    ok = ok && wr(f, SLART_MAGIC, SLART_MAGIC_LEN);
    ok = ok && wr_u32(f, SLART_VERSION);
    ok = ok && wr_u32(f, 0x01020304u); /* serialization endian marker */
    ok = ok && wr_u32(f, (uint32_t)vm->codelen);
    ok = ok && wr_u32(f, (uint32_t)vm->nconst);
    ok = ok && wr_u32(f, (uint32_t)vm->nstr);
    ok = ok && wr_u32(f, (uint32_t)vm->nglobal);
    ok = ok && wr_u32(f, (uint32_t)vm->nfunc);
    ok = ok && wr_u32(f, (uint32_t)vm->nstruct_types);
    ok = ok && wr_i32(f, (int32_t)vm->entry);

    for (int i = 0; ok && i < vm->codelen; ++i) {
        ok = wr(f, &vm->code[i].op, sizeof(vm->code[i].op)) &&
             wr_i32(f, vm->code[i].a);
    }
    for (int i = 0; ok && i < vm->nconst; ++i) ok = wr_value(f, vm, vm->consts[i]);
    for (int i = 0; ok && i < vm->nstr; ++i) ok = wr_string(f, vm->strs[i]);
    for (int i = 0; ok && i < vm->nglobal; ++i) {
        ok = wr_string(f, vm->gnames[i]);
        if (ok) ok = wr_value(f, vm, vm->globals[i]);
    }
    for (int i = 0; ok && i < vm->nfunc; ++i) {
        ok = wr_string(f, vm->funcs[i].name);
        ok = ok && wr_i32(f, vm->funcs[i].entry);
        ok = ok && wr_i32(f, vm->funcs[i].nargs);
        ok = ok && wr_i32(f, vm->funcs[i].nlocals);
    }
    for (int i = 0; ok && i < vm->nstruct_types; ++i) {
        ok = wr_string(f, vm->struct_types[i].name);
        ok = ok && wr_i32(f, vm->struct_types[i].nfields);
        for (int j = 0; ok && j < vm->struct_types[i].nfields; ++j)
            ok = wr_string(f, vm->struct_types[i].fields[j]);
    }

    if (fclose(f) != 0) ok = 0;
    if (!ok) remove(path);
    return ok ? 0 : -3;
}

SLVM* slvm_load_file(const char* path) {
    if (!path) return NULL;
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;

    char magic[SLART_MAGIC_LEN];
    uint32_t version = 0, endian = 0, codelen = 0, nconst = 0, nstr = 0;
    uint32_t nglobal = 0, nfunc = 0, nstruct = 0;
    int32_t entry = -1;

    if (!rd(f, magic, sizeof(magic)) || memcmp(magic, SLART_MAGIC, sizeof(magic)) != 0 ||
        !rd_u32(f, &version) || !rd_u32(f, &endian) ||
        (version != 1u && version != 2u && version != SLART_VERSION) ||
        endian != 0x01020304u || !rd_u32(f, &codelen) || !rd_u32(f, &nconst) ||
        !rd_u32(f, &nstr) || !rd_u32(f, &nglobal) || !rd_u32(f, &nfunc) ||
        (version >= 2u && !rd_u32(f, &nstruct)) ||
        !rd_i32(f, &entry) ||
        !sane_count(codelen, 10000000u) || !sane_count(nconst, 1000000u) ||
        !sane_count(nstr, 1000000u) || !sane_count(nglobal, 100000u) ||
        !sane_count(nfunc, 100000u) || !sane_count(nstruct, SL_MAX_STRUCT_TYPES)) {
        fclose(f);
        return NULL;
    }

    SLVM* vm = slvm_new();
    if (!vm) { fclose(f); return NULL; }

    int ok = 1;
    vm->code = codelen ? (SLInstr*)calloc(codelen, sizeof(SLInstr)) : NULL;
    vm->consts = nconst ? (SLValue*)calloc(nconst, sizeof(SLValue)) : NULL;
    vm->strs = nstr ? (char**)calloc(nstr, sizeof(char*)) : NULL;
    vm->globals = nglobal ? (SLValue*)calloc(nglobal, sizeof(SLValue)) : NULL;
    vm->gnames = nglobal ? (char**)calloc(nglobal, sizeof(char*)) : NULL;
    vm->funcs = nfunc ? (SLFunc*)calloc(nfunc, sizeof(SLFunc)) : NULL;
    /* The layout table is a growable heap array (slvm_new leaves it NULL); the
     * loader sizes it exactly to the artifact's struct-type count, which
     * sane_count already bounded by the SL_MAX_STRUCT_TYPES ceiling. */
    vm->struct_types = nstruct ? (SLStructType*)calloc(nstruct, sizeof(SLStructType)) : NULL;
    vm->struct_typecap = (int)nstruct;
    if ((codelen && !vm->code) || (nconst && !vm->consts) || (nstr && !vm->strs) ||
        (nglobal && (!vm->globals || !vm->gnames)) || (nfunc && !vm->funcs) ||
        (nstruct && !vm->struct_types)) ok = 0;

    if (ok) {
        for (uint32_t i = 0; ok && i < codelen; ++i)
            ok = rd(f, &vm->code[i].op, sizeof(vm->code[i].op)) && rd_i32(f, &vm->code[i].a);
        for (uint32_t i = 0; ok && i < nconst; ++i) ok = rd_value(f, vm, &vm->consts[i], version);
        for (uint32_t i = 0; ok && i < nstr; ++i) vm->strs[i] = rd_string(f), ok = vm->strs[i] != NULL;
        for (uint32_t i = 0; ok && i < nglobal; ++i) {
            vm->gnames[i] = rd_string(f);
            ok = vm->gnames[i] != NULL;
            if (ok) ok = rd_value(f, vm, &vm->globals[i], version);
        }
        for (uint32_t i = 0; ok && i < nfunc; ++i) {
            vm->funcs[i].name = rd_string(f);
            ok = vm->funcs[i].name != NULL;
            if (ok) ok = rd_i32(f, &vm->funcs[i].entry);
            if (ok) ok = rd_i32(f, &vm->funcs[i].nargs);
            if (ok) ok = rd_i32(f, &vm->funcs[i].nlocals);
        }
        for (uint32_t i = 0; ok && i < nstruct; ++i) {
            SLStructType* st = &vm->struct_types[i];
            st->name = rd_string(f);
            ok = st->name != NULL;
            int32_t nf = 0;
            if (ok) ok = rd_i32(f, &nf);
            if (ok && (nf < 0 || nf > SL_MAX_STRUCT_FIELDS)) ok = 0;
            if (ok) {
                st->nfields = nf;
                for (int32_t j = 0; ok && j < nf; ++j) {
                    st->fields[j] = rd_string(f);
                    ok = st->fields[j] != NULL;
                }
            }
        }
        if (ok) vm->nstruct_types = (int)nstruct;
    }
    fclose(f);

    if (!ok || entry < 0 || entry >= (int32_t)nfunc) {
        slvm_free(vm);
        return NULL;
    }

    vm->codelen = vm->codecap = (int)codelen;
    vm->nconst = vm->constcap = (int)nconst;
    vm->nstr = vm->strcap = (int)nstr;
    vm->nglobal = vm->globalcap = (int)nglobal;
    vm->nfunc = vm->funccap = (int)nfunc;
    vm->entry = entry;
    vm->cur_func = -1;
    { char abi_error_text[256]; if (!validate_loaded_vm(vm, abi_error_text, sizeof(abi_error_text))) { slvm_free(vm); return NULL; } }
    return vm;
}

int slvm_validate_artifact_file(const char* path, char* error, size_t error_size) {
    if (!path) { abi_error(error,error_size,"artifact path is null"); return 0; }
    SLVM* vm = slvm_load_file(path);
    if (!vm) { abi_error(error,error_size,"artifact failed format/ABI validation"); return 0; }
    int ok = validate_loaded_vm(vm,error,error_size);
    slvm_free(vm);
    return ok;
}
