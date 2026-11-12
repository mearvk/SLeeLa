#!/usr/bin/env python3
"""Emit bodies for java.util Arrays, Collections, Optional*, Random."""
from pathlib import Path
import tempfile
import implement
import re

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))
coll = (Path(__file__).resolve().parent / "gen_collections.py").read_text()
# reuse the VALUE helper block from the collections generator
VALUE = re.search(r"VALUE = r'''(.*?)'''", coll, re.S).group(1)


def typed_array_ops(T, suffix, cmp_lt, eq, hash_expr, text_expr, zero):
    """Arrays operations over T[] with a name suffix ('' for int)."""
    return f'''
  < {T}[] operations >
  String toString{suffix}({T}[] a) {{
    if (a == null) {{ return "null"; }}
    String sb = "[";
    int i = 0;
    int n = arrayLength(a);
    while (i < n) {{
      if (i > 0) {{ sb = sb + ", "; }}
      sb = sb + {text_expr};
      i = i + 1;
    }}
    return sb + "]";
  }}
  void sort{suffix}({T}[] a) {{ sortRange{suffix}(a, 0, arrayLength(a)); }}
  void sortRange{suffix}({T}[] a, int fromIndex, int toIndex) {{
    rangeCheck(arrayLength(a), fromIndex, toIndex);
    {T}[] tmp = new {T}[arrayLength(a)];
    msort{suffix}(a, tmp, fromIndex, toIndex);
  }}
  void msort{suffix}({T}[] a, {T}[] tmp, int lo, int hi) {{
    if (hi - lo < 2) {{ return; }}
    int mid = (lo + hi) / 2;
    msort{suffix}(a, tmp, lo, mid);
    msort{suffix}(a, tmp, mid, hi);
    int i = lo;
    int j = mid;
    int k = lo;
    while (i < mid && j < hi) {{
      {T} x = a[i];
      {T} y = a[j];
      if ({cmp_lt.format(a="y", b="x")}) {{ tmp[k] = y; j = j + 1; }} else {{ tmp[k] = x; i = i + 1; }}
      k = k + 1;
    }}
    while (i < mid) {{ tmp[k] = a[i]; i = i + 1; k = k + 1; }}
    while (j < hi) {{ tmp[k] = a[j]; j = j + 1; k = k + 1; }}
    k = lo;
    while (k < hi) {{ a[k] = tmp[k]; k = k + 1; }}
  }}
  void fill{suffix}({T}[] a, {T} val) {{
    int i = 0;
    while (i < arrayLength(a)) {{ a[i] = val; i = i + 1; }}
  }}
  void fillRange{suffix}({T}[] a, int fromIndex, int toIndex, {T} val) {{
    rangeCheck(arrayLength(a), fromIndex, toIndex);
    int i = fromIndex;
    while (i < toIndex) {{ a[i] = val; i = i + 1; }}
  }}
  {T}[] copyOf{suffix}({T}[] original, int newLength) {{
    if (newLength < 0) {{ throw new NegativeArraySizeException("" + newLength); }}
    {T}[] out = new {T}[newLength];
    int i = 0;
    while (i < newLength && i < arrayLength(original)) {{ out[i] = original[i]; i = i + 1; }}
    while (i < newLength) {{ out[i] = {zero}; i = i + 1; }}
    return out;
  }}
  {T}[] copyOfRange{suffix}({T}[] original, int from, int to) {{
    if (from > to) {{ throw new IllegalArgumentException(from + " > " + to); }}
    if (from < 0 || from > arrayLength(original)) {{ throw new ArrayIndexOutOfBoundsException("Array index out of range: " + from); }}
    {T}[] out = new {T}[to - from];
    int i = from;
    while (i < to && i < arrayLength(original)) {{ out[i - from] = original[i]; i = i + 1; }}
    while (i < to) {{ out[i - from] = {zero}; i = i + 1; }}
    return out;
  }}
  boolean equals{suffix}({T}[] a, {T}[] b) {{
    if (a == null || b == null) {{ return a == null && b == null; }}
    if (arrayLength(a) != arrayLength(b)) {{ return false; }}
    int i = 0;
    while (i < arrayLength(a)) {{
      {T} x = a[i];
      {T} y = b[i];
      if (!({eq.format(a="x", b="y")})) {{ return false; }}
      i = i + 1;
    }}
    return true;
  }}
  int hashCode{suffix}({T}[] a) {{
    if (a == null) {{ return 0; }}
    int h = 1;
    int i = 0;
    while (i < arrayLength(a)) {{
      {T} e = a[i];
      h = wrap32(31 * h + {hash_expr});
      i = i + 1;
    }}
    return h;
  }}
  << Binary search of a sorted array: the index of key, else -(insertion point) - 1. >>
  int binarySearch{suffix}({T}[] a, {T} key) {{
    int lo = 0;
    int hi = arrayLength(a) - 1;
    while (lo <= hi) {{
      int mid = (lo + hi) / 2;
      {T} m = a[mid];
      if ({cmp_lt.format(a="m", b="key")}) {{ lo = mid + 1; }}
      else {{ if ({cmp_lt.format(a="key", b="m")}) {{ hi = mid - 1; }} else {{ return mid; }} }}
    }}
    return 0 - (lo + 1);
  }}
  << Index of the first mismatch of a and b, or -1 when they are equal. >>
  int mismatch{suffix}({T}[] a, {T}[] b) {{
    int n = arrayLength(a);
    if (arrayLength(b) < n) {{ n = arrayLength(b); }}
    int i = 0;
    while (i < n) {{
      {T} x = a[i];
      {T} y = b[i];
      if (!({eq.format(a="x", b="y")})) {{ return i; }}
      i = i + 1;
    }}
    if (arrayLength(a) == arrayLength(b)) {{ return -1; }}
    return n;
  }}
'''


