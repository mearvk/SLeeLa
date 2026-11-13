<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# preferred-routers: Tutorial

| | |
|---|---|
| Nature | protocol (`bin/sleelactl help preferred-routers`) |
| Registry summary | Preferred-router selection model |
| Lives in | `preferred-routers/` |
| Writeup type | Tutorial, because the C selector can be compiled and called from a short program, and every step below ran. The module's own Makefile only syntax-checks, so the program is one you write. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl validate preferred-routers` exit 0; a scratch C program linked with `preferred_router.c` returned the outputs shown. |

## What it is

A routing-preference policy. A small config file (`preferred-routers.conf`) names a country, a preferred role and a minimum score. The C function `sleela_preferred_router_select` returns a role, a preference tier from 1 to 5 and the VM generation for an HTTP protocol string ([`README.md`](../../../preferred-routers/README.md)). The tier is a SLeeLa preference, not a measurement of latency or quality. The module does not install routes. `http-1.0` and `http-2.0` compile `preferred_router.c` into their servers (`http-1.0/Makefile:16`, `http-2.0/Makefile:24`). A data catalog (`preferred-routers.csv` / `.json`, 996 rows for 249 countries) sits next to the code.

## Before you start

- Work from the repository root. You need `cc`, `c++`, `make` and `python3`.
- `sleelactl` needs the usual environment:

```sh
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

- Do not run `refresh_peeringdb.py` for this tutorial. It calls the PeeringDB web API and rewrites the CSV and JSON in place. I did not run it, so its behavior is unverified.

## Step 1 - Syntax-check the module

```sh
bin/sleelactl validate preferred-routers
```

```
  make -C preferred-routers syntax
cc -std=c11 -O2 -Wall -Wextra -Wpedantic -I. -fsyntax-only preferred_router.c
g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic -I. -fsyntax-only preferred_router.cpp
  ✓ preferred-routers validated (syntax)
