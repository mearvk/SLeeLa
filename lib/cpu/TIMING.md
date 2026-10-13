# `lib/cpu` — Voltages, Variable Clocks, and Expert Timings

This document covers the timing/voltage/expertise layer added on top of the
hardware stack in [`CPU.md`](CPU.md). The point of this layer is a single
thesis:

> A compiled `.sleela` program should carry the **stated** operating point it
> was built for, so a compiler or VM operates those **declared** timings — the
> actual voltages, the dialed clocks, the expert bus/CPU/I/O/RAM timings —
> rather than *assuming the quality of the hardware as a confidence about how
> CPUs operate*.

Everything here is **nameful and quality-first**: every timing path carries a
signal name and a human reason, every voltage rail is named, and nothing is
dialed to a speed its budgets and its settled voltage do not **honor**.

## The classes

| Class | Role |
|---|---|
| `SLVoltageRail` | One named, adjustable supply rail with **actual voltages in millivolts** — nominal, operator offset, hard floor/ceiling, load-line droop, and the settled `effectiveMv()` every timing is proven against. |
| `SLTiming` | One **named, careful timing budget** (ps): setup, hold, propagation, jitter, guardband, checked against a period. The atom all expert timings are built from. Reports slack and the max frequency it honors. |
| `SLBusTiming` | A bus's **variable clock speed** (floor/ceiling you dial between) plus three named budgets — address, **data-strobe** (the carefully-named one), control. Rides a rail: settled voltage caps the trustworthy clock. |
| `SLCpuTiming` | **Expert CPU timings**: variable core clock + Vcore rail + per-stage pipeline budgets (fetch/decode/execute/writeback). The critical stage sets the honest max clock. |
| `SLIOTiming` | **Expert timings for control of I/O**: variable link clock + Vio rail + named handshake / turnaround / recovery budgets + clock-domain-crossing synchronizer latency. |
| `SLRamTiming` | **Expert timings for RAM**: variable data rate + Vddq rail + CL/tRCD/tRP/tRAS/tRC/command-rate as cycles, with real-nanosecond access latency derived. |
| `SLTimingClass` | The expertise **tier as a Class Object** — Novice / Basic / Advanced / Expert — carrying the brand/medium and the policy (guardband, headroom, clock headroom, whether expert knobs/rails may be changed). |
| `SLTimingProfile` | The one object that gathers all rails + all expert timings under one `SLTimingClass`, dials them to the tier, checks `honored()`, and seals an `SLTimingManifest`. |
| `SLTimingManifest` | The compact, **serializable record a compiled `.sleela` image carries** — tier, dialed clocks, settled voltages, RAM primaries, honored flag, worst slack. Read back by the VM/loader so declared timings are honored. |

## Actual voltages, as adjustable settings

`SLVoltageRail` models the real electrical operating point in **millivolts**:

```sleela
SLVoltageRail vcore = new SLVoltageRail();
vcore.configure("Vcore", 1200, 950, 1450);  // nominal / floor / ceiling (mV)
vcore.setLoadLine(500);                      // 0.5 mOhm load line (micro-ohms)
vcore.setLoad(60000);                        // 60 A draw (milliamps)
vcore.adjust(-30);                           // undervolt 30 mV (clamped to window)
int settled = vcore.effectiveMv();           // nominal + offset - droop
```

The settled voltage is what every timing budget on that rail is checked
against. A locked rail clamps any adjustment back inside `[floor, ceiling]`, so
a dialed point is always electrically safe; `unlock()` (Expert tier only) lifts
the cap for bench work.

## Variable clock speeds for buses (careful and nameful)

A bus is **not** assumed to run at a frequency — its clock is dialed between a
floor and a ceiling, and the ceiling is additionally capped by what the settled
rail voltage supports:

```sleela
SLBusTiming mem = new SLBusTiming();
mem.configure("MemBus", 400, 2400, 64, 2);   // floor/ceiling MHz, 64-bit, DDR
mem.attachRail(vsoc, 1);                      // 1 mV of settled Vsoc per MHz
mem.setDataPath(90, 25, 18, 10);              // the carefully-named data strobe
int actual = mem.setClock(2133);              // clamped into range AND voltage
boolean ok = mem.honored();                   // all three lanes + rail together
```

