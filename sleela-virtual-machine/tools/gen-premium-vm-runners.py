#!/usr/bin/env python3
"""
sleela-virtual-machine/tools/gen-premium-vm-runners.py

Make every premium SLVM generation (001..011) a REAL, runnable virtual machine
whose SOURCE expresses that grade's DISTINGUISHING FEATURE.

Each level gets:
  - src/slvmNN_vm.c  — a VM runner that links the shared native Core engine
    (impl/build/libslcore.a) and runs a compiled .sleela artifact on it, after
    a grade-specific ADMISSION step that constructs a well-formed instance of
    that grade's feature and calls that grade's real validator. The runner's
    --selfcheck additionally proves a MALFORMED instance is rejected, so the
    admission is demonstrably real (not a constant `ok = 1`).
  - a Makefile that compiles+links that grade's feature source(s) alongside the
    runner and the Core archive.

Grade feature validators (each in NNN/src/*.c, header in NNN/include/):
  001 security supervisor  (slvm_security_*)        002 crypto provider (slvm2_crypto_*)
  003 isolation policy     (slvm3_isolation_*)      004 distributed exec (slvm4_*)
  005 policy snapshot      (slvm5_policy_*)          006 execution lease (slvm6_lease_*)
  007 manager health       (slvm7_health_*)          008 admission       (slvm8_admission_*)
  009 filesystem substrate (slvm9_filesystem_*)      010 verified storage (slvm10_storage_*)
  011 filesystem module    (slvm11_filesystem_module_*)

Run from anywhere:
  python3 sleela-virtual-machine/tools/gen-premium-vm-runners.py          # write
  python3 sleela-virtual-machine/tools/gen-premium-vm-runners.py --check   # dry run
"""
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # sleela-virtual-machine/

