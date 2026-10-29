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
| **daemon** | long-lived process + systemd unit | `start stop restart reload status enable disable` | `systemctl` on the module's unit |
| **registry** | closed handler set, load/unload by id (`SLPluginService`) | `load unload enable disable list probe can status` | the `lib/<pkg>` Controller via `sleela run` |
| **datasource** | open/close/active data surface + pooling (`SLDataSource`) | `open close status available` | reports availability of the `lib/<pkg>` surface |
| **protocol** | spec / stack / reference model | `negotiate grade simulate validate info` | the module's `make` targets / sim CLI |
| **tool** | on-demand, one-shot toolchain component | `run build inspect info` | the module's built CLI / `make` |
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
```

`sleelactl <verb> all` applies the verb to every module whose nature accepts it
(e.g. `sleelactl enable all` enables the daemons; `sleelactl build all` builds
the tools).

## The module registry

The module → nature map lives in
[`bin/sleelactl.d/registry.conf`](sleelactl.d/registry.conf) — plain text, one
module per line (`name  nature  handle  summary`). Adding a module is a one-line
append; the dispatcher is never edited. `sleelactl` finds the registry beside
the script, in the installed runtime tree, or via `SLEELACTL_REGISTRY=<path>`.

## Honest degradation

- No `systemctl` on the host → daemon verbs report that plainly and exit
  non-zero (no fake start).
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
