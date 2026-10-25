# Author field + `∴` branders + the originated Source Document (syntax 2.4) — DESIGN

**Status:** DESIGN / proposal for sign-off. Not yet implemented; the compiler
is unchanged and `maxSupportedSyntax()` is still `2.3`. This document records
the agreed design so the lexer → parser → semantic analyzer → emitter work can
proceed in slices against a fixed target.

**Author of record:** feature requested by the repository owner; drafted by the
Kiro agent. Points marked **[DECISION — confirm]** are reasoned defaults chosen
where the brief left latitude; correct any and the implementation follows.

---

## 1. Summary

Syntax 2.4 introduces three linked things:

1. **The `author` field** (synonym: `authorial`) — a single, modifier-less word
   that marks a *document* as **handled by professional council**. It behaves
   like a static class belonging to the document itself, not a typed instance
   field.
2. **Legal duplicate members.** When a document declares the `author` field, it
   may legally declare **duplicate fields and methods** (same name). Identity is
   preserved by giving every member a **unique id and hash**.
3. **Two compile outputs.** The standard compile process emits (a) the original
   compiled artifact, exactly as today, and (b) a new **Source Document** — a
   regenerated `.sleela` source in which every field and method is *located and
   originated* and carries a **`∴` brander** (the house analogue of Java's `@`
   annotation). Together these are "clean and united": the artifact to run, and
   the branded source to audit.

The `∴` (U+2234 THEREFORE) glyph is the brander marker, replacing Java's `@`.

---

## 2. The `author` field

### 2.1 Form

A single bare word at document (top) scope, outside any class body:

    author;

`authorial;` is **exactly equivalent** — the two words are synonyms and either
marks the document identically. It takes **no modifier and no type**. It is read
as a *static class unto the document itself*: a document-level flag, not a
member of any class.

**[DECISION — confirm] Placement.** The marker is a top-level statement that may
appear once, after the `#sleela` version line and before (or among) the class
declarations. A second occurrence is ignored with a warning rather than an
error. (Alternative you might prefer: allow it only as the first statement after
the version line.)

### 2.2 Meaning

Presence of `author` / `authorial` means **the document is handled by
professional council**. Its single operational effect in 2.4 is to **enable the
duplicate-member allowance** (§3) and to request the **originated Source
Document** (§5) for that document. A document without the marker keeps today's
rule: duplicate members remain a semantic error (exactly the `notEmpty()`
situation already fixed in `lib/core/SLString.sleela`).

### 2.3 Version gate

The marker requires `#sleela 2.4`; a source below 2.4 using `author`/`authorial`
is rejected (`author field requires syntax 2.4`). Below 2.4 the words are not
reserved and remain ordinary identifiers.

---

## 3. Legal duplicate members and member identity

### 3.1 The allowance

In an `author` document, two or more fields, or two or more methods, may share a
name. The semantic analyzer's current "duplicate member" rejection is **relaxed
only for author documents**; every other document is unchanged.

### 3.2 The member id

Each field and each method receives a **unique id** that is stable for a given
source + edit date. The id is a per-document counter assigned in **source
declaration order** starting at 1 (1, 2, 3, …). It is what the Source Document
and the ordering map (§4) refer to.

### 3.3 The member hash — the brander value

The hash **is** the brander. It is computed over exactly these inputs, in this
order:

1. the **document name** (the source file's base name, e.g. `SLString.sleela`),
2. the **method name** (member name),
3. the **`int` parameters** of the member,
4. — the parameter set is restricted to **`String` and `int` only**; any member
   whose signature uses another parameter type is **not eligible** for the
   duplicate allowance and is branded with its declaration-order id but a hash
   marked `n/a` (**[DECISION — confirm]**: reject instead?),
5. the **date the document was edited** (`YYYY-MM-DD`, the compile-time edit
   date of the source).

**[DECISION — confirm] Exact preimage.** The hashed byte string is the fields
above joined with a single `|` separator, parameters rendered as an ordered,
comma-separated type list using the literal tokens `int`/`String`:

    <docName>|<memberName>|<paramTypeList>|<editDate>

Example preimage for `boolean compare(String a, int n)` edited 2026-10-25 in
`SLString.sleela`:

    SLString.sleela|compare|String,int|2026-10-25

### 3.4 The hash algorithm

**[DECISION — confirm] Algorithm.** The brief offers "SHA or MGEM, a new
standard." Default: use **SHA-256** (already vendored and used by the
integrity-manifest toolchain — no new dependency, auditable, collision-safe),
emitting the lowercase hex digest, optionally truncated to the first 16 hex
chars in the brander for readability with the full digest retained in the map.

**MGEM** is not yet a defined algorithm in this repository. If MGEM is the
intended standard, it must be specified (inputs, block/word size, mixing
function, output width, test vectors) before it can be implemented; until then
SHA-256 is the concrete default. This document does **not** invent an algorithm.

---

## 4. The ordering map (duplicate dispatch)

When a name has several definitions, the VM needs a deterministic order in which
to reach them. The brief fixes the shape — "an array list or hashmap for the
ordering of the numbers that direct to the functions," with a
**frequency-of-use adjustment** that lets the VM pick slot N+1, N+2, … when a
slot is "busy by use or ordering," and permits **direct call of a method when
the document supposes the author field**.

### 4.1 Structure