# Per-grade feature specification. Each entry carries:
#   dir, NN, codename, feature-oneliner,
#   headers:    list of feature headers to #include in the runner,
#   sources:    list of feature .c basenames to compile+link (relative to src/),
#   extra_inc:  extra -I dirs for compiling the feature sources (relative), [] usual,
#   admit_ok:   C statements building a VALID instance; must set `int rc0`,
#   admit_bad:  C statements building an INVALID instance; must set `int rc1`.
# Admission passes iff the valid instance is accepted (rc0==0) AND the invalid
# instance is rejected (rc1!=0) — proving the grade's validator really discriminates.
LEVELS = [
    dict(dir="001", nn="01", codename="Foundation",
         feature="dedicated VM foundation: broker, capabilities, security supervisor, OS boundary",
         headers=["slvm_security.h"], sources=["slvm_security.c"],
         admit_ok="""
    slvm_security_t s; slvm_security_init(&s);
    int rc0 = (slvm_security_observe_instruction(&s) == 1 &&
               slvm_security_observe_io(&s, SLVM_IO_FILE, 1024) == 1 &&
               slvm_security_state(&s) == SLVM_SECURITY_NORMAL) ? 0 : -1;
""",
         admit_bad="""
    /* A NULL supervisor must be reported DENIED and observation refused (0). */
    int rc1 = (slvm_security_state(NULL) == SLVM_SECURITY_DENIED &&
               slvm_security_observe_instruction(NULL) == 0) ? -1 : 0;
"""),
    dict(dir="002", nn="02", codename="Operator",
         feature="cryptographic and VM-link evolution over the Foundation",
         headers=["slvm2_crypto.h"], sources=["slvm2_crypto.c"],
         admit_ok="""
    static const slvm2_crypto_provider_api_t api = {
        (int(*)(slvm2_crypto_provider_t*,const void*,size_t,void*,size_t*))1,
        (int(*)(slvm2_crypto_provider_t*,const void*,size_t,const void*,size_t,const void*,size_t))1,
    };
    slvm2_crypto_provider_t p = { "sleela-ref-crypto", 1u, &api, 0 };
    int rc0 = slvm2_crypto_validate_provider(&p);
""",
         admit_bad="""
    slvm2_crypto_provider_t bad = { "no-api", 1u, 0, 0 };  /* missing api vtable */
    int rc1 = slvm2_crypto_validate_provider(&bad);
"""),
    dict(dir="003", nn="03", codename="Specialist",
         feature="verified execution: isolation, deterministic policy, supply-chain integrity",
         headers=["slvm3_isolation.h"], sources=["slvm3_isolation.c"],
         admit_ok="""
    slvm3_isolation_policy_t pol = { 512u*1024u*1024u, 256, 1u<<20, 1u<<24, 1000000, 5000, 1, 1, 0 };
    int rc0 = slvm3_isolation_validate(&pol);
    if (rc0 == 0) rc0 = slvm3_isolation_charge(&pol, 4096, 2048, 8192);
""",
         admit_bad="""
    slvm3_isolation_policy_t none = { 0,0,0,0,0,0, 0,0,0 };  /* bounds nothing */
    int rc1 = slvm3_isolation_validate(&none);
"""),
    dict(dir="004", nn="04", codename="Supervisor",
         feature="authenticated and distributed execution",
         headers=["slvm4_distributed.h"], sources=["slvm4_distributed.c"],
         admit_ok="""
    static const slvm4_node_t nodes[2] = {
        { 1,1,7, "spki://node-a", "att-a", 1,1,1 },
        { 2,1,7, "spki://node-b", "att-b", 1,1,1 },
    };
    slvm4_execution_t ex = { 42u, 7u, nodes, 2u, 2u };
    int rc0 = slvm4_execution_validate(&ex);
""",
         admit_bad="""
    static const slvm4_node_t bad_nodes[1] = { { 1,1,7, "spki://x", "att", 1,0,1 } }; /* not attested */
    slvm4_execution_t bad = { 42u, 7u, bad_nodes, 1u, 1u };
    int rc1 = slvm4_execution_validate(&bad);
"""),
    dict(dir="005", nn="05", codename="Manager",
         feature="reproducible, policy-verifiable, fault-tolerant execution",
         headers=["slvm5_policy.h"], sources=["slvm5_policy.c"],
         admit_ok="""
    slvm5_policy_snapshot_t snap = { 3u, "policy-hash", "mearvk", 1000u, 9000u, 1u };
    int rc0 = slvm5_policy_validate(&snap, 5000u);
""",
         admit_bad="""
    slvm5_policy_snapshot_t expired = { 3u, "policy-hash", "mearvk", 1000u, 2000u, 1u };
    int rc1 = slvm5_policy_validate(&expired, 5000u);  /* now past expiry */
"""),
    dict(dir="006", nn="06", codename="Director",
         feature="continuously verified, migratable execution: lineage, attestation, leases",
         headers=["slvm6_lease.h"], sources=["slvm6_lease.c"],
         admit_ok="""
    slvm6_lease_t lease = { 10u, 100u, 1000u, 9000u, 5u, "holder-x", 0x3u, 0u };
    int rc0 = slvm6_lease_validate(&lease, 5000u, 5u);
""",
         admit_bad="""
    slvm6_lease_t stale = { 10u, 100u, 1000u, 9000u, 4u, "holder-x", 0x3u, 0u };
    int rc1 = slvm6_lease_validate(&stale, 5000u, 5u);  /* revocation epoch superseded */
"""),
    dict(dir="007", nn="07", codename="Administrator",
         feature="hardened management and recovery: logging, health, checkpoints, resources",
         headers=["slvm7.h", "slvm7_health.h"], sources=["slvm7.c"],
         admit_ok="""
    slvm7_health_state_t h = { 0,0,0,0, 0u, 1u<<20, 0u, 0u, 0u };
    int rc0 = slvm7_health_validate(&h);
""",
         admit_bad="""
    slvm7_health_state_t stalled = { 0,0,0,0, 0u, 1u<<20, 1u, 0u, 0u };  /* stalled */
    int rc1 = slvm7_health_validate(&stalled);
"""),
    dict(dir="008", nn="08", codename="Executive",
         feature="supervised execution: admission, leases, transactions, audit",
         headers=["slvm8.h", "slvm8_admission.h"], sources=["slvm8.c"],
         admit_ok="""
    slvm8_admission_t a = { 1,1,1,1,1,1,1, 0 };
    int rc0 = slvm8_admission_evaluate(&a);
    if (rc0 == 0) rc0 = slvm8_admission_validate(&a);
""",
         admit_bad="""
    slvm8_admission_t deny = { 1,0,1,1,1,1,1, 0 };  /* policy_valid cleared */
    int rc1 = slvm8_admission_evaluate(&deny);
"""),
    dict(dir="009", nn="09", codename="Authority",
         feature="portable, capability-controlled filesystem substrate with OS adapters",
         headers=["slvm9.h", "slvm9_filesystem.h"], sources=["slvm9.c"],
         admit_ok="""
    slvm9_filesystem_t f = { 1u,1u, 1u<<30, 1u<<20, 4096u, "ext4", "uuid-9", "/mnt/9",
                             SLVM9_FS_READ|SLVM9_FS_WRITE, 1u, 0u, 1u, 1u };
    int rc0 = slvm9_filesystem_validate(&f);
""",
         admit_bad="""
    slvm9_filesystem_t unmounted = { 1u,1u, 1u<<30, 1u<<20, 4096u, "ext4", "uuid-9", "/mnt/9",
                                     SLVM9_FS_READ, 0u, 0u, 1u, 1u };  /* not mounted */
    int rc1 = slvm9_filesystem_validate(&unmounted);
"""),
    dict(dir="010", nn="10", codename="Principal",
         feature="verified storage execution and execution-assurance layer",
         headers=["slvm10.h", "slvm10_storage.h"], sources=["slvm10.c"],
         admit_ok="""
    slvm10_storage_t s = { 1u,1u, 1u<<30, 1u<<20, 4096u, "ext4", "disk-uuid",
                           SLVM10_READ|SLVM10_WRITE, 1u, 0u, 1u, 1u };
    int rc0 = slvm10_storage_validate(&s);
""",
         admit_bad="""
    slvm10_storage_t noint = { 1u,1u, 1u<<30, 1u<<20, 4096u, "ext4", "disk-uuid",
                               SLVM10_READ, 1u, 0u, 0u, 1u };  /* integrity cleared */
    int rc1 = slvm10_storage_validate(&noint);
"""),
    dict(dir="011", nn="11", codename="Sovereign",
         feature="filesystem-module host above the SLVM/9 and SLVM/10 storage contracts (TAC3)",
         headers=["slvm11_filesystem_module.h"], sources=["slvm11_filesystem_module.c"],
         admit_ok="""
    slvm11_filesystem_module_t tac3 = {
        SLVM11_FS_MODULE_TAC3_ID, SLVM11_FS_MODULE_SCHEMA, "TAC3", 1, 0, 4096,
        1, 1, 1, 1, 1 };
    int rc0 = (slvm11_filesystem_module_validate(&tac3) == 0 &&
               slvm11_filesystem_module_is_tac3(&tac3)) ? 0 : -1;
""",
         admit_bad="""
    slvm11_filesystem_module_t badschema = {
        SLVM11_FS_MODULE_TAC3_ID, "wrong.schema/v9", "TAC3", 1, 0, 4096,
        1, 1, 1, 1, 1 };
    int rc1 = slvm11_filesystem_module_validate(&badschema);  /* wrong schema */
"""),
]

