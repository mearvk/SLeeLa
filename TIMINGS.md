# TIMINGS.md — The SLeeLa Timing & Best-Of Selection Model

    Scope:      Synchro measurement · Munction/RMI packet routing · Best-of selection
    Since:      Sleela syntax 1.3
    Core:       impl/core/sleela_synchro.{h,c}, sleela_munction.{h,c}, sleela_bestof.{h,c}
    Companions: SYNCHRO.md · MUNCTION.md · QOS.md · RMI.md · SLEELA_TIME_API.md
    Stance:     measure, don't misrepresent. Select and report; never fabricate
                accuracy, never promise a delivery guarantee the transport cannot make.

---

## 0. Why this document exists

SLeeLa can *measure* how the network behaved (Synchro), *reach* over it
(Munction), and *call* across it (RMI). This document ties those together with
the **best-of** method: a configurable way to decide **which parts of the
internet** — routes, settings, flags, replays — to use so that Synchro's
measured accuracy and the packets sent over Munction/RMI are as good as the
observed conditions allow.

The governing rule is the same one QOS.md sets out: timing here is **observed
and advisory**. Best-of *chooses among* candidates using real measurements; it
never claims a route is good without evidence and never turns an advisory signal
into a guarantee.

---

## 1. The three clocks and one honesty rule

| Concern | Source | Unit | Honesty rule |
|---|---|---|---|
| **Round-trip time (RTT)** | Synchro dispatch, monotonic clock | microseconds (µs) | Measured per probe; a lost probe is `-1`, never a guessed number. |
| **Jitter** | derived: mean \|RTTₙ − RTTₙ₋₁\| | µs | Only over *matched* replies; undefined (0) with fewer than two samples. |
| **Loss / continuity** | dispatches − matched replies | permille (0..1000) | A timeout is loss, counted honestly; it is not retried silently into a lower number. |

All timing uses a **monotonic** clock (elapsed time that never steps backward),
exactly as QOS.md requires. Wall-clock time is never used for RTT/jitter math.

> There is **no synchronized network clock** in this reference. Every "on time"
> or deadline judgment is relative to the caller-supplied reference; best-of
> only compares *SLeeLa's own* consistent measurements against each other.

---

## 2. Synchro: the measurement primitive

Synchro (`SYNCHRO.md`) is the honest input to everything here. One dispatch:

```
synchroDispatch(probe, payloadLen, timeoutMs)
   -> measured RTT in µs   (a matched reply)
   -> -1                   (honest loss / timeout within the budget)
```

Its cumulative readings (`synchroMean`, `synchroP95`, `synchroLoss`, …) are the
raw material. Synchro decides *nothing* about routing — it only reports what
happened.

---

## 3. QoS decisions layer (advisory)

Best-of's **decisions** axis consumes the QOS.md advisory model. Each measured
packet is clean or carries one or more advisory flags; the running fraction of
clean packets is the **carrier certainty** in `[0..1]` (expressed here as
`0..1000` permille):

| QoS flag | Meaning | Effect on certainty |
|---|---|---|
| `OVER_RATE` | inter-arrival gap below the min-gap | lowers |
| `LATE` | arrived after `deadline + grace` | lowers |
| `UNBALANCED` | jitter outside the allowed band | lowers |
| `GAP` | missing/out-of-order in the sequence | lowers |
| *(clean)* | none of the above | raises |

Best-of derives its per-candidate certainty from **delivered fraction dampened
by jitter** — the same spirit as `carrier_certainty()`: a candidate that
delivers most probes with low jitter earns high certainty; one that loses or
jitters earns low certainty. **An unmeasured candidate has certainty 0** — it is
never assumed good.

---

## 4. The best-of method

Best-of (`impl/core/sleela_bestof.{h,c}`) is a small, transparent selector. You
configure it, register candidate routes, fold in Synchro measurements, and ask
for the winner. It answers: *given my data, decisions, costs, and version
requirements, which route + settings + flags + replays should I use?*

### 4.1 The four configurable axes

