<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-3.0: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-3.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/3.0 protocol metadata generation |
| Lives in | `http-3.0/` (envelope/naming/pipeline `http3_*.c`, integrity `http3_mac.c`/`http3_intactx.c`/`http3_basket.c`, crypto substrate `crypto_*.c`, `station/`, `http3_server.c`, Python `http3_flow.py`) |
| Writeup type | Tutorial, because the pipeline demo, server self-test, crypto self-test and Python tests all run end to end here and each step below was executed. |
| Verified | 2026-11-13 against `master` e048707fd: `make test` (demo + station + Python), `make server-test`, `make selftest` + `./crypto_selftest`, `sleelactl validate http-3.0` all pass. The demo fails if a stale baseline file is present (see Step 4). |

## What it is

SLeeLa HTTP 3.0 is the project's own **application protocol**, not IETF HTTP/3 (RFC 9114). The README says it is "not a claim of IETF HTTP/3 semantics beyond the selected transport carrier (HTTP/3 + QUIC may provide the transport)". The implemented server in this directory carries H3 envelopes in HTTP/1.x over `sleela_net` (README "Architecture": "HTTP/1.x request / connection"); no QUIC or RFC 9114 framing code is in the module.

What it adds to the `http-2.0` envelope is an **integrity gate** between parse and dispatch: a SipHash-2-4 keyed-MAC `DIGEST`, a monotonic `NONCE` replay guard, an `INTACTX` 64-bit host fingerprint whose variance triggers a tamper `RESET`, and a fixed 14-item `BASKET` of goods and services priced per gram. It also has capability handshake levels (L1-L4), carrier-certainty timing (advisory), a 160-bit logical port, a crypto substrate (X25519/HKDF/AES-GCM via OpenSSL, key distribution server) and "station" TCP-flag signalling.

## Before you start

* `cc`, `make`, `python3`; OpenSSL development files for the crypto self-test (the protocol core, demo and server need only libc).
* Build products land in `http-3.0/` (`*.o`, `http3_pipeline_demo`, `http3_server`, `crypto_selftest`, `station/station_demo`, `.http3_intactx_baseline.demo`). They are untracked or ignored; remove them with `make -C http-3.0 clean` and delete the baseline file.
* Run everything from the repository root unless a step says otherwise.

## Step 1 - Ask sleelactl

```sh
bin/sleelactl info http-3.0
```

```text
  ✓ http-3.0 — HTTP/3.0 protocol metadata generation
  location: http-3.0
  see: http-3.0/README.md
    doc: BONDING_COLLAR.md
    doc: FLOW.md
    doc: HANDSHAKE.md
```

`negotiate`, `grade` (exit 3, "no standalone CLI is built") and `simulate` (exit 3, no `sim=`) do nothing for this module. `bin/sleelactl validate http-3.0` runs `make -C http-3.0 test` (there is no `validate` or `syntax` target) and ended `✓ http-3.0 validated (test)`.

## Step 2 - Run the server self-test

```sh
make -C http-3.0 server-test
```

```text
./http3_server --self-test
h3s_self_test: all tests passed
```

## Step 3 - Build and run the crypto self-test

```sh
make -C http-3.0 selftest
http-3.0/crypto_selftest
```

```text
HTTP 3.0 crypto self-test: PASS
```

## Step 4 - Run the pipeline demo

```sh
cd http-3.0
rm -f .http3_intactx_baseline.demo     # start from a clean baseline
make demo
```

The demo walks a request through envelope -> pipeline -> response, printing `[ok]` checks. Abridged real output:

