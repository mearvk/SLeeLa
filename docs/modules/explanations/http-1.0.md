<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# http-1.0: Explanation

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help http-1.0`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | HTTP/1.0 protocol metadata generation |
| Lives in | `http-1.0/` (server `http1_server.c`/`.h`, `http1_main.c`; negotiation `http_negotiation.{c,h,cpp,hpp}`; docs `README.md`, `STATUS.md`, `FLOW.md`, `HTTP.NEGOTIATION.md`) |
| Writeup type | Explanation, because the module does not build at this commit (`make` and `make syntax` fail with a duplicate function definition), so no end-to-end path can be executed. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl info/negotiate/grade/simulate/validate http-1.0`, `make -C http-1.0`, `make -C http-1.0 test`; build fails; a scratch copy with the duplicate removed builds and passes its self-test. |

## What it is

SLeeLa HTTP 1.0 is the first generation of the project's "HTTP family". Its README describes it as "a portable, real-wire HTTP/1.0 server built on the `sleela_net` socket abstraction" (`http-1.0/README.md`). It speaks plain HTTP/1.0 request/response on the wire and adds SLeeLa application headers (`X-SLeeLa-Version`, `X-SLeeLa-Accept`, `X-SLeeLa-Port`, `X-SLeeLa-Service-ID`, `X-SLeeLa-Op-ID`) that carry a SLeeLa *logical port*, service id and operation id. The README states that a logical port "is not a replacement for, or necessarily a one-to-one mapping with, a native TCP or UDP port".

**Standard or own design?** The wire protocol it serves is HTTP/1.0 (the IETF standard, RFC 1945). The module's own files do not cite an RFC number; they call it an "HTTP/1.0 server" and "compatibility target" (`HTTP.NEGOTIATION.md`: "HTTP/1.0: compatibility target"). The SLeeLa layer on top (the `X-SLeeLa-*` headers, logical ports, the DOWNLOAD resume record) is this project's own design, and `HTTP.NEGOTIATION.md` calls such headers "application headers, not new HTTP standards". The repository's separate `http-servers/` tree (not this module) states that its HTTP/1.x parser follows RFC 1945 for HTTP/1.0 (`http-servers/README.md`, line 18).

## How it works

```text
TCP socket (sleela_net)
  -> HTTP/1.0 request, one request per connection, serial server
  -> X-SLeeLa-* headers -> logical PORT -> SERVICE-ID -> OP-ID
  -> handler -> HTTP/1.0 response (always adds X-SLeeLa-Version: 1.0, X-SLeeLa-Accept: 1.0, 1.1)
```

* Built-in endpoints (README): `GET /__sleela/health`, `/__sleela/status`, `/__sleela/negotiate`.
* **Negotiation** (`http_negotiation.c`, 48 lines): `sleela_http_negotiate(requested, peer_accept, allow_fallback)` returns `requested` if it appears in the peer's comma/space separated accept list; otherwise, only if `allow_fallback` is set, returns `"1.1"` then `"1.0"` if listed; otherwise NULL. It first consults two policy hooks, `sleela_resolver_event_policy("HTTP/1.0", 0)` and `sleela_preferred_router_packet_policy("HTTP/1.0+", 4)`, and returns NULL if either refuses. The same function, with a different version string, is repeated in every `http-N.0` module (the C files differ per module).
* **DOWNLOAD mode**: for files over 50 MB the server keeps resume metadata `SESSION-ID | DATETIME | FILE-ID | FILE-NAME | INDEX | OFFSET | TOTAL-SIZE` (`http1_download_*` in `http1_server.c`). The README says the 50 MB threshold selects resume mode and is "not a maximum file size".
* **Parser limits** (README "Security and validation"): bounded header/body sizes, rejection of control characters, duplicate headers, duplicate/malformed Content-Length and Transfer-Encoding, and a 10 second total read deadline. The server is serial (one connection at a time), has no TLS, and its `bind_address` config is "reserved and not yet enforced"; the listener binds all interfaces.

## What `sleelactl` does for this module