# ---------------------------------------------------------------------------
# intXXX / uintXXX number-series feature, shared by EVERY premium VM generation.
# Each level gets slvmNN_intxxx_module.{h,c} (a Core-independent recognizer +
# admission for the fixed-width integer series int32..int16777216 /
# uint32..uint16777216), and the runner weaves the module's validation plus a
# live Core SLValue round-trip (SL_INT for <=64 bits, SL_INT128 for >=128) into
# the grade's admit/reject. So "support for the intXXX number series" is a
# property of ALL eleven generations, proven at each VM's self-check, on top of
# whatever distinguishing grade feature that VM already carries.
# ---------------------------------------------------------------------------
def intxxx_header(nn):
    return ('''/* ==========================================================================
 * slvm{nn}_intxxx_module.h  --  SLVM/{n} intXXX/uintXXX number support.
 *
 * First-class admission for SLeeLa's fixed-width integer number series -- the
 * signed  int32 .. int16777216  (syntax 2.7) and the unsigned uint32 ..
 * uint16777216 (syntax 2.8). A member is the prefix ("int"/"uint") followed by
 * a bit-width that must be a power of two in [2^5, 2^24]; twenty widths each.
 * This module lets the VM PROVE, at admission, that it genuinely carries the
 * series through the shared native Core value model, using the exact width ->
 * runtime-slot split the compiler uses:
 *
 *     width <= 64 bits  (int32/int64, uint32/uint64)  -> SL_INT    (int64_t)
 *     width >= 128 bits (int128.., uint128..)         -> SL_INT128 (__int128)
 *
 * Unsigned-ness is a compile-time (analyzer) discipline -- positive-only,
 * width-strict -- so at runtime a uint<width> shares the SL_INT / SL_INT128
 * tag with the signed series. This module validates the type-name series; the
 * runner proves the live Core round-trip. Shared by all premium generations.
 *
 * Generated by sleela-virtual-machine/tools/gen-premium-vm-runners.py.
 * Copyright (c) Max Rupplin - MEARVK LLC - 2026
 * ========================================================================== */
#ifndef SLVM{nn}_INTXXX_MODULE_H
#define SLVM{nn}_INTXXX_MODULE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {{
#endif

/* Series bounds, shared by the signed and unsigned families. Mirrors
 * impl/frontend/wide_integer.h (kMinimumWidth / kMaximumWidth) and
 * SLEELA.syntax Sections 24 (signed) and 25 (unsigned). */
#define SLVM{nn}_INTXXX_MIN_WIDTH        32u         /* 2^5  */
#define SLVM{nn}_INTXXX_MAX_WIDTH        16777216u   /* 2^24 */
#define SLVM{nn}_INTXXX_NATIVE_NARROW    64u         /* <= this width uses SL_INT */
#define SLVM{nn}_INTXXX_SERIES_COUNT     20u         /* widths per family (2^5..2^24) */

/* Runtime slot a width maps to, matching the Core SLType tags SL_INT / SL_INT128. */
typedef enum {{
    SLVM{nn}_SLOT_NONE  = 0,   /* not a valid series width                 */
    SLVM{nn}_SLOT_INT   = 1,   /* width <= 64  -> SL_INT    (int64_t slot)  */
    SLVM{nn}_SLOT_INT128 = 2   /* width >= 128 -> SL_INT128 (__int128 slot) */
}} slvm{nn}_intxxx_slot_t;

/* An admission descriptor: the series this VM claims to support. */
typedef struct {{
    uint32_t             min_width;     /* must equal SLVM{nn}_INTXXX_MIN_WIDTH   */
    uint32_t             max_width;     /* must equal SLVM{nn}_INTXXX_MAX_WIDTH   */
    uint32_t             narrow_width;  /* must equal SLVM{nn}_INTXXX_NATIVE_NARROW */
    uint32_t             series_count;  /* must equal SLVM{nn}_INTXXX_SERIES_COUNT */
    int                  has_signed;    /* 1: supports the int<width> family     */
    int                  has_unsigned;  /* 1: supports the uint<width> family    */
}} slvm{nn}_intxxx_module_t;

/* 1 iff `w` is a power of two within [MIN_WIDTH, MAX_WIDTH]. */
int slvm{nn}_intxxx_is_series_width(uint32_t w);
/* The runtime slot a width maps to (SLVM{nn}_SLOT_NONE for a non-series width). */
slvm{nn}_intxxx_slot_t slvm{nn}_intxxx_slot_for_width(uint32_t w);
/* Count of valid series widths in [MIN_WIDTH, MAX_WIDTH]; must be SERIES_COUNT. */
uint32_t slvm{nn}_intxxx_count_series_widths(void);
/* Validate an admission descriptor against the series rules. 0 if well-formed. */
int slvm{nn}_intxxx_module_validate(const slvm{nn}_intxxx_module_t* m);
/* 1 iff the descriptor is the full, canonical SLeeLa series (both families). */
int slvm{nn}_intxxx_module_is_full_series(const slvm{nn}_intxxx_module_t* m);

#ifdef __cplusplus
}}
#endif

#endif /* SLVM{nn}_INTXXX_MODULE_H */
''').format(nn=nn, n=int(nn))


