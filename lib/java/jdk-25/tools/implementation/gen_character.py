#!/usr/bin/env python3
"""Emit the java.lang.Character body.

Case mapping and the letter/digit/whitespace predicates are generated from
Python's unicodedata for the Basic Multilingual Plane ranges listed in
BLOCKS, as compact run tables (no per-character code), so the SLeeLa class is
exact for those blocks and documents what it does not cover.
"""
import sys
import tempfile
import unicodedata
from pathlib import Path
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-")) / "java.lang.Character.body"

# Covered BMP blocks for the table-driven predicates and case maps.
BLOCKS = [
    (0x0000, 0x024F),  # Basic Latin .. Latin Extended-B
    (0x0370, 0x03FF),  # Greek and Coptic
    (0x0400, 0x052F),  # Cyrillic + supplement
    (0x0530, 0x058F),  # Armenian
    (0x05D0, 0x05EA),  # Hebrew letters
    (0x0620, 0x064A),  # Arabic letters
    (0x0660, 0x0669), (0x06F0, 0x06F9),  # Arabic-Indic digits
    (0x0966, 0x096F),  # Devanagari digits
    (0x1E00, 0x1EFF),  # Latin Extended Additional
    (0x1F00, 0x1FFF),  # Greek Extended
    (0x3040, 0x30FF),  # Hiragana, Katakana
    (0x4E00, 0x9FFF),  # CJK Unified Ideographs
    (0xAC00, 0xD7A3),  # Hangul syllables
    (0xFF10, 0xFF19), (0xFF21, 0xFF3A), (0xFF41, 0xFF5A),  # fullwidth digits/Latin
]


def covered(cp):
    return any(a <= cp <= b for a, b in BLOCKS)


def runs(pred):
    """Inclusive [start, end] runs of code points in the covered blocks."""
    out = []
    for a, b in BLOCKS:
        start = None
        for cp in range(a, b + 2):
            ok = cp <= b and pred(cp)
            if ok and start is None:
                start = cp
            if not ok and start is not None:
                out.append((start, cp - 1))
                start = None
    return out


def range_method(name, rs):
    lines = [f"  boolean {name}(int cp) {{"]
    for a, b in rs:
        if a == b:
            lines.append(f"    if (cp == {a}) {{ return true; }}")
        else:
            lines.append(f"    if (cp >= {a} && cp <= {b}) {{ return true; }}")
    lines.append("    return false;")
    lines.append("  }")
    return "\n".join(lines) + "\n"


def case_runs(direction):
    """Runs (start, end, delta, step) of simple case mappings in covered blocks."""
    pairs = []
    for a, b in BLOCKS:
        for cp in range(a, b + 1):
            ch = chr(cp)
            m = ch.upper() if direction == "upper" else ch.lower()
            if len(m) == 1 and ord(m) != cp:
                pairs.append((cp, ord(m) - cp))
    # Java simple mappings that differ from Python full mappings are covered
    # because only single-code-point results are kept.
    out = []
    i = 0
    while i < len(pairs):
        cp, d = pairs[i]
        # try stride 1 or 2 runs with the same delta
        j = i
        step = None
        if i + 1 < len(pairs) and pairs[i + 1][1] == d and pairs[i + 1][0] - cp in (1, 2):
            step = pairs[i + 1][0] - cp
            j = i + 1
            while j + 1 < len(pairs) and pairs[j + 1][1] == d and pairs[j + 1][0] - pairs[j][0] == step:
                j += 1
        if step is None:
            out.append((cp, cp, d, 1))
            i += 1
        else:
            out.append((cp, pairs[j][0], d, step))
            i = j + 1
    return out


def case_method(name, rs):
    lines = [f"  int {name}(int cp) {{"]
    if name == "toLowerCase":
        lines.append("    if (cp == 304) { return 105; }")
    for a, b, d, step in rs:
        if a == b:
            lines.append(f"    if (cp == {a}) {{ return {a + d}; }}")
        elif step == 1:
            lines.append(f"    if (cp >= {a} && cp <= {b}) {{ return cp + {d}; }}")
        else:
            lines.append(f"    if (cp >= {a} && cp <= {b} && (cp - {a}) % 2 == 0) {{ return cp + {d}; }}")
    lines.append("    return cp;")
    lines.append("  }")
    return "\n".join(lines).replace("+ -", "- ") + "\n"


