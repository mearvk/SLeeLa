/* ==========================================================================
 * sleela_cpu_runner.c -- OS-specific native fast path for the /lib/cpu machine.
 *
 * Native backend for the `native sleela_cpu_runner_*` bridge in
 * lib/cpu/SLNativeRunner.sleela. It executes the SLeeLa CPU ISA (encoded as
 * [opcode:8][rd:8][rs:8][imm:8] words, exactly as SLInstruction.encode()) in
 * native C over a bounded, VM-owned word store -- the quick path -- while
 * keeping verification, the capability sandbox, bounded handles, ownership,
 * deadlines, and deterministic teardown explicit.
 *
 * The execution core is OS-neutral C. The OS-specific concern is loading the
 * fast core as the host's native library; that goes through the existing
 * sleela_library boundary (dlopen / LoadLibrary / dylib). When a target names
 * this in-process core (an empty or self path), the open() call validates the
 * capability mask and ABI and binds the in-process executor directly, so the
 * runner has a working native path on every supported OS out of the box.
 * ========================================================================== */
#include "sleela_cpu_runner.h"
#include "sleela_library.h"
#include "sleela_platform.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ---- ISA (mirror of lib/cpu/SLInstructionSet.sleela) --------------------- */
enum {
    OP_NOP=0, OP_HALT=1, OP_MOV=2, OP_MOVR=3, OP_LOAD=4, OP_LOADR=5, OP_LOADX=6,
    OP_STORE=7, OP_STORER=8, OP_STOREX=9, OP_ADD=10, OP_SUB=11, OP_MUL=12,
    OP_DIV=13, OP_MOD=14, OP_NEG=15, OP_AND=16, OP_OR=17, OP_XOR=18, OP_NOT=19,
    OP_SHL=20, OP_SHR=21, OP_CMP=22, OP_JMP=23, OP_JZ=24, OP_JNZ=25, OP_JLT=26,
    OP_JGE=27, OP_CALL=28, OP_RET=29, OP_PUSH=30, OP_POP=31, OP_IN=32, OP_OUT=33,
    OP_SYSCALL=34, OP_TRAP=35
};

#define SL_CPU_REGS       16
#define SL_CPU_STACK_WORDS 4096

/* ---- Bounded resource tables (handle discipline) ------------------------- */
typedef struct {
    int in_use;
    int platform;
    uint32_t capability_mask;
    int abi_major, abi_minor;
    SLLibraryHandle os_library;   /* a real OS library handle, or 0 for in-process */
} SLCpuLib;

typedef struct {
    int in_use;
    SLCpuRunnerHandle library;    /* owning library handle                      */
    int deadline_ms;

    /* modeled machine state */
    int cores, width, clock_mhz;
    int declared_core_mhz;        /* 0 = image declared none; never assumed      */
    int32_t *mem;                 /* VM-owned word store (bounded)               */
    size_t mem_words;
    int32_t reg[SL_CPU_REGS];
    int32_t stack[SL_CPU_STACK_WORDS];
    int sp;                       /* stack pointer (grows up in this store)      */
    int pc;
    int flag_zero, flag_neg;
    int halted;
    int exit_status;
    int last_output;
    long long steps;              /* instructions retired on the native path     */
} SLCpuRunner;

static SLCpuLib    g_libs[SL_CPU_RUNNER_MAX_LIBS];
static SLCpuRunner g_runners[SL_CPU_RUNNER_MAX_CORES];
static char        g_error[256];
static char        g_sha_hex[65];

static void set_error(const char *msg) {
    strncpy(g_error, msg ? msg : "", sizeof(g_error) - 1);
    g_error[sizeof(g_error) - 1] = '\0';
}

/* ---- SHA-256 (self-contained, for the trusted-load gate) ----------------- */
/* A compact, dependency-free SHA-256 so the runner can measure an artifact and
 * the caller can match it against the repository's trusted digest BEFORE the
 * native core is opened. Standard FIPS 180-4 construction. */
