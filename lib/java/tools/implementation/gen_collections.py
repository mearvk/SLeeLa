#!/usr/bin/env python3
"""Emit bodies for the java.util collection classes.

Element/key/value type is Any (generic erasure). All classes share these
building blocks, emitted textually into each class because base-class field
initializers do not run for subclass instances and dispatch is static:

  * VALUE helpers   -- hashing, equality and natural ordering of Any values
  * SEQ core        -- a circular Any[] buffer with indexed access, used by
                       ArrayList, Vector, Stack, LinkedList and ArrayDeque
  * HASH core       -- an insertion-ordered open-addressing hash table, used by
                       HashMap, LinkedHashMap, HashSet, LinkedHashSet
  * SORTED core     -- a sorted Any[] with binary search, used by TreeMap and
                       TreeSet
"""
from pathlib import Path
import tempfile
import implement

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))

VALUE = r'''
  < value semantics shared by the collections (hash, equality, natural order) >
  int gcSlot;
  int utfOps;
  << Periodic short-term GC hint: Java UTF-16 String handles used for hashing
     and ordering are reclaimed by the collector, not on scope exit. >>
  void utfTick() {
    utfOps = utfOps + 1;
    if (utfOps % 512 == 0) { gcSlot = gc 40; }
  }
  << Java hashCode of a value: int and long as Integer/Long.hashCode, boolean
     as Boolean.hashCode, double as Double.hashCode, everything else as the
     Java String hash of its SLeeLa text (a String hashes exactly as in Java;
     an object hashes by its identity text). >>
  int hashOf(Any o) {
    if (o == null) { return 0; }
    if (o instanceof boolean) { if ((boolean) o) { return 1231; } return 1237; }
    if (o instanceof int) { return wrapHash((long) ((int) o)); }
    if (o instanceof long) { return wrapHash((long) o); }
    if (o instanceof double) { Double d = new Double((double) o); return d.hashCode(); }
    utfTick();
    return javaStringHashCode(javaStringFromUtf8("" + o));
  }
  int wrapHash(long v) {
    long hi = v / 4294967296;
    long lo = v % 4294967296;
    if (lo < 0) { lo = lo + 4294967296; hi = hi - 1; }
    if (hi < 0) { hi = hi + 4294967296; }
    long h = lo;
    if (hi != 0) {
      long r = 0;
      long bit = 1;
      long x = lo;
      long y = hi;
      int i = 0;
      while (i < 32) {
        if ((x % 2) != (y % 2)) { r = r + bit; }
        x = x / 2; y = y / 2; bit = bit * 2; i = i + 1;
      }
      h = r;
    }
    if (h > 2147483647) { h = h - 4294967296; }
    return (int) h;
  }
  int wrap32(long v) {
    long m = v % 4294967296;
    if (m < 0) { m = m + 4294967296; }
    if (m > 2147483647) { m = m - 4294967296; }
    return (int) m;
  }
  << Value equality: numbers and strings by value, objects by identity. >>
  boolean same(Any a, Any b) { return a == b; }
  << Natural ordering (Comparable): numbers numerically, everything else by
     Java String.compareTo of the SLeeLa text. >>
  int compareValues(Any a, Any b) {
    if (a == null || b == null) { throw new NullPointerException(null); }
    boolean an = a instanceof int || a instanceof long || a instanceof double;
    boolean bn = b instanceof int || b instanceof long || b instanceof double;
    if (an && bn) {
      double x = (double) a;
      double y = (double) b;
      if (x < y) { return -1; }
      if (x > y) { return 1; }
      return 0;
    }
    if (a instanceof boolean && b instanceof boolean) {
      boolean p = (boolean) a;
      boolean q = (boolean) b;
      if (p == q) { return 0; }
      if (p) { return 1; }
      return -1;
    }
    utfTick();
    return javaStringCompareTo(javaStringFromUtf8("" + a), javaStringFromUtf8("" + b));
  }
  String textOf(Any o) {
    if (o == null) { return "null"; }
    return "" + o;
  }
  << Elements of any collection implemented in this package, as an array. >>
  Any[] elementsOf(Any c) {
    if (c == null) { throw new NullPointerException(null); }
    if (c instanceof ArrayList) { ArrayList x1 = (ArrayList) c; return x1.toArray(); }
    if (c instanceof LinkedList) { LinkedList x2 = (LinkedList) c; return x2.toArray(); }
    if (c instanceof ArrayDeque) { ArrayDeque x3 = (ArrayDeque) c; return x3.toArray(); }
    if (c instanceof Vector) { Vector x4 = (Vector) c; return x4.toArray(); }
    if (c instanceof HashSet) { HashSet x5 = (HashSet) c; return x5.toArray(); }
    if (c instanceof TreeSet) { TreeSet x6 = (TreeSet) c; return x6.toArray(); }
    if (c instanceof PriorityQueue) { PriorityQueue x7 = (PriorityQueue) c; return x7.toArray(); }
    throw new UnsupportedOperationException("not a java.util collection implemented in SLeeLa");
  }
  IndexOutOfBoundsException outOfBounds(int index, int length) {
    return new IndexOutOfBoundsException("Index " + index + " out of bounds for length " + length);
  }
'''

