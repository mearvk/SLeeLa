# CPU keep-warm idle floor

A modest, scientifically-bounded background load that keeps each of the
server's CPUs lightly busy — a target band of **~11% (floor) up to 14%
(ceiling) per core** — even when no request traffic is present. The intent is
to keep the server *warm*: discourage cores from dropping into deep idle
C-states / low P-states, so the first request after a quiet period does not pay
a frequency-ramp / cold-path penalty.

## Policy

The feature is **ON by default** (a deliberate, stated operational preference:
keep the server warm). An **administrator may disable it** with `--no-warm`, or
retune the band. The warmer runs **one thread per CPU**, with the thread count
being the detected online CPU count **clamped to the range 1–16**.

## Why a closed-loop controller, not a fixed sleep ratio

"11% of a CPU" is not a fixed busy/sleep ratio. Measured utilization drifts with
frequency scaling, turbo, and scheduler placement, so a hard-coded duty cycle
reads as very different utilization across machines. The warmer is therefore a
**closed-loop controller**: each per-core thread measures its *own* CPU time
(`CLOCK_THREAD_CPUTIME_ID`) against wall-clock time (`CLOCK_MONOTONIC`) over a
short window and adjusts its busy/idle duty cycle with a proportional controller
to converge into the configured band. It targets the **middle of the band**
(12.5% for the 11–14 default) so normal jitter stays inside the edges.

The controller is the defensible way to actually land near 11–14% rather than
hope a magic ratio does. The convergence test (`cpu_warmer-test.cpp`) measures
the achieved utilization two independent ways (the controller's own estimate and
process-level CPU time ÷ thread count) and asserts both land in band; on the
build host both read ~12.5%.

## Deference to real work

Warmer threads run at the lowest practical scheduling class (`SCHED_IDLE` on
Linux where available). They fill otherwise-idle time only; genuine request
handling always preempts them, so the floor never steals measurable throughput.
The busy phase does a small, non-optimizable arithmetic kernel (fed through a
`volatile` sink) that touches the pipeline and caches without allocating or
doing any I/O.

## Bounds (always enforced)

The configuration is clamped into a safe range so it can never become a runaway
hot loop:

- floor ∈ [1, 90] %, ceiling ∈ [floor, 95] %;
- cores ∈ [1, 16];
- control window ∈ [50 ms, 2000 ms];
- duty cycle ∈ [0.5%, min(95%, ceiling+10%)] — never 0 (so it always warms a
  little) and never a full-core spin.

## Control surface (startup flags or config file)

```sh
--no-warm            # administrator disables the keep-warm floor
--warm-floor N       # target floor percent per core (default 11)
--warm-ceiling N     # target ceiling percent per core (default 14)
--warm-cores N       # max warmer threads, clamped 1..16 (default 16)
```
```text
# or in the config file:
cpu-warm         = on        # on (default) | off
cpu-warm-floor   = 11
cpu-warm-ceiling = 14
cpu-warm-cores   = 16
```

The command line overrides the config file.

## Observability

`/__sleela/status` reports:

```json
"cpu_warm": true,
"cpu_warm_floor_pct": 11,
"cpu_warm_ceiling_pct": 14,
"cpu_warm_cores": 16,
"cpu_warm_measured_pct": 12.50
```

and the startup log records `cpu-warm=on floor=11% ceiling=14% cores=N` (or
`cpu-warm=off (administrator disabled)`).

## Honest caveats

- The measured percentage is a per-thread average and is approximate; it will
  jitter with real load and with the host's power-management behaviour.
- On a host whose governor already prevents deep idle, or in a container with
  constrained CPU shares, the achieved floor may differ; the controller still
  aims for the band but cannot override an external cap.
- This keeps cores *warm*; it is not a benchmark or a load generator, and it is
  intentionally incapable of consuming a whole core.

## Tests

- `native/cpu_warmer-test.cpp` — convergence test (`make check`).
- `native/warm-live-test.sh` — live status + the `--no-warm` admin override.
