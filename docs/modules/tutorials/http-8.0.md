<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-8.0: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-8.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/8.0 protocol metadata generation |
| Lives in | `http-8.0/` (`http8_protocol.{c,h}`, `http8_server.{c,h}`, `http8_main.c`, `http80.c`, `http80.cpp`, `DarkPower.{cpp,hpp}`, `http80_crypto.hpp`, `CRYPTO.SUPPORT.md`, `HTTP80.EARLY.SECURITY.conf`, negotiation files) |
| Writeup type | Tutorial, because the demos build and run and the server self-test passes, and every step below was executed successfully; the one caveat is that `make test` rewrites a tracked binary (see Step 4). |
| Verified | 2026-11-13 against `master` e048707fd: `make demo`, both demos, `./http8_server --self-test`, `sleelactl validate http-8.0` all pass. The server was not started as a listener. |

## What it is

SLeeLa HTTP 8.0 is **this project's own experimental generation, not an IETF HTTP/8 standard** (`README.md`, `STATUS.md`). It builds on the 24 frame types of `http-7.0` and adds four (`SESSION_BEGIN` 25, `SESSION_ACCEPT` 26, `NATIONAL_SIGNAL` 27, `CRYPTO_POLICY` 28) and a `CRYPTO_REQUIRED` flag (0x100), for 28 types total. The README describes a session model (subscription state, radio state, international context, "binary-checker" agreement, a two-way handshake begin -> offer -> accept -> complete), National Signal Frequency (NSF) *descriptive* metadata, and "DarkPower", a typed session descriptor with a 24-character schedule term and the integer value `0x18ae`; it says NSF records and DarkPower are data formats, "not an access grant". `CRYPTO.SUPPORT.md` is a catalogue of cryptographic families (SHA-2/3, AES-GCM, ChaCha20-Poly1305, X25519, ML-KEM, ML-DSA and others) and says it is "a registry and adapter contract, not an independent implementation of every cryptographic primitive"; "supported means catalogued". `HTTP80.EARLY.SECURITY.conf` sets policy flags (RSA-2048 minimum, fail closed, replay protection required).

## Before you start

* Repository root `SLeeLa/`; `cc` and `c++` (C++17).
* Products are written into `http-8.0/` (`http80_demo`, `http80_demo_cpp`, objects). Remove with `make -C http-8.0 clean` **only after reading Step 4**: `clean` deletes `http8_server`, which is a tracked file.

## Step 1 - Ask sleelactl

```sh
bin/sleelactl info http-8.0
```

```text
  ✓ http-8.0 — HTTP/8.0 protocol metadata generation
  location: http-8.0
  see: http-8.0/README.md
    doc: CRYPTO.SUPPORT.md
    doc: HTTP.NEGOTIATION.md
    doc: README.md
```

`negotiate` and `grade` print "no standalone ... CLI is built" (exit 3); `simulate` reports no registered simulator (exit 3).

## Step 2 - Build the demos

```sh
make -C http-8.0 demo
```

```text
cc  -std=c11 -O2 -Wall -Wextra -Wpedantic -I. http80.c -o http80_demo
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -I. http80.cpp DarkPower.cpp -o http80_demo_cpp
```

## Step 3 - Run the demos

```sh
http-8.0/http80_demo
http-8.0/http80_demo_cpp
```

Both print the same 12 lines (checked by running both):

```text
HTTP/8.0 200 OK
Protocol: HTTP/8.0
Subscription: 2
Radio: 1
National-Emblem: national-emblem-reference
Signal: authorized-signal-reference
Frequency-Unit: MHz
Frequency-Range: bounded-descriptive-range
National-Signal-Frequency-Metadata: valid
Binary-Checker-Agreement: passed
Two-Way-Handshaking: complete
Message-Exchange: accepted
```

This is the result of a hard-coded two-way handshake in `http80.c`; every NSF value is a placeholder string, and the output is plain text printed to stdout, not a network exchange. `http80.cpp` does not include `DarkPower.hpp` (its only includes are `<iostream>` and `<string>`), so `DarkPower.cpp` is linked into `http80_demo_cpp` but DarkPower is not exercised by this demo.

## Step 4 - Run the server self-test

```sh
cd http-8.0 && ./http8_server --self-test
```

```text
h8s_self_test: all tests passed
```

This used the **committed** `http-8.0/http8_server` ELF binary. The documented route is `make -C http-8.0 test`, which also passes (`h8s_self_test: all tests passed`), **but** `http8_server` is a tracked file (`git ls-files http-8.0/http8_server`), and `make` rebuilds it in place whenever the sources look newer: in this verification it turned `git status` into ` M http-8.0/http8_server` (committed binary 45296 bytes, rebuilt one 38280 bytes). To avoid dirtying the tree, build into a scratch copy of the directory, or restore the file with `git checkout -- http-8.0/http8_server` afterwards.

## Step 5 - Validate with sleelactl

```sh
bin/sleelactl validate http-8.0
```

This runs `make -C http-8.0 syntax` (syntax-only compile of `http8_protocol.c`, `http8_server.c`, `http8_main.c`, `http_negotiation.c/.cpp`, `http80.c`, `DarkPower.cpp`) and ends `✓ http-8.0 validated (syntax)`, exit 0, with many `-Wmisleading-indentation` warnings.

## What you did

You ran the HTTP 8.0 reference demos (a printed two-way handshake with placeholder NSF values) and the server's built-in self-test (the README says `make test` covers frames, session handshake and negotiation), and used `sleelactl` to syntax-validate the module.

## Things to know

* `POST /__sleela/h8` and the other endpoints were not exercised over a socket (no listener was started).
* Untested or unreferenced here: `http80_crypto.hpp` is not included by any source or Makefile target in the module; `CRYPTO.SUPPORT.md` describes a catalogue, not code run in this verification. `ICC.{c,cpp,h,hpp}`, `USTreasury.{c,cpp,h,hpp}` and `AmericanCourtsCustodies*.{cpp,hpp}` sit in the directory but are not mentioned in `README.md`, `STATUS.md` or the `Makefile`; `USTreasury.hpp` itself says "Project model only; not an official U.S. Treasury or Supreme Court security API".
* `STATUS.md` states the required hardening is still open: provider availability, algorithm policy, key handling, authentication, replay behaviour, secret storage and explicit fallback policy.
* There is no `http8-*` workflow in `.github/workflows/`.

## Next steps

* [`../../../http-8.0/README.md`](../../../http-8.0/README.md), [`CRYPTO.SUPPORT.md`](../../../http-8.0/CRYPTO.SUPPORT.md), [`STATUS.md`](../../../http-8.0/STATUS.md)
* [`http-7.0`](http-7.0.md) - the 24-type predecessor
* [`http-9.0`](../explanations/http-9.0.md) - metadata successor

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `http-8.0/{README.md,STATUS.md,CRYPTO.SUPPORT.md,HTTP80.EARLY.SECURITY.conf,Makefile,http80.cpp (includes),USTreasury.hpp (head)}`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-8.0`, `make -C http-8.0 test`, `make -C http-8.0 demo`, `./http80_demo`, `./http80_demo_cpp`, `./http8_server --self-test`. The tracked `http8_server` was restored to its committed content (`git show HEAD:http-8.0/http8_server`) after `make test` rewrote it.
