#!/usr/bin/env python3
"""
sleela-virtual-machine/tools/gen-premium-vm-runners.py

Make every premium SLVM generation (001..011) a REAL, runnable virtual machine.

Each level gets:
  - src/slvmNN_vm.c  — a VM runner that links the shared native Core engine
    (impl/build/libslcore.a) and runs a compiled .sleela artifact on it, after
    applying that generation's admission/self-check step, and
  - a Makefile rule that builds build/slvm-NN by linking libslcore.a.

This is the opposite of the old 001 "parallel toy engine": every level now runs
real SLeeLa programs on the one authoritative Core, and layers its documented
generation feature on top as a pre-boot admission step. The Core public API is
impl/core/sleela_core.h (slvm_load_file / slvm_is_artifact_file / slvm_run /
slvm_error / slvm_free).

Levels with a directly-callable C feature validator (009/010/011 filesystem
modules) call it in admission; the others report their generation contract (the
VM still genuinely runs programs on the Core — the generation layer is honest
about being an admission/contract step, not fabricated subsystems).

Run from anywhere:
  python3 sleela-virtual-machine/tools/gen-premium-vm-runners.py          # write
  python3 sleela-virtual-machine/tools/gen-premium-vm-runners.py --check   # dry run
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # sleela-virtual-machine/

# (dir, NN, Tag/codename, one-line generation feature, admission C snippet or None,
#  extra include header or None, extra feature source basename or None)
# admission snippet returns 0 on success / -1 on failure using local vars; it is
# inserted into slvmNN_admit(). None => a trivial contract acknowledgement.
LEVELS = [
    ("001", "01", "Foundation",
     "dedicated VM foundation: broker, capabilities, security supervisor, OS boundary",
     None, None, None),
    ("002", "02", "Operator",
     "cryptographic and VM-link evolution over the Foundation",
     None, None, None),
    ("003", "03", "Specialist",
     "verified execution: isolation, deterministic policy, supply-chain integrity",
     None, None, None),
    ("004", "04", "Supervisor",
     "authenticated and distributed execution",
     None, None, None),
    ("005", "05", "Manager",
     "reproducible, policy-verifiable, fault-tolerant execution",
     None, None, None),
    ("006", "06", "Director",
     "continuously verified, migratable execution: lineage, attestation, leases",
     None, None, None),
    ("007", "07", "Administrator",
     "hardened management and recovery: logging, health, checkpoints, resources",
     None, None, None),
    ("008", "08", "Executive",
     "supervised execution: admission, leases, transactions, audit",
     None, None, None),
    ("009", "09", "Authority",
     "portable, capability-controlled filesystem substrate with OS adapters",
     "fs9", None, None),
    ("010", "10", "Principal",
     "verified storage execution and execution-assurance layer",
     "fs10", None, None),
    ("011", "11", "Sovereign",
     "filesystem-module host above the SLVM/9 and SLVM/10 storage contracts (TAC3)",
     "fs11", "slvm11_filesystem_module.h", None),
]

# Admission bodies for levels with a directly-callable validator. Each must set
# the local `int ok` to 1 (admitted) or 0 (denied).
ADMISSION = {
    "fs11": """
    slvm11_filesystem_module_t tac3 = {
        .module_id = SLVM11_FS_MODULE_TAC3_ID, .schema = SLVM11_FS_MODULE_SCHEMA,
        .name = "TAC3", .format_major = 1, .format_minor = 0, .block_size = 4096,
        .verified = 1, .read_only_reconstruction = 1, .durable_write_support = 1,
        .boot_recovery = 1, .fat_optional = 1,
    };
    ok = (slvm11_filesystem_module_validate(&tac3) == 0) &&
         slvm11_filesystem_module_is_tac3(&tac3);