| Command | Real behaviour (run here) | Exit |
|---|---|---|
| `bin/sleelactl info http-1.0` | prints the registry summary, location `http-1.0`, "see: http-1.0/README.md" and lists the first three `*.md` files (`FLOW.md`, `HTTP.NEGOTIATION.md`, `README.md`) | 0 |
| `bin/sleelactl validate http-1.0` | picks the first Makefile target among `validate`, `syntax`, `test`: runs `make -C http-1.0 syntax`, which **fails** (see below) | 1 |
| `bin/sleelactl negotiate http-1.0` | prints "a protocol-metadata operation defined by the generated spec", points to a "negotiate section" in the README and warns "no standalone negotiate CLI is built" | 3 |
| `bin/sleelactl grade http-1.0` | same message for `grade` | 3 |
| `bin/sleelactl simulate http-1.0` | "no simulator registered (sim= unset)" (the registry line for `http-1.0` has no `sim=`) | 3 |

`negotiate` and `grade` are stubs in `bin/sleelactl` (the `negotiate|grade)` branch of `protocol_cmd`): they never run code. `http-1.0/README.md` has no section about negotiation or grading, so the pointer leads nowhere.

## Where it lives

| File | Role |
|---|---|
| `http-1.0/http1_server.c`, `http1_server.h` | Parser, router, responses, download state, `--self-test` |
| `http-1.0/http1_main.c` | CLI (`--port`, `--self-test`, `--help`), demo handler |
| `http-1.0/http_negotiation.{c,h,cpp,hpp}` | Version negotiation (C and C++ wrapper) |
| `http-1.0/Makefile` | Targets `all server syntax test sanitize fuzz-download clean` |
| `http-1.0/build/Makefile`, `negotiation.mk` | Local build fragment (syntax checks of `../*.c`, `../*.cpp`) |
| `tests/http-1.0/fuzz_download_metadata.c` | libFuzzer harness used by `make fuzz-download` (needs clang; not run) |
| `../preferred-routers/`, `../resolver/`, `../impl/core/sleela_net.c` | Dependencies linked into the server |

## Status and limits

**Broken at this commit: the module does not compile.**

```sh
$ make -C http-1.0
cc -std=c11 -O2 -Wall -Wextra -Wpedantic -I. -I.. http1_server.c http1_main.c http_negotiation.c ../preferred-routers/preferred_router.c ../resolver/resolver.c ../impl/core/sleela_net.c -o http1_server
http1_server.c:406:5: error: redefinition of 'http1_download_should_resume'
make: *** [Makefile:36: http1_server] Error 1
```

`http1_download_should_resume(uint64_t)` is defined twice with the same body, at `http1_server.c` lines 390 and 406. `make test` depends on the server binary and fails the same way, so the README's "run the built-in self-test" path cannot be executed. `make syntax` (which `sleelactl validate` runs) fails at the same line. It also warns at `http1_server.c:447` (`'\\t'` is a multi-character constant: the tab test in the download parser is written with a doubled backslash, so it compares against a two-character constant and never matches a tab).

**Scratch check (not committed):** in a copy of the module under a temporary directory, deleting the second definition (lines 405-409) made `make` succeed and `./http1_server --self-test` print `http1_self_test: all tests passed`. So the defect is a single duplicated function, not a design problem. Nothing in the repository was changed.

Not run: a live server (no daemon was started for this writeup), `make sanitize`, `make fuzz-download`. The README's claims about the 10 second deadline and parser rejections were not exercised.

A related but separate implementation exists: `impl/build/http_server.o` is built from `http-servers/common/http_server.cpp` and is run with `sleela http-server 1` (see `impl/frontend/http_server_cli.cpp`). It does not use `http-1.0/` source files.

## Related

* [`http-2.0`](../tutorials/http-2.0.md) - next generation, adds the compact envelope.
* `http-1.0/README.md`, `STATUS.md`, `FLOW.md`, `HTTP.NEGOTIATION.md`
* `http-servers/README.md` - the separate native server grades 1/2/3
* [`../../../bin/SLEELACTL.md`](../../../bin/SLEELACTL.md)

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 47), `bin/sleelactl` (`protocol_cmd`), `bin/SLEELACTL.md`, `http-1.0/{README.md,STATUS.md,HTTP.NEGOTIATION.md,Makefile,build/README.md,http1_server.c,http_negotiation.c}`, `http-servers/README.md`, `impl/frontend/http_server_cli.cpp`.
Commands run: `bin/sleelactl info|negotiate|grade|simulate|validate http-1.0`; `make -C http-1.0`; `make -C http-1.0 test`; scratch-copy build and `--self-test` after removing the duplicate function. No build products remain (the failed build produced none).
