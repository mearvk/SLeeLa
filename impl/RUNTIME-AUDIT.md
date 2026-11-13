# SLeeLa runtime audit

Revision 1.0, 2026-11-13. Scope: the execution core (`impl/core/sleela_core.{h,c}`), the collector
(`runtime/garbage_collector.{h,c}`) and the interpreter's allocation, exception and thread paths.
The contract is `impl/RUNTIME-SERVICES.md` and `lib/vm/RUNTIME-SERVICES.md`.

This page records what was **run or read**, what was **fixed**, and what is **open**. A row without
evidence says so. Tests: `bash impl/tests/runtime/pressure/run_pressure_tests.sh`
(`make -C impl test-runtime-pressure`, part of `make test`).

## 1. Verified by test (impl/tests/runtime/pressure)

| Area | What the test does | Result |
|---|---|---|
| Array churn | 20,000 short-lived arrays, with one 100-element array, one 30x30 grid and a 200-row jagged array held live | all contents intact after every collection |
| Struct churn with live structs | 3,000 live structs, then 30,000 short-lived ones, then mixed array and struct churn | **failed before the fix**, passes now (see 2.1) |
| `finally` that allocates | a method returns an array while its `finally` allocates 6,000 arrays; an exception path does the same | returned value intact (the saved return value is a rooted local) |
| Bounded tables fail closed | hold 5,000 live arrays, then 5,000 live structs | a clear error naming the limit; the program does not continue |
| Worker threads | two workers write disjoint arrays and allocate while the main thread allocates, repeated 5 times | deterministic result, no crash |

## 2. Fixed

### 2.1 Struct allocation did not retry after a full collection
`OP_NEWARRAY` runs one complete root-aware collection and retries when the live-array table is full.
`OP_NEWSTRUCT` and `OP_STRUCTUNPACK` did not, so a program holding a few thousand live objects failed
with "store exhausted" while most of the table was garbage. Both now go through `struct_alloc_gc`,
which does what `array_alloc_gc` does. A table whose slots are all reachable still fails.

### 2.2 Misleading allocation errors
An array allocation that failed because the managed heap was full (default 1 GiB) said "negative or
too-large size or store exhausted". The errors for arrays and structs now name the three real causes
(size, memory, table) and give the limits: 4096 live arrays, 4096 live structs, 32 bytes per element,
a 1 GiB managed heap unless `SLEELA_GC_LIMIT_BYTES` is set.

### 2.3 The build gate
When the source tree does not match the trusted manifest, `make` stops with a hash mismatch (it was
never silent). It now also says that nothing was rebuilt and that any existing `build/sleela` is the
previous build. Earlier sessions ran tests against that stale binary without noticing; the message
removes the trap. (A past claim that the failure was silent was wrong: output had been filtered.)

### 2.5 Negative array size
Java throws `NegativeArraySizeException`; the VM ended the program. The converter wraps each non-constant size in
`JavaChecks.size(n)` (`lib/java/JavaChecks.sleela`); `--unchecked-sizes` omits it. Test: `Bounds`.

### 2.6 Methods named like a built-in
A Java method called `read`, `write`, `accept`, `lock`, `send`, `listen`, `join`, ... was taken for the SLeeLa built-in
by an unqualified call ("read(handle) takes one argument"). The converter now qualifies such a call with its owner class
(static) or `this.` (instance). The list is `sleelactl/convert/builtin_names.h`, taken from the compiler's call table.
Not covered: a Java method named like a Munction verb (`connect`, `send`, `enable`, ...) called ON a receiver, which the
compiler also treats as the verb on every receiver.

### 2.4 Stale documents
`impl/RUNTIME-SERVICES.md` said no collector existed and that the ISA has 98 opcodes. Both are corrected.

## 3. Findings, not changed

