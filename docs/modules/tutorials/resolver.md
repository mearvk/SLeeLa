<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# resolver: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help resolver`: legal verbs `negotiate grade simulate validate info`) |
| Registry summary | Shared target-validation resolver (resolve/validate) |
| Lives in | `resolver/` |
| Writeup type | Tutorial, because `resolver/Makefile` builds a real CLI (`sleela-resolver`) and a self-test, and every step below ran successfully. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl validate resolver` exit 0, `make -C resolver test` exit 0, `--resolve` and the C event-policy call run with the outputs shown. |

## What it is

The resolver is a small C library (`resolver.c`, `resolver.h`) with a C++ wrapper (`resolver.cpp`, `resolver.hpp`) and a CLI (`resolver_cli.c`). It turns an input string into one of three results: a forward DNS lookup (hostname to IP), a reverse lookup (IP to hostname), or a canonical filesystem path ([`resolver/RESOLUTION.md`](../../../resolver/RESOLUTION.md)). It also has an event policy, `sleela_resolver_event_policy`, that the HTTP servers call. The policy is permissive unless a target is configured through `SLEELA_RESOLVER_TARGET` ([`resolver/CONFIG.md`](../../../resolver/CONFIG.md)). It is advisory only: it never changes routes or firewall rules.

## Before you start

- Work from the repository root . You need `cc`, `c++` and `make`.
- `bin/sleelactl` needs the usual environment:

```sh
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

- In this checkout `resolver/build/` already held `sleela-resolver`, `resolver.o` and `resolver_cli.o`, so `make` found nothing to rebuild. The directory is git-ignored (`**/build/`). On a clean tree `make -C resolver` creates them.

## Step 1 - Ask sleelactl about the module

```sh
bin/sleelactl info resolver
```

```
  ✓ resolver — Shared target-validation resolver (resolve/validate)
  location: resolver
  see: resolver/README.md
    doc: CONFIG.md
    doc: README.md
    doc: RESOLUTION.md
```

## Step 2 - Syntax-check both language bindings

For a protocol module, `validate` runs the module's `syntax` make target.

```sh
bin/sleelactl validate resolver
```

```
  make -C resolver syntax
cc -std=c11 -O2 -Wall -Wextra -Wpedantic -fsyntax-only resolver.c
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -fsyntax-only resolver.cpp
  ✓ resolver validated (syntax)
```

## Step 3 - Run the built-in self-test

```sh
timeout 120 make -C resolver test
```

```
build/sleela-resolver --self-test
```

Exit code 0. The self-test (`sleela_resolver_self_test` in `resolver.c`) resolves `localhost`, reverse-resolves the address it got, and canonicalizes `.`. It prints nothing; the exit code is the result (0 pass, 1 fail).

## Step 4 - Resolve a few targets

`--resolve` prints `kind`, `family`, `dynamic`, `canonical` and `address`. The kind values come from `resolver.h`: 1 forward, 2 reverse, 4 path. The family values are the Linux numbers 2 (IPv4) and 10 (IPv6).

```sh
resolver/build/sleela-resolver --resolve localhost
resolver/build/sleela-resolver --resolve 127.0.0.1
resolver/build/sleela-resolver --resolve ::1
resolver/build/sleela-resolver --resolve /tmp
```

```
kind=1 family=2 dynamic=1 canonical=localhost address=127.0.0.1
kind=2 family=2 dynamic=1 canonical=localhost address=127.0.0.1
kind=2 family=10 dynamic=1 canonical=localhost address=::1
kind=4 family=0 dynamic=0 canonical=/tmp address=
```

A path result has `dynamic=0`. Anything that is neither resolvable nor an existing path exits with code 2 and prints nothing:

```sh
resolver/build/sleela-resolver --resolve nonexistent.invalid   # exit 2
resolver/build/sleela-resolver --resolve /no/such/path         # exit 2
resolver/build/sleela-resolver --resolve ""                    # exit 2
resolver/build/sleela-resolver                                 # usage on stderr, exit 2
```

`--resolve 8.8.8.8` returned `kind=2 ... canonical=dns.google` in this sandbox, so the sandbox could reach a DNS resolver. That result depends on the network, so do not expect it everywhere.

## Step 5 - Call the event policy from your own C program

