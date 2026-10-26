# lib/web-server-technology — SLeeLa facade for the dynamic SQL web server

This package is the SLeeLa-side model of the dynamic, SQL-backed HTTP/1.1
server whose native implementation lives in
[`web-server-technology/native`](../../web-server-technology). The facade
classes describe the server's configuration, the dynamic `/db` endpoint policy,
and the world-reachable dual-stack binding; they do not open sockets
themselves — the native server does that.

## Classes

| Class | Role |
|---|---|
| `SLPackage` | package manifest (executive-tier metadata record) |
| `SLWebServer` | server configuration + lifecycle (`configure`/`open`/`shutdownServer`/`active`), document root, port, threads, bind address, and the `/db` enable + writable flags |
| `SLSqlEndpoint` | the dynamic `/db` route policy: route prefix, read-only vs writable, statement/row bounds, and read-verb classification that mirrors the native `statement_is_readonly` logic |
| `SLDualStackBinding` | the binding descriptor; `reachesWorld()` is true only when bound to all interfaces as dual-stack IPv6+IPv4 |
| `SLWebProgram` | Side 2: a mounted program whose backing kind is `static`, `sql`, or `meme` (named `SLWebProgram` to avoid colliding with `lib/cpu/SLProgram`) |
| `SLWarrant` | Side 2: a cause served for a subject by a program; `matchesHost` models the host/enabled gate |
| `SLEnvelopment` | Side 2: a domain that envelops many programs; `active()` enforces the Scientist gate (`turnOn()` + `grantScientist()` + at least one warrant) |

## Validate

Library classes have no `main()`, so validate through the sibling demo (which
`sleela run` auto-links against every `.sleela` in this directory):

```sh
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"   # from repo root
impl/build/sleela check lib/web-server-technology/demo-web-server-technology.sleela
impl/build/sleela run   lib/web-server-technology/demo-web-server-technology.sleela
```

The demo asserts the policy correspondence with the native server: `SELECT` is
permitted read-only, `INSERT` is denied until writes are allowed, and a
single-literal bind drops world reach.

## Native C and C++ library (adjacent set)

Alongside the SLeeLa facade classes, this package ships a reusable native
library of the same model — the dependency-free decision/model layer the
standalone server builds on — in both C and C++, mirroring the `lib/sleela-sql`
layout:

```text
lib/web-server-technology/
  native/include/slweb.h      C11 public ABI
  native/src/slweb.c          C11 core implementation
  cpp/include/slweb.hpp       C++17 RAII facade over the same ABI
  cpp/src/slweb.cpp           facade translation unit
  tests/test_c.c              C smoke tests
  tests/test_cpp.cpp          C++ smoke tests
  Makefile                    builds libslweb.a, libslweb++.a; `make test`
```

It covers the whole subsystem model with no sockets and no HTTP wire handling:

- **Side 2 envelopment** — `slweb_envelopment` / `sleela::web::Envelopment`:
  register programs (meme/static/sql) and warrants, the Scientist-gated
  `active()` rule, and longest-path-prefix `resolve()`.
- **Hardening** — `slweb_rate` / `RateLimiter` (sliding-window per-peer +
  global), constant-time compare and bearer-token `WriteAuth`, and
  trusted-proxy `client_ip()` anti-spoofing.
- **CPU keep-warm band math** — `slweb_warm` / `Warm`: the 11–14% default band,
  clamping, midpoint target, and core-count (1–16). The live threaded
  controller stays in the server (`web-server-technology/native/cpu_warmer.*`).

### Build and test

```sh
cd lib/web-server-technology
make        # -> build/libslweb.a and build/libslweb++.a
make test   # C and C++ smoke tests
```

### Portability

The library is dependency-free (no sockets, no HTTP): it builds on Linux,
macOS (Intel + Apple Silicon), and Windows 7–11 with a C++17 toolchain (MSVC
2017+ or MinGW-w64). The one platform-specific call is the CPU count
(`GetSystemInfo` on Windows, `sysconf` on POSIX/macOS). Windows XP can build the
**C core** but not the C++17 facade. See [`PORTABILITY.md`](PORTABILITY.md) for
the full matrix and the honest verification status.

Consumers of the C++ facade link both the C++ and C core libraries (as with
`lib/sleela-sql`). Example:

```c
slweb_envelopment *e = slweb_env_new("science.example");
slweb_env_add_program(e, "lab", SLWEB_PROGRAM_SQL, "/lab", "./db", 0);
slweb_env_add_warrant(e, "expose", "science.example", "/lab", "lab", 1);
slweb_env_set_enabled(e, 1); slweb_env_set_scientist(e, 1);
char prog[SLWEB_MAX_NAME], cause[SLWEB_MAX_PATH]; slweb_program_kind k;
slweb_env_resolve(e, "science.example", "/lab/db/query", prog, cause, &k);
```

```cpp
sleela::web::Envelopment env("science.example");
env.add_program("lab", sleela::web::ProgramKind::Sql, "/lab", "./db");
env.add_warrant("expose", "science.example", "/lab", "lab");
env.set_enabled(true); env.set_scientist(true);
if (auto r = env.resolve("science.example", "/lab/db/query"))
    /* r->program == "lab", r->kind == ProgramKind::Sql */;
```

## Metadata

Every class carries the Descriptive-metadata block required by
`.kiro/steering/metadata-class-standard.md`. The package and facade classes are
executive-tier and cite real governing instruments in `metaNationalSchedule`
(RFC 9112 for HTTP/1.1 framing, RFC 8259 for JSON, RFC 4291 for IPv6
addressing).