`bestOfWeight(handle, axis, weight)` sets each axis weight in `0..100`
(defaults `data=50 decisions=30 costs=10 versions=10`):

| Axis | id | What it rewards | Built from |
|---|---|---|---|
| **data** | 0 | Lower RTT, lower loss, lower jitter | measured Synchro readings |
| **decisions** | 1 | Higher carrier certainty | QOS.md advisory model |
| **costs** | 2 | Lower cost (cheaper routes) | per-candidate `cost` vs. budget |
| **versions** | 3 | Higher protocol/packet-version fitness | per-candidate `version` vs. `min-version` |

These are exactly the four things the task asks to configure: **data,
decisions, costs, versions.**

### 4.2 A candidate = a slice of the internet

`bestOfCandidate(handle, name, route, timeoutMs, payloadLen, gapMs, flags,
version, cost, replays)` registers one candidate. It bundles the parts of the
internet best-of can choose among:

| Field | Kind | Meaning |
|---|---|---|
| `route` | **route** | endpoint / URI (`sdps://…`, `tcp://…`, an RMI endpoint, …) |
| `timeoutMs`, `payloadLen`, `gapMs` | **settings** | dispatch budget, probe size, min-gap pacing |
| `flags` | **flags** | bitset: `crypto=1 retry=2 pacing=4 dscp=8 replay=16` |
| `version` | **version** | protocol/packet version this route speaks |
| `cost` | **cost** | relative expense (budget-gated) |
| `replays` | **replays** | how many probe repeats to average for this route |

### 4.3 Feeding measurements

Probe a candidate's `route` with Synchro and fold each result in:

```
bestOfRecord(handle, candidateIdx, rttUs)   // rttUs from synchroDispatch, or -1 for loss
```

Best-of maintains, per candidate: running **mean RTT**, **jitter**, **loss**,
and the derived **certainty**. Read them with `bestOfMean/bestOfLoss/
bestOfJitter/bestOfCertainty(handle, idx)`.

### 4.4 The transparent score

`bestOfScore(handle, idx)` is a weighted sum of four normalized terms, each in
`0..1000` (higher is better):

```
score =  w_data      * data_term        // (rtt_goodness + delivered) / 2
       + w_decisions * certainty         // 0..1000 carrier certainty
       + w_costs     * (1000 - cost_norm)// cheaper -> higher
       + w_versions  * version_fitness   // >= min_version -> 500 + 100*over, capped
```

`rtt_goodness` maps a lower mean RTT to a higher number against a fixed
reference ceiling (a *scaling constant*, not a limit or a promise). A candidate
with **no measurements** contributes `data_term = 0` and `certainty = 0`, so it
scores from its static cost/version fitness only — a measured-good route always
beats an unproven one.

### 4.5 Gates and the winner

`bestOfBest(handle)` returns the highest-scoring candidate that also passes:

- **version gate** — `version >= min-version` (`bestOfMinVersion`);
- **cost budget** — `cost <= cost-budget` (`bestOfCostBudget`, `0` = unbounded).

`bestOfChoice(handle)` renders the winner as a one-line description of the
selected parts of the internet, e.g.:

```
edge sdps://edge:19866 timeout=200ms payload=64B gap=5ms flags=[crypto,pacing] version=2 replays=3 mean=845us loss=0permille certainty=1000 score=98450
```

`bestOfReport(handle)` prints every candidate with a `*` on the winner — the
full, inspectable decision.

---

## 5. Adjusting Synchro / Munction / RMI accuracy

The winning choice is *applied*, not merely reported:

```text
Synchro probes each candidate route ─┐
                                     ▼
                              best-of records
                                     │
                        best-of scores + gates
                                     ▼
                       best-of chooses route+settings+flags+replays
                                     │
        ┌────────────────────────────┼─────────────────────────────┐
        ▼                            ▼                              ▼
  Synchro re-probes           Munction connects            RMI call is aimed
  the winning route           the winning route            at the winning route
  with its replays            with its flags               with its timeout/version
  → tighter accuracy          → coherent reach             → bounded, versioned call
```

