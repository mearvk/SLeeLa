# SYNCHRO.md — Synchro: honest packet dispatch + measurement (Sleela 1.3)

    Feature:        Synchro (measurement built-ins)
    Syntax version: 1.3
    Core module:    impl/core/sleela_synchro.{h,c}
    Source surface: synchroOpen / synchroDispatch / synchro* / synchroReport / synchroClose
    Companion:      synchro/ (the original Python reference); MUNCTION.md
    Status:         Implemented in the direct Sleela engine build

---

## 1. What Synchro is

Synchro is the C/C++ re-authoring — for the **direct Sleela engine** — of the
`synchro/` packet dispatch and measurement layer. It sends timestamped probes to
a destination, matches replies, and reports the **measured** latency
distribution per destination.

Synchro is **honest**: it makes **no delivery-time guarantee**, because none is
physically possible for arbitrary destinations (the speed of light is a hard
floor; the Internet is best-effort). What Synchro reports is what actually
happened — measured RTTs to microsecond resolution, with the loss rate stated
alongside. A lost probe is recorded as loss, never as a fabricated latency.

## 2. Source surface (syntax 1.3)

Synchro is exposed as contextual built-ins recognized in call position. A
Wrapper™ using them declares `#sleela 1.3`.

| Built-in | Result | Purpose |
|---|---|---|
| `synchroOpen(host, port)` | `int` | Open a UDP probe to `host:port`; returns a VM-local handle or `-1`. |
| `synchroDispatch(handle, len, timeoutMs)` | `int` | Send one timestamped probe of `len` bytes; return the measured RTT (µs) or `-1` on honest loss/timeout. |
| `synchroSent(handle)` | `int` | Dispatches so far. |
| `synchroReceived(handle)` | `int` | Matched replies so far. |
| `synchroLoss(handle)` | `int` | Loss per 1000 dispatches (0..1000). |
| `synchroMean(handle)` | `int` | Mean RTT (µs), or `-1` if nothing received. |
| `synchroMin(handle)` / `synchroMax(handle)` | `int` | Min/max RTT (µs), or `-1`. |
| `synchroP95(handle)` | `int` | 95th-percentile RTT (µs), or `-1`. |
| `synchroReport(handle)` | `String` | A one-line honest report. |
| `synchroClose(handle)` | `null` | Release the probe. |

### Example

```sleela
#sleela 1.3
class Ping {
    void main() {
        int p = synchroOpen("127.0.0.1", 9);
        if (p >= 0) {
            int rtt = synchroDispatch(p, 32, 50);
            print(synchroReport(p));   // sent=1 recv=0 loss=1000permille mean=-1us ...
            synchroClose(p);
        }
    }
}
```

See [`impl/examples/synchro_measure.sleela`](impl/examples/synchro_measure.sleela).

## 3. Engine model

Synchro follows the same architecture as the Sleela network layer: source-level
calls are ordinary calls; the compiler lowers the recognized names to VM
opcodes; the C core performs the runtime operation against bounded, VM-owned
state.

```
.sleela source (synchroOpen/Dispatch/...)
   -> compiler tryEmitBuiltin (gated #sleela 1.3)
   -> OP_SYN_OPEN / OP_SYN_DISPATCH / OP_SYN_STAT / OP_SYN_REPORT / OP_SYN_CLOSE
   -> C VM dispatch loop
   -> VM-owned probe table (SL_SYNCHRO_MAX)
   -> sleela_synchro.c (connected UDP socket, monotonic-clock timing)
```

Probe handles are VM-local integers, never raw OS descriptors. Timing uses a
monotonic clock; retained samples are bounded (`SL_SYNCHRO_MAX_SAMPLES`).

## 4. Nordshrift SST

A `.sst` sheet may declare a first-class `measure:` block; see
[`impl/nordshrift/SST.SYNCHRO.md`](impl/nordshrift/SST.SYNCHRO.md).

## 5. Testing

- C smoke: `make test-synchro` (`impl/core/synchro_smoke.c`).
- Language: `impl/examples/synchro_measure.sleela` (run by `make test-sleela`).

## 6. Relationship to `synchro/`

The Python project under `synchro/` remains the reference implementation and the
prose rationale (the speed-of-light / best-effort honesty argument). This module
is its engine-native counterpart: the same honest-measurement contract, executed
directly by the Sleela C core.