SEQ = r'''
  < circular Any[] buffer: element i lives at data[(head + i) % capacity] >
  Any[] data;
  int head;
  int count;
  int modCount;
  void seqInit(int capacity) {
    int c = capacity;
    if (c < 4) { c = 4; }
    this.data = new Any[c];
    this.head = 0;
    this.count = 0;
    this.modCount = 0;
    this.gcSlot = 0;
    this.utfOps = 0;
  }
  int slot(int i) { return (head + i) % arrayLength(this.data); }
  Any at(int i) { return this.data[slot(i)]; }
  void putAt(int i, Any e) { this.data[slot(i)] = e; }
  void grow(int minCapacity) {
    int cap = arrayLength(this.data);
    if (minCapacity <= cap) { return; }
    int nc = cap + cap / 2 + 1;
    if (nc < minCapacity) { nc = minCapacity; }
    Any[] nd = new Any[nc];
    int i = 0;
    while (i < count) { nd[i] = at(i); i = i + 1; }
    this.data = nd;
    head = 0;
  }
  void insertAt(int index, Any e) {
    grow(count + 1);
    if (index == 0) {
      head = (head - 1 + arrayLength(this.data)) % arrayLength(this.data);
      this.data[head] = e;
    } else {
      int i = count;
      while (i > index) { putAt(i, at(i - 1)); i = i - 1; }
      putAt(index, e);
    }
    count = count + 1;
    modCount = modCount + 1;
  }
  Any deleteAt(int index) {
    Any old = at(index);
    if (index == 0) {
      this.data[head] = null;
      head = (head + 1) % arrayLength(this.data);
    } else {
      int i = index;
      while (i < count - 1) { putAt(i, at(i + 1)); i = i + 1; }
      putAt(count - 1, null);
    }
    count = count - 1;
    modCount = modCount + 1;
    return old;
  }
  void checkElementIndex(int index) {
    if (index < 0 || index >= count) { throw outOfBounds(index, count); }
  }
  void checkPositionIndex(int index) {
    if (index < 0 || index > count) { throw outOfBounds(index, count); }
  }

  < java.util.Collection >
  int size() { return count; }
  boolean isEmpty() { return count == 0; }
  boolean contains(Any o) { return indexOf(o) >= 0; }
  Any[] toArray() {
    Any[] out = new Any[count];
    int i = 0;
    while (i < count) { out[i] = at(i); i = i + 1; }
    return out;
  }
  void clear() {
    int i = 0;
    while (i < count) { putAt(i, null); i = i + 1; }
    count = 0;
    head = 0;
    modCount = modCount + 1;
  }
  boolean containsAll(Any c) {
    Any[] xs = elementsOf(c);
    int i = 0;
    while (i < arrayLength(xs)) { if (!contains(xs[i])) { return false; } i = i + 1; }
    return true;
  }
  boolean removeObject(Any o) {
    int i = indexOf(o);
    if (i < 0) { return false; }
    deleteAt(i);
    return true;
  }
  boolean removeAll(Any c) {
    Any[] xs = elementsOf(c);
    boolean changed = false;
    int i = count - 1;
    while (i >= 0) {
      Any e = at(i);
      int j = 0;
      boolean hit = false;
      while (j < arrayLength(xs)) { if (same(xs[j], e)) { hit = true; } j = j + 1; }
      if (hit) { deleteAt(i); changed = true; }
      i = i - 1;
    }
    return changed;
  }
  boolean retainAll(Any c) {
    Any[] xs = elementsOf(c);
    boolean changed = false;
    int i = count - 1;
    while (i >= 0) {
      Any e = at(i);
      int j = 0;
      boolean hit = false;
      while (j < arrayLength(xs)) { if (same(xs[j], e)) { hit = true; } j = j + 1; }
      if (!hit) { deleteAt(i); changed = true; }
      i = i - 1;
    }
    return changed;
  }
  Iterator iterator() { return new Iterator(toArray()); }
  String toString() {
    String sb = "[";
    int i = 0;
    while (i < count) {
      if (i > 0) { sb = sb + ", "; }
      sb = sb + textOf(at(i));
      i = i + 1;
    }
    return sb + "]";
  }
  int hashCode() {
    int h = 1;
    int i = 0;
    while (i < count) { h = wrap32(31 * h + hashOf(at(i))); i = i + 1; }
    return h;
  }
  << Java List.equals: same size and pairwise-equal elements in order. >>
  boolean equals(Any o) {
    if (o == null) { return false; }
    Any[] xs = elementsOf(o);
    if (arrayLength(xs) != count) { return false; }
    int i = 0;
    while (i < count) { if (!same(xs[i], at(i))) { return false; } i = i + 1; }
    return true;
  }

  < java.util.List >
  Any get(int index) { checkElementIndex(index); return at(index); }
  Any set(int index, Any element) {
    checkElementIndex(index);
    Any old = at(index);
    putAt(index, element);
    return old;
  }
  void addAt(int index, Any element) { checkPositionIndex(index); insertAt(index, element); }
  Any remove(int index) { checkElementIndex(index); return deleteAt(index); }
  int indexOf(Any o) {
    int i = 0;
    while (i < count) { if (same(at(i), o)) { return i; } i = i + 1; }
    return -1;
  }
  int lastIndexOf(Any o) {
    int i = count - 1;
    while (i >= 0) { if (same(at(i), o)) { return i; } i = i - 1; }
    return -1;
  }
  boolean addAllAt(int index, Any c) {
    checkPositionIndex(index);
    Any[] xs = elementsOf(c);
    int i = 0;
    while (i < arrayLength(xs)) { insertAt(index + i, xs[i]); i = i + 1; }
    return arrayLength(xs) > 0;
  }
  << Natural-order sort (stable merge sort). A Comparator argument is not
     dispatched yet, so a non-null comparator is rejected. >>
  void sort(Any c) {
    if (c != null) { throw new UnsupportedOperationException("Comparator dispatch is not yet supported; pass null for natural ordering"); }
    Any[] a = toArray();
    Any[] tmp = new Any[count];
    mergeSort(a, tmp, 0, count);
    int i = 0;
    while (i < count) { putAt(i, a[i]); i = i + 1; }
    modCount = modCount + 1;
  }
  void mergeSort(Any[] a, Any[] tmp, int lo, int hi) {
    if (hi - lo < 2) { return; }
    int mid = (lo + hi) / 2;
    mergeSort(a, tmp, lo, mid);
    mergeSort(a, tmp, mid, hi);
    int i = lo;
    int j = mid;
    int k = lo;
    while (i < mid && j < hi) {
      if (compareValues(a[j], a[i]) < 0) { tmp[k] = a[j]; j = j + 1; }
      else { tmp[k] = a[i]; i = i + 1; }
      k = k + 1;
    }
    while (i < mid) { tmp[k] = a[i]; i = i + 1; k = k + 1; }
    while (j < hi) { tmp[k] = a[j]; j = j + 1; k = k + 1; }
    k = lo;
    while (k < hi) { a[k] = tmp[k]; k = k + 1; }
  }

  < java.util.SequencedCollection (Java 21+) >
  Any getFirst() { if (count == 0) { throw new NoSuchElementException(null); } return at(0); }
  Any getLast() { if (count == 0) { throw new NoSuchElementException(null); } return at(count - 1); }
  void addFirst(Any e) { insertAt(0, e); }
  void addLast(Any e) { insertAt(count, e); }
  Any removeFirst() { if (count == 0) { throw new NoSuchElementException(null); } return deleteAt(0); }
  Any removeLast() { if (count == 0) { throw new NoSuchElementException(null); } return deleteAt(count - 1); }
'''

