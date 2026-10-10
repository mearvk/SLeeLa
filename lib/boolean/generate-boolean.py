#!/usr/bin/env python3
"""Generate the SLeeLa `boolean` package: Boolean/Binary word classes.

The /lib filesystem is the single source of truth for the SLeeLa class
vocabulary (see lib/LIBRARY.INDEX.md). This tool materializes the Boolean
syllable-word vocabulary described by MEARVK LLC:

  BOOLEAN.TRUE and BOOLEAN.FALSE are the two primitive syllables. A Boolean
  *word* is an ordered sequence of such syllables, e.g. BOOLEAN.TRUE.FALSE or
  BOOLEAN.TRUE.FALSE.FALSE. The HEAD syllable is the *result*; every following
  syllable is a *carry* (a caveat structure) that must be observed/agree.

Depth families:
  - depth 8  : the full enumeration of all 2^8 = 256 TRUE/FALSE words, one
               class per file, under boolean/8/.
  - depth 16 : the generative word factory (BooleanWord16) + canonical boundary
               words under boolean/16/.  Full enumeration (2^16 = 65,536 files)
               is NOT materialized.
  - depth 32 : the generative word factory (BooleanWord32) + canonical boundary
               words under boolean/32/.  Full enumeration (2^32 = 4,294,967,296
               files) is physically impossible.

File naming is coherent to the value: each syllable is one glyph, T=TRUE,
F=FALSE, so BOOLEAN.TRUE.FALSE.FALSE -> Bool_TFF.sleela.

Language note: SLeeLa library classes are value/getter classes. These classes
avoid String iteration (not available) and the `long` return type (not
available); every property is precomputed here and emitted as a literal
constant return, matching the existing minimal library style.

Usage:
  python3 lib/boolean/generate-boolean.py            # write all files
  python3 lib/boolean/generate-boolean.py --count    # print counts only
"""
import argparse
import itertools
import sys
from pathlib import Path

PKG = "boolean"
SLEELA_VERSION = "#sleela 1.3"


def pkg_dir() -> Path:
    return Path(__file__).resolve().parent


def lower(b: bool) -> str:
    return "true" if b else "false"


def header(rel_path: str, definition: str) -> str:
    return (
        "/*\n"
        f" * {rel_path}\n"
        " * SLeeLa Standard Library Definition.\n"
        f" * Definition: {definition}\n"
        " */\n"
        f"{SLEELA_VERSION}\n"
    )


def word_class(glyphs: str, depth: int, subdir: str):
    """Return (class_name, rel_path, source) for one concrete Boolean word."""
    class_name = f"Bool_{glyphs}"
    rel = f"lib/{PKG}/{subdir}/{class_name}.sleela"
    syllables = ".".join("TRUE" if g == "T" else "FALSE" for g in glyphs)
    dotted = "BOOLEAN." + syllables
    head = glyphs[0]
    head_word = "TRUE" if head == "T" else "FALSE"
    carry = glyphs[1:]
    trues = glyphs.count("T")
    falses = glyphs.count("F")
    bits = "".join("1" if g == "T" else "0" for g in glyphs)
    value = int(bits, 2)
    carry_has_false = "F" in carry
    uniform = not (("F" in glyphs) and ("T" in glyphs))

    definition = (
        f"Defines the Boolean/Binary word {dotted} "
        f"(depth {depth}) as a SLeeLa standard-library constant. "
        f"HEAD syllable is the result ({head_word}); the remaining "
        f"{depth - 1} syllable(s) are carried caveat structure."
    )
    # value(): int fits for depth 8 (max 255); for depth 16/32 the exact
    # decimal is returned as a String to stay within supported return types.
    if depth <= 8:
        value_member = f"  int value() {{ return {value}; }}"
    else:
        value_member = f'  String value() {{ return "{value}"; }}'

    body = f"""{header(rel, definition)}// SLeeLa boolean/{subdir} front-end constant — one Boolean word per file.
// Word: {dotted}
// Family: boolean. The HEAD syllable is the RESULT; trailing syllables are
// CARRY (caveat structure) that must be observed/agree. A FALSE carried behind
// a TRUE result means: result is TRUE, but a concern of carry is noted.
class {class_name} {{
  String word() {{ return "{dotted}"; }}
  String glyphs() {{ return "{glyphs}"; }}
  int depth() {{ return {depth}; }}
  // Head syllable = the asserted result.
  boolean head() {{ return {lower(head == 'T')}; }}
  String headSyllable() {{ return "{head_word}"; }}
  // Carry = the trailing syllables, read as a caveat structure.
  String carry() {{ return "{carry}"; }}
  int carryDepth() {{ return {depth - 1}; }}
  // A caveat exists when any carried syllable disagrees (a FALSE is carried).
  boolean caveat() {{ return {lower(carry_has_false)}; }}
  // Agreement: the whole word is uniform (all syllables equal the head).
  boolean agrees() {{ return {lower(uniform)}; }}
  int trueCount() {{ return {trues}; }}
  int falseCount() {{ return {falses}; }}
  // Binary reading: TRUE=1, FALSE=0, head syllable most significant.
  String bits() {{ return "{bits}"; }}
{value_member}
}}
"""
    return class_name, rel, body


