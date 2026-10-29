<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Standard Library Front End

The `lib/` tree is the unified SLeeLa source layer for the language-facing SDK and VM/OS object model.

SLeeLa is treated as a Turing-complete language whose front end should be expressed in SLeeLa wherever the language can carry the contract. Native C/C++ remains below explicit VM/OS bridge boundaries for facilities requiring kernel, device, memory, networking, cryptography, or process primitives.

The library uses one SLeeLa source file per front-end object. This makes the object inventory measurable and gives the project a path toward a roughly 2,000-object standard library without hiding declarations inside aggregate files.

Families: `core/`, `collections/`, `text/`, `xml/`, `sleeladoc/`, `io/`, `vm/`, `os/`, `net/`, `wifi/`, `security/`, `opcodes/`, `sldocument/`, `coorenagraph/`, `moral-capacitor/`.

### Moral Capacitor™ — the ethical gate for every family

The `moral-capacitor/` family provides the **Moral Capacitor™**: a small, uniform
ethical gate any `/lib` class can place in front of a consequential operation. It
**accumulates moral charge** from ethical frameworks (deontology, utilitarianism,
virtue ethics, beneficence, consent) and only **discharges — permits the
operation — when stored charge meets a policy threshold and no absolute red line
is tripped** (child safety, weapons/CBRN, non-consensual surveillance, deception,
scaled abuse, irreversible harm without consent). It is intended for all `/lib`
libraries and is placed first and foremost in front of the **VM**, **OS**, and
**Machine** surfaces via `SLVMMoralGuard`, `SLOSMoralGate`, and
`SLMachineMoralGate`. It refines the governance `ETHICS_NORMS` axis and draws its
vocabulary from `coorenagraph`.

The family also puts the moral code **into the products** with a warranty: the
**Certificate of Morals™** (`SLMoralCertificate`, `SLMoralWarranty`,
`SLMoralCertificateAuthority`) is a signed attestation — travelling with a SLeeLa
software unit — that the unit embeds the Moral Capacitor™ and enforces it; and
the **Guard of the Moral Code** (`SLMoralCodeGuard`) admits a product to run or
ship only when that certificate is present, authentic, unexpired, and consistent
with the embedded capacitor. Embed one `SLMoralCertified` to make a product both
gated and certified; the VM, OS, and Machine adapters each gained `certify(...)`
and `admit(...)`. The project strives to certify **all** SLeeLa software this
way. See `moral-capacitor/ARCHITECTURE.md`.

The standard-library target is **2,048 object types**. This is an architectural target, not a claim that all 2,048 objects are implemented today.

**SLeeLa — MEARVK LLC — 2026**

## Cross-platform support — Windows 10+, macOS, and Linux

Every `/lib` package runs on **Windows 10+**, **macOS** (Darwin), and **Linux**.
This is the project's best offer to customers: one SLeeLa source library, the
same on all three desktop platforms.

Portability is structural, not per-package. A `/lib` class is either pure SLeeLa
(inherently portable) or it crosses the **explicit VM/OS bridge**, and that
bridge resolves to the genuine host System API for whatever OS the program runs
on — the OS-aware abstraction layer in [`impl/core`](../impl/core) with POSIX
(Linux/macOS) and Win32 (Windows) backends. The runtime reports its native
backend as `linux`, `macos`, or `windows`. Because every bridged facility
(threads, sockets, files, pipes/named-pipes, paths, terminal, dynamic libraries,
time, process, and the OS namespace/identity services) has all three backends,
no `/lib` package is single-platform.

Where the platforms genuinely differ (shells, path/list separators, directory
layout, permission/ACL vs. mode-bit models, signals vs. `TerminateProcess`), the
`os/` package makes those differences explicit rather than hiding them:

- **General, write-once classes** — `SLOperatingSystem`, `SLEnvironment`,
  `SLProcess`, `SLFileSystem`, `SLFile`, `SLDirectory`, `SLPath`,
  `SLPermissions`, `SLClock`, `SLEventSignal` — expose the portable intersection
  and run identically on all three OSes.
- **OS-specific flavor classes** — `SLWindowsOS`, `SLMacOS`, `SLLinuxOS` — expose
  each platform's native idioms (cmd.exe / PowerShell / `%USERPROFILE%` /
  `%APPDATA%` on Windows; `open(1)` / `~/Library` / Homebrew on macOS; `/bin/sh` /
  XDG / the FHS roots on Linux). Each has `isHost()` so a program can branch
  safely.

See [`os/OS.md`](os/OS.md) for the full host OS surface, and the repository
[`README.md`](../README.md#status--platform-support) platform table (each OS is
built in CI: `build-linux.yml`, `build-macos.yml`, `build-windows.yml`).