DEQUE = r'''
  < java.util.Queue / java.util.Deque >
  boolean offer(Any e) { addLast(e); return true; }
  boolean offerFirst(Any e) { addFirst(e); return true; }
  boolean offerLast(Any e) { addLast(e); return true; }
  Any poll() { if (count == 0) { return null; } return deleteAt(0); }
  Any pollFirst() { if (count == 0) { return null; } return deleteAt(0); }
  Any pollLast() { if (count == 0) { return null; } return deleteAt(count - 1); }
  Any peek() { if (count == 0) { return null; } return at(0); }
  Any peekFirst() { if (count == 0) { return null; } return at(0); }
  Any peekLast() { if (count == 0) { return null; } return at(count - 1); }
  Any element() { return getFirst(); }
  void push(Any e) { addFirst(e); }
  Any pop() { return removeFirst(); }
  boolean removeFirstOccurrence(Any o) { return removeObject(o); }
  boolean removeLastOccurrence(Any o) {
    int i = lastIndexOf(o);
    if (i < 0) { return false; }
    deleteAt(i);
    return true;
  }
  Iterator descendingIterator() {
    Any[] out = new Any[count];
    int i = 0;
    while (i < count) { out[i] = at(count - 1 - i); i = i + 1; }
    return new Iterator(out);
  }
'''


def list_class(name, java, ctor_extra="", extra="", extends=None, add_null_check=False):
    nullchk = 'if (e == null) { throw new NullPointerException(null); }\n    ' if add_null_check else ""
    return (f"@extends {extends}\n" if extends else "") + f'''  < ---- {java} members (procedural implementation in SLeeLa) ---- >
  << {java} over Any elements (generic erasure). Equality of elements is SLeeLa
     value equality (numbers and strings by value, objects by identity);
     natural ordering compares numbers numerically and other values by Java
     String order of their text. Java overloads are renamed by role:
     add(int, E) is addAt, remove(Object) is removeObject, addAll(int, c) is
     addAllAt. iterator() returns a snapshot java.util.Iterator. >>
  {name}() {{ seqInit(10);{ctor_extra} }}

  boolean add(Any e) {{
    {nullchk}insertAt(count, e);
    return true;
  }}
  boolean addAll(Any c) {{
    Any[] xs = elementsOf(c);
    int i = 0;
    while (i < arrayLength(xs)) {{ add(xs[i]); i = i + 1; }}
    return arrayLength(xs) > 0;
  }}
  {name} subList(int fromIndex, int toIndex) {{
    if (fromIndex < 0 || toIndex > count || fromIndex > toIndex) {{
      throw new IndexOutOfBoundsException("fromIndex: " + fromIndex + ", toIndex: " + toIndex + ", size: " + count);
    }}
    {name} out = new {name}();
    int i = fromIndex;
    while (i < toIndex) {{ out.add(at(i)); i = i + 1; }}
    return out;
  }}
  {name} reversed() {{
    {name} out = new {name}();
    int i = count - 1;
    while (i >= 0) {{ out.add(at(i)); i = i - 1; }}
    return out;
  }}
  void ensureCapacity(int minCapacity) {{ grow(minCapacity); }}
  void trimToSize() {{ }}
''' + extra + SEQ + VALUE


VECTOR_EXTRA = r'''
  < java.util.Vector legacy API >
  Any elementAt(int index) { return get(index); }
  Any firstElement() { if (count == 0) { throw new NoSuchElementException(null); } return at(0); }
  Any lastElement() { if (count == 0) { throw new NoSuchElementException(null); } return at(count - 1); }
  void addElement(Any obj) { insertAt(count, obj); }
  void insertElementAt(Any obj, int index) { addAt(index, obj); }
  void setElementAt(Any obj, int index) { set(index, obj); }
  void removeElementAt(int index) { remove(index); }
  boolean removeElement(Any obj) { return removeObject(obj); }
  void removeAllElements() { clear(); }
  int capacity() { return arrayLength(this.data); }
  void setSize(int newSize) {
    while (count < newSize) { insertAt(count, null); }
    while (count > newSize) { deleteAt(count - 1); }
  }
'''

STACK = r'''@extends Vector
  < ---- java.util.Stack members (procedural implementation in SLeeLa) ---- >
  << java.util.Stack: the LIFO view of the Vector it extends. The top of the
     stack is the last element of the Vector, as in Java. >>
  Stack() { seqInit(10); }

  Any push(Any item) { insertAt(count, item); return item; }
  Any pop() {
    if (count == 0) { throw new EmptyStackException(); }
    return deleteAt(count - 1);
  }
  Any peek() {
    if (count == 0) { throw new EmptyStackException(); }
    return at(count - 1);
  }
  boolean empty() { return count == 0; }
  << 1-based distance from the top of the stack, or -1. >>
  int search(Any o) {
    int i = lastIndexOf(o);
    if (i >= 0) { return count - i; }
    return -1;
  }
'''

ITERATOR = r'''  < ---- java.util.Iterator members (procedural implementation in SLeeLa) ---- >
  << A concrete iterator over a snapshot of a collection (the collections
     return one from iterator()). remove() is optional in Java and is not
     supported by a snapshot. Note the Java name next() is kept. >>
  Any[] items;
  int cursor;
  Iterator(Any[] elements) { this.items = elements; this.cursor = 0; }
  boolean hasNext() { return cursor < arrayLength(this.items); }
  Any next() {
    if (cursor >= arrayLength(this.items)) { throw new NoSuchElementException(null); }
    Any e = this.items[cursor];
    cursor = cursor + 1;
    return e;
  }
  void remove() { throw new UnsupportedOperationException("remove"); }
'''

