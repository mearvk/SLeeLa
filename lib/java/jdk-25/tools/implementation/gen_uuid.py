#!/usr/bin/env python3
"""Emit the body of java.util.UUID: every public method of the JDK 28 API.

The VM has no bitwise operators, so all bit work is arithmetic: a 64-bit long
is split into two unsigned 32-bit halves (slHi / slLo) and put back together
with slMake. nameUUIDFromBytes needs MD5, which is written here as plain
SLeeLa arithmetic (RFC 1321) because /lib has no MD5 yet. randomUUID draws
from java.util.Random seeded from the VM clocks: it is NOT a cryptographically
secure source (Java uses SecureRandom), and the body says so.

Written from the JDK 28 API specification (Java Source in the unit header);
no Java source text is copied.
"""
import math
from pathlib import Path
import tempfile
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))

K = [int(abs(math.sin(i + 1)) * 4294967296) & 0xFFFFFFFF for i in range(64)]
S = [7, 12, 17, 22] * 4 + [5, 9, 14, 20] * 4 + [4, 11, 16, 23] * 4 + [6, 10, 15, 21] * 4
KINIT = "\n".join(f"    k[{i}] = {v};" for i, v in enumerate(K))
SINIT = "\n".join(f"    s[{i}] = {v};" for i, v in enumerate(S))

