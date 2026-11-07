# NATIVE.ADD — getting the Pi computation closer to native C

**Goal.** Make SLeeLa's Pi computation much faster — ideally within a small
constant factor of a native C program — and identify exactly *what* to add to
the VM to get there.

**Scope.** This is an analysis + roadmap, grounded in the current VM source
(`impl/core/sleela_core.c`, `runtime/garbage_collector.c`) and the two existing
generators (`PiGenerator.sleela` = Rabinowitz–Wagon spigot, O(d²) interpreted;
`PiMachin.sleela` = Machin's formula over the native big-integer kernel, O(d²)
with a smaller constant). Numbers below were measured with the source-built
compiler (`impl/build/sleela`) on the development machine; treat them as
order-of-magnitude, not benchmarks.

---

## Status — implemented (this session)

The roadmap below was partly executed. Shipped and verified:

- **Three new native big-integer opcodes** (A1 primitives), all base-1e9
  little-endian, mirroring the existing `bigmul`/`bigadd`/`bigdivmod` plumbing:
  - `bigsub(a, b, result)` → `OP_BIGSUB` (160) — big − big (a ≥ b).
  - `bigdiv(num, den, quoOut, remOut)` → `OP_BIGDIV` (161) — big ÷ big
    (schoolbook long division; quotient + remainder).
  - `bigisqrt(a, result)` → `OP_BIGISQRT` (162) — floor(√a) by Newton.
- **`PiChudnovsky.sleela`** — binary-splitting Chudnovsky over the native
  kernel, with signed big integers built in SLeeLa on top of
  `bigmul`/`bigadd`/`bigsub`. Verified **digit-for-digit** against an
  independent reference at 50, 1,000, and 10,000 digits.
- **GC allocation robustness** — `OP_NEWARRAY` now runs a full root-aware
  collection and retries once when the live-array table is full, instead of
  failing. Allocation-heavy code (the Chudnovsky recursion churns many
  short-lived limb arrays) no longer trips `array allocation failed`.
- **ISA checker fix** — `tools/verify-osls-isa-extension.py` no longer
  hard-codes a 156-opcode total; it verifies the frozen 155 baseline + OSLS at
  155 + native/source agreement over the whole append-only list (it was already
  failing on master, which had grown to 160).

Measured (source-built `sleela`, same machine):

| Digits | Spigot | Machin | Chudnovsky |
|---:|---:|---:|---:|
| 1,000 | ~2 s | <1 s | ~2 s |
| 10,000 | ~51 s | ~3 s | ~9 s |
| 50,000 | — | — | ~194 s |

**Honest status of the exponent.** The binary-splitting *recursion* is
quasi-linear (all +, −, × on the NTT multiply), but the single **final
division and √** still use the schoolbook O(n²) kernel, and that term now
dominates (Chudnovsky 10k→50k is ~21× for 5× digits ≈ exponent 1.9). So the
machine today is "correct Chudnovsky with a quasi-linear core but an O(d²)
tail." The remaining A1 work — a **Newton–Raphson reciprocal** `bigdiv` and
Newton `bigisqrt` that ride the NTT multiply — is what removes the tail and
finally delivers the full ~O(d·log²d) regime. It is the single highest-value
follow-up; everything needed for it (fast `bigmul`, `bigsub`) is now in place.

## Reassessment (post-v3.0) — the speed equation re-measured

The original roadmap below was written *before* Chudnovsky existed and ranked
"add binary splitting" as the top win. That is now done, so the bottleneck has
moved. Here is the current speed equation, from a **phase-timed** Chudnovsky run
(`timeMonotonicNanos()` around each stage):

| Phase | 10,000 digits | 50,000 digits | Scaling (5× digits) |
|---|---:|---:|---|
| Binary-splitting recursion (`bs`) | 72 ms (1%) | 399 ms (0.2%) | **5.5× → ~linear (done)** |
| Final `bigisqrt` | **6,405 ms (94%)** | **170,025 ms (94%)** | **26.5× → ~O(n²·log n)** |
| Final `bigdiv` | 368 ms (5%) | 10,495 ms (6%) | 28.5× → ~O(n²) |

So the total time is ≈ `isqrt + div`, and **`bigisqrt` alone is ~94%**. The
recursion — the thing v3.0 set out to make fast — is already negligible and
scales linearly. The whole speed equation now reduces to **two schoolbook
kernels in the final scaling**, and one of them dominates by ~17×.