HASH = r'''
  << insertion-ordered hash table: entries are appended to keys/vals/hashes;
     table[] holds entry index + 1 (0 = empty, -1 = deleted) with linear probing >>
  Any[] keys;
  Any[] vals;
  int[] hashes;
  boolean[] live;
  int[] table;
  int used;
  int count;
  int modCount;
  << New arrays hold null until written, so int/boolean tables are cleared
     explicitly to the Java defaults. >>
  int[] zeros(int n) {
    int[] a = new int[n];
    int i = 0;
    while (i < n) { a[i] = 0; i = i + 1; }
    return a;
  }
  boolean[] falses(int n) {
    boolean[] a = new boolean[n];
    int i = 0;
    while (i < n) { a[i] = false; i = i + 1; }
    return a;
  }
  void hashInit(int capacity) {
    int c = 16;
    while (c < capacity * 2) { c = c * 2; }
    this.keys = new Any[c / 2];
    this.vals = new Any[c / 2];
    this.hashes = zeros(c / 2);
    this.live = falses(c / 2);
    this.table = zeros(c);
    this.used = 0;
    this.count = 0;
    this.modCount = 0;
    this.gcSlot = 0;
    this.utfOps = 0;
  }
  int spread(int h) {
    long v = h;
    if (v < 0) { v = v + 4294967296; }
    return (int) (v % 2147483647);
  }
  << Entry index of key, or -1. >>
  int findEntry(Any key, int h) {
    int cap = arrayLength(this.table);
    int i = spread(h) % cap;
    int probes = 0;
    while (probes < cap) {
      int t = this.table[i];
      if (t == 0) { return -1; }
      if (t > 0) {
        int e = t - 1;
        if (this.hashes[e] == h && same(this.keys[e], key)) { return e; }
      }
      i = (i + 1) % cap;
      probes = probes + 1;
    }
    return -1;
  }
  void rehash(int newCap) {
    Any[] ok = this.keys;
    Any[] ov = this.vals;
    int[] oh = this.hashes;
    boolean[] ol = this.live;
    int ou = used;
    this.keys = new Any[newCap / 2];
    this.vals = new Any[newCap / 2];
    this.hashes = zeros(newCap / 2);
    this.live = falses(newCap / 2);
    this.table = zeros(newCap);
    used = 0;
    int j = 0;
    while (j < ou) {
      if (ol[j]) { appendEntry(ok[j], ov[j], oh[j]); }
      j = j + 1;
    }
  }
  void appendEntry(Any key, Any value, int h) {
    int e = used;
    this.keys[e] = key;
    this.vals[e] = value;
    this.hashes[e] = h;
    this.live[e] = true;
    used = used + 1;
    int cap = arrayLength(this.table);
    int i = spread(h) % cap;
    while (this.table[i] > 0) { i = (i + 1) % cap; }
    this.table[i] = e + 1;
  }
  << Insert or replace; returns the previous value (null when absent). >>
  Any putEntry(Any key, Any value) {
    int h = hashOf(key);
    int e = findEntry(key, h);
    if (e >= 0) {
      Any old = this.vals[e];
      this.vals[e] = value;
      return old;
    }
    if (used + 1 > arrayLength(this.keys)) {
      int cap = arrayLength(this.table);
      if (count * 2 >= arrayLength(this.keys)) { cap = cap * 2; }
      rehash(cap);
    }
    appendEntry(key, value, h);
    count = count + 1;
    modCount = modCount + 1;
    return null;
  }
  Any removeEntry(Any key) {
    int h = hashOf(key);
    int cap = arrayLength(this.table);
    int i = spread(h) % cap;
    int probes = 0;
    while (probes < cap) {
      int t = this.table[i];
      if (t == 0) { return null; }
      if (t > 0) {
        int e = t - 1;
        if (this.hashes[e] == h && same(this.keys[e], key)) {
          Any old = this.vals[e];
          this.table[i] = -1;
          this.live[e] = false;
          this.keys[e] = null;
          this.vals[e] = null;
          count = count - 1;
          modCount = modCount + 1;
          return old;
        }
      }
      i = (i + 1) % cap;
      probes = probes + 1;
    }
    return null;
  }
  boolean hasKey(Any key) { return findEntry(key, hashOf(key)) >= 0; }
  Any valueFor(Any key) {
    int e = findEntry(key, hashOf(key));
    if (e < 0) { return null; }
    return this.vals[e];
  }
  void clearEntries() { hashInit(8); }
  Any[] liveKeys() {
    Any[] out = new Any[count];
    int k = 0;
    int j = 0;
    while (j < used) { if (this.live[j]) { out[k] = this.keys[j]; k = k + 1; } j = j + 1; }
    return out;
  }
  Any[] liveValues() {
    Any[] out = new Any[count];
    int k = 0;
    int j = 0;
    while (j < used) { if (this.live[j]) { out[k] = this.vals[j]; k = k + 1; } j = j + 1; }
    return out;
  }
'''