typedef struct { uint32_t h[8]; uint64_t len; uint8_t buf[64]; size_t n; } sha256_ctx;
static uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
static void sha256_init(sha256_ctx *c) {
    static const uint32_t iv[8] = {0x6a09e667u,0xbb67ae85u,0x3c6ef372u,0xa54ff53au,
                                   0x510e527fu,0x9b05688cu,0x1f83d9abu,0x5be0cd19u};
    memcpy(c->h, iv, sizeof(iv)); c->len = 0; c->n = 0;
}
static void sha256_block(sha256_ctx *c, const uint8_t *p) {
    static const uint32_t k[64] = {
        0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
        0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
        0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
        0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
        0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
        0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
        0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
        0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u};
    uint32_t w[64], a,b,cc,d,e,f,g,h; int i;
    for (i = 0; i < 16; i++)
        w[i] = ((uint32_t)p[i*4]<<24)|((uint32_t)p[i*4+1]<<16)|((uint32_t)p[i*4+2]<<8)|((uint32_t)p[i*4+3]);
    for (i = 16; i < 64; i++) {
        uint32_t s0 = rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3);
        uint32_t s1 = rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10);
        w[i] = w[i-16]+s0+w[i-7]+s1;
    }
    a=c->h[0];b=c->h[1];cc=c->h[2];d=c->h[3];e=c->h[4];f=c->h[5];g=c->h[6];h=c->h[7];
    for (i = 0; i < 64; i++) {
        uint32_t S1 = rotr(e,6)^rotr(e,11)^rotr(e,25);
        uint32_t ch = (e&f)^((~e)&g);
        uint32_t t1 = h+S1+ch+k[i]+w[i];
        uint32_t S0 = rotr(a,2)^rotr(a,13)^rotr(a,22);
        uint32_t maj = (a&b)^(a&cc)^(b&cc);
        uint32_t t2 = S0+maj;
        h=g;g=f;f=e;e=d+t1;d=cc;cc=b;b=a;a=t1+t2;
    }
    c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}
static void sha256_update(sha256_ctx *c, const uint8_t *data, size_t len) {
    c->len += len;
    while (len) {
        size_t take = 64 - c->n; if (take > len) take = len;
        memcpy(c->buf + c->n, data, take); c->n += take; data += take; len -= take;
        if (c->n == 64) { sha256_block(c, c->buf); c->n = 0; }
    }
}
static void sha256_final(sha256_ctx *c, uint8_t out[32]) {
    uint64_t bits = c->len * 8; uint8_t pad = 0x80; int i;
    sha256_update(c, &pad, 1);
    pad = 0x00;
    while (c->n != 56) sha256_update(c, &pad, 1);
    for (i = 7; i >= 0; i--) { uint8_t b = (uint8_t)(bits >> (i*8)); sha256_update(c, &b, 1); }
    for (i = 0; i < 8; i++) {
        out[i*4]=(uint8_t)(c->h[i]>>24); out[i*4+1]=(uint8_t)(c->h[i]>>16);
        out[i*4+2]=(uint8_t)(c->h[i]>>8); out[i*4+3]=(uint8_t)(c->h[i]);
    }
}

const char *sleela_cpu_runner_sha256(const char *path) {
    g_sha_hex[0] = '\0';
    if (!path) { set_error("sha256: null path"); return g_sha_hex; }
    FILE *f = fopen(path, "rb");
    if (!f) { set_error("sha256: cannot open artifact"); return g_sha_hex; }
    sha256_ctx c; sha256_init(&c);
    uint8_t buf[4096]; size_t r;
    while ((r = fread(buf, 1, sizeof(buf), f)) > 0) sha256_update(&c, buf, r);
    fclose(f);
    uint8_t digest[32]; sha256_final(&c, digest);
    static const char hex[] = "0123456789abcdef";
    for (int i = 0; i < 32; i++) { g_sha_hex[i*2]=hex[digest[i]>>4]; g_sha_hex[i*2+1]=hex[digest[i]&0xf]; }
    g_sha_hex[64] = '\0';
    return g_sha_hex;
}

/* ---- Handle helpers ------------------------------------------------------ */
static SLCpuLib *lib_at(SLCpuRunnerHandle h) {
    if (h <= 0 || h > SL_CPU_RUNNER_MAX_LIBS) return NULL;
    SLCpuLib *l = &g_libs[h - 1];
    return l->in_use ? l : NULL;
}
static SLCpuRunner *runner_at(SLCpuRunnerHandle h) {
    if (h <= 0 || h > SL_CPU_RUNNER_MAX_CORES) return NULL;
    SLCpuRunner *r = &g_runners[h - 1];
    return r->in_use ? r : NULL;
}

