# sleelactl — the unified SLeeLa control plane

`sleelactl` is one command that controls every SLeeLa module family: the
daemons and servers, the HTTP and wireless protocols, the codec / compression /
video registries, the compiler and decompiler tools, and the availability of
the data systems.

```
sleelactl <verb> <module> [args]      control one module
sleelactl <verb> all                  fan a verb across every compatible module
sleelactl list [nature]               list modules (optionally by nature)
sleelactl status [module|all]         show running status
sleelactl natures                     explain the natures and their verbs
sleelactl help [verb|module]          help, or the verbs legal for a module
```

## Why the verbs differ by module

SLeeLa modules are genuinely different *kinds* of thing, so forcing one verb set
(`start/stop`) onto all of them would be dishonest — the repo's own
[`lib/core/SERVICE-STANDARD.md`](../lib/core/SERVICE-STANDARD.md) says
"a worse abstraction forced onto a subsystem is worse than a little
duplication." `sleelactl` therefore classifies each module by **nature** and
offers only the verbs that fit it. An unsupported verb is **refused** with the
correct verb set — `sleelactl` never fakes an action a module cannot perform.

| Nature | What it is | Legal verbs | Mechanism |
| --- | --- | --- | --- |
| **daemon** | long-lived process + systemd unit | `start stop restart reload status enable disable` | `systemctl --system` by default, or `systemctl --user` when `SLEELACTL_SYSTEMCTL_SCOPE=user` |
| **registry** | closed handler set, load/unload by id (`SLPluginService`) | `load unload enable disable list probe can status` | the `lib/<pkg>` Controller via `sleela run` |
| **datasource** | open/close/active data surface + pooling (`SLDataSource`) | `open close status available` | reports availability of the `lib/<pkg>` surface |
| **protocol** | spec / stack / reference model | `negotiate grade simulate validate info` | the module's `make` targets / sim CLI |
| **tool** | on-demand, one-shot toolchain component | `run build inspect info` | the module's built CLI / `make` |
| **subject** | native C/C++ subject library compiled into the runtime (`import <name>;`) | `run info` | its reference demo via `sleela run` |
| **library** | pure library | `info` | informational only |

The verb vocabulary is drawn directly from the code:
`start/enable` from the systemd units (`server-edition/.../sleela-server.service`,
`antivirus/systemd/sleela-antivirus.service`);
`load/unload/canProcess` from `SLPluginService` and `lib/codecs`;
`open/close/active` from `SLDataSource`;
`negotiate/grade/simulate/validate` from the HTTP and CDMA protocol trees.

## Examples

```sh
sleelactl start server-edition      # start the server daemon (systemd)
sleelactl enable antivirus          # enable the antivirus coordinator at boot
sleelactl status all                # status of every controllable module

sleelactl load codecs 2             # load FLAC (id 2) in the codec registry
sleelactl list registry             # just the registry-nature modules
sleelactl can video 5               # can the video registry process handler 5?

sleelactl available database        # report the database data-source availability
sleelactl open ledger               # how to open the ledger data surface

sleelactl validate http-9.0         # validate the HTTP/9.0 protocol spec
sleelactl simulate cdma --std wcdma # run the CDMA simulator
sleelactl build decompiler          # build the Slecompiler CLI
sleelactl run decompiler <binary>   # invoke the built decompiler

sleelactl run math                  # exercise the native math subject (__NativeMath)
sleelactl info math                 # how the math subject is reached (import math;)\nsleelactl run physics               # execute the registered physics reference demo

sleelactl info native-drive
sleelactl build native-drive main.c helper.c -o build/app
sleelactl build native-drive plugin.cpp --kind shared --require-sleela-abi -o build/plugin.so
sleelactl run native-drive --capture -- build/app
sleelactl inspect native-drive xclass --mode info Example.xclass
sleelactl inspect native-drive service --name worker --executable /opt/worker --working-directory /var/lib/worker
```

