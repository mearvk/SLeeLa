#!/usr/bin/env python3
"""Write the implementation fragments of java.lang.Short and java.lang.Byte.

usage: python3 lib/java/jdk-28/tools/gen_small_integrals.py
then   python3 lib/java/jdk-28/tools/put_impl.py java.lang.Short tools/impl/java.lang.Short.sleela-frag
       python3 lib/java/jdk-28/tools/drop_stubs.py java.lang.Short     (same for Byte)

Both types are the same shape: a boxed value narrowed to a fixed number of bits,
parse/decode/valueOf built on Integer with a range check, and the unsigned views.
The text is written from the JDK 28 API specification (Short, Byte)."""
from pathlib import Path

IMPL = Path(__file__).resolve().parent / "impl"

TEMPLATE = '''  << java.lang.@T@ in SLeeLa, written from the JDK 28 API specification
     (Java Source above). Java names, static modifiers and return types are
     kept. The VM has no @prim@ primitive: a @T@ value is an int held inside the
     Java range @MIN@..@MAX@, and slNarrow wraps any int back into it with the
     two's-complement rule Java applies on overflow. parse@T@, valueOf and decode
     go through Integer and then range-check, throwing NumberFormatException
     with the JDK messages. >>
  @prim@ value;

  public @T@(@prim@ value) { this.value = value; }
  @throwable("java.lang.NumberFormatException")
  public @T@(java.lang.String s) { this.value = parse@T@(s, 10); }

  static int slNarrow(long v) {
    long m = v % @MOD@;
    if (m < 0) { m = m + @MOD@; }
    if (m > @MAX@) { m = m - @MOD@; }
    return m;
  }
  static int slRange(int i, java.lang.String s, int radix) {
    if (i < @MIN@ || i > @MAX@) {
      throw new NumberFormatException("Value out of range. Value:\\"" + s + "\\" Radix:" + radix);
    }
    return i;
  }

  < instance members >
  public byte byteValue() { return Byte.slNarrow(value); }
  public short shortValue() { return slNarrowShort(value); }
  public int intValue() { return value; }
  public long longValue() { return value; }
  public float floatValue() { double d = value; return d; }
  public double doubleValue() { double d = value; return d; }
  public java.lang.String toString() { return Integer.toString(value); }
  public int hashCode() { return value; }
  public boolean equals(java.lang.Object obj) {
    if (obj instanceof @T@) { @T@ o = (@T@) obj; return o.value == value; }
    return false;
  }
  public int compareTo(java.lang.@T@ another@T@) { @T@ o = (@T@) another@T@; return compare(value, o.value); }

  < static members >
  public static java.lang.String toString(@prim@ x) { return Integer.toString(x); }
  public static int hashCode(@prim@ value) { return value; }
  public static int compare(@prim@ x, @prim@ y) { return x - y; }
  public static int compareUnsigned(@prim@ x, @prim@ y) { return toUnsignedInt(x) - toUnsignedInt(y); }
  public static int toUnsignedInt(@prim@ x) { return x & @UMASK@; }
  public static long toUnsignedLong(@prim@ x) { return x & @UMASK@; }
@REVERSE@
  @throwable("java.lang.NumberFormatException")
  public static @prim@ parse@T@(java.lang.String s, int radix) {
    int i = Integer.parseInt(s, radix);
    return slRange(i, s, radix);
  }
  @throwable("java.lang.NumberFormatException")
  public static @prim@ parse@T@(java.lang.String s) { return parse@T@(s, 10); }
  @throwable("java.lang.NumberFormatException")
  public static java.lang.@T@ valueOf(java.lang.String s, int radix) { return new @T@(parse@T@(s, radix)); }
  @throwable("java.lang.NumberFormatException")
  public static java.lang.@T@ valueOf(java.lang.String s) { return new @T@(parse@T@(s, 10)); }
  public static java.lang.@T@ valueOf(@prim@ x) { return new @T@(x); }
  @throwable("java.lang.NumberFormatException")
  public static java.lang.@T@ decode(java.lang.String nm) {
    Integer boxed = (Integer) Integer.decode(nm);
    int i = boxed.intValue();
    if (i < @MIN@ || i > @MAX@) {
      throw new NumberFormatException("Value " + i + " out of range from input " + nm);
    }
    return new @T@(i);
  }
'''

REVERSE_SHORT = '''  public static short reverseBytes(short i) {
    int lo = i & 255;
    int hi = (i >> 8) & 255;
    return slNarrowShort(lo * 256 + hi);
  }
  static int slNarrowShort(long v) {
    long m = v % 65536;
    if (m < 0) { m = m + 65536; }
    if (m > 32767) { m = m - 65536; }
    return m;
  }'''
EXTRA_BYTE = '''  static int slNarrowShort(long v) { return v; }'''


def build(t, prim, bits):
    mod = 1 << bits
    mx = mod // 2 - 1
    text = TEMPLATE
    rev = REVERSE_SHORT if t == "Short" else EXTRA_BYTE
    for k, v in {"@REVERSE@": rev, "@T@": t, "@prim@": prim, "@MIN@": str(-mod // 2),
                 "@MAX@": str(mx), "@MOD@": str(mod), "@UMASK@": str(mod - 1)}.items():
        text = text.replace(k, v)
    if t == "Byte":
        text = text.replace("Byte.slNarrow(value)", "value")
    if t == "Short":
        text = text.replace("public byte byteValue() { return value; }", "")
    return text


def main():
    IMPL.mkdir(exist_ok=True)
    short = build("Short", "short", 16).replace("public byte byteValue() { return Byte.slNarrow(value); }",
                                                 "public byte byteValue() { long m = value % 256; if (m < 0) { m = m + 256; } if (m > 127) { m = m - 256; } return m; }")
    (IMPL / "java.lang.Short.sleela-frag").write_text(short)
    (IMPL / "java.lang.Byte.sleela-frag").write_text(build("Byte", "byte", 8))
    print("wrote Short and Byte fragments")


main()
