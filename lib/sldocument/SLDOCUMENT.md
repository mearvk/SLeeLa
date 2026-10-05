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

## A compile choice from the SLeeLa compiler

`.sldocument` is a selectable **compile choice**, not a separate toolchain. The
compiler family exposes it through two classes in `lib/compiler`:

- **`SLSourceForm`** — names each SLeeLa source form and its extension:
  `SLEELA` (`.sleela`), `SLDOCUMENT` (`.sldocument`), `SLSCRIPT`
  (`.sleela-script`). It can infer the form from a file name (the longer
  `.sldocument` extension is matched before `.sleela`).
- **`SLCompileChoice`** — records which form to compile, either explicitly
  (`choose(path, SLSourceForm.SLDOCUMENT)`) or by inferring it from the path
  (`chooseFromPath`). `compile()` then routes a document through the
  document-aware path and a program through the ordinary `.sleela` path. The
  choice is fail-closed: an unrecognised extension is surfaced as a diagnostic,
  never guessed.

This slots into the compiler selection model
(`language → family → version → producer → source form → IR/object → target`):
`.sldocument` is one of the recognised **source forms**. The extension is also
registered in `lib/compiler/LANGUAGE.FORMAT.REFERENCE.md`.

## Naming conventions — comparing `.sleela` and `.sldocument`

The two forms name things differently: a `.sleela` program names **every**
method, while a `.sldocument` **may leave steps anonymous** (identified only by
order or `@function` role). That is fine while a document runs — but if an
engineer converts a `.sldocument` to a `.sleela` **for safekeeping**, every step
must become a named method.

- **`SLDocumentNaming`** is the convention authority. It gives each step a stable,
  legal `.sleela` method name, deterministically:
  1. keep an explicit method name (sanitized to a legal identifier);
  2. else derive from a `@function` role (`"reconcile accounts"` →
     `reconcileAccounts`);
  3. else synthesize from the ordered position (`@order(3)` or textual position
     3 → `step003`), which is stable and collision-resistant.
- **`SLSourceNameComparison`** pairs each document step with the `.sleela` method
  name it *would* have, flagging which names had to be **synthesized** and
  whether any two steps **collide**. `convertible()` is true only when there are
  no collisions.
- **`SLDocumentConverter`** performs the conversion: it `plan()`s the names,
  refuses to proceed on a collision (fail-closed), and emits a `.sleela` class
  whose methods are the named steps plus a `run()` that calls them in the same
  top-down order — preserving the single-binary / veritable-and-kind contract.
  `convertToFile(doc, "Kept.sleela")` writes the kept program.

So an anonymous ordered document round-trips into a fully named program for
archival, review, or further hand-editing, without losing its established order.

## Classes

| Class | Role |
|---|---|
| `SLDocument` | the ordered document; runs steps top-down |
| `SLDocumentStep` | one annotated/ordered step method |
| `SLDocumentAnnotation` | the `@order` / `@function` / bare-method vocabulary |
| `SLVeritable` | the single binary veritable-and-kind return value |
| `SLDocumentResult` | the ordered collection of step values |
| `SLDocumentCompiler` | compiles the document with standard SLeeLa source |
| `SLDocumentNaming` | names anonymous steps for `.sleela` conversion |
| `SLSourceNameComparison` | compares `.sleela` vs `.sldocument` step naming |
| `SLDocumentConverter` | converts a `.sldocument` to a `.sleela` for safekeeping |
| `SLSourceForm` *(lib/compiler)* | the source-form / extension catalogue |
| `SLCompileChoice` *(lib/compiler)* | selects `.sldocument` vs `.sleela` to compile |

## Example

See [`examples/national-ledger.sldocument`](examples/national-ledger.sldocument):
a settled four-step national-ledger close (verify → reconcile → post to the
national register → finalize), compiled with its companion `.sleela` sources.

## Native bridge

- `sleela_sldocument_compile(title, version, companions)` — compile with companion sources; returns a frame handle.
- `sleela_sldocument_invoke(frame, method, order)` — invoke one step; returns the single binary veritable item (1/0).
- `sleela_sldocument_kind(frame, method)` — report whether the step's value is kind (well-formed/benign).
- `sleela_compile_choice(path, form)` — compile a chosen source form (`.sldocument` or `.sleela`); returns a frame handle.
- `sleela_sldocument_synth_name(prefix, order, …)` — synthesize a stable name (`step003`) for an anonymous step.
- `sleela_sldocument_sanitize_identifier(raw, …)` — make a string a legal SLeeLa identifier.
- `sleela_sldocument_camel_from_role(role, …)` — role → camelCase (`reconcileAccounts`).
- `sleela_sldocument_emit_sleela(class, title, steps, …)` — emit the `.sleela` class for a converted document.
- `sleela_sldocument_write(path, source)` — write the converted `.sleela` for safekeeping.

```sh
cd native
make test   # builds + self-tests audio, opcode, governance, and sldocument bridges
```

**Max Rupplin — MEARVK LLC — 2026**
