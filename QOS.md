# Quality of Service (QoS) over standard TCP/HTTP

This document explains how SLeeLa's **advisory timing** layer
([`http-3.0/http3_timing.{h,c}`](http-3.0/)) maps onto **Quality of Service**
for ordinary TCP and HTTP, and how to use it *carefully* from both the SLeeLa
and Java sides.

The guiding stance is the same as the rest of the timing work: **measure, don't
misrepresent.** QoS here is observed and reported; it does not silently reshape
traffic, and it never claims guarantees the transport cannot make.

## What QoS means here

Classic network QoS covers four families of metric. SLeeLa's timing layer
provides an application-level *observation* of each, over whatever transport
carries the packets (plain TCP, HTTP/1.1, HTTP/2, or the HTTP 3.0 envelope):

| QoS metric | SLeeLa timing concern | Flag |
|---|---|---|
| **Throughput / rate** | max speed — minimum inter-arrival gap | `OVER_RATE` |
| **Latency / deadline** | on time — arrival within `deadline + grace` | `LATE` |
| **Jitter** | balance — inter-arrival deviation within a band | `UNBALANCED` |
| **Reliability** | carrier certainty — running fraction of clean packets, in `[0,1]` | — |
| **Loss / gaps** | continuity — missing or out-of-order arrivals in the sequence | `GAP` |

These are **advisory**: the pipeline records `late_packets`,
`over_rate_packets`, `unbalanced_packets`, and a `carrier_certainty()` estimate.
Nothing new travels on the wire, and no packet is rejected by QoS.

### Continuity (`GAP`) — loss and reordering

Classic QoS also cares about *packet loss* and *reordering*. Over standard
TCP the byte stream is delivered reliably and in order, so loss shows up as
**added latency** (retransmission) rather than missing bytes — which the `LATE`
flag already captures. Where the observer sees an explicit application-level
sequence number (for example the HTTP 3.0 NONCE ordering, or an RMI call index),
a break or backward step in that sequence is reported as `GAP`:

- `GAP` is **advisory** like the others: it flags a discontinuity to log or act
  on; it does not itself request retransmission (the transport already does that
  for TCP) and it never drops or reorders packets.
- On a datagram transport with no automatic retransmission (e.g. QUIC/UDP
  paths), `GAP` is the primary loss signal and should be paired with the
  reliability estimate below.
- A `GAP` lowers `carrier_certainty()` the same way a `LATE`/`OVER_RATE`/
  `UNBALANCED` packet does — a discontinuity is a not-clean packet.

## QoS is a hint, not a guarantee (standard TCP/HTTP)

Over standard TCP/HTTP there is **no end-to-end delivery-time guarantee**. TCP
gives reliable, ordered bytes; it does *not* promise a deadline. HTTP layers
request/response semantics on top but adds no timing guarantee either. So:

- Treat `carrier_certainty()` as an **estimate of recent behavior**, not a
  contract. A high value means the path has *been* dependable, not that the next
  packet *will* arrive on time.
- Treat `LATE` / `OVER_RATE` / `UNBALANCED` as **signals to act on** (log, shed
  load, back off, pick another route), not as protocol errors.
- "On time" is measured against a **caller-supplied clock**. There is no
  synchronized network clock in this reference, so a deadline is meaningful only
  relative to the reference the caller provides. Say so wherever you surface it.

## Using it carefully — SLeeLa side

Feed each arrival's timestamp to the pipeline; read the certainty to decide.

```text
flags = http3_pipeline_observe_timing(pipe, arrival_ns, deadline_ns);  // 0 == clean
c     = http3_pipeline_carrier_certainty(pipe);                        // 0.0 .. 1.0
```

Careful practices:

- **Use a monotonic clock** for `arrival_ns` (elapsed time), not a wall clock
  that can jump backward; jitter/rate math assumes time only moves forward.
- **Pass `deadline_ns = 0` when there is no real deadline.** A fabricated
  deadline produces meaningless `LATE` flags. Only set a deadline you can defend.
- **Do not gate correctness on QoS.** Timing is advisory; business logic must
  still run (or fail) on its own merits. Use QoS to *prioritize*, not to decide
  whether a request was valid.
- **Bound your reactions.** If low certainty triggers back-off, cap the back-off
  and make it recover — don't let an advisory metric wedge the system.

## Using it carefully — Java side

