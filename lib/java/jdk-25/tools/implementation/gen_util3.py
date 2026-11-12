#!/usr/bin/env python3
"""Emit bodies for java.util.concurrent.atomic, the summary statistics,
java.util.zip checksums, java.util.BitSet and java.lang.StackTraceElement."""
from pathlib import Path
import tempfile
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))

LOCK = 31  # VM lock id reserved for java.util.concurrent.atomic

WRAP32 = '''  int wrap32(long v) {
    long m = v % 4294967296;
    if (m < 0) { m = m + 4294967296; }
    if (m > 2147483647) { m = m - 4294967296; }
    return (int) m;
  }
'''


def atomic_scalar(name, T, wrap):
    w = (lambda e: f"wrap32({e})") if wrap else (lambda e: e)
    num = "" if T in ("boolean", "Any") else f'''
  < java.lang.Number views >
  int intValue() {{ return wrap32(get()); }}
  long longValue() {{ long v = get(); return v; }}
  double doubleValue() {{ double d = get(); return d; }}
  double floatValue() {{ Float f = new Float(doubleValue()); return f.doubleValue(); }}
  int byteValue() {{ Integer i = new Integer(0); return (int) i.wrapBits(get(), 8); }}
  int shortValue() {{ Integer i = new Integer(0); return (int) i.wrapBits(get(), 16); }}
'''
    arith = "" if T in ("boolean", "Any") else f'''
  {T} getAndIncrement() {{ return getAndAdd(1); }}
  {T} getAndDecrement() {{ return getAndAdd(-1); }}
  {T} getAndAdd({T} delta) {{
    lock({LOCK});
    {T} old = this.value;
    this.value = {w("old + delta")};
    unlock({LOCK});
    return old;
  }}
  {T} incrementAndGet() {{ return addAndGet(1); }}
  {T} decrementAndGet() {{ return addAndGet(-1); }}
  {T} addAndGet({T} delta) {{
    lock({LOCK});
    this.value = {w("this.value + delta")};
    {T} now = this.value;
    unlock({LOCK});
    return now;
  }}
'''
    zero = {"int": "0", "long": "0", "boolean": "false", "Any": "null"}[T]
    text = '"" + v' if T != "Any" else 'if (v == null) { return "null"; } return "" + v'
    tostr = f"  String toString() {{ {T} v = get(); return \"\" + v; }}" if T != "Any" else \
        '  String toString() { Any v = get(); if (v == null) { return "null"; } return "" + v; }'
    return f'''  < ---- java.util.concurrent.atomic.{name} members (procedural implementation in SLeeLa) ---- >
  << A {T} value updated atomically: every read-modify-write runs under VM
     lock {LOCK} (reserved for java.util.concurrent.atomic), so updates from
     spawned threads never interleave. The plain / opaque / acquire / release
     access modes are all sequentially consistent here, which satisfies each
     of their Java contracts. compareAndSet compares with == (value equality
     for numbers, identity for objects). The functional update methods wait
     for lambda dispatch. >>
  {T} value;
  {name}({T} initialValue) {{ this.value = initialValue; }}

  {T} get() {{ return this.value; }}
  void set({T} newValue) {{ lock({LOCK}); this.value = newValue; unlock({LOCK}); }}
  void lazySet({T} newValue) {{ set(newValue); }}
  {T} getPlain() {{ return this.value; }}
  void setPlain({T} newValue) {{ this.value = newValue; }}
  {T} getOpaque() {{ return get(); }}
  void setOpaque({T} newValue) {{ set(newValue); }}
  {T} getAcquire() {{ return get(); }}
  void setRelease({T} newValue) {{ set(newValue); }}
  {T} getAndSet({T} newValue) {{
    lock({LOCK});
    {T} old = this.value;
    this.value = newValue;
    unlock({LOCK});
    return old;
  }}
  boolean compareAndSet({T} expectedValue, {T} newValue) {{
    lock({LOCK});
    boolean hit = this.value == expectedValue;
    if (hit) {{ this.value = newValue; }}
    unlock({LOCK});
    return hit;
  }}
  boolean weakCompareAndSet({T} expectedValue, {T} newValue) {{ return compareAndSet(expectedValue, newValue); }}
  boolean weakCompareAndSetPlain({T} expectedValue, {T} newValue) {{ return compareAndSet(expectedValue, newValue); }}
  boolean weakCompareAndSetVolatile({T} expectedValue, {T} newValue) {{ return compareAndSet(expectedValue, newValue); }}
  boolean weakCompareAndSetAcquire({T} expectedValue, {T} newValue) {{ return compareAndSet(expectedValue, newValue); }}
  boolean weakCompareAndSetRelease({T} expectedValue, {T} newValue) {{ return compareAndSet(expectedValue, newValue); }}
  {T} compareAndExchange({T} expectedValue, {T} newValue) {{
    lock({LOCK});
    {T} witness = this.value;
    if (witness == expectedValue) {{ this.value = newValue; }}
    unlock({LOCK});
    return witness;
  }}
  {T} compareAndExchangeAcquire({T} expectedValue, {T} newValue) {{ return compareAndExchange(expectedValue, newValue); }}
  {T} compareAndExchangeRelease({T} expectedValue, {T} newValue) {{ return compareAndExchange(expectedValue, newValue); }}
{arith}{tostr}
{num}''' + (WRAP32 if T != "Any" and T != "boolean" else "")


