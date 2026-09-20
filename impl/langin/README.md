# impl/langin — JVM language family as SLeeLa input

`langin` makes SLeeLa accept the **JVM "brother languages"** as modern compiled
input types. They all compile to JVM bytecode and share one runtime, so SLeeLa
ingests them as one family and reconstructs a runnable `sleela::Program` from
each — the same way [`impl/xclass`](../xclass) ingests SecureJDK 28 `.xclass`
files.

| Language | Extensions | Role in the family |
|---|---|---|
| **Java** | `.java` | the foundation of the ecosystem |
| **Kotlin** | `.kt`, `.kts` | pragmatic, null-safe, 100% Java-interop |
| **Scala** | `.scala`, `.sc` | object-oriented + advanced functional |
| **Groovy** | `.groovy`, `.gvy` | flexible/dynamic; drives Gradle |
| **Clojure** | `.clj`, `.cljs`, `.cljc` | a Lisp dialect; data-as-code |

## What it does

It is a **structural transpile** (skeleton only), native and dependency-free —
no `javac`/`kotlinc`/`scalac`/`groovyc`/Clojure runtime required. It recovers the
package/namespace, the class/interface/object/trait/namespace name, fields, and
method signatures, maps their types onto SLeeLa's surface types, and can emit
`.sleela` source and/or an `.xclass` descriptor. Method **bodies are synthesized
skeletons** (a default return) — structure and type contracts are preserved, not
executable logic. See [`LANGIN.model`](LANGIN.model) (LNG-0001) for the normative
mapping.

## Usage

```sh
# Reconstruct + run (default). Also works via `sleela run <file>` or `sleela <file>`.
sleela langin --run       impl/langin/samples/Greeter.kt

# Emit the reconstructed Sleela source
sleela langin --emit-sleela impl/langin/samples/Greeter.kt

# Emit an .xclass structural descriptor (compiler="langin")
sleela langin --emit-xclass impl/langin/samples/Greeter.java

# One-line summary per ingested unit
sleela langin --info      impl/langin/samples/*.scala
```

The SHA-256 execution gate (`SLEELA_SHA256_MANIFEST`) applies as on every other
run path.

## Samples

[`samples/`](samples) carries one `Greeter` per language
(`Greeter.java`, `Greeter.kt`, `Greeter.scala`, `Greeter.groovy`,
`greeter.clj`), each with a couple of fields and methods, for exercising the
four modes above.

## Build

Built by [`../Makefile`](../Makefile) into the `sleela` binary on all three OSes
(the Makefile auto-detects Linux/macOS/Windows and adjusts link flags); `langin`
adds no new dependency.

## Honesty about coverage

`langin` reports only what it can structurally recover from the source text; it
is deliberately skeleton-only and omits (rather than guesses) anything it cannot
recognize. Groovy fields declared without a modifier are an example of a
recognized-methods / partially-recognized-fields case — the method surface is
reconstructed regardless.