def map_api(name, java, keyset_cls):
    return f'''
  < java.util.Map >
  int size() {{ return count; }}
  boolean isEmpty() {{ return count == 0; }}
  Any put(Any key, Any value) {{ return putEntry(key, value); }}
  Any get(Any key) {{ return valueFor(key); }}
  Any getOrDefault(Any key, Any defaultValue) {{
    if (containsKey(key)) {{ return get(key); }}
    return defaultValue;
  }}
  boolean containsKey(Any key) {{ return hasKey(key); }}
  boolean containsValue(Any value) {{
    Any[] vs = valuesArray();
    int i = 0;
    while (i < arrayLength(vs)) {{ if (same(vs[i], value)) {{ return true; }} i = i + 1; }}
    return false;
  }}
  Any remove(Any key) {{ return removeEntry(key); }}
  void clear() {{ clearEntries(); }}
  Any putIfAbsent(Any key, Any value) {{
    Any v = get(key);
    if (v == null) {{ put(key, value); }}
    return v;
  }}
  Any replace(Any key, Any value) {{
    if (containsKey(key)) {{ return put(key, value); }}
    return null;
  }}
  boolean replaceValue(Any key, Any oldValue, Any newValue) {{
    if (containsKey(key) && same(get(key), oldValue)) {{ put(key, newValue); return true; }}
    return false;
  }}
  boolean removeMapping(Any key, Any value) {{
    if (containsKey(key) && same(get(key), value)) {{ remove(key); return true; }}
    return false;
  }}
  void putAll(Any m) {{
    if (m instanceof HashMap) {{ HashMap hm = (HashMap) m; copyPairs(hm.keysArray(), hm.valuesArray()); return; }}
    if (m instanceof TreeMap) {{ TreeMap tm = (TreeMap) m; copyPairs(tm.keysArray(), tm.valuesArray()); return; }}
    throw new UnsupportedOperationException("not a java.util map implemented in SLeeLa");
  }}
  void copyPairs(Any[] ks, Any[] vs) {{
    int i = 0;
    while (i < arrayLength(ks)) {{ put(ks[i], vs[i]); i = i + 1; }}
  }}
  {keyset_cls} keySet() {{
    {keyset_cls} s = new {keyset_cls}();
    Any[] ks = keysArray();
    int i = 0;
    while (i < arrayLength(ks)) {{ s.add(ks[i]); i = i + 1; }}
    return s;
  }}
  ArrayList values() {{
    ArrayList l = new ArrayList();
    Any[] vs = valuesArray();
    int i = 0;
    while (i < arrayLength(vs)) {{ l.add(vs[i]); i = i + 1; }}
    return l;
  }}
  << Snapshot of the mappings as KeyValueHolder entries (Map.Entry). >>
  ArrayList entrySet() {{
    ArrayList l = new ArrayList();
    Any[] ks = keysArray();
    Any[] vs = valuesArray();
    int i = 0;
    while (i < arrayLength(ks)) {{ l.add(new KeyValueHolder(ks[i], vs[i])); i = i + 1; }}
    return l;
  }}
  String toString() {{
    Any[] ks = keysArray();
    Any[] vs = valuesArray();
    String sb = "{{";
    int i = 0;
    while (i < arrayLength(ks)) {{
      if (i > 0) {{ sb = sb + ", "; }}
      sb = sb + textOf(ks[i]) + "=" + textOf(vs[i]);
      i = i + 1;
    }}
    return sb + "}}";
  }}
  << Java Map.hashCode: the sum of key.hashCode() ^ value.hashCode(). >>
  int hashCode() {{
    Any[] ks = keysArray();
    Any[] vs = valuesArray();
    long h = 0;
    int i = 0;
    while (i < arrayLength(ks)) {{
      Integer I = new Integer(0);
      long a = I.toUnsignedLong(hashOf(ks[i]));
      long b = I.toUnsignedLong(hashOf(vs[i]));
      long x = 0;
      long bit = 1;
      int k = 0;
      while (k < 32) {{
        if ((a % 2) != (b % 2)) {{ x = x + bit; }}
        a = a / 2; b = b / 2; bit = bit * 2; k = k + 1;
      }}
      h = h + x;
      i = i + 1;
    }}
    return wrap32(h);
  }}
  boolean equals(Any o) {{
    Any[] ks = null;
    Any[] vs = null;
    if (o instanceof HashMap) {{ HashMap hm = (HashMap) o; ks = hm.keysArray(); vs = hm.valuesArray(); }}
    if (o instanceof TreeMap) {{ TreeMap tm = (TreeMap) o; ks = tm.keysArray(); vs = tm.valuesArray(); }}
    if (ks == null) {{ return false; }}
    if (arrayLength(ks) != count) {{ return false; }}
    int i = 0;
    while (i < arrayLength(ks)) {{
      if (!containsKey(ks[i]) || !same(get(ks[i]), vs[i])) {{ return false; }}
      i = i + 1;
    }}
    return true;
  }}
'''


def hashmap(name, java, extends=None):
    return (f"@extends {extends}\n" if extends else "") + f'''  < ---- {java} members (procedural implementation in SLeeLa) ---- >
  << {java} over Any keys and values. Iteration order is insertion order
     (a valid HashMap order, and exactly the LinkedHashMap contract). Keys
     match by SLeeLa value equality and hash with the Java hashCode of
     numbers, booleans and Strings. keySet/values/entrySet are snapshots, not
     live views. compute/merge (functional arguments) are not provided until
     lambda dispatch lands. >>
  {name}() {{ hashInit(8); }}
  Any[] keysArray() {{ return liveKeys(); }}
  Any[] valuesArray() {{ return liveValues(); }}
''' + map_api(name, java, "LinkedHashSet" if name == "LinkedHashMap" else "HashSet") + HASH + VALUE


def hashset(name, java, extends=None):
    return (f"@extends {extends}\n" if extends else "") + f'''  < ---- {java} members (procedural implementation in SLeeLa) ---- >
  << {java} over Any elements: an insertion-ordered hash table of keys (a
     valid HashSet order, and exactly the LinkedHashSet contract). Elements
     match by SLeeLa value equality. iterator() is a snapshot. >>
  {name}() {{ hashInit(8); }}

  int size() {{ return count; }}
  boolean isEmpty() {{ return count == 0; }}
  boolean add(Any e) {{
    if (hasKey(e)) {{ return false; }}
    putEntry(e, true);
    return true;
  }}
  boolean contains(Any o) {{ return hasKey(o); }}
  boolean remove(Any o) {{
    if (!hasKey(o)) {{ return false; }}
    removeEntry(o);
    return true;
  }}
  void clear() {{ clearEntries(); }}
  Any[] toArray() {{ return liveKeys(); }}
  Iterator iterator() {{ return new Iterator(liveKeys()); }}
  boolean addAll(Any c) {{
    Any[] xs = elementsOf(c);
    boolean changed = false;
    int i = 0;
    while (i < arrayLength(xs)) {{ if (add(xs[i])) {{ changed = true; }} i = i + 1; }}
    return changed;
  }}
  boolean containsAll(Any c) {{
    Any[] xs = elementsOf(c);
    int i = 0;
    while (i < arrayLength(xs)) {{ if (!contains(xs[i])) {{ return false; }} i = i + 1; }}
    return true;
  }}
  boolean removeAll(Any c) {{
    Any[] xs = elementsOf(c);
    boolean changed = false;
    int i = 0;
    while (i < arrayLength(xs)) {{ if (remove(xs[i])) {{ changed = true; }} i = i + 1; }}
    return changed;
  }}
  boolean retainAll(Any c) {{
    Any[] xs = elementsOf(c);
    Any[] mine = liveKeys();
    boolean changed = false;
    int i = 0;
    while (i < arrayLength(mine)) {{
      boolean hit = false;
      int j = 0;
      while (j < arrayLength(xs)) {{ if (same(xs[j], mine[i])) {{ hit = true; }} j = j + 1; }}
      if (!hit) {{ remove(mine[i]); changed = true; }}
      i = i + 1;
    }}
    return changed;
  }}
  String toString() {{
    Any[] ks = liveKeys();
    String sb = "[";
    int i = 0;
    while (i < arrayLength(ks)) {{
      if (i > 0) {{ sb = sb + ", "; }}
      sb = sb + textOf(ks[i]);
      i = i + 1;
    }}
    return sb + "]";
  }}
  << Java Set.hashCode: the sum of the element hash codes. >>
  int hashCode() {{
    Any[] ks = liveKeys();
    long h = 0;
    int i = 0;
    while (i < arrayLength(ks)) {{ h = h + hashOf(ks[i]); i = i + 1; }}
    return wrap32(h);
  }}
  boolean equals(Any o) {{
    if (!(o instanceof HashSet) && !(o instanceof TreeSet)) {{ return false; }}
    Any[] xs = elementsOf(o);
    if (arrayLength(xs) != count) {{ return false; }}
    return containsAll(o);
  }}
''' + HASH + VALUE