Why `bigisqrt` is so much worse than a single divide: its Newton loop runs
~log₂(limbs) iterations and **each iteration calls the schoolbook O(n²)
`bigdiv`**, so it is effectively O(n²·log n) — strictly worse than the single
O(n²) final divide. That is the 94%.

### Re-ranked improvements — by order of magnitude, then by size

| # | Improvement | Magnitude | Size |
|---|---|---|---|
| **R1** | **Newton-reciprocal `bigisqrt`** — compute 1/√ (and the √ correction) with Newton iterations that ride the NTT `bigmul`, instead of a schoolbook `bigdiv` per iteration. Kills the 94% term. | **~50–100×** on the dominant phase; turns 50k from ~194 s toward ~15 s | **Medium** (one C kernel; isqrt already exists to diff against) |
| **R2** | **Newton-reciprocal `bigdiv`** — replace the schoolbook final division with `num · (1/den)` via a Newton reciprocal on the NTT multiply. Removes the remaining ~5–6% O(n²) tail. | **~10–30×** on the divide phase (now ~2nd); once R1 lands this becomes the new top term | **Medium** (shares the reciprocal core with R1) |
| **R3** | **Reduce the final working precision / scale** — the `bigisqrt` operand is `10005·S²` (≈ 2× the digit width); computing √ at exactly the needed precision instead of the squared scale halves the isqrt input size. Pure SLeeLa-side change in `PiChudnovsky.sleela`. | **~2–4×** on isqrt until R1 lands (constant factor) | **Small** (generator-only; no VM change) |
| **R4** | **Trim bs() allocation churn** — the recursion is already ~linear in time but allocates many short-lived limb arrays (it leans on the OP_NEWARRAY GC-retry). Reusing buffers / in-place ops trims the 1% and the GC pressure. | **~1.1–1.3×** overall (bs is only 1%) | **Small–Medium** |
| **R5** | Interpreter/representation wins from the original roadmap (direct-threaded dispatch, unboxed arrays) | **1.2–3×** broad, but near-irrelevant to Chudnovsky (99% of its time is in native kernels, not the interpreter) | Medium–Large |

**The one thing that matters now: R1 (Newton-reciprocal `bigisqrt`).** It targets
94% of the runtime; nothing else is close. R2 is the natural companion (shared
reciprocal core) and becomes the top term once R1 lands. R3 is a cheap,
VM-free stopgap that helps today. R4/R5 are polish — note that for *this*
workload the interpreter speedups (R5) barely register, because post-v3.0
Chudnovsky spends ~99% of its time inside native C kernels, not in bytecode.

**Recommended order:** R3 (small, immediate, no VM change) → R1 (the big one) →
R2 (finishes the tail) → R4. After R1+R2 the final scaling joins the
quasi-linear recursion and the whole generator should be ~O(d·log²d) end to end
— the "millions of digits in minutes" regime.

### Outcome so far (implemented)

| # | Status | Result |
|---|---|---|
| **R1** | **Landed** (different mechanism than first planned) | Instead of a Newton-reciprocal isqrt, the fast win came from a **tight seed**: seed `bigisqrt` from the top two limbs (64-bit floor-sqrt, +1 for a strict overestimate; ×31623 for an odd low-limb count) so Newton descends to `floor(sqrt)` in ~2-3 iterations instead of ~`log2(BASE^(na/2))`. Verified exact. **10k: 10 s → 4 s; 50k: 194 s → 87 s (~2.2–2.5×).** Phase split at 50k is now bs 0.4 s / isqrt 62.6 s (was 170 s) / div 10.5 s. |
| **R2** | **Scaffolded, gated off** | A Newton-reciprocal `sl_bigdiv` is in the tree but did not converge to an exact quotient in base 1e9 (it fell back to schoolbook 100% of the time), so it is gated to dispatch straight to the exact schoolbook divide. An exactness-or-fallback guard (verify `0≤rem<den`, else schoolbook) makes it provably safe; the big-divide speed win is **deferred** until the reciprocal is corrected. |
| **R3** | **Reassessed as low-value here** | The isqrt operand is inherently `10005·S²` (~2W limbs) to yield a W-limb `√10005·S`; there is no cheap way to halve it without changing the result precision. Superseded in practice by R1's iteration cut. |
| **R4** | **Not needed yet** | bs is now ~0.5% of runtime; trimming its churn is noise until R2 lands and isqrt/div stop dominating. |