The **data-strobe** path (`"data-strobe"`, "payload must settle inside the data
eye on every transfer edge") is given its own name and reason because, per the
thesis, timing issues for buses must be careful and nameful. `worstSlackPs()`
names how close to the edge the bus runs.

## Expert timings: CPU, I/O, RAM

- **CPU** (`SLCpuTiming`): state measured per-stage delays; the slowest stage
  (`criticalStage()`, usually the ALU carry chain) sets `criticalMaxMHz()`, and
  `setClock()` refuses anything above that or above what settled Vcore allows.
- **I/O control** (`SLIOTiming`): named `req->ack`, `bus-turnaround`, and
  `recovery` budgets, plus `synchronizerLatencyPs()` for crossing the device
  clock domain — I/O control timing is accounted, never assumed instant.
- **RAM** (`SLRamTiming`): set `CL/tRCD/tRP/tRAS/tRC` as command-clock cycles;
  `firstWordLatencyNs()` and `randomAccessLatencyNs()` report the real latency,
  and `honored()` enforces `tRAS >= tRCD + CL` and `tRC >= tRAS + tRP`.

## Expertise tiers as Class Objects

Novice / Basic / Advanced / Expert are **objects**, not flags. A tier *is* its
policy:

```sleela
SLTimingClass t = new SLTimingClass();
t.expert("ACME DDR5 B-die");          // brand/medium label
boolean mayTune = t.allowsKnob();     // Expert: true; Novice/Basic: false
int dial = t.dialableMHz(5000);       // Expert takes the ceiling; Novice backs off 30%
```

| Tier | Guardband | Rail headroom | Clock headroom | Expert knobs | Rails unlockable |
|---|---|---|---|---|---|
| Novice | 120 ps | 120 mV | 30 % below ceiling | read-only | no |
| Basic | 80 ps | 80 mV | 20 % | read-only | no |
| Advanced | 50 ps | 40 mV | 10 % | writable | no |
| Expert | 20 ps | 10 mV | 0 % (edge) | writable | yes |

A safer tier runs the rails a little **higher** (more margin) and the clocks
**lower**; Expert runs lean and fast. All four tiers are internally honored out
of the box.

## The whole profile → a manifest a `.sleela` file carries

```sleela
SLTimingProfile p = new SLTimingProfile();
p.configure(SLTimingClass.TIER_ADVANCED, "ACME SoC rev C");
p.dialCore(1500);          // tier-governed: refused above the honest ceiling
p.adjustVcore(-20);        // undervolt; raising needs Advanced+
boolean ok = p.honored();  // every expert timing on every rail, together
SLTimingManifest man = p.toManifest();   // flatten to the embeddable record
```

### Operating declared timings instead of assuming hardware quality

The compiler stamps the sealed manifest into every program it emits, and the VM
adopts it — so the engine honors what the image **states**:

```sleela
SLCompilerDriver driver = new SLCompilerDriver();
driver.configure(isa);
driver.useTimingProfile(p);                       // stamp the operating point
SLProgram prog = driver.compileFile("app.sleela", "app");
// prog.hasTiming() == true; prog.timingHonored() reflects the sealed check

SLSleelaVM vm = new SLSleelaVM(); vm.configure(4096, 1024, 256);
vm.adoptProgramTiming(prog);                      // operate the DECLARED point
int coreMHz = vm.declaredCoreMHz();               // 0 if the image stated none
```

`SLProgram` grows an optional `SLTimingManifest`; `SLTimingManifest.writeHeader`
/ `readHeader` serialize it as a 17-word header (magic `5213`) ahead of the
instruction stream, so any consumer can tell "this image carries a stated
profile" from "this image expects me to assume one" — and never fabricates a
clock or voltage it was not told.

### End to end via `SLMachine`

```sleela
SLMachine m = new SLMachine();
m.powerOnAndRun(4, 32, 3200, 8);
SLTimingProfile prof = m.selectTiming(SLTimingClass.TIER_EXPERT, "SleelaWorks SL-Core-X");
SLProgram app = m.compileWithTiming("app.sleela", 0, "app");  // carries the manifest
boolean honored = m.timingHonored();
```

## Fidelity note

Like the rest of `lib/cpu`, this is a clear **behavioral model**: millivolt and
picosecond integer arithmetic that captures the real structure — settled
voltage gating clock speed, critical-path-limited frequency, DRAM primaries
deriving real latency, clock-domain-crossing cost, and an expertise policy that
governs how far any of it may be pushed — at a level that stays readable as
SLeeLa source.