| # | Finding | Evidence | Why it matters | Suggested action |
|---|---|---|---|---|
| 1 | **Every array element is 32 bytes** (`sizeof(SLValue)` is 32 because the value union carries a 128-bit integer) | measured with a C probe; 20,000,000 `int` elements need 640 MB, so a second one exceeds the 1 GiB heap | Java `byte[]`, `int[]`, `char[]` cost 8 to 32 times more than on a JVM; large arrays fail early | typed array storage (a VM change); until then document the cost |
| 2 | **Marking is quadratic in the object count**: `mark_internal` calls `live()`, a linear scan of every object, per mark; `push_unique` is linear too | read in `garbage_collector.c` (not profiled) | a full collection with N live objects does O(N squared) work; bounded today by the 4096 tables but grows with strings and externals | index objects by a flag or a hash; profile first |
| 3 | **The memory manager appears not to see VM arrays**: after a program that allocated a 640 MB array, its exit banner still reports `allocs=0  frees=0  peak=0 bytes` | observed in the exit counters only; not confirmed with a mid-run reading | the 2 GiB "hard limit" is not what stops an array; the 1 GiB GC limit is | account for array storage, or document that the GC limit is the operative one |
| 4 | **Catchable VM faults: partly in place.** Since 2026-11-13 (SLEELA.syntax 37.5, `lib/java/support/SLJavaFaults`) integer divide/modulo by zero, array index out of range and a failed checked cast are thrown inside a `try`. **Not covered:** null dereference, stack overflow, a negative array size (handled by the Java tier, 2.5 below), and every fault with no active handler (fatal, as an uncaught exception ends a Java program) | verified: `sleelactl/convert/tests/java/Bounds.java` (index reads, writes, compound stores, nested arrays) matches a JDK without any converter check | an earlier revision of this page listed this as wholly open; that was stale because the fix had landed the same day in another change and I had not re-read the spec | extend the fault kinds to null dereference (needs a typed null check at each dereference) |
| 5 | **The unlocked array fast path depends on `workers_cached`**, refreshed at a safepoint and forced at `SPAWN` | read; the threaded test passes 5 of 5 | a stale zero while a worker exists would let reads and writes race | argue it formally or take the lock when any thread was ever spawned; add a randomized thread test |
| 6 | **Unsynchronized shared updates lose writes** (two workers incrementing the same array: 399,956 of 400,000) | observed | the same as Java without `synchronized`; but the language has no atomic or monitor to fix it | document the guarantees: what `SPAWN`, `JOINALL`, locks and mailboxes order |
| 7 | **Lock ordering is not audited**: the core has `global_mtx`, `array_mtx`, `struct_mtx`, `thr_mtx` and others | not read end to end | a path that takes two locks in opposite orders deadlocks | build the lock-order graph; add a test |
| 8 | **Two string kinds** (plain UTF-8 and the VM-owned UTF-16 kind) | known | an unpaired surrogate or NUL is a runtime error for the plain kind only; each opcode that takes a string needs a stated rule for the other kind | a table of opcode by string kind |
| 9 | **The verifier is a Python script** found by searching upward from the current directory | observed when `SLEELA_HOME` is unset | an installed runtime without the repository cannot run | decide whether verification is part of an installed runtime |
| 10 | **Platform backends are only CI-covered where a workflow exists** (`native-portability`, `wifi-native`, `utf4088`, ...) | workflow list | Windows and macOS paths for sockets, terminal, threads | one table: backend by platform by workflow |
| 11 | `working-binaries/` report syntax 1.0 to 3.0 | read | users get "future edition" on 3.1 code | refresh and update the listed documents |

## 4. Decision recorded (2026-11-13)

Catchable runtime faults for Java: the owner chose to **keep the rule** (Java improvements live in the Java
tier, not in the compiler, VM or runtime) "for now". The VM mechanism of 37.5 is a generic fault hook, and
the Java-specific exceptions are made by `SLJavaFaults` in `lib/java`, so the rule holds. The converter adds only
what the hook does not cover: `JavaChecks.size` for a negative array size. Null dereference and stack overflow
stay fatal and are listed as open in `lib/java/CONVERSION.PRINCIPLES.md`.

## 5. How to extend this audit

Add a test under `impl/tests/runtime/pressure` that fails before a fix, fix the cause, move the row from
section 3 to section 2. A finding stays in section 3 until a test proves it fixed.