**What's left for the full quasi-linear regime:** a *correct* Newton-reciprocal
`bigdiv` (R2). isqrt is still ~85% of a 50k run because each of its (now few)
iterations still calls the schoolbook O(n²) divide; once that divide is
quasi-linear, isqrt and the final scaling both drop to ~O(n log n) and the whole
generator reaches the "millions in minutes" target. The reciprocal kernel and
its safe fallback are already wired in — only the convergence math remains.

## TL;DR — ranked by impact, then by size

> Historical note: the ranking below predates v3.0 and is kept for the roadmap
> rationale. The current, measured ranking is the Reassessment section above.

Two orderings are requested. First by **order of magnitude of speedup**
(biggest win first); within each tier, smallest implementation first.

| # | Improvement | Impact (order of magnitude) | Size |
|---|---|---|---|
| **A1** | Native `bigdivmod` by a *wide* divisor **+** native **big ÷ big** divide and **big subtract** → enables **binary-splitting Chudnovsky** | **1000×+** (changes the *exponent*: O(d²) → ~O(d·log²d)) | Large |
| **A2** | A whole-series **native Pi kernel** opcode (`OP_PI_SPIGOT` / `OP_PI_MACHIN`) — move the entire hot loop into C | **100–1000×** (full native speed, one VM call) | Medium–Large |
| **B1** | Remove the **per-call marshalling tax** on `bigmul`/`bigadd`/`bigdivmod` (the `malloc`+snapshot+write-back around every call) by operating on the VM limb buffer in place | **2–5×** on `PiMachin` | Medium |
| **B2** | Add native `bigadd`-in-place / a native **limb-array copy / fill / shift** opcode so the O(d) per-term interpreted array copy disappears | **2–4×** on `PiMachin` | Small–Medium |
| **C1** | Faster interpreter dispatch (computed-goto / direct-threaded) | **1.2–2×** everywhere | Medium |
| **C2** | Unbox small-int arrays (store `int64` arrays flat, not 32-byte `SLValue`) | **1.5–3×** on array-heavy loops | Large |
| **C3** | Micro-tuning the existing fast paths (hoist bounds, widen safepoint stride) | **1.05–1.3×** | Small |

The honest headline: **only tier A changes the asymptotic class.** Everything in
B and C is constant-factor. If the real target is "millions of digits in
minutes," **A1 is the one that matters**; the rest is polish.

---

## Where the time actually goes

### The interpreter loop (`impl/core/sleela_core.c`, `run_thread`, ~line 1459)

```c
for (;;) {
    if (t->underflow) TERR("stack underflow");
    if (ip < 0 || ip >= vm->codelen) TERR("ip out of range");
    SLInstr in = vm->code[ip++];
    if (++t->safepoint_tick >= SL_GC_SAFEPOINT_STRIDE) { ... slvm_gc_safepoint ...; }
    switch (in.op) { ... }
}
```

Plain `switch` dispatch, one instruction fetched per iteration, plus a batched
GC-safepoint tick. Every arithmetic op is: fetch → bounds-check `ip` → switch →
a couple of stack `POP`s → the work → `PUSH`. For a tight numeric loop this
fixed per-opcode overhead dwarfs the actual arithmetic.

### Array element access (`OP_ARRGET` ~2258, `OP_ARRSET` ~2281)

There is already a **lock-free single-threaded fast path** (good — no mutex per
element when there are no workers):

```c
if (t->workers_cached == 0) {
    SLArrayInstance* ai=&vm->arrays[av.as.h];
    if(idxv.as.i<0||idxv.as.i>=ai->len) TERR("array index out of range");
    PUSH(ai->elems[idxv.as.i]);
}
```

But each `a[i]` still costs: two `POP`s, two type checks, `array_valid_handle`,
a bounds check, and a move of a **32-byte boxed `SLValue`**
(`typedef struct { SLType type; union { int64_t i; double d; ... sl_int128_t i128; } as; } SLValue;`).
Native C would be a single `int64_t` load from a flat array — roughly an order
of magnitude fewer operations per element.

### The big-integer kernel call overhead (`OP_BIGMUL`/`OP_BIGADD`/`OP_BIGDIVMOD`, ~2460–2640)