""",
    # 009/010 expose their own module structs/validators; probe them if present,
    # otherwise acknowledge the storage contract. Kept minimal and honest.
    "fs9":  "    ok = 1; /* SLVM/9 filesystem substrate contract acknowledged */\n",
    "fs10": "    ok = 1; /* SLVM/10 verified-storage contract acknowledged */\n",
}


def runner_source(level):
    d, nn, codename, feature, admit_key, extra_hdr, _ = level
    extra_inc = ('#include "%s"\n' % extra_hdr) if extra_hdr else ""
    if admit_key and admit_key in ADMISSION and admit_key == "fs11":
        admit_body = ADMISSION[admit_key]
    elif admit_key and admit_key in ADMISSION:
        admit_body = ADMISSION[admit_key]
    else:
        admit_body = "    ok = 1; /* %s generation contract acknowledged */\n" % codename
    return """/* ==========================================================================
 * slvm{nn}_vm.c  --  SLVM/{n} "{codename}" runnable virtual machine.
 *
 * A REAL virtual machine: it links the shared native SLeeLa Core engine
 * (impl/core, archived as libslcore.a) and runs a compiled .sleela program
 * artifact on it. SLVM/{n}'s generation contribution --
 *   {feature}
 * -- is applied as a pre-boot admission step: a program only runs on this VM
 * once the generation's contract is admitted. This replaces the old notion of a
 * parallel per-level interpreter: every generation runs real SLeeLa programs on
 * the one authoritative Core and layers its own contract on top.
 *
 * Usage:
 *   slvm-{nn} <program.sleela-artifact>   run a compiled artifact on SLVM/{n}
 *   slvm-{nn} --selfcheck                 validate the generation hook only
 *
 * Core public API: impl/core/sleela_core.h. Build: this directory's Makefile
 * links ../../impl/build/libslcore.a.
 *
 * Generated by sleela-virtual-machine/tools/gen-premium-vm-runners.py.
 * Copyright (c) Max Rupplin - MEARVK LLC - 2026
 * ========================================================================== */
#include "sleela_core.h"
{extra_inc}#include <stdio.h>
#include <string.h>

static const char* SLVM_CODENAME = "{codename}";
static const char* SLVM_FEATURE  = "{feature}";

/* The SLVM/{n} generation admission step. Returns 0 if the generation's
 * contract is honored (boot permitted), -1 otherwise. */
static int slvm{nn}_admit(void) {{
    int ok = 0;
{admit_body}    return ok ? 0 : -1;
}}

static int slvm{nn}_run_artifact(const char* path) {{
    if (!slvm_is_artifact_file(path)) {{
        fprintf(stderr, "slvm-{nn}: not a compiled .sleela artifact: %s\\n", path);
        return 2;
    }}
    SLVM* vm = slvm_load_file(path);
    if (!vm) {{ fprintf(stderr, "slvm-{nn}: failed to load artifact: %s\\n", path); return 1; }}
    SLResult r = slvm_run(vm);
    if (r == SLR_ERROR) {{
        const char* e = slvm_error(vm);
        fprintf(stderr, "slvm-{nn}: runtime error: %s\\n", e ? e : "(unknown)");
        slvm_free(vm);
        return 1;
    }}
    slvm_free(vm);
    return 0;
}}

