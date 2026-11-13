# TODO — A Recipe

A recipe for the work ahead. Each numbered course is prepared the same way:
**Ingredients** (what it needs), **Method** (how to make it), and a **Moral**
(the principle that governs the dish). Cook them in order or to taste; every
course must satisfy its Moral before it is served.

---

## 1. The "WE WILL" line for the base README

**Ingredients:** the exact wording to follow "WE WILL"; [`README.md`](README.md).
**Method:** take the phrase as given, fold it into the README verbatim, let it
rest under a clear heading, then push to `master` and `main`.
**Moral — Say only what you mean.** A declaration is written in the author's
own words, never invented on their behalf.

---

## 2. Runnable capstone under `capstone/`

**Ingredients:** [`tutorial/12-capstone.md`](tutorial/12-capstone.md); the
verified `nordshrift` toolchain.
**Method:** create `capstone/src/Ledgerize.sleela` and `capstone/build.sst`,
build them, and confirm the artifact and its `.ledger` verify.
**Moral — Proof over promise.** A tutorial that claims it runs must actually run.

---

## 3. HTTP 2.1 core: fill the sketch stubs

**Ingredients:** [`http-2.0/`](http-2.0/) naming/pipeline stubs; the 3.0 core
as a reference.
**Method:** implement `h21_naming.c` and `h21_pipeline.c` (minus the integrity
substrate), add a demo + parity tests, keep the `H21` wire byte-identical
across C and Python.
**Moral — Finish what you frame.** A sketch is a debt; honor it before adding
new rooms to the house.

---

## 4. Deadline on the wire (optional, breaking)

**Ingredients:** the advisory timing layer ([`http-3.0/http3_timing.{h,c}`](http-3.0/));
agreement that a wire change is wanted.
**Method:** add a MAC-covered `deadline` field to the envelope, make late
arrivals a hard reject, and version the change so peers negotiate it.
**Moral — Change the contract in the open.** A breaking change is announced and
negotiated, never slipped in.

---

## 5. Replay window that survives reordering

**Ingredients:** the NONCE high-water mark in the pipeline.
**Method:** replace the single high-water mark with a sliding replay window
(a bitmap of recently seen nonces), keyed per sender.
**Moral — Guard the common case without punishing the honest one.** Security
must not reject legitimate, merely out-of-order, traffic.

---

## 6. QoS worked example in the RMI demo

**Ingredients:** [`QOS.md`](QOS.md); the Java RMI client
([`rmi/java/.../SleelaRmiClient.java`](rmi/java/com/mearvk/sleela/rmi/SleelaRmiClient.java)).
**Method:** wrap a remote call with an explicit timeout and an `isHealthy()`
back-off, and show the latency/certainty reading it produces.
**Moral — Advice earns trust by example.** A guideline is worth more when it is
demonstrated than when

 it is merely stated.

---

## 7. HTTP colors for the 2.1 sketch

**Ingredients:** [`http-3.0/HTTP-COLORS.md`](http-3.0/HTTP-COLORS.md); the 2.1
core once it exists (course 3).
**Method:** define a trimmed 2.1 palette (wire form + flags only — no MAC/
INTACTX/NONCE/basket tiers), mirror the loader, and document it.
**Moral — Adapt the pattern, do not copy it whole.** A younger generation
inherits the shape of a standard, not the parts it cannot yet bear.

---

## 8. Cross-link the term-care note

**Ingredients:** [`GLOSSARY.md`](GLOSSARY.md) §B.2; [`NUMERAL-INTENT.md`](NUMERAL-INTENT.md);
[`FIDUCIARY.md`](FIDUCIARY.md), [`BRITISH.md`](BRITISH.md), [`politico/SAKES.md`](politico/SAKES.md).
**Method:** add a one-line reference to the Celebrity term-care standard from
each document that touches the term, so the care travels with the word.
**Moral — Keep the guardrail where the road turns.** A safeguard belongs at
every place the hazard appears, not only where it was first written.