The kernels themselves are fast native C (`sl_bigmul` is schoolbook → Karatsuba
→ NTT; `sl_bigadd`, `sl_bigdivmod_small`). But **every call pays an O(n)
marshalling tax**: it `malloc`s `uint32_t` buffers, runs an O(n) loop copying the
VM's boxed `int[]` into them, calls the kernel, runs an O(n) loop writing the
result back into the VM array, then `free`s. For `PiMachin` that tax is paid on
*every one of ~d series terms*, so it is a real fraction of the runtime even
though each individual copy is "just" O(n).

### The missing operations (confirmed)

The VM exposes exactly three big-integer builtins — registered in
`impl/frontend/semantic.cpp` (`"bigmul","bigadd","bigdivmod"`) and lowered in
`impl/frontend/compiler.cpp`:

- `bigmul(a,b,result)` — big × big (NTT-fast).
- `bigadd(a,b,result)` — big + big.
- `bigdivmod(dividend, divisor, result)` — big ÷ **small** (divisor < 10⁹), returns remainder.

There is **no** native big ÷ big divide, **no** big subtract, and **no** big
square root. That absence is *the* reason `PiMachin` is still O(d²) and why full
Chudnovsky cannot be expressed today.

---

## Tier A — change the exponent (1000×+). The only tier that reaches "minutes."

### A1 — native big÷big divide + big subtract → binary-splitting Chudnovsky *(Large)*

**Why it's the biggest win.** Both existing generators are **O(d²)**: they run
~d series terms, each touching an O(d) number. Chudnovsky evaluated by **binary
splitting** with fast multiplication is roughly **O(d · log²d · loglog d)** — a
change of *complexity class*, not a constant. At 7,000,000 digits that is the
difference between ~272 days (current spigot) and minutes.

**What binary splitting needs that we don't have:**

1. **Big ÷ big division** (`bigdiv(num, den, quotient, remainder)`), typically
   via Newton–Raphson reciprocal on top of the existing NTT `bigmul`.
2. **Big subtraction** (`bigsub(a, b, result)`), limb-wise with borrow — trivial
   next to the divide.
3. **An integer square root** of a big number (for √10005 in the Chudnovsky
   constant), also Newton's method over `bigmul`/`bigdiv`.

Then Chudnovsky's `P(a,b)/Q(a,b)/T(a,b)` recursion is pure big ×, +, −, and one
final ÷ and √ — all expressible in SLeeLa once the three natives exist.

**Size: Large.** Three new native kernels (divide is the hard one — a correct,
fast reciprocal-Newton big÷big is a few hundred lines of careful C), three new
opcodes, and the per-opcode plumbing in the A-tier checklist below. This is the
right investment if millions of digits is a real requirement.

### A2 — a whole-series native Pi opcode *(Medium–Large)*

**Idea.** Instead of exposing primitives and looping in interpreted SLeeLa, add
one opcode — e.g. `OP_PI_DIGITS(n, out)` — whose C implementation runs the
*entire* chosen algorithm (spigot, Machin, or Chudnovsky) natively and writes
the digit array back once. The SLeeLa program becomes a one-liner that calls it.

**Impact.** Removes 100% of the interpreter and marshalling overhead — the
program runs at literally native-C speed for the chosen algorithm. With a
Chudnovsky core this also gets the A1 exponent; with a spigot core it is "only"
the full constant-factor win (still 100–1000× over the interpreted spigot).

**Trade-off.** It is the least "SLeeLa-expressed" option — the algorithm lives
in C, not in a `.sleela` file — so it is best as a *library kernel* the language
calls, not as the teaching example. Size is Medium if it wraps the spigot,
Large if it embeds Chudnovsky (then it subsumes A1).

---

## Tier B — shrink the big-kernel constant (2–5×). Keeps the algorithm in SLeeLa.

### B1 — operate on the VM limb buffer in place (remove the marshalling tax) *(Medium)*

Today each `bigmul`/`bigadd`/`bigdivmod` `malloc`s scratch `uint32_t` buffers,
copies the boxed `int[]` in, computes, copies back, and `free`s. For an
algorithm that calls these ~d times (`PiMachin`), that per-call O(n) copy is a
2–5× tax. Two ways to kill it:

- Give the kernels a path that reads/writes the VM `SLArrayInstance.elems`
  directly (the single-threaded fast path already proves direct access is safe),
  skipping the snapshot/write-back loops; or
- Store big integers in a **flat `uint32_t` limb buffer** type the kernels own,
  so no boxing/unboxing happens at all.

