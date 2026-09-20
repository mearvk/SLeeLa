# Lesson 05 — The `.sst` Sheet & Nordshrift

> Series: [index](README.md) · prev: [04](04-control-flow.md) · next: [06 — The triplet](06-the-triplet.md)

You have Wrappers. Now you need something to **drive** them. That is the job of a
`.sst` **control sheet** and the **Nordshrift** driver.

A sheet is **not** source — it names *which* sources to build, *which* target to
emit, and declares supporting models (network, finance, subjects).

## A real sheet, annotated

From `impl/nordshrift/examples/demo/build.sst`:

```sst
#nordshrift 1.0
#sleela     1.1

sheet demo:
  version      1.0.0
  author       "Sleela Design Council"
  description  "Transpiles the demo Sleela sources; sleela target runs on the core."

source:
  root     "src"
  glob     "**/*.sleela"
  exclude  [ "build/**", "**/build/**" ]

target:
  root            "out"
  layout          mirror-source
  java-version    21
  package-root    "com.example.demo"
  target-language sleela
```

- **`#nordshrift` / `#sleela`** — version pragmas for the driver and language.
- **`sheet demo:`** — names the build unit.
- **`source:`** — where the Wrappers are (`root`) and which to include (`glob`),
  minus `exclude`d paths.
- **`target:`** — where output goes and which triplet target to emit
  (`target-language sleela | java | c`).

## Running it

Once the compiler is built (`impl/build/nordshrift` — see Lesson 09 for the
verified build), from the repo root:

```sh
# Type-check the sheet and its sources without emitting:
impl/build/nordshrift check impl/nordshrift/examples/demo/build.sst

# Build (emit the target selected by target-language):
impl/build/nordshrift build impl/nordshrift/examples/demo/build.sst
```

For the **sleela** target you'll see, per source, a line like:

```text
nordshrift: .../src/Demo.sleela -> .../src/build/Demo.sleela (runnable Sleela Core artifact)
nordshrift: .../src/build/Demo.sleela -> .../src/build/Demo.sleela.ledger (+ .qr.svg)
```

That second line is the compiler emitting a **`.ledger`** next to the artifact —
the QR-insignia + SHA-256-chain record you'll meet in Lesson 10.

## The mental model

```text
   .sleela sources  ─┐
                     ├─►  Nordshrift (driven by .sst)  ─►  target (java|sleela|c)
   .sst control sheet ┘                                     └─►  .ledger (per artifact)
```

**Next:** [Lesson 06 — The triplet: Java, Sleela, C](06-the-triplet.md)