---

## 9. Ledger the whole tutorial tree

**Ingredients:** [`ledger/ledger.py`](ledger/ledger.py); the 12 demo sources.
**Method:** build one `.ledger` chain over all tutorial demos, publish the QR
insignia set, and add a `make`-able verify step.
**Moral — Account for everything you ship.** What is worth teaching is worth
sealing against silent change.

---

## 10. A verification pass across the docs

**Ingredients:** every `.md` touched this session; the link checker in your head.
**Method:** confirm each cross-reference resolves, each claim matches the code,
and each honest caveat (no synced clock, UTC-not-RFC-3161, structural-only QR
validation) is still stated where relevant.
**Moral — Tell the truth about your own work.** A document earns belief by
naming its limits as plainly as its features.

---

## 11. Fold in the secure-transition client (`feat/secure-transition-stp`)

**Ingredients:** the branch `feat/secure-transition-stp` (~1,809 lines: a new
`impl/transition/` STP-0001 client — `stp_client`, `stp_wire`, `stp_crypto`,
`mem_model`, portable SHA-256 — plus edits to `impl/Makefile`,
`impl/core/sleela_core.{c,h}`, and `impl/frontend/driver.cpp`); a clean checkout;
the SHA-256 build gate; the VM test targets.
**Method:** merge it onto a working branch, regenerate the manifest, then
**rebuild the whole VM** (`make all`) and run `test-version`, `test-threads`,
`test-network`, `test-runtime`, `test-memmgr` before it touches `master`/`main`.
It modifies the freshly-health-checked VM core, so it is **not** a clean add —
resolve any drift against the current core deliberately.
**Moral — Touch the engine only with proof in hand.** Core changes earn their
place by rebuilding green, not by merging quietly.

---

## 12. Reconcile the metatag / seal metalayer (`feature/metatags-mt-meta-0001`)

**Ingredients:** the branch `feature/metatags-mt-meta-0001` (~1,950 lines:
`METATAGS.{md,index,sheet}`, `SEAL.md`, `impl/metatag/`, `impl/seal/seal.py`, an
`mt-check` CI gate, `.mt/seals/`); note it also **modifies** `README.md`,
`SHEET.sheet`, `SST.model`, and `impl/README.md`.
**Method:** rebase or three-way merge so the shared-file edits layer on top of
the current `README`/`SHEET`/`SST` (which have moved since the branch was cut);
confirm the `mt-check` workflow is well-formed; then land on `master` and `main`
with the READMEs reconciled, not clobbered.
**Moral — Merge the change, keep the ground it stands on.** Shared files are
reconciled, never overwritten.

---

## 13. Retire or supersede `add-common-rails-sst-sleela`

**Ingredients:** the branch `add-common-rails-sst-sleela`; the **current**
`common-rails/` on `master` (which already exists in a newer form).
**Method:** diff the branch's older `CommonRails.sleela` and `README.md` against
the live files; salvage anything still wanted, then **close the branch without
merging** so the newer versions are not regressed. Record the decision.
**Moral — Newer ground is not overwritten by older footprints.** A superseded
branch is retired on purpose, not merged by reflex.

> **Consolidation note (2026-09-20).** The low-risk branch
> `feature/pixel-terminal-60hz` was already merged to `master` and `main`
> (`terminal_pixel/pixel_terminal.h`). Courses 11–13 are the remaining
> non-`main` branches, held back because each is risky or superseded rather than
> a clean add. The `backup/main-before-library-sync-*` branch is a snapshot and
> is intentionally left untouched.

---

## 14. Java operand semantics: close the last gaps

