# SLeeLa language examples

This is the guided language-learning sequence. The foundational examples use syntax 1.7; the newest integer examples demonstrate syntax 2.7 and 2.8.

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
10. [NEW signed wide integers](NEW-signed-wide-integers-2.7.sleela) — syntax 2.7 signed-width family.
11. [NEW unsigned wide integers](NEW-unsigned-wide-integers-2.8.sleela) — syntax 2.8 unsigned-width family.

For the child-friendly lesson order, vocabulary, and exercises, see [LEARNER-PATH](LEARNER-PATH.md).

## Commands

See [COMMANDS-and-workflow](COMMANDS-and-workflow.md) for the documented `version`, `check`, `run`, `compile`, `validate-artifact`, `xclass`, `langin`, `nordshrift`, and other command variations.

## Run

From the repository root, after building the runtime:

```sh
make -C impl
./impl/build/sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
./impl/build/sleela run lib/examples/sleela/CLASSIC-hello-world.sleela
```

The runtime's authoritative supported syntax range is reported by `sleela version` and defined in `impl/frontend/version.h`. The repository's `impl/README.md` contains an older syntax-range note; prefer the compiler's version output and `version.h`.

Some examples in other subsystem folders are contract, policy, or simulation demonstrations. Their comments and the catalogue explain when they do not access hardware, external services, or an implemented native bridge.
