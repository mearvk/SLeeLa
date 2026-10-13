/* ==========================================================================
 * cpu_runner_smoke.c -- verified smoke test for the native /lib/cpu fast path.
 *
 * Exercises the whole safety-first lifecycle of sleela_cpu_runner:
 *   1. measure an artifact's SHA-256 (the trusted-load gate),
 *   2. refuse to open without the dynamic-library capability (deny-by-default),
 *   3. open the in-process core WITH the capability granted,
 *   4. bind a CPU shape, load a tiny program (2 + 3 -> SYS_WRITE -> SYS_EXIT),
 *   5. run it in bounded, cancellable slices,
 *   6. verify the observable output and clean exit,
 *   7. tear down and confirm no native resources leak.
 *
 * The program is the same demo SLMachine.loadDemoProgram builds, encoded as
 * [opcode:8][rd:8][rs:8][imm:8] words exactly like SLInstruction.encode().
 * ========================================================================== */
#include "sleela_cpu_runner.h"
#include "sleela_platform.h"
#include <stdio.h>
#include <string.h>

/* ISA opcodes (mirror of SLInstructionSet). */
enum { MOV=2, ADD=10, SYSCALL=34, HALT=1 };

static int enc(int op, int rd, int rs, int imm) {
    return ((op & 0xff) << 24) | ((rd & 0xff) << 16) | ((rs & 0xff) << 8) | (imm & 0xff);
}

static int failures = 0;
static void check(const char *what, int cond) {
    printf("  [%s] %s\n", cond ? "ok" : "FAIL", what);
    if (!cond) failures++;
}

int main(void) {
    printf("=== SLeeLa native CPU runner smoke ===\n");

    /* 1. SHA-256 of a known file (this source) is a 64-char lowercase hex. */
    const char *digest = sleela_cpu_runner_sha256("core/cpu_runner_smoke.c");
    if (!digest[0]) digest = sleela_cpu_runner_sha256("impl/core/cpu_runner_smoke.c");
    check("sha256 produced a 64-char digest", (int)strlen(digest) == 64);

    /* 2. Deny-by-default: opening with an empty capability mask must be refused. */
    SLCpuRunnerHandle denied = sleela_cpu_runner_open(SL_PLATFORM_LINUX, "@in-process",
                                                      "sleela_cpu_entry", 1, 1, 0u);
    check("open refused without dynamic-library capability", denied <= 0);

    /* 3. Grant exactly the dynamic-library capability (bit = SL_CAP_DYNAMIC_LIBRARY). */
    uint32_t cap = (1u << SL_CAP_DYNAMIC_LIBRARY) | (1u << SL_CAP_MEMORY);
    SLCpuRunnerHandle lib = sleela_cpu_runner_open(SL_PLATFORM_LINUX, "@in-process",
                                                   "sleela_cpu_entry", 1, 1, cap);
    check("open accepted with granted capability", lib > 0);

    SLCpuRunnerHandle run = sleela_cpu_runner_create(lib, 1000 /* ms deadline */);
    check("runner created", run > 0);

    check("bind cpu (4 cores, 32-bit, 3200 MHz)",
          sleela_cpu_runner_bind_cpu(run, 4, 32, 3200) == 0);

    /* 4. The demo program at load address 0 (declared core 0 = assume nothing). */
    int prog[] = {
        enc(MOV, 1, 0, 2),   /* r1 = 2            */
        enc(MOV, 2, 0, 3),   /* r2 = 3            */
        enc(ADD, 1, 2, 0),   /* r1 = r1 + r2 = 5  */
        enc(MOV, 2, 0, 65),  /* r2 = 'A'          */
        enc(SYSCALL, 0, 0, 0),/* SYS_WRITE: out r2 */
        enc(SYSCALL, 0, 0, 4),/* SYS_EXIT          */
        enc(HALT, 0, 0, 0)
    };
    int n = (int)(sizeof(prog) / sizeof(prog[0]));
    check("load program image", sleela_cpu_runner_load_program(run, 0, n, 0, 0) == 0);
    for (int i = 0; i < n; i++)
        if (sleela_cpu_runner_poke(run, i, prog[i]) != 0) { check("poke word", 0); break; }

    /* 5. Run in bounded slices (as SLNativeRunner.run does). */
    int total = 0, slice;
    while ((slice = sleela_cpu_runner_run(run, 2)) > 0) {
        total += slice;
        if (sleela_cpu_runner_halted(run) == 1) break;
        if (total > 1000) break; /* safety bound */
    }
    check("program halted", sleela_cpu_runner_halted(run) == 1);
    check("clean exit status (0)", sleela_cpu_runner_exit_status(run) == 0);

    /* 6. The observable output must be 'A' (65). */
    check("printed 'A' (65) via SYS_WRITE", sleela_cpu_runner_last_output(run) == 65);

    /* out-of-range poke is rejected (fail-closed). */
    check("out-of-range poke rejected", sleela_cpu_runner_poke(run, -1, 0) == -1);

    /* 7. Teardown is ordered and idempotent; no resources leak. */
    sleela_cpu_runner_destroy(run);
    sleela_cpu_runner_destroy(run);   /* idempotent: safe twice */
    sleela_cpu_runner_close(lib);
    sleela_cpu_runner_close(lib);
    check("no live runners after teardown", sleela_cpu_runner_live_runners() == 0);
    check("no live libraries after teardown", sleela_cpu_runner_live_libraries() == 0);

    printf("=== %s (%d failure%s) ===\n", failures ? "FAILED" : "PASSED",
           failures, failures == 1 ? "" : "s");
    return failures ? 1 : 0;
}