ARRAYS = "@import java.java.util\n" + '''  < ---- java.util.Arrays members (procedural implementation in SLeeLa) ---- >
  << The java.util.Arrays static utilities as instance methods (call as
     new Arrays().sort(a)). The int[] forms carry the Java names; the
     double[] and Object[] (Any[]) forms carry a Doubles / Objects suffix
     because calls resolve by name only. Sorting is a stable merge sort;
     double ordering follows Double.compare (-0.0 before 0.0, NaN last);
     Object ordering is natural ordering. asList returns an ArrayList copy. >>
  Arrays() { gcSlot = 0; utfOps = 0; }

  void rangeCheck(int arrayLength, int fromIndex, int toIndex) {
    if (fromIndex > toIndex) {
      throw new IllegalArgumentException("fromIndex(" + fromIndex + ") > toIndex(" + toIndex + ")");
    }
    if (fromIndex < 0) { throw new ArrayIndexOutOfBoundsException("Array index out of range: " + fromIndex); }
    if (toIndex > arrayLength) { throw new ArrayIndexOutOfBoundsException("Array index out of range: " + toIndex); }
  }
  << Double.compare-style strict ordering for sorting doubles. >>
  boolean dLess(double x, double y) {
    Double D = new Double(0.0);
    return D.compare(x, y) < 0;
  }
  boolean dSame(double x, double y) {
    Double D = new Double(0.0);
    return D.doubleToLongBits(x) == D.doubleToLongBits(y);
  }
  int dHash(double x) {
    Double D = new Double(x);
    return D.hashCode();
  }
  String dText(double x) {
    Double D = new Double(x);
    return D.toString();
  }
  ArrayList asList(Any[] a) {
    ArrayList l = new ArrayList();
    int i = 0;
    while (i < arrayLength(a)) { l.add(a[i]); i = i + 1; }
    return l;
  }
''' + typed_array_ops("int", "", "{a} < {b}", "{a} == {b}", "e", '("" + a[i])', "0") \
    + typed_array_ops("double", "Doubles", "dLess({a}, {b})", "dSame({a}, {b})", "dHash(e)", "dText(a[i])", "0.0") \
    + typed_array_ops("Any", "Objects", "compareValues({a}, {b}) < 0", "same({a}, {b})", "hashOf(e)", "textOf(a[i])", "null") \
    + VALUE