def primitive_class(name: str, is_true: bool):
    rel = f"lib/{PKG}/{name}.sleela"
    word = "TRUE" if is_true else "FALSE"
    glyph = "T" if is_true else "F"
    definition = (
        f"Defines the primitive Boolean syllable BOOLEAN.{word} "
        "as a SLeeLa standard-library constant."
    )
    body = f"""{header(rel, definition)}// SLeeLa boolean primitive syllable — BOOLEAN.{word}.
// Family: boolean. The two primitive syllables from which every Boolean word
// (BOOLEAN.TRUE.FALSE, BOOLEAN.TRUE.FALSE.FALSE, ...) is composed.
class {name} {{
  String word() {{ return "BOOLEAN.{word}"; }}
  String glyphs() {{ return "{glyph}"; }}
  int depth() {{ return 1; }}
  boolean head() {{ return {lower(is_true)}; }}
  String headSyllable() {{ return "{word}"; }}
  boolean value() {{ return {lower(is_true)}; }}
  int bit() {{ return {1 if is_true else 0}; }}
}}
"""
    return rel, body


def factory_class(depth: int):
    name = f"BooleanWord{depth}"
    rel = f"lib/{PKG}/{depth}/{name}.sleela"
    total = 2 ** depth
    definition = (
        f"Defines BooleanWord{depth}, the generative factory for all {total} "
        f"depth-{depth} Boolean/Binary words. Full materialization of every "
        f"combination as a file is impractical at this depth; this factory "
        f"models any such word by its HEAD (result) and CARRY (caveat) "
        f"composition, which is validated and read on demand."
    )
    body = f"""{header(rel, definition)}// SLeeLa boolean/{depth} generative word factory.
// All {total} combinations of BOOLEAN.TRUE / BOOLEAN.FALSE at depth {depth}
// are modeled by this type rather than materialized as {total} separate files.
// A word is summarized by its HEAD syllable (the RESULT) and the composition of
// its CARRY (the caveat structure): how many carried syllables are TRUE and how
// many are FALSE. A FALSE carried behind the head raises a caveat to observe.
class {name} {{
  int headBit;        // 1 = TRUE result, 0 = FALSE result
  int carryTrue;      // number of carried TRUE syllables
  int carryFalse;     // number of carried FALSE syllables
  // Configure one word of this depth: the head result and the carry makeup.
  void configure(int head, int cTrue, int cFalse) {{
    headBit = head; carryTrue = cTrue; carryFalse = cFalse;
  }}
  int depth() {{ return {depth}; }}
  int carryDepth() {{ return {depth - 1}; }}
  // Valid when the head bit is 0/1 and the carry syllables fill the carry slots.
  boolean valid() {{
    return (headBit == 0 || headBit == 1)
        && carryTrue >= 0 && carryFalse >= 0
        && carryTrue + carryFalse == {depth - 1};
  }}
  // Head syllable = the asserted result.
  boolean head() {{ return headBit == 1; }}
  String headSyllable() {{ return headBit == 1 ? "TRUE" : "FALSE"; }}
  // A caveat exists when any carried syllable is FALSE.
  boolean caveat() {{ return carryFalse > 0; }}
  // Agreement: the word is uniform (carry entirely matches the head).
  boolean agrees() {{
    return (headBit == 1 && carryFalse == 0) || (headBit == 0 && carryTrue == 0);
  }}
  int trueCount() {{ return carryTrue + (headBit == 1 ? 1 : 0); }}
  int falseCount() {{ return carryFalse + (headBit == 0 ? 1 : 0); }}
  // Dotted label for the result and the caveat posture.
  String describe() {{
    return "BOOLEAN." + headSyllable()
         + (caveat() ? " +carry-caveat" : " +clean");
  }}
}}
"""
    return name, rel, body