def intxxx_source(nn):
    return ('''/* ==========================================================================
 * slvm{nn}_intxxx_module.c  --  SLVM/{n} intXXX/uintXXX number-series support.
 *
 * Recognizer + admission for the SLeeLa fixed-width integer number series: the
 * power-of-two width rule (2^5..2^24), the twenty-width count per family, and
 * the width -> runtime-slot split (SL_INT for <=64, SL_INT128 for >=128) that
 * matches the compiler. Same "admit a well-formed instance; reject a malformed
 * one" discipline every grade feature uses. Core-independent; the live Core
 * SLValue round-trip for each slot is proven in the VM runner's admission.
 *
 * Generated by sleela-virtual-machine/tools/gen-premium-vm-runners.py.
 * Copyright (c) Max Rupplin - MEARVK LLC - 2026
 * ========================================================================== */
#include "slvm{nn}_intxxx_module.h"

static int slvm{nn}_is_power_of_two(uint32_t w) {{
    return w != 0u && (w & (w - 1u)) == 0u;
}}

int slvm{nn}_intxxx_is_series_width(uint32_t w) {{
    if (!slvm{nn}_is_power_of_two(w)) return 0;
    if (w < SLVM{nn}_INTXXX_MIN_WIDTH) return 0;
    if (w > SLVM{nn}_INTXXX_MAX_WIDTH) return 0;
    return 1;
}}

slvm{nn}_intxxx_slot_t slvm{nn}_intxxx_slot_for_width(uint32_t w) {{
    if (!slvm{nn}_intxxx_is_series_width(w)) return SLVM{nn}_SLOT_NONE;
    return (w <= SLVM{nn}_INTXXX_NATIVE_NARROW) ? SLVM{nn}_SLOT_INT : SLVM{nn}_SLOT_INT128;
}}

uint32_t slvm{nn}_intxxx_count_series_widths(void) {{
    uint32_t count = 0u;
    uint32_t w;
    for (w = SLVM{nn}_INTXXX_MIN_WIDTH; w <= SLVM{nn}_INTXXX_MAX_WIDTH; w <<= 1) {{
        if (slvm{nn}_intxxx_is_series_width(w)) count++;
    }}
    return count;
}}

int slvm{nn}_intxxx_module_validate(const slvm{nn}_intxxx_module_t* m) {{
    if (m == 0) return -1;
    if (m->min_width    != SLVM{nn}_INTXXX_MIN_WIDTH)     return -1;
    if (m->max_width    != SLVM{nn}_INTXXX_MAX_WIDTH)     return -1;
    if (m->narrow_width != SLVM{nn}_INTXXX_NATIVE_NARROW) return -1;
    if (m->series_count != SLVM{nn}_INTXXX_SERIES_COUNT)  return -1;
    if (!m->has_signed && !m->has_unsigned)              return -1;
    if (slvm{nn}_intxxx_count_series_widths() != m->series_count) return -1;
    if (slvm{nn}_intxxx_slot_for_width(32u)       != SLVM{nn}_SLOT_INT)    return -1;
    if (slvm{nn}_intxxx_slot_for_width(64u)       != SLVM{nn}_SLOT_INT)    return -1;
    if (slvm{nn}_intxxx_slot_for_width(128u)      != SLVM{nn}_SLOT_INT128) return -1;
    if (slvm{nn}_intxxx_slot_for_width(16777216u) != SLVM{nn}_SLOT_INT128) return -1;
    if (slvm{nn}_intxxx_slot_for_width(16u)       != SLVM{nn}_SLOT_NONE)   return -1; /* below floor  */
    if (slvm{nn}_intxxx_slot_for_width(48u)       != SLVM{nn}_SLOT_NONE)   return -1; /* not pow2      */
    if (slvm{nn}_intxxx_slot_for_width(33554432u) != SLVM{nn}_SLOT_NONE)   return -1; /* above ceiling */
    return 0;
}}

int slvm{nn}_intxxx_module_is_full_series(const slvm{nn}_intxxx_module_t* m) {{
    if (m == 0) return 0;
    return (m->has_signed && m->has_unsigned &&
            m->min_width    == SLVM{nn}_INTXXX_MIN_WIDTH &&
            m->max_width    == SLVM{nn}_INTXXX_MAX_WIDTH &&
            m->series_count == SLVM{nn}_INTXXX_SERIES_COUNT) ? 1 : 0;
}}
''').format(nn=nn, n=int(nn))


