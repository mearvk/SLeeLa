/* ==========================================================================
 * thread_smoke.c  --  Exercises the Sleela core threading model directly
 * through the builder API (no Sleela source involved).
 *
 * Program assembled:
 *
 *   global counter = 0
 *
 *   worker():                     // runs on its own thread
 *       lock 0
 *       counter = counter + 1
 *       unlock 0
 *       send (slot 1) <- 1        // announce completion on the burble line
 *       ret
 *
 *   main():
 *       i = 0
 *       while (i < N):
 *           spawn worker
 *           i = i + 1
 *       // drain N completion tuples off mailbox slot 1
 *       j = 0
 *       while (j < N):
 *           recv slot 1           // (value discarded)
 *           j = j + 1
 *       joinall
 *       print counter             // must equal N
 *       ret
 *
 * With N up to 128 and a shared lock, `counter` must come out exactly N,
 * demonstrating race-free increments and working tuple hand-off.
 * ========================================================================== */
#include "sleela_core.h"
#include <stdio.h>
#include <stdlib.h>

static int N = 128;   /* number of worker threads */

int main(int argc, char** argv) {
    if (argc > 1) N = atoi(argv[1]);
    if (N < 1) N = 1;
    if (N > SL_MAX_THREADS) N = SL_MAX_THREADS;

    SLVM* vm = slvm_new();

    int g_counter = slvm_declare_global(vm, "counter");

    /* constants */
    int c0  = slvm_add_const_int(vm, 0);
    int c1  = slvm_add_const_int(vm, 1);
    int cN  = slvm_add_const_int(vm, N);

    /* ---- worker() : func index 0 ; 0 args, 0 locals ---- */
    int fworker = slvm_begin_func(vm, "worker", 0, 0);
    slvm_emit(vm, OP_LOCK, 0);
    slvm_emit(vm, OP_LOADG, g_counter);
    slvm_emit(vm, OP_CONST, c1);
    slvm_emit0(vm, OP_ADD);
    slvm_emit(vm, OP_STOREG, g_counter);
    slvm_emit(vm, OP_UNLOCK, 0);
    slvm_emit(vm, OP_CONST, c1);        /* value of the completion tuple */
    slvm_emit(vm, OP_SEND, 1);          /* send (slot 1, 1) */
    slvm_emit(vm, OP_CONST, c0);        /* return 0 */
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);

    /* ---- main() : func index 1 ; 0 args, 2 locals (i=0, j=1) ---- */
    int fmain = slvm_begin_func(vm, "main", 0, 2);
    /* i = 0 */
    slvm_emit(vm, OP_CONST, c0);
    slvm_emit(vm, OP_STOREL, 0);
    /* while (i < N) { spawn worker; i = i + 1; } */
    int loop1 = slvm_here(vm);
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_CONST, cN);
    slvm_emit0(vm, OP_LT);
    int j1 = slvm_emit(vm, OP_JMPF, 0);
    slvm_emit(vm, OP_SPAWN, fworker);
    slvm_emit0(vm, OP_POP);             /* discard the thread id */
    slvm_emit(vm, OP_LOADL, 0);
    slvm_emit(vm, OP_CONST, c1);
    slvm_emit0(vm, OP_ADD);
    slvm_emit(vm, OP_STOREL, 0);
    slvm_emit(vm, OP_JMP, loop1);
    slvm_patch(vm, j1, slvm_here(vm));
    /* j = 0 */
    slvm_emit(vm, OP_CONST, c0);
    slvm_emit(vm, OP_STOREL, 1);
    /* while (j < N) { recv slot 1; j = j + 1; } */
    int loop2 = slvm_here(vm);
    slvm_emit(vm, OP_LOADL, 1);
    slvm_emit(vm, OP_CONST, cN);
    slvm_emit0(vm, OP_LT);
    int j2 = slvm_emit(vm, OP_JMPF, 0);
    slvm_emit(vm, OP_RECV, 1);
    slvm_emit0(vm, OP_POP);             /* discard received value */
    slvm_emit(vm, OP_LOADL, 1);
    slvm_emit(vm, OP_CONST, c1);
    slvm_emit0(vm, OP_ADD);
    slvm_emit(vm, OP_STOREL, 1);
    slvm_emit(vm, OP_JMP, loop2);
    slvm_patch(vm, j2, slvm_here(vm));
    /* joinall; print counter; ret 0 */
    slvm_emit0(vm, OP_JOINALL);
    slvm_emit(vm, OP_LOADG, g_counter);
    slvm_emit0(vm, OP_PRINT);
    slvm_emit(vm, OP_CONST, c0);
    slvm_emit0(vm, OP_RET);
    slvm_end_func(vm);

    slvm_set_entry(vm, fmain);

    SLResult r = slvm_run(vm);
    SLValue res = slvm_result(vm);
    (void)res;
    if (r == SLR_ERROR) {
        fprintf(stderr, "thread_smoke: error: %s\n", slvm_error(vm));
        slvm_free(vm);
        return 1;
    }
    printf("expected %d\n", N);
    slvm_free(vm);
    return 0;
}
