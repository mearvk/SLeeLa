# SLeeLa XLT Library Package

`/lib/xlt` is the SLeeLa source-facing representation of the **XLT (`.xlt`)
CPU-model sheet** layer — the counterpart of `/lib/sst` for *CPU models* rather
than for systems of library symbols.

## What XLT is

Where an `.sst` sheet declaratively authors a **system** of components, an
`.xlt` sheet declaratively authors a **CPU model**: a chosen CPU SKU plus the
board, memory, refresh cadence, storage, and bus parts that assemble it.

```
#xlt 1.0
model EpycWithDdr2 {
  cpu      "SLAMDEPYC9575F";   // a real CPU SKU class
  memory   "ddr2_800" 4096;    // a real SLMemoryModule preset + capacity (MiB)
  cadence  "DDR2";             // present a DDR2 refresh cadence (SLCadenceConnector)
  storage  "nvmeGen4";         // optional
  bus      "pci33";            // optional
  gpu;                         // optional capability
  dma;                         // optional capability
  nativeram;                   // optional capability
  target   sleela;
}
```

### Capability directives

Three optional **bare** directives (no quoted ref; each declared at most once,
like every other slot) request runtime capabilities for the model:

| Directive | Effect |
| --- | --- |
| `gpu;` | native GPU compute — wires to `SLExecutorBuild.enableGPU()`. |
| `dma;` | a DMA bulk-move engine — wires to `SLExecutorBuild.enableDMA()`. |
| `nativeram;` | run the VM's linear memory on real host RAM — records the `NATIVE_RAM` feature bit the VM creator (`SleelaVMCreator.requestNativeRam()`) honors when it hosts the build. |

They appear in the build list on success (e.g. `capability: gpu (native
compute)`).

### Memory tiers (usual vs. accurate)

A model may split memory into two tiers so a program can run **usual activities**
on a cheaper tier (e.g. DDR2) and **competency/accuracy-critical** work on a
faster tier (e.g. DDR5):

```
memtier  "ddr2_800"  usual;      // usual-activities tier
memtier  "ddr5_6400" accurate;   // competency / accuracy tier
maintier "accurate";             // the main program's default tier
```

- `memtier "<memory-preset>" usual|accurate;` — designate a tier (each at most
  once; the ref must be a registered memory preset).
- `maintier "usual|accurate";` — which tier the main program uses when a
  function is not marked (default: `accurate`). `maintier` must name a declared
  tier, else a `[disagrees]` congruence.

At the **language** level, individual functions choose their tier with two
modifier keywords (added alongside `const`, composing with `public`/`static`/
`final` as normal):

```
usualmem    void   logActivity()  { ... }   // runs on the usual (DDR2) tier
accuratemem int    settleLedger() { ... }   // runs on the accurate (DDR5) tier
int main() { ... }                          // inherits the main tier (DDR5)
```

Unmarked declarations inherit the program's main tier. See
`examples/epyc-tiered-memory.xlt`.

### Running a program on the built model

An `.xlt` model can both **build the machine and politely run a program on it**,
so the user "calls their program" from the one declarative document:

```
program  "app.sleela" sleela;   // source path + language (sleela|c|cpp|java)
grant    65536 1000000;          // memory words + cycle budget it is admitted under
```

When a congruent build declares a `program`, `SLXltCompiler` takes the sanctioned
path: it assembles the build, creates an `SLCPUResource` grant and an
`SLWorkload` (source + language), and runs it on `build.processor()` via
`SleelaVMCreator` — admission-checked, through the model's fetch-decode-execute
loop, never reaching around the CPU model. The declared capabilities flow into
the run (`nativeram`→`requestNativeRam()`, `gpu`→`requestGPU()`,
`dma`→`requestDMA()`), and the program's `usualmem`/`accuratemem` functions
dispatch to the DDR2/DDR5 tiers the model declared.

On success the build list gains `program: <path> (<lang>) -> output N`
(`SLXltBuildResult.programRan()` / `output()`); a missing source, an unknown
language, or a non-congruent machine yields a congruence reason instead. `grant`
is optional — the compiler uses polite defaults (4096 words / 1,000,000 cycles)
when it is omitted. The native `xltc` validates the program/grant and reports it
as ready-to-run (it does not host a VM); the Sleela-side compiler performs the
actual run.

It is the declarative analog of the hand-written builder in
`lib/cpu/amd-epyc/examples/epyc-with-ddr2-settings.sleela`.

### Integer literals (decimal / hex / binary / octal)

Integer values in an `.xlt` sheet — the memory capacity and the `grant`
words/budget — accept **radix-prefixed literals**, in parity with the SLeeLa
frontend (`SLEELA.syntax` §1.4):

