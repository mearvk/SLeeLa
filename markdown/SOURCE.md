# SOURCE.md — Sleela source file characteristics

The Sleela **source file** is the `.sleela` file, named **Wrapper™**. It is the
*program*: the human-authored unit the toolchain compiles and runs. A `.sst`
sheet is **not** source — it is a control surface (see
[`NORDSHRIFT.md`](NORDSHRIFT.md)). For the filetype's fuller treatment and its
distribution marks, see [`SLEELA.md`](SLEELA.md); for the compiler, see
[`COMPILER.md`](COMPILER.md).

## Characteristics

| Property        | Value |
|-----------------|-------|
| Filetype name   | **Wrapper™** |
| Extension       | `.sleela` |
| Role            | Program source (the "program") |
| Encoding        | UTF-8 |
| Governed by     | SL-META-0001, the Sleela Language Metadocument (the *metadocument addend*) |
| Surface         | Java-like: one or more `class`es; execution enters at `main()` |
| Syntax version  | Declared per file via the `#sleela MAJOR.MINOR` pragma (§ below) |
| Compiled to     | Sleela Core bytecode, then run on the C/C++ core via `slcore_exchange` |

## Structure

A Wrapper™ is one or more classes; `main()` is the entry point. The accepted
surface (authoritative list in `impl/README.md`):

- typed locals — `int`, `double`, `boolean`, `String`, `void`;
- methods with recursion; class **fields** (shared state);
- control flow — `if`/`else`, `while`, `for`;
- the operator set — `+ - * / %`, comparisons, `&& || !`, unary `-`;
- Java-style string concatenation with `+`; `print(...)`;
- a bounded threading model — `spawn`/`join`/`lock`/`unlock`/`send`/`recv`;
- **conducted methods** backed by `SHEET.sheet` —
  `conduct`/`role`/`insight`/`congruent`/`route`/`sysdepth`/`degreemax`.

## Unsigned integer type signatures (U1–U1048576)

SLeeLa source signatures define a parameterized unsigned, fixed-width integer family:

- **Type form:** `U<n>`, where `n` is a decimal integer in the inclusive range `1..1048576`.
- **Meaning:** `U<n>` stores exactly `n` value bits and has range `0..(2^n - 1)`.
- **Examples:** `U1`, `U8`, `U16`, `U32`, `U64`, and `U1048576`.
- **Signature use:** types may be written in field, local-variable, parameter, and return-type positions wherever the compiler supports this type family.
- **No signedness ambiguity:** unsigned values cannot represent negative numbers; conversions that narrow a value must be explicit or rejected when out of range.
- **Implementation requirement:** parsers and type-checkers must validate the width without overflowing host integers. Runtime storage, arithmetic, serialization, and ABI lowering must support arbitrary bit widths or report a specific unsupported-operation diagnostic; they must not silently substitute a host `int`/64-bit type.

The family is parameterized, not a requirement to create 1,048,576 separate hand-written declarations. `U8` and `U16` are ordinary instances of the same width-checked type constructor. This specification records the intended source signature; runtime/compiler conformance must be demonstrated by tests before claiming all widths execute end-to-end.

## Character-set capacity signatures (C1–C1048576)

SLeeLa also defines a parameterized **character-set capacity signature** family, parallel in notation to the `U<n>` unsigned-integer family but with a different meaning:

- **Type/signature form:** `C<n>`, where `n` is a decimal integer in the inclusive range `1..1048576`.
- **Meaning:** `C<n>` describes a character-set/catalogue capable of addressing up to `n` distinct character entries. It is a capacity declaration, **not** an integer with `n` bits and not a promise that all entries have assigned meanings.
- **Default example:** `C1048576` means a target capacity of `1024 * 1024 = 1,048,576` distinct character entries.
- **UTF-4088 example:** the experimental `utf-4088/` catalogue can be described as a character-set family whose intended capacity is declared separately from its currently generated 16,606-entry front-end registry. The current UTF-4088 implementation uses a 64-bit container and an experimental 33-bit identifier interval; neither fact means its present registry contains 1,048,576 semantically defined characters.
- **Identity and encoding:** a character-set entry has a stable catalogue ID and may map to a code point, glyph/bitmap, optional semantic annotation, and provenance record. IDs, Unicode scalar values, glyphs, and meanings are distinct fields and must not be conflated.
- **Capacity vs. population:** `C<n>` sets an upper capacity, not the number of populated, validated, or culturally attested entries. A registry must report its actual population separately.
- **Validation:** zero, leading-zero forms, non-decimal values, and values above `1048576` are invalid. Any registry overflow must be rejected or handled by an explicit extension/version mechanism; entries must never silently collide or be truncated.
- **Implementation status:** this catalogue entry defines the source-level signature contract. Full compiler type-checking, runtime representation, serialization, and UTF-4088 registry binding must each be covered by implementation tests before being described as end-to-end supported.

Examples: `C8`, `C256`, `C65536`, and `C1048576`. The number is a count of addressable character entries, unlike `U8` or `U64`, where the number is a bit width.

## The version pragma

Per SL-META-0001 §4.4, a Wrapper™ declares its **syntax version** on the first
non-blank, non-comment line:

```java
#sleela 1.0
class Hello {
    void main() { print("hello from a Wrapper\u2122"); }
}
```

Form is `#sleela MAJOR.MINOR` (a trailing `.PATCH` is tolerated and ignored).
The compiler is version aware: it accepts declared syntax versions from `1.3`
through `1.7`, warns and assumes the floor when the pragma is absent, and rejects
a version outside that range or a malformed pragma. Syntax version `1.7` adds
the `U1` through `U1048576` arbitrary-width unsigned integer type signatures. See
[`COMPILER.md`](COMPILER.md) §3.

## Lifecycle

```
Wrapper™ (.sleela)  ──►  Sleela front end (version check → lex → parse → compile)
                          ──►  Sleela Core bytecode  ──►  run on the C/C++ core
```

Two entry points consume the same source through the same front end:

- **Direct** — `sleela run file.sleela` compiles and runs one Wrapper™.
- **Via Nordshrift** — a `.sst` sheet names a set of Wrapper™ files with a
  `source:` glob (e.g. `**/*.sleela`) and transpiles them to the triplet target
  (Java, Sleela, or C). See [`NORDSHRIFT.md`](NORDSHRIFT.md).

## Not source

To keep the vocabulary precise, these are **not** Sleela source files:

| File        | What it is                                             |
|-------------|--------------------------------------------------------|
| `.sst`      | Nordshrift control sheet (NS-SST-0001) — names sources |
| `.sheet`    | `SHEET.sheet` object catalog                           |
| `.manifest` | SL-META-0001 metadocument text                         |
| `.xclass`   | SecureJDK 28 ingest input, reconstructed into a program |

## Unsigned integer implementation status

The C++ arbitrary-width value foundation is in `impl/frontend/unsigned_integer.h`, with an independent smoke test under `impl/tests/unsigned_integer_test.cpp` and the `test-unsigned-integer` Makefile target. It stores normalized little-endian bytes for widths 1..1048576 and supports decimal parsing/formatting, width-checked byte serialization, same-width comparison, checked addition/subtraction, and overflow detection. The type checker recognizes the `U<n>` type-name family. This is an implementation phase, not yet a claim that all operators are lowered through the VM's stable runtime value ABI.