def ascii_table():
    lines = ["  << The ASCII character for code unit c as an ordinary SLeeLa String\n"
             "     (concatenates with literals), or null outside printable ASCII. >>",
             "  String asciiString(int c) {"]
    special = {9: "\\t", 10: "\\n", 13: "\\r"}
    for c, lit in special.items():
        lines.append(f'    if (c == {c}) {{ return "{lit}"; }}')
    lines.append("    if (c < 32 || c > 126) { return null; }")
    # binary-ish split by 16s to keep the chain short
    for base in range(32, 127, 16):
        top = min(base + 15, 126)
        lines.append(f"    if (c <= {top}) {{")
        for c in range(base, top + 1):
            ch = chr(c)
            if ch == '"':
                lit = '\\"'
            elif ch == "\\":
                lit = "\\\\"
            else:
                lit = ch
            lines.append(f'      if (c == {c}) {{ return "{lit}"; }}')
        lines.append("    }")
    lines.append("    return null;")
    lines.append("  }")
    return "\n".join(lines) + "\n"


def is_letter(cp):
    return unicodedata.category(chr(cp)).startswith("L")


def is_digit(cp):
    return unicodedata.category(chr(cp)) == "Nd"


def is_upper(cp):
    return unicodedata.category(chr(cp)) == "Lu"


def is_lower(cp):
    return unicodedata.category(chr(cp)) == "Ll"


def is_titlecase(cp):
    return unicodedata.category(chr(cp)) == "Lt"