The event policy is what the HTTP servers use. Compile a tiny caller against `resolver.c`:

```sh
mkdir -p /tmp/rt && cat > /tmp/rt/t.c <<'EOF'
#include "resolver.h"
#include <stdio.h>
int main(int c, char **v) {
    printf("policy=%d\n", sleela_resolver_event_policy("http", c > 1 ? v[1] : NULL));
    return 0;
}
EOF
cc -Iresolver /tmp/rt/t.c resolver/resolver.c -o /tmp/rt/t
/tmp/rt/t                                   # nothing configured
/tmp/rt/t /tmp                              # explicit path target
/tmp/rt/t /no/such                          # explicit, unresolvable target
SLEELA_RESOLVER_TARGET=/tmp SLEELA_RESOLVER_REQUIRE_DYNAMIC=true /tmp/rt/t
SLEELA_RESOLVER_TARGET=localhost SLEELA_RESOLVER_REQUIRE_DYNAMIC=true /tmp/rt/t
SLEELA_RESOLVER_TARGET=/no/such /tmp/rt/t
```

```
policy=1     (nothing configured: permissive)
policy=1     (/tmp resolves as a path)
policy=0     (unresolvable explicit target: fail closed)
policy=0     (REQUIRE_DYNAMIC=true, but /tmp is a static path)
policy=1     (REQUIRE_DYNAMIC=true, localhost resolved live)
policy=0     (env target unresolvable)
```

This matches rules 4 and 5 of `RESOLUTION.md`.

## What you did

You checked the module with `sleelactl`, built and ran the self-test, resolved a hostname, two IPs and a path, and exercised the event policy with and without `SLEELA_RESOLVER_TARGET` and `SLEELA_RESOLVER_REQUIRE_DYNAMIC`.

## What the other sleelactl verbs do for this module

| Command | Result |
|---|---|
| `bin/sleelactl --dry-run validate resolver` | `[dry-run] would run: make -C resolver syntax`, exit 0 |
| `bin/sleelactl negotiate resolver` | Says it is "a protocol-metadata operation defined by the generated spec", points to "the negotiate section in resolver/README.md", then `no standalone negotiate CLI is built`. Exit 3. `resolver/README.md` has no such section (`grep -i negotiate resolver/README.md` finds nothing). |
| `bin/sleelactl grade resolver` | Same message for `grade`. Exit 3. |
| `bin/sleelactl simulate resolver` | `no simulator registered (sim= unset)`. Exit 3. |

## Things to know

- `SLEELA_RESOLVER_CONFIG` is listed in `CONFIG.md` as reserved. `resolver.c` never reads it (the only `getenv` calls are for `SLEELA_RESOLVER_TARGET` and `SLEELA_RESOLVER_REQUIRE_DYNAMIC`).
- `SLEELA_RESOLVER_REQUIRE_DYNAMIC` only counts the exact string `true`.
- A reverse lookup that finds no PTR record still succeeds (`dynamic=0`, the input echoed back as `canonical`), so `REQUIRE_DYNAMIC=true` rejects such an IP.
- `resolver.cpp` defines `sleela::resolver::event` as a plain wrapper; `make syntax` compiles it but nothing links it. The HTTP Makefiles compile `resolver.c`.

## Next steps

- [`resolver/README.md`](../../../resolver/README.md), [`CONFIG.md`](../../../resolver/CONFIG.md), [`RESOLUTION.md`](../../../resolver/RESOLUTION.md)
- [preferred-routers](preferred-routers.md): the README says the resolver result is combined with the preferred-router subsystem.
- [route](../explanations/route.md): the route-data contract says dynamic targets use the resolver.

## Sources

- Files read: `resolver/README.md`, `CONFIG.md`, `RESOLUTION.md`, `Makefile`, `resolver.h`, `resolver.c`, `resolver_cli.c`, `resolver.cpp`, `resolver.hpp`; `bin/sleelactl.d/registry.conf` (line 63).
- Commands run: `bin/sleelactl [--dry-run] info|validate|negotiate|grade|simulate resolver`, `bin/sleelactl help resolver`, `make -C resolver test`, `resolver/build/sleela-resolver --self-test` and `--resolve ...` as shown, and the scratch policy program above (built in `/tmp`, removed with the session's `/tmp`).