def atomic_array(name, T, wrap):
    w = (lambda e: f"wrap32({e})") if wrap else (lambda e: e)
    zero = {"int": "0", "long": "0", "Any": "null"}[T]
    arith = "" if T == "Any" else f'''
  {T} getAndIncrement(int i) {{ return getAndAdd(i, 1); }}
  {T} getAndDecrement(int i) {{ return getAndAdd(i, -1); }}
  {T} getAndAdd(int i, {T} delta) {{
    check(i);
    lock({LOCK});
    {T} old = this.array[i];
    this.array[i] = {w("old + delta")};
    unlock({LOCK});
    return old;
  }}
  {T} incrementAndGet(int i) {{ return addAndGet(i, 1); }}
  {T} decrementAndGet(int i) {{ return addAndGet(i, -1); }}
  {T} addAndGet(int i, {T} delta) {{
    check(i);
    lock({LOCK});
    {T} now = {w("this.array[i] + delta")};
    this.array[i] = now;
    unlock({LOCK});
    return now;
  }}
'''
    elem_text = '"" + this.array[i]' if T != "Any" else 'textOf(this.array[i])'
    extra = '''  String textOf(Any o) { if (o == null) { return "null"; } return "" + o; }
''' if T == "Any" else WRAP32
    return f'''  < ---- java.util.concurrent.atomic.{name} members (procedural implementation in SLeeLa) ---- >
  << A {T} array whose elements are updated atomically under VM lock {LOCK}
     (reserved for java.util.concurrent.atomic). The constructor takes the
     length (Java {name}(int length)); every element starts at {zero}. >>
  {T}[] array;
  {name}(int length) {{
    this.array = new {T}[length];
    int i = 0;
    while (i < length) {{ this.array[i] = {zero}; i = i + 1; }}
  }}
  void check(int i) {{
    if (i < 0 || i >= arrayLength(this.array)) {{
      throw new ArrayIndexOutOfBoundsException("Index " + i + " out of bounds for length " + arrayLength(this.array));
    }}
  }}
  int length() {{ return arrayLength(this.array); }}
  {T} get(int i) {{ check(i); return this.array[i]; }}
  void set(int i, {T} newValue) {{ check(i); lock({LOCK}); this.array[i] = newValue; unlock({LOCK}); }}
  void lazySet(int i, {T} newValue) {{ set(i, newValue); }}
  {T} getPlain(int i) {{ return get(i); }}
  void setPlain(int i, {T} newValue) {{ set(i, newValue); }}
  {T} getOpaque(int i) {{ return get(i); }}
  void setOpaque(int i, {T} newValue) {{ set(i, newValue); }}
  {T} getAcquire(int i) {{ return get(i); }}
  void setRelease(int i, {T} newValue) {{ set(i, newValue); }}
  {T} getAndSet(int i, {T} newValue) {{
    check(i);
    lock({LOCK});
    {T} old = this.array[i];
    this.array[i] = newValue;
    unlock({LOCK});
    return old;
  }}
  boolean compareAndSet(int i, {T} expectedValue, {T} newValue) {{
    check(i);
    lock({LOCK});
    boolean hit = this.array[i] == expectedValue;
    if (hit) {{ this.array[i] = newValue; }}
    unlock({LOCK});
    return hit;
  }}
  boolean weakCompareAndSet(int i, {T} expectedValue, {T} newValue) {{ return compareAndSet(i, expectedValue, newValue); }}
  boolean weakCompareAndSetPlain(int i, {T} expectedValue, {T} newValue) {{ return compareAndSet(i, expectedValue, newValue); }}
  boolean weakCompareAndSetVolatile(int i, {T} expectedValue, {T} newValue) {{ return compareAndSet(i, expectedValue, newValue); }}
  boolean weakCompareAndSetAcquire(int i, {T} expectedValue, {T} newValue) {{ return compareAndSet(i, expectedValue, newValue); }}
  boolean weakCompareAndSetRelease(int i, {T} expectedValue, {T} newValue) {{ return compareAndSet(i, expectedValue, newValue); }}
  {T} compareAndExchange(int i, {T} expectedValue, {T} newValue) {{
    check(i);
    lock({LOCK});
    {T} witness = this.array[i];
    if (witness == expectedValue) {{ this.array[i] = newValue; }}
    unlock({LOCK});
    return witness;
  }}
  {T} compareAndExchangeAcquire(int i, {T} expectedValue, {T} newValue) {{ return compareAndExchange(i, expectedValue, newValue); }}
  {T} compareAndExchangeRelease(int i, {T} expectedValue, {T} newValue) {{ return compareAndExchange(i, expectedValue, newValue); }}
{arith}  String toString() {{
    int n = arrayLength(this.array);
    if (n == 0) {{ return "[]"; }}
    String sb = "[";
    int i = 0;
    while (i < n) {{
      if (i > 0) {{ sb = sb + ", "; }}
      sb = sb + {elem_text};
      i = i + 1;
    }}
    return sb + "]";
  }}
''' + extra


