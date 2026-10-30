# XLT examples — what to learn here

XLT (`.xlt`) is SLeeLa's **CPU-model sheet** layer — the hardware counterpart of
Nordshrift's system sheets. An `.xlt` file declaratively *assembles a machine*
from registered catalog parts: a CPU SKU, memory, refresh cadence, storage, a
bus, optional capabilities, memory tiers, and even a program to run on the
result. These examples teach the model format from a two-line machine up to a
tiered machine that runs a program.

Build the validator once, then check each model:

```sh
make -C impl/xlt
impl/xlt/build/xltc lib/examples/xlt/BASIC-cpu-memory.xlt
```

Read the examples in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-cpu-memory.xlt`](BASIC-cpu-memory.xlt) | The minimum model: a CPU SKU, a memory preset + capacity, and a target. |
| Moderate | [`MODERATE-storage-bus-capabilities.xlt`](MODERATE-storage-bus-capabilities.xlt) | Add cadence, storage, a bus, and the optional `gpu;`/`dma;` capability directives. |
| Advanced | [`ADVANCED-tiered-memory.xlt`](ADVANCED-tiered-memory.xlt) (+ [`ADVANCED-app.sleela`](ADVANCED-app.sleela)) | Two memory tiers (usual/accurate) and running a program under a resource grant. |

The original [`CLASSIC-epyc-ddr2-model.xlt`](CLASSIC-epyc-ddr2-model.xlt) is the
full-featured starter.

## What we want you to take away

1. **A machine is a parts list.** Each line fills exactly one slot (`cpu`,
   `memory`, `cadence`, `storage`, `bus`, …), and every ref must be a *registered
   catalog part* — `xltc` rejects an unknown SKU or preset. Reading a model tells
   you the whole machine. (Valid refs include CPUs like `SLAMDEPYC9755`, memory
   presets like `ddr4_3200`/`ddr5_6400`, storage like `nvmeGen4`, buses like
   `pcieGen5x16`.)
2. **Capabilities are bare directives.** `gpu;`, `dma;`, and `nativeram;` request
   runtime features; each may appear at most once and shows up in the build list
   on success. They are optional — the basic model omits them entirely.
3. **Memory can be tiered.** The advanced model declares a cheap `usual` tier
   (DDR2) and a fast `accurate` tier (DDR5), and picks the default with
   `maintier`. At the language level, functions choose a tier with the
   `usualmem` / `accuratemem` modifiers; unmarked code inherits the main tier.
4. **A model can run a program.** With `program "<source>" <lang>;` and a
   `grant <words> <cycles>;`, the model builds the machine *and* admits and runs
   the program through the built CPU's fetch-decode-execute loop under that
   resource budget — one declarative document that describes the hardware and
   calls your code.

## A note on the tier modifiers

The `usualmem` / `accuratemem` **function** modifiers are an XLT-aware language
feature recognized by the source-built toolchain (`make compiler`). The bundled
`working-binaries/sleela` (an older build) does not accept them, so
`ADVANCED-app.sleela` is kept as plain SLeeLa 1.7 that validates everywhere, and
the memory tiers are declared at the **model** level in the `.xlt` sheet. For a
model-level tiered example from the library, see
`lib/xlt/examples/epyc-tiered-memory.xlt`.

## Validate them

```sh
impl/xlt/build/xltc lib/examples/xlt/MODERATE-storage-bus-capabilities.xlt
impl/xlt/build/xltc lib/examples/xlt/ADVANCED-tiered-memory.xlt
```

Each prints `BUILD SUCCESS` and lists the assembled parts (the advanced model
lists 10, including the program ready to run under its grant).
