# BodiSysCtl and Gardulus.II

## Purpose

`bodisysctl` is the Linux-native observation companion for Bodi/Wiggle XML structures. It is installed by the Bodi installation script when the local toolchain is available.

It monitors a bounded local picture of:

- system memory
- network connection counts
- structural relations
- observed items
- status
- iteration count
- P/B orbital phase
- containment count

It does not modify kernel state, alter process scheduling, or claim physical resonance. The orbital terminology is a deterministic telemetry model around an XML object representation held by the local Bodi/Wiggle system.

## Integer Input and P/B Orbitals

The C monitor accepts a positive integer in milliseconds:

```text
bodisysctl INTERVAL_MS ITERATIONS [LOG_FILE] [UNIX_SOCKET]
```

The integer controls the observation cadence. Each sample contains two deterministic phases:

```text
P phase = f(time, interval)
B phase = f(time, interval + 1)
```

The phases are expressed in degrees and wrap at 360 degrees. They are deliberately software-defined control signals, not electromagnetic orbital measurements.

Conceptually:

```text
                 XML OBJECT IN MEMORY
                         │
              ┌──────────┴──────────┐
              │                     │
          P-orbital             B-orbital
          easy control        higher middle-
          / local state       control state
              │                     │
              └──────────┬──────────┘
                         ↓
                  Gardulus.II sample
```

P represents local/easy-control telemetry. B represents the higher middle-management/control-plane telemetry associated with a structural set. The implementation keeps both channels predictable and observable.

## Local Monitoring

The Linux C implementation reads only local, ordinary observation interfaces such as `/proc/meminfo` and `/proc/net/tcp*`. It does not require root privileges for the normal telemetry path.

The monitor may emit JSON Lines to:

1. standard output;
2. a log file; or
3. a connected local Unix-domain socket.

A local Java VM may consume compatible telemetry through `GardulusII.java`.

## XML-Scoped Lifetime

The installation configuration identifies XML structural families that may own a monitor:

```properties
bodi.xml.structures=tree,record,reference,state,command
bodi.gardulus.enabled=true
bodi.gardulus.interval_ms=1000
```

The intended lifecycle is:

```text
XML feature discovered
        ↓
Wiggle structural profile
        ↓
monitor requested
        ↓
bodisysctl starts
        ↓
P/B telemetry
        ↓
Science listener
        ↓
feature complete
        ↓
monitor stops / unloads
```

`bodisysctl` can also be run for a finite iteration count. A signal causes an orderly stop. No persistent daemon is required by the design.

## Gardulus.II Science API

The native API is declared in `tools/bodi/gardulus_ii.h`.

A sample contains:

```text
iteration
timestamp_ms
input_integer
p_phase
b_phase
memory_total_kb
memory_available_kb
connections
relations
items
status
containment_count
```

The API provides:

```c
int gardulus_emit_json(...);
int gardulus_run(int interval_ms, int iterations,
                 gardulus_listener listener, void *user_data);
```

A Science officer or local analysis process can therefore subscribe through a callback, consume JSON Lines, or read the output file.

## Java VM Listener

`implementations._001_.bodi.GardulusII` provides local Java consumption from a telemetry file or TCP stream. It deliberately treats the telemetry as data rather than giving it authority over the Bodi change layer.

## Resonance and Tone

The P/B phase pair can be used to study software-defined resonance patterns, tone, cadence, recurrence, and structural correspondence. For example, a Science process can compare phase against:

- XML structural changes
- Wiggle satisfaction counts
- Bodi Witness sequences
- connection counts
- memory pressure
- item/relation counts
- containment boundaries

A correlation is not automatically a causal relationship.

The proper scientific chain remains:

```text
measurement
  ↓
observation
  ↓
correlation
  ↓
hypothesis
  ↓
controlled comparison
  ↓
confidence
```

## Safety and Semantics

The following distinctions remain mandatory:

```text
telemetry       ≠ control authority
phase           ≠ physical orbit
resonance       ≠ proof of causation
count           ≠ truth
correlation     ≠ causation
local monitor   ≠ persistent system daemon
```

Gardulus.II is therefore a **science observation API**, while Bodi remains the semantic change layer and Wiggle remains the XML exploration layer.

## Layered Architecture

```text
             XML / SLeeLa structure
                       │
                       ▼
                    Wiggle
              structural inference
                       │
                       ▼
                     Bodi
              semantic change/witness
                       │
                       ▼
                  bodisysctl
               Linux observation
                       │
                 ┌─────┴─────┐
                 ▼           ▼
             P-orbital   B-orbital
                 │           │
                 └─────┬─────┘
                       ▼
                 Gardulus.II
                 science API
                       │
              ┌────────┴────────┐
              ▼                 ▼
          log/file          local listener
```

The governing rule is:

> **Wiggle discovers structure; Bodi witnesses change; bodisysctl observes the local system; Gardulus.II makes those observations available for disciplined scientific analysis.**