def adder(name, T):
    zero = "0" if T == "long" else "0.0"
    return f'''@extends Number
  < ---- java.util.concurrent.atomic.{name} members (procedural implementation in SLeeLa) ---- >
  << A sum maintained atomically under VM lock {LOCK}. Java spreads the sum
     over cells to reduce contention; a single locked cell has the same
     observable results. >>
  {T} total;
  {name}() {{ this.total = {zero}; }}
  void add({T} x) {{ lock({LOCK}); this.total = this.total + x; unlock({LOCK}); }}
''' + (f'''  void increment() {{ add(1); }}
  void decrement() {{ add(-1); }}
''' if T == "long" else "") + f'''  {T} sum() {{ return this.total; }}
  void reset() {{ lock({LOCK}); this.total = {zero}; unlock({LOCK}); }}
  {T} sumThenReset() {{
    lock({LOCK});
    {T} s = this.total;
    this.total = {zero};
    unlock({LOCK});
    return s;
  }}
  String toString() {{ ''' + ('return "" + sum();' if T == "long" else 'Double d = new Double(sum()); return d.toString();') + f''' }}
  long longValue() {{ return (long) sum(); }}
  int intValue() {{ Double d = new Double(sum()); return d.intValue(); }}
  double doubleValue() {{ double d = sum(); return d; }}
  double floatValue() {{ Float f = new Float(doubleValue()); return f.doubleValue(); }}
'''


FMT = '''  << Java %f: fixed notation with six decimals, rounded half-up on the
     exact binary value (via the Double decimal machinery). >>
  String fixed6(double d) {
    Double D = new Double(0.0);
    if (d != d) { return "NaN"; }
    if (D.isInfinite(d)) { if (d > 0.0) { return "Infinity"; } return "-Infinity"; }
    String sign = "";
    double a = d;
    if (a < 0.0 || D.isNegativeZero(a)) { sign = "-"; a = 0.0 - a; }
    if (a >= 1.0e12) { return sign + D.toStringOf(a); }
    long m = D.scaledRound(a, 6);
    long ip = m / 1000000;
    long fp = m % 1000000;
    String fs = "" + fp;
    while (fs != "" && lengthOf(fs) < 6) { fs = "0" + fs; }
    return sign + ip + "." + fs;
  }
  int lengthOf(String s) { return javaStringLength(javaStringFromUtf8(s)); }
'''


