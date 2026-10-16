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

It is the declarative analog of the hand-written builder in
`lib/cpu/amd-epyc/examples/epyc-with-ddr2-settings.sleela`.

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
- `examples/compile-xlt-models.sleela` — compiles a congruent model (success +
  build list) and a non-congruent one (congruence report), end to end.
