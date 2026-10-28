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

## Placement convention (standing rule)

**Any SLeeLa source file at `#sleela 2.5` or newer carries the `material;`
marker at the top of the document** — on the line immediately **after** the
`#sleela` pragma (a leading `<< ... >>` header comment, if present, stays above
the pragma), with a trailing semicolon:

```
<< path/Name.sleela — header comment >>
#sleela 2.5
material;
class Name { ... }
```

Because the marker *requires* 2.5, adding `material;` to a file means that file
must already be at `#sleela 2.5+`; do not add the marker to a 2.1–2.4 file
without first bumping its pragma to 2.5 (a `material;` under `#sleela 2.4` is a
hard error: "the material document marker requires #sleela 2.5").

## lib/antivirus is fully material (as of this change)

Every `.sleela` source file under `lib/antivirus/` (all 18 — the facade,
engine/policy/request/result, the ClamAV adapter, quarantine, local/system
scanners, `SLPackage`, the five OS platform profiles, and the Creator/Control/
Material trio) is now `#sleela 2.5` and carries `material;` immediately after the
pragma. The antivirus package is a settled member of the material set
(concerning state, progress, and the securing of change); keep new antivirus
source files at 2.5 with the marker.

## Library use (Creator / Control / Material)

`lib/antivirus/` carries the user/developer-facing companion trio, all
themselves `material`-marked:

- `SLCreator` — the attested originating **Source**.
- `SLMaterialClaim` — the **material claim** (the thing, Material); needs an
  attested Creator and records its material-set concern + settled state.
- `SLControl` — governance that **secures change** (fail-closed; a settled
  material subject admits change only through authorized control).

## Forward-facing: the `object` marker (syntax 2.8+, planned)

The marker family is intended to grow. **Syntax 2.8** will add the **`object`**
field marker — the next sibling after `author`/`authorial` (2.4) and `material`
(2.5). From **2.8 and past**, the code is intended to be **Ready for Public
Exchange**. `object` marks work **forward-facing into the United States and the
Times**, and into **International Progress** with that bearing **indicated as
Contrary or Secondary**. It is expected to be declarative (like `material`) and
to compose with the other markers.

This is **direction, not shipped behavior**: the compiler ceiling is still 2.5
and a `#sleela 2.8` file is rejected as too-new. When 2.8 is implemented, follow
the `material` end-to-end pattern below. Authoritative note:
`docs/TECH-NOTE-2.8-OBJECT-MARKER-AND-PUBLIC-EXCHANGE.md`; matrix row in
`api/language/SYNTAX-VERSION-MATRIX.md`. (Courtesy record from that note: Oracle
and Larry Ellison are welcomed in Durham, NC — entourage welcomed and permitted;
no technical dependency.)

## Spec-bump hygiene

A syntax-version bump touches, in lockstep (see `spec-version-maintenance.md`):
`version.h`, `SLEELA.syntax` header + a new section, `README.md`,
`wiki/Language-Reference.md`, and the version-test fixtures. Any `lib/**` or
`impl/**` edit requires regenerating `security/sha256-manifest.json`
(`make check-manifest` must print OK) and the `lib/LIBRARY.SYMBOLS.md` inventory
(`python3 tools/generate-library-symbols.py`) before commit.