**Size: Medium** — touches the three `OP_BIG*` cases and the array representation
assumptions, but no new opcodes.

### B2 — native limb-array copy / fill / shift *(Small–Medium)*

`PiMachin`'s per-term `copyBigN(sum)` (and the zeroing in `newBig`) are O(d)
*interpreted* loops that run once per term → a hidden O(d²) interpreted cost on
top of the native divides. A single native `arrayCopy(dst, src, n)` /
`arrayFill(dst, v, n)` opcode (or letting `bigadd` write in place by relaxing its
strict `len(out) ≥ max+1` rule so double-buffering works) removes it. This is the
cheapest B-tier change and directly attacks `PiMachin`'s measured cost.

---

## Tier C — interpreter & representation polish (1.05–3×). Broad but constant.

### C3 — micro-tune existing fast paths *(Small)*

Hoist the `ai->len` bounds comparison, widen `SL_GC_SAFEPOINT_STRIDE` for pure
compute loops, keep `t->workers_cached == 0` cheap. Smallest possible change,
modest payoff, zero risk — a good warm-up.

### C1 — direct-threaded dispatch *(Medium)*

Replace the `switch(in.op)` with a computed-goto dispatch table
(`&&label` / `goto *table[op]`). Removes the switch's range check + jump per op;
typically 1.2–2× on interpreter-bound loops. Mechanical but touches the whole
opcode table and is compiler-specific (GCC/Clang extension).

### C2 — unboxed primitive arrays *(Large)*

Store `int[]`/`double[]` as flat `int64_t`/`double` buffers instead of 32-byte
`SLValue` cells, with a typed-array fast path in `OP_ARRGET`/`OP_ARRSET`. Cuts
memory 4× and makes element access a single load/store. Large because `SLValue`
boxing is woven through the array, GC, and struct code — but it is the change
that most helps *every* array-heavy SLeeLa program, not just Pi.

---

## Recommended path

1. **B2** (small) — kill `PiMachin`'s per-term interpreted copy. Cheapest real
   win; validates the approach.
2. **B1** (medium) — remove the big-op marshalling tax. Now `PiMachin` is close
   to "native kernel, SLeeLa glue."
3. **A1** (large) — add native `bigsub`, big ÷ big, and big `isqrt`, then write
   `PiChudnovsky.sleela`. **This is the step that reaches millions of digits in
   minutes** and is the one worth doing if that is the goal.
4. **C1 / C2** — general interpreter/representation speedups that help the whole
   language, pursued independently of Pi.

Tiers B and C make the *interpreted* parts faster; only **A** gets the whole
computation into the native-C performance regime. If the single ask is "much
closer to native C for Pi," the highest-leverage deliverable is **A1 (native
big÷big + subtract + isqrt) feeding a binary-splitting Chudnovsky generator.**

---

## Checklist — adding one native opcode (for any A/B item that needs one)

Each new big-integer opcode (e.g. `OP_BIGSUB`, `OP_BIGDIV`, `OP_BIGISQRT`) must
touch, in order:

1. `impl/core/sleela_core.h` — add the enumerator to `SLOp` (append-only, before
   `OP__COUNT`), and bump the `static_assert`/`_Static_assert` opcode-count
   guards (the extended ISA count).
2. `impl/core/sleela_core.c` — add the `case OP_...:` in `run_thread` plus the
   native kernel function, mirroring the existing `OP_BIGMUL`/`OP_BIGADD`
   marshalling (or the B1 in-place path).
3. `impl/frontend/semantic.cpp` — add the builtin name to the recognised set and
   its return type.
4. `impl/frontend/compiler.cpp` — lower the builtin call to the new opcode
   (argument count/validation like the existing `bigdivmod` lowering).
5. `lib/vm/InstructionSet.sleela` — append the `< OP_... N -- desc >` record and
   its `class SLISA...` so `tools/sleela-build.py inventory-lib` passes.
6. Regenerate `security/sha256-manifest.json`
   (`make -C impl regen-manifest`) — `sleela_core.h`/`.c` and
   `lib/vm/InstructionSet.sleela` are gated; the build's `verify-security` fails
   closed otherwise.
7. Rebuild (`cd impl && make build/sleela`) and verify
   `inventory-lib` PASS + `check-manifest` OK.

---

**Max Rupplin — MEARVK LLC — 2026**
