#!/usr/bin/env python3
"""Emit the body of java.util.UUID: every public method of the JDK 28 API.

The VM has no bitwise operators, so all bit work is arithmetic: a 64-bit long
is split into two unsigned 32-bit halves (slHi / slLo) and put back together
with SLJavaBits.make. The bit arithmetic, hex text and MD5 live in the shared
/lib/java/support classes (SLJavaBits, SLJavaHex, SLJavaMd5). randomUUID draws
from java.util.Random seeded from the VM clocks: it is NOT a cryptographically
secure source (Java uses SecureRandom), and the body says so.

Written from the JDK 28 API specification (Java Source in the unit header);
no Java source text is copied.
"""
from pathlib import Path
import tempfile
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))


UUID = '''  < ---- java.util.UUID members (procedural implementation in SLeeLa) ---- >
  << A UUID is the pair of 64-bit values mostSigBits and leastSigBits. The VM
     has no bitwise operators, so every bit operation goes through the shared
     /lib/java/support classes (arithmetic on unsigned 32-bit halves). toString, fromString, version, variant, timestamp,
     clockSequence, node, hashCode, equals, compareTo and nameUUIDFromBytes
     (MD5, RFC 1321) are written from the JDK 28 API specification.
     randomUUID is seeded from the VM clocks through java.util.Random and is
     not a cryptographically secure source. >>

  long heldLeastSignificantBits;
  long heldMostSignificantBits;
  public UUID(long mostSigBits, long leastSigBits) { this.heldLeastSignificantBits = leastSigBits; this.heldMostSignificantBits = mostSigBits; }
  public long getLeastSignificantBits() { return this.heldLeastSignificantBits; }
  public long getMostSignificantBits() { return this.heldMostSignificantBits; }

  < Long.parseLong(s, begin, end, 16) with its exact exception messages >
  static String slSegment(String s, int begin, int end) {
    return s.substring(begin, end);
  }
  static NumberFormatException slBadIndex(String s, int begin, int end, int at) {
    return new NumberFormatException("Error at index " + (at - begin) + " in: \\"" + slSegment(s, begin, end) + "\\"");
  }
  static long slParseHex(String s, int begin, int end) {
    if (begin == end) { throw new NumberFormatException("For input string: \\"\\" under radix 16"); }
    int i = begin;
    long limit = -9223372036854775807;
    boolean negative = false;
    int first = s.charAt(i);
    if (first < 48) {
      if (first == 45) { negative = true; limit = -9223372036854775807 - 1; i = i + 1; }
      else if (first == 43) { i = i + 1; }
      else { throw slBadIndex(s, begin, end, i); }
      if (i == end) { throw slBadIndex(s, begin, end, i); }
    }
    long multmin = limit / 16;
    long result = 0;
    while (i < end) {
      long digit = SLJavaHex.digit(s.charAt(i));
      i = i + 1;
      if (digit < 0 || result < multmin) { throw slBadIndex(s, begin, end, i - 1); }
      result = result * 16;
      if (result < limit + digit) { throw slBadIndex(s, begin, end, i - 1); }
      result = result - digit;
    }
    if (negative) { return result; }
    return -result;
  }
  public static UUID fromString(java.lang.String name) {
    int len = name.length();
    if (len > 36) { throw new IllegalArgumentException("UUID string too large"); }
    int dash1 = name.indexOf(45, 0);
    int dash2 = name.indexOf(45, dash1 + 1);
    int dash3 = name.indexOf(45, dash2 + 1);
    int dash4 = name.indexOf(45, dash3 + 1);
    int dash5 = name.indexOf(45, dash4 + 1);
    if (dash4 < 0 || dash5 >= 0) { throw new IllegalArgumentException("Invalid UUID string: " + name); }
    long timeLow = SLJavaBits.mod(slParseHex(name, 0, dash1), 4294967296);
    long timeMid = SLJavaBits.mod(slParseHex(name, dash1 + 1, dash2), 65536);
    long timeHi = SLJavaBits.mod(slParseHex(name, dash2 + 1, dash3), 65536);
    long clock = SLJavaBits.mod(slParseHex(name, dash3 + 1, dash4), 65536);
    long node = SLJavaBits.mod(slParseHex(name, dash4 + 1, len), 281474976710656);
    long msb = SLJavaBits.make(timeLow, timeMid * 65536 + timeHi);
    long lsb = SLJavaBits.make(clock * 65536 + node / 4294967296, node % 4294967296);
    return new UUID(msb, lsb);
  }

  public java.lang.String toString() {
    long mh = SLJavaBits.hi(this.heldMostSignificantBits);
    long ml = SLJavaBits.lo(this.heldMostSignificantBits);
    long lh = SLJavaBits.hi(this.heldLeastSignificantBits);
    long ll = SLJavaBits.lo(this.heldLeastSignificantBits);
    return SLJavaHex.hex(mh, 8) + "-" + SLJavaHex.hex(ml / 65536, 4) + "-" + SLJavaHex.hex(ml % 65536, 4) + "-"
      + SLJavaHex.hex(lh / 65536, 4) + "-" + SLJavaHex.hex(lh % 65536, 4) + SLJavaHex.hex(ll, 8);
  }

  public int version() {
    return SLJavaBits.lo(this.heldMostSignificantBits) / 4096 % 16;
  }
  public int variant() {
    long lsb = this.heldLeastSignificantBits;
    if (lsb >= 0) { return 0; }
    long top3 = SLJavaBits.hi(lsb) / 536870912;
    if (top3 >= 6) { return top3; }
    return 2;
  }
  public long timestamp() {
    if (version() != 1) { throw new UnsupportedOperationException("Not a time-based UUID"); }
    long ml = SLJavaBits.lo(this.heldMostSignificantBits);
    long timeHi = ml % 4096;
    long timeMid = ml / 65536;
    long timeLow = SLJavaBits.hi(this.heldMostSignificantBits);
    return timeHi * 281474976710656 + timeMid * 4294967296 + timeLow;
  }
  public int clockSequence() {
    if (version() != 1) { throw new UnsupportedOperationException("Not a time-based UUID"); }
    return SLJavaBits.hi(this.heldLeastSignificantBits) / 65536 % 16384;
  }
  public long node() {
    if (version() != 1) { throw new UnsupportedOperationException("Not a time-based UUID"); }
    return SLJavaBits.hi(this.heldLeastSignificantBits) % 65536 * 4294967296 + SLJavaBits.lo(this.heldLeastSignificantBits);
  }

  public int hashCode() {
    long a = SLJavaBits.bit(SLJavaBits.hi(this.heldMostSignificantBits), SLJavaBits.hi(this.heldLeastSignificantBits), 2);
    long b = SLJavaBits.bit(SLJavaBits.lo(this.heldMostSignificantBits), SLJavaBits.lo(this.heldLeastSignificantBits), 2);
    long h = SLJavaBits.bit(a, b, 2);
    if (h >= 2147483648) { h = h - 4294967296; }
    return h;
  }
  public boolean equals(java.lang.Object obj) {
    if (obj == null) { return false; }
    if (!(obj instanceof UUID)) { return false; }
    UUID o = (UUID) obj;
    return this.heldMostSignificantBits == o.heldMostSignificantBits && this.heldLeastSignificantBits == o.heldLeastSignificantBits;
  }
  public int compareTo(UUID val) {
    if (val == null) { throw new NullPointerException(); }
    if (this.heldMostSignificantBits < val.heldMostSignificantBits) { return -1; }
    if (this.heldMostSignificantBits > val.heldMostSignificantBits) { return 1; }
    if (this.heldLeastSignificantBits < val.heldLeastSignificantBits) { return -1; }
    if (this.heldLeastSignificantBits > val.heldLeastSignificantBits) { return 1; }
    return 0;
  }

  < a counter keeps two calls in the same clock tick distinct >
  static long slRandomCounter = 0;
  public static UUID randomUUID() {
    slRandomCounter = slRandomCounter + 1;
    long seed = timeUtcMillis() * 1000003 + timeMonotonicNanos() % 1000003 * 7919 + slRandomCounter * 104729;
    Random rng = new Random(seed);
    long msb = rng.nextLong();
    long lsb = rng.nextLong();
    long ml = SLJavaBits.lo(msb);
    ml = ml - ml / 4096 % 16 * 4096 + 4 * 4096;
    long lh = SLJavaBits.hi(lsb);
    lh = lh % 1073741824 + 2 * 1073741824;
    return new UUID(SLJavaBits.make(SLJavaBits.hi(msb), ml), SLJavaBits.make(lh, SLJavaBits.lo(lsb)));
  }

  public static UUID nameUUIDFromBytes(byte[] name) {
    int[] md = SLJavaMd5.digest(name);
    md[6] = md[6] % 16 + 48;
    md[8] = md[8] % 64 + 128;
    long mh = md[0] * 16777216 + md[1] * 65536 + md[2] * 256 + md[3];
    long ml = md[4] * 16777216 + md[5] * 65536 + md[6] * 256 + md[7];
    long lh = md[8] * 16777216 + md[9] * 65536 + md[10] * 256 + md[11];
    long ll = md[12] * 16777216 + md[13] * 65536 + md[14] * 256 + md[15];
    return new UUID(SLJavaBits.make(mh, ml), SLJavaBits.make(lh, ll));
  }
'''

(OUT / "java.util.UUID.body").write_text(UUID)
implement.apply_dir(OUT)
