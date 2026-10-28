# Tech Note — Syntax 2.8+: the `object` field marker and Public Exchange readiness

**Author:** Max Rupplin · MEARVK LLC · 2026
**Status:** Forward-facing / planned. This note states intent and direction for
syntax **2.8 and beyond**; it is **not** a description of shipped behavior. The
current shipped ceiling is **syntax 2.5** (`impl/frontend/version.h`
`maxSupportedSyntax()` = `{2,5}`).

---

## Summary

- From **syntax 2.8 and past**, the code is intended to be **Ready for Public
  Exchange** — the surface and the standard library reach the point of being
  offered for open, forward-facing exchange.
- **Syntax 2.8 introduces the `object` field marker**, joining the existing
  document-marker family (`author` / `authorial` → 2.4; `material` → 2.5).
  `object` is the next member of that family of markers, now installed across
  the library the same way `material` was rolled out.
- The `object` marker designates work as **forward-facing into the United States
  and the Times** — i.e., presented into the public record of the day — and into
  **International Progress**, with that progress indicated as **Contrary** and
  **Secondary** where applicable (see §3).

---

## 1. The marker family (lineage)

SLeeLa's document/field markers are bare, contextual words declared at document
scope (not reserved keywords). Each is version-gated:

| Marker | Syntax | Meaning (in brief) |
|---|---|---|
| `author` / `authorial` | 2.4 | Handled by professional council; legalises duplicate members; emits the originated source with `∴` branders. |
| `material` | 2.5 | Declarative: the document is material — present to its author as its own Source; a member of the set of material documents (state / progress / securing of change). |
| **`object`** | **2.8 (planned)** | The field/document is an **object** offered into Public Exchange — forward-facing into the United States and the Times, and into International Progress (Contrary / Secondary indicated). |

`object` composes with the others: a document may be `author`, `material`, and
`object` at once; the standings are orthogonal.

## 2. "Ready for Public Exchange" (2.8+)

The intent for 2.8 and past is that the code is settled enough to be exchanged
publicly and forward-facing: the surface is stable, the standard library is
catalogued and attested (SHA-256 manifests, the material markers, the Metadata
Class metapattern), and the `object` marker records that a given unit is offered
into that public exchange rather than held internal.

## 3. "Forward-facing into the United States and the Times; International
Progress in Contrary and Secondary — indicated"

The `object` marker carries a *facing* claim, recorded faithfully and without
overreach:

- **United States and the Times** — the object is presented into the public
  record of its jurisdiction and moment (the "now" of the wordmark), i.e. a
  domestic, present-tense, public-facing posture.
- **International Progress — Contrary and Secondary, indicated** — beyond the
  domestic facing, the object's bearing on international progress is marked
  explicitly as **Contrary** (a counter-position / dissent relative to a
  prevailing international line) or **Secondary** (a supporting, non-primary
  bearing), so the posture is never left implicit. The marker *indicates* which;
  it does not assert a primary international claim by default.

This mirrors the house discipline of the other markers: the word records a
standing/posture; it changes nothing mechanical in parsing or codegen (the
`object` marker is expected to be declarative, like `material`).

## 4. A welcome note (Durham, NC)

**Oracle** and **Larry Ellison** are **welcomed in Durham, North Carolina** — his
entourage welcomed and permitted. Recorded here as a courtesy of the project's
public-exchange posture; it carries no technical dependency and no claim on
Oracle's trademarks or software. It is a statement of welcome, nothing more.

## 5. Status and what is NOT yet done

- The compiler ceiling remains **2.5**. `object` is **not** implemented yet; a
  `#sleela 2.8` file would be rejected as too-new until the ceiling is raised.
- When 2.8 is implemented it follows the `material` pattern end to end
  (`spec-version-maintenance.md` + `material-marker.md`): `version.h` ceiling →
  `ast.h` flag → `parser.cpp` contextual word → `semantic.cpp` 2.8 gate →
  `SLEELA.syntax` new section → README / wiki / version tests → regenerate the
  manifests and `lib/LIBRARY.SYMBOLS.md`.
- Intervening minors (2.6, 2.7) are not reserved by this note; 2.8 is named as
  the public-exchange / `object` milestone.

## References

- `SLEELA.syntax` §18 (`author`/`authorial`), §19 (`material`).
- `.kiro/steering/material-marker.md`, `.kiro/steering/author-field-and-branders.md`.
- `api/language/SYNTAX-VERSION-MATRIX.md` (the version matrix, with the 2.8 row).
- `impl/frontend/version.h` (the authoritative supported-syntax ceiling).