UUID = '''  < ---- java.util.UUID members (procedural implementation in SLeeLa) ---- >
  << A UUID is the pair of 64-bit values mostSigBits and leastSigBits. The VM
     has no bitwise operators, so every bit operation below is arithmetic on
     unsigned 32-bit halves. toString, fromString, version, variant, timestamp,
     clockSequence, node, hashCode, equals, compareTo and nameUUIDFromBytes
     (MD5, RFC 1321) are written from the JDK 28 API specification.
     randomUUID is seeded from the VM clocks through java.util.Random and is
     not a cryptographically secure source. >>

  long heldLeastSignificantBits;
  long heldMostSignificantBits;
  public UUID(long mostSigBits, long leastSigBits) { this.heldLeastSignificantBits = leastSigBits; this.heldMostSignificantBits = mostSigBits; }
  public long getLeastSignificantBits() { return this.heldLeastSignificantBits; }
  public long getMostSignificantBits() { return this.heldMostSignificantBits; }

  < the unsigned high and low 32-bit halves of a long, and the inverse >
  static long slLo(long v) {
    long lo = v % 4294967296;
    if (lo < 0) { lo = lo + 4294967296; }
    return lo;
  }
  static long slHi(long v) {
    long lo = slLo(v);
    long hi = (v - lo) / 4294967296;
    if (hi < 0) { hi = hi + 4294967296; }
    return hi;
  }
  static long slMake(long hiUnsigned, long loUnsigned) {
    long hi = hiUnsigned;
    if (hi >= 2147483648) { hi = hi - 4294967296; }
    return hi * 4294967296 + loUnsigned;
  }

  < 32-bit and, or, xor on unsigned values: op 0 = and, 1 = or, 2 = xor >
  static long slBit(long a, long b, int op) {
    long r = 0;
    long p = 1;
    int i = 0;
    while (i < 32) {
      long x = a % 2;
      long y = b % 2;
      long z = 0;
      if (op == 0) { z = x * y; }
      if (op == 1) { z = x + y; if (z > 1) { z = 1; } }
      if (op == 2) { z = (x + y) % 2; }
      r = r + z * p;
      p = p * 2;
      a = a / 2;
      b = b / 2;
      i = i + 1;
    }
    return r;
  }
  static long slNot(long a) { return 4294967295 - a; }
  static long slRotl(long x, int n) {
    long p = 1;
    int i = 0;
    while (i < n) { p = p * 2; i = i + 1; }
    long hi = x / (4294967296 / p);
    long lo = (x % (4294967296 / p)) * p;
    return lo + hi;
  }

  < lower-case hex digits of an unsigned value, always exactly width digits >
  static String slHex(long u, int width) {
    String digits = "0123456789abcdef";
    String out = "";
    int i = 0;
    while (i < width) {
      long d = u % 16;
      out = digits.substring(d, d + 1) + out;
      u = u / 16;
      i = i + 1;
    }
    return out;
  }

  < Long.parseLong(s, begin, end, 16) with its exact exception messages >
  static String slSegment(String s, int begin, int end) {
    return s.substring(begin, end);
  }
  static NumberFormatException slBadIndex(String s, int begin, int end, int at) {
    return new NumberFormatException("Error at index " + (at - begin) + " in: \\"" + slSegment(s, begin, end) + "\\"");
  }
  static long slDigit(int c) {
    if (c >= 48 && c <= 57) { return c - 48; }
    if (c >= 97 && c <= 102) { return c - 87; }
    if (c >= 65 && c <= 70) { return c - 55; }
    return -1;
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
      long digit = slDigit(s.charAt(i));
      i = i + 1;
      if (digit < 0 || result < multmin) { throw slBadIndex(s, begin, end, i - 1); }
      result = result * 16;
      if (result < limit + digit) { throw slBadIndex(s, begin, end, i - 1); }
      result = result - digit;
    }
    if (negative) { return result; }
    return -result;
  }
  static long slMod(long x, long m) {
    long r = x % m;
    if (r < 0) { r = r + m; }
    return r;
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
    long timeLow = slMod(slParseHex(name, 0, dash1), 4294967296);
    long timeMid = slMod(slParseHex(name, dash1 + 1, dash2), 65536);
    long timeHi = slMod(slParseHex(name, dash2 + 1, dash3), 65536);
    long clock = slMod(slParseHex(name, dash3 + 1, dash4), 65536);
    long node = slMod(slParseHex(name, dash4 + 1, len), 281474976710656);
    long msb = slMake(timeLow, timeMid * 65536 + timeHi);
    long lsb = slMake(clock * 65536 + node / 4294967296, node % 4294967296);
    return new UUID(msb, lsb);
  }

  public java.lang.String toString() {
    long mh = slHi(this.heldMostSignificantBits);
    long ml = slLo(this.heldMostSignificantBits);
    long lh = slHi(this.heldLeastSignificantBits);
    long ll = slLo(this.heldLeastSignificantBits);
    return slHex(mh, 8) + "-" + slHex(ml / 65536, 4) + "-" + slHex(ml % 65536, 4) + "-"
      + slHex(lh / 65536, 4) + "-" + slHex(lh % 65536, 4) + slHex(ll, 8);
  }

  public int version() {
    return slLo(this.heldMostSignificantBits) / 4096 % 16;
  }
  public int variant() {
    long lsb = this.heldLeastSignificantBits;
    if (lsb >= 0) { return 0; }
    long top3 = slHi(lsb) / 536870912;
    if (top3 >= 6) { return top3; }
    return 2;
  }
  public long timestamp() {
    if (version() != 1) { throw new UnsupportedOperationException("Not a time-based UUID"); }
    long ml = slLo(this.heldMostSignificantBits);
    long timeHi = ml % 4096;
    long timeMid = ml / 65536;
    long timeLow = slHi(this.heldMostSignificantBits);
    return timeHi * 281474976710656 + timeMid * 4294967296 + timeLow;
  }
  public int clockSequence() {
    if (version() != 1) { throw new UnsupportedOperationException("Not a time-based UUID"); }
    return slHi(this.heldLeastSignificantBits) / 65536 % 16384;
  }
  public long node() {
    if (version() != 1) { throw new UnsupportedOperationException("Not a time-based UUID"); }
    return slHi(this.heldLeastSignificantBits) % 65536 * 4294967296 + slLo(this.heldLeastSignificantBits);
  }

  public int hashCode() {
    long a = slBit(slHi(this.heldMostSignificantBits), slHi(this.heldLeastSignificantBits), 2);
    long b = slBit(slLo(this.heldMostSignificantBits), slLo(this.heldLeastSignificantBits), 2);
    long h = slBit(a, b, 2);
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
    long ml = slLo(msb);
    ml = ml - ml / 4096 % 16 * 4096 + 4 * 4096;
    long lh = slHi(lsb);
    lh = lh % 1073741824 + 2 * 1073741824;
    return new UUID(slMake(slHi(msb), ml), slMake(lh, slLo(lsb)));
  }

  < MD5 (RFC 1321) of a byte array: returns the 16 digest bytes as 0..255 >
  static int[] slMd5(byte[] data) {
    long[] k = new long[64];
    int[] s = new int[64];
''' + KINIT + '''
''' + SINIT + '''
    int n = arrayLength(data);
    int total = (n + 8) / 64 * 64 + 64;
    int[] buf = new int[total];
    < array elements start out null, which would turn the sums below into doubles >
    int i = 0;
    while (i < total) { buf[i] = 0; i = i + 1; }
    i = 0;
    while (i < n) { buf[i] = slMod(data[i], 256); i = i + 1; }
    buf[n] = 128;
    long bits = n * 8;
    i = 0;
    while (i < 8) { buf[total - 8 + i] = bits % 256; bits = bits / 256; i = i + 1; }
    long a0 = 1732584193;
    long b0 = 4023233417;
    long c0 = 2562383102;
    long d0 = 271733878;
    int blk = 0;
    while (blk < total) {
      long[] m = new long[16];
      int w = 0;
      while (w < 16) {
        int p = blk + w * 4;
        m[w] = buf[p] + buf[p + 1] * 256 + buf[p + 2] * 65536 + buf[p + 3] * 16777216;
        w = w + 1;
      }
      long a = a0;
      long b = b0;
      long c = c0;
      long d = d0;
      int r = 0;
      while (r < 64) {
        long f = 0;
        int g = 0;
        if (r < 16) { f = slBit(slBit(b, c, 0), slBit(slNot(b), d, 0), 1); g = r; }
        else if (r < 32) { f = slBit(slBit(d, b, 0), slBit(slNot(d), c, 0), 1); g = (5 * r + 1) % 16; }
        else if (r < 48) { f = slBit(slBit(b, c, 2), d, 2); g = (3 * r + 5) % 16; }
        else { f = slBit(c, slBit(b, slNot(d), 1), 2); g = 7 * r % 16; }
        f = (f + a + k[r] + m[g]) % 4294967296;
        a = d;
        d = c;
        c = b;
        b = (b + slRotl(f, s[r])) % 4294967296;
        r = r + 1;
      }
      a0 = (a0 + a) % 4294967296;
      b0 = (b0 + b) % 4294967296;
      c0 = (c0 + c) % 4294967296;
      d0 = (d0 + d) % 4294967296;
      blk = blk + 64;
    }
    int[] out = new int[16];
    long[] words = new long[4];
    words[0] = a0;
    words[1] = b0;
    words[2] = c0;
    words[3] = d0;
    int q = 0;
    while (q < 16) {
      long word = words[q / 4];
      int sh = q % 4;
      while (sh > 0) { word = word / 256; sh = sh - 1; }
      out[q] = word % 256;
      q = q + 1;
    }
    return out;
  }

  public static UUID nameUUIDFromBytes(byte[] name) {
    int[] md = slMd5(name);
    md[6] = md[6] % 16 + 48;
    md[8] = md[8] % 64 + 128;
    long mh = md[0] * 16777216 + md[1] * 65536 + md[2] * 256 + md[3];
    long ml = md[4] * 16777216 + md[5] * 65536 + md[6] * 256 + md[7];
    long lh = md[8] * 16777216 + md[9] * 65536 + md[10] * 256 + md[11];
    long ll = md[12] * 16777216 + md[13] * 65536 + md[14] * 256 + md[15];
    return new UUID(slMake(mh, ml), slMake(lh, ll));
  }
'''

(OUT / "java.util.UUID.body").write_text(UUID)
implement.apply_dir(OUT)
