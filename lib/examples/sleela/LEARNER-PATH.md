# A friendly first path through SLeeLa

This path is designed for a first-time learner. You do not need to understand the whole language at once. Read one lesson, predict what the program will print, run it, and then make one small change.

## Before you begin

Build the repository toolchain from the repository root:

```sh
make -C impl
```

Check the supported syntax range:

```sh
./impl/build/sleela version
```

The compiler source currently defines the supported syntax range as **1.0–2.8** in `impl/frontend/version.h`. The first lessons use syntax 1.7; the advanced examples use 2.1, and the newest integer lessons use 2.7 and 2.8.

## The eleven first lessons

| Step | File | What to learn |
|---:|---|---|
| 1 | [CLASSIC-hello-world.sleela](CLASSIC-hello-world.sleela) | A class, the `main` method, and printing text |
| 2 | [ORIGINAL-fizz-buzz.sleela](ORIGINAL-fizz-buzz.sleela) | Numbers, a `for` loop, `if`, and the remainder operator |
| 3 | [USED-methods-and-return-values.sleela](USED-methods-and-return-values.sleela) | Give a method a value and get an answer back |
| 4 | [USED-switch-selection.sleela](USED-switch-selection.sleela) | Choose one path from several cases |
| 5 | [USED-short-circuit-guards.sleela](USED-short-circuit-guards.sleela) | Combine conditions without unnecessary checks |
| 6 | [NEW-dynamic-arrays.sleela](NEW-dynamic-arrays.sleela) | Store a list of values and add another one |
| 7 | [NEW-struct-record.sleela](NEW-struct-record.sleela) | Keep related data together |
| 8 | [NEW-nested-classes.sleela](NEW-nested-classes.sleela) | Put a helper type inside the class that uses it |
| 9 | [NEW-native-financial-import.sleela](NEW-native-financial-import.sleela) | Import a library and call its functions |
| 10 | [NEW-signed-wide-integers-2.7.sleela](NEW-signed-wide-integers-2.7.sleela) | Meet the newer signed integer widths |
| 11 | [NEW-unsigned-wide-integers-2.8.sleela](NEW-unsigned-wide-integers-2.8.sleela) | Meet positive-only unsigned integer widths |

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

Write a program that stores three scores in an array, calculates their total, and prints the total. Then add a fourth score with `arrayPush` and print the new array length.

## Where to go next

After the first lessons, open [the advanced feature catalogue](README.md#advanced-feature-examples). Try the pipe, time, or struct-transport example next. Network examples use loopback only; read each source comment before running it.

## Important distinction

The examples in this directory are intended to be language demonstrations. Some examples in other subsystem folders are explicitly labelled as simulations, checklists, policy tests, or draft API vocabulary. Their README files explain which ones require native bridges, external services, or hardware.
