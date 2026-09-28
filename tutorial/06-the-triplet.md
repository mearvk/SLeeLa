# Lesson 06 — The Triplet: Java, Sleela, C

> Series: [index](README.md) · prev: [05](05-sst-and-nordshrift.md) · next: [07 — Conducted methods](07-conducted-methods.md)

One SLeeLa source renders faithfully into **three** targets — the **triplet**.
You choose the target with `target-language` in the sheet.

| Target | What you get | Note |
|---|---|---|
| `java` | idiomatic Java: a class with static methods | printed to stdout |
| `sleela` | canonical Sleela, **runnable** on the C core | written as an artifact |
| `c` | C with a tiny tagged-value runtime | printed to stdout |

## Why three?

Because SLeeLa's governing principle (§20) is that *any* client — C, C++, Java,
or other — can implement the connector without becoming a SLeeLa runtime. The
triplet proves the same logical program is expressible in each idiom.

## Retarget the same source

Only one line changes in the sheet:

```sst
target:
  target-language java     # or: sleela | c
```

Then:

```sh
impl/build/nordshrift build impl/nordshrift/examples/demo/build.sst
```

- With **`sleela`**, the driver writes a runnable artifact (and its `.ledger`)
  and reports the path.
- With **`java`** or **`c`**, the driver prints the emitted source under a banner
  like `// ==== <src>  ->  Java ====` so you can see the rendering.

## The runnable Sleela artifact

The `sleela` target is special: its output runs directly on the Sleela Core VM.
That's what makes the demo end-to-end — Nordshrift compiles `Demo.sleela` to a
runnable `build/Demo.sleela` artifact, then the core can execute it.

## Same logic, three faces

A method like `square(n)` becomes:

- **Java:** `static long square(long n) { return n * n; }`
- **Sleela:** the canonical `int square(int n) { return n * n; }`
- **C:** a tagged-value function over the tiny runtime.

The *behavior* is identical; only the idiom differs. That fidelity across
targets is a core part of "SLeeLa at her source best."

**Next:** [Lesson 07 — Conducted methods](07-conducted-methods.md)