**Ingredients:** [`lib/java/STATUS.md`](../lib/java/STATUS.md) section 8;
`lib/java/JavaOperands.sleela`; `impl/frontend/compiler.cpp` (`emitJavaNarrow`,
`coerceToDeclared`); `examples/java/differential/OperandSemantics`; a real JDK
for the side-by-side run.
**Method:** done so far (2026-11-12, `3340262e`): stores into and casts to
`byte`, `short`, `char`, `int32` wrap at the Java width; `JavaOperands` supplies
Java `int` arithmetic, shifts, the `long` division corner and the JLS 5.1.3
conversions; 189 lines identical to OpenJDK 25.0.2. Remaining, in order of
value, each with a differential row added **before** the fix:

| # | Gap | Evidence | Fix |
|---|---|---|---|
| 1 | Stores into `byte[]` / `short[]` / `char[]` elements do not wrap | `byte[] a; a[0] = 127; a[0] += 1;` prints `128`, Java `-128` (checked 2026-11-12; locals, fields, `+=`, `++` all wrap) | narrow on `ARRSET` using the array element type; keep reads un-narrowed (new elements read `null`) |
| 2 | `char` literals do not lex | `'a'` is a lexical error | lexer: `'x'`, escapes, `\uXXXX` to an integer token typed `char`; decide `"" + c` prints text, not the number |
| 3 | Doubles print `inf` / `nan` | `5.0 / 0` prints `inf`, Java `Infinity` | change the VM double-to-text path, or route through `Double.toString`; update fixtures |
| 4 | `float` is a double | no 32-bit rounding, `Float` bit views are done in library code | round-to-float on `float` stores |
| 5 | Plain `int` is 64-bit | `2147483647 + 1` is `2147483648` | **decision needed**: keep (current, safe) or add a `#sleela` Java source mode where `int` wraps; never change `int` globally |
| 6 | Integer division corners are fatal in the VM | Checked 2026-11-12: `long.MIN_VALUE / -1` **crashes the VM with SIGFPE** (exit 136, core dump; the C division traps); `5 / 0` stops the program with `runtime error: integer divide by zero`, which `catch (Any e)` does **not** catch. Java gives `MIN_VALUE` and `ArithmeticException`. `JavaOperands.divLong` / `remLong` / `divInt` guard both cases today | VM: handle the `-1` divisor before dividing (wrap, as Java does) and raise a catchable `ArithmeticException` for a zero divisor. The crash is a robustness bug, independent of Java compatibility |

**Moral — Measure the Java, do not imagine it.** Every operand rule is settled
by running the real JDK beside the SLeeLa VM and diffing the output.

---

## 15. Complete `/lib/java`: assessment of what remains

**Ingredients:** [`lib/java/COVERAGE.md`](../lib/java/COVERAGE.md) (regenerated by
`lib/java/tools/jdk_api_coverage.py`); [`lib/java/STATUS.md`](../lib/java/STATUS.md);
`examples/java/run-conformance.sh`; `examples/java/differential/run-differential.sh`.
**Method:** measured state (2026-11-12; every figure re-run, see
[`lib/java/STATUS.md`](../lib/java/STATUS.md) sections 0, 1 and 4). `/lib/java`
holds **two trees** measured against one yardstick (the reflected API of
OpenJDK 25.0.2: 4,801 types, 51,554 members):

| | `jdk-25/` tree | `jdk-28/` tree (`import java.jdk-28;`, the one new code uses) |
|---|---:|---:|
| Types with a file | 4,801 (100%) | 4,656 (97.0%) |
| Members covered | 23,432 (45.5%) | 12,591 (24.4%) |
| Members with working behavior | 5,165 (10.0%) | 4,123 (8.0%) |
| Exact-signature methods awaiting logic | 24,101 | 33,021 |

Verification today: **9 / 9** conformance programs; **17 / 17** differential
tests (641 lines identical to a real JDK); 6 / 6 `jdk-28` demos run; compiler
suite **49 / 50** targets (`test-osls-isa` fails on the untouched baseline too);
accessor contracts **48 / 78** packages (30 fail, identical on the earlier
compiler, undiagnosed). The older "386 constructors, 0 failures", "all
compiler tests pass" and "19 differential tests" figures were wrong or stale.