```

Exit 0. `-Wall` prints several warnings for `preferred_router.c` (misleading indentation, and `snprintf` truncation notes on the 16-byte country and 64-byte role fields). They are warnings only. The Makefile's `all` target is `syntax`, and `clean` is `true`. No binary is built.

## Step 2 - Write a small caller

The module has no CLI, so write one. This program asks for the router for `HTTP/3.0` and prints it.

```sh
mkdir -p /tmp/pr-demo && cat > /tmp/pr-demo/demo.c <<'EOF'
#include "preferred_router.h"
#include <stdio.h>
int main(int argc, char **argv) {
    sleela_preferred_router r;
    int rc = sleela_preferred_router_select(argc > 1 ? argv[1] : NULL, "HTTP/3.0", &r);
    printf("rc=%d country=%s role=%s preference=%d vm=%d/%s %s\n", rc, r.country, r.role,
           r.routing_preference, r.vm_generation, r.vm_name, r.vm_path);
    printf("candidate=%s\n", r.candidate);
    printf("packet_policy(HTTP/3.0, min 5) = %d\n", sleela_preferred_router_packet_policy("HTTP/3.0", 5));
    return 0;
}
EOF
cc -std=c11 -Ipreferred-routers /tmp/pr-demo/demo.c preferred-routers/preferred_router.c -o /tmp/pr-demo/demo
```

The compile prints `-Wmisleading-indentation` warnings from `preferred_router.c` and succeeds.

## Step 3 - Select with defaults and with the repository config

```sh
/tmp/pr-demo/demo
/tmp/pr-demo/demo preferred-routers/preferred-routers.conf
```

Both print:

```
rc=0 country=US role=national_backbone preference=5 vm=3/Specialist /3
candidate=US — configured national_backbone anchor
packet_policy(HTTP/3.0, min 5) = 1
```

The first call has no config path and no `SLEELA_PREFERRED_ROUTER_CONFIG`, so the C code uses its built-in defaults (US, `national_backbone`, `min_score=4`). The repository file sets the same values.

## Step 4 - Change the policy

```sh
printf 'country=DE\npreferred_role=regional_carrier\nmin_score=5\n' > /tmp/pr-demo/de.conf
/tmp/pr-demo/demo /tmp/pr-demo/de.conf
SLEELA_PREFERRED_ROUTER_CONFIG=/tmp/pr-demo/de.conf /tmp/pr-demo/demo
```

```
rc=0 country=DE role=national_backbone preference=5 vm=3/Specialist /3
candidate=DE — configured national_backbone anchor
packet_policy(HTTP/3.0, min 5) = 1
```

The config asked for `regional_carrier` (tier 4) with `min_score=5`. The selector raised the role to `national_backbone` (tier 5). Scores are fixed in code: `national_backbone` and `national_ixp` are 5; `regional_carrier` and `international_dc` are 4; any other role is 1. The same output appears whether you pass the path or set the environment variable.

```sh
printf 'country=FR\npreferred_role=international_dc\nmin_score=4\n' > /tmp/pr-demo/fr.conf
/tmp/pr-demo/demo /tmp/pr-demo/fr.conf
```

```
rc=0 country=FR role=international_dc preference=4 vm=3/Specialist /3
candidate=FR — configured international_dc anchor
packet_policy(HTTP/3.0, min 5) = 1
```

The last line is `1` even though FR's tier is 4. `sleela_preferred_router_packet_policy` calls `select` with a `NULL` path, so it reads the environment variable or the defaults, not the file you passed to your own `select` call. Set `SLEELA_PREFERRED_ROUTER_CONFIG` if you want the policy hook to see your file.

## Step 5 - Look at the catalog

```sh
python3 - <<'EOF'
import csv, collections
rows = list(csv.DictReader(open('preferred-routers/preferred-routers.csv')))
print(len(rows), len({r['country_code'] for r in rows}))
print(collections.Counter((r['role'], r['routing_preference']) for r in rows))
print(collections.Counter(r['selection_mode'] for r in rows))
EOF
```

```
996 249
Counter({('national_backbone', '5'): 249, ('national_ixp', '5'): 249, ('regional_carrier', '4'): 249, ('international_dc', '4'): 249})
Counter({'role-anchor': 996})
```

This is what `README.md` and `DATA.md` say: four roles per country, every row a placeholder `role-anchor` and none refreshed from PeeringDB. `preferred-routers.json` has `schema_version` 1.1 and the same 996 entries. `backup/MANIFEST.md` lists earlier editions.

## What you did

You syntax-checked the module, linked the selector into a program, read the policy from defaults, a file and an environment variable, saw the `min_score` escalation, and checked the catalog totals.

## What the other sleelactl verbs do for this module

| Command | Result |
|---|---|
| `bin/sleelactl negotiate preferred-routers` | "protocol-metadata operation defined by the generated spec", points to "the negotiate section in preferred-routers/README.md", then `no standalone negotiate CLI is built`. Exit 3. The README has no such section. |
| `bin/sleelactl grade preferred-routers` | same, for `grade`. Exit 3. |
| `bin/sleelactl simulate preferred-routers` | `no simulator registered (sim= unset)`. Exit 3. |
| `bin/sleelactl info preferred-routers` | exit 0; `see: preferred-routers/README.md`; docs `DATA.md`, `README.md`, `REFRESH.md` |

## Things to know

- **The selector does not read the catalog.** `preferred_router.c` never opens the CSV or JSON. The `candidate` string is either `candidate=` from the conf or a generated `"<country> — configured <role> anchor"`. The country in the config is not checked against the catalog. The `international` key is parsed and then discarded (`(void)international`).
- **The C and C++ versions differ.** `preferred_router.cpp` has no `sleela_preferred_router_packet_policy` (linking my demo against it fails with an undefined reference). It does not fill `vm_generation`, `vm_name`, `vm_path`, `protocol` or `server_surface` (I called it with `HTTP/3.0` and got `vm=0/` and an empty protocol). When the config is missing it returns `-2` instead of using defaults (`rc=-2` for `""` and `/no/such` with no env var); the C version returns 0 with defaults. When the C++ version does find a config, the country, role and score selection give the same results as C. The HTTP servers link the C file.
- **Doc formatting.** `README.md` (9 places) and `REFRESH.md` (2 places) contain literal backslash-backtick sequences where Markdown code spans were intended.
- `preferred-routers/preferred_router.o` is present in the directory (git-ignored `*.o`). I did not create or change it.

## Next steps

- [`README.md`](../../../preferred-routers/README.md), [`DATA.md`](../../../preferred-routers/DATA.md), [`REFRESH.md`](../../../preferred-routers/REFRESH.md)
- [resolver](resolver.md): the HTTP servers combine its result with this selector.
- [route](../explanations/route.md): the VM generation table.

## Sources

- Files read: `preferred-routers/README.md`, `DATA.md`, `REFRESH.md`, `Makefile`, `preferred_router.h`, `preferred_router.c`, `preferred_router.cpp`, `preferred-routers.conf`, `refresh_peeringdb.py`, the head of the CSV and the keys of the JSON; `bin/sleelactl.d/registry.conf` (line 66).
- Commands run: `bin/sleelactl [--dry-run] info|validate|negotiate|grade|simulate preferred-routers`, `make` in `preferred-routers/`, the scratch programs above (C and C++, built in `/tmp`), and the Python catalog count.