```text
== HTTP 3.0 data flow: request -> envelope -> pipeline -> response ==

-- INTACTX host identity --
  baseline established, intactx=182864859566235 variance=0

-- §4 fast naming --
  service "orders"    -> id 1
  op "calculate"      -> id 1 (retry=READ)
  op "place"          -> id 2 (retry=MUTATING)
  [ok]   compact ids assigned in first-use order
...
-- basket of goods & services (14 items, ISO USD/840 per gram) --
  # 1  Bleached pulp paper              56249759 uUSD/g
  ...
  #14  Structural steel                 35480545 uUSD/g
  basket block: 172 bytes; total 527204238 uUSD/g
...
-- §5 compact envelope (textual wire) --
  H3 3 0 1 1 1001 1234...48 1 14872861615445708499 182864859566235 0348000e...
...
HTTP 3.0 pipeline demo: PASS
```

Further sections of the demo exercise a forged packet (`BAD_DIGEST`), a tampered host (`TAMPERED` + `RESET`), a replayed packet (`REPLAYED`), timing flags, handshake levels (`L1_BASELINE`, `L3_PACING`, `L4_ECHO`) and counters.

**The baseline file matters.** The first run prints `baseline established`; later runs print `baseline loaded`. `http3_intactx.c` derives the fingerprint from the machine and from environment variables (it reads `USER`, `LOGNAME`, `HOME`, `SHELL`, `PWD`, `TERM`, `LANG` and others), so a baseline written under one environment makes a run under a different environment look "tampered". Reproduced here: with a baseline present, `TERM=other-term ./http3_pipeline_demo` printed `baseline loaded, ... variance=28672`, five `[FAIL]` lines and `HTTP 3.0 pipeline demo: FAIL (5)`; `env -i` gave variance 29696 and failures too. The same effect made `make demo` fail earlier in this verification when the baseline had been written by a different invocation (through `sleelactl validate`). After deleting `.http3_intactx_baseline.demo`, `make demo` printed `HTTP 3.0 pipeline demo: PASS` and 6 consecutive direct runs reported 0 failures. If the demo fails, delete the file and rerun.

## Step 5 - Run the full test target

```sh
make -C http-3.0 test
```

This runs the demo, `make station-test` (station signalling demo ending `Station manualog demo: PASS` and `python3 station/test_station.py` ending `Station parity tests: PASS`) and `python3 test_http3_flow.py` (14 tests, ending `HTTP 3.0 flow tests: PASS`).

## What you did

You exercised the whole HTTP 3.0 reference core: envelope encode/decode, name-to-id caching, retry classes, the keyed-MAC / nonce / INTACTX / basket integrity gate, the capability handshake, station signalling and the crypto substrate, using the repository's own demo and tests.

## Things to know

* The HTTP endpoint `POST /__sleela/h3` and `/__sleela/{health,status,negotiate}` were **not** run (no listener was started).
* Parts of the README are marked "Partial" or "Not yet" in `STATUS.md`: streaming, compression, a full negotiation handshake, Java/RMI connectors.
* `STATUS.md` is dated 2026-09-14 and refers to a branch `http3-spec-flow`; treat it as a point-in-time record.
* `make test` and `make demo` write to the working directory (`.http3_intactx_baseline.demo`), so run them where that is acceptable.

## Next steps

* [`../../../http-3.0/README.md`](../../../http-3.0/README.md), [`HANDSHAKE.md`](../../../http-3.0/HANDSHAKE.md), [`FLOW.md`](../../../http-3.0/FLOW.md), [`STATUS.md`](../../../http-3.0/STATUS.md)
* [`http-2.0`](http-2.0.md) - the same core without the integrity gate
* [`http-4.0`](../explanations/http-4.0.md) - frame-based successor

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `http-3.0/{README.md,STATUS.md,Makefile,http3_pipeline_demo.c,http3_intactx.c}`, `http-2.0/HTTP.NEGOTIATION.md`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-3.0`, `make -C http-3.0 test` (via validate), `make -C http-3.0 server-test`, `make -C http-3.0 demo` (failed once with stale baseline, passed after `rm`), `make -C http-3.0 selftest`, `./crypto_selftest`, repeated `./http3_pipeline_demo`.