| Form | Prefix | Example |
| --- | --- | --- |
| decimal | *(none)* | `4096` |
| hexadecimal | `0x` / `0X` | `0x10000` (= 65536) |
| binary | `0b` / `0B` | `0b1_0000_0000_0000_0000` |
| octal | `0o` / `0O` | `0o200000` |

An underscore `_` may separate digits for readability and is ignored
(`0xFF_EC`, `1_000_000`). A prefix with no following digit (e.g. `0x`) is a
lexical error (`XLT-E-0003`). All forms denote the same 64-bit integer; the
prefix only selects the base. This is the natural way to write power-of-two
memory sizes and cycle budgets:

```
memory "ddr5_6400" 0x10000;              // 64 GiB
grant  0b1_0000_0000_0000_0000 0xF4240;  // 65536 words, 1_000_000 cycles
```

See `examples/epyc-radix-literals.xlt`.

## Design: orthogonality is the contract

XLT deliberately needs **no new keyword and no setters**. Each part occupies one
orthogonal slot, declared at most once, read in document order. That
orthogonality is itself what guarantees the model's integrity and a clear
compilation — a duplicated or conflicting declaration is a reportable failure,
not a silent override.

## Compile path

```
.xlt -> SLXltCompiler -> SLExecutorBuild (real catalog parts) -> SLProgram/VM
```

Parallel to the SST path (`.sst -> Nordshrift -> .sleela -> SLeeLa Compiler`).

## Compilation output

`SLXltCompiler.compile(source)` returns an `SLXltBuildResult` that is exactly
one of:

- **SUCCESS** — the model built, with an ordered list of **what was built** (the
  CPU SKU and every part that resolved and agreed), or
- **a CONGRUENCE report** — an ordered list of why it could **not** be built:
  - `[missing]` — a declared part resolved to nothing (unknown SKU/preset), or
  - `[disagrees]` — a part resolves but is **not agreeable to design** (e.g.
    8-channel memory declared on a 4-channel board).

The compiler reads parts in document order and checks each both **resolves**
(exists in the roster / parts bin) and **agrees** with the rest (orthogonal and
mutually consistent — reusing checks like `SLExecutorBuild.boardFits()`).

## Classes

| Class | Role |
| --- | --- |
| `SLXltSource` | source-time representation of an `.xlt` sheet: the declared cpu/board/memory/cadence/storage/bus, each an orthogonal slot. |
| `SLXltProducerRegistry` | the explicit registry of recognizable part refs per slot (+ channel attributes for the agreement check). A new part becomes recognizable by registering it, not by editing the compiler. |
| `SLXltCompiler` | the ordered, orthogonal, congruent interpreter: consults the registry to recognize each part, resolves it against `SLComponentCatalog` + the CPU roster, and assembles an `SLExecutorBuild`. |
| `SLXltBuildResult` | the outcome: SUCCESS + build list, or a congruence report of missing / disagreeing parts. |
| `SLXltRefBank` | internal fixed string-bank helper for the registry. |

## Native front-end (`impl/xlt/`)

A standalone C++ front-end tokenizes and parses `.xlt` files directly (the
counterpart of `impl/nordshrift/` for SST), so an `.xlt` file is a first-class
compilable document rather than only a source-builder API:

```
.xlt  ->  xlt_lexer  ->  xlt_parser (XltModel)  ->  xlt_resolve  ->  xltc CLI
```

- `xlt_lexer` / `xlt_parser` — brace-delimited tokenizer + recursive-descent
  parser; enforces **orthogonality** (a duplicate slot is `XLT-E-0020`, not a
  silent override) and emits structured `XLT-{tier}-{code}` diagnostics.
- `xlt_resolve` — the ordered, congruent resolver (native mirror of
  `SLXltCompiler` + the registry): SUCCESS + build list, or a congruence report
  of `[missing]` / `[disagrees]` parts.
- `xltc` — the CLI. Build and run:

```
make -C impl/xlt
impl/xlt/build/xltc lib/xlt/examples/epyc-ddr2.xlt    # SUCCESS, exit 0
impl/xlt/build/xltc lib/xlt/examples/bad-model.xlt    # congruence report, exit 1
```

## Examples

- `examples/epyc-ddr2.xlt` — the EPYC + DDR2 CPU model as an XLT sheet.
- `examples/epyc-tiered-memory.xlt` — a model with usual/accurate memory tiers.
- `examples/epyc-radix-literals.xlt` — hex/binary integer literals for the
  memory capacity and the grant words/budget.
- `examples/compile-xlt-models.sleela` — compiles a congruent model (success +
  build list) and a non-congruent one (congruence report), end to end.
