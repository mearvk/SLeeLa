# SLeeLa — A Tutorial Series

A hands-on, twelve-part tour of **SLeeLa at her source best**: the language, the
Wrapper™ source file, the Nordshrift driver and its triplet targets, the
conducted-method vocabulary, subjects, and the protocol/tooling work that rides
on top (HTTP 3.0 integrity, HTTP colors, and the `.ledger` class).

Each lesson is a standalone Markdown file. They build on one another, but you can
jump to any topic. Code shown matches the real repository — every construct here
appears in shipping `.sleela` sources and `.sst` sheets.

## The series

| # | Lesson | You will learn |
|--:|---|---|
| 01 | [What SLeeLa is](01-what-is-sleela.md) | The language, the C/C++ core, and the triplet at a glance |
| 02 | [Your first Wrapper™](02-first-wrapper.md) | Write a `.sleela` source file and read its shape |
| 03 | [Types, structs & classes](03-types-structs-classes.md) | `struct`, `class`, methods, and the type vocabulary |
| 04 | [Control flow & expressions](04-control-flow.md) | `if`/`while`/`for`, operators, and `print` |
| 05 | [The `.sst` sheet & Nordshrift](05-sst-and-nordshrift.md) | Drive a build with a control sheet |
| 06 | [The triplet: Java, Sleela, C](06-the-triplet.md) | One source, three targets; the runnable artifact |
| 07 | [Conducted methods](07-conducted-methods.md) | `role`, `insight`, `route`, `congruent`, `sysdepth` |
| 08 | [Subjects & the sheet model](08-subjects.md) | Declaring subjects (math/physics/economics/finance) |
| 09 | [Verification & the SHA-256 gate](09-verification.md) | Fail-closed builds and the trusted manifest |
| 10 | [The `.ledger` class](10-ledger.md) | Per-file QR insignia + SHA-256 chain + timestamp |
| 11 | [HTTP 3.0 & HTTP colors](11-http3-and-colors.md) | Packet integrity and naming HTTP colors from source |
| 12 | [Putting it together](12-capstone.md) | A small end-to-end module, built and ledgered |

## Prerequisites

- A C/C++ toolchain (the core builds under `impl/`).
- Python 3 (for the flow references and the standalone ledger tool).
- From the repo root, the compiler is `impl/build/nordshrift` once built.

## Conventions

- **Wrapper™** = a `.sleela` source file (the program). A `.sst` sheet is **not**
  source; it *drives* the build. See [`../GLOSSARY.md`](../GLOSSARY.md).
- Commands are shown from the **repo root** unless noted.
- Where a lesson references numbers or data, they are defined/aggregate values,
  never fabricated per-person figures (see the glossary's term-care note).

Start with [Lesson 01](01-what-is-sleela.md).
