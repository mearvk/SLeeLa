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
#define SLART_VERSION 1u

static int wr(FILE* f, const void* p, size_t n) { return fwrite(p, 1, n, f) == n; }
static int rd(FILE* f, void* p, size_t n) { return fread(p, 1, n, f) == n; }

static int wr_u32(FILE* f, uint32_t v) { return wr(f, &v, sizeof(v)); }
static int rd_u32(FILE* f, uint32_t* v) { return rd(f, v, sizeof(*v)); }
static int wr_i32(FILE* f, int32_t v) { return wr(f, &v, sizeof(v)); }
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

static int wr_value(FILE* f, SLValue v) {
    if (!wr_i32(f, (int32_t)v.type)) return 0;
    switch (v.type) {
        case SL_INT:    return wr_u64(f, (uint64_t)v.as.i);
        case SL_DOUBLE: return wr(f, &v.as.d, sizeof(v.as.d));
        case SL_BOOL:   return wr_i32(f, (int32_t)v.as.b);
        case SL_STR:    return wr_i32(f, v.as.s);
        case SL_NULL:   return 1;
        default:        return 0;
    }
}

static int rd_value(FILE* f, SLValue* v) {
    int32_t t = 0;
    if (!rd_i32(f, &t) || t < SL_NULL || t > SL_STR) return 0;
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
        case SL_NULL: return 1;
        default: return 0;
    }
}

static int sane_count(uint32_t n, uint32_t max) { return n <= max; }

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
    ok = ok && wr_i32(f, (int32_t)vm->entry);

    for (int i = 0; ok && i < vm->codelen; ++i) {
        ok = wr(f, &vm->code[i].op, sizeof(vm->code[i].op)) &&
             wr_i32(f, vm->code[i].a);
    }
    for (int i = 0; ok && i < vm->nconst; ++i) ok = wr_value(f, vm->consts[i]);
    for (int i = 0; ok && i < vm->nstr; ++i) ok = wr_string(f, vm->strs[i]);
    for (int i = 0; ok && i < vm->nglobal; ++i) {
        ok = wr_string(f, vm->gnames[i]);
        if (ok) ok = wr_value(f, vm->globals[i]);
    }
    for (int i = 0; ok && i < vm->nfunc; ++i) {
        ok = wr_string(f, vm->funcs[i].name);
        ok = ok && wr_i32(f, vm->funcs[i].entry);
        ok = ok && wr_i32(f, vm->funcs[i].nargs);
        ok = ok && wr_i32(f, vm->funcs[i].nlocals);
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
    uint32_t nglobal = 0, nfunc = 0;
    int32_t entry = -1;

    if (!rd(f, magic, sizeof(magic)) || memcmp(magic, SLART_MAGIC, sizeof(magic)) != 0 ||
        !rd_u32(f, &version) || !rd_u32(f, &endian) || version != SLART_VERSION ||
        endian != 0x01020304u || !rd_u32(f, &codelen) || !rd_u32(f, &nconst) ||
        !rd_u32(f, &nstr) || !rd_u32(f, &nglobal) || !rd_u32(f, &nfunc) ||
        !rd_i32(f, &entry) ||
        !sane_count(codelen, 10000000u) || !sane_count(nconst, 1000000u) ||
        !sane_count(nstr, 1000000u) || !sane_count(nglobal, 100000u) ||
        !sane_count(nfunc, 100000u)) {
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
    if ((codelen && !vm->code) || (nconst && !vm->consts) || (nstr && !vm->strs) ||
        (nglobal && (!vm->globals || !vm->gnames)) || (nfunc && !vm->funcs)) ok = 0;

    if (ok) {
        for (uint32_t i = 0; i < codelen; ++i)
            ok = rd(f, &vm->code[i].op, sizeof(vm->code[i].op)) && rd_i32(f, &vm->code[i].a);
        for (uint32_t i = 0; ok && i < nconst; ++i) ok = rd_value(f, &vm->consts[i]);
        for (uint32_t i = 0; ok && i < nstr; ++i) vm->strs[i] = rd_string(f), ok = vm->strs[i] != NULL;
        for (uint32_t i = 0; ok && i < nglobal; ++i) {
            vm->gnames[i] = rd_string(f);
            ok = vm->gnames[i] != NULL;
            if (ok) ok = rd_value(f, &vm->globals[i]);
        }
        for (uint32_t i = 0; ok && i < nfunc; ++i) {
            vm->funcs[i].name = rd_string(f);
            ok = vm->funcs[i].name != NULL;
            if (ok) ok = rd_i32(f, &vm->funcs[i].entry);
            if (ok) ok = rd_i32(f, &vm->funcs[i].nargs);
            if (ok) ok = rd_i32(f, &vm->funcs[i].nlocals);
        }
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
    return vm;
}
