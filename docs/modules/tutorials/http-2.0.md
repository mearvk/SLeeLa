<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-2.0: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-2.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/2.0 protocol metadata generation |
| Lives in | `http-2.0/` (`h21_envelope.c`, `h21_naming.c`, `h21_pipeline.c`, `h21_server.c`, `h21_main.c`, `h21_flow.py`, `test_h21_flow.py`, negotiation files) |
| Writeup type | Tutorial, because you can build the C server, run its self-test and drive the envelope/pipeline from Python, and every step below was executed successfully. |
| Verified | 2026-11-13 against `master` e048707fd: `make`, `make test`, `make test-python`, `sleelactl validate http-2.0` all pass; a Python session round-trips an envelope. The server was not started as a listener. |

## What it is

"SLeeLa HTTP 2.0 / 2.1" is the project's own **application protocol**, not the IETF HTTP/2 (RFC 9113). The module's own files say so: the native server "does **not** implement IETF HTTP/2 framing, HPACK, stream state, or TLS" and talks HTTP/1.0 or HTTP/1.1 over `sleela_net`; HTTP/2 multiplexing in earlier notes "is aspirational, not an implemented transport feature" (`http-2.0/HTTP21.SECURITY.md`, `README.md` "Port and stream multiplexing (design model, not current transport)"). What the module does implement is a **compact application envelope** (tag `H21`), connection-local service/operation naming, a response model, retry classes (`READ`, `IDEMPOTENT`, `MUTATING`) and a dispatch pipeline. Compared with `http-3.0` it deliberately has no MAC, nonce, INTACTX or basket.

This tutorial builds the module, runs its tests, and then uses the Python parity implementation to send an envelope through a pipeline.

## Before you start

* Repository root `SLeeLa/`, a C11 compiler (`cc`) and `python3` (3.11 was used).
* Nothing else is needed; the server links `impl/core/sleela_net.c`, `preferred-routers/` and `resolver/` straight from the tree.
* Build products appear in `http-2.0/` (`h21_server`, `*.o`). They are untracked; remove them with `make -C http-2.0 clean` when done.

## Step 1 - Ask sleelactl about the module

```sh
bin/sleelactl info http-2.0
```

```text
  ✓ http-2.0 — HTTP/2.0 protocol metadata generation
  location: http-2.0
  see: http-2.0/README.md
    doc: FLOW.md
    doc: HTTP21.SECURITY.md
    doc: HTTP.NEGOTIATION.md
```

`info` only prints the registry summary and the first three `*.md` files. Note that `negotiate` and `grade` print "no standalone ... CLI is built" and exit 3, and `simulate` reports "no simulator registered (sim= unset)" and exits 3: these three verbs do nothing for this module.

## Step 2 - Build

```sh
make -C http-2.0
```

The build compiles the server (`h21_server`) in one `cc` command, then `h21_envelope.o`, `h21_naming.o`, `h21_pipeline.o` and the two negotiation objects. It exits 0 with a few warnings (for example `h21_envelope.c:78: implicit declaration of function 'strtoull'`, i.e. `<stdlib.h>` is not included).

## Step 3 - Run the C self-test

```sh
make -C http-2.0 test
```

```text
./h21_server --self-test
h21_self_test: all tests passed
```

## Step 4 - Use the envelope and pipeline from Python

`h21_flow.py` is the Python mirror of the C wire format. Run from `http-2.0/`:

```python
from h21_flow import Pipeline, Envelope, Response, RetryClass, Status
p = Pipeline()
sid, oid = p.register("echo", "echo", RetryClass.READ, lambda payload, ctx: (Status.OK, payload))
print("ids:", sid, oid)
wire = Envelope(sid, oid, 42, b"ping").pack_text()
print("request :", wire)
reply = p.handle_wire(wire)
print("response:", reply)
print("decoded :", Response.unpack_text(reply))
print("bad     :", p.handle_wire(b"H21 9 0 1 1 7 0:\n"))
print("unknown :", p.handle_wire(Envelope(9, 9, 5, b"x").pack_text()))
```

Real output:

```text
ids: 1 1
request : b'H21 2 0 1 1 42 4:ping\n'
response: b'H21R 0 42 4:ping\n'
decoded : Response(status=<Status.OK: 0>, request_id=42, result=b'ping')
bad     : b'H21R 4 0 0:\n'
unknown : b'H21R 2 5 0:\n'
```

Reading it: the textual envelope is `H21 <ver> <flags> <service_id> <op_id> <request_id> <len>:<payload>\n` (version is `2`). The reply echoes the request id (`42`) and carries status `0` (`OK`). A version-9 envelope is rejected with status `4` (`BAD_ENVELOPE`, request id 0), and an unregistered service id gives status `2` (`UNKNOWN_SERVICE`). Status codes are from `h21_flow.py`: `OK=0, APP_ERROR=1, UNKNOWN_SERVICE=2, UNKNOWN_OPERATION=3, BAD_ENVELOPE=4, TOO_LARGE=5, RETRY_DENIED=6`.

## Step 5 - Run the Python regression tests and the sleelactl validation

```sh
make -C http-2.0 test-python
bin/sleelactl validate http-2.0
```

`test-python` runs `python3 -m unittest -v test_h21_flow.py` and ends `Ran 7 tests ... OK`. `sleelactl validate` picks the first of the targets `validate`, `syntax`, `test` that exists in the Makefile (here `syntax`), runs the syntax-only compile of every source and ends `✓ http-2.0 validated (syntax)` (exit 0).

## What you did

You built a reference implementation of an experimental application envelope that rides on ordinary HTTP/1.x, ran its self-tests, and pushed one request and two error cases through the dispatch pipeline using the Python parity code.

## Things to know

* `STATUS.md` and `FLOW.md` call `h21_naming.{h,c}` and `h21_pipeline.{h,c}` a "Sketch/STUB ... bodies TODO". The code contradicts that: both files have real bodies (134 and 220 lines, no TODO markers) and the Python `Naming`/`Pipeline` classes you just used work. `README.md` ("Core capabilities") is the accurate document.
* The HTTP endpoint `POST /__sleela/h21` (documented in the README with a `curl` example) was **not** run, because that needs a listening server process.
* There is no integrity, replay or authentication layer in 2.0 (`HTTP21.SECURITY.md`); do not expose the server without a protected transport.

## Next steps

* [`../../../http-2.0/README.md`](../../../http-2.0/README.md) - wire forms, endpoints, file inventory
* [`../../../http-2.0/HTTP21.SECURITY.md`](../../../http-2.0/HTTP21.SECURITY.md) - deployment contract
* [`http-3.0`](http-3.0.md) - the same core plus the integrity gate

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/sleelactl` (`protocol_cmd`), `http-2.0/{README.md,STATUS.md,FLOW.md,HTTP21.SECURITY.md,Makefile,h21_flow.py,h21_naming.c,h21_pipeline.c}`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-2.0`, `make -C http-2.0`, `make -C http-2.0 test`, `make -C http-2.0 test-python`, the Python session above (with `PYTHONDONTWRITEBYTECODE=1`), `./h21_server --help`.
