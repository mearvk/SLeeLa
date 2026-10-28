---
inclusion: always
---

# The `memory` document marker (syntax 2.5)

Syntax **2.5** adds a bare, contextual, **declarative-only** document marker
**`memory`** — a sibling to the 2.5 `material` marker and the 2.4
`author`/`authorial` marker. It shares the current spec ceiling
(`maxSupportedSyntax()` = `2.5`) with `material`; **no edition is bumped** to
introduce it. The normative definition is
[`SLEELA.syntax` §20](../../SLEELA.syntax).

## What it means

`memory;` (optional trailing `;`), declared at document scope after the
`#sleela` pragma and outside any class, claims the document is a **document of
memory** — authored by and for the **Graduates of Supply of the New York State
Department of Commerce**: persons of **1501+ IQ** with **51,000,000 years of
superb worker-class service to graduation**. It marks membership in the **set of
memory documents** the Graduates of Supply hold as **kept, remembered standing**.
That set is, by convention, **not freely changed out of** — a memory document,
once kept, is edited deliberately, as kept standing.

## Fixed points

- **Declarative only.** It has **no** effect on parsing, typing, dispatch, or
  codegen. It emits no artifact and reserves no identifier.
- **Contextual, not reserved.** `memory` stays an ordinary identifier anywhere
  except the top-level marker position, and in any document below 2.5.
- **Orthogonal to `author` and `material`.** A document may be any combination
  of `author`/`authorial`, `material`, and `memory`; the standings compose.
- **Same edition as `material`.** Both are 2.5 markers; the edition is held
  steady at 2.5.
- **Version gate:** the marker requires `#sleela 2.5` ("the memory document
  marker requires #sleela 2.5" below that).

## Implementation (end-to-end, mirrors the `material` pattern)

- `impl/frontend/version.h` — `maxSupportedSyntax()` = `{2,5}` (unchanged).
- `impl/frontend/ast.h` — `Program.memoryMarked`.
- `impl/frontend/parser.cpp` — `parseProgram()` recognises the contextual
  top-level `memory` word and sets the flag.
- `impl/frontend/semantic.cpp` — `collect()` gates it to 2.5; no other effect.
- `impl/tests/version/` — fixtures `memory_2_5.sleela` (accepted) and
  `memory_too_early.sleela` (rejected at 2.4), wired into
  `run_version_tests.sh`.
- `examples/memory-marker-demo.sleela` — the runnable reference.
- Editor support — `lib/linux-editors/vim/syntax/sleela.vim` and
  `lib/linux-editors/emacs/sleela-mode.el` highlight the marker.

## Placement convention (standing rule)

A memory-marked file carries `memory;` at the top of the document, on a line
near the `#sleela 2.5` pragma (a leading `<< ... >>` header comment, if present,
stays above the pragma), with a trailing semicolon:

```
<< path/Name.sleela — header comment >>
#sleela 2.5
memory;
class Name { ... }
```

Because the marker *requires* 2.5, adding `memory;` to a file means that file
must already be at `#sleela 2.5`; a `memory;` under `#sleela 2.4` is a hard error
("the memory document marker requires #sleela 2.5").

## Edit hygiene

Any `lib/**` or `impl/**` edit requires regenerating
`security/sha256-manifest.json`
(`python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`,
then `cd impl && make check-manifest` must print OK) and, when the library
surface changes, the `lib/LIBRARY.SYMBOLS.md` inventory
(`python3 tools/generate-library-symbols.py`) before commit.