SORTED = r'''
  < sorted parallel arrays with binary search (natural ordering) >
  Any[] keys;
  Any[] vals;
  int count;
  int modCount;
  void sortedInit() {
    this.keys = new Any[8];
    this.vals = new Any[8];
    this.count = 0;
    this.modCount = 0;
    this.gcSlot = 0;
    this.utfOps = 0;
  }
  << Index of key if present, else -(insertion point) - 1. >>
  int search(Any key) {
    int lo = 0;
    int hi = count - 1;
    while (lo <= hi) {
      int mid = (lo + hi) / 2;
      int c = compareValues(this.keys[mid], key);
      if (c < 0) { lo = mid + 1; }
      else { if (c > 0) { hi = mid - 1; } else { return mid; } }
    }
    return 0 - (lo + 1);
  }
  void insertSorted(int at, Any key, Any value) {
    if (count == arrayLength(this.keys)) {
      Any[] nk = new Any[count * 2];
      Any[] nv = new Any[count * 2];
      int i = 0;
      while (i < count) { nk[i] = this.keys[i]; nv[i] = this.vals[i]; i = i + 1; }
      this.keys = nk;
      this.vals = nv;
    }
    int j = count;
    while (j > at) { this.keys[j] = this.keys[j - 1]; this.vals[j] = this.vals[j - 1]; j = j - 1; }
    this.keys[at] = key;
    this.vals[at] = value;
    count = count + 1;
    modCount = modCount + 1;
  }
  void deleteSorted(int at) {
    int j = at;
    while (j < count - 1) { this.keys[j] = this.keys[j + 1]; this.vals[j] = this.vals[j + 1]; j = j + 1; }
    this.keys[count - 1] = null;
    this.vals[count - 1] = null;
    count = count - 1;
    modCount = modCount + 1;
  }
  << Navigation: index of the greatest key <= k (floor), < k (lower), the
     least key >= k (ceiling), > k (higher); -1 when there is none. >>
  int floorIndex(Any k) { int i = search(k); if (i >= 0) { return i; } return (0 - i - 1) - 1; }
  int lowerIndex(Any k) { int i = search(k); if (i >= 0) { return i - 1; } return (0 - i - 1) - 1; }
  int ceilingIndex(Any k) {
    int i = search(k);
    if (i < 0) { i = 0 - i - 1; }
    if (i >= count) { return -1; }
    return i;
  }
  int higherIndex(Any k) {
    int i = search(k);
    if (i >= 0) { i = i + 1; } else { i = 0 - i - 1; }
    if (i >= count) { return -1; }
    return i;
  }
  Any keyAtOrNull(int i) { if (i < 0) { return null; } return this.keys[i]; }
  Any[] sliceKeys() {
    Any[] out = new Any[count];
    int i = 0;
    while (i < count) { out[i] = this.keys[i]; i = i + 1; }
    return out;
  }
  Any[] sliceVals() {
    Any[] out = new Any[count];
    int i = 0;
    while (i < count) { out[i] = this.vals[i]; i = i + 1; }
    return out;
  }
'''

TREEMAP = '''@extends AbstractMap
  < ---- java.util.TreeMap members (procedural implementation in SLeeLa) ---- >
  << java.util.TreeMap over Any keys in natural ordering (numbers
     numerically, other keys by Java String order of their text), stored as
     sorted arrays with binary search, so iteration is ascending key order.
     A Comparator is not dispatched yet. Navigation methods return keys;
     first/last on an empty map throw NoSuchElementException. >>
  TreeMap() { sortedInit(); }
  Any[] keysArray() { return sliceKeys(); }
  Any[] valuesArray() { return sliceVals(); }
  Any putEntry(Any key, Any value) {
    if (key == null) { throw new NullPointerException(null); }
    int i = search(key);
    if (i >= 0) { Any old = this.vals[i]; this.vals[i] = value; return old; }
    insertSorted(0 - i - 1, key, value);
    return null;
  }
  Any valueFor(Any key) {
    int i = search(key);
    if (i < 0) { return null; }
    return this.vals[i];
  }
  boolean hasKey(Any key) { return search(key) >= 0; }
  Any removeEntry(Any key) {
    int i = search(key);
    if (i < 0) { return null; }
    Any old = this.vals[i];
    deleteSorted(i);
    return old;
  }
  void clearEntries() { sortedInit(); }

  < java.util.NavigableMap / SortedMap >
  Any firstKey() { if (count == 0) { throw new NoSuchElementException(null); } return this.keys[0]; }
  Any lastKey() { if (count == 0) { throw new NoSuchElementException(null); } return this.keys[count - 1]; }
  Any floorKey(Any key) { return keyAtOrNull(floorIndex(key)); }
  Any lowerKey(Any key) { return keyAtOrNull(lowerIndex(key)); }
  Any ceilingKey(Any key) { return keyAtOrNull(ceilingIndex(key)); }
  Any higherKey(Any key) { return keyAtOrNull(higherIndex(key)); }
  KeyValueHolder firstEntry() { if (count == 0) { return null; } return new KeyValueHolder(this.keys[0], this.vals[0]); }
  KeyValueHolder lastEntry() { if (count == 0) { return null; } return new KeyValueHolder(this.keys[count - 1], this.vals[count - 1]); }
  KeyValueHolder pollFirstEntry() {
    if (count == 0) { return null; }
    KeyValueHolder e = new KeyValueHolder(this.keys[0], this.vals[0]);
    deleteSorted(0);
    return e;
  }
  KeyValueHolder pollLastEntry() {
    if (count == 0) { return null; }
    KeyValueHolder e = new KeyValueHolder(this.keys[count - 1], this.vals[count - 1]);
    deleteSorted(count - 1);
    return e;
  }
  << headMap(toKey) is the keys strictly less than toKey; tailMap(fromKey) is
     the keys greater than or equal to fromKey (Java defaults), as copies. >>
  TreeMap headMap(Any toKey) {
    TreeMap m = new TreeMap();
    int i = 0;
    while (i < count && compareValues(this.keys[i], toKey) < 0) { m.put(this.keys[i], this.vals[i]); i = i + 1; }
    return m;
  }
  TreeMap tailMap(Any fromKey) {
    TreeMap m = new TreeMap();
    int i = 0;
    while (i < count) { if (compareValues(this.keys[i], fromKey) >= 0) { m.put(this.keys[i], this.vals[i]); } i = i + 1; }
    return m;
  }
  TreeMap subMap(Any fromKey, Any toKey) {
    TreeMap m = new TreeMap();
    int i = 0;
    while (i < count) {
      if (compareValues(this.keys[i], fromKey) >= 0 && compareValues(this.keys[i], toKey) < 0) { m.put(this.keys[i], this.vals[i]); }
      i = i + 1;
    }
    return m;
  }
  TreeMap descendingMap() {
    TreeMap m = new TreeMap();
    int i = 0;
    while (i < count) { m.put(this.keys[i], this.vals[i]); i = i + 1; }
    return m;
  }
'''