COLLECTIONS = "@import java.java.util\n" + '''  < ---- java.util.Collections members (procedural implementation in SLeeLa) ---- >
  << The java.util.Collections static algorithms as instance methods over the
     list, set and map classes of this package (ArrayList, LinkedList,
     Vector, Stack, ArrayDeque, HashSet, TreeSet, PriorityQueue, HashMap,
     TreeMap). Lists are rewritten in place through their List API. Natural
     ordering only (Comparator dispatch is a later semantics pass). The
     unmodifiable / synchronized wrappers return copies (no write guard yet).
     shuffle uses a java.util.Random with the Java algorithm. >>
  Collections() { gcSlot = 0; utfOps = 0; }

  int sizeOf(Any list) { return arrayLength(elementsOf(list)); }
  << Overwrite element i of a list of this package. >>
  void setAt(Any list, int i, Any v) {
    if (list instanceof ArrayList) { ArrayList a = (ArrayList) list; a.set(i, v); return; }
    if (list instanceof LinkedList) { LinkedList b = (LinkedList) list; b.set(i, v); return; }
    if (list instanceof Vector) { Vector c = (Vector) list; c.set(i, v); return; }
    throw new UnsupportedOperationException("not a java.util list implemented in SLeeLa");
  }
  void writeBack(Any list, Any[] xs) {
    int i = 0;
    while (i < arrayLength(xs)) { setAt(list, i, xs[i]); i = i + 1; }
  }
  void sort(Any list) {
    Any[] xs = elementsOf(list);
    Arrays A = new Arrays();
    A.sortObjects(xs);
    writeBack(list, xs);
  }
  void reverse(Any list) {
    Any[] xs = elementsOf(list);
    int n = arrayLength(xs);
    Any[] r = new Any[n];
    int i = 0;
    while (i < n) { r[i] = xs[n - 1 - i]; i = i + 1; }
    writeBack(list, r);
  }
  void swap(Any list, int i, int j) {
    Any[] xs = elementsOf(list);
    Any t = xs[i];
    setAt(list, i, xs[j]);
    setAt(list, j, t);
  }
  void fill(Any list, Any obj) {
    int n = sizeOf(list);
    int i = 0;
    while (i < n) { setAt(list, i, obj); i = i + 1; }
  }
  << Rotate by distance: element i moves to (i + distance) mod size. >>
  void rotate(Any list, int distance) {
    Any[] xs = elementsOf(list);
    int n = arrayLength(xs);
    if (n == 0) { return; }
    int d = distance % n;
    if (d < 0) { d = d + n; }
    Any[] r = new Any[n];
    int i = 0;
    while (i < n) { r[(i + d) % n] = xs[i]; i = i + 1; }
    writeBack(list, r);
  }
  << Fisher-Yates shuffle driven by the given Random (Java algorithm). >>
  void shuffle(Any list, Random rnd) {
    Any[] xs = elementsOf(list);
    int i = arrayLength(xs);
    while (i > 1) {
      int j = rnd.nextIntBound(i);
      Any t = xs[i - 1];
      xs[i - 1] = xs[j];
      xs[j] = t;
      i = i - 1;
    }
    writeBack(list, xs);
  }
  Any max(Any coll) {
    Any[] xs = elementsOf(coll);
    if (arrayLength(xs) == 0) { throw new NoSuchElementException(null); }
    Any best = xs[0];
    int i = 1;
    while (i < arrayLength(xs)) { if (compareValues(xs[i], best) > 0) { best = xs[i]; } i = i + 1; }
    return best;
  }
  Any min(Any coll) {
    Any[] xs = elementsOf(coll);
    if (arrayLength(xs) == 0) { throw new NoSuchElementException(null); }
    Any best = xs[0];
    int i = 1;
    while (i < arrayLength(xs)) { if (compareValues(xs[i], best) < 0) { best = xs[i]; } i = i + 1; }
    return best;
  }
  int frequency(Any c, Any o) {
    Any[] xs = elementsOf(c);
    int n = 0;
    int i = 0;
    while (i < arrayLength(xs)) { if (same(xs[i], o)) { n = n + 1; } i = i + 1; }
    return n;
  }
  boolean disjoint(Any c1, Any c2) {
    Any[] a = elementsOf(c1);
    Any[] b = elementsOf(c2);
    int i = 0;
    while (i < arrayLength(a)) {
      int j = 0;
      while (j < arrayLength(b)) { if (same(a[i], b[j])) { return false; } j = j + 1; }
      i = i + 1;
    }
    return true;
  }
  int binarySearch(Any list, Any key) {
    Arrays A = new Arrays();
    return A.binarySearchObjects(elementsOf(list), key);
  }
  int indexOfSubList(Any source, Any target) {
    Any[] s = elementsOf(source);
    Any[] t = elementsOf(target);
    int n = arrayLength(s);
    int m = arrayLength(t);
    int i = 0;
    while (i + m <= n) {
      int j = 0;
      while (j < m && same(s[i + j], t[j])) { j = j + 1; }
      if (j == m) { return i; }
      i = i + 1;
    }
    return -1;
  }
  boolean addAll(Any c, Any[] elements) {
    boolean changed = false;
    int i = 0;
    while (i < arrayLength(elements)) {
      Any e = elements[i];
      if (c instanceof ArrayList) { ArrayList a = (ArrayList) c; a.add(e); changed = true; }
      if (c instanceof LinkedList) { LinkedList b = (LinkedList) c; b.add(e); changed = true; }
      if (c instanceof Vector) { Vector v = (Vector) c; v.add(e); changed = true; }
      if (c instanceof HashSet) { HashSet h = (HashSet) c; if (h.add(e)) { changed = true; } }
      if (c instanceof TreeSet) { TreeSet t = (TreeSet) c; if (t.add(e)) { changed = true; } }
      i = i + 1;
    }
    return changed;
  }
  ArrayList nCopies(int n, Any o) {
    if (n < 0) { throw new IllegalArgumentException("List length = " + n); }
    ArrayList l = new ArrayList();
    int i = 0;
    while (i < n) { l.add(o); i = i + 1; }
    return l;
  }
  ArrayList emptyList() { return new ArrayList(); }
  HashSet emptySet() { return new HashSet(); }
  HashMap emptyMap() { return new HashMap(); }
  ArrayList singletonList(Any o) { ArrayList l = new ArrayList(); l.add(o); return l; }
  HashSet singleton(Any o) { HashSet s = new HashSet(); s.add(o); return s; }
  HashMap singletonMap(Any key, Any value) { HashMap m = new HashMap(); m.put(key, value); return m; }
  ArrayList list(Any c) {
    ArrayList l = new ArrayList();
    l.addAll(c);
    return l;
  }
  ArrayList unmodifiableList(Any list) { return list(list); }
''' + VALUE

