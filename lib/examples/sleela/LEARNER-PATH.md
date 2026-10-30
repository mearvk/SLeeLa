# A friendly first path through SLeeLa

This path is designed for a first-time learner. You do not need to understand the whole language at once. Read one lesson, predict what the program will print, run it, and then make one small change.

## Before you begin

Build the repository toolchain from the repository root:

```sh
make -C impl
```

Check that the runtime is available:

```sh
./impl/build/sleela version
```

The runtime README documents syntax versions 1.3 through 1.7. These examples use `#sleela 1.7`.

## The nine small lessons

| Step | File | What to learn |
|---:|---|---|
| 1 | [CLASSIC-hello-world.sleela](CLASSIC-hello-world.sleela) | A class, the `main` method, and printing text |
| 2 | [ORIGINAL-fizz-buzz.sleela](ORIGINAL-fizz-buzz.sleela) | Numbers, a `for` loop, `if`, and the remainder operator |
| 3 | [USED-methods-and-return-values.sleela](USED-methods-and-return-values.sleela) | Give a method a value and get an answer back |
| 4 | [USED-switch-selection.sleela](USED-switch-selection.sleela) | Choose one path from several cases |
| 5 | [USED-short-circuit-guards.sleela](USED-short-circuit-guards.sleela) | Combine conditions without evaluating unnecessary checks |
| 6 | [NEW-dynamic-arrays.sleela](NEW-dynamic-arrays.sleela) | Store a list of values and add another one |
| 7 | [NEW-struct-record.sleela](NEW-struct-record.sleela) | Keep related data together |
| 8 | [NEW-nested-classes.sleela](NEW-nested-classes.sleela) | Put a helper type inside the class that uses it |
| 9 | [NEW-native-financial-import.sleela](NEW-native-financial-import.sleela) | Import a library and call its functions |

## How to run each lesson

Replace the filename with the lesson you want to try:

```sh
./impl/build/sleela check lib/examples/sleela/CLASSIC-hello-world.sleela
./impl/build/sleela run lib/examples/sleela/CLASSIC-hello-world.sleela
```

You can also create a reusable runnable artifact:

```sh
./impl/build/sleela compile lib/examples/sleela/CLASSIC-hello-world.sleela -o /tmp/hello.sleela
./impl/build/sleela validate-artifact /tmp/hello.sleela
./impl/build/sleela run /tmp/hello.sleela
```

## Three habits for learning

1. **Predict first.** Read the program and write down what you think it will print.
2. **Change one thing.** Change one number or one word, then run it again.
3. **Keep the example working.** If a change causes an error, undo just that change and try a smaller one.

## A small challenge

After completing the nine lessons, write a program that stores three scores in an array, calculates their total, and prints the total. Then add a fourth score with `arrayPush` and print the new array length.

## Important distinction

The examples in this directory are intended to be runnable language lessons. Some examples in other subsystem folders are explicitly labelled as simulations, checklists, policy tests, or draft API vocabulary. Their README files explain which ones require native bridges, external services, or hardware.