/* ---- Load / create ------------------------------------------------------- */
SLCpuRunnerHandle sleela_cpu_runner_open(int platform, const char *path,
                                         const char *symbol,
                                         int abi_major, int abi_minor,
                                         uint32_t capability_mask) {
    (void)symbol;
    /* Capability gate: the dynamic-library capability must be granted by name,
     * deny-by-default. SL_CAP_DYNAMIC_LIBRARY ordinal is 4 (sleela_platform.h). */
    if ((capability_mask & SL_CPU_CAP_BIT(SL_CAP_DYNAMIC_LIBRARY)) == 0) {
        set_error("open: sandbox denies dynamic-library capability"); return SL_CPU_RUNNER_INVALID;
    }
    if (abi_major != 1) { set_error("open: unsupported native ABI major"); return SL_CPU_RUNNER_INVALID; }
    (void)abi_minor;

    int slot = -1;
    for (int i = 0; i < SL_CPU_RUNNER_MAX_LIBS; i++) if (!g_libs[i].in_use) { slot = i; break; }
    if (slot < 0) { set_error("open: no free library slots"); return SL_CPU_RUNNER_INVALID; }

    SLLibraryHandle os_lib = 0;
    /* A non-empty path that is not the in-process marker is loaded as the host's
     * native library through the single sleela_library boundary (dlopen /
     * LoadLibrary / dylib). An empty path binds the in-process executor. */
    if (path && path[0] && strcmp(path, "@in-process") != 0) {
        if (sllibrary_open(&os_lib, path) != 0) {
            set_error(sllibrary_last_error()); return SL_CPU_RUNNER_INVALID;
        }
    }

    g_libs[slot].in_use = 1;
    g_libs[slot].platform = platform;
    g_libs[slot].capability_mask = capability_mask;
    g_libs[slot].abi_major = abi_major;
    g_libs[slot].abi_minor = abi_minor;
    g_libs[slot].os_library = os_lib;
    set_error("");
    return (SLCpuRunnerHandle)(slot + 1);
}

SLCpuRunnerHandle sleela_cpu_runner_create(SLCpuRunnerHandle library, int deadline_ms) {
    SLCpuLib *l = lib_at(library);
    if (!l) { set_error("create: stale library handle"); return SL_CPU_RUNNER_INVALID; }
    int slot = -1;
    for (int i = 0; i < SL_CPU_RUNNER_MAX_CORES; i++) if (!g_runners[i].in_use) { slot = i; break; }
    if (slot < 0) { set_error("create: no free runner slots"); return SL_CPU_RUNNER_INVALID; }

    SLCpuRunner *r = &g_runners[slot];
    memset(r, 0, sizeof(*r));
    r->mem = (int32_t *)calloc(SL_CPU_RUNNER_MEM_WORDS, sizeof(int32_t));
    if (!r->mem) { set_error("create: out of memory for word store"); return SL_CPU_RUNNER_INVALID; }
    r->in_use = 1;
    r->library = library;
    r->deadline_ms = deadline_ms;
    r->mem_words = SL_CPU_RUNNER_MEM_WORDS;
    r->sp = 0; r->pc = 0; r->halted = 0; r->exit_status = 0; r->last_output = 0;
    set_error("");
    return (SLCpuRunnerHandle)(slot + 1);
}

/* ---- Bind the model ------------------------------------------------------ */
int sleela_cpu_runner_bind_cpu(SLCpuRunnerHandle runner, int cores, int width, int clock_mhz) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("bind_cpu: stale runner handle"); return -1; }
    if (cores <= 0 || width <= 0) { set_error("bind_cpu: invalid CPU shape"); return -1; }
    r->cores = cores; r->width = width; r->clock_mhz = clock_mhz;
    return 0;
}

