# BodiSysCtl and Gardulus.II

`bodisysctl` is the Linux-native observation companion for Bodi/Wiggle XML structures. It monitors a bounded local picture of memory, connections, relations, items, status, iterations, P/B phase, and containment count.

The integer argument is an observation interval in milliseconds. P and B are deterministic software telemetry phases rotating around the XML object representation in memory; they are not physical orbital or electromagnetic measurements.

```text
XML / SLeeLa structure
        ↓
      Wiggle
        ↓
       Bodi
        ↓
   bodisysctl
     ↙     ↘
 P-orbital B-orbital
     \     /
   Gardulus.II
       ↓
 science listeners
```

## Command

```text
bodisysctl INTERVAL_MS ITERATIONS [LOG_FILE] [UNIX_SOCKET]
```

`ITERATIONS=0` means run until SIGINT/SIGTERM. A finite count causes the monitor to unload naturally when complete.

## Gardulus.II

The native API is in `tools/bodi/gardulus_ii.h` and provides `gardulus_run()` and `gardulus_emit_json()`. JSON Lines may be written to stdout, a file, or a connected Unix socket. `GardulusII.java` provides a local Java VM consumer for file or stream telemetry.

A sample records iteration, timestamp, input integer, P/B phase, memory totals, connection/relation counts, items, status, and containment count.

The system is observational. Telemetry does not gain control authority over Bodi and counters are not proof of causation. Correlation must remain distinct from causation.

## Installation

`tools/bodi/install-bodi.sh` installs/compiles the C and C++ monitors when a local compiler is available and writes the XML/Gardulus configuration under the SLeeLa prefix.

```properties
bodi.xml.structures=tree,record,reference,state,command
bodi.gardulus.enabled=true
bodi.gardulus.interval_ms=1000
```

The governing rule is: **Wiggle discovers structure; Bodi witnesses change; bodisysctl observes the local system; Gardulus.II makes those observations available for disciplined scientific analysis.**