OPTIONAL = "@import java.java.util\n" + '''  < ---- java.util.Optional members (procedural implementation in SLeeLa) ---- >
  << A container that may hold one non-null value. The static factories are
     instance methods (new Optional(null).of(x)); the constructor argument is
     the held value (null = empty). Functional methods (map, filter,
     ifPresent ...) wait for lambda dispatch. >>
  Any value;
  Optional(Any v) { this.value = v; }

  Optional empty() { return new Optional(null); }
  Optional of(Any v) {
    if (v == null) { throw new NullPointerException(null); }
    return new Optional(v);
  }
  Optional ofNullable(Any v) { return new Optional(v); }
  boolean isPresent() { return this.value != null; }
  boolean isEmpty() { return this.value == null; }
  Any get() {
    if (this.value == null) { throw new NoSuchElementException("No value present"); }
    return this.value;
  }
  Any orElseThrow() { return get(); }
  Any orElse(Any other) {
    if (this.value != null) { return this.value; }
    return other;
  }
  String toString() {
    if (this.value == null) { return "Optional.empty"; }
    return "Optional[" + this.value + "]";
  }
  boolean equals(Any obj) {
    if (obj instanceof Optional) {
      Optional o = (Optional) obj;
      return o.value == this.value;
    }
    return false;
  }
  int hashCode() {
    if (this.value == null) { return 0; }
    Objects O = new Objects();
    return O.hashCode(this.value);
  }
'''


