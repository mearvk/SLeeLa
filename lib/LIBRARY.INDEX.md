# SLeeLa /lib Library Index

> Revision 0.69 adds the **`advantage`** SLeeLa standard-library package (`lib/advantage/`): the Sequitur onion model for advantaging a unique dividend with respect to a model. It ships `SLAdvantageLayer` (one concentric onion layer), `SLAdvantageDividend` (the unique dividend held wrt a named model), `SLAdvantageSequitur` (the ordered, navigable onion of layers — dividend, review, concerns, maths, additions, subtraction, regrouping, iteration — with inward/outward cursor navigation), and two encapsulations over the same engine: `SLAdvantageUserModel` (simple, outcome-facing) and `SLAdvantageDeveloperModel` (full-control). A `SLPackage` facade and a runnable `advantage-demo` (validated via `sleela run`) complete the package.

> Revision 0.68 tightens the AI-facing facade contracts: neural-network configuration now owns defensive copies of layer sizes, adapter-only operations cannot claim native readiness, and character-recognition validates input-buffer dimensions and finite similarity thresholds. The native C APIs remain the execution authority until the embedding runtime supplies adapters.

> Revision 0.67 adds two AI-facing SLeeLa standard-library packages. **`neural-nets`** (`lib/neural-nets/`) mirrors the native C ABI v1 with `SLNeuralNetworkConfig`, `SLNeuralNetwork`, and a package facade; it exposes the validated network configuration and lifecycle contract while inference/training remain explicit native-adapter operations. **`character-recognition`** (`lib/character-recognition/`) mirrors the native C11 glyph-recognition API with `SLCharacterRecognition`, `SLCharacterResult`, and a package facade; normalization and recognition remain native-adapter operations. Both packages document their exact native boundaries and limitations. The authoritative C/C++ implementations remain under `artificial-intelligence/`; these SLeeLa packages do not duplicate kernels or claim native execution without an adapter.


**Revision:** 0.69  
**Packages:** 117  
**SLeeLa source units:** 11924  
**Module-facade symbols:** 84  
**Total symbol records:** 12008  
**Symbol manifest:** `LIBRARY.SYMBOLS.md`

> Revision 0.66 brings three big-number improvements that make a 10,000,000-digit
> multiply run in ~3 seconds (from ~75 s). (1) **NTT multiply**: OP_BIGMUL now
> uses a number-theoretic transform (exact, mod a 62-bit prime, base-1000
> sub-limbs) above 512 limbs — O(n log n) vs Karatsuba's O(n^1.585). A
> 10M-digit multiply dropped from ~39 s to ~3 s and scales linearly. (2) **New
> big-integer opcodes** OP_BIGADD and OP_BIGDIVMOD (the `bigadd(a,b,result)` and
> `bigdivmod(dividend,divisor,result)` builtins), completing the op set needed
> for base conversion and a native Chudnovsky pi. (3) **Lock-free primitive
> arrays + O(1) GC tracing**: the array opcodes skip the per-element mutex when
> single-threaded, and the collector no longer re-scans a scalars-only array
> (a new has_refs flag), removing an O(n^2) cost — filling a 1,000,000-element
> array went from ~9 s to ~0.1 s. Regression guards `examples/BigNumOpsVerify`
> (Karatsuba+NTT multiply, add, divmod vs references), `examples/BigMulVerify`,
> and `examples/BigArrayGCVerify` all pass; threading/synchro/munction tests
> pass (the lock-free path falls back to locking as soon as a worker spawns).

> Revision 0.65 adds the `slrtp` package — the Sleela Retrouval Tactical
> Processor, SLeeLa's AOP-style interception framework (aspects, pointcuts,
> join points, and before / after-returning / after-throwing / around advice).
> Seven new `#sleela 3.0` units under `lib/slrtp/` (SLPackage, SLRTPJoinPoint,
> SLRTPPointcut, SLRTPAdvice, SLRTPAspect, SLRTPProcessor, and a runnable
> slrtp-demo), one new package and one new facade. Because the VM has no
> bytecode weaver and the checker does not upcast, interception is explicit and
> advice is data-driven (a pointcut plus a built-in action). Counts rise from
> 113/11907/80/11987 to 114/11913/81/11994.