def stats(name, T):
    if T == "int" or T == "long":
        acc_t = T
        return f'''  < ---- java.util.{name} members (procedural implementation in SLeeLa) ---- >
  << count, sum, min, max and average of the accepted values, with the Java
     empty-state values (min = MAX_VALUE, max = MIN_VALUE, average 0.0) and the
     Java toString layout. >>
  long count;
  long sum;
  {T} min;
  {T} max;
  {name}() {{
    this.count = 0;
    this.sum = 0;
    this.min = {"2147483647" if T == "int" else "9223372036854775807"};
    this.max = {"-2147483648" if T == "int" else "-9223372036854775807 - 1"};
  }}
  void accept({T} value) {{
    this.count = this.count + 1;
    this.sum = this.sum + value;
    if (value < this.min) {{ this.min = value; }}
    if (value > this.max) {{ this.max = value; }}
  }}
  void combine({name} other) {{
    this.count = this.count + other.getCount();
    this.sum = this.sum + other.getSum();
    if (other.getMin() < this.min) {{ this.min = other.getMin(); }}
    if (other.getMax() > this.max) {{ this.max = other.getMax(); }}
  }}
  long getCount() {{ return this.count; }}
  long getSum() {{ return this.sum; }}
  {T} getMin() {{ return this.min; }}
  {T} getMax() {{ return this.max; }}
  double getAverage() {{
    if (this.count == 0) {{ return 0.0; }}
    double s = this.sum;
    double c = this.count;
    return s / c;
  }}
  String toString() {{
    return "{name}{{count=" + this.count + ", sum=" + this.sum + ", min=" + this.min + ", average=" + fixed6(getAverage()) + ", max=" + this.max + "}}";
  }}
''' + FMT
    return f'''  < ---- java.util.{name} members (procedural implementation in SLeeLa) ---- >
  << count, sum, min, max and average of the accepted doubles. The sum uses
     the same Kahan compensated summation as Java (plus the simple sum for the
     infinite/NaN case), so getSum and getAverage match Java bit for bit. >>
  long count;
  double sum;
  double sumCompensation;
  double simpleSum;
  double min;
  double max;
  {name}() {{
    Double D = new Double(0.0);
    this.count = 0;
    this.sum = 0.0;
    this.sumCompensation = 0.0;
    this.simpleSum = 0.0;
    this.min = D.positiveInfinity();
    this.max = D.negativeInfinity();
  }}
  void sumWithCompensation(double value) {{
    double tmp = value - this.sumCompensation;
    double velvel = this.sum + tmp;
    this.sumCompensation = (velvel - this.sum) - tmp;
    this.sum = velvel;
  }}
  void accept(double value) {{
    Math M = new Math();
    this.count = this.count + 1;
    this.simpleSum = this.simpleSum + value;
    sumWithCompensation(value);
    this.min = M.min(this.min, value);
    this.max = M.max(this.max, value);
  }}
  void combine({name} other) {{
    Math M = new Math();
    this.count = this.count + other.count;
    this.simpleSum = this.simpleSum + other.simpleSum;
    sumWithCompensation(other.sum);
    sumWithCompensation(0.0 - other.sumCompensation);
    this.min = M.min(this.min, other.min);
    this.max = M.max(this.max, other.max);
  }}
  long getCount() {{ return this.count; }}
  double getSum() {{
    Double D = new Double(0.0);
    double tmp = this.sum - this.sumCompensation;
    if (tmp != tmp && D.isInfinite(this.simpleSum)) {{ return this.simpleSum; }}
    return tmp;
  }}
  double getMin() {{ return this.min; }}
  double getMax() {{ return this.max; }}
  double getAverage() {{
    if (this.count == 0) {{ return 0.0; }}
    double c = this.count;
    return getSum() / c;
  }}
  String toString() {{
    return "{name}{{count=" + this.count + ", sum=" + fixed6(getSum()) + ", min=" + fixed6(this.min) + ", average=" + fixed6(getAverage()) + ", max=" + fixed6(this.max) + "}}";
  }}
''' + FMT


XOR = '''  << Bitwise operations on unsigned 32-bit values through a 4-bit table
     (the VM has no bitwise operators). >>
  int[] nib;
  void initNibbles() {
    this.nib = new int[256];
    int a = 0;
    while (a < 16) {
      int b = 0;
      while (b < 16) {
        int r = 0;
        int bit = 1;
        int x = a;
        int y = b;
        int k = 0;
        while (k < 4) {
          if ((x % 2) != (y % 2)) { r = r + bit; }
          x = x / 2; y = y / 2; bit = bit * 2; k = k + 1;
        }
        this.nib[a * 16 + b] = r;
        b = b + 1;
      }
      a = a + 1;
    }
  }
  long xor32(long a, long b) {
    long r = 0;
    long mul = 1;
    long x = a;
    long y = b;
    int k = 0;
    while (k < 8) {
      r = r + this.nib[(int) ((x % 16) * 16 + (y % 16))] * mul;
      x = x / 16; y = y / 16; mul = mul * 16; k = k + 1;
    }
    return r;
  }
'''