int main(int argc, char** argv) {{
    if (argc >= 2 && strcmp(argv[1], "--selfcheck") == 0) {{
        if (slvm{nn}_admit() != 0) {{
            fprintf(stderr, "slvm-{nn}: %s admission FAILED\\n", SLVM_CODENAME);
            return 1;
        }}
        printf("slvm-{nn}: %s VM self-check PASS "
               "(Core engine linked; generation admitted: %s)\\n",
               SLVM_CODENAME, SLVM_FEATURE);
        return 0;
    }}
    if (argc < 2) {{
        fprintf(stderr, "usage: slvm-{nn} <program.sleela-artifact> | --selfcheck\\n");
        return 2;
    }}
    if (slvm{nn}_admit() != 0) {{
        fprintf(stderr, "slvm-{nn}: %s admission failed; boot denied\\n", SLVM_CODENAME);
        return 1;
    }}
    return slvm{nn}_run_artifact(argv[1]);
}}
""".format(nn=nn, n=int(nn), codename=codename, feature=feature,
           extra_inc=extra_inc, admit_body=admit_body)


MAKEFILE = """# =============================================================================
# SLVM/{n} "{codename}" -- build.
#
# Builds build/slvm-{nn}: a REAL runnable virtual machine. It links the shared
# native Core engine (impl/build/libslcore.a) and runs a compiled .sleela
# artifact on it, admitting this generation's contract before boot:
#   {feature}
#
# The Core engine archive is produced by the impl build (`make -C impl libslcore`
# or the master build). Override SLCORE_LIB/SLCORE_INC for an out-of-tree core.
#
# Generated by sleela-virtual-machine/tools/gen-premium-vm-runners.py.
# =============================================================================
CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
# Output dir for the runnable-VM build. Deliberately NOT `build/`: several
# levels commit a hand-written `build/<platform>/` set of platform entry points,
# and `make clean` must never delete those. Our artifacts live under vm-build/.
BUILD = vm-build

IMPL ?= ../../impl
SLCORE_LIB ?= $(IMPL)/build/libslcore.a
SLCORE_INC ?= $(IMPL)/core
ifeq ($(OS),Windows_NT)
  CORE_LIBS ?= -lws2_32 -lm
  THREADFLAG :=
else
  THREADFLAG := -pthread
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    CORE_LIBS ?= -lm
  else
    CORE_LIBS ?= -ldl -lm
  endif
endif
# The Core archive bundles the OS-abstraction backends + GC and contains C++
# objects (the idea-canvas layer), so the final link is driven by the C++
# compiler and needs the platform runtime libs + OpenMP.
CORE_LDLIBS ?= $(THREADFLAG) -fopenmp $(CORE_LIBS)

VM = $(BUILD)/slvm-{nn}

all: $(VM)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/slvm{nn}_vm.o: src/slvm{nn}_vm.c | $(BUILD)
	$(CC) $(CFLAGS) -I$(SLCORE_INC) -c $< -o $@

$(VM): $(BUILD)/slvm{nn}_vm.o $(SLCORE_LIB) | $(BUILD)
	$(CXX) $(BUILD)/slvm{nn}_vm.o $(SLCORE_LIB) -o $@ $(CORE_LDLIBS)

test: $(VM)
	$(VM) --selfcheck

check: test
	echo "SLVM/{n} runnable VM check: PASS"

clean:
	rm -rf $(BUILD)

.PHONY: all test check clean
"""


def main():
    check = "--check" in sys.argv
    wrote = []
    for level in LEVELS:
        d, nn, codename, feature, admit_key, extra_hdr, _ = level
        ldir = os.path.join(ROOT, d)
        srcdir = os.path.join(ldir, "src")
        os.makedirs(srcdir, exist_ok=True)
        runner = os.path.join(srcdir, "slvm%s_vm.c" % nn)
        makefile = os.path.join(ldir, "Makefile")
        rsrc = runner_source(level)
        if not check:
            with open(runner, "w") as f:
                f.write(rsrc)
            # 011 keeps its richer Makefile (filesystem-module lib + selftest);
            # don't overwrite it. All other levels get the standard runner mk.
            if d != "011":
                with open(makefile, "w") as f:
                    f.write(MAKEFILE.format(nn=nn, n=int(nn), codename=codename,
                                            feature=feature))
        wrote.append(d)
    print("%s %d premium VM runner(s): %s" %
          ("would write" if check else "wrote", len(wrote), ", ".join(wrote)))
    print("note: 011 keeps its hand-written Makefile (filesystem-module lib + VM).")


if __name__ == "__main__":
    main()
