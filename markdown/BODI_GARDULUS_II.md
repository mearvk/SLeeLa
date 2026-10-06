# BodiSysCtl and Gardulus.II

`bodisysctl` is the Linux-native observation companion for Bodi/Wiggle XML structures. It monitors a bounded local picture of memory, connections, relations, items, status, iterations, P/B phase, containment count, and a 3D software-orbital representation of the addressed XML structure.

## XML-Sized 3D Circular Orbits

The P/B telemetry is now modeled primarily as **3D circular orbits** around the XML object representation in memory. Each orbit remains a circle mathematically, but the circle is embedded in three-dimensional space by an inclination and, for B, a node rotation. P and B therefore have distinct orbital planes while remaining directly comparable.

Each sample exposes:

```text
xml_size_bytes
orbit_radius_units
p_orbit_inclination_deg
b_orbit_inclination_deg
p_orbit_x / p_orbit_y / p_orbit_z
b_orbit_x / b_orbit_y / b_orbit_z
```

The reference orbit radius is the serialized/improved XML structure size in bytes, expressed as software geometry units. The implementation enforces the explicit ceiling:

```text
orbit_radius <= 1.5 × xml_size_bytes
```

The default reference size is 1024 bytes when the caller does not provide a structure size. `bodisysctl` accepts an optional XML size so the monitor can use the actual serialized structure size rather than the default:

```text
bodisysctl INTERVAL_MS ITERATIONS [LOG_FILE] [UNIX_SOCKET] [XML_SIZE_BYTES]
```

The intended operating point is approximately one-to-one with the XML structure size; the 1.5× bound is a hard upper limit, not a target.

The 3D embedding is deterministic. P uses a 35° inclination and B uses a 55° inclination with a 25° node offset. The phase fields drive the position around each circular path. Thus, at every observation instant, Gardulus.II can report both the scalar synchronization state and the corresponding P/B spatial coordinates.

Conceptually:

```text
                         Z
                         │
                    B orbit
                   ╱       ╲
                  ╱         ╲
        XML OBJECT ●─────────╲──── Y
                  ╲           ╲
                   ╲   P orbit ╲
                    ╲           ╲
                     X
```

The diagram is conceptual; the actual coordinates are emitted numerically in each JSON sample.

## P/B Start, Speed, and Relative Synchronization

Gardulus.II exposes:

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

## Command

```text
bodisysctl INTERVAL_MS ITERATIONS [LOG_FILE] [UNIX_SOCKET] [XML_SIZE_BYTES]
```

`ITERATIONS=0` means run until SIGINT/SIGTERM. A finite count causes the monitor to unload naturally when complete. `XML_SIZE_BYTES` should represent the size of the improved/serialized XML structure being observed.

## Gardulus.II

The native API is in `tools/bodi/gardulus_ii.h` and provides `gardulus_run()`, `gardulus_run_with_xml_size()`, and `gardulus_emit_json()`. JSON Lines may be written to stdout, a file, or a connected Unix socket. `GardulusII.java` provides a local Java VM consumer for file or stream telemetry.

A sample records iteration, timestamp, input integer, P/B phase, deterministic starts, angular speeds, orbital frequencies, relative phase/speed, synchronization ratio, synchronization coherence, XML size, orbital radius, orbital-plane parameters, 3D P/B coordinates, memory totals, connection/relation counts, items, status, and containment count.

## Configuration

The installation properties expose the model explicitly:

```properties
bodi.gardulus.enabled=true
bodi.gardulus.interval_ms=1000
bodi.gardulus.p.start_phase_deg=0.0
bodi.gardulus.b.start_phase_deg=0.0
bodi.gardulus.p.speed_deg_s=360.0
bodi.gardulus.b.speed_deg_s=359.640359640
bodi.gardulus.orbit.xml_size_bytes=1024
bodi.gardulus.orbit.max_scale=1.5
bodi.gardulus.sync.relative=true
bodi.gardulus.sync.coherence=true
```

The properties document the intended control surface for XML-scoped overrides. The runtime API receives the concrete XML size so the geometric radius can be tied to the actual observed structure.

## Resonance and Tone

The P/B pair can be compared against XML structural changes, Wiggle satisfaction counts, Bodi Witness sequences, connection counts, memory pressure, item/relation counts, containment boundaries, and the 3D position of each software orbit.

Useful Science measures include:

```text
phase difference over time
speed difference over time
frequency ratio
coherence distribution
phase-lock intervals
3D orbital separation over time
orbital-plane relationship
structural event → phase/position relationship
```

These permit software-defined resonance and tone studies. A measured correlation remains a correlation until controlled analysis supports a stronger conclusion.

## Safety and Semantics

```text
software phase     ≠ physical orbit
software speed     ≠ particle velocity
software radius    ≠ physical size
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
P 3D orbit B 3D orbit
     \     /
 relative synchronization
        ↓
   Gardulus.II
        ↓
 Science listeners
```

The governing rule is:

> **Wiggle discovers structure; Bodi witnesses change; bodisysctl observes the local system; P/B telemetry supplies reproducible 3D phase relationships whose scale is bounded by the XML structure; Gardulus.II makes those observations available for disciplined scientific analysis.**