def optional_prim(name, T, getter, default_text):
    return f'''  < ---- java.util.{name} members (procedural implementation in SLeeLa) ---- >
  << A container that may hold one {T} value. Factories are instance methods
     (new {name}(false, 0).of(5)); the constructor takes presence and value. >>
  boolean present;
  {T} value;
  {name}(boolean isPresent, {T} v) {{ this.present = isPresent; this.value = v; }}

  {name} empty() {{ return new {name}(false, {default_text}); }}
  {name} of({T} v) {{ return new {name}(true, v); }}
  boolean isPresent() {{ return present; }}
  boolean isEmpty() {{ return !present; }}
  {T} {getter}() {{
    if (!present) {{ throw new NoSuchElementException("No value present"); }}
    return value;
  }}
  {T} orElseThrow() {{ return {getter}(); }}
  {T} orElse({T} other) {{
    if (present) {{ return value; }}
    return other;
  }}
  String toString() {{
    if (!present) {{ return "{name}.empty"; }}
    return "{name}[" + value + "]";
  }}
  boolean equals(Any obj) {{
    if (obj instanceof {name}) {{
      {name} o = ({name}) obj;
      if (!present && !o.present) {{ return true; }}
      return present == o.present && value == o.value;
    }}
    return false;
  }}
'''