The **subject** nature is for the native C/C++ subject libraries compiled into
the `sleela` runtime (`impl/subjects/<name>/`). They are not daemons, handler
registries, data connections, or specs — a program reaches them with
`import <name>;` and qualified calls (e.g. `math.sqrt(144.0)`), which the
frontend lowers onto the native `__Native<Subject>` methods. `sleelactl run
<subject>` executes that native backend by running the subject's reference demo
through `sleela run`. There is deliberately no `check` verb: `sleela check` does
not load the SHEET-registered native subjects, so `run` is the one faithful
execution verb. Registered runnable subjects include `math` and `physics`. The math subject currently lowers to synthesized Core methods; the typed C++ `<cmath>` kernel is in `impl/subjects/math/math_kernel.hpp` and is wired into SLVM through append-only `OP_MATH_CALL` (179). Scalar numeric arguments execute in the kernel; VM-owned argument values use the synthesized Core helper fallback. Run `make -C impl test-math` for source-to-VM regression coverage and `make -C impl test-math-kernel` for standalone kernel coverage.

The `native-drive` tool builds multi-unit C/C++ executables and shared
libraries, compiles the supported SLeeLa AOT subset, executes processes without
an implicit shell, and routes XClass operations through the production loader.
Use `sleelactl inspect native-drive service ...` to render or explicitly install
a hardened systemd unit. The VM owns an authorized, bounded native-module
lifecycle API; bytecode dispatch, typed SLeeLa values, interpreter fallback,
and parser-enforced process annotations remain future compiler/runtime work. See
[Native Drive](../impl/native-drive/README.md).

`sleelactl <verb> all` applies the verb to every module whose nature accepts it
(e.g. `sleelactl enable all` enables the daemons; `sleelactl build all` builds
the tools).

## The module registry

The module → nature map lives in
[`bin/sleelactl.d/registry.conf`](sleelactl.d/registry.conf) — plain text, one
module per line (`name  nature  handle  summary`). Adding a module is a one-line
append; the dispatcher is never edited. `sleelactl` finds the registry beside
the script, in the installed runtime tree, or via `SLEELACTL_REGISTRY=<path>`.

## systemd scope and honest degradation

Daemon units use the **system** manager by default. Root invokes `systemctl --system` directly; an unprivileged caller uses `sudo systemctl --system` and therefore needs sudo authorization. For a unit installed in the caller's user manager, set `SLEELACTL_SYSTEMCTL_SCOPE=user`; this invokes `systemctl --user` and never sudo. The registry's daemon handles must match the selected manager's unit scope.

- Invalid `SLEELACTL_SYSTEMCTL_SCOPE` values are rejected; only `system` and `user` are accepted.
- No `systemctl` on the host → daemon verbs report that plainly and exit non-zero (no fake start).
- Missing `sudo` for system-scope control by a non-root caller is reported before attempting a command.
- SLeeLa runtime not built → registry verbs say so and point at
  `(cd impl && make build/sleela)` (no fake load/decode).
- Data systems → `sleelactl` reports *availability*; the actual open/close acts
  on an `SLDataSource` instance inside a running SLeeLa program, which the tool
  shows you how to write rather than pretending to hold a connection itself.

## Install

`sleelactl` and its registry are installed by the standard installer:

```sh
make -C install install          # installs sleela, sleelvac, nordshrift, AND sleelactl
```

The command lands in `<prefix>/bin/sleelactl`; its registry in
`<prefix>/lib/sleela/bin/sleelactl.d/registry.conf`.


## Native Drive service units

Native Drive's unit generator is exposed through the tool nature:

```sh
sleelactl inspect native-drive service --name worker --executable /opt/sleela/bin/worker --working-directory /opt/sleela
sleelactl inspect native-drive service --name worker --executable /opt/sleela/bin/worker --working-directory /opt/sleela --install --enable --start
sleelactl inspect native-drive service --name worker --status
sleelactl inspect native-drive service --name worker --rollback
```

The generator defaults to a user unit. Add `--system --user ACCOUNT` for an explicitly scoped system unit (run installation as root). Installation does not enable or start a unit unless the corresponding flags are supplied. Replacement is opt-in and preserves one rollback copy; failed installs attempt to restore the prior unit and undo enable/start side effects. Lifecycle operations are Linux/systemd-only; they do not claim to provide Windows Service Control Manager or macOS launchd integration.


## Regression test

Run the focused scope-selection test from the repository root:

```sh
sh tests/sleelactl_systemctl_scope_test.sh
```

It stubs `systemctl`, `sudo`, and the caller UID; it does not start or modify a real service.
