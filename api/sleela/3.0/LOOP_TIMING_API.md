# SLeeLa Loop-Timing API (syntax 3.0)

**Normative specification:** [`SLEELA.syntax` §32](../../../SLEELA.syntax)
**Opcode:** `OP_SLEEP` (ISA index 157) — see [`lib/vm/InstructionSet.sleela`](../../../lib/vm/InstructionSet.sleela) and the generated [`vm` package page](version-014/package/vm.html) (`SLISASleep`).

## Purpose

The loop-timing API lets a program pace a `for`, `while`, or `do` loop without
hand-writing timing code: either a fixed pause per iteration, or a total
wall-clock budget spread evenly over a known iteration count. It also exposes a
general `sleep(ms)` built-in usable anywhere.

These are the **first statement-level, behavioral annotations** in SLeeLa. Every
other annotation (`@scope`, `@area`, `@next`, `@modium`, …) is declaration-level
metadata that changes nothing at runtime; `@loopDelay` and `@loopBudget` attach
to a loop statement and insert a real pause into its execution.

## Annotations

| Annotation | Form | Meaning |
|---|---|---|
| `@loopDelay` | `@loopDelay(<N>)` | sleep `N` **milliseconds** per iteration (bare number = ms) |
| `@loopDelay` | `@loopDelay(ms = <N>)` | sleep `N` milliseconds per iteration |
| `@loopDelay` | `@loopDelay(seconds = <N>)` | sleep `N` seconds per iteration |
| `@loopBudget` | `@loopBudget(seconds = <T>, iterations = <K>)` | spread `K` iterations over `T` seconds → `(T*1000)/K` ms each |
| `@loopBudget` | `@loopBudget(ms = <T>, iterations = <K>)` | spread `K` iterations over `T` ms → `T/K` ms each |

### Placement

The annotation sits on its own line immediately before the loop keyword it
governs. It is a parse error to place it before anything other than `for`,
`while`, or `do`.

```sleela
@loopDelay(ms = 250)
while (i < n) { ... }

@loopBudget(seconds = 10, iterations = 1000)
for (int i = 0; i < 1000; i = i + 1) { ... }
```

### Semantics

- The pause runs at the **top of each iteration** of the loop body. A loop that
  makes zero passes sleeps zero times.
- `@loopBudget` integer-divides the total by the iteration count. The result is
  the delay **added** per pass; it does not subtract the loop body's own run
  time, so a budget is a **floor**, not a hard real-time guarantee.
- Error paths fail closed at parse time: an annotation not followed by a loop,
  or `@loopBudget` without a positive `iterations`, is a syntax error.

## The `sleep` built-in

```sleela
sleep(<milliseconds>);   // block the current thread; 0 or negative is a no-op
```

`sleep(ms)` takes one integer argument and returns no value. It is callable
anywhere, not only in loops. Both the annotations and this built-in lower to the
same opcode, `OP_SLEEP`, which pops an integer millisecond count and blocks the
current thread via the portable `sltime_sleep_millis()` time primitive (POSIX
`nanosleep`, Windows `Sleep`).

## Worked example

A runnable reference lives at
[`examples/loop-timing-annotation-demo.sleela`](../../../examples/loop-timing-annotation-demo.sleela):

```sleela
#sleela 3.0
class LoopTimingDemo {
    void main() {
        int i = 0;
        @loopDelay(ms = 200)
        while (i < 3) { print("tick " + i); i = i + 1; }

        @loopBudget(ms = 900, iterations = 3)
        for (int k = 0; k < 3; k = k + 1) { print("budget " + k); }

        sleep(250);
        print("done");
    }
}
```

Run it:

```sh
sleela run examples/loop-timing-annotation-demo.sleela
```

## Safety

`sleep` and the loop-timing annotations only pause the current thread; they
grant no capability, perform no I/O, and do not bypass the memory manager,
security supervisor, or any routing policy. A delay is wall-clock pacing only.