The Java RMI facade ([`rmi/java/.../SleelaRmiClient.java`](rmi/java/com/mearvk/sleela/rmi/SleelaRmiClient.java))
is where TCP/RMI QoS gets handled on the Java end. Careful practices, matching
what the code already does and where it should be extended:

- **Probe liveness without throwing.** `SleelaRmiClient.isHealthy()` already
  swallows the exception and returns a boolean — use that for QoS/administration
  polling instead of catching exceptions inline.

  ```java
  if (!client.isHealthy()) {
      // reliability signal: treat as low carrier certainty, back off / reroute
  }
  ```

- **Set explicit timeouts.** Plain RMI/TCP calls can block indefinitely. For QoS
  you *must* bound them — set `sun.rmi.transport.tcp.responseTimeout` (or wrap
  calls in a `Future` with a timeout) so a stalled carrier surfaces as `LATE`
  rather than a hung thread. A call with no timeout has no QoS.
- **Validate endpoints up front.** `SleelaRmiEndpoint` is a `record` that already
  rejects blank hosts/service names and out-of-range ports in its compact
  constructor — keep that discipline; do not construct endpoints from unchecked
  input.
- **Close what you own.** `SleelaRmiClient` implements `AutoCloseable`; use
  try-with-resources. A client lookup does not own the registry/export, so
  releasing it must not tear down the server (the current `close()` is
  deliberately a no-op — keep it that way).
- **Keep the remote contract narrow.** QoS instrumentation belongs *around* the
  `SleelaRemote` contract (`serviceName` / `invoke` / `health`), not inside it —
  do not widen the interface with timing parameters; measure at the call site.
- **Never expose unauthenticated RMI publicly.** QoS does not substitute for
  security; see the deployment notes in [`RMI.md`](RMI.md).

## Mapping to DiffServ / HTTP priorities (optional)

If a deployment wants to influence the *actual* network, the advisory signals
can drive standard mechanisms — without this layer pretending to be them:

- Set **DSCP / ToS** on the socket for coarse network QoS classes.
- Use **HTTP/2 or HTTP/3 stream priorities** where the transport supports them.
- Apply **application-level pacing** (respect the min-gap) before send.

The timing layer *informs* these choices; it does not perform them itself.

## Cross-language parity

The C reference (`http3_timing.c`) and the Python reference
(`http3_flow.py`, `Timing`) compute **identical** flags and carrier certainty
for the same arrival sequence — so a QoS reading is reproducible regardless of
which side observes it. See [`http-3.0/FLOW.md`](http-3.0/FLOW.md) and
[`http-3.0/STATUS.md`](http-3.0/STATUS.md).

## Worked example — reading a short arrival sequence

The flags are per-packet; certainty is cumulative. For a stream with a 10 ms
minimum gap and a per-packet deadline, a typical reading looks like:

```text
pkt  arrival(ms)  gap(ms)  vs deadline   flags          carrier_certainty
 1        0          —        on time     (clean)              1.00
 2        9          9        on time     OVER_RATE            0.50   # gap < 10 ms min
 3       21         12        on time     (clean)              0.67
 4       55         34        LATE + late  LATE                0.50   # missed deadline+grace
 5       65         10        on time     (clean)              0.60
 6       —          —        (missing)    GAP                  0.50   # seq 6 never arrived
```

Read it as *recent behavior*, not a promise: certainty rises as clean packets
accumulate and dips on every flagged packet. Use the trend to prioritize or back
off — never to decide whether a request was valid.

## Changelog

- **Continuity / `GAP` metric added.** Loss and reordering are now an explicit
  advisory QoS metric alongside throughput, latency, jitter, and reliability,
  with guidance for both reliable (TCP) and datagram (QUIC/UDP) transports.
- **Worked arrival-sequence example added** to show how per-packet flags and the
  cumulative `carrier_certainty()` estimate relate.

## Summary

- QoS here = **observed** throughput, latency, jitter, reliability, and
  continuity (loss/reordering) over standard TCP/HTTP.
- It is **advisory** — a set of signals and a `[0,1]` certainty, never a
  guarantee and never a gate on correctness.
- On the **SLeeLa** side: monotonic clocks, honest deadlines, bounded reactions.
- On the **Java** side: explicit timeouts, non-throwing health probes, validated
  endpoints, `AutoCloseable` clients, a narrow remote contract.
