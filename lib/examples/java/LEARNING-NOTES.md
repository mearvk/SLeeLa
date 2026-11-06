# Java interop examples — what to learn here

These examples teach the **decisions a Java launcher makes before any code
runs**: is the runtime present, which class provides a name, and which main
class will actually execute. They are preflight/decision models — **no JVM is
started and no JAR is executed** — so each program is deterministic.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-launch-preflight.sleela`](BASIC-launch-preflight.sleela) | The launch checklist: runtime → locate → validate → resolve → launch. |
| Moderate | [`MODERATE-classpath-resolution.sleela`](MODERATE-classpath-resolution.sleela) | Class-path search order: the first entry that provides a class wins. |
| Advanced | [`ADVANCED-main-class-resolution.sleela`](ADVANCED-main-class-resolution.sleela) | Main-class precedence: CLI override beats manifest `Main-Class`; neither = reject. |

## What we want you to take away

1. **Most "wrong version loaded" bugs are ordering bugs.** The moderate example
   makes two class-path roots both provide `com.app.Util`; the earlier root wins.
   This single rule explains a huge share of real Java classpath confusion.
2. **Precedence, then fail closed.** The advanced example resolves the launch
   target in a fixed order (command line → manifest → reject). When nothing
   supplies a main class, it refuses to launch rather than guessing — the safe
   default.
3. **Preflight is cheap; a bad launch is not.** Checking the runtime, the JAR,
   the manifest, and the main class *before* starting the JVM turns a confusing
   runtime crash into a clear, early error message.

## Practical notes for SLeeLa 1.7

- **"Not provided" is modeled as `""`.** The advanced resolver treats an empty
  string as "absent" and compares with `==`/`!=`. This keeps the example self
  contained without any null handling.
- **Lookup over an object array** uses the standard idiom (static-field array,
  elements built in typed locals, read via typed locals) — see the moderate
  resolver loop.
- **Interop is a contract, not a call here.** These files model the launcher's
  decisions in SLeeLa; actually starting a JVM is a separate native-bridge step
  the Java subsystem README documents.

## Run them

```sh
sleela run lib/examples/java/MODERATE-classpath-resolution.sleela
sleela run lib/examples/java/ADVANCED-main-class-resolution.sleela
```

Output is deterministic: `com.app.Util` resolves to `build/classes` (first
wins); the advanced resolver runs the CLI class in case A, the manifest class in
case B, and rejects case C.