# The admit/reject snippets the runner appends for the intXXX series (every VM).
def intxxx_admit_ok(nn):
    return ('''
    /* intXXX/uintXXX number series (every premium generation): the full,
     * canonical series -- both families, the 2^5..2^24 bounds, the 64-bit
     * narrow boundary, and the twenty widths -- plus a live proof that the
     * shared Core value model carries both runtime slots: a 64-bit value
     * through SL_INT and a value wider than 64 bits through SL_INT128. */
    slvm{nn}_intxxx_module_t intxxx = {{
        SLVM{nn}_INTXXX_MIN_WIDTH, SLVM{nn}_INTXXX_MAX_WIDTH,
        SLVM{nn}_INTXXX_NATIVE_NARROW, SLVM{nn}_INTXXX_SERIES_COUNT, 1, 1 }};
    SLValue intxxx_narrow = slval_int(9223372036854775807LL);    /* INT64_MAX */
    int intxxx_carry = (intxxx_narrow.type == SL_INT && intxxx_narrow.as.i == 9223372036854775807LL);
#if SLEELA_HAVE_INT128
    sl_int128_t intxxx_deep = ((sl_int128_t)1 << 100) + (sl_int128_t)1;
    SLValue intxxx_wide = slval_int128(intxxx_deep);
    intxxx_carry = intxxx_carry && intxxx_wide.type == SL_INT128 && intxxx_wide.as.i128 == intxxx_deep;
#else
    SLValue intxxx_wide = slval_int128((sl_int128_t)1234567890123456789LL);
    intxxx_carry = intxxx_carry && intxxx_wide.type == SL_INT128;
#endif
    int rc_intxxx = (slvm{nn}_intxxx_module_validate(&intxxx) == 0 &&
                     slvm{nn}_intxxx_module_is_full_series(&intxxx) && intxxx_carry) ? 0 : -1;
''').format(nn=nn)