RANDOM = "@import java.java.util\n" + '''  < ---- java.util.Random members (procedural implementation in SLeeLa) ---- >
  << The Java 48-bit linear congruential generator, bit for bit: seed' =
     (seed * 0x5DEECE66D + 0xB) mod 2^48, and next(bits) is the top bits of
     the new seed, seed / 2^(48 - bits).
     Given the same seed it yields exactly the Java sequence for nextInt,
     nextInt(bound), nextLong, nextBoolean, nextDouble and nextFloat;
     nextGaussian uses the same polar method, with sqrt/log from this
     package StrictMath (accurate to about 1e-15 rather than fdlibm-exact). The constructor takes the seed (Java Random(long seed));
     the overloaded nextInt(bound) is nextIntBound. >>
  long seed;
  double nextNextGaussian;
  boolean haveNextNextGaussian;
  Random(long s) { setSeed(s); }

  long mask48(long v) {
    long m = v % 281474976710656;
    if (m < 0) { m = m + 281474976710656; }
    return m;
  }
  << Bitwise xor of two non-negative values below 2^48. >>
  long xor48(long a, long b) {
    long r = 0;
    long bit = 1;
    long x = a;
    long y = b;
    int i = 0;
    while (i < 48) {
      if ((x % 2) != (y % 2)) { r = r + bit; }
      x = x / 2; y = y / 2; bit = bit * 2; i = i + 1;
    }
    return r;
  }
  void setSeed(long s) {
    seed = xor48(mask48(s), 25214903917);
    haveNextNextGaussian = false;
    nextNextGaussian = 0.0;
  }
  int toInt32(long v) {
    long m = v % 4294967296;
    if (m < 0) { m = m + 4294967296; }
    if (m > 2147483647) { m = m - 4294967296; }
    return (int) m;
  }
  << The low 48 bits of seed * 0x5DEECE66D + 0xB, without 64-bit overflow:
     the multiplier is split into 24-bit halves. >>
  int next(int bits) {
    long lo = seed % 16777216;
    long hi = seed / 16777216;
    long mlo = 15525485;
    long mhi = 1502;
    long cross = mask48((hi * mlo + lo * mhi) % 16777216 * 16777216);
    long low = lo * mlo;
    seed = mask48(low + cross + 11);
    long div = 1;
    int k = 0;
    while (k < 48 - bits) { div = div * 2; k = k + 1; }
    return toInt32(seed / div);
  }
  int nextInt() { return next(32); }
  int nextIntBound(int bound) {
    if (bound <= 0) { throw new IllegalArgumentException("bound must be positive"); }
    long b = bound;
    boolean pow2 = false;
    long t = 1;
    while (t < b) { t = t * 2; }
    if (t == b) { pow2 = true; }
    if (pow2) {
      long r = next(31);
      return toInt32(b * r / 2147483648);
    }
    int bits = next(31);
    int val = bits % bound;
    while (bits - val + (bound - 1) > 2147483647) {
      bits = next(31);
      val = bits % bound;
    }
    return val;
  }
  int nextIntRange(int origin, int bound) {
    if (origin >= bound) { throw new IllegalArgumentException("bound must be greater than origin"); }
    return origin + nextIntBound(bound - origin);
  }
  long nextLong() {
    long hi = next(32);
    long lo = next(32);
    return hi * 4294967296 + lo;
  }
  boolean nextBoolean() { return next(1) != 0; }
  double nextDouble() {
    long a = next(26);
    long b = next(27);
    double v = a * 134217728 + b;
    return v / 9007199254740992.0;
  }
  double nextFloat() {
    double v = next(24);
    return v / 16777216.0;
  }
  << Polar method of Box, Muller and Marsaglia, caching the second value
     exactly as java.util.Random does (StrictMath.sqrt / StrictMath.log). >>
  double nextGaussian() {
    if (haveNextNextGaussian) {
      haveNextNextGaussian = false;
      return nextNextGaussian;
    }
    StrictMath sm = new StrictMath();
    double v1 = 0.0;
    double v2 = 0.0;
    double s = 0.0;
    boolean again = true;
    while (again) {
      v1 = 2 * nextDouble() - 1;
      v2 = 2 * nextDouble() - 1;
      s = v1 * v1 + v2 * v2;
      again = s >= 1 || s == 0;
    }
    double multiplier = sm.sqrt(-2 * sm.log(s) / s);
    nextNextGaussian = v2 * multiplier;
    haveNextNextGaussian = true;
    return v1 * multiplier;
  }
  void nextBytes(int[] bytes) {
    int i = 0;
    int n = arrayLength(bytes);
    while (i < n) {
      int rnd = nextInt();
      long u = rnd;
      if (u < 0) { u = u + 4294967296; }
      int k = 0;
      while (k < 4 && i < n) {
        int b = (int) (u % 256);
        if (b > 127) { b = b - 256; }
        bytes[i] = b;
        u = u / 256;
        i = i + 1;
        k = k + 1;
      }
    }
  }
'''

files = {
    "java.util.Arrays": ARRAYS,
    "java.util.Collections": COLLECTIONS,
    "java.util.Optional": OPTIONAL,
    "java.util.OptionalInt": "@import java.java.util\n" + optional_prim("OptionalInt", "int", "getAsInt", "0"),
    "java.util.OptionalLong": "@import java.java.util\n" + optional_prim("OptionalLong", "long", "getAsLong", "0"),
    "java.util.OptionalDouble": "@import java.java.util\n" + optional_prim("OptionalDouble", "double", "getAsDouble", "0.0"),
    "java.util.Random": RANDOM,
}
for k, v in files.items():
    (OUT / f"{k}.body").write_text(v)
print("wrote", len(files))

implement.apply_dir(OUT.parent if OUT.suffix == ".body" else OUT)