- **Synchro accuracy** improves by concentrating probes (and `replays`) on the
  route best-of found most dependable, and by using its `payloadLen`/`gapMs`
  settings so the measured distribution reflects real intended traffic.
- **Munction packets** use the winning `route` + `flags` (e.g. `crypto`,
  `pacing`) so a reach travels the path best-of scored highest; coherent send +
  receipt still apply (MUNCTION.md).
- **RMI packets** are aimed at the winning endpoint with its `timeoutMs` and
  `version` — honoring QOS.md's "set explicit timeouts" and "pick another route"
  guidance, and RMI.md's "Java transports, SLeeLa decides."

Because the winner is chosen from measured data, this *adjusts* accuracy toward
what the network has actually been able to deliver — never toward a fabricated
target.

---

## 6. Replays

`replays` is the count of probe repeats best-of averages for a candidate. More
replays tighten the measured mean/jitter (lower sampling error) at higher cost
and time. Replays are a **settings** dial, not a guarantee: replaying a lossy
route surfaces the loss more clearly; it does not make the route reliable.

---

## 7. Worked reading

Config `data=70 decisions=20 costs=5 versions=5`, `min-version=1`,
`cost-budget=50`, three candidates measured over loopback:

```
best-of weights[data=70 decisions=20 costs=5 versions=5] min-version=1 cost-budget=50 candidates=3
  * edge  sdps://edge:19866 v2 cost=10 flags=[crypto,pacing] mean=845us  loss=0permille   certainty=1000 score=98450
    relay tcp://relay:8080  v1 cost=30 flags=[retry]         mean=40000us loss=300permille certainty=700  score=79950
    spare tcp://spare:9090  v2 cost=5  flags=[none]          mean=-1us    loss=0permille   certainty=0    score=8475
```

Read it as *recent behavior*: `edge` wins because it is measured fast, clean,
and version-fit within budget; `relay` is penalized for latency + loss; `spare`
is unproven (certainty 0) and only scores from static fitness. Change the
weights and the winner can change — the method is a policy dial, not a verdict.

---

## 8. Bounds & safety

- All resources are **VM-owned and bounded** (`SL_BESTOF_MAX` selectors,
  `SL_BESTOF_CANDIDATES` per selector, `SL_SYNCHRO_MAX` probes) — the same
  discipline as sockets/files.
- Best-of **never gates correctness**: it prioritizes and routes; business logic
  still runs on its own merits (QOS.md).
- Reactions must be **bounded** — a low-certainty result should back off and
  recover, not wedge the system.
- Nothing new travels on the wire because of best-of; it selects from what was
  measured and names what it chose.

---

## 9. Source-language surface (syntax 1.3)

```
bestOfNew() -> handle
bestOfWeight(h, axis, weight)           // axis: 0 data · 1 decisions · 2 costs · 3 versions
bestOfMinVersion(h, minVersion)
bestOfCostBudget(h, budget)             // 0 = unbounded
bestOfCandidate(h, name, route, timeoutMs, payloadLen, gapMs, flags, version, cost, replays) -> idx
bestOfRecord(h, idx, rttUs)             // rttUs from synchroDispatch, or -1 for loss
bestOfMean(h, idx) · bestOfLoss(h, idx) · bestOfJitter(h, idx) · bestOfCertainty(h, idx)
bestOfScore(h, idx) · bestOfBest(h) -> idx (-1 none)
bestOfChoice(h) -> String · bestOfReport(h) -> String
bestOfClose(h)
```

Flag bits combine by addition: `crypto=1 retry=2 pacing=4 dscp=8 replay=16`.

See [`impl/examples/bestof_route.sleela`](impl/examples/bestof_route.sleela) for
a full Synchro → best-of → choice program, and `make test-bestof` for the C
smoke test.

---

Timing here is **measured**, best-of is **transparent**, and the choice is
**named**. Use it to prioritize routes and tune accuracy — never to promise a
guarantee the internet cannot make.
