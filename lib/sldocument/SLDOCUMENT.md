<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

# The `.sldocument` Format — Ordered, Top-Down SLeeLa Documents

**Family:** `lib/sldocument`
**Format extension:** `.sldocument`
**Format version:** `1.0`
**Compiles with:** standard SLeeLa source (`.sleela`) via the SLeeLa Compiler
**Native bridge:** `native/include/sleela_sldocument.h`, `native/src/sleela_sldocument.cpp`

An `.sldocument` is an **ordered document**: its code runs **top-down**, step by
step, and it is compiled **against and with standard SLeeLa source**. Each step
is a method, and that method generally returns **a single binary item** that is
"**veritable and kind**" — a verified truth of the right kind.

These documents suit tasks more sophisticated than scripting in bash, and clear
**national-program** work where the order is already established, so the author
moves in assumed flow and architecture rather than writing control plumbing.

## Where it sits among SLeeLa source forms

| Form | Extension | Shape | Use |
|---|---|---|---|
| SLeeLa program | `.sleela` | classes of fields and methods | the general language |
| Sleela Script | `.sleela-script` | dynamic procedural script | short automation, glue |
| **SLeeLa Document** | **`.sldocument`** | **ordered top-down steps** | **settled, ordered flows** |

A document is not a replacement for `.sleela`; it is a *conductor* over compiled
SLeeLa methods, giving an established order a first-class, readable home.

## Document structure

```
#sldocument 1.0
document "title"
with source "companion/One.sleela"
with source "companion/Two.sleela"

// steps, top-down
first_step()

@function("role")
second_step()

@order(3)
third_step()

final_step()
```

- `#sldocument 1.0` — the format version line (required, first line).
- `document "title"` — the document's name.
- `with source "..."` — a companion `.sleela` source compiled alongside, so step
  methods resolve to ordinary compiled SLeeLa.
- the remainder is the **ordered step list**.

## Steps, naming, and annotations

Each step is a method invocation. How a step is named and ordered follows one of
three annotation forms (`SLDocumentAnnotation`):

| Form | Meaning |
|---|---|
| *bare method name* | the method name **is** the step name; its order is its **textual (top-down) position** |
| `@order(n)` | an **explicit** 1-based position; governs over textual order |
| `@function("role")` | annotates the step's **declared role/function**; does **not** change order |

When `@order` is present it governs; otherwise textual order governs. A clean
document resolves to a settled `1..N` sequence (the compiler validates there are
no gaps or duplicate `@order` values).

## The single binary return — veritable and kind

Each step usually returns one binary item, modeled by `SLVeritable`:

- **veritable** — the binary truth: did the step hold? (`bit()` is `1` or `0`)
- **kind** — is the result well-formed and benign (the right *kind* of value)?

A value is **sound** only when it is **veritable AND kind**. A step that returns
a true-but-ill-formed result is *veritable but not kind*, and the document does
not treat it as a clean pass. Logical composition (`and`, `or`) propagates
kindness, so a fault never hides behind a true bit.

## Running a document

`SLDocument.run()` runs top-down:

1. steps are **arranged by effective order** (explicit `@order`, else top-down);
2. each step is **invoked in order**, and its single binary veritable value is
   recorded in `SLDocumentResult`;
3. by default the document **stops at the first unsound step** (fail-closed) —
   an ordered program should not proceed past a broken step. `permitContinue()`
   relaxes this to record every step.

`SLDocumentResult.finalBit()` is the document's final binary item (the last
step's bit, which in an established order is the final result of the flow), and
`sound()` holds only when every recorded step was sound. `firstFault()` points
to exactly where an ordered national program broke.

## Compiling with standard SLeeLa source

`SLDocumentCompiler` drives the normal SLeeLa Compiler path with document
semantics:

1. register companion `.sleela` sources (`withSource`) so step symbols resolve;
2. lower each annotated step to a VM-invocable method entry;
3. `validateOrder()` — confirm a clean `1..N` sequence (no gaps/duplicates);
4. `compile()` yields a VM frame handle the document runs against;
5. `compileAndRun()` compiles and runs in one call.

The authoritative compilation remains the SLeeLa Compiler (`lib/compiler`,
`impl/frontend`); this class is the document-aware front end to it.

## Classes

| Class | Role |
|---|---|
| `SLDocument` | the ordered document; runs steps top-down |
| `SLDocumentStep` | one annotated/ordered step method |
| `SLDocumentAnnotation` | the `@order` / `@function` / bare-method vocabulary |
| `SLVeritable` | the single binary veritable-and-kind return value |
| `SLDocumentResult` | the ordered collection of step values |
| `SLDocumentCompiler` | compiles the document with standard SLeeLa source |

## Example

See [`examples/national-ledger.sldocument`](examples/national-ledger.sldocument):
a settled four-step national-ledger close (verify → reconcile → post to the
national register → finalize), compiled with its companion `.sleela` sources.

## Native bridge

- `sleela_sldocument_compile(title, version, companions)` — compile with companion sources; returns a frame handle.
- `sleela_sldocument_invoke(frame, method, order)` — invoke one step; returns the single binary veritable item (1/0).
- `sleela_sldocument_kind(frame, method)` — report whether the step's value is kind (well-formed/benign).

```sh
cd native
make test   # builds + self-tests audio, opcode, governance, and sldocument bridges
```

**Max Rupplin — MEARVK LLC — 2026**
