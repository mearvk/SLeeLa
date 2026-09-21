# SERVER.EDITION.md — The SLeeLa Server Edition

    Location:   server-edition/
    Since:      Sleela syntax 1.3
    Runs on:    the direct Sleela engine (impl/build/sleela)
    Companions: MUNCTION.md · SYNCHRO.md · TIMINGS.md · RMI.md · QOS.md
    Stance:     quiet by default; processes her own inputs; measures, never fabricates.

---

## 0. Who she is

The **Server Edition** is a small, self-contained local Sleela server. She is
born here, in [`server-edition/`](server-edition/). She **runs quietly** — no
public port, no network wait, no chatter — and she **takes and meaningfully
processes at least her own inputs**: the request lines she owns under
`inbox/`, driven through the real engine surfaces she was given —
**Munction** reaches, **Synchro** measurement, and the **best-of** route/accuracy
selector (see [`TIMINGS.md`](TIMINGS.md)).

She is a SLeeLa program. `Server.sleela` *is* the server; the `serverd` launcher
just resolves the engine and runs her over her inbox, appending honest results
to her own state log.

---

## 1. Layout

```text
server-edition/
  serverd                 quiet launcher (sh): resolve engine + run one pass
  src/Server.sleela       the server herself (syntax 1.3)
  inbox/                  HER OWN INPUTS: newline-delimited request lines
    requests.txt          (runtime; created on first run)
    .keep
  state/                  HER OWN STATE: what she did with her inputs
    results.log           (runtime; appended each pass)
    intake                (runtime; her self-processing pipe frames)
    .keep
  .gitignore              ignores the runtime files, keeps the folders
SERVER.EDITION.md         this document
```

Only `serverd`, `src/Server.sleela`, the `.keep` files, and `.gitignore` are
source. `inbox/requests.txt`, `state/results.log`, and `state/intake` are
runtime artifacts she writes herself.

---

## 2. Running her

```sh
# One quiet pass over the current inbox (default):
server-edition/serverd
#   -> serverd: processed inbox -> .../state/results.log (quiet)

# Feed her one more of HER OWN inputs (a timestamped tick), then run:
server-edition/serverd --tick

# Run once and also echo the last results to the terminal:
server-edition/serverd --foreground

server-edition/serverd --help
```

She resolves the engine from `SLEELA_BIN`, else `bin/sleela`, else
`impl/build/sleela`, and the SHA-256 execution manifest from
`SLEELA_SHA256_MANIFEST`, else `security/sha256-manifest.json`. Build the engine
first if needed:

```sh
cd impl && make VERIFY_MANIFEST=../security/sha256-manifest.json build/sleela
```

---

## 3. What "processes her own inputs" means

Each pass she:

1. **Takes her own inputs.** She opens `inbox/requests.txt` (a file she owns)
   with the 1.1 file built-ins and reads her request batch. If there is no
   inbox, she stays idle and quiet — she never blocks waiting on the outside.
2. **Reaches over each input (Munction).** She runs a `Munction` reach into her
   own `state/intake` pipe: `start → connect(pipe:…) → send(datum) →
   thatch(frame,record) → consume → latch → closeWithReceipt`. The reach is
   **coherent** (offered bytes == acknowledged bytes) and **receivable** (it
   yields a receipt she records).
3. **Measures honestly (Synchro).** She opens a `synchro` probe on local
   loopback and dispatches; the measured RTT (or an honest loss) is folded into
   her best-of selector so her routing decisions rest on real data.
4. **Decides (best-of).** She scores her candidate routes across
   data/decisions/costs/versions and picks the best-of route — the parts of the
   internet she would use to adjust Synchro/RMI packet accuracy (see
   [`TIMINGS.md`](TIMINGS.md) §4–§5).
5. **Reports quietly.** She appends an honest per-request line and a summary to
   `state/results.log`. Nothing is printed to the terminal by default beyond one
   "processed inbox" line.

A real run's log lines look like:

```text
request 1: reached; receipt{name=intake scheme=pipe ... verbs=7 sent=20 ack=20 recv=1 interims=[frame,record] latched=true coherent=true outcome=REACHED}
server: best-of winner index 0
server: chosen route -> local-sdps sdps://127.0.0.1:19866 timeout=200ms payload=64B gap=5ms flags=[crypto,pacing] version=2 replays=3 mean=-1us loss=1000permille certainty=0 score=12000
server: processed requests=1 reaches=1 probes=1
```

The `mean=-1us loss=1000permille` on loopback discard is **honest**: nothing
answered the probe, so she reports loss rather than inventing a latency. That is
the whole point — she processes and reports what actually happened.

---

## 4. Her decisions are configured, not hard-coded

Her best-of selector is configured in `Server.sleela::configureSelector()`:

```sleela
selector = bestOfNew();
bestOfWeight(selector, 0, 55);  // data
bestOfWeight(selector, 1, 30);  // decisions (QoS certainty)
bestOfWeight(selector, 2, 10);  // costs
bestOfWeight(selector, 3, 5);   // versions
bestOfMinVersion(selector, 1);
bestOfCostBudget(selector, 100);
bestOfCandidate(selector, "local-sdps", "sdps://127.0.0.1:19866", 200, 64, 5, 5, 2, 10, 3);
bestOfCandidate(selector, "local-tcp",  "tcp://127.0.0.1:8080",   500, 128, 0, 2, 1, 20, 1);
```

Change the weights, gates, or candidate routes and her routing decisions change
accordingly — the same transparent, honest method documented in
[`TIMINGS.md`](TIMINGS.md).

---

## 5. Relationship to RMI

She is deliberately **local and quiet**, not a public RMI service. When a
deployment wants her business logic reachable remotely, the established path is
RMI ([`RMI.md`](RMI.md)): *Java transports, SLeeLa decides.* The Server Edition
is the SLeeLa side of "decides" — her best-of choice is exactly the
route/timeout/version an RMI (or Munction) caller should aim packets at. She
never becomes an unauthenticated public port on her own.

---

## 6. Safety & bounds

- **Quiet by default.** One terminal line per pass; everything else goes to her
  own log.
- **Her own inputs only.** She reads a file she owns; she opens no listener and
  waits on no external peer.
- **Bounded resources.** Munction reaches, Synchro probes, and best-of selectors
  are VM-owned bounded handles.
- **Honest.** She measures and reports; she never fabricates accuracy and never
  claims a delivery guarantee (QOS.md / TIMINGS.md).
- **Fail-closed.** She runs under the engine's SHA-256 execution gate like every
  other Sleela program.

---

She is born here: a quiet local Sleela server who takes her own inputs, reaches
over them, measures honestly, decides transparently, and keeps her own record.
