---
inclusion: always
---

# SLeeLa authoring gotchas (verified against the source-built compiler)

Confirmed behaviours of the toolchain that `sleela check` alone will not catch.
These were verified against the current compiler, which supports **source
syntax 1.0 – 3.0**. Keep new/edited `.sleela` code within these rules.

## Compiler choice

- The prebuilt `working-binaries/sleela` is current: it reports
  `SLeeLa™ Language/Runtime 1.3.2-pro` and supports **syntax 1.0 – 3.0**, so it
  checks and runs the whole tree (including `#sleela 2.x` and `#sleela 3.0`
  files). Use it directly:

      ./working-binaries/sleela check <file>
      ./working-binaries/sleela run   <file>

- To build the same compiler from source (authoritative, and required on any
  non-Linux-x86-64 host):

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

3. **New arrays start as `null`, not zero.** `new int[n]` / `new double[n]`
   elements read as `null` until assigned, so `used[j] == 0` is false and a sum
   over unassigned slots misbehaves. Assign every element you will read. This
   fails at `run` with `array index out of range` when the value is later used
   as an index, even though `check` passes. (Found in
   `lib/advantage/trading/SLTradingCounsel.sleela`, whose `rank` initialises its
   working arrays for this reason.)

4. **int / int is integer division.** `held / total` of two `int`s truncates;
   write `(held * 1.0) / total` when a fraction is intended.

5. **`String` has no methods.** The built-in `String` exposes only `+`
   concatenation and `==` / `!=`: no `length`, `charAt`, `substring`, `trim`.
   Text that needs parsing (for example rate-limit header values) must be
   parsed by the caller or the C layer, and passed in as numbers.

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

## Integer comparisons are exact only up to 2^53 - 1

Measured with `working-binaries/sleela` (0.3.30-dev / 1.3.2-pro): `int` is a
64-bit signed integer and `+ - * / %` are exact over the whole range, but the
comparison operators (`== != < <= > >=`) lose precision above 2^53 - 1
(9,007,199,254,740,991). For example `9007199254740993 == 9007199254740992` is
`true`, and `(3037000499 * 3037000499) <= (3037000499 * 3037000499 - 1)` is
`true`. A search such as an integer square root can therefore return a wrong
answer silently when values pass that limit.

Rule: keep every value that is ever compared at or below 9007199254740991, and
refuse (do not clamp or wrap) inputs that could push it past. See
`lib/advantage/trading-actions-on-verb/SLEuclid.sleela` for the pattern
(`maxExact`) and `TaUse15EuclidExhaustive` for the edge tests.

## Built-in names and type tests (found while implementing /lib/java, 2026-11)

- **`read`, `write`, `sleep` are VM built-ins.** An unqualified call inside a
  class binds to the built-in, not to the class method of the same name
  (`read(b, off, len)` fails with `read(handle) takes one argument`). Call your
  own method as `this.read(...)` / `this.write(...)`.
- **`instanceof` with a built-in value type uses the lowercase VM name:**
  `x instanceof string`, `int`, `long`, `double`, `boolean`.
  `x instanceof String` is always false for a built-in string, and the cast
  `(String) x` fails at run time ("not a valid class/struct instance").
  To turn an `Any` that holds a string into a `String`, write `"" + x`.
- **Java `byte`/`char`/`short` ride the int slot.** `int` and `long` overloads
  cannot be told apart, so do not implement a Java method whose result depends
  on which one the caller passed (for example `HexFormat.toHexDigits(value)`).
- **Plain vs Java UTF-16 strings (syntax 3.0).** `javaStringFromUtf8(s)` goes
  plain -> UTF-16; `javaStringToUtf8(u)` goes back for any Unicode text but is a
  fatal VM error on NUL, an unpaired surrogate or >65536 bytes, so in library
  code call `JavaText.plainOf(u)` (checks first, throws a catchable
  `UnsupportedOperationException`). Build text per character in UTF-16 with
  `javaStringFromCodeUnit(c)` / `JavaText.codePointU(cp)` and convert once at
  the end; appending one plain character at a time splits surrogate pairs.
  A string literal cannot contain a `\uD83D`-style surrogate escape (lexer
  error); write the character itself in UTF-8.
