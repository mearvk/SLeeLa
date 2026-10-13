# CPU Roster, Fleet & Net Results

This document covers the "choose from a full roster of CPUs and simulate a real
world with net results" surface of `lib/cpu`.

## The pieces

| Class | Role |
|---|---|
| [`SLCPUCatalog`](SLCPUCatalog.sleela) | The **by-name roster** of runnable CPU models — the missing CPU family alongside `SLHardwareRegistry`'s memory/SSD/HDD/USB/GPU catalogs. Pick a processor by key (`"6502"`, `"arm"`, `"riscv"`, `"x86"`, `"ps5"`, `"xbox-series-x"`, …) with `make(key)` instead of hard-coding `new SL6502CPU()`. `list()`/`keyAt(i)` enumerate the roster for a chooser. |
| [`SLCPUFleet`](SLCPUFleet.sleela) | A **selected set of CPUs run as a group to produce net results**. `select(key)`/`selectAll()` assemble the fleet from the catalog; `runAcross(...)` runs a workload on every member; `scoreboard()` reports the aggregate. |
| [`SLCPURuntime`](SLCPURuntime.sleela) | The runnable CPU base (admission + `runWorkload` + clock). Every `SL<ARCH>CPU` extends it. |
| [`SLClockPolicy`](SLClockPolicy.sleela) | Spec clock, overclock-to-target, stability ceiling, `effectiveMHz()`/`speedRatioMilli()` — feeds the fleet's aggregate-throughput accounting. |
| [`SLWorkload`](SLWorkload.sleela) / [`SLCPUResource`](SLCPUResource.sleela) | The unit of work and its bounded grant (memory words + instruction budget). |

## From roster to net results

```text
   SLCPUCatalog            choose              SLCPUFleet
  (full roster of   ->  select(key) /    ->  (chosen CPUs)
   runnable CPUs)        selectAll()              |
                                                  | runAcross(workload)
                                                  v
                                     per-CPU runWorkload()  (same result,
                                     each within its grant   any architecture)
                                                  |
                                                  v
                                     NET RESULTS scoreboard:
                                       net output (sum of observables)
                                       aggregate spec vs. effective MHz
                                       completed / rejected counts
                                       fastest / slowest member
                                       retired instruction total
```

### Net results, defined
- **Net output** — the sum of each CPU's observable result. Because any supported
  source language lowers to the same image and every architecture maps it onto
  the canonical opcode substrate, each CPU returns the *same* observable (the
  1:1 Turing-effect guarantee), so the net output scales with fleet size, not
  architecture.
- **Aggregate clock** — the sum of effective (post-overclock) MHz vs. the sum of
  spec MHz, with a `fleetSpeedRatioMilli()` (1000 = at spec). Overclocking the
  fleet raises the aggregate effective clock while leaving the net output
  invariant (the result is clock-independent) — the throughput/feasibility
  dimension of the "net result."

## Creator over the network

The [`SleelaVMCreator`](../vm/creator/SleelaVMCreator.sleela) chain
(Creator → SLVM → Sleela CPU → program) extends to a **distributed fleet**: a
creator dispatches a workload to every node of a fleet "over the network,"
accounts per-hop latency and delivery, and aggregates the fleet's net results —
the world-level view of running real work on many machines at once.

## Run it

The runnable aggregate [`examples/world-net-results.sleela`](examples/world-net-results.sleela)
inlines a faithful behavioral model of all of the above (the split `lib/cpu/SL*`
classes are the discovery surface; the aggregate is the executable twin, exactly
as `autocad/creator/creator.sleela` is for the AutoCAD Creator). It builds the
roster, chooses a fleet, runs work to produce net results, overclocks and
re-runs, drives the same over the Creator network across the whole roster, and
ends with a SELF-CHECK.

```sh
make -C lib/cpu world     # -> out/world.log, prints the scoreboards + SELF-CHECK
```

`make world` also runs `verify`, which fails the build unless the simulation's
`SELF-CHECK: PASS` line is present.

## Fidelity

These are **behavioral models for net-result accounting**, not cycle-accurate
emulators (see [`CPU.md`](CPU.md) "Note on fidelity"). Instruction retirement and
observable output are deterministic and reproducible; per-architecture bus and
timing detail live in each arch folder's markdown and the
[`SLTiming*`](TIMING.md) classes.