TREESET = '''@extends AbstractSet
  < ---- java.util.TreeSet members (procedural implementation in SLeeLa) ---- >
  << java.util.TreeSet over Any elements in natural ordering (numbers
     numerically, other values by Java String order of their text), stored as
     a sorted array with binary search; iteration is ascending. >>
  TreeSet() { sortedInit(); }

  int size() { return count; }
  boolean isEmpty() { return count == 0; }
  boolean add(Any e) {
    if (e == null) { throw new NullPointerException(null); }
    int i = search(e);
    if (i >= 0) { return false; }
    insertSorted(0 - i - 1, e, null);
    return true;
  }
  boolean contains(Any o) { return search(o) >= 0; }
  boolean remove(Any o) {
    int i = search(o);
    if (i < 0) { return false; }
    deleteSorted(i);
    return true;
  }
  void clear() { sortedInit(); }
  Any[] toArray() { return sliceKeys(); }
  Iterator iterator() { return new Iterator(sliceKeys()); }
  Iterator descendingIterator() {
    Any[] out = new Any[count];
    int i = 0;
    while (i < count) { out[i] = this.keys[count - 1 - i]; i = i + 1; }
    return new Iterator(out);
  }
  boolean addAll(Any c) {
    Any[] xs = elementsOf(c);
    boolean changed = false;
    int i = 0;
    while (i < arrayLength(xs)) { if (add(xs[i])) { changed = true; } i = i + 1; }
    return changed;
  }
  boolean containsAll(Any c) {
    Any[] xs = elementsOf(c);
    int i = 0;
    while (i < arrayLength(xs)) { if (!contains(xs[i])) { return false; } i = i + 1; }
    return true;
  }
  boolean removeAll(Any c) {
    Any[] xs = elementsOf(c);
    boolean changed = false;
    int i = 0;
    while (i < arrayLength(xs)) { if (remove(xs[i])) { changed = true; } i = i + 1; }
    return changed;
  }
  Any first() { if (count == 0) { throw new NoSuchElementException(null); } return this.keys[0]; }
  Any last() { if (count == 0) { throw new NoSuchElementException(null); } return this.keys[count - 1]; }
  Any floor(Any e) { return keyAtOrNull(floorIndex(e)); }
  Any lower(Any e) { return keyAtOrNull(lowerIndex(e)); }
  Any ceiling(Any e) { return keyAtOrNull(ceilingIndex(e)); }
  Any higher(Any e) { return keyAtOrNull(higherIndex(e)); }
  Any pollFirst() { if (count == 0) { return null; } Any e = this.keys[0]; deleteSorted(0); return e; }
  Any pollLast() { if (count == 0) { return null; } Any e = this.keys[count - 1]; deleteSorted(count - 1); return e; }
  TreeSet headSet(Any toElement) {
    TreeSet s = new TreeSet();
    int i = 0;
    while (i < count && compareValues(this.keys[i], toElement) < 0) { s.add(this.keys[i]); i = i + 1; }
    return s;
  }
  TreeSet tailSet(Any fromElement) {
    TreeSet s = new TreeSet();
    int i = 0;
    while (i < count) { if (compareValues(this.keys[i], fromElement) >= 0) { s.add(this.keys[i]); } i = i + 1; }
    return s;
  }
  String toString() {
    String sb = "[";
    int i = 0;
    while (i < count) {
      if (i > 0) { sb = sb + ", "; }
      sb = sb + textOf(this.keys[i]);
      i = i + 1;
    }
    return sb + "]";
  }
  int hashCode() {
    long h = 0;
    int i = 0;
    while (i < count) { h = h + hashOf(this.keys[i]); i = i + 1; }
    return wrap32(h);
  }
  boolean equals(Any o) {
    if (!(o instanceof HashSet) && !(o instanceof TreeSet)) { return false; }
    Any[] xs = elementsOf(o);
    if (arrayLength(xs) != count) { return false; }
    return containsAll(o);
  }
'''