*Honest scope.* Behavior is 10% of members. Of the 46,389 members not yet
working in the `jdk-25/` tree, about **12,000** are in `java.desktop` AWT/Swing/print/imageio
presentation code that needs a window system, and a further ~3,200 are in
`java.sql`, `java.lang.classfile`, `org.w3c.dom.html` and
`com.sun.source.*`. A rule of thumb for planning: reaching high behavior
coverage of `java.base` (17,496 members, 18.3% now) is realistic; reaching it
for the desktop modules is a different project and should be declared out of
scope or deferred explicitly rather than counted as a failure.

**Stage 0: bring the `jdk-28/` tree up to the `jdk-25/` tree.** New code
imports `jdk-28`, which trails by 21 points of coverage (24.4% versus 45.5%).
(a) carry the generated declarations (constants, fields, enums, records,
abstract methods, probed accessors) into `jdk-28/`; (b) create the 145 missing
types, 119 of them nested `Outer.Inner` (needs A4); (c) point the generators
in `jdk-25/tools/` at the `jdk-28/` layout; (d) every "done" item must say
which tree it landed in. Also: diagnose the 30 failing accessor-contract
packages and fix or waive `test-osls-isa`.

**Stage A: language and VM prerequisites (they unblock most of Stage B).**
These are the compiler gaps in `STATUS.md` section 6 and
`JDK28-PROCEDURAL-IMPLEMENTATION.md`; each removes a class of
`UnsupportedOperationException` stubs at once.

| # | Prerequisite | Unblocks |
|---|---|---|
| A1 | ~~Virtual dispatch through a base-typed receiver; `super(...)` lowering; base field initializers for subclasses~~ **done** (SLEELA.syntax 43.6; `impl/tests/java_references/oop/`) | regenerate the library classes that still work around it (exception messages composed at construction) |
| A2 | ~~Typed `catch`, multiple catch clauses, `finally`~~ **done** (43.6 rule 11). Still open: `return` / `break` / `continue` out of a try with `finally` (43.5), try-with-resources, catchable VM faults (`/ 0`) | faithful `try`-with-resources, `Closeable`/`AutoCloseable`, exception flow in `java.io` / `java.net` |
| A3 | Lambdas and functional interfaces (`Comparator`, `Function`, `Consumer`, `Supplier`, `Predicate`) | comparator sorting, `compute` / `merge` / `forEach`, `Optional.map`, then `java.util.stream` |
| A4 | Nested types resolved by the loader (`Map.Entry`, `Character.UnicodeBlock`) and exact-package import matching | `entrySet`, `Map.Entry`, ~hundreds of `Outer.Inner` files that are unreachable today; also the largest cause of `check` errors in converted JDK code (course 16) |
| A5 | Live collection views (`keySet`, `values`, `entrySet`, `subList` return snapshots) | Java-correct iteration and mutation semantics |
| A6 | Zero-fill numeric arrays by element type (new `int[]` elements read `null`) | removes explicit clearing in library code and the 2^53 `long`-sum hazard |
| A7 | Task 14 above (array-element narrowing, `char` literals, `Infinity` text) | `char`-heavy code: `String`, `StringBuilder`, `Character`, `java.text` |