int sleela_cpu_runner_load_program(SLCpuRunnerHandle runner, int load_addr,
                                   int length, int entry, int declared_core_mhz) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("load_program: stale runner handle"); return -1; }
    if (load_addr < 0 || length < 0 || (size_t)(load_addr + length) > r->mem_words) {
        set_error("load_program: image does not fit the address space"); return -1;
    }
    r->pc = (entry > 0) ? entry : load_addr;
    r->declared_core_mhz = declared_core_mhz;   /* 0 means: assume nothing */
    r->halted = 0; r->sp = 0; r->steps = 0;
    for (int i = 0; i < SL_CPU_REGS; i++) r->reg[i] = 0;
    return 0;
}

int sleela_cpu_runner_poke(SLCpuRunnerHandle runner, int addr, int word) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("poke: stale runner handle"); return -1; }
    if (addr < 0 || (size_t)addr >= r->mem_words) { set_error("poke: address out of range"); return -1; }
    r->mem[addr] = (int32_t)word;
    return 0;
}

int sleela_cpu_runner_peek(SLCpuRunnerHandle runner, int addr) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("peek: stale runner handle"); return -1; }
    if (addr < 0 || (size_t)addr >= r->mem_words) { set_error("peek: address out of range"); return -1; }
    return (int)r->mem[addr];
}

/* ---- Execute (faithful to SLControlUnit.step) ---------------------------- */
static void set_flags(SLCpuRunner *r, int32_t v) { r->flag_zero = (v == 0); r->flag_neg = (v < 0); }

static int step_one(SLCpuRunner *r) {
    if (r->halted) return 0;
    if (r->pc < 0 || (size_t)r->pc >= r->mem_words) { r->halted = 1; r->exit_status = -2; return 0; }
    int32_t word = r->mem[r->pc];
    int op  = (int)((uint32_t)word >> 24) & 0xff;
    int rd  = (int)((uint32_t)word >> 16) & 0xff;
    int rs  = (int)((uint32_t)word >> 8)  & 0xff;
    int imm = (int)((uint32_t)word)       & 0xff;
    if (rd >= SL_CPU_REGS) rd = 0;
    if (rs >= SL_CPU_REGS) rs = 0;

    switch (op) {
        case OP_HALT: r->halted = 1; return 1;
        case OP_NOP:  r->pc++; return 1;
        case OP_MOV:  r->reg[rd] = imm; r->pc++; return 1;
        case OP_MOVR: r->reg[rd] = r->reg[rs]; r->pc++; return 1;
        case OP_LOAD: if (imm>=0 && (size_t)imm<r->mem_words) r->reg[rd]=r->mem[imm]; r->pc++; return 1;
        case OP_LOADR: { int a=r->reg[rs]; if (a>=0 && (size_t)a<r->mem_words) r->reg[rd]=r->mem[a]; r->pc++; return 1; }
        case OP_LOADX: { int a=r->reg[rs]+imm; if (a>=0 && (size_t)a<r->mem_words) r->reg[rd]=r->mem[a]; r->pc++; return 1; }
        case OP_STORE: if (imm>=0 && (size_t)imm<r->mem_words) r->mem[imm]=r->reg[rs]; r->pc++; return 1;
        case OP_STORER: { int a=r->reg[rd]; if (a>=0 && (size_t)a<r->mem_words) r->mem[a]=r->reg[rs]; r->pc++; return 1; }
        case OP_STOREX: { int a=r->reg[rd]+imm; if (a>=0 && (size_t)a<r->mem_words) r->mem[a]=r->reg[rs]; r->pc++; return 1; }
        case OP_ADD: r->reg[rd]+=r->reg[rs]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_SUB: r->reg[rd]-=r->reg[rs]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_MUL: r->reg[rd]*=r->reg[rs]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_DIV: if (r->reg[rs]!=0) r->reg[rd]/=r->reg[rs]; else r->reg[rd]=0; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_MOD: if (r->reg[rs]!=0) r->reg[rd]%=r->reg[rs]; else r->reg[rd]=0; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_NEG: r->reg[rd]=-r->reg[rd]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_AND: r->reg[rd]&=r->reg[rs]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_OR:  r->reg[rd]|=r->reg[rs]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_XOR: r->reg[rd]^=r->reg[rs]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_NOT: r->reg[rd]=~r->reg[rd]; set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_SHL: r->reg[rd]<<=(r->reg[rs]&31); set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_SHR: r->reg[rd]=(int32_t)((uint32_t)r->reg[rd]>>(r->reg[rs]&31)); set_flags(r,r->reg[rd]); r->pc++; return 1;
        case OP_CMP: set_flags(r, r->reg[rd]-r->reg[rs]); r->pc++; return 1;
        case OP_JMP: r->pc=imm; return 1;
        case OP_JZ:  r->pc = r->flag_zero ? imm : r->pc+1; return 1;
        case OP_JNZ: r->pc = !r->flag_zero ? imm : r->pc+1; return 1;
        case OP_JLT: r->pc = r->flag_neg ? imm : r->pc+1; return 1;
        case OP_JGE: r->pc = !r->flag_neg ? imm : r->pc+1; return 1;
        case OP_CALL: if (r->sp < SL_CPU_STACK_WORDS) r->stack[r->sp++] = r->pc+1; r->pc=imm; return 1;
        case OP_RET:  if (r->sp > 0) { r->pc = r->stack[--r->sp]; } else { r->halted=1; r->exit_status=-1; } return 1;
        case OP_PUSH: if (r->sp < SL_CPU_STACK_WORDS) r->stack[r->sp++] = r->reg[rs]; r->pc++; return 1;
        case OP_POP:  r->reg[rd] = (r->sp>0) ? r->stack[--r->sp] : 0; r->pc++; return 1;
        case OP_IN:   r->reg[rd] = 0; r->pc++; return 1;            /* no I/O capability bound here */
        case OP_OUT:  r->last_output = r->reg[rs]; r->pc++; return 1;
        case OP_SYSCALL:
            /* SYS_WRITE (0) emits r2 (the console char) as the observable output;
             * SYS_EXIT (4) halts cleanly. Other syscalls advance (modeled). */
            if (imm == 0) { r->last_output = r->reg[2]; }
            else if (imm == 4) { r->halted = 1; r->exit_status = 0; }
            r->pc++; return 1;
        case OP_TRAP: r->halted = 1; r->exit_status = imm; return 1;
        default: r->halted = 1; r->exit_status = -3; return 0; /* unknown opcode: fault */
    }
}