PQ = '''@extends AbstractQueue
  < ---- java.util.PriorityQueue members (procedural implementation in SLeeLa) ---- >
  << java.util.PriorityQueue: a binary min-heap over Any elements in natural
     ordering (numbers numerically, other values by Java String order of
     their text). Iteration order (toArray, iterator, toString) is the heap
     array order, as in Java. A Comparator is not dispatched yet. >>
  Any[] heap;
  int count;
  int modCount;
  PriorityQueue() {
    this.heap = new Any[11];
    this.count = 0;
    this.modCount = 0;
    this.gcSlot = 0;
    this.utfOps = 0;
  }
  int size() { return count; }
  boolean isEmpty() { return count == 0; }
  boolean add(Any e) { return offer(e); }
  boolean offer(Any e) {
    if (e == null) { throw new NullPointerException(null); }
    if (count == arrayLength(this.heap)) {
      Any[] nh = new Any[count * 2 + 2];
      int i = 0;
      while (i < count) { nh[i] = this.heap[i]; i = i + 1; }
      this.heap = nh;
    }
    int k = count;
    count = count + 1;
    while (k > 0) {
      int parent = (k - 1) / 2;
      if (compareValues(e, this.heap[parent]) >= 0) { break; }
      this.heap[k] = this.heap[parent];
      k = parent;
    }
    this.heap[k] = e;
    modCount = modCount + 1;
    return true;
  }
  Any peek() { if (count == 0) { return null; } return this.heap[0]; }
  Any element() { if (count == 0) { throw new NoSuchElementException(null); } return this.heap[0]; }
  Any poll() {
    if (count == 0) { return null; }
    Any result = this.heap[0];
    removeAtIndex(0);
    return result;
  }
  void removeAtIndex(int i) {
    count = count - 1;
    modCount = modCount + 1;
    if (i == count) { this.heap[i] = null; return; }
    Any moved = this.heap[count];
    this.heap[count] = null;
    siftDown(i, moved);
    if (this.heap[i] == moved) { siftUp(i, moved); }
  }
  void siftDown(int k0, Any x) {
    int k = k0;
    int half = count / 2;
    while (k < half) {
      int child = 2 * k + 1;
      int right = child + 1;
      if (right < count && compareValues(this.heap[child], this.heap[right]) > 0) { child = right; }
      if (compareValues(x, this.heap[child]) <= 0) { break; }
      this.heap[k] = this.heap[child];
      k = child;
    }
    this.heap[k] = x;
  }
  void siftUp(int k0, Any x) {
    int k = k0;
    while (k > 0) {
      int parent = (k - 1) / 2;
      if (compareValues(x, this.heap[parent]) >= 0) { break; }
      this.heap[k] = this.heap[parent];
      k = parent;
    }
    this.heap[k] = x;
  }
  boolean remove(Any o) {
    int i = 0;
    while (i < count) {
      if (same(this.heap[i], o)) { removeAtIndex(i); return true; }
      i = i + 1;
    }
    return false;
  }
  boolean contains(Any o) {
    int i = 0;
    while (i < count) { if (same(this.heap[i], o)) { return true; } i = i + 1; }
    return false;
  }
  void clear() {
    int i = 0;
    while (i < count) { this.heap[i] = null; i = i + 1; }
    count = 0;
  }
  Any[] toArray() {
    Any[] out = new Any[count];
    int i = 0;
    while (i < count) { out[i] = this.heap[i]; i = i + 1; }
    return out;
  }
  Iterator iterator() { return new Iterator(toArray()); }
  boolean addAll(Any c) {
    Any[] xs = elementsOf(c);
    int i = 0;
    while (i < arrayLength(xs)) { offer(xs[i]); i = i + 1; }
    return arrayLength(xs) > 0;
  }
  String toString() {
    String sb = "[";
    int i = 0;
    while (i < count) {
      if (i > 0) { sb = sb + ", "; }
      sb = sb + textOf(this.heap[i]);
      i = i + 1;
    }
    return sb + "]";
  }
'''

KVH = r'''  < ---- java.util.KeyValueHolder members (procedural implementation in SLeeLa) ---- >
  << The immutable Map.Entry returned by Map.entry and by the maps of this
     package (entrySet, firstEntry, ...). setValue is unsupported, as in Java. >>
  Any key;
  Any value;
  KeyValueHolder(Any k, Any v) { this.key = k; this.value = v; }
  Any getKey() { return this.key; }
  Any getValue() { return this.value; }
  Any setValue(Any v) { throw new UnsupportedOperationException("not supported"); }
  String toString() {
    String ks = "null";
    String vs = "null";
    if (this.key != null) { ks = "" + this.key; }
    if (this.value != null) { vs = "" + this.value; }
    return ks + "=" + vs;
  }
  boolean equals(Any o) {
    if (o instanceof KeyValueHolder) {
      KeyValueHolder e = (KeyValueHolder) o;
      return e.getKey() == this.key && e.getValue() == this.value;
    }
    return false;
  }
'''

files = {
    "java.util.ArrayList": list_class("ArrayList", "java.util.ArrayList", extends="AbstractList"),
    "java.util.Vector": list_class("Vector", "java.util.Vector", extra=VECTOR_EXTRA, extends="AbstractList"),
    "java.util.LinkedList": list_class("LinkedList", "java.util.LinkedList", extra=DEQUE, extends="AbstractSequentialList"),
    "java.util.ArrayDeque": list_class("ArrayDeque", "java.util.ArrayDeque", extra=DEQUE, extends="AbstractCollection", add_null_check=True),
    "java.util.Stack": STACK,
    "java.util.Iterator": ITERATOR,
    "java.util.HashMap": hashmap("HashMap", "java.util.HashMap", extends="AbstractMap"),
    "java.util.LinkedHashMap": hashmap("LinkedHashMap", "java.util.LinkedHashMap", extends="HashMap"),
    "java.util.HashSet": hashset("HashSet", "java.util.HashSet", extends="AbstractSet"),
    "java.util.LinkedHashSet": hashset("LinkedHashSet", "java.util.LinkedHashSet", extends="HashSet"),
    "java.util.TreeMap": TREEMAP + map_api("TreeMap", "java.util.TreeMap", "TreeSet") + SORTED + VALUE,
    "java.util.TreeSet": TREESET + SORTED + VALUE,
    "java.util.PriorityQueue": PQ + VALUE,
    "java.util.KeyValueHolder": KVH,
}
# ArrayDeque has no index-based List API in Java; it keeps the shared core but
# the List methods are harmless extras. Stack inherits Vector's body.
for k, v in files.items():
    # Vector, List, Map ... also exist elsewhere in /lib; select java.util.
    (OUT / f"{k}.body").write_text("@import java.java.util\n" + v)
print("wrote", len(files))

implement.apply_dir(OUT.parent if OUT.suffix == ".body" else OUT)