def intxxx_admit_bad(nn):
    return ('''
    /* A malformed intXXX descriptor (wrong ceiling + bogus width count) must be
     * rejected, proving the number-series validator really discriminates. */
    slvm{nn}_intxxx_module_t badintxxx = {{
        SLVM{nn}_INTXXX_MIN_WIDTH, 33554432u /* 2^25: above ceiling */,
        SLVM{nn}_INTXXX_NATIVE_NARROW, 99u /* wrong count */, 1, 1 }};
    int rc_intxxx_bad = (slvm{nn}_intxxx_module_validate(&badintxxx) != 0) ? -1 : 0;
''').format(nn=nn)


def runner_source(L):
    nn = L["nn"]; n = int(nn)
    # Every VM also includes the shared intXXX/uintXXX number-series module.
    headers = list(L["headers"]) + ["slvm%s_intxxx_module.h" % nn]
    incs = "".join('#include "%s"\n' % h for h in headers)
    # Weave the intXXX series admission into the grade's admit/reject: the
    # grade feature AND the number series must both admit the valid instance
    # and reject the malformed one.
    admit_ok = L["admit_ok"] + intxxx_admit_ok(nn)
    admit_ok_return = "    return (rc0 == 0 && rc_intxxx == 0) ? 0 : -1;"
    admit_bad = L["admit_bad"] + intxxx_admit_bad(nn)
    admit_bad_return = "    return (rc1 != 0 && rc_intxxx_bad != 0) ? 0 : -1;"
    return """/* ==========================================================================
 * slvm{nn}_vm.c  --  SLVM/{n} "{codename}" runnable virtual machine.
 *
 * A REAL virtual machine: it links the shared native SLeeLa Core engine
 * (impl/core, archived as libslcore.a) and runs a compiled .sleela program
 * artifact on it. SLVM/{n}'s distinguishing contribution --
 *   {feature}
 * -- is expressed in SOURCE here: the admission step builds a well-formed
 * instance of this grade's own feature and calls this grade's real validator,
 * and (under --selfcheck) proves a malformed instance is REJECTED. A program
 * only runs on this VM once that grade-specific contract is admitted. This is
 * what distinguishes grade {n} from the other grades at the source level.
 *
 * Usage:
 *   slvm-{nn} <program.sleela-artifact>   run a compiled artifact on SLVM/{n}
 *   slvm-{nn} --selfcheck                 exercise the grade's validator only
 *
 * Core public API: impl/core/sleela_core.h. Build: this directory's Makefile
 * links ../../impl/build/libslcore.a and this grade's feature source(s).
 *
 * Generated by sleela-virtual-machine/tools/gen-premium-vm-runners.py.
 * Copyright (c) Max Rupplin - MEARVK LLC - 2026
 * ========================================================================== */
#include "sleela_core.h"
{incs}#include <stdio.h>
#include <string.h>

static const char* SLVM_CODENAME = "{codename}";
static const char* SLVM_FEATURE  = "{feature}";

/* Admit a well-formed instance of this grade's feature. Returns 0 if this
 * grade's real validator ACCEPTS the well-formed instance, -1 otherwise.
 * Named slvm{nn}_vm_admit (not slvm{nn}_admit) to avoid clashing with any
 * grade feature API that already defines an admit entry point. */
static int slvm{nn}_vm_admit(void) {{
{admit_ok}
{admit_ok_return}
}}

/* Prove the grade's validator is real: a deliberately malformed instance must
 * be REJECTED. Returns 0 if the bad instance is correctly rejected. */
static int slvm{nn}_vm_reject(void) {{
{admit_bad}
{admit_bad_return}
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
        if (slvm{nn}_vm_admit() != 0) {{
            fprintf(stderr, "slvm-{nn}: %s admission FAILED (valid instance rejected)\\n", SLVM_CODENAME);
            return 1;
        }}
        if (slvm{nn}_vm_reject() != 0) {{
            fprintf(stderr, "slvm-{nn}: %s validator is not discriminating (bad instance admitted)\\n", SLVM_CODENAME);
            return 1;
        }}
        printf("slvm-{nn}: %s VM self-check PASS "
               "(Core engine linked; grade validator admits valid + rejects invalid: %s)\\n",
               SLVM_CODENAME, SLVM_FEATURE);
        return 0;
    }}
    if (argc < 2) {{
        fprintf(stderr, "usage: slvm-{nn} <program.sleela-artifact> | --selfcheck\\n");
        return 2;
    }}
    if (slvm{nn}_vm_admit() != 0) {{
        fprintf(stderr, "slvm-{nn}: %s admission failed; boot denied\\n", SLVM_CODENAME);
        return 1;
    }}
    return slvm{nn}_run_artifact(argv[1]);
}}
""".format(nn=nn, n=n, codename=L["codename"],
           feature=L["feature"] + "; plus the intXXX/uintXXX fixed-width number series "
                                   "(int32..int16777216, uint32..uint16777216)",
           incs=incs, admit_ok=admit_ok, admit_ok_return=admit_ok_return,
           admit_bad=admit_bad, admit_bad_return=admit_bad_return)