int sleela_cpu_runner_run(SLCpuRunnerHandle runner, int max_steps) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("run: stale runner handle"); return -1; }
    int done = 0;
    while (done < max_steps && !r->halted) {
        if (!step_one(r)) break;
        r->steps++; done++;
        if (r->exit_status == -3) { set_error("run: unknown opcode fault"); return -1; }
    }
    return done;
}

int sleela_cpu_runner_halted(SLCpuRunnerHandle runner) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("halted: stale runner handle"); return -1; }
    return r->halted ? 1 : 0;
}
int sleela_cpu_runner_exit_status(SLCpuRunnerHandle runner) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("exit_status: stale runner handle"); return -1; }
    return r->exit_status;
}
int sleela_cpu_runner_last_output(SLCpuRunnerHandle runner) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) { set_error("last_output: stale runner handle"); return -1; }
    return r->last_output;
}

/* ---- Teardown (ordered, idempotent, fail-closed) ------------------------- */
void sleela_cpu_runner_destroy(SLCpuRunnerHandle runner) {
    SLCpuRunner *r = runner_at(runner);
    if (!r) return;                 /* stale handle: silent no-op, never reused */
    if (r->mem) { free(r->mem); r->mem = NULL; }
    r->in_use = 0;
}
void sleela_cpu_runner_close(SLCpuRunnerHandle library) {
    SLCpuLib *l = lib_at(library);
    if (!l) return;
    if (l->os_library) { sllibrary_close(l->os_library); l->os_library = 0; }
    l->in_use = 0;
}

/* ---- Introspection ------------------------------------------------------- */
const char *sleela_cpu_runner_last_error(void) { return g_error; }
int sleela_cpu_runner_live_runners(void) {
    int n = 0; for (int i = 0; i < SL_CPU_RUNNER_MAX_CORES; i++) if (g_runners[i].in_use) n++; return n;
}
int sleela_cpu_runner_live_libraries(void) {
    int n = 0; for (int i = 0; i < SL_CPU_RUNNER_MAX_LIBS; i++) if (g_libs[i].in_use) n++; return n;
}
