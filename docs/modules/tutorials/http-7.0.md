<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-7.0: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-7.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/7.0 protocol metadata generation |
| Lives in | `http-7.0/` (`http7_protocol.{c,h}`, `http7_server.{c,h}`, `http7_main.c`, `http70.c`, `http70.cpp`, `http7_logger_bridge.*`, `http7_logger_test.cpp`, negotiation files, `SPEC.md`, `SECURITY.md`, `STATUS.md`) |
| Writeup type | Tutorial, because the two reference demos, the server self-test and the logger test all build and run here, and every step below was executed successfully. |
| Verified | 2026-11-13 against `master` e048707fd: `make demo`, both demos, `make test` (server self-test + logger test) and `sleelactl validate http-7.0` pass. The server was not started as a listener. |

## What it is

SLeeLa HTTP 7.0 is **this project's own experimental protocol generation, not an IETF HTTP/7 standard** (`README.md`, `SPEC.md`, `STATUS.md`). It keeps the 32-byte big-endian frame header and the 20 frame types of `http-6.0` and adds four **Reality Assertion** frames: `ASSERTION` (21), `ASSERTION_SOURCE` (22), `ASSERTION_STATUS` (23), `ASSERTION_AUTHOR` (24), plus a `FACTUAL` flag (0x80). An assertion is a classified statement with a status (`FACTUAL_DOCUMENTED`, `USER_AUTHORED`, `FICTIONAL`, `COUNTERFACTUAL`, `DISPUTED`, `UNVERIFIED`), source, source date, author and optional document reference, serialised as six tab-separated fields. The README's rule: carrying a claim does not make it true, so the protocol separates "transmitted" from "established by evidence". The server carries frames in HTTP/1.x over TCP (loopback by default, `--bind HOST` to change, fallback negotiation off unless `--allow-fallback`), with no TLS or authentication (`SECURITY.md`).

The module also holds two small original demos (`http70.c`, `http70.cpp`) that print two fixed "protocol assertions".

## Before you start

* Repository root `SLeeLa/`; `cc`, `c++` (C++17, pthreads).
* Products are written into `http-7.0/` (`http70_demo`, `http70_demo_cpp`, `http7_server`, `http7_logger_test`, `*.o`, and an empty `logs/` directory made by the logger test). Remove them when done with `make -C http-7.0 clean` plus `rmdir http-7.0/logs`.

## Step 1 - Ask sleelactl

```sh
bin/sleelactl info http-7.0
```

```text
  ✓ http-7.0 — HTTP/7.0 protocol metadata generation
  location: http-7.0
  see: http-7.0/README.md
    doc: HTTP.NEGOTIATION.md
    doc: README.md
    doc: SECURITY.md
```

`negotiate` and `grade` print "no standalone ... CLI is built" (exit 3); `simulate` says no simulator is registered (exit 3).

## Step 2 - Build the demos

```sh
make -C http-7.0 demo
```

```text
cc -std=c11 -O2 -Wall -Wextra -Wpedantic -I. http70.c -o http70_demo
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread -I. http70.cpp -o http70_demo_cpp
```

## Step 3 - Run the C demo

```sh
http-7.0/http70_demo
```

```text
HTTP/7.0 200 OK
Content-Type: text/http70
X-HTTP70-Semantics: enabled

HTTP/7.0
Method: GET
Target: /semantic/assertions

Assertion-1: PROTOCOL-NAMESPACE
Status: 1001
Subject: Megan Rapinoe
Statement: The HTTP/7.0 protocol namespace does not define this subject as an HTTP protocol entity.

Assertion-2: REAL-WORLD-CATEGORY
Status: 1002
Subject: nuclear arms
Statement: HTTP/7.0 recognizes nuclear arms as a real-world subject category for factual representation.
```

The two assertions and numeric statuses 1001/1002 are hard-coded in `http70.c`; its header comment says they are "protocol assertions, not independent claims about the existence or non-existence of people or weapons". Note that this is a printed text, not a request/response over a socket.

## Step 4 - Run the C++ demo

```sh
http-7.0/http70_demo_cpp
```

Its output is byte-for-byte identical to the C demo (checked with `diff`, 18 lines).

## Step 5 - Run the self-tests

```sh
make -C http-7.0 test
```

This builds the protocol objects, the server (`http7_server`, linked with `g++` together with the router, resolver, `sleela_net` and logger bridge) and `http7_logger_test`, then runs:

```text
./http7_server --self-test
h7s_self_test: all tests passed
./http7_logger_test
```

`http7_logger_test` prints nothing and exits 0 (it uses `assert`). It opens the shared logger, records three metadata-only events, checks that `logs/http7/packets-000000.jsonl` contains the `SLeeLa-PacketLog-1` marker and the event names, and that no `payload_base64` was written; it then deletes `logs/http7`.

## Step 6 - Validate with sleelactl

```sh
bin/sleelactl validate http-7.0
```

This runs `make -C http-7.0 syntax` (syntax-only compile of the C sources, router/resolver/net sources, C++ negotiation and logger files, and both demos) and ends `✓ http-7.0 validated (syntax)`, exit 0, with misleading-indentation warnings from `http7_server.c` and `preferred-routers/preferred_router.c`.

## What you did

You built and ran the two original HTTP 7.0 demos, then ran the real implementation's tests: the frame/assertion/HTTP-parsing self-test of the server and the metadata-only logging test.

## Things to know

* The assertion frames, `POST /__sleela/h7`, and the assertion parser are covered only by the server's built-in `--self-test`; no assertion was sent over a socket here.
* `STATUS.md` lists open items: no TLS, authentication, authorization, absolute request deadline, rate limiting, or complete stream/flow-control/replay/resume implementation; the server is serial.
* CI: `.github/workflows/http7-hardening.yml` exists for this module (not run here).

## Next steps

* [`../../../http-7.0/README.md`](../../../http-7.0/README.md), [`SPEC.md`](../../../http-7.0/SPEC.md), [`SECURITY.md`](../../../http-7.0/SECURITY.md), [`STATUS.md`](../../../http-7.0/STATUS.md)
* [`http-6.0`](../explanations/http-6.0.md) - the 20-type predecessor
* [`http-8.0`](http-8.0.md) - adds session/crypto/NSF frames

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `http-7.0/{README.md,STATUS.md,SPEC.md,SECURITY.md,Makefile,http70.c (head),http7_logger_test.cpp}`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-7.0`, `make -C http-7.0 test`, `make -C http-7.0 demo`, `./http70_demo`, `./http70_demo_cpp` (+ `diff`), `./http7_server --self-test`, `./http7_logger_test`.