def crc(name, poly, doc):
    return f'''  < ---- java.util.zip.{name} members (procedural implementation in SLeeLa) ---- >
  << {doc} Table driven (reflected polynomial {poly}), exactly the checksum
     java.util.zip.{name} computes. Bytes are ints (only the low 8 bits are
     used). update(byte[], off, len) is updateBytes; update(byte[]) is
     updateAll. >>
  long crc;
  long[] table;
  {name}() {{
    initNibbles();
    this.table = new long[256];
    int n = 0;
    while (n < 256) {{
      long c = n;
      int k = 0;
      while (k < 8) {{
        if (c % 2 == 1) {{ c = xor32({poly}, c / 2); }} else {{ c = c / 2; }}
        k = k + 1;
      }}
      this.table[n] = c;
      n = n + 1;
    }}
    this.crc = 4294967295;
  }}
  void update(int b) {{
    long byteVal = b % 256;
    if (byteVal < 0) {{ byteVal = byteVal + 256; }}
    long idx = xor32(this.crc % 256, byteVal);
    this.crc = xor32(this.table[(int) idx], this.crc / 256);
  }}
  void updateBytes(int[] b, int off, int len) {{
    if (off < 0 || len < 0 || off > arrayLength(b) - len) {{ throw new ArrayIndexOutOfBoundsException(null); }}
    int i = off;
    while (i < off + len) {{ update(b[i]); i = i + 1; }}
  }}
  void updateAll(int[] b) {{ updateBytes(b, 0, arrayLength(b)); }}
  long getValue() {{ return xor32(this.crc, 4294967295); }}
  void reset() {{ this.crc = 4294967295; }}
''' + XOR


ADLER = '''  < ---- java.util.zip.Adler32 members (procedural implementation in SLeeLa) ---- >
  << The Adler-32 checksum of RFC 1950, exactly as java.util.zip.Adler32.
     update(byte[], off, len) is updateBytes; update(byte[]) is updateAll. >>
  long a;
  long b;
  Adler32() { this.a = 1; this.b = 0; }
  void update(int x) {
    long v = x % 256;
    if (v < 0) { v = v + 256; }
    this.a = (this.a + v) % 65521;
    this.b = (this.b + this.a) % 65521;
  }
  void updateBytes(int[] buf, int off, int len) {
    if (off < 0 || len < 0 || off > arrayLength(buf) - len) { throw new ArrayIndexOutOfBoundsException(null); }
    int i = off;
    while (i < off + len) { update(buf[i]); i = i + 1; }
  }
  void updateAll(int[] buf) { updateBytes(buf, 0, arrayLength(buf)); }
  long getValue() { return this.b * 65536 + this.a; }
  void reset() { this.a = 1; this.b = 0; }
'''

STE = '''  < ---- java.lang.StackTraceElement members (procedural implementation in SLeeLa) ---- >
  << One frame of a stack trace with the Java accessors, toString layout and
     hashCode formula. The constructor is the Java 4-argument form
     (declaringClass, methodName, fileName, lineNumber); a line number of -2
     marks a native method. Module and class-loader names are null. >>
  String declaringClass;
  String methodName;
  String fileName;
  int lineNumber;
  StackTraceElement(String declaringClass, String methodName, String fileName, int lineNumber) {
    if (declaringClass == null) { throw new NullPointerException("Declaring class is null"); }
    if (methodName == null) { throw new NullPointerException("Method name is null"); }
    this.declaringClass = declaringClass;
    this.methodName = methodName;
    this.fileName = fileName;
    this.lineNumber = lineNumber;
  }
  String getClassName() { return this.declaringClass; }
  String getMethodName() { return this.methodName; }
  String getFileName() { return this.fileName; }
  int getLineNumber() { return this.lineNumber; }
  String getModuleName() { return null; }
  String getModuleVersion() { return null; }
  String getClassLoaderName() { return null; }
  boolean isNativeMethod() { return this.lineNumber == -2; }
  String toString() {
    String where = "";
    if (isNativeMethod()) { where = "Native Method"; }
    else {
      if (this.fileName == null) { where = "Unknown Source"; }
      else {
        if (this.lineNumber >= 0) { where = this.fileName + ":" + this.lineNumber; }
        else { where = this.fileName; }
      }
    }
    return this.declaringClass + "." + this.methodName + "(" + where + ")";
  }
  boolean equals(Any obj) {
    if (obj instanceof StackTraceElement) {
      StackTraceElement e = (StackTraceElement) obj;
      return e.getClassName() == this.declaringClass && e.getMethodName() == this.methodName && e.getFileName() == this.fileName && e.getLineNumber() == this.lineNumber;
    }
    return false;
  }
  int strHash(String s) {
    if (s == null) { return 0; }
    return javaStringHashCode(javaStringFromUtf8(s));
  }
  int hashCode() {
    long r = 31 * strHash(this.declaringClass) + strHash(this.methodName);
    r = wrap32(r);
    r = wrap32(31 * r);
    r = wrap32(31 * r);
    r = wrap32(31 * r);
    r = wrap32(31 * r + strHash(this.fileName));
    r = wrap32(31 * r + this.lineNumber);
    return (int) r;
  }
''' + WRAP32

