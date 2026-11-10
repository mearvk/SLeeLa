#!/usr/bin/env python3
"""Emit bodies for java.lang.Integer / Long / Short / Byte."""
from pathlib import Path
import tempfile
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))

SPEC = {
    # name: (prim, bits, min literal expr, max literal, java fq)
    "Integer": ("int", 32, "-2147483648", "2147483647"),
    "Long": ("long", 64, "-9223372036854775807 - 1", "9223372036854775807"),
    "Short": ("short", 16, "-32768", "32767"),
    "Byte": ("byte", 8, "-128", "127"),
}

COMMON_PARSE = '''
  << Digit value of UTF-16 code unit c in the given radix, or -1. >>
  int digitOf(int c, int radix) {
    int d = -1;
    if (c >= 48 && c <= 57) { d = c - 48; }
    if (c >= 97 && c <= 122) { d = c - 87; }
    if (c >= 65 && c <= 90) { d = c - 55; }
    if (d >= radix) { return -1; }
    return d;
  }
  << A single digit character for value d (0..35), lower case as in Java. >>
  String digitChar(int d) {
    if (d < 10) { return "" + d; }
    if (d == 10) { return "a"; } if (d == 11) { return "b"; } if (d == 12) { return "c"; }
    if (d == 13) { return "d"; } if (d == 14) { return "e"; } if (d == 15) { return "f"; }
    if (d == 16) { return "g"; } if (d == 17) { return "h"; } if (d == 18) { return "i"; }
    if (d == 19) { return "j"; } if (d == 20) { return "k"; } if (d == 21) { return "l"; }
    if (d == 22) { return "m"; } if (d == 23) { return "n"; } if (d == 24) { return "o"; }
    if (d == 25) { return "p"; } if (d == 26) { return "q"; } if (d == 27) { return "r"; }
    if (d == 28) { return "s"; } if (d == 29) { return "t"; } if (d == 30) { return "u"; }
    if (d == 31) { return "v"; } if (d == 32) { return "w"; } if (d == 33) { return "x"; }
    if (d == 34) { return "y"; }
    return "z";
  }
  NumberFormatException forInputString(String s, int radix) {
    if (radix == 10) { return new NumberFormatException("For input string: \\"" + s + "\\""); }
    return new NumberFormatException("For input string: \\"" + s + "\\" under radix " + radix);
  }
'''


