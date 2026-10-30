# SLeeLa language examples

This is the guided language-learning sequence. The source files use the runtime's documented syntax range (1.3–1.7) and declare `#sleela 1.7`.

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

For the child-friendly lesson order, vocabulary, and small exercises, see [LEARNER-PATH](LEARNER-PATH.md).

## Commands

See [COMMANDS-and-workflow](COMMANDS-and-workflow.md) for the documented `version`, `check`, `run`, `compile`, `validate-artifact`, `xclass`, `langin`, and `nordshrift` command variations.

## Run

From the repository root, after building the runtime:

```sh
./impl/build/sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
./impl/build/sleela run lib/examples/sleela/CLASSIC-hello-world.sleela
```

For each lesson, read the comments, predict the output, run the file, and then change one input to see how the result changes. The examples are teaching material; do not assume every other subsystem demonstration is a live hardware or external-service integration.