def makefile_text(L):
    nn = L["nn"]; n = int(nn)
    # Every VM links its grade feature source(s) PLUS the shared intXXX module.
    sources = list(L["sources"]) + ["slvm%s_intxxx_module.c" % nn]
    objs = " ".join("$(BUILD)/%s.o" % os.path.splitext(s)[0] for s in sources)
    feat_rules = ""
    for s in sources:
        base = os.path.splitext(s)[0]
        feat_rules += ("$(BUILD)/%s.o: src/%s | $(BUILD)\n"
                       "\t$(CC) $(CFLAGS) -c $< -o $@\n") % (base, s)
    return """# =============================================================================
# SLVM/{n} "{codename}" -- build.
#
# Builds vm-build/slvm-{nn}: a REAL runnable virtual machine whose SOURCE
# expresses this grade's distinguishing feature. It links the shared native
# Core engine (impl/build/libslcore.a), this grade's own feature object(s), and
# the runner; admission exercises this grade's validator before boot:
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
# Shared native UI, Wi-Fi, and Ballum platform link dependencies.
VM_PLATFORM_LIBS_MK := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))../platform-libs.mk)
include $(VM_PLATFORM_LIBS_MK)
# The Core archive bundles the OS-abstraction backends + GC and contains C++
# objects (the idea-canvas layer), so the final link is driven by the C++
# compiler and needs the platform runtime libs + OpenMP.
CORE_LDLIBS ?= $(THREADFLAG) -fopenmp $(CORE_LIBS) $(CORE_UI_LIBS)

FEATURE_OBJS = {objs}
VM = $(BUILD)/slvm-{nn}

all: $(VM)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/slvm{nn}_vm.o: src/slvm{nn}_vm.c | $(BUILD)
	$(CC) $(CFLAGS) -I$(SLCORE_INC) -c $< -o $@
{feat_rules}
$(VM): $(BUILD)/slvm{nn}_vm.o $(FEATURE_OBJS) $(SLCORE_LIB) | $(BUILD)
	$(CXX) $(BUILD)/slvm{nn}_vm.o $(FEATURE_OBJS) $(SLCORE_LIB) -o $@ $(CORE_LDLIBS)

test: $(VM)
	$(VM) --selfcheck

check: test
	echo "SLVM/{n} runnable VM check: PASS"

clean:
	rm -rf $(BUILD)

.PHONY: all test check clean
""".format(nn=nn, n=n, codename=L["codename"], feature=L["feature"],
           objs=objs, feat_rules=feat_rules)


def main():
    check = "--check" in sys.argv
    wrote = []
    for L in LEVELS:
        d = L["dir"]; nn = L["nn"]
        ldir = os.path.join(ROOT, d)
        srcdir = os.path.join(ldir, "src")
        incdir = os.path.join(ldir, "include")
        os.makedirs(srcdir, exist_ok=True)
        os.makedirs(incdir, exist_ok=True)
        runner = os.path.join(srcdir, "slvm%s_vm.c" % nn)
        makefile = os.path.join(ldir, "Makefile")
        intxxx_h = os.path.join(incdir, "slvm%s_intxxx_module.h" % nn)
        intxxx_c = os.path.join(srcdir, "slvm%s_intxxx_module.c" % nn)
        if not check:
            with open(runner, "w") as f:
                f.write(runner_source(L))
            with open(makefile, "w") as f:
                f.write(makefile_text(L))
            with open(intxxx_h, "w") as f:
                f.write(intxxx_header(nn))
            with open(intxxx_c, "w") as f:
                f.write(intxxx_source(nn))
        wrote.append(d)
    print("%s %d premium VM runner(s) + Makefiles: %s" %
          ("would write" if check else "wrote", len(wrote), ", ".join(wrote)))


if __name__ == "__main__":
    main()