## Library discovery

The /lib tree is recursively indexed by the compiler and Nordshrift loader. Current canonical collection: **106 package families / 11,662 .sleela source classes / 74 `SLPackage.sleela` module facades / 11,736 total symbol records** (verified filesystem count via `tools/generate-library-symbols.py`, reconciled with `LIBRARY.INDEX.md` Revision 0.53). See `LIBRARY.SYMBOLS.md` for the complete collection.

The `sldocument` family defines the `.sldocument` format: an ordered, top-down SLeeLa document that compiles against and with standard `.sleela` source. Its annotated method steps run in order (`@order` / `@function` / bare method name), and each step usually returns a single binary **veritable-and-kind** value (`SLVeritable`). These documents suit tasks more sophisticated than bash scripting and clear national-program work where order is already established. The compile/invoke primitives sit below the explicit VM/OS bridge in `native/src/sleela_sldocument.cpp`. See `sldocument/SLDOCUMENT.md`.

`.sldocument` is a selectable **compile choice** from the SLeeLa compiler: `lib/compiler/SLSourceForm` and `lib/compiler/SLCompileChoice` let the compiler be told (explicitly or by extension) to compile a `.sleela` program or a `.sldocument`. Because a `.sldocument` may leave steps anonymous while a `.sleela` names every method, `sldocument/SLDocumentNaming`, `sldocument/SLSourceNameComparison`, and `sldocument/SLDocumentConverter` provide a deterministic naming convention (keep explicit names, derive from `@function` roles, else synthesize `step003`-style names) so an engineer can convert a `.sldocument` to a fully named `.sleela` for safekeeping without losing its established order.

## Wi-Fi OOD and native connectivity

The `wifi/` package contains the SLeeLa source object model for access-point records, credential modes, security policy, manager state, OOD state, and generation-specific Wi-Fi 4/5/6 facades. Its native counterpart in `wifi-common/` provides the C11 and C++17 operating-system providers for live scanning and connection operations. The SLVM bridge contract is documented in `wifi/WIFI.VM.BRIDGE.md`; a live SLeeLa OOD must bind that native API explicitly and must not report success while the bridge is unavailable. See `wifi/README.md`.

The compiler and Nordshrift share recursive `/lib` discovery; new package directories and source units require no compiler allow-list update.

## XML — a Document Object Model

The `xml/` package is the SLeeLa-side **XML DOM**: a document owns a tree of
element / text / comment / CDATA / processing-instruction nodes, each node owns
ordered children and named attributes, a serializer renders the tree back to
well-formed XML (compact or pretty, with XML 1.0 §2.4 escaping), and a char-cell
parser reads text back into a DOM for the common subset. The DOM core is
`SLXmlDocument` (the W3C `Document`: node factory, root management,
`getElementById`, `getElementsByTagName`, serialization), `SLXmlNode` (children,
attributes, `textContent`, depth/counts), `SLXmlAttribute`, and `SLXmlNodeType`
(the DOM `nodeType` vocabulary); `SLXmlSerializer` and `SLXmlParser` are the
text boundary. `SLXmlAspect` is an executive-grade aspect/quality register that
rates — honestly, against each aspect's real governing W3C instrument — how
fully the library addresses well-formedness, DOM, namespaces, serialization, and
validity. Node equality is a three-rung **tie of trust** (identity → same
subject → structure), reflexive, symmetric, and deterministic. See
`xml/README.md` and the runnable `xml/xml-dom.sleela`. This is the DOM data model
in `lib/`; it is distinct from `markdown/XML.SLEELA.DEFINITION.md`, which defines
an XML **interchange form for `.sleela`/`.sst` source artifacts**.

## SleelaDoc — documentation extraction (the Javadoc-tool analogue)

The `sleeladoc/` package is the SLeeLa-side **SleelaDoc extractor**: it reads
structured doc-comments out of a `.sleela` Wrapper and renders a Markdown or HTML
API page, the way `javadoc` renders Java — documenting *intent*, not just
signatures. A doc comment is an ordinary angle-bracket comment whose sentinel
token is `@doc` (or `@page`), so it lexes today with no compiler change.
`SLSleelaDoc` is the façade (`begin` → `feed` tokenized lines → `toMarkdown`/
`toHtml`); it owns an `SLDocScanner` (front end) and `SLDocRenderer` (back end)
over the model `SLDocModel`/`SLDocMember`/`SLDocComment`/`SLDocTag`. Because the
built-in String is opaque, the scanner reads tokenized lines (the char-cell
boundary used across `lib/text`/`lib/xml`). See `sleeladoc/README.md`, the
convention in `manifesto/SLEELADOC.md`, and the runnable
`sleeladoc/sleeladoc-demo.sleela`. It complements the compiler's own
author-marked doc outputs (`--wiki` wikitext + the originated twin), documented
in `manifesto/SLEELADOC.WIKI.md`.

