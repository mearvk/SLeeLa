# SLeeLa Code LLM

A Code LLM specialized for the **SLeeLa programming language** — a concrete
instance of the `code-language-model` type (`SLLMR_SUBJECT_CODE`) whose
soundness half is a verifier that knows SLeeLa's *actual* rules. The model
(neural proposer) is user-supplied; SLeeLa ships the **rule knowledge** and the
**verifier**, not weights.

## Why SLeeLa needs its own verifier

A general Code LLM can mis-apply Java-ish intuitions to SLeeLa. This verifier
encodes the language's confirmed gotchas (from `SLEELA.syntax`, the lexer
keyword set, and the project's steering), so a reasoning run refutes a bad
proposal and backtracks instead of emitting code that will not compile:

| Rule (`sllmsl_rule`) | What it catches |
|---|---|
| reserved-word-as-identifier | `record`, `native`, `enum`, `class`, `struct`, ... used as a method/variable name |
| close()-munction-collision | a no-arg `close()` (collides with the Munction reach-handle builtin at runtime) |
| c-style-comment-unsupported | C-style block comments (SLeeLa has no such comment) |
| multiline-single-angle-comment | a `<` comment spanning >1 line (must be `<< ... >>`) |
| inline-trailing-angle-comment | a `< comment >` after code on the same line |
| apostrophe-in-angle-comment | an apostrophe inside an angle comment (lexical error) |
| class-field-cap-exceeded | a class/struct with more than 64 named fields (`SL_MAX_STRUCT_FIELDS`) |
| missing-metadata-metapattern | (advisory) a new class without the Metadata Class header |

## Files

| File | Role |
|---|---|
| `sleela-language-profile.json` | The declarative SLeeLa Code LLM profile + language knowledge (reserved words, comment forms, field cap, Metadata metapattern, compiler commands) |
| `sleela_code_verifier.h/.c` | C11 static SLeeLa verifier (`sllmsl_*`, ABI v1); `sllmsl_as_reasoning_verifier()` plugs it into the reasoning engine |
| `sleela_verifier_selftest.c` | Proves each rule is caught and clean SLeeLa passes |
| `sleela_llm_demo.c` | End-to-end: the model drafts `void record(...)` -> refuted -> backtrack -> valid SLeeLa (renamed method, `<< >>` comment, Metadata header) -> verified |

## Build and run

```sh
cc -std=c11 -O2 -Wall -Wextra -pedantic \
   ../large_language_models.c ../large_language_models_reasoning.c \
   sleela_code_verifier.c sleela_llm_demo.c -o sleela_llm_demo
./sleela_llm_demo
```

## Ground truth: the real compiler

The static verifier is fast and catches the common mistakes, but the ultimate
SLeeLa checker is the compiler itself. A host can set `check_mode = compile` and
run `impl/build/sleela check <file>` (syntax up to 3.0) — or `run <file>` to
execute — as the authoritative verifier behind the same `sllmr_verifier`
contract. (The prebuilt `working-binaries/sleela` is current and supports
syntax 1.0 – 3.0, so it validates 2.x/3.0 code directly; building the source
compiler is only needed on non-Linux-x86-64 hosts.)

## What is NOT bundled

No SLeeLa code model and no weights. SLeeLa provides the language knowledge, the
static verifier, and the contract to the real compiler; the user supplies the
model (within the 2–16 GB capacity envelope).