def facade():
    rel = f"lib/{PKG}/SLPackage.sleela"
    body = f"""{header(rel, 'Defines the SLeeLa standard-library package for boolean.')}// Library facade for module package: boolean
// Exposes the package to the SLeeLa compiler, Loader, and Nordshrift.
// The Master Classes (BooleanTrue, BooleanFalse, the depth-8 enumeration under
// 8/, and the depth-16/32 word factories under 16/ and 32/) are first-class
// one-class-per-file source units in this package; this facade only names the
// package itself, mirroring lib/citizen/SLPackage.sleela.
class SLPackage {{
  String packageName;
  void configure(String name) {{ packageName = name; }}
  String name() {{ return packageName; }}
  boolean available() {{ return packageName != null; }}
}}
"""
    return rel, body


def write(path: Path, text: str, dry: bool):
    if dry:
        return
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--count", action="store_true", help="print counts and exit")
    args = ap.parse_args()

    base = pkg_dir()
    repo_root = base.parent.parent  # .../  (lib/boolean -> repo root)
    dry = args.count

    written = []

    rel, body = facade()
    write(repo_root / rel, body, dry)
    written.append((rel, "facade"))

    for name, is_true in (("BooleanTrue", True), ("BooleanFalse", False)):
        rel, body = primitive_class(name, is_true)
        write(repo_root / rel, body, dry)
        written.append((rel, "source"))

    # Depth 8: full enumeration — all 256 words, one class per file.
    for combo in itertools.product("TF", repeat=8):
        glyphs = "".join(combo)
        _cn, rel, body = word_class(glyphs, 8, "8")
        write(repo_root / rel, body, dry)
        written.append((rel, "source"))

    # Depth 16 and 32: generative factory + canonical boundary words.
    for depth in (16, 32):
        subdir = str(depth)
        _cn, rel, body = factory_class(depth)
        write(repo_root / rel, body, dry)
        written.append((rel, "source"))
        boundaries = {
            "allTrue": "T" * depth,
            "allFalse": "F" * depth,
            "trueCarryFalse": "T" + "F" * (depth - 1),
            "falseCarryTrue": "F" + "T" * (depth - 1),
        }
        for _label, glyphs in boundaries.items():
            _cn2, rel2, body2 = word_class(glyphs, depth, subdir)
            write(repo_root / rel2, body2, dry)
            written.append((rel2, "source"))

    sources = sum(1 for _r, k in written if k == "source")
    facades = sum(1 for _r, k in written if k == "facade")
    print(f"boolean package: files={len(written)} sources={sources} facades={facades}")
    if dry:
        for r, k in written[:6]:
            print(f"  {k:7} {r}")
        print("  ...")
    return 0


if __name__ == "__main__":
    sys.exit(main())