The `opcodes` family provides SLeeLa wrapper classes for the core opcode subset: 103 `SLOp*` classes (codes 0–102; the base 98 are `OP_NOP`..`OP_AUDIO_PLATFORM`), plus `SLOpcodeBase` and `SLOpcodeStream`. It is not the complete canonical VM ISA; that registry is `lib/vm/InstructionSet.sleela` and currently mirrors native codes 0–154 (155 opcodes). Each class carries a single opcode and honours the fetch-then-execute-one contract — carefully call the VM to the next instruction, then execute exactly that one opcode — modeling `impl/core`'s dispatch loop at the SLeeLa layer. The fetch/dispatch primitives sit below the explicit VM/OS bridge in `native/src/sleela_opcode.cpp`. See `opcodes/OPCODES.md`, including the rationale for why 98 opcodes is complete for a modern program and developer.

The `opcodes/governance` sub-family adds procedural discretion over opcode execution so programs are not run raw into the VM without consideration: a **Registrar** considers a program A→B *before* it runs, a **Listener** confirms the admitted sequence fits the live VM program *during*, and an **Event Observer** judges the whole as a musical, ordered process *after* — weighing base concepts (straightness, linear reals, outright goals, ethics/norms, finalization, times-upon-counts, final goals) and emitting graded verdicts (admit / warn / patch / pause-as-unrest / reject), including fault patching via known symbol maps. SLeeLa and the VM both listen for ordering at the BEFORE/DURING/AFTER phases through `native/src/sleela_gov.cpp`. See `opcodes/governance/GOVERNANCE.md`.

The `opcodes/running` sub-family adds richer ways to run opcodes beyond a flat stream: **grouping** (`SLOpcodeGroup`, `SLOpcodeGroupSet`) runs cohesive clusters as units; a **conditional-reactive** layer (`SLOpcodeCondition`, `SLOpcodeConditionalReactive`, `SLOpcodeReactorBank`) reacts to VM/program signals by warming, gating, running, or skipping a group on its rising edge; and **warming** (`SLOpcodeWarmer`) pre-arms and pre-stages hot paths. These compose with the governance series and use two added bridge primitives (`sleela_opcode_signal`, `sleela_opcode_prestage`). See `opcodes/running/RUNNING.md`.

## Compiler — modular multi-language framework

The `/lib/compiler` package is a **modular framework for building compilers for
any publicly known programming language**. A developer adds a language by
writing one small SLeeLa front end that extends the language-neutral
`SLLanguageCompiler` contract; many independent front ends register into one
shared `SLCompilerRegistry` with no per-language allow-list. Each front end
lowers its own language toward the common SLeeLa IR and onward to a VM-ready
artifact. Shipped reference front ends under `compiler/frontends/<lang>/` cover
C, C++, Java, Python, JavaScript, Rust, and Go — spanning native, JVM, and
scripting families. The framework identifies, plans, and reports only; it never
executes an input program (compilation is not execution). Its native support —
a C ABI, a C++ orchestration facade, the `.sleela` VM/OS bridge, and a
behavioral self-test — builds and runs on Windows 10+, macOS, and Linux. See
`compiler/MULTI-LANGUAGE.FRAMEWORK.md`.

## Decompiler

The /lib/decompiler package provides the SLeeLa-sourced and SLeeLa-driven decompiler model. It supports explicit source language/version selection, expected input, desired output, fractional-input handling, loadable language modules, weighted OS/ABI discernment, evidence-preserving reconstruction, and VM-ready validation.

## Compiler and Decompiler Reference Catalogs

Compiler: lib/compiler/LANGUAGE.FORMAT.REFERENCE.md

Decompiler: lib/decompiler/LANGUAGE.FORMAT.REFERENCE.md

These catalogs provide language names, producer-program mappings, source/IR/object relationships, known binary and executable formats, OS/ABI associations, and safety-aware native reference data.

## SST and Nordshrift source/compile integration

`/lib/sst` and `/lib/nordshrift` are canonical SLeeLa source-facing packages. They participate in the same recursive `/lib` discovery used by the compiler and loader.

The path is:

`.sst source -> Nordshrift -> /lib discovery -> .sleela source -> SLeeLa Compiler -> SLeeLa IR -> SLVM/SLJVM`

SST remains declarative input. Nordshrift resolves and emits SLeeLa source. The compiler consumes that output through the normal library index; there is no second package allow-list.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