BITSET = '''  < ---- java.util.BitSet members (procedural implementation in SLeeLa) ---- >
  << A growable vector of bits. Storage is one boolean per bit; the Java
     long-word view is reproduced where it is observable (size() is a
     multiple of 64, toLongArray / valueOf / hashCode use the 64-bit word
     layout). The constructor takes the initial size in bits. Java overloads
     carry suffixes: set(i, value) is setTo, set(from, to) is setRange,
     set(from, to, value) is setRangeTo, clear(from, to) is clearRange,
     clear() is clearAll, get(from, to) is getRange, flip(from, to) is
     flipRange. >>
  boolean[] bits;
  int wordsInUse;
  BitSet(int nbits) {
    if (nbits < 0) { throw new NegativeArraySizeException("nbits < 0: " + nbits); }
    int words = (nbits + 63) / 64;
    this.bits = newBits(words * 64);
    this.wordsInUse = 0;
  }
  boolean[] newBits(int n) {
    boolean[] b = new boolean[n];
    int i = 0;
    while (i < n) { b[i] = false; i = i + 1; }
    return b;
  }
  void ensure(int bitIndex) {
    int cap = arrayLength(this.bits);
    if (bitIndex < cap) { return; }
    int need = (bitIndex / 64 + 1) * 64;
    int nc = cap * 2;
    if (nc < need) { nc = need; }
    boolean[] nb = newBits(nc);
    int i = 0;
    while (i < cap) { nb[i] = this.bits[i]; i = i + 1; }
    this.bits = nb;
  }
  void checkIndex(int bitIndex) {
    if (bitIndex < 0) { throw new IndexOutOfBoundsException("bitIndex < 0: " + bitIndex); }
  }
  void checkRange(int fromIndex, int toIndex) {
    if (fromIndex < 0) { throw new IndexOutOfBoundsException("fromIndex < 0: " + fromIndex); }
    if (toIndex < 0) { throw new IndexOutOfBoundsException("toIndex < 0: " + toIndex); }
    if (fromIndex > toIndex) { throw new IndexOutOfBoundsException("fromIndex: " + fromIndex + " > toIndex: " + toIndex); }
  }
  void set(int bitIndex) { setTo(bitIndex, true); }
  void setTo(int bitIndex, boolean value) {
    checkIndex(bitIndex);
    if (value) { ensure(bitIndex); this.bits[bitIndex] = true; }
    else { if (bitIndex < arrayLength(this.bits)) { this.bits[bitIndex] = false; } }
  }
  void setRange(int fromIndex, int toIndex) { setRangeTo(fromIndex, toIndex, true); }
  void setRangeTo(int fromIndex, int toIndex, boolean value) {
    checkRange(fromIndex, toIndex);
    int i = fromIndex;
    while (i < toIndex) { setTo(i, value); i = i + 1; }
  }
  void clear(int bitIndex) { setTo(bitIndex, false); }
  void clearRange(int fromIndex, int toIndex) { setRangeTo(fromIndex, toIndex, false); }
  void clearAll() {
    int i = 0;
    while (i < arrayLength(this.bits)) { this.bits[i] = false; i = i + 1; }
  }
  boolean get(int bitIndex) {
    checkIndex(bitIndex);
    if (bitIndex >= arrayLength(this.bits)) { return false; }
    return this.bits[bitIndex];
  }
  BitSet getRange(int fromIndex, int toIndex) {
    checkRange(fromIndex, toIndex);
    BitSet r = new BitSet(0);
    int i = fromIndex;
    while (i < toIndex) { if (get(i)) { r.set(i - fromIndex); } i = i + 1; }
    return r;
  }
  void flip(int bitIndex) { setTo(bitIndex, !get(bitIndex)); }
  void flipRange(int fromIndex, int toIndex) {
    checkRange(fromIndex, toIndex);
    int i = fromIndex;
    while (i < toIndex) { flip(i); i = i + 1; }
  }
  << Index of the highest set bit plus one. >>
  int length() {
    int i = arrayLength(this.bits) - 1;
    while (i >= 0) { if (this.bits[i]) { return i + 1; } i = i - 1; }
    return 0;
  }
  int size() { return arrayLength(this.bits); }
  boolean isEmpty() { return length() == 0; }
  int cardinality() {
    int n = 0;
    int i = 0;
    while (i < arrayLength(this.bits)) { if (this.bits[i]) { n = n + 1; } i = i + 1; }
    return n;
  }
  int nextSetBit(int fromIndex) {
    if (fromIndex < 0) { throw new IndexOutOfBoundsException("fromIndex < 0: " + fromIndex); }
    int i = fromIndex;
    while (i < arrayLength(this.bits)) { if (this.bits[i]) { return i; } i = i + 1; }
    return -1;
  }
  int nextClearBit(int fromIndex) {
    if (fromIndex < 0) { throw new IndexOutOfBoundsException("fromIndex < 0: " + fromIndex); }
    int i = fromIndex;
    while (i < arrayLength(this.bits)) { if (!this.bits[i]) { return i; } i = i + 1; }
    return i;
  }
  int previousSetBit(int fromIndex) {
    if (fromIndex < -1) { throw new IndexOutOfBoundsException("fromIndex < -1: " + fromIndex); }
    int i = fromIndex;
    while (i >= 0) { if (get(i)) { return i; } i = i - 1; }
    return -1;
  }
  int previousClearBit(int fromIndex) {
    if (fromIndex < -1) { throw new IndexOutOfBoundsException("fromIndex < -1: " + fromIndex); }
    int i = fromIndex;
    while (i >= 0) { if (!get(i)) { return i; } i = i - 1; }
    return -1;
  }
  void and(BitSet set) {
    int i = 0;
    while (i < arrayLength(this.bits)) { if (this.bits[i] && !set.get(i)) { this.bits[i] = false; } i = i + 1; }
  }
  void or(BitSet set) {
    int n = set.length();
    int i = 0;
    while (i < n) { if (set.get(i)) { setTo(i, true); } i = i + 1; }
  }
  void xor(BitSet set) {
    int n = set.length();
    int i = 0;
    while (i < n) { if (set.get(i)) { flip(i); } i = i + 1; }
  }
  void andNot(BitSet set) {
    int i = 0;
    while (i < arrayLength(this.bits)) { if (this.bits[i] && set.get(i)) { this.bits[i] = false; } i = i + 1; }
  }
  boolean intersects(BitSet set) {
    int i = 0;
    while (i < arrayLength(this.bits)) { if (this.bits[i] && set.get(i)) { return true; } i = i + 1; }
    return false;
  }
  << The 64-bit word w of the Java long[] layout (bit i of word w is bit 64w+i). >>
  long word(int w) {
    long v = 0;
    long p = 1;
    int i = 0;
    while (i < 63) {
      if (get(w * 64 + i)) { v = v + p; }
      p = p * 2;
      i = i + 1;
    }
    if (get(w * 64 + 63)) { v = v + (-9223372036854775807 - 1); }
    return v;
  }
  long[] toLongArray() {
    int n = (length() + 63) / 64;
    long[] out = new long[n];
    int w = 0;
    while (w < n) { out[w] = word(w); w = w + 1; }
    return out;
  }
  BitSet valueOf(long[] longs) {
    BitSet r = new BitSet(arrayLength(longs) * 64);
    int w = 0;
    while (w < arrayLength(longs)) {
      long v = longs[w];
      boolean top = v < 0;
      if (top) { v = v - (-9223372036854775807 - 1); }
      int i = 0;
      while (i < 63) {
        if (v % 2 == 1) { r.set(w * 64 + i); }
        v = v / 2;
        i = i + 1;
      }
      if (top) { r.set(w * 64 + 63); }
      w = w + 1;
    }
    return r;
  }
  int[] toByteArray() {
    int n = (length() + 7) / 8;
    int[] out = new int[n];
    int k = 0;
    while (k < n) {
      int v = 0;
      int p = 1;
      int i = 0;
      while (i < 8) { if (get(k * 8 + i)) { v = v + p; } p = p * 2; i = i + 1; }
      if (v > 127) { v = v - 256; }
      out[k] = v;
      k = k + 1;
    }
    return out;
  }
  << Java hashCode: h = 1234 xor-accumulated with word[i] * (i + 1), folded
     to 32 bits; computed on 32-bit halves. >>
  int hashCode() {
    initNibbles();
    long hHi = 0;
    long hLo = 1234;
    int n = (length() + 63) / 64;
    int i = n - 1;
    while (i >= 0) {
      long prodHi = 0;
      long prodLo = 0;
      mul64(word(i), i + 1);
      hHi = xor32(hHi, this.mulHi);
      hLo = xor32(hLo, this.mulLo);
      i = i - 1;
    }
    long r = xor32(hHi, hLo);
    if (r > 2147483647) { r = r - 4294967296; }
    return (int) r;
  }
  long mulHi;
  long mulLo;
  << The low 64 bits of v * k (k small, positive) as unsigned 32-bit halves. >>
  void mul64(long v, int k) {
    long lo = v % 4294967296;
    long hi = v / 4294967296;
    if (lo < 0) { lo = lo + 4294967296; hi = hi - 1; }
    if (hi < 0) { hi = hi + 4294967296; }
    long pl = lo * k;
    long carry = pl / 4294967296;
    this.mulLo = pl % 4294967296;
    this.mulHi = (hi * k + carry) % 4294967296;
  }
  boolean equals(Any obj) {
    if (obj instanceof BitSet) {
      BitSet o = (BitSet) obj;
      int n = length();
      if (o.length() != n) { return false; }
      int i = 0;
      while (i < n) { if (get(i) != o.get(i)) { return false; } i = i + 1; }
      return true;
    }
    return false;
  }
  BitSet clone() {
    BitSet r = new BitSet(arrayLength(this.bits));
    r.or(this);
    return r;
  }
  String toString() {
    String sb = "{";
    boolean first = true;
    int i = 0;
    while (i < arrayLength(this.bits)) {
      if (this.bits[i]) {
        if (!first) { sb = sb + ", "; }
        sb = sb + i;
        first = false;
      }
      i = i + 1;
    }
    return sb + "}";
  }
''' + XOR

