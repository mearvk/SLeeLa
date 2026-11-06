# SLeeLa language examples

This is the guided language-learning sequence. The foundational examples use syntax 1.7; the advanced examples use documented syntax 2.1 and the newest integer examples demonstrate syntax 2.7 and 2.8.

## Start here

1. [CLASSIC Hello World](CLASSIC-hello-world.sleela) — class, entry point, and `print`.
2. [ORIGINAL FizzBuzz](ORIGINAL-fizz-buzz.sleela) — variables, loops, remainder, and decisions.
3. [USED methods](USED-methods-and-return-values.sleela) — parameters, return values, and reuse.
4. [USED switch selection](USED-switch-selection.sleela) — multi-way branching.
5. [USED short-circuit guards](USED-short-circuit-guards.sleela) — safe boolean expressions.
6. [NEW dynamic arrays](NEW-dynamic-arrays.sleela) — indexed data and append.
7. [NEW struct records](NEW-struct-record.sleela) — data aggregates.
8. [NEW nested classes](NEW-nested-classes.sleela) — grouping related types.
9. [NEW native imports](NEW-native-financial-import.sleela) — calling library modules.

## Advanced feature examples

| File | Feature |
|---|---|
| [NEW-file-pipes](NEW-file-pipes.sleela) | Pipe creation, worker thread, read/write, close, join |
| [NEW-Synchro](NEW-synchro-measure.sleela) | Packet dispatch and honest latency/loss measurements |
| [NEW-Munction](NEW-munction-file-reach.sleela) | Composed local file-channel reach and receipt |
| [NEW-Best-of](NEW-bestof-route.sleela) | Weighted route selection using measured outcomes |
| [NEW-struct-network-transport](NEW-struct-network-transport.sleela) | Pack/unpack structs across loopback TCP |
| [NEW-time-api](NEW-time-api.sleela) | UTC, monotonic time, precision, HTTP date, JSON time |
| [NEW-static-protected-access](NEW-static-protected-access.sleela) | Static protected members and controlled access |

## Newest syntax examples

- [Signed wide integers — syntax 2.7](NEW-signed-wide-integers-2.7.sleela)
- [Unsigned wide integers — syntax 2.8](NEW-unsigned-wide-integers-2.8.sleela)

For the child-friendly lesson order, vocabulary, and exercises, see [LEARNER-PATH](LEARNER-PATH.md).

## Commands

See [COMMANDS-and-workflow](COMMANDS-and-workflow.md) for the command overview. The option-by-option cookbook is indexed below; it follows `man/man1/sleela.1` and the C++ command dispatcher.

## Command-line examples

| Example | Coverage |
|---|---|
| [CLI-01](CLI-01-global-memory-options.md) | Global memory-manager flags and size suffixes |
| [CLI-02](CLI-02-source-and-artifact.md) | Version, check, run, compile, artifact validation |
| [CLI-03](CLI-03-multi-source-compilation.md) | Multiple source inputs |
| [CLI-04](CLI-04-native-and-exec.md) | Native/exec, config, memory option, option terminator |
| [CLI-05](CLI-05-sleeladoc-topic-lookup.md) | SleelaDoc topic/package/class lookup |
| [CLI-06](CLI-06-sleeladoc-generate-live.md) | Documentation generation and live parsing |
| [CLI-07](CLI-07-sleeladoc-facade.md) | Facade search and resource caps |
| [CLI-08](CLI-08-vm-commands.md) | VM list/select/info/run |
| [CLI-09](CLI-09-modium-commands.md) | Modium scan/status/restore |
| [CLI-10](CLI-10-registry-commands.md) | Registry read/write/export |
| [CLI-11](CLI-11-xclass-and-langin.md) | XClass and JVM-family source modes |
| [CLI-12](CLI-12-nordshrift-options.md) | Nordshrift emit/roundtrip/targets |
| [CLI-13](CLI-13-http-server-options.md) | HTTP versions and server flags |
| [CLI-14](CLI-14-design-activity.md) | Seven design-activity axes |
| [CLI-15](CLI-15-defender-safety.md) | Defender safety gates |
| [CLI-16](CLI-16-environment-and-exit-status.md) | Environment variables and exit status |

These are documentation examples. Confirm each operation against the exact
binary built on the target platform; server, VM, registry, and Defender actions
may depend on configuration or have side effects.

## Run

From the repository root, after building the runtime:

```sh
make -C impl
./impl/build/sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
./impl/build/sleela run lib/examples/sleela/CLASSIC-hello-world.sleela
```

The authoritative supported syntax range is reported by `sleela version` and defined in `impl/frontend/version.h`. The repository's `impl/README.md` contains an older syntax-range note; prefer the compiler's version output and `version.h`.

Some examples in other subsystem folders are contract, policy, or simulation demonstrations. Their comments and the catalogue explain when they do not access hardware, external services, or an implemented native bridge.