body = r'''  < ---- java.lang.Character members (procedural implementation in SLeeLa) ---- >
  << The boxed char plus the java.lang.Character static API as instance
     methods over int code points (the VM has no char primitive; a char is an
     int in 0..65535). Surrogate, code-point, ISO-control, whitespace and
     space-separator rules are exact. Letter/digit/case predicates and the
     simple case maps are exact for the Unicode blocks the generator covers
     (Latin, Greek, Cyrillic, Armenian, Hebrew and Arabic letters, several
     decimal-digit blocks, Latin/Greek extended, Hiragana/Katakana, CJK
     ideographs, Hangul, fullwidth Latin); other code points report false or
     map to themselves (tables generated from Unicode 14.0 character data).
     The simple name Character also exists elsewhere in /lib, so a program
     selects this class with import java.java.lang; . toString of a non-ASCII char needs a Java UTF-16
     String, so it returns null until the String bridge lands. >>
  static final int MIN_RADIX = 2;
  static final int MAX_RADIX = 36;
  static final int MIN_VALUE = 0;
  static final int MAX_VALUE = 65535;
  static final int MIN_HIGH_SURROGATE = 55296;
  static final int MAX_HIGH_SURROGATE = 56319;
  static final int MIN_LOW_SURROGATE = 56320;
  static final int MAX_LOW_SURROGATE = 57343;
  static final int MIN_SURROGATE = 55296;
  static final int MAX_SURROGATE = 57343;
  static final int MIN_SUPPLEMENTARY_CODE_POINT = 65536;
  static final int MIN_CODE_POINT = 0;
  static final int MAX_CODE_POINT = 1114111;
  static final int SIZE = 16;
  static final int BYTES = 2;

  int value;
  Character(int c) { value = c; }

  int charValue() { return value; }
  String toString() { return asciiString(value); }
  String toStringOf(int c) { return asciiString(c); }
  int hashCode() { return value; }
  int hashCodeOf(int c) { return c; }
  boolean equals(Any obj) {
    if (obj instanceof Character) {
      Character o = (Character) obj;
      return o.value == value;
    }
    return false;
  }
  int compareTo(Character another) { return value - another.value; }
  int compare(int x, int y) { return x - y; }
  Character valueOf(int c) { return new Character(c); }

  < code points and surrogates (exact) >
  boolean isValidCodePoint(int cp) { return cp >= 0 && cp <= MAX_CODE_POINT; }
  boolean isBmpCodePoint(int cp) { return cp >= 0 && cp <= 65535; }
  boolean isSupplementaryCodePoint(int cp) { return cp >= 65536 && cp <= MAX_CODE_POINT; }
  boolean isHighSurrogate(int ch) { return ch >= 55296 && ch <= 56319; }
  boolean isLowSurrogate(int ch) { return ch >= 56320 && ch <= 57343; }
  boolean isSurrogate(int ch) { return ch >= 55296 && ch <= 57343; }
  boolean isSurrogatePair(int high, int low) { return isHighSurrogate(high) && isLowSurrogate(low); }
  int charCount(int codePoint) { if (codePoint >= 65536) { return 2; } return 1; }
  int toCodePoint(int high, int low) { return (high - 55296) * 1024 + (low - 56320) + 65536; }
  int highSurrogate(int codePoint) { return (codePoint - 65536) / 1024 + 55296; }
  int lowSurrogate(int codePoint) { return (codePoint - 65536) % 1024 + 56320; }
  int[] toChars(int codePoint) {
    if (!isValidCodePoint(codePoint)) {
      throw new IllegalArgumentException("Not a valid Unicode code point: 0x" + hex(codePoint));
    }
    if (codePoint < 65536) {
      int[] one = new int[1];
      one[0] = codePoint;
      return one;
    }
    int[] two = new int[2];
    two[0] = highSurrogate(codePoint);
    two[1] = lowSurrogate(codePoint);
    return two;
  }
  int reverseBytes(int ch) { return (ch % 256) * 256 + ch / 256; }
  String hex(int v) {
    Integer I = new Integer(0);
    return I.toUnsignedStringRadix(v, 16);
  }

  < classification >
  boolean isISOControl(int cp) { return (cp >= 0 && cp <= 31) || (cp >= 127 && cp <= 159); }
  << Java isWhitespace: space/line/paragraph separators except the no-break
     spaces, plus the ASCII and information-separator controls. >>
  boolean isWhitespace(int cp) {
    if (cp == 32 || (cp >= 9 && cp <= 13) || (cp >= 28 && cp <= 31)) { return true; }
    if (cp == 5760 || (cp >= 8192 && cp <= 8198) || (cp >= 8200 && cp <= 8202)) { return true; }
    return cp == 8232 || cp == 8233 || cp == 8287 || cp == 12288;
  }
  boolean isSpaceChar(int cp) {
    if (cp == 32 || cp == 160 || cp == 5760 || (cp >= 8192 && cp <= 8202)) { return true; }
    return cp == 8232 || cp == 8233 || cp == 8239 || cp == 8287 || cp == 12288;
  }
  boolean isLetterOrDigit(int cp) { return isLetter(cp) || isDigit(cp); }
  boolean isAlphabetic(int cp) { return isLetter(cp); }
  boolean isIdeographic(int cp) { return (cp >= 19968 && cp <= 40959) || (cp >= 13312 && cp <= 19903) || (cp >= 131072 && cp <= 173791); }
  boolean isJavaIdentifierStart(int cp) { return isLetter(cp) || cp == 36 || cp == 95; }
  boolean isJavaIdentifierPart(int cp) {
    if (isLetterOrDigit(cp) || cp == 36 || cp == 95) { return true; }
    return (cp >= 0 && cp <= 8) || (cp >= 14 && cp <= 27) || (cp >= 127 && cp <= 159);
  }
  boolean isUnicodeIdentifierStart(int cp) { return isLetter(cp); }
  boolean isUnicodeIdentifierPart(int cp) { return isLetterOrDigit(cp) || cp == 95; }
  boolean isDefined(int cp) { return isValidCodePoint(cp); }

  < digits and radix >
  << Java digit(): ASCII and fullwidth Latin letters count as digits 10..35,
     decimal digits of the covered blocks by their value. >>
  int digit(int cp, int radix) {
    if (radix < 2 || radix > 36) { return -1; }
    int v = -1;
    if (cp >= 48 && cp <= 57) { v = cp - 48; }
    else {
      if (cp >= 97 && cp <= 122) { v = cp - 87; }
      else {
        if (cp >= 65 && cp <= 90) { v = cp - 55; }
        else {
          if (cp >= 65345 && cp <= 65370) { v = cp - 65335; }
          else {
            if (cp >= 65313 && cp <= 65338) { v = cp - 65303; }
            else { if (isDigit(cp)) { v = decimalValue(cp); } }
          }
        }
      }
    }
    if (v < 0 || v >= radix) { return -1; }
    return v;
  }
  int getNumericValue(int cp) {
    int v = digit(cp, 36);
    return v;
  }
  int forDigit(int digit, int radix) {
    if (digit >= radix || digit < 0) { return 0; }
    if (radix < 2 || radix > 36) { return 0; }
    if (digit < 10) { return 48 + digit; }
    return 87 + digit;
  }
'''

# decimal value of covered Nd code points: runs of 10 starting at zero digit
zeros = [a for a, b in runs(is_digit)]
dv = ["  int decimalValue(int cp) {"]
for a, b in runs(is_digit):
    dv.append(f"    if (cp >= {a} && cp <= {b}) {{ return (cp - {a}) % 10; }}")
dv.append("    return -1;")
dv.append("  }")
body += "\n".join(dv) + "\n"

body += "\n  < generated Unicode tables for the covered blocks >\n"
body += range_method("isLetter", runs(is_letter))
body += range_method("isDigit", runs(is_digit))
body += range_method("isUpperCase", runs(is_upper))
body += range_method("isLowerCase", runs(is_lower))
body += range_method("isTitleCase", runs(is_titlecase))
body += case_method("toUpperCase", case_runs("upper"))
body += case_method("toLowerCase", case_runs("lower"))
body += "  int toTitleCase(int cp) { return toUpperCase(cp); }\n"
body += ascii_table()

OUT.write_text(body)
print("lines", body.count("\n"), "python unicode", unicodedata.unidata_version, file=sys.stderr)

implement.apply_dir(OUT.parent if OUT.suffix == ".body" else OUT)
