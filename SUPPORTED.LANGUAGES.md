# SUPPORTED.LANGUAGES.md

The languages and input formats **SLeeLa accepts as input** — the set it "trades
in." SLeeLa is a Java-like language on a Turing-complete C/C++ execution core;
around that core it ingests several source languages and control formats and
reconstructs a runnable Sleela program.

**US frame.** This registry is maintained under the project's US Standard
conduct frame (the same `conduct-frame="US"` used by the `.xclass` model and the
US-Policy note in the README). It records languages the project *accepts and
works with*; it makes no claim about import/export trade of software.

---

## Native language

| Language | Extension(s) | Accepted as | Notes |
|---|---|---|---|
| **Sleela** | `.sleela` | source **and** compiled artifact | The native language — a Java-like surface on the C/C++ core. A `.sleela` source file is a **Wrapper™**; a compiled `.sleela` is a runnable Core artifact. |

## JVM language family — ingested by `langin`

The five JVM "brother languages" all compile to JVM bytecode and share one
runtime, so SLeeLa ingests them as one family
([`impl/langin`](impl/langin), spec LNG-0001). Each reconstructs a runnable
`sleela::Program`; SLeeLa can emit `.sleela` and/or `.xclass` from it.
Ingest is a **structural** (skeleton) transpile: package/namespace, class,
fields, and method signatures — not executable bodies.

| Language | Extension(s) | Family role |
|---|---|---|
| **Java** | `.java` | The foundation of the ecosystem. |
| **Kotlin** | `.kt`, `.kts` | Pragmatic, null-safe, 100% Java-interoperable. |
| **Scala** | `.scala`, `.sc` | Object-oriented + advanced functional. |
| **Groovy** | `.groovy`, `.gvy` | Flexible/dynamic; drives the Gradle build system. |
| **Clojure** | `.clj`, `.cljs`, `.cljc` | A Lisp dialect; data-as-code, pure-functional. |

Usage: `sleela langin [--run|--emit-sleela|--emit-xclass|--info] <file...>`,
or by extension via `sleela run <file>` / bare `sleela <file>`.

## Structural class ingest

| Format | Extension | Accepted as | Notes |
|---|---|---|---|
| **XClass** (SecureJDK 28 XML class) | `.xclass` | structural class input | Ingested by [`impl/xclass`](impl/xclass) (spec XCI-0001) into a runnable Sleela program; can be `--emit`ed back to `.sleela`. |

## Control / transpiler surface

| Format | Extension | Accepted as | Notes |
|---|---|---|---|
| **Nordshrift sheet** | `.sst` | control sheet (transpiler input) | Nordshrift (spec NS-SST-0001, `SST.model`) names Wrapper™ inputs and transpiles to the **triplet** target — **Java**, **Sleela**, or **C**. |

## Transpile / emit targets

For completeness, the formats SLeeLa **produces**:

| Target | Produced by | Notes |
|---|---|---|
| `.sleela` (source) | `langin --emit-sleela`, `xclass --emit`, Nordshrift | Reconstructed Sleela source. |
| `.sleela` (artifact) | `sleela compile` | Runnable Core artifact. |
| `.xclass` | `langin --emit-xclass` | Structural class descriptor (`compiler="langin"`). |
| Java / Sleela / C | Nordshrift `.sst` | The Nordshrift triplet targets. |

---

## Summary

**Accepted inputs:** `.sleela` (native, source + artifact); the JVM family —
**Java, Kotlin, Scala, Groovy, Clojure**; `.xclass`; and Nordshrift `.sst`.

## Honesty note

The JVM-family and `.xclass` ingests are **structural** — they preserve a class's
structure and type contracts, not its executable logic (method bodies are
synthesized skeletons). This registry lists what SLeeLa *accepts*; consult each
component's spec ([`impl/langin/LANGIN.model`](impl/langin/LANGIN.model),
[`impl/xclass/XCLASS.model`](impl/xclass/XCLASS.model),
[`SST.model`](SST.model)) for the exact mapping and its limits.
