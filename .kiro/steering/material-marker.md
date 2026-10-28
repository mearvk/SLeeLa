---
inclusion: always
---

# The `material` document marker (syntax 2.5)

Syntax **2.5** adds a bare, contextual, **declarative-only** document marker
**`material`** — a sibling to the 2.4 `author`/`authorial` marker. It is the
current spec ceiling (`maxSupportedSyntax()` = `2.5`). The normative definition
is [`SLEELA.syntax` §19](../../SLEELA.syntax).

## What it means

`material;` (optional trailing `;`), declared at document scope after the
`#sleela` pragma and outside any class, claims the document is **material**:
already well considered, **present to its author as its own Source** — the thing,
**Material**. It marks membership in the **set of material documents**: those
concerning **state**, **progress**, or the **securing of change**. That set is,
by convention, **not freely changed out of** — a material document, once
settled, is edited deliberately, as settled Source.

## Fixed points

- **Declarative only.** It has **no** effect on parsing, typing, dispatch, or
  codegen. It emits no artifact and reserves no identifier.
- **Contextual, not reserved.** `material` stays an ordinary identifier anywhere
  except the top-level marker position, and in any document below 2.5.
- **Orthogonal to `author`.** A document may be both `author`/`authorial` and
  `material`; the two standings compose.
- **Version gate:** the marker requires `#sleela 2.5` ("the material document
  marker requires #sleela 2.5" below that).

## Implementation (end-to-end, mirrors the `author` pattern)

- `impl/frontend/version.h` — `maxSupportedSyntax()` = `{2,5}`.
- `impl/frontend/ast.h` — `Program.materialMarked`.
- `impl/frontend/parser.cpp` — `parseProgram()` recognises the contextual
  top-level `material` word and sets the flag.
- `impl/frontend/semantic.cpp` — `collect()` gates it to 2.5; no other effect.
- `impl/tests/version/` — fixtures `material_2_5.sleela` (accepted) and
  `material_too_early.sleela` (rejected at 2.4); `minor_ahead.sleela` is now
  `#sleela 2.6` (one above the ceiling).

## Library use (Creator / Control / Material)

`lib/antivirus/` carries the user/developer-facing companion trio, all
themselves `material`-marked:

- `SLCreator` — the attested originating **Source**.
- `SLMaterialClaim` — the **material claim** (the thing, Material); needs an
  attested Creator and records its material-set concern + settled state.
- `SLControl` — governance that **secures change** (fail-closed; a settled
  material subject admits change only through authorized control).

## Spec-bump hygiene

A syntax-version bump touches, in lockstep (see `spec-version-maintenance.md`):
`version.h`, `SLEELA.syntax` header + a new section, `README.md`,
`wiki/Language-Reference.md`, and the version-test fixtures. Any `lib/**` or
`impl/**` edit requires regenerating `security/sha256-manifest.json`
(`make check-manifest` must print OK) and the `lib/LIBRARY.SYMBOLS.md` inventory
(`python3 tools/generate-library-symbols.py`) before commit.