**Stage B: behavior by package, largest practical payoff first** (unimplemented
members, from the `jdk-25/` tree's `COVERAGE.md`; `java.base` first, presentation last).

| Order | Package | Unimplemented | Notes |
|---:|---|---:|---|
| 1 | `java.util` | 1,549 | `Map`/`List`/`Set` interfaces and defaults, `Comparator`, `Scanner`, `Formatter`, `StringJoiner`, `Locale`; needs A1, A3, A4 |
| 2 | `java.lang` | 1,519 | `String` / `StringBuilder` statics (`valueOf`, `join`, `format`), `Thread`, `Class`, `Enum`, `Record`, `ProcessBuilder`; needs A1, A2 |
| 3 | `java.util.concurrent` | 878 | executors, futures, locks, `CompletableFuture`; maps onto the VM thread and lock opcodes |
| 4 | `java.io` | 780 | streams, readers, writers, `File` on the SLeeLa IO layer |
| 5 | `java.net` | 710 | `URI`, `URL`, `InetAddress`, sockets on `sleela_net` |
| 6 | `java.security` | 599 | digests, `MessageDigest`, `SecureRandom`; keep the clean-room, no-proprietary-code posture |
| 7 | `java.text` | 471 | `DecimalFormat`, `SimpleDateFormat`, `MessageFormat`, `Collator` |
| 8 | `java.time` + `java.time.chrono` | 429 + 392 | formatters, zones, `ChronoUnit`; core types already behave |
| 9 | `java.util.stream` (new), `java.util.regex` | tbd | needs A3; regex has native support to bind |
| 10 | `java.sql`, `javax.management`, `java.lang.classfile` | 1,170 / 433 / 1,103 | contract-level only until a driver / agent story exists |
| deferred | `java.awt`, `javax.swing`, `javax.print`, `javax.imageio`, `org.w3c.dom.html` | ~12,000 | declare out of scope, or bind to the native SleelaUI toolkit as a separate project |

**Stage C: keep the numbers honest.**
1. ~~Reconcile the stale figures~~ done 2026-11-12: `STATUS.md`,
   `lib/java/README.md` and `jdk-28/README.md` now carry the measured numbers.
   Keep it that way: update the tables from `COVERAGE.md` and
   `COVERAGE-JDK28.md`, never from memory.
2. Regenerate `COVERAGE.md` (`--tier jdk-25`) and `COVERAGE-JDK28.md`
   (`--tier jdk-28`) after each batch and refresh the chart in `STATUS.md`
   from them, never by hand.
3. Add each new class to the conformance or differential runner **in the same
   commit**, and run both runners plus `lib/java/validate.py` and
   `test-suites/test-library-inventory.sh` before pushing.
4. After any `lib/**` or `impl/` change: regenerate `security/sha256-manifest.json`,
   `lib/LIBRARY.SYMBOLS.md`, the four `EXPECTED_*` in the inventory test and the
   `LIBRARY.INDEX.md` header counts.
5. Prefer a differential test (real JDK beside the VM) over a hand-written
   expected value; the JDK is the source of truth.

**Suggested next steps:** (A1 and A2 have landed since this was written; see the table.) (0) Stage 0, the `jdk-28/` parity gap, because it is the largest measured shortfall; (1) Task 14 item 1 (array-element narrowing,
small and verifiable now); (2) A2, typed `catch` and `finally` (many library
bodies are waiting on it); (3) A1, virtual dispatch (largest single unlock).
**Moral — Count what runs, not what is declared.** A signature that throws
`UnsupportedOperationException` is a promise, not a feature; the progress bar
counts only behavior that a real JDK agrees with.

---

## 16. The Java converter (`lib/java/converter`): from first cut to useful

**Ingredients:** [`lib/java/converter/README.md`](../lib/java/converter/README.md)
(rule table, stub reasons, measurements); `lib/java/STATUS.md` section 11; a JDK
`src.zip` (OpenJDK 25.0.2 is installed in the sandbox); the separate
`sleelactl java-convert` and its steering
`.kiro/steering/java-to-sleela-conversion.md`.
**Method:** state on 2026-11-13, converter 0.2.0 (syntax 3.1 forms; `for`
headers with commas and pattern `instanceof` lowered): on a 17-file JDK sample,
16 converted, 36 SLeeLa files, 627 members, 35 fail-loud stubs (5.6%; 47 in
0.1.0), **0 syntax errors**, 8 of 36 files pass `sleela check`; its own sample
program runs 23 of 24 lines identical to a real JDK. Next, in order:

| # | Step | Why |
|---|---|---|
| 0 | ~~Decide the policy~~ **decided: per-member fail-loud stubs**; shared laws use the same `conversion.model` templates | |
| 1 | ~~A bundle mode~~ **done in 0.3.0** (`JCONV_BUNDLE=1`): 39% fewer check errors on the sample; the sample program runs as one document | |
| 2 | Loader: resolve `Outer.Inner.sleela`; compiler: inner classes reach the enclosing instance; inline nested classes (SLEELA.syntax 14) with constructors, `extends`, statics and arrays | most `check` errors in `HashMap` / `AbstractList` (course 15, A4). Inline nesting was measured and is too limited today (converter README section 6) |
| 3 | Library: real statics for `Float`, `Objects`, `Arrays`, `Character` | "instance method must be called on an instance" errors |
| 4 | Expression types: names, literals, casts, array elements (0.3.0); **call results through the model's `ret` rows, read from `conversion.model` (0.4.0; model 1.1.0 has 8,922 reflected rows)**. **`int` arithmetic where its value leaves int arithmetic, and `/` `%` through `divInt` / `remInt` (0.5.0, `JConvExpr`)**. Next: callee parameter types for arguments (drop unneeded wraps), receivers typed by an interface the model does not list, generic results (`List<String>.get` is a type variable), stores into `int` array elements (the VM does not wrap them) | output stays at syntax 3.1 (owner decision) |
| 5 | Lower lambdas and method references (`lib/java/jdk-28/LOWERING.md` section 2) | the largest group of stubs in real JDK code |
| 6 | Non-ASCII sources: a byte-safe read, or a VM helper | 12 of 15,020 JDK files are skipped |
| 7 | A differential pair per supported construct (`samples/`), run occasionally | equivalence is shown by output, not claimed |

**Moral — A converter is measured by what runs.** Count the files that check
and the lines that match a real JDK, never the files written.

---

*Serve in order for a full meal, or take any single course. No dish is done
until its Moral is met.*


# Version / Compiler Work Closure — Items 1–7

**Status:** COMPLETE — 2026-09-27  
**Current development line:** SLeeLa 1.3.2-pro / Sleelvac 1.3.2-pro / Nordshrift 2.7.1-dev  

The first seven version-and-toolchain work items are now closed. Future work must preserve the version identities established here and update version numbers as implementation milestones advance.

1. **Sleelvac / compiler identity — COMPLETE**
   - Sleelvac is identified as **1.3.2-pro**.
   - Compiler documentation and implementation identity are synchronized.

2. **Version-string cleanup — COMPLETE**
   - Known stale 0.1/0.2/Sleelvac 1.5 and Nordshrift 2.1–2.5 development identifiers were audited; no remaining indexed matches were found.

3. **Nordshrift milestone synchronization — COMPLETE**
   - Nordshrift is identified as **2.6-dev** across the active implementation and documentation.

4. **Syntax compatibility identity — COMPLETE**
   - Sleela syntax support is **1.3 .. 1.3** and compiler defaults are synchronized to syntax 1.3.

5. **Runtime artifact ABI identity — COMPLETE**
   - VM ABI is **1.0** and artifact format is **2**; runtime artifact validation is documented and versioned.

6. **Cross-version compatibility gate — COMPLETE**
   - Nordshrift 2.6-dev compatibility fixtures and the compatibility test gate are recorded as complete.

7. **Complete version registry / documentation closure — COMPLETE**
   - The complete component registry is maintained in VERSION.md, including independently versioned API/server, server-edition, moral, port-awareness, terminal, standard, and metadata components.

**Ongoing rule:** Every subsequent implementation milestone updates its applicable version numbers, source identity, tests, and master documentation together. Independent component versions are changed only when that component itself changes.
