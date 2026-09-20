# Lesson 01 — What SLeeLa Is

> Series: [index](README.md) · next: [02 — Your first Wrapper™](02-first-wrapper.md)

**SLeeLa** is a Java-like programming language that runs on a Turing-complete,
thread-friendly **C/C++ execution core**. You write ordinary-looking classes and
methods; the core executes them through a small, stable bytecode VM.

Three ideas make SLeeLa distinct:

## 1. The Wrapper™ — source is a governed unit

A SLeeLa source file has the extension `.sleela` and is called a **Wrapper™**.
It is the *program*, and it is governed by the Sleela Language Metadocument
(SL-META-0001). A `.sst` control sheet is **not** source — it *drives* which
sources get built (that's Lesson 05).

```sleela
#sleela 1.2
class Hello {
    void main() {
        print("hello from a Wrapper");
    }
}
```

The first line, `#sleela <version>`, declares the language generation the file
targets (you'll see `1.1` and `1.2` in the repo).

## 2. Nordshrift and the triplet

**Nordshrift** is the transpiler *driver*. Given a `.sst` sheet, it renders each
source into one of three **triplet** targets:

- **Java** — idiomatic Java (class + static methods),
- **Sleela** — canonical Sleela, additionally *runnable* on the C core,
- **C** — C with a tiny tagged-value runtime.

One source, three faithful renderings. Lesson 06 runs all three.

## 3. A self-describing system

SLeeLa programs can ask about their own structure through **conducted methods**
(`role`, `insight`, `route`, `congruent`, `sysdepth`, …), resolved against a
shared model sheet (`SHEET.sheet`). That is how a program prints, e.g., the
*role* of an object it names — see Lesson 07.

## Where things live

| Thing | Path |
|---|---|
| The execution core, front end, VM | [`../impl/`](../impl/) |
| The Nordshrift driver | [`../impl/nordshrift/`](../impl/nordshrift/) |
| Example sources | [`../examples/`](../examples/), [`../impl/nordshrift/examples/`](../impl/nordshrift/examples/) |
| Language/spec docs | [`../SLEELA.md`](../SLEELA.md), [`../NORDSHRIFT.md`](../NORDSHRIFT.md), [`../GLOSSARY.md`](../GLOSSARY.md) |

## Try it (orientation only)

You don't need to build anything yet. Just look at a real Wrapper™:

```sh
sed -n '1,40p' examples/symmetry/src/core/Message.sleela
```

You'll see a `class`, integer methods, and `print(...)` — the same shapes you'll
write next.

**Next:** [Lesson 02 — Your first Wrapper™](02-first-wrapper.md)