Per duplicated name the compiler emits an **ordering table**: an ordered list of
member ids (the slots) plus a small **use-count** per slot the VM maintains at
run time.

    order["compare"] = [ id=2, id=5, id=1 ]   // slots 1..k, each -> a member id
    uses["compare"]  = [  0,     0,    0   ]   // frequency counters

### 4.2 Selection (open-addressing probe with a frequency tilt)

**[DECISION — confirm] This is the one rule the brief described only by
examples, which were not reproducible as a closed formula. The chosen, fully
specified rule is below; it realises "if the first is busy, use the next" and
the "frequency adjustment" directly.**

To dispatch a bare call to a duplicated name:

1. Start at slot 1 of the name's ordering table.
2. If that slot is **not busy**, select it; otherwise **probe forward**
   (slot → slot+1, wrapping to 1 after the last) until a non-busy slot is found.
3. A slot is **busy** if it is currently in use on the active call path
   (re-entrancy guard) **or** its use-count exceeds the running minimum by the
   frequency band B (**[DECISION — confirm] B = 1**), which spreads load across
   duplicates (the N+1 / N+2 behaviour).
4. On selection, increment that slot's use-count.

This is deterministic given the table and the live use-counts, terminates (the
table is finite and at least one slot is always eligible), and reduces to
"call the single definition" when a name has exactly one.

> Note on the example sequences (`3→1,2,3`, `6→1,3,2,6,1`, `5→1,4,2,1`): these
> could not be derived as a function of N (they vary in length and repeat/skip
> slots), so they are read as **illustrations of a live probe trace**, not as a
> table-construction formula. If instead they are meant to be the literal
> initial table contents for those arities, provide the rule that generates
> them and this section will adopt it verbatim.

### 4.3 Direct call

When the document supposes the `author` field, a caller **may** address a
specific definition directly by its id/hash brander (bypassing the probe),
e.g. a call annotated with the member's `∴` id. The bare-name call uses the
probe in §4.2. (Direct-call surface syntax is **deferred** — "suppose we will
allow it at some point"; the id/hash are reserved now so it is additive later.)

---

## 5. The originated Source Document (second output)

### 5.1 What it is

Alongside the normal compiled artifact, the compiler regenerates the input as a
**Source Document**: valid `.sleela` source, semantically identical, in which
**every field and method is preceded by a `∴` brander** recording where it is
located and where it originated. This is the auditable twin of the artifact.

### 5.2 Brander line form

**[DECISION — confirm] Shape.** One `∴` line immediately above each member:

    ∴ origin(id=2, hash=9f3a1c…, doc="SLString.sleela", member="compare", sig="(String,int)", edited="2026-10-25")
    boolean compare(String a, int n) { ... }

- `∴` is the marker (replaces `@`). `origin(...)` is the brander keyword and its
  named arguments. Branders are comments-with-structure: they must lex as
  angle-bracket-safe trivia so the regenerated document still compiles (so the
  emitter wraps each `∴` line appropriately for the lexer — see §6).

### 5.3 Location

Produced as part of the **standard compile process** (not an extra command),
written to the **directory of the original source**, or to the directory given
by a **compile-time flag** (**[DECISION — confirm]** flag name:
`--originate-dir=<dir>`; default = source directory). **[DECISION — confirm]**
naming: `<original>.originated.sleela` so it never clobbers the input.

---

## 6. Implementation plan (slices; each validated with `impl/build/sleela`)

The feature spans the front end and a new emitter. Proposed order, each a
self-contained, buildable step gated behind `#sleela 2.4`:

1. **Spec + version gate.** Add `SLEELA.syntax` §18 (promoting this doc),
   bump `maxSupportedSyntax()` to `{2,4}`, update the version-banner/doc touch
   points per `.kiro/steering/spec-version-maintenance.md`, add version-test
   fixtures.
2. **Lexer/parser.** Recognise `author`/`authorial` as a 2.4 top-level marker;
   carry a document flag on the AST.
3. **Semantic analyzer.** When the flag is set, relax the duplicate-member
   rejection; assign each member a declaration-order id; restrict the duplicate
   allowance to `String`/`int` signatures.
4. **Hash + ordering map.** Compute the SHA-256 brander per §3.3–3.4; build the
   per-name ordering table per §4; thread a use-count into the VM dispatch.
5. **Originated emitter.** Regenerate the branded Source Document per §5, honour
   `--originate-dir`, keep output valid/compilable.
6. **Dispatch.** Wire the probe (§4.2) into the call path; reserve the
   id/hash direct-call brander.

Every `lib/**` or `impl/**` edit trips the SHA-256 manifest gate: regenerate
`security/sha256-manifest.json` and confirm `make check-manifest` before commit
(see `.kiro/steering/comment-style.md` and `sleela-authoring-gotchas.md`).

---

## 7. Open decisions (please confirm or correct)

- **[A]** Hash algorithm: SHA-256 default, or define MGEM (needs a full spec).
- **[B]** Ordering table §4.2: adopt the specified probe + frequency band B=1,
  or supply the generating rule behind the `1,3,2,6,1` / `1,4,2,1` examples.
- **[C]** Exact hash preimage string and separator (§3.3).
- **[D]** Non-`String`/`int` parameter members: brand-with-`n/a` vs reject
  (§3.3).
- **[E]** Brander line keyword/shape and how it stays compilable (§5.2, §6).
- **[F]** Originated file name + flag name/default (§5.3).
- **[G]** `author` placement rule and duplicate-marker handling (§2.1).
