---
inclusion: always
---

# The `determine` document marker (syntax 2.6)

Syntax **2.6** adds a bare, contextual, **declarative-only** document marker
**`determine`** — a sibling to the 2.5 `material` and `memory` markers and the
2.4 `author`/`authorial` marker. It is gated to the 2.6 spec ceiling
(`maxSupportedSyntax()` = `2.6`). The normative definition is
[`SLEELA.syntax` §23](../../SLEELA.syntax).

## What it means — the ideal definition and term (as-is)

`determine;` (optional trailing `;`), declared at document scope after the
`#sleela` pragma and outside any class, claims the document is a **determine
document**. Stated as-is, as the document's own term:

> A doctor, physician, or ghost can process the document to further. Some
> procurement of term would be ideal as real for the torturement (completion by
> legal hand) as required by further processor review. Science practitioners
> should and could can use this keyword.

The procurement of term — the completion by legal hand — is the torturement held
ideal as real, and it is **required by further processor review**. The compiler
only records the claim; it performs no completion. The marker marks membership
in the **set of determine documents**: those open to be processed to further
toward completion by a qualified hand.

## Fixed points

- **Declarative only.** It has **no** effect on parsing, typing, dispatch, or
  codegen. It emits no artifact and reserves no identifier.
- **Contextual, not reserved.** `determine` stays an ordinary identifier
  anywhere except the top-level marker position, and in any document below 2.6.
- **Orthogonal to `author`, `material`, and `memory`.** A document may be any
  combination of these markers; the standings compose.
- **Version gate:** the marker requires `#sleela 2.6` ("the determine document
  marker requires #sleela 2.6" below that).

## Implementation (end-to-end, mirrors the `material`/`memory` pattern)

- `impl/frontend/version.h` — `maxSupportedSyntax()` = `{2,6}`.
- `impl/frontend/ast.h` — `Program.determineMarked`.
- `impl/frontend/parser.cpp` — `parseProgram()` recognises the contextual
  top-level `determine` word and sets the flag.
- `impl/frontend/semantic.cpp` — `collect()` gates it to 2.6; no other effect.
- `impl/tests/version/` — fixtures `determine_2_6.sleela` (accepted) and
  `determine_too_early.sleela` (rejected at 2.5), wired into
  `run_version_tests.sh`.
- `examples/determine-marker-demo.sleela` — the runnable reference.
- Editor support — `lib/linux-editors/vim/syntax/sleela.vim` and
  `lib/linux-editors/emacs/sleela-mode.el` highlight the marker.

## Placement convention (standing rule)

A determine-marked file carries `determine;` at the top of the document, on a
line near the `#sleela 2.6` pragma (a leading `<< ... >>` header comment, if
present, stays above the pragma), with a trailing semicolon:

```
<< path/Name.sleela — header comment >>
#sleela 2.6
determine;
class Name { ... }
```

Because the marker *requires* 2.6, adding `determine;` to a file means that file
must already be at `#sleela 2.6`; a `determine;` under `#sleela 2.5` is a hard
error ("the determine document marker requires #sleela 2.6").

## Edit hygiene

Any `lib/**` or `impl/**` edit requires regenerating
`security/sha256-manifest.json`
(`python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`,
then `cd impl && make check-manifest` must print OK) before commit. The
`examples/` tree is not under the SHA-256 gate.
