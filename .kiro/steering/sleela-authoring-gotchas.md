---
inclusion: always
---

# SLeeLa authoring gotchas (verified against the source-built compiler)

Confirmed behaviours of the toolchain that `sleela check` alone will not catch.
These were verified with the source-built compiler (`impl/build/sleela`), which
supports syntax up to 2.2. Keep new/edited `.sleela` code within these rules.

## Compiler choice for `#sleela 2.x`

- The prebuilt `working-binaries/sleela` only supports syntax up to **1.11**, so
  it **cannot** check or run `#sleela 2.x` files. Build the source compiler to
  validate 2.x library code:

      cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela

  (needs `g++`, X11/fontconfig/freetype/`libdl`; takes a few minutes.) Then use
  `impl/build/sleela check <file>` / `impl/build/sleela run <file>`.

## Validating library classes

- A standalone library class checked alone reports
  `Semantic error: no 'main' method found` — that is expected, not a failure.
  Validate a library class through a **sibling demo** that has a `main()`:
  `run` auto-links sibling `.sleela` files in the same directory (no imports
  needed). The text family's reference is `lib/text/text-string-demo.sleela`.
- If two `/lib` files declare the same class name (e.g. `core/SLString` and
  `text/SLString`), linking is **ambiguous**; a runnable demo must carry a
  versioned import such as `import gui.version-military;` to disambiguate, or
  live in a directory that does not pull in both.

## Runtime gotchas that pass `check` but fail at `run`

1. **Typed-local binding for custom-class array elements.** Calling a method on
   a custom-class array element inline (e.g. `guides[i].getName()` where
   `guides` is `SLPhilosopherGuide[]`) fails at runtime with
   `class '<unknown>' has no method 'X'` — the element type is lost. Bind the
   element to a typed local first, then call:

       SLPhilosopherGuide g = guides[i];
       g.getName();

   This is needed in every method that calls through an array element (render /
   broadcast / dispose loops, etc.), even though `check` passes without it.
   (Elements that are built-in `String[]`/`int[]` are fine — this applies to
   arrays of custom classes.)

2. **`close()` is reserved.** A no-arg method named `close()` collides with the
   reserved Munction reach-handle built-in and fails at runtime with
   `Munction closeWithReceipt() requires a reach handle`. Name it something else
   (`requestClose()`, `sealMoment()`, `dispose()`).

## Comment placement (reminder)

Angle-bracket comments (`<` / `<<`) must start a line (only whitespace before).
An inline trailing `code; < comment >` is a syntax error
(`unexpected token '<' in expression`). Put the comment on its own line. (Full
rule in `comment-style.md`.)

## Manifest gate (reminder)

Editing any file under `lib/**` (or the impl core/frontend) breaks the
verified-build manifest check. After intentional edits, regenerate and confirm:

    python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
    cd impl && make check-manifest        # must print OK

Commit the updated `security/sha256-manifest.json` alongside the source. The
manifest covers `lib/**/*.sleela` (including demos) plus lib C/C++ helpers, but
**not** `.md` files.
