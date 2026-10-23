---
inclusion: always
---

# Angle-bracket comment style (`<` / `<<`)

SLeeLa has a new, preferred comment style built on leading angle brackets. The
frontend lexer and version resolver recognize it (see `impl/frontend/lexer.cpp`
and `impl/frontend/version.cpp`). There are two forms:

- **Line comment** — a leading `<` begins a comment that runs to the matching
  `>` (or end of line). It must appear at the start of a line (only whitespace
  may precede it).

      < this is a single-line angle-bracket comment >

- **Block comment** — a leading `<<` opens a block comment that spans lines and
  is closed by `>>`. It too must start the line (only leading whitespace
  allowed).

      << this is an angle-bracket block
         comment spanning multiple lines >>

## The rule

**Every file we touch must use the new angle-bracket comment style.** When you
create a new file, or edit/add comments in an existing file, write comments in
the `<` / `<<` form rather than the legacy `//` and `/* */` forms.

- New files: all comments use `<` / `<<`.
- Existing files you modify: convert comments in the regions you touch (and any
  comment you add) to the angle-bracket form. You do not need to rewrite
  untouched comments elsewhere in the file, but prefer migrating them when it is
  low-risk and in-scope.
- The legacy `//` and `/* */` forms still lex and remain valid for untouched
  code; the angle-bracket form is the go-forward house style.

## Notes

- Placement matters: the opener (`<` or `<<`) must be the first non-whitespace
  on its line. Inline trailing comments after code on the same line are not part
  of this style — keep comments on their own line.
- This applies across the repo (SLeeLa source, C/C++ frontend/impl, demos,
  tooling) for any file we edit.
- Reminder (existing gate): edits under `lib/**` or the impl core/frontend break
  the verified-build manifest check. After intentional edits, regenerate with
  `python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json`,
  confirm `cd impl && make check-manifest` prints OK, and commit the updated
  manifest alongside the source.