> Revision 0.64 makes very large numbers computable in SLeeLa by fixing two VM
> bottlenecks exposed by big-integer work. (1) **Batched GC safepoint**: the
> interpreter polled the collector -- taking a thread mutex -- on every single
> instruction; it now polls once per 1,024 instructions (allocation opcodes poll
> directly), so a tight fill/loop no longer pays a mutex per opcode. Filling a
> 50,000-element array went from timing out (>100 s) to ~15 ms. (2) **GC
> allocate-black**: the incremental generational collector snapshots roots once
> at the start of marking, so an object allocated mid-marking was not rooted and
> was swept while still live (surfacing as "index assignment on a non-array
> value" when a second large array was built during a collection). New objects
> allocated during an active cycle are now marked live for that cycle. Together
> these let a 10,000,000-digit multiply run end to end for the first time
> (~75 s total on Gen1), and put a ~1,000,000-digit single multiply at about one
> second (fill 200 ms + multiply 1,120 ms). New regression guards:
> `examples/BigArrayGCVerify` (two large arrays across a collection) and the
> existing `examples/BigMulVerify`.

> Revision 0.63 fixes a correctness bug in the OP_BIGMUL Karatsuba kernel and
> adds a regression guard. The `z1 = z1 - z0 - z2` recombination subtracts two
> operands from one limb, so the running borrow can reach two per limb, but the
> kernel only handled a borrow of one -- producing wrong high limbs (an exact
> multiple of 2^32) for products above the Karatsuba threshold. The borrow is
> now carried as an integer count of bases. New `examples/BigMulVerify` checks
> native `bigmul()` against a pure-SLeeLa schoolbook multiply across sizes from
> 41 to 2,223 limbs (~370 to ~20,000 digits) and asserts the limb arrays are
> identical. Measured after the fix: a ~20,000-digit multiply is byte-identical
> to the reference and about 1,389x faster than the schoolbook multiply
> (~2.1 ms native vs ~2.87 s pure-SLeeLa).

> Revision 0.62 adds a native big-integer multiply to the VM and wires it into
> `SLBigInteger`. A new append-only opcode **`OP_BIGMUL`** (156) multiplies two
> base-1,000,000,000 limb `int[]` arrays with a C Karatsuba kernel in a single
> native call -- bypassing the per-element array mutex and the per-instruction
> interpreter overhead that make a hand-written limb loop slow. It is exposed to
> SLeeLa as the `bigmul(a, b, result)` builtin and used by
> `SLBigInteger.multiply()` (plus new `fromLimbs`/`toDecimalString` helpers and a
> `examples/BigIntegerDemo`). Measured: a 1000-limb x 1000-limb product (about
> 9,000 decimal digits each) completes in well under a millisecond, versus the
> ~1,000,000 interpreted limb-pair array operations a pure-SLeeLa schoolbook
> multiply would run. This is the VM bridge for heavy arithmetic that
> `SLBigInteger` was originally designed to call, and the foundation for
> computing very large numbers (e.g. high-digit pi) in SLeeLa itself.

> Revision 0.61 adds the math precision norms and the Modium pi reference.
> `SLMathNorms` carries the editable norms -- irrational-digit carry 2048,
> repeating-digit carry 58, and a Finance-Verb carry with a hard floor of 1023
> digits (requests below the floor are rejected and pinned). The norms are
> documented and user-editable in `lib/math/MATH.PRECISION.NORMS.md` and
> `lib/math/math-precision-norms.json`. `SLPiReference` is the Modium static
> reference variable for pi: it carries pi inline to the 2048-digit irrational
> norm, records its digit count and digit checksum, and self-verifies (head/tail
> composition + echo) using only the string equality and concatenation that
> SLeeLa guarantees -- the sibling `examples/PiReferenceDemo` reports pi
> correctness on compile and run. The SLeeLa registry (`SLRegistry.seedDefaults`)
> seeds a `Math` key with the three norms, the pi digit count, and the pi head
> as the default start values a fresh install begins from. (A deeper reference
> set in the millions of digits is produced out-of-band and resolved by handle;
> the in-worker generator could not hold the full 7M-digit working set, so the
> inline catalog value is pinned at the 2048-digit irrational norm.)

> Revision 0.60 brings the `math` package to life. All **243** `lib/math`
> classes, which previously carried only a generic `state`/`count` stub, now
> have real, compiler-verified SLeeLa implementations spanning number theory,
> statistics and probability, distributions, regression and hypothesis tests,
> linear algebra (determinant/inverse/eigen), calculus and numerical methods,
> ODE/PDE steps, Fourier/Laplace/Z transforms and signal processing,
> optimization, sequences/series/combinatorics, set theory, geometry and
> transforms, number types (complex/rational/big), expressions and symbolic
> rules, fields, units, graphs, and tensors. The package also gains its
> `SLPackage` facade and a runnable `examples/MathDemo`. Elementary functions
> (`sqrt`, `exp`, `ln`, `sin`/`cos`, nth roots) are implemented inline in pure
> SLeeLa; matrices/tensors are flat row-major `double[]`. Every result was
> checked against known values via `impl/build/sleela run`.

> Revision 0.59 adds the SLeeLa mirrors of the recently-built AI/LLM C/C++
> subsystem as two new `/lib` packages. **`large-language-models`**
> (`lib/large-language-models/`) mirrors
> `artificial-intelligence/large-language-models`: `SLLlmEngine` (backend-driven
> engine lifecycle), `SLLlmGenerationOptions`, `SLLlmVocabulary` (stable
> insertion-order token ids), `SLLlmBaseVocabulary` (the `sleela-base-en-1.0`
> seed with ten specials at fixed low ids), `SLLlmReasoning` (the RLM layer:
> subjects, step kinds, verdicts, and the UNKNOWN-never-promoted commit policy),
> `SLLlmCapacity` (the 2–16 GB envelope and derived reasoning budget),
> `SLLlmCodeModel` (procedural code-model profile), `SLLlmSleelaVerifier` (the
> static SLeeLa-rule checker that acts as the reasoning layer's symbolic half),
> `SLLlmParametry` (known-by-name or local-by-path model binding, fail-closed),
> and `SLLlmLoader` (the fixed-width loader/handler idiom), plus a runnable
> `examples/LlmReasoningDemo`. **`rrs`** (`lib/rrs/`) mirrors
> `artificial-intelligence/rrs`: `SLRrs` (Reasonable Responsibility Solidity —
> the three-stage congruent feed-forward over the ternary 1/0/2 signal, the fold
> kernel, and the `potestas` culmination) and `SLRrsSignalMonitor` (read-only
> observability over the signal), plus a runnable `examples/RrsDemo`. The native
> C/C++/Python sources remain authoritative; these classes expose the same
> contracts to the SLeeLa compiler, Loader, and Nordshrift, and carry no model
> or weights. Both demos pass `sleela check` and `sleela run`.

> Revision 0.58 adds **VMSharp™** under the `vm` package (`lib/vm/vmsharp/`): a
> VM manifest that can start as a self-installer bootloader for either a
> **Minimal Desktop** or an **OS install by user-designated download**, on
> commodity desktop processors — 32-bit **x86** and 64-bit **x64**, whether
> **AMD** or **Intel** (x64 → UEFI, x86 → BIOS/MBR; live CPU facts via the OSLS
> `oslsGetCPUInfo` read-only call). Five source classes plus the package facade:
> **`VMSharp`** (the composing manifest), **`VMSharpBootloader`** (architecture +
> vendor admission), **`VMSharpInstallPlan`** (the two startup modes with a
> fail-closed designation gate — a download/install never proceeds without an
> explicit, recorded source and target), **`VMSharpLevy`** (the declared
> USD 380 per-use/install figure directed to a United States national retirement
> process — disclosed, never auto-collected), **`VMSharpIncludedApp`** (an app
> the image carries as a pre-designated install), and a runnable
> `examples/VMSharpDemo`. The first included app is the **Java National GUI
> FrontEnd Aspect** (`github.com/mearvk/Java.National.GUI.FrontEnd.Aspect` @
> `main`), included for **Graduated Presidents of the United States**: a Java
> Swing app built with a JDK (`javac`→`out/`, run `java -cp out Main`) that runs
> on the VMSharp Desktop; its quiet install auto-designates the source while the
> install target still passes the fail-closed gate. The classes are declarative
> models: no firmware is flashed, no disk is written, no funds move, nothing is
> cloned/compiled/run here; a real bootable image and a real build are produced
> only by the pinned OS Creator toolchain. VMSharp can also **reach a GitHub repo
> over a networking stack, acquire it, and run the OS Creator** against it:
> **`VMSharpNetworkStack`** (HTTPS/TCP reach + OSLS `oslsNetworkResolve`),
> **`VMSharpRepoAcquisition`** (clone / tarball / zip with the exact command/URL
> for each), and **`VMSharpOSCreatorRun`** (the first-brush orchestrator targeting
> `github.com/mearvk/OS.Sensitivity.III` for install instructions/data/code, with
> a fail-closed gate — the OS Creator runs only after the repo is reachable AND
> acquired) plus `examples/VMSharpOSCreatorDemo`; these reach nothing, clone
> nothing, and build nothing. The fine finish adds the **install screens**:
> **`VMSharpInstallWizard`** (welcome → CPU-admission gate → mode-select →
> desktop/OS.Sensitivity.III path screens → fee disclosure → fail-closed
> confirm) and the selectable **`VMSharpInstallOption`** component, plus
> `examples/VMSharpInstallWizardDemo`; the wizard renders text screens and
> drives the plan objects, and confirmation is gated on an admitted CPU and a
> ready plan (OS path: reachable AND acquired AND target designated). It also
> adds **USB boot media**: **`VMSharpUsbMedia`** models `make` → `make install`
> → firmware loads from USB → VMSharp starts in a configured option, with an
> engineering-preference knob (`sequitur` | `iq` | `havingness`), plus
> `examples/VMSharpUsbMediaDemo` and **real per-OS Makefiles** under
> `lib/vm/vmsharp/usb/` (a brand-detecting dispatcher + `Makefile.linux`,
> `Makefile.windows`, `Makefile.macos`). `make` stages the files; `make install`
> is fail-closed — it refuses without an explicit `USB_DEVICE` and an
> `I_UNDERSTAND_THIS_ERASES=yes` confirmation, never auto-selects a disk, and
> stages to a git-ignored `build/` dir outside the inventory. No firmware binary
> is fabricated (VMSharp is a model; the media carries files + a boot
> descriptor). And **`VMSharpDiskLayout`** lets VMSharp **choose the root
> filesystem** (EXT4 default, or `xfs`/`btrfs`/`zfs`/`f2fs`/`ntfs`/`apfs`;
> unrootable choices refused, faithful to `SLFilesystemCatalog`) and produce the
> **initial hard-drive manifest** (GPT: FAT32 ESP + root + swap) and the
> `parted`/`mkfs` partition plan — fail-closed until a target disk is designated,
> and `VMSharpOSCreatorRun.osCreatorReady()` now also requires the disk layout
> ready; the install wizard gained a disk-layout screen. It reads no disk, writes
> no partition table, and runs no mkfs. Authored
> for the User MEARVK LLC
> (Max Rupplin 2026; George Soros a Kindness 2507+), respectful of United States
> law and of the persons and groupings of peoples it concerns. Every class
> carries the Metadata Class metapattern. Counts reconciled by
> `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`. See `lib/vm/vmsharp/`.

> Revision 0.57 grows the registered **`osls_*`** VM surface from **12 to 25**
> read-only operations (OP_OSLS_CALL IDs 0–24) — no new opcode (155 stays
> frozen; the typed registry grew). The 13 added calls are all read-only or
> pure computation and bind under the existing host-read capability:
> `osls_get_memory_info`, `osls_get_available_memory`, `osls_get_disk_space`,
> `osls_get_volume_info`, `osls_get_system_load`, `osls_get_capabilities`,
> `osls_get_runtime_info`, `osls_get_native_error`, `osls_path_normalize`,
> `osls_path_join`, `osls_path_resolve`, `osls_get_path_info`,
> `osls_network_resolve`. `SLOsSupportOSLSElements` gains the 13 matching element
> factories, `SLOsSupportNative` the 13 wrappers, and `SLOsSupportCatalog` now
> separates `getRegisteredInspectionFunctions` (live) from
> `getStateChangingPlannedFunctions` (28 file/socket/timer/process operations
> that stay out of the read-only path by design, per DESIGNATE.md §9). Native
> C: new `os-level-support/osls_extended2.c`; and the long-standing per-platform
> unevenness in the original twelve is fixed — Windows network enumeration (IP
> Helper), CPU vendor/model + cache-line on all three platforms, and the real
> Windows timezone key name. No new `.sleela` files, so the counts above are
> unchanged. See `os-level-support/DESIGNATE.md` §11 and
> `lib/os-level-support/README.md`.

> Revision 0.56 indexes the **`os-level-support`** package (`lib/os-level-support/`)
> in `LIBRARY.SYMBOLS.md` for the first time — the 20 source classes plus two
> runnable examples that mirror and bind the native OSLS C ABI. New in this
> revision: **`SLOsRuntimeStatus`** (the in-process runtime status space,
> `slosh_status` / `osls_runtime_status`), split out from **`SLOsSupportStatus`**
> which now carries the core `osls_status` values it was conflating;
> **`SLOsSupportTyped`** (a typed-result binding catalogue pairing each OSLS
> record call with its `lib/systems` record shape); and
> **`SLOsSupportConsistencyExample`** (a runnable round-trip guard that asserts
> the 12 OSLS element factories agree with the native registry, the status maps
> round-trip, and the typed bindings name a known record class). Every
> `lib/os-level-support` class now carries the Metadata Class metapattern; the
> `*Source` facades delegate their grouping terms to the backend catalogues
> instead of re-declaring them; `SLOsSupportNative` widens PID/epoch arguments to
> `long` to match the `int64_t` C ABI; and the previously unrunnable example is
> fixed (the reserved words `native` were renamed and multi-line `<` headers
> converted to `<<`/`>>` blocks so the whole package loads and runs). The header
> counts above are reconciled to the live `/lib` tree by
> `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`. See `os-level-support/README.md` and
> `lib/os-level-support/README.md`.

> Revision 0.55 appends **`OP_OSLS_CALL` (155)** to the canonical ISA and
> adds its `SLOpOslsCall` wrapper. The native VM dispatch now reaches the
> registered `osls_*` ABI table through typed operation IDs 0–11, with bounded
> JSON result marshalling. The opcode wrapper family now contains **156**
> `SLOp*` classes for codes **0–155**, one-for-one with the native enum and
> dispatch. The OSLS source facade and platform-specific source facades are
> indexed alongside the wrapper. See `opcodes/OPCODES.md` and
> `os-level-support/README.md`.

> Revision 0.53 adds the new **`sleeladoc`** package (`lib/sleeladoc/`): the
> SLeeLa-side **SleelaDoc extractor** — the Javadoc-tool analogue. It reads
> structured doc-comments out of a `.sleela` Wrapper and renders a Markdown or
> HTML API page, documenting *intent* (not just signatures). A SleelaDoc comment
> is an ordinary angle-bracket comment whose sentinel token is `@doc` (or
> `@page`), so it lexes today with no compiler change. Eight source Master
> Classes plus the facade: **`SLSleelaDoc`** (the façade a caller drives —
> `begin` → `feed` tokenized lines → `toMarkdown`/`toHtml`), **`SLDocScanner`**
> (the token-stream front end that lifts doc blocks and `@tags` and attaches each
> to the following declaration), **`SLDocRenderer`** (the Markdown/HTML back end),
> and the model **`SLDocModel`** / **`SLDocMember`** / **`SLDocComment`** /
> **`SLDocTag`**. Because the built-in String is opaque, the scanner reads
> tokenized lines (the char-cell boundary used across `lib/text`/`lib/xml`); node
> storage uses growable VM-managed object arrays. A runnable reference
> (`sleeladoc-demo.sleela`) feeds a small documented class and prints the rendered
> Markdown and HTML; `lib/sleeladoc/README.md` documents the surface, the
> convention lives in `manifesto/SLEELADOC.md`, and the compiler's own
> author-marked doc outputs (`--wiki` wikitext + the originated twin) are
> documented in `manifesto/SLEELADOC.WIKI.md`. Every class carries the Metadata
> Class metapattern.
>
> One new package (105 → **106**), one new facade (73 → **74**), eight new source
> classes (11654 → **11662** sources; 11727 → **11736** total records). Counts
> reconciled to the live `/lib` tree by `tools/generate-library-symbols.py` and
> verified by `test-suites/test-library-inventory.sh`.

> Revision 0.52 adds the new **`xml`** package (`lib/xml/`): a SLeeLa-side XML
> **Document Object Model (DOM)**. Eight source Master Classes plus the package
> facade. The DOM core — **`SLXmlDocument`** (the W3C `Document`: node factory
> `createElement`/`createTextNode`/…, root management, `getElementById`,
> `getElementsByTagName`, the XML declaration + serialization), **`SLXmlNode`**
> (the central node: ordered children, named attributes, `textContent`,
> `appendChild`/`removeChild`, depth/counts), **`SLXmlAttribute`** (a
> name/value attribute with namespace prefix/local parts), and **`SLXmlNodeType`**
> (the DOM `nodeType` vocabulary + walker predicates). The text boundary —
> **`SLXmlSerializer`** (DOM → well-formed XML, compact or pretty, with XML 1.0
> §2.4 character-data escaping) and **`SLXmlParser`** (a char-cell text → DOM
> reader for the common subset plus a structural well-formedness check). And
> **`SLXmlAspect`**, an executive-grade **aspect/quality register** that rates how
> fully the library addresses each XML aspect (well-formedness, DOM, namespaces,
> serialization, validity), each against its real governing W3C instrument, with
> honest 0..10 scores (unimplemented validity scores low, never inflated).
>
> Node equality is a three-rung **tie of trust**: **identity** (`sameReference`,
> native `==` on objects = same memory), **same subject** (`sameSubject`: same
> nature/name/id/value, case-sensitive, children ignored), and **structure**
> (`equals`/`sameStructure`: deep-equal subtrees — same subject, same attribute
> set, same children in order). `tieStrength`/`tieName` report the rung; every
> rung is reflexive, symmetric, and deterministic (idempotent). `SLXmlAttribute`
> and `SLXmlDocument` carry the matching ladder. Node storage uses growable
> VM-managed object arrays so a node's footprint tracks its real content. A
> runnable reference (`xml-dom.sleela`) builds, queries, serializes, round-trips,
> compares, and verifies the equality guarantees; `lib/xml/README.md` documents
> the surface. Every class carries the Metadata Class metapattern.
>
> One new package (104 → **105**), one new facade (72 → **73**), eight new source
> classes plus one upstream-reconciled source unit (11645 → **11654** sources;
> 11717 → **11727** total records). Counts reconciled to the live `/lib` tree by
> `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.51 adds two companion `core` classes expressing the standing of a
> work and its wins, both author-marked (syntax 2.4) and carrying the full
> Metadata Class metapattern. **`SLAuthorship`** is the authorship *Method :
> Class* — continuance (the work is carried forward), dignity (a modest,
> protected standing; an empty claim honestly becomes `"unverified"`), and free
> guidance (`imposes()` is false by contract). It carries any distinction as a
> cited historiometric *estimate* with provenance, never an asserted score, and
> ranks no person. **`SLVictory`** is the Victory Model: a *win* is a **check
> that resolves to an exact item** (exact equality against a declared item),
> measured against a named *calculus* (default: the U.S. reference standard,
> citing NIST), in the direction *Humanities → Science* — "means are goals and
> directions without worship elsewhere." Two new source classes; no new package
> or facade. Counts reconciled to the live `/lib` tree by
> `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.50 defines the **`philosophers`** vocabulary categorically at the
> IQ-201+ register. The twenty-one Guides are renamed and defined in seven
> ternary categories — **Admission** (Approach, Notice, Warrant), **Inception**
> (Patch, Seed, Frame), **Intent** (Aim, Course, Measure), **Vitality** (Health,
> Vigor, Pattern), **Governance** (Guide, Ministry, Counsel), **Standing**
> (Office, Saint, Favor), **Closure** (Impart, Conform, Approval) — in
> `SLPhilosopherSeating` (`SEAT_*`, `CAT_*`, `category()`/`categoryName()`). A
> new **`SLOrdain`** class adds **Ordain** as an *orthogonal* axis: the
> ordinality itself (rank, category, nearness to the ordaining crown Approval),
> which stands across all twenty-one rather than occupying a seat. The
> categorical terms are related to the Source Diagram and the Language Syntax in
> `lib/philosophers/PHILOSOPHERS.CATEGORICAL.md`, and the VIM/EMACS coloring
> (`lib/linux-editors/`) is updated to the new Guide names plus a distinct
> Ordain face. One new source class (`SLOrdain`); no new package or facade.
> Counts reconciled to the live `/lib` tree by
> `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.49 adds the new **`philosophers`** package (`lib/philosophers/`):
> a model of the **perfect Constructor** of procedural design and its Guide
> chain, declared with the `::` growth separator, e.g.
> `Constructor() :: Patch :: Constructor :: Aim :: Guide`. Six Master Classes —
> `SLPhilosopherSeating` (the finite register: base 21 seatings plus a signed
> band [-2, +6] → capacity [19, 27], the 21 named Approach → Approval),
> `SLPhilosopherGuide` (one `::`-separated growth / Wisdom, carrying a Gold or
> Green color impart for the VIM/EMACS coloring), `SLFavorMechanism` (catches
> the Notice of Approach at call time and grants/withholds runnability by
> quorum), `SLConstructorContract` (the instantiative moment of nature and
> time; the whistle only ticks ahead, ≤ ~2 min, then the object lives free),
> `SLPhilosopherChain` (the ordered Guides under the seating capacity), and
> `SLPhilosopherConstructor` (the orchestrator) — plus a runnable
> `philosophers-demo.sleela`. The companion editor coloring lives under
> `lib/linux-editors/` (`vim/syntax/sleela-philosophers.vim`,
> `vim/ftplugin/sleela-philosophers.vim`, `emacs/sleela-philosophers.el`),
> rendering the `::` separator and each Guide Gold/Yellow (approving) or Green
> (growing). This adds **one new package and one new facade** (100 → **101**
> and 69 → **70**) and 7 new source classes. The header counts above were also
> **reconciled to the live `/lib` tree** by `tools/generate-library-symbols.py`
> (the prior 0.48 header had drifted behind the filesystem); the counts now
> match `find lib -type f -name '*.sleela'` exactly and are verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.48 adds **per-airline brand sites** to the **`website`** package:
> one subdirectory per known modern airline under `lib/website/` (e.g.
> `piedmont-airlines/`, `delta-air-lines/`, …), each holding a self-contained,
> runnable Master Class (`<Airline>Website`) that builds a complete HTML site
> from the website model — a brand design (brand/base/accent + an energy dial)
> plus an ordered list of semantic signs (nav, hero, feature, testimonial, cta,
> footer). 45 airlines across North America, Europe, the Middle East,
> Asia-Pacific, Latin America, and Africa; generated by
> `lib/website/generate-airlines.py` and indexed in `lib/website/AIRLINES.md`.
> Nested subdirectories stay within the first-level `website` package, so this
> adds **no new package and no new facade** (both unchanged at 90 and 62); 45
> new source classes, 10,862 → **10,907** source classes and 10,924 →
> **10,969** total records. `LIBRARY.SYMBOLS.md` was regenerated from the live
> tree by `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.47 populates the **`gui`** package (`lib/gui/`) — previously only a
> facade — as the SLeeLa-facing *consequence* of the top-level `gui/` work (the
> Java host, the native C ABI bridge `gui/native/sleela_gui_bridge.{h,cpp}`, and
> the 1..14 document-change listener). Nine Master Classes mirror the `gui/`
> model: `SLGuiBackend` (toolkit-neutral backend selector), `SLGuiWindow`
> (window intent + the `refresh(id,revision)` document sink), `SLGuiAction`
> (named action with take-the-latch activation), `SLGuiDocument` (immutable
> watched-document snapshot with an OS revision fingerprint),
> `SLGuiDocumentListener` (the OS-driven 1..14 watch with revision coalescing),
> `SLGuiRuntime` (Path 2 — SLeeLa intent drives the window), `SLGuiHost`
> (Path 1 — a Java host ties actions/changes to SLeeLa operations), `SLGuiBridge`
> (Path 3 — the shared native ABI), and `SLGuiIntegration` (the recommended
> three-path posture), plus a runnable `gui-demo.sleela` and
> `gui/ARCHITECTURE.md`. The native bridge gains a `Makefile` and a `-Werror`
> round-trip test (`gui/native/slgui_bridge_test.cpp`) and a dedicated
> `.github/workflows/gui-ci.yml`. Ten new source classes in the existing `gui`
> package (packages and facades unchanged at 90 and 62); 10,852 → **10,862**
> source classes and 10,914 → **10,924** total records. `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree by `tools/generate-library-symbols.py` and
> verified by `test-suites/test-library-inventory.sh`.

> Revision 0.46 adds the **`boolean`** package (`lib/boolean/`): a
> Boolean/Binary **syllable-word** vocabulary. The two primitive syllables
> `BooleanTrue` (`BOOLEAN.TRUE`) and `BooleanFalse` (`BOOLEAN.FALSE`) compose
> into ordered **words** (`BOOLEAN.TRUE.FALSE`, `BOOLEAN.TRUE.FALSE.FALSE`, …)
> where the **HEAD** syllable is the *result* and every trailing syllable is a
> *carry* — a caveat structure that must be observed or agree. Depth 8 is the
> **full enumeration** of all 2⁸ = 256 words, one class per file under
> `boolean/8/` (`Bool_<glyphs>.sleela`, names coherent to their value: `T`=TRUE,
> `F`=FALSE). Depths 16 and 32 ship the generative word factories
> `BooleanWord16` / `BooleanWord32` plus canonical boundary words under
> `boolean/16/` and `boolean/32/`; full enumeration (2¹⁶ = 65,536 and
> 2³² = 4,294,967,296 files) is not materialized — any such word is constructed,
> validated, and read on demand with the same result/caveat semantics. See
> `boolean/ARCHITECTURE.md` and the runnable `boolean/boolean-demo.sleela`. This
> revision also **reconciles the manifest to the live tree**: 29 previously
> unrecorded `codecs`/`video` codec source units (already present on disk) are
> now counted. One new package (89 → **90**), one new facade (61 → **62**);
> 10,554 → **10,852** source classes and 10,615 → **10,914** total records.
> `LIBRARY.SYMBOLS.md` was regenerated from the live tree by
> `tools/generate-library-symbols.py` and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.45 improves the **`alphabet`** package for **Sleela 1.8+**. `Letter`
> and `Alphabet` are upgraded to `#sleela 1.8` and gain the two 1.8 ergonomics
> without breaking the classic path: `Letter` now offers a one-line constructor
> `new Letter(glyph, order, vowel)` (the load()+set() two-step and zero-arg
> `new Letter()` used by `Alphabet.append()` are retained), and `Alphabet` gains
> `appendLetter(Letter)` to accept constructor-built glyphs. A new
> subclass-free **`AlphabetBuilder`** assembles a custom writing system inline
> with `let` + Letter constructors. Two new scripts broaden coverage —
> **`HebrewAlphabet`** and **`ArabicAlphabet`** (both abjads) — alongside Latin,
> Greek, and Cyrillic. Three new source units (10,612 → **10,615** total
> records; 10,551 → **10,554** source classes); packages and facades unchanged
> at 89 and 61. A runnable 1.8 demonstrator lives at
> `alphabet/sleela/AlphabetDemo18.sleela` (outside `/lib`, so not counted here).
> `LIBRARY.SYMBOLS.md` was regenerated from the live tree and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.44 extends the **Moral Capacitor™** family with a **Certificate of
> Morals™** and a **Guard of the Moral Code**, putting the moral code — with a
> warranty — into our products. Six new source units: `SLMoralWarranty` (the
> warranty terms), `SLMoralCertificate` (the signed, sealed attestation that a
> product embeds and enforces the capacitor), `SLMoralCertificateAuthority`
> (issues/verifies certificates), `SLMoralCodeGuard` (admits a product only when
> a valid certificate + live capacitor are present), `SLMoralCertified` (the
> one-object certify-a-product convention), and the runnable demonstrator
> `moral-certificate.sleela`. The VM/OS/Machine adapters each gained
> `certify(...)` + `admit(...)` so they ship as morally certified products.
> 10,606 → **10,612** total records (10,545 → **10,551** source classes);
> packages and facades unchanged at 89 and 61. `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree and verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.43 adds the **Moral Capacitor™** (`moral-capacitor/`): a uniform
> ethical gate every `/lib` library can place in front of a consequential
> operation. It accumulates moral **charge** from ethical frameworks
> (deontology, utilitarianism, virtue ethics, beneficence, consent) and only
> **discharges — permits the operation — when charge meets a policy threshold and
> no absolute red line is tripped** (`SLMoralCapacitor`, `SLMoralCharge`,
> `SLMoralVeto`, `SLMoralFramework`, `SLMoralVerdict`, `SLMoralPolicy`,
> `SLMoralOperation`, `SLMoralLedger`). It is placed first and foremost in front
> of the VM, OS, and Machine via `SLVMMoralGuard`, `SLOSMoralGate`, and
> `SLMachineMoralGate`, and ships a runnable demonstrator `moral-capacitor.sleela`
> and `moral-capacitor/ARCHITECTURE.md`. One new package (89), thirteen new source
> units of which one is the package facade (10,593 → **10,606** total records;
> 10,533 → **10,545** source classes; 60 → **61** facades). This revision also
> reconciles the header counts and `test-suites/test-library-inventory.sh`
> expectations with the live tree. `LIBRARY.SYMBOLS.md` was regenerated from the
> live tree and verified by `test-suites/test-library-inventory.sh`.

> Revision 0.42 adds a **flow/layout manager** to the `user-interface` package.
> Two new one-class-per-file Master Classes: `SLMeasure` (a standardized unit of
> measurement across US customary — in/ft/pt/pica — Eurasian metric — mm/cm/m —
> device px, and relative %/fr/em, resolving to pixels at a DPI) and `SLLayout`
> (the manager: a UI as an orthogonal conflation of unit parts and
> organizations, with named **groups** of named **items** adjusted singly, by
> group, or **n-ary**; several flow solutions — stack, wrap, grid, dock, and
> central weight/mass; Named placement where **MEDIUM** implies center and
> **CENTRAL** implies weight/mass; and named ergonomic presets for the
> ergonomics of publics). Each bottoms out in `measure*`/`layout*` SLVM built-ins
> calling the genuine C ABI (`sleela_ui_layout.h`); see
> `user-interface/LAYOUT.md`. Two new source units: 10,463 -> **10,465** total
> records (10,403 -> **10,405** source classes); facade and package counts
> unchanged at 60 and 88. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.41 adds **moods, washes, a Calculus-8, and millimetre light
> placement** to the `user-interface` package. Four new one-class-per-file
> Master Classes: `SLMm` (place a light/dark/shadow emitter in millimetres
> relative to the text's font-paint location — a HEIGHT it shines down from plus
> left/right/up/down, resolved to pixels at a DPI), `SLCalculus8` (a careful
> 8-stage calculus over **100 differentiable** inputs that maps a light's control
> points into a mood scalar, with an analytic d(mood)/d(input) slope),
> `SLWash` (an Excellent Wash — a soft multi-stop graded colour field), and
> `SLMood` (a named mood = wash + a natural tanor glow for lightless bulbs, with
> a refresh and an "a little left" bias, + a Calculus-8; colours and places
> lights in one call). Each bottoms out in `mm*`/`calc8*`/`wash*`/`mood*` SLVM
> built-ins calling the genuine C ABI (`sleela_ui_mood.h`); see
> `user-interface/MOODS.md`. Four new source units: 10,459 -> **10,463** total
> records (10,399 -> **10,403** source classes); facade and package counts
> unchanged at 60 and 88. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.40 adds the new `gameplay` package — a turn/move system where a
> player makes one of three moves per turn, each resolved by deterministic,
> seeded rolls (so a replayed game, luck included, repeats). Thirteen
> one-class-per-file Master Classes. Core: `DiceRoll` (a seeded MINSTD roller
> giving the 80/20 and 50/50 rolls), `Player` (experience, Person/Citizen
> quality that sets information price, cash, boss understanding),
> `ManagementCourtroom` (banked file/tact/rolls and standing), `MoveOutcome`
> (the result record a move returns and applies to the player), and `GameTurn`
> (the orchestrator). Move (b)'s brain: `InferenceEngine` with `ExpectedFuture`
> (the on-schedule projection on a winning 50/50) and `DetailedReport` (the
> boss's pre-decided Game-Engineering moves on a losing 50/50, priced by player
> quality). Move (c)'s substance: `PortfolioDecision` (a medium bet on a
> realistic industry; a found BIG bet always wins). The three moves subclass a
> `GameMove` base: `HelpIndexMove` (a — help index/friend, 80% then 20% with
> live subevents), `AskForHelpMove` (b — ask for help/new routes, 50/50), and
> `NewDesireMove` (c — create a new desire, review portfolios for profits and
> futures). The base/subclass + override pattern mirrors lib/compiler's
> `SLLanguageCompiler` front ends. A matching `SLPackage` facade names the
> package, and a runnable orchestration example lives outside `/lib` at
> `gameplay/sleela/GameplayDemo.sleela` (so it is not double-counted). One new
> package and fourteen new `.sleela` units: 10,445 -> **10,459** total records
> (10,386 -> **10,399** source classes), 59 -> **60** facades, 87 -> **88**
> packages. `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the
> counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.39 adds **fonts and font effects** to the `user-interface` package.
> Two new one-class-per-file Master Classes: `SLFont` (a face — family, size,
> weight, slant, spacing, line height — plus an ordered effect stack and a
> quality level draft..ultra; draws into an `SLDrawContext` and attaches to a
> text widget via `SLWidget.setFont`) and `SLFontEffect` (one effect: drop/inner
> shadow, glow, directional light sheen, an EMITTER that radiates the text into
> a light scene — a pure emitter reserving nothing, or a source binding an
> anchor — outline, relief emboss/engrave, or gradient fill). Each bottoms out in
> `font*`/`ui*` SLVM built-ins calling the genuine SleelaUI Font C ABI
> (`sleela_ui_font.h`); see `user-interface/FONTS.md`. Two new source units:
> 10,443 -> **10,445** total records (10,384 -> **10,386** source classes);
> facade and package counts unchanged at 59 and 87. `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.38 adds the new `alphabet` package — colors, themes, and alphabets,
> thematically important to all users of Sleela. It introduces eleven
> one-class-per-file Master Classes. The color/theme family: `Color` (one RGB
> color with hex, blending, and luminance), `Palette` (an ordered, labeled set
> of Colors), `Theme` (the base that binds a Palette + role colors
> background/foreground/accent + an Alphabet into one user-facing look), and
> three concrete themes `LightTheme`, `DarkTheme`, and `SolarizedTheme`. The
> writing-system family: `Letter` (one glyph with order and vowel flag),
> `Alphabet` (the base writing system a developer extends by overriding
> `compose()`), and three concrete scripts `LatinAlphabet` (26), `GreekAlphabet`
> (24), and `CyrillicAlphabet` (33). The base/subclass + override pattern mirrors
> lib/compiler's `SLLanguageCompiler` front ends. A matching `SLPackage` facade
> names the package, and a runnable orchestration example lives outside `/lib`
> at `alphabet/sleela/AlphabetDemo.sleela` (so it is not double-counted). One new
> package and twelve new `.sleela` units: 10,431 -> **10,443** total records
> (10,373 -> **10,384** source classes), 58 -> **59** facades, 86 -> **87**
> packages. `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the
> counts are verified in lockstep by `test-suites/test-library-inventory.sh`.

> Revision 0.37 sets Sleela's UI **base colour to #2B1608** (a deep warm umber;
> the default `SLUI_THEME_SLEELA_BASE`, configurable at code or config time, with
> the whole warm palette derived from the one base) and adds a **lighting /
> shadow / relief** system for Canvas-aware objects. Three new one-class-per-file
> Master Classes in `user-interface`: `SLLight` (a light descriptor with a KIND,
> a ROLE — SOURCE reserves its anchor object, EMITTER reserves nothing — and a
> POLARITY so both light and shadow are first-class emissions), `SLLightScene`
> (a scene owning the source-reservation table and ambient fill), and
> `SLMaterial` (a relief profile — flat/rounded/bevel/engraved/embossed — plus
> depth, gloss, and occlusion). `SLTheme` gains base-colour hooks (setBaseColor /
> loadConfig). Each bottoms out in `ui*`/`light*` SLVM built-ins calling the
> genuine C ABI (`sleela_ui_light.h`); see `user-interface/LIGHTING.md` and
> `user-interface/THEMING.md`. Three new source units: 10,428 -> **10,431** total
> records (10,370 -> **10,373** source classes); facade and package counts
> unchanged at 58 and 86. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.36 adds motion and a comprehensive drawing surface to the
> **`user-interface`** package: four new one-class-per-file Master Classes.
> `SLThrobber` is a width-adjustable, full-motion, colour-predictive, water-like
> flowing activity field (a fluid shallow-water crest field whose hue leads the
> flow; see `user-interface/THROBBER.md`). `SLCanvasView` is a general animated
> drawing surface whose per-frame draw routine paints with the Draw API.
> `SLDrawContext` binds the comprehensive SleelaUI Draw API
> (`user-interface/include/sleela_ui_draw.h`): owned RGBA surfaces, single/double
> buffering, pixel-level get/set/blend, blend modes, a clip stack, and the full
> primitive set (rects, rounded rects, lines, circles, ellipses, arcs,
> triangles, gradients, text, blits). `SLFrameClock` binds the refresh-rate
> controller (target FPS, delta time, measured rate, sleep hint, fixed-step).
> Each bottoms out in `ui*`/`draw*` SLVM built-ins calling the genuine C ABI;
> see `user-interface/DRAW-API.md`. Four new source units: 10,424 -> **10,428**
> total records (10,366 -> **10,370** source classes); facade and package counts
> unchanged at 58 and 86. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.35 expands the **`user-interface`** package with the full SleelaUI™
> widget collection: twenty-two new one-class-per-file Master Classes binding
> the toolkit's expanded native widget set. Selection/boolean: `SLCheckBox`,
> `SLRadioButton`, `SLComboBox`, `SLSpinButton`. Indicators: `SLProgressBar`,
> `SLLevelBar`, `SLSpinner`, `SLScrollBar`. Containers: `SLFrame`, `SLCard`,
> `SLGrid`, `SLStatusBar`. Display/feedback: `SLImage`, `SLAvatar`, `SLBadge`,
> `SLChip`, `SLHeading`, `SLTooltip`, `SLInfoBar`. Text variants: `SLLinkButton`,
> `SLSearchEntry`, `SLPasswordEntry`. Each bottoms out in a `ui*` SLVM built-in
> calling the SleelaUI C ABI (`user-interface/include/sleela_ui.h`); the full
> catalogue is tabulated in `user-interface/WIDGETS.DESCRIPTION.md` and the class
> roster + `ui*` surface in `lib/user-interface/USER-INTERFACE.md`. Twenty-two
> new source units: 10,402 -> **10,424** total records (10,344 -> **10,366**
> source classes); facade and package counts unchanged at 58 and 86.
> `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the counts are
> verified in lockstep by `test-suites/test-library-inventory.sh`.

> Revision 0.34 adds the **`user-interface`** package: the SLeeLa-source binding
> for **SleelaUI™**, SLeeLa's own original cross-platform UI toolkit (native
> toolkit under `/user-interface`; not GTK/Qt). It gains fifteen one-class-per-
> file Master Classes — the capstone `SLUserInterface`, `SLWindow`, the widget
> base `SLWidget`, containers `SLBox`/`SLHeaderBar`, controls `SLLabel`/
> `SLButton`/`SLToggle`/`SLEntry`/`SLSlider`/`SLSeparator`/`SLSpacer`, the
> palette pair `SLTheme`/`SLColor` (default **Slick Black**), and the runnable
> `SLGalleryDemo` — plus the `SLPackage` facade. Every method bottoms out in a
> `ui*` SLVM built-in that calls the genuine SleelaUI C ABI
> (`user-interface/include/sleela_ui.h`), which drives X11 / Cocoa / Win32; the
> `ui*` surface and the class roster are documented in
> `lib/user-interface/USER-INTERFACE.md`. Sixteen new source units (1 facade +
> 15 sources): 10,386 -> **10,402** total records (10,329 -> **10,344** source
> classes), 57 -> **58** facades, 85 -> **86** packages. `LIBRARY.SYMBOLS.md`
> was regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.33 copies the fourteen `character` business-model Master Classes
> into a new nested sub-area, `lib/citizen/character/`, so the `citizen` package
> carries its own in-place copy of `Character`, `BusinessModel`, and the twelve
> concrete models (`SubscriptionModel`, `FreemiumModel`, `MarketplaceModel`,
> `AdvertisingModel`, `RetailModel`, `WholesaleModel`, `FranchiseModel`,
> `LicensingModel`, `SaaSModel`, `ConsultingModel`, `ManufacturingModel`,
> `BrokerageModel`). The sources are unchanged apart from their header path
> comments. Following the one-facade-per-package convention (see
> `lib/compiler/frontends/*`, which hold source classes but no nested
> `SLPackage`), the copied `SLPackage` facade was NOT duplicated into the
> sub-area — the `citizen` package keeps its single root facade. Fourteen new
> source units, all attributed to the `citizen` package: 10,372 -> **10,386**
> total records (10,315 -> **10,329** source classes); facade and package counts
> unchanged at 57 and 85. `LIBRARY.SYMBOLS.md` was regenerated from the live
> tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.32 adds two new packages — `citizen` and `character` — together.
>
> The `citizen` package is the civic layer of the Republic of Sleela. It
> introduces four one-class-per-file Master Classes: `Citizen` (one honest,
> taxpaying, coffee-drinking person who is, famously, able), `Industry` (their
> place across the whole economy, sector by sector), `BankAccount` (the banking
> relationship, with a 9-digit ABA routing number and account number), and
> `FederalReserveID` (the central-bank clearing identity that maps money to one
> of the twelve Federal Reserve Districts). A runnable example lives outside
> `/lib` at `citizen/sleela/CitizenDemo.sleela`.
>
> The `character` package is a classful study in business models. It introduces
> fourteen one-class-per-file Master Classes: `Character` (a named actor who
> adopts exactly one model and lives or dies by its monthly profit),
> `BusinessModel` (the language-neutral base contract a developer extends,
> following the same base/subclass pattern as lib/compiler's
> `SLLanguageCompiler` front ends), and twelve concrete models that each override
> the revenue/cost math — `SubscriptionModel`, `FreemiumModel`,
> `MarketplaceModel`, `AdvertisingModel`, `RetailModel`, `WholesaleModel`,
> `FranchiseModel`, `LicensingModel`, `SaaSModel`, `ConsultingModel`,
> `ManufacturingModel`, and `BrokerageModel`. A `Character` holds its model
> through a single base-typed reference, so any one of the twelve plugs in
> interchangeably and answers the same question — did we make money? — with
> entirely different arithmetic. A runnable example lives outside `/lib` at
> `character/sleela/CharacterDemo.sleela`.
>
> Each package ships a matching `SLPackage` facade, and the runnable examples
> live outside `/lib` so they are not double-counted in the inventory. Two new
> packages and twenty new `.sleela` units: 10,352 -> **10,372** total records
> (10,297 -> **10,315** source classes), 55 -> **57** facades, 83 -> **85**
> packages. `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the
> counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.31 promotes the emblematic Skya telephony modules into first-class
> `/lib` Master Classes. The `telephony-skya` package gains six one-class-per-
> file source units — `Socio` (social fabric), `Network` (transport
> reachability), `Servers` (server-side presence), `Communication` (message
> exchange), `RealAcquaintances` (confirmed trust roster), and the `SkyaModules`
> loader — so they are counted and discoverable as Master Classes rather than
> living only as facade stubs or top-level runnables. The matching facade stubs
> were removed from `telephony-skya/SLPackage.sleela` to avoid duplicate class
> declarations. Six new source units: 10,346 -> **10,352** total records (10,291
> -> **10,297** source classes); package count unchanged at 83.
> `LIBRARY.SYMBOLS.md` was regenerated from the live tree and the counts are
> verified in lockstep by `test-suites/test-library-inventory.sh`.

> Revision 0.30 reconciles the `/lib` inventory to the live tree after the
> **modular multi-language compiler framework** and the operating-system
> system-call work. The `compiler` package gains the language-neutral framework
> (`SLLanguageCompiler`, `SLCompileRequest`, `SLCompilePlan`,
> `SLCompilerRegistry`) and seven modular front ends under `frontends/<lang>/`
> (C, C++, Java, Python, JavaScript, Rust, Go), each an independent front end
> that lowers its own language toward the common SLeeLa IR; see
> `lib/compiler/MULTI-LANGUAGE.FRAMEWORK.md`. This revision also folds in four
> previously-uncounted units (`operating-system/SLLinuxOS`, `operating-system/SLMacOS`, `operating-system/SLWindowsOS`,
> `vm/SleelaVMSystemCallBridge`). Sixteen `.sleela` units in total: 10,330 ->
> **10,346** total records (10,275 -> **10,291** source classes); package count
> unchanged at 83. `LIBRARY.SYMBOLS.md` was regenerated from the live tree
> (`collection-revision: 2.1`) and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.29 adds `SLGuestExecutable`: a simple on-disk guest executable
> format (a miniature ELF - magic header + entry + opcode section). A compiled
> program serializes to it, is written to the guest filesystem, and is loaded
> back FROM DISK into the substrate (header validated) - so guest programs come
> from storage, not only inline compilation. SLGuestInit gains execFromImage()/
> installAndExec() (compile -> install -> load-from-disk -> run). 10,274 ->
> **10,275** source classes (**10,330** total). See `cpu/EXECUTION.md`.

> Revision 0.28 adds `SLGuestShell`: a minimal interactive shell spawned by
> init as its first child (builtins help/echo/ls/cat/ps/exit), issuing every
> command through the guest syscall ABI (console write, file read, getpid,
> exit). It is the guest's first interactive user program and exercises the full
> user->kernel boundary. 10,273 -> **10,274** source classes (**10,329** total).
> See `cpu/EXECUTION.md`.

> Revision 0.27 adds guest IPC (2 classes): `SLGuestPipe` (a unidirectional
> byte FIFO between tasks with would-block/EOF semantics) and `SLGuestSignal`
> (asynchronous signal delivery with per-task pending sets; default actions for
> SIGKILL/SIGTERM terminate the target). SLGuestSyscall gains pipe()/kill() plus
> pipe-fd read/write routing; SLGuestVM delivers pending signals at scheduling
> points. 10,271 -> **10,273** source classes (**10,328** total). See
> `cpu/EXECUTION.md`.

> Revision 0.26 adds `SLGuestSyscall`: the guest kernel's syscall ABI +
> dispatcher (write/read/open/close/brk/yield/getpid/exit). Guest service-opcode
> VM-exits are now routed through it by SLGuestVM.handleExit() and emulated
> against the guest console, filesystem, and scheduler, with the result pushed
> back for the guest to resume (SYS_EXIT finishes the running task). This
> completes the user->kernel boundary. 10,270 -> **10,271** source classes
> (**10,326** total). See `cpu/EXECUTION.md`.

> Revision 0.25 adds `SLGuestFileSystem`: a minimal in-guest filesystem over
> the guest block device - a flat directory of files and an open-file-descriptor
> table with sequential read/write, mounted by SLGuestLinux on the block device.
> It backs the guest open/read/write/close syscalls. 10,269 -> **10,270** source
> classes (**10,325** total). See `cpu/EXECUTION.md`.

> Revision 0.24 adds guest user-space bring-up (2 classes): `SLGuestProgram`
> (a loadable guest *user* program compiled from source via the shared toolchain)
> and `SLGuestInit` (the guest init process, PID 1, which loads programs into the
> guest substrate and admits them as scheduler tasks). The guest now goes from
> "kernel running" to "running user processes" that the timer-driven scheduler
> multitasks. 10,267 -> **10,269** source classes (**10,324** total). See
> `cpu/EXECUTION.md`.

> Revision 0.23 completes the guest OS-VM run loop and multitasking (OS-VM
> roadmap steps 5-6). Step 5 (code changes only) wires VM-exit routing: hosted
> SLSleelaVM parks service opcodes as VM-exits that SLGuestVM routes to the
> device model and resumes. Step 6 adds 2 classes - `SLGuestTask` and
> `SLGuestScheduler` - giving the guest kernel timer-driven round-robin
> preemptive multitasking (the SLGuestTimer IRQ is the scheduler tick; task
> context = saved substrate IP). 10,265 -> **10,267** source classes (**10,322**
> total). See `cpu/EXECUTION.md`.

> Revision 0.22 lets the guest OS-VM boot a GENUINELY COMPILED kernel (2
> classes): `SLGuestKernelImage` (a bootable opcode-stream image + entry) and
> `SLGuestKernelBuilder` (compiles kernel source in any supported language
> through SLCompilerDriver -> register ISA -> SLOpcodeMap -> opcode image). The
> guest substrate loads and executes the compiled image instead of a hand-
> written stub (OS-VM roadmap step 4; a custom kernel, not upstream Linux).
> 10,263 -> **10,265** source classes (**10,320** total). See `cpu/EXECUTION.md`.

> Revision 0.21 adds a guest device model to the `cpu` package (5 classes):
> `SLGuestDevice` (MMIO base), `SLGuestTimer` (programmable interval timer +
> IRQ), `SLGuestConsole` (serial/console output), `SLGuestBlockDevice`
> (virtio-blk-style storage over a host drive), and `SLGuestDeviceBus` (MMIO
> routing + IRQ aggregation into the guest interrupt controller). The guest
> OS-VM now has the timer/console/block surface a kernel needs (OS-VM roadmap
> step 3). 10,258 -> **10,263** source classes (**10,318** total). See
> `cpu/EXECUTION.md`.

> Revision 0.20 adds `SLMMU` to the `cpu` package: a Memory Management Unit with
> single-level page tables, per-page present/write/exec permissions, and page
> faults (unmapped + protection), giving the guest OS-VM real second-level
> address translation (OS-VM roadmap step 2). 10,257 -> **10,258** source
> classes (**10,313** total). See `cpu/EXECUTION.md`.

> Revision 0.19 maps the `cpu` model onto the canonical SLeeLa opcode substrate
> (4 new classes): `SLSleelaOpcode` (the base-98 Turing-complete opcode registry
> with exact native codes), `SLSleelaVM` (a stack machine executing those
> opcodes directly, mirroring the native dispatch loop), `SLOpcodeMap` (maps the
> register ISA and OS system calls onto Sleela-opcode sequences), and
> `SLTuringBridge` (reduces any-language programs to the substrate and proves
> C/C++/Java/Sleela are 1:1 in Turing effect). This takes the collection from
> 10,253 to **10,257** source classes (**10,312** total records); package count
> unchanged at 83. See `cpu/EXECUTION.md` §4. Counts verified by
> `test-suites/test-library-inventory.sh`.

> Revision 0.18 expands the `cpu` package with a real multi-language toolchain
> and a virtualization layer (13 new classes): a hardware stack (`SLStack`) and
> an expanded `SLInstructionSet`/`SLControlUnit` with a full CALL/RET/PUSH/POP
> protocol and MUL/DIV/MOD; a common IR (`SLIRInstruction`, `SLIR`), a shared
> backend (`SLLowering`), language frontends (`SLFrontend`, `SLFrontendSleela`,
> `SLFrontendC`, `SLFrontendCpp`, `SLFrontendJava`), and a `SLCompilerDriver` so
> the ISA accepts C/C++/Java/Sleela; plus a hosted hypervisor (`SLHypervisor`,
> `SLGuestVM`, `SLGuestLinux`) modeling a Linux-style guest on the Sleela CPU.
> This takes the collection from 10,240 to **10,253** source classes (**10,308**
> total records); package count is unchanged at 83. See `cpu/EXECUTION.md`.
> Counts are verified by `test-suites/test-library-inventory.sh`.

> Revision 0.17 adds the new `cpu` package family — a complete CPU, operating
> system, and program stack in SLeeLa source (45 classes): logic gates, adders,
> an ALU, latches/flip-flops/registers/register file, RAM/cache/hard drive,
> clock/bus/program counter, an instruction set + decoder + control unit,
> interrupt controller, multi-core `SLCPU`, I/O ports/bus/DMA and the device
> driver hierarchy, and the OS layer (bootloader, memory manager, scheduler,
> processes, syscalls, filesystem, kernel) up to `SLProgram`/`SLAssembler`/
> `SLProgramLoader` and the capstone `SLMachine`. This takes the collection from
> 82 to **83** package families and from 10,195 to **10,240** source classes
> (**10,295** total records). See `cpu/CPU.md`. `LIBRARY.SYMBOLS.md` was
> regenerated and the counts are verified by `test-suites/test-library-inventory.sh`.

> Revision 0.16 adds the national-grade cryptography façades to the `crypto`
> package family — `SLNationalSuite`, `SLSha256`, `SLSha512`, `SLSha3`, `SLAes`,
> `SLAesGcm`, `SLHkdf`, `SLMlKem`, and `SLMlDsa` — the SLeeLa-layer front ends
> for the native suite implemented under `/crypto` (NSA CNSA / NIST FIPS: AES,
> SHA-2/SHA-3, AES-GCM, HMAC, HKDF, and the ML-KEM/ML-DSA post-quantum
> interfaces). Nine new source units take the collection from 10,186 to **10,195**
> source classes (**10,250** total records). `LIBRARY.SYMBOLS.md` was
> regenerated from the live tree and the counts are verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.15 fully regenerates `LIBRARY.SYMBOLS.md` from the live `/lib`
> tree via `tools/generate-library-symbols.py` (one row per `.sleela` unit)
> and reconciles every count to the filesystem: **82** package families,
> **10,186** source classes + **55** `SLPackage.sleela` module facades =
> **10,241** total symbol records. This picks up the package families that had
> been added since the manifest was last written — `autocad`, `java`,
> `languages`, `opcodes`, `sldocument`, and `website` — and corrects the earlier
> internally inconsistent header (which claimed 10,054 sources / 90 facades over
> a 982-row body). The manifest body and header are now verified in lockstep by
> `test-suites/test-library-inventory.sh`.

> Revision 0.14 reconciled the headline source-unit figure to the verified
> filesystem count of 10,241 `.sleela` units under `/lib` (matching
> `CLASS.INVENTORY.md` Revision 2.1). Earlier revision stamps lagged the live
> count; the figure here is the authoritative total.

> Revision 0.9 adds the new `opcodes` package family: one SLeeLa class per
> canonical VM opcode (103 classes, codes 0–102; the base 98 are OP_NOP..
> OP_AUDIO_PLATFORM) plus `SLOpcodeBase` and `SLOpcodeStream` — 105 `.sleela`
> source units. Each class carries a single opcode and honours the fetch-then-
> execute-one contract against the VM. See `opcodes/OPCODES.md`.
>
> Revision 0.10 adds the `opcodes/governance` series — 8 classes giving SLeeLa
> procedural discretion over opcode execution: a Registrar considers a program
> A→B BEFORE it runs, a Listener confirms live fit DURING, and an Event Observer
> judges the whole as a musical, ordered process AFTER (with graded verdicts,
> base-concept checks, and known-symbol-map patching). See
> `opcodes/governance/GOVERNANCE.md`.
>
> Revision 0.11 adds the `opcodes/running` sub-family — 6 classes for richer
> opcode execution: grouping (`SLOpcodeGroup`, `SLOpcodeGroupSet`), a
> conditional-reactive layer (`SLOpcodeCondition`, `SLOpcodeConditionalReactive`,
> `SLOpcodeReactorBank`) that warms/gates/runs/skips groups on program state, and
> warming (`SLOpcodeWarmer`). See `opcodes/running/RUNNING.md`.
>
> Revision 0.12 adds the new `sldocument` package family and the `.sldocument`
> format — 6 classes for an ordered, top-down document that compiles against and
> with standard SLeeLa source (`SLDocument`, `SLDocumentStep`,
> `SLDocumentAnnotation`, `SLVeritable`, `SLDocumentResult`,
> `SLDocumentCompiler`). Each step is an annotated method that runs in order and
> usually returns a single binary veritable-and-kind value. See
> `sldocument/SLDOCUMENT.md`.
>
> Revision 0.13 makes `.sldocument` a selectable compile choice and adds naming
> conventions for comparing/converting the forms: `lib/compiler/SLSourceForm` and
> `lib/compiler/SLCompileChoice` let the compiler be told to compile a `.sleela`
> or a `.sldocument`; and `sldocument/SLDocumentNaming`,
> `sldocument/SLSourceNameComparison`, and `sldocument/SLDocumentConverter`
> synthesize method names for anonymous document steps so an engineer can convert
> a `.sldocument` to a named `.sleela` for safekeeping. 5 new `.sleela` classes.

The `/lib` tree is the canonical language-facing source collection. The compiler and Nordshrift use the same recursive library discovery implementation, so a package becomes importable when its directory contains SLeeLa source.

| Coverage | Count |
|---|---:|
| Repository module families represented under /lib | 83 |
| SLeeLa source units | 10,297 |
| Module-facade symbols | 55 |
| Total symbol records | 10,352 |

Every repository-level module family that is a language/runtime/package concern now has at least one SLeeLa source unit under `/lib`. Documentation, images, generated build output, tests, and CI-only directories remain non-library artifacts and are intentionally not presented as language packages.

## Compiler and Loader Resolution

Resolution is shared by the SLeeLa compiler and Nordshrift:

1. `$SLEELA_LIB`
2. `lib`
3. `../lib`
4. `../../lib`

Discovery is recursive. Package presence is therefore derived from the actual `/lib` tree rather than a hand-maintained package allow-list. `validateImports()` rejects an imported package that is not present.

The VM-facing `SLVMModuleLoader` source mirrors this contract: package names are discovered from the canonical library root, registered, and checked before use. Native loader facilities remain below the explicit C/C++ OS bridge.

## Inventory

The complete path-level and facade-level symbol collection is maintained in `LIBRARY.SYMBOLS.md`.

**Max Rupplin — MEARVK LLC — 2026**


## Compiler / Nordshrift / Loader Contract

The shared `sleela::library::Index` recursively discovers the 83 package families and all 10,352 `.sleela` source units. It now exposes package counts, per-package symbol counts, symbol lookup, and source-path resolution. Compiler and Nordshrift use this index; `lib/vm/SLVMModuleLoader.sleela` represents the same discovered package/symbol state at the SLeeLa layer.

## SST / Nordshrift Symbol Contract

SST declarations are typed symbols with package-qualified identity. Nordshrift resolves them through the same `sleela::library::Index` used by the compiler; `/lib` remains the sole authoritative symbol collection. See `impl/nordshrift/SST.SYMBOLS.md` and `impl/nordshrift/NORDSHRIFT.SYMBOLS.md`.
