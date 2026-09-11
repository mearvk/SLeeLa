# BodiSysCtl and Gardulus.II

`bodisysctl` is the Linux-native observation companion for Bodi/Wiggle XML structures. It monitors a bounded local picture of memory, connections, relations, items, status, iterations, P/B phase, and containment count.

The integer argument is an observation interval in milliseconds. P and B are deterministic software telemetry phases rotating around the XML object representation in memory; they are not physical orbital or electromagnetic measurements.

## P/B Start, Speed, and Relative Synchronization

Gardulus.II now exposes a richer software-orbital telemetry set for each sample:

```text
p_start_phase
b_start_phase
p_speed_deg_s
b_speed_deg_s
p_orbital_frequency_hz
b_orbital_frequency_hz
relative_phase_deg
relative_speed_deg_s
relative_sync_ratio
sync_coherence
```

The integer input establishes the cadence and the base P frequency:

```text
fP = 1000 / interval_ms
fB = 1000 / (interval_ms + 1)

vP = 360 · fP
vB = 360 · fB
```

The start phases are deterministic software seeds. The relative fields compare the two channels at the same observation instant. `relative_sync_ratio` is `fB / fP`. `sync_coherence` is a bounded 0–1 phase-alignment indicator derived from the observed relative phase.

The existing “electron start” terminology may be represented in analysis as a **software electron-start metaphor**: the initial phase/seed at which a P or B telemetry orbit begins. It is not an electron, charge, particle velocity, or physical orbital state.

Conceptually:

```text
                  XML OBJECT IN MEMORY
                           │
                ┌──────────┴──────────┐
                │                     │
          P software orbit       B software orbit
                │                     │
       start → speed → phase   start → speed → phase
                │                     │
                └──────────┬──────────┘
                           ↓
                 relative synchronization
                           ↓
                    Gardulus.II sample
```

This makes relative synchronization directly measurable by the Science API without asserting that the software telemetry corresponds to a physical orbital system.

## Command

```text
bodisysctl INTERVAL_MS ITERATIONS [LOG_FILE] [UNIX_SOCKET]
```

`ITERATIONS=0` means run until SIGINT/SIGTERM. A finite count causes the monitor to unload naturally when complete.

## Gardulus.II

The native API is in `tools/bodi/gardulus_ii.h` and provides `gardulus_run()` and `gardulus_emit_json()`. JSON Lines may be written to stdout, a file, or a connected Unix socket. `GardulusII.java` provides a local Java VM consumer for file or stream telemetry.

A sample now records iteration, timestamp, input integer, P/B phase, deterministic starts, angular speeds, orbital frequencies, relative phase/speed, synchronization ratio, synchronization coherence, memory totals, connection/relation counts, items, status, and containment count.

## Configuration

The installation properties expose the model explicitly:

```properties
bodi.gardulus.enabled=true
bodi.gardulus.interval_ms=1000
bodi.gardulus.p.start_phase_deg=0.0
bodi.gardulus.b.start_phase_deg=0.0
bodi.gardulus.p.speed_deg_s=360.0
bodi.gardulus.b.speed_deg_s=359.640359640
bodi.gardulus.sync.relative=true
bodi.gardulus.sync.coherence=true
```

The runtime currently derives the deterministic start and speed fields from the supplied integer so telemetry remains reproducible from the observation configuration. The properties document the intended control surface for later XML-scoped overrides.

## Resonance and Tone

The P/B pair can be compared against XML structural changes, Wiggle satisfaction counts, Bodi Witness sequences, connection counts, memory pressure, item/relation counts, and containment boundaries.

Useful Science measures include:

```text
phase difference over time
speed difference over time
frequency ratio
coherence distribution
phase-lock intervals
structural event → phase relationship
```

These permit software-defined resonance and tone studies. A measured correlation remains a correlation until controlled analysis supports a stronger conclusion.

## Safety and Semantics

```text
software phase     ≠ physical orbit
software speed     ≠ particle velocity
start phase        ≠ electron emission
resonance          ≠ proof of causation
count              ≠ truth
correlation        ≠ causation
telemetry          ≠ control authority
```

Gardulus.II remains an observation API. Bodi retains semantic change authority; Wiggle retains XML exploration authority; the P/B layer supplies structured telemetry for scientific comparison.

## Layered Architecture

```text
XML / SLeeLa structure
        ↓
     Wiggle
        ↓
      Bodi
        ↓
   bodisysctl
     ↙     ↘
P telemetry B telemetry
     \     /
 relative synchronization
        ↓
   Gardulus.II
        ↓
 Science listeners
```

The governing rule is:

> **Wiggle discovers structure; Bodi witnesses change; bodisysctl observes the local system; P/B telemetry supplies reproducible phase relationships; Gardulus.II makes those observations available for disciplined scientific analysis.**