def body(name, prim, bits, lo, hi):
    pname = "Int" if name == "Integer" else name
    T = "long" if bits == 64 else "int"
    unsigned_mod = {8: "256", 16: "65536", 32: "4294967296"}.get(bits)
    out = []
    out.append(f'''  < ---- java.lang.{name} members (procedural implementation in SLeeLa) ---- >
  << The boxed {prim} value plus the java.lang.{name} static API rendered as
     instance methods (call as new {name}(0).parse{pname}("42")). The VM integer
     is 64-bit, so every {bits}-bit result is wrapped two-complement style with
     wrap(), exactly as Java arithmetic overflows. Parsing reads the text as a
     Java UTF-16 String (javaStringFromUtf8), so pass an ordinary SLeeLa String.
     Overloads are suffixed (toStringOf, toStringRadix, parse{pname}Radix, ...)
     because calls resolve by name only. The VM has no byte/short/float/char
     primitive, so those Java types are carried as int (or double for float),
     always held inside the Java range. >>
  static final {T} MIN_VALUE = {lo};
  static final {T} MAX_VALUE = {hi};
  static final int SIZE = {bits};
  static final int BYTES = {bits // 8};

  {T} value;
  {name}({T} v) {{ value = wrap(v); }}

  {T} minValue() {{ return MIN_VALUE; }}
  {T} maxValue() {{ return MAX_VALUE; }}
''')
    if bits == 64:
        out.append('''  << The VM integer is already 64-bit two-complement, so wrap is the identity. >>
  long wrap(long v) { return v; }
''')
    else:
        out.append(f'''  << Reduce any 64-bit value to the {bits}-bit two-complement range. >>
  int wrap(long v) {{
    long m = v % {unsigned_mod};
    if (m < 0) {{ m = m + {unsigned_mod}; }}
    if (m > {hi}) {{ m = m - {unsigned_mod}; }}
    return (int) m;
  }}
''')
    out.append(f'''
  < instance accessors (java.lang.Number) >
  int byteValue() {{ return (int) wrapBits(value, 8); }}
  int shortValue() {{ return (int) wrapBits(value, 16); }}
  int intValue() {{ return (int) wrapBits(value, 32); }}
  long longValue() {{ return value; }}
  double doubleValue() {{ double d = value; return d; }}
  double floatValue() {{ double d = value; return d; }}
  long wrapBits(long v, int nbits) {{
    if (nbits >= 64) {{ return v; }}
    long mod = 1;
    int i = 0;
    while (i < nbits) {{ mod = mod * 2; i = i + 1; }}
    long half = mod / 2;
    long m = v % mod;
    if (m < 0) {{ m = m + mod; }}
    if (m >= half) {{ m = m - mod; }}
    return m;
  }}
  String toString() {{ return toStringOf(value); }}
  int hashCode() {{ return hashCodeOf(value); }}
  boolean equals(Any obj) {{
    if (obj instanceof {name}) {{
      {name} o = ({name}) obj;
      return o.value == value;
    }}
    return false;
  }}
  int compareTo({name} another) {{ return compare(value, another.value); }}

  < static API >
  int compare({T} x, {T} y) {{
    if (x < y) {{ return -1; }}
    if (x == y) {{ return 0; }}
    return 1;
  }}
  int signum({T} i) {{
    if (i > 0) {{ return 1; }}
    if (i < 0) {{ return -1; }}
    return 0;
  }}
  {T} sum({T} a, {T} b) {{ return wrap(a + b); }}
  {T} max({T} a, {T} b) {{ if (a >= b) {{ return a; }} return b; }}
  {T} min({T} a, {T} b) {{ if (a <= b) {{ return a; }} return b; }}
  {name} valueOf({T} i) {{ return new {name}(i); }}
  {name} valueOfString(String s) {{ return new {name}(parse{pname}(s)); }}
''')
    if bits == 64:
        out.append('''  int hashCodeOf(long v) {
    long hi = v / 4294967296;
    long lo = v % 4294967296;
    if (lo < 0) { lo = lo + 4294967296; hi = hi - 1; }
    long h = xor32(lo, hi);
    if (h > 2147483647) { h = h - 4294967296; }
    return (int) h;
  }
  << Bitwise xor of two unsigned 32-bit values, by binary digits. >>
  long xor32(long a, long b) {
    long r = 0;
    long bit = 1;
    long x = a;
    long y = b;
    if (x < 0) { x = x + 4294967296; }
    if (y < 0) { y = y + 4294967296; }
    int i = 0;
    while (i < 32) {
      if ((x % 2) != (y % 2)) { r = r + bit; }
      x = x / 2;
      y = y / 2;
      bit = bit * 2;
      i = i + 1;
    }
    return r;
  }
''')
    else:
        out.append(f'''  int hashCodeOf({T} v) {{ return (int) v; }}
''')
    # toString family
    out.append(f'''
  String toStringOf({T} i) {{ return toStringRadix(i, 10); }}
  String toStringRadix({T} i, int radix) {{
    int r = radix;
    if (r < 2 || r > 36) {{ r = 10; }}
    if (i == 0) {{ return "0"; }}
    boolean neg = i < 0;
    String out = "";
    {T} v = i;
    while (v != 0) {{
      {T} q = v / r;
      {T} d = v - q * r;
      if (d < 0) {{ d = 0 - d; }}
      out = digitChar((int) d) + out;
      v = q;
    }}
    if (neg) {{ return "-" + out; }}
    return out;
  }}
''')
    if bits in (32, 64):
        U = "long"
        if bits == 32:
            out.append('''  << The unsigned 32-bit value of i as a long (Integer.toUnsignedLong). >>
  long toUnsignedLong(int i) {
    long v = i;
    if (v < 0) { v = v + 4294967296; }
    return v;
  }
  String toUnsignedStringRadix(int i, int radix) {
    long v = toUnsignedLong(i);
    int r = radix;
    if (r < 2 || r > 36) { r = 10; }
    if (v == 0) { return "0"; }
    String out = "";
    while (v > 0) { out = digitChar((int) (v % r)) + out; v = v / r; }
    return out;
  }
  String toUnsignedString(int i) { return toUnsignedStringRadix(i, 10); }
  String toHexString(int i) { return toUnsignedStringRadix(i, 16); }
  String toOctalString(int i) { return toUnsignedStringRadix(i, 8); }
  String toBinaryString(int i) { return toUnsignedStringRadix(i, 2); }
  int compareUnsigned(int x, int y) { return compare(wrap(x + MIN_VALUE), wrap(y + MIN_VALUE)); }
  int divideUnsigned(int dividend, int divisor) { return wrap(toUnsignedLong(dividend) / toUnsignedLong(divisor)); }
  int remainderUnsigned(int dividend, int divisor) { return wrap(toUnsignedLong(dividend) % toUnsignedLong(divisor)); }
  int parseUnsignedInt(String s) { return wrap(parseUnsignedLongValue(s, 10, 4294967295)); }

  < bit twiddling over the unsigned 32-bit pattern >
  int bitCount(int i) {
    long v = toUnsignedLong(i);
    int n = 0;
    while (v > 0) { n = n + (int) (v % 2); v = v / 2; }
    return n;
  }
  int numberOfLeadingZeros(int i) {
    long v = toUnsignedLong(i);
    int n = 32;
    while (v > 0) { v = v / 2; n = n - 1; }
    return n;
  }
  int numberOfTrailingZeros(int i) {
    long v = toUnsignedLong(i);
    if (v == 0) { return 32; }
    int n = 0;
    while (v % 2 == 0) { v = v / 2; n = n + 1; }
    return n;
  }
  int highestOneBit(int i) {
    long v = toUnsignedLong(i);
    if (v == 0) { return 0; }
    long b = 1;
    while (b * 2 <= v) { b = b * 2; }
    return wrap(b);
  }
  int lowestOneBit(int i) {
    long v = toUnsignedLong(i);
    if (v == 0) { return 0; }
    long b = 1;
    while (v % 2 == 0) { v = v / 2; b = b * 2; }
    return wrap(b);
  }
  int rotateLeft(int i, int distance) {
    int d = distance % 32;
    if (d < 0) { d = d + 32; }
    long v = toUnsignedLong(i);
    int k = 0;
    while (k < d) {
      long top = v / 2147483648;
      v = (v % 2147483648) * 2 + top;
      k = k + 1;
    }
    return wrap(v);
  }
  int rotateRight(int i, int distance) { return rotateLeft(i, 32 - (distance % 32)); }
  int reverse(int i) {
    long v = toUnsignedLong(i);
    long r = 0;
    int k = 0;
    while (k < 32) { r = r * 2 + v % 2; v = v / 2; k = k + 1; }
    return wrap(r);
  }
  int reverseBytes(int i) {
    long v = toUnsignedLong(i);
    long r = 0;
    int k = 0;
    while (k < 4) { r = r * 256 + v % 256; v = v / 256; k = k + 1; }
    return wrap(r);
  }
''')
        else:
            out.append('''  String toHexString(long i) { return toUnsignedStringRadix(i, 16); }
  String toOctalString(long i) { return toUnsignedStringRadix(i, 8); }
  String toBinaryString(long i) { return toUnsignedStringRadix(i, 2); }
  String toUnsignedString(long i) { return toUnsignedStringRadix(i, 10); }
  << Unsigned 64-bit rendering: split off the low digit of the unsigned value
     without overflowing the signed VM integer. >>
  String toUnsignedStringRadix(long i, int radix) {
    int r = radix;
    if (r < 2 || r > 36) { r = 10; }
    if (i >= 0) { return toStringRadix(i, r); }
    long shifted = (i - MIN_VALUE) / 2 + 4611686018427387904;
    long q = shifted / r * 2;
    long rem = i - q * r;
    if (rem < 0 || rem >= r) { q = q + 1; rem = rem - r; }
    return toStringRadix(q, r) + digitChar((int) rem);
  }
  int bitCount(long i) {
    int n = 0;
    long v = i;
    if (v < 0) { n = 1; v = v - MIN_VALUE; }
    while (v > 0) { n = n + (int) (v % 2); v = v / 2; }
    return n;
  }
  int numberOfLeadingZeros(long i) {
    if (i < 0) { return 0; }
    long v = i;
    int n = 64;
    while (v > 0) { v = v / 2; n = n - 1; }
    return n;
  }
  int numberOfTrailingZeros(long i) {
    if (i == 0) { return 64; }
    long v = i;
    int n = 0;
    while (v % 2 == 0) { v = v / 2; n = n + 1; }
    return n;
  }
  long highestOneBit(long i) {
    if (i < 0) { return MIN_VALUE; }
    if (i == 0) { return 0; }
    long b = 1;
    while (b <= i / 2) { b = b * 2; }
    return b;
  }
  long lowestOneBit(long i) {
    if (i == 0) { return 0; }
    long v = i;
    long b = 1;
    while (v % 2 == 0) { v = v / 2; b = b * 2; }
    return b;
  }
  int compareUnsigned(long x, long y) { return compare(x + MIN_VALUE, y + MIN_VALUE); }
''')
    # parsing
    out.append(f'''
  {T} parse{pname}(String s) {{ return parse{pname}Radix(s, 10); }}
  {T} parse{pname}Radix(String s, int radix) {{
    if (s == null) {{ throw new NumberFormatException("Cannot parse null string: null"); }}
    if (radix < 2 || radix > 36) {{ throw new NumberFormatException("radix " + radix + " out of range"); }}
    String u = javaStringFromUtf8(s);
    int n = javaStringLength(u);
    if (n == 0) {{ throw new NumberFormatException("For input string: \\"\\"" ); }}
    int i = 0;
    boolean neg = false;
    int first = javaStringCharAt(u, 0);
    if (first == 45 || first == 43) {{
      neg = first == 45;
      i = 1;
      if (n == 1) {{ throw forInputString(s, radix); }}
    }}
    {T} limit = 0 - MAX_VALUE;
    if (neg) {{ limit = MIN_VALUE; }}
    {T} multmin = limit / radix;
    {T} result = 0;
    while (i < n) {{
      int d = digitOf(javaStringCharAt(u, i), radix);
      if (d < 0 || result < multmin) {{ throw forInputString(s, radix); }}
      result = result * radix;
      if (result < limit + d) {{ throw forInputString(s, radix); }}
      result = result - d;
      i = i + 1;
    }}
    if (neg) {{ return result; }}
    return 0 - result;
  }}
  {name} decode(String nm) {{
    String u = javaStringFromUtf8(nm);
    int n = javaStringLength(u);
    if (n == 0) {{ throw new NumberFormatException("Zero length string"); }}
    int i = 0;
    boolean neg = false;
    int c0 = javaStringCharAt(u, 0);
    if (c0 == 45 || c0 == 43) {{ neg = c0 == 45; i = 1; }}
    int radix = 10;
    if (i + 1 < n && javaStringCharAt(u, i) == 48 && (javaStringCharAt(u, i + 1) == 120 || javaStringCharAt(u, i + 1) == 88)) {{ radix = 16; i = i + 2; }}
    else {{ if (i < n && javaStringCharAt(u, i) == 35) {{ radix = 16; i = i + 1; }}
    else {{ if (i + 1 < n && javaStringCharAt(u, i) == 48) {{ radix = 8; i = i + 1; }} }} }}
    {T} mag = 0;
    if (i >= n) {{ throw forInputString(nm, radix); }}
    while (i < n) {{
      int d = digitOf(javaStringCharAt(u, i), radix);
      if (d < 0) {{ throw forInputString(nm, radix); }}
      mag = mag * radix - d;
      i = i + 1;
    }}
    if (neg) {{ return new {name}(mag); }}
    if (mag == MIN_VALUE) {{ throw forInputString(nm, radix); }}
    return new {name}(0 - mag);
  }}
''')
    if bits == 32:
        out.append('''  << Parse an unsigned magnitude no larger than max (used by parseUnsignedInt). >>
  long parseUnsignedLongValue(String s, int radix, long max) {
    String u = javaStringFromUtf8(s);
    int n = javaStringLength(u);
    if (n == 0) { throw forInputString(s, radix); }
    int i = 0;
    if (javaStringCharAt(u, 0) == 43) { i = 1; }
    if (javaStringCharAt(u, 0) == 45) {
      throw new NumberFormatException("Illegal leading minus sign on unsigned string " + s + ".");
    }
    long r = 0;
    while (i < n) {
      int d = digitOf(javaStringCharAt(u, i), radix);
      if (d < 0) { throw forInputString(s, radix); }
      r = r * radix + d;
      if (r > max) {
        throw new NumberFormatException("String value " + s + " exceeds range of unsigned int.");
      }
      i = i + 1;
    }
    return r;
  }
''')
    if bits in (8, 16):
        out.append(f'''  int toUnsignedInt({T} x) {{
    int v = (int) x;
    if (v < 0) {{ v = v + {unsigned_mod}; }}
    return v;
  }}
  long toUnsignedLong({T} x) {{
    long v = x;
    if (v < 0) {{ v = v + {unsigned_mod}; }}
    return v;
  }}
  int compareUnsigned({T} x, {T} y) {{ return toUnsignedInt(x) - toUnsignedInt(y); }}
''')
    if bits == 16:
        out.append('''  int reverseBytes(int i) {
    int v = toUnsignedInt(i);
    return wrap((v % 256) * 256 + v / 256);
  }
''')
    out.append(COMMON_PARSE)
    return "@extends Number\n" + "".join(out)


for name, (prim, bits, lo, hi) in SPEC.items():
    (OUT / f"java.lang.{name}.body").write_text(body(name, prim, bits, lo, hi))
    print("wrote", name)

implement.apply_dir(OUT.parent if OUT.suffix == ".body" else OUT)
