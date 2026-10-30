# SLeeLa Library Examples

This directory is the learning and demonstration catalogue for the SLeeLa library and its related development tools. Examples are grouped by subsystem; reusable implementation code remains in its owning `/lib` or `/impl` directory.

## How the catalogue is organized

Each file is named for the concept it teaches. Most subsystem folders now carry a
three-step learning progression plus a plain-English guide:

- **BASIC** — the single core idea of the subsystem, in the shortest honest form.
- **MODERATE** — the idea applied to realistic data or a small workflow.
- **ADVANCED** — a fuller, closer-to-real case that combines several ideas.
- **LEARNING-NOTES.md** — a plain-English guide in each subsystem folder: what the
  examples teach, what to take away, and the SLeeLa gotchas they illustrate. **Start
  there when you open a folder.**

Other name prefixes you will see:

- **CLASSIC** — foundational, minimal patterns (the original per-folder starters).
- **ORIGINAL** — familiar teaching exercises and canonical algorithms.
- **USED** — practical, established workflows.
- **NEW** — newer language or library capabilities.
- **DRAFT** — proposed source vocabulary that is not guaranteed to be implemented as a compiler intrinsic.
- **SAFE / PREFLIGHT** — non-destructive validation and operational checklists.

Start with the [child-friendly SLeeLa learning path](sleela/LEARNER-PATH.md), review [SLeeLa's command workflow](sleela/COMMANDS-and-workflow.md), then run the [classic Hello World](sleela/CLASSIC-hello-world.sleela).

Nearly every subsystem folder below has a **LEARNING-NOTES.md** that explains, in
simple terms, what the BASIC → MODERATE → ADVANCED examples are for and what you
should understand after reading them. The table below links the recommended first
example per folder; open the folder's `LEARNING-NOTES.md` for the guided path.

## Example catalogue

| Directory | Starting example | Scope / run notes |
|---|---|---|
| [sleela](sleela/) | [CLASSIC Hello World](sleela/CLASSIC-hello-world.sleela) | Language foundations, loops, methods, arrays, structs, nested classes, imports, and [16 CLI option/workflow examples](sleela/README.md#command-line-examples) |
| [sleelvac](sleelvac/) | [Compiler invocation](sleelvac/CLASSIC-compiler-invocation.md) | Separate compiler product; confirm options with `sleelvac --help` |
| [nordshrift](nordshrift/) | [Capstone sheet](nordshrift/CLASSIC-capstone.sst) | Paired with [Ledgerize source](nordshrift/CLASSIC-Ledgerize.sleela); build using the repository's Nordshrift tool |
| [xlt](xlt/) | [EPYC + DDR2 model](xlt/CLASSIC-epyc-ddr2-model.xlt) | Validate with the built `xltc` tool and current catalog |
| [ballum](ballum/) | [Display frame](ballum/CLASSIC-display-frame.sleela) | Frame arithmetic illustration; does not open a native display backend |
| [autocad](autocad/) | [Geometry primitives](autocad/CLASSIC-geometry-primitives.sleela) | Geometry calculation illustration; does not call AutoCAD APIs |
| [gui](gui/) | [Layout bounds](gui/CLASSIC-layout-bounds.sleela) | Parent/child sizing illustration; does not open a GUI window |
| [runtime](runtime/) | [Thread workers](runtime/USED-thread-workers.sleela) | Threads, guarded shared state, mailbox messages, and join |
| [compiler](compiler/) | [Source validation workflow](compiler/CLASSIC-source-validation-workflow.md) | Check, compile, and run a source artifact |
| [network](network/) | [Loopback TCP echo](network/USED-loopback-tcp-echo.sleela) | Uses local TCP loopback on port 45218 |
| [wifi](wifi/) | [Wi-Fi policy smoke](wifi/USED-wifi-policy-smoke.sleela) | Policy/state check; physical radio access requires the native VM bridge |
| [cdma](cdma/) | [Link budget](cdma/CLASSIC-link-budget.sleela) | Numerical illustration; no radio transmission |
| [sleela-sql](sleela-sql/) | [Create table](sleela-sql/CLASSIC-create-table.sql) | Local CSV-backed SQL subset; build/test instructions live in `/lib/sleela-sql` |
| [antivirus](antivirus/) | [Integrity preflight](antivirus/SAFE-integrity-preflight.md) | SHA-256 manifest verification, not a malware scan |
| [crypto](crypto/) | [Hash integrity workflow](crypto/CLASSIC-hash-integrity-workflow.sleela) | Describes the trusted verifier workflow; not a cryptographic implementation |
| [regex](regex/) | [Literal find draft](regex/DRAFT-literal-find.sleela) | Proposed regex vocabulary; explicitly marked draft |
| [debugger](debugger/) | [Debugging checklist](debugger/CLASSIC-debugging-checklist.sleela) | Reproducible diagnostic workflow |
| [decompiler](decompiler/) | [Artifact inspection](decompiler/CLASSIC-artifact-inspection.sleela) | Safe inspection workflow; does not decode an artifact |
| [connector](connector/) | [Connector lifecycle](connector/CLASSIC-connector-lifecycle.sleela) | Lifecycle illustration; does not contact an external service |
| [java](java/) | [Java launch preflight](java/CLASSIC-java-launch-preflight.sleela) | Preflight checklist; does not launch a JVM |
| [terminal](terminal/) | [Terminal menu](terminal/CLASSIC-terminal-menu.sleela) | Console output example; no interactive input |

## Running the SLeeLa language examples

After building/installing the runtime, run a source example with:

```sh
sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
sleela run lib/examples/sleela/CLASSIC-hello-world.sleela
```

To create and run a persistent artifact:

```sh
sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o /tmp/hello.sleela
sleela run /tmp/hello.sleela
```

Some examples are deliberately *contract, policy, or simulation demonstrations*. Their comments and this index state when they do not access hardware, external services, or an implemented native bridge. Do not interpret those examples as proof of an integration that the corresponding subsystem README identifies as pending.

## Contribution rules

- Keep each example focused and name it for the concept it teaches.
- Include complete source and small, non-sensitive test data.
- Document prerequisites, exact build/compile/run commands, expected output, and verification steps.
- Prefer deterministic examples that do not require network access, hardware, elevated privileges, or external credentials.
- Clearly label simulations, proposed APIs, and integration contracts.
- Keep reusable implementation code in its owning subsystem directory; this catalogue is for teaching and demonstration.