files = {
    "java.util.concurrent.atomic.AtomicInteger": "@extends Number\n" + atomic_scalar("AtomicInteger", "int", True),
    "java.util.concurrent.atomic.AtomicLong": "@extends Number\n" + atomic_scalar("AtomicLong", "long", False),
    "java.util.concurrent.atomic.AtomicBoolean": atomic_scalar("AtomicBoolean", "boolean", False),
    "java.util.concurrent.atomic.AtomicReference": atomic_scalar("AtomicReference", "Any", False),
    "java.util.concurrent.atomic.AtomicIntegerArray": atomic_array("AtomicIntegerArray", "int", True),
    "java.util.concurrent.atomic.AtomicLongArray": atomic_array("AtomicLongArray", "long", False),
    "java.util.concurrent.atomic.AtomicReferenceArray": atomic_array("AtomicReferenceArray", "Any", False),
    "java.util.concurrent.atomic.LongAdder": adder("LongAdder", "long"),
    "java.util.concurrent.atomic.DoubleAdder": adder("DoubleAdder", "double"),
    "java.util.IntSummaryStatistics": stats("IntSummaryStatistics", "int"),
    "java.util.LongSummaryStatistics": stats("LongSummaryStatistics", "long"),
    "java.util.DoubleSummaryStatistics": stats("DoubleSummaryStatistics", "double"),
    "java.util.zip.CRC32": crc("CRC32", "3988292384", "The CRC-32 of ISO 3309 / ITU-T V.42 (zlib)."),
    "java.util.zip.CRC32C": crc("CRC32C", "2197175160", "The CRC-32C (Castagnoli) of RFC 3720."),
    "java.util.zip.Adler32": ADLER,
    "java.lang.StackTraceElement": STE,
    "java.util.BitSet": BITSET,
}
for k, v in files.items():
    (OUT / f"{k}.body").write_text(v)

implement.apply_dir(OUT)
