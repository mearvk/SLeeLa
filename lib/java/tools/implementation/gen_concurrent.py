#!/usr/bin/env python3
"""Emit the concurrent and legacy synchronized collections.

They reuse the cores of gen_collections.py (importing it also re-applies the
java.util collections, which is idempotent). Every structural mutation runs
under VM lock 30 (reserved for java.util concurrency), elements/keys/values
are null-hostile as in Java, and the class-specific API is added.
"""
import tempfile
from pathlib import Path

import implement
import gen_collections as gc

OUT = Path(tempfile.mkdtemp(prefix="jdk28-bodies-"))
CLOCK = 30
LOCKED = [
    ("void insertAt(int index, Any e) {", "void insertAt(int index, Any e) { lock({L}); insertAtRaw(index, e); unlock({L}); }\n  void insertAtRaw(int index, Any e) {"),
    ("Any deleteAt(int index) {", "Any deleteAt(int index) { lock({L}); Any r = deleteAtRaw(index); unlock({L}); return r; }\n  Any deleteAtRaw(int index) {"),
    ("Any putEntry(Any key, Any value) {", "Any putEntry(Any key, Any value) { lock({L}); Any r = putEntryRaw(key, value); unlock({L}); return r; }\n  Any putEntryRaw(Any key, Any value) {"),
    ("Any removeEntry(Any key) {", "Any removeEntry(Any key) { lock({L}); Any r = removeEntryRaw(key); unlock({L}); return r; }\n  Any removeEntryRaw(Any key) {"),
    ("void insertSorted(int at, Any key, Any value) {", "void insertSorted(int at, Any key, Any value) { lock({L}); insertSortedRaw(at, key, value); unlock({L}); }\n  void insertSortedRaw(int at, Any key, Any value) {"),
    ("void deleteSorted(int at) {", "void deleteSorted(int at) { lock({L}); deleteSortedRaw(at); unlock({L}); }\n  void deleteSortedRaw(int at) {"),
]


def locked(text, skip=()):
    """Wrap the core mutation primitives in lock/unlock. Primitives that call
    another locked primitive are listed in skip (the VM mutex is not
    reentrant)."""
    hits = 0
    for a, b in LOCKED:
        if any(a.startswith(x) for x in skip):
            continue
        if ("  " + a) in text:
            text = text.replace("  " + a, "  " + b.replace("{L}", str(CLOCK)), 1)
            hits = hits + 1
    assert hits >= 2, "no lockable core found"
    return text


def noted(text, note):
    """Prepend a sentence to the first << >> doc block of a body."""
    head = text.index("<<")
    return text[:head] + "<< " + note + "\n     " + text[head + 3:]


NONNULL_MAP = '''
  < null-hostile Map entry points (Java NullPointerException) >
  Any put(Any key, Any value) {
    if (key == null || value == null) { throw new NullPointerException(null); }
    return putEntry(key, value);
  }
  Any get(Any key) {
    if (key == null) { throw new NullPointerException(null); }
    return valueFor(key);
  }
  boolean containsKey(Any key) {
    if (key == null) { throw new NullPointerException(null); }
    return hasKey(key);
  }
'''

LEGACY_MAP = '''
  < legacy enumeration views and contains(value) >
  boolean contains(Any value) {
    if (value == null) { throw new NullPointerException(null); }
    return containsValue(value);
  }
  Enumeration keys() { return new Enumeration(keysArray()); }
  Enumeration elements() { return new Enumeration(valuesArray()); }
'''

CHM_EXTRA = '''
  long mappingCount() { long n = count; return n; }
'''

COW_EXTRA = '''
  < java.util.concurrent.CopyOnWriteArrayList additions >
  boolean addIfAbsent(Any e) {
    if (indexOf(e) >= 0) { return false; }
    insertAt(count, e);
    return true;
  }
  int addAllAbsent(Any c) {
    Any[] xs = elementsOf(c);
    int added = 0;
    int i = 0;
    while (i < arrayLength(xs)) { if (addIfAbsent(xs[i])) { added = added + 1; } i = i + 1; }
    return added;
  }
'''


def blocking(bounded):
    cap = "this.capacityLimit" if bounded else "2147483647"
    return f'''
  << java.util.concurrent.BlockingQueue / BlockingDeque. A blocked put or
     take re-checks the queue every millisecond (sleep), so producers and
     consumers on spawned threads hand elements over as in Java. The timed
     offer / poll take the timeout in milliseconds (offerMillis, pollMillis)
     until TimeUnit can be passed through static access. >>
  int remainingCapacity() {{ return {cap} - count; }}
  void put(Any e) {{
    if (e == null) {{ throw new NullPointerException(null); }}
    while (count >= {cap}) {{ sleep(1); }}
    insertAt(count, e);
  }}
  void putFirst(Any e) {{
    if (e == null) {{ throw new NullPointerException(null); }}
    while (count >= {cap}) {{ sleep(1); }}
    insertAt(0, e);
  }}
  void putLast(Any e) {{ put(e); }}
  Any take() {{
    while (count == 0) {{ sleep(1); }}
    return deleteAt(0);
  }}
  Any takeFirst() {{ return take(); }}
  Any takeLast() {{
    while (count == 0) {{ sleep(1); }}
    return deleteAt(count - 1);
  }}
  boolean offerMillis(Any e, long timeoutMillis) {{
    if (e == null) {{ throw new NullPointerException(null); }}
    long waited = 0;
    while (count >= {cap}) {{
      if (waited >= timeoutMillis) {{ return false; }}
      sleep(1);
      waited = waited + 1;
    }}
    insertAt(count, e);
    return true;
  }}
  Any pollMillis(long timeoutMillis) {{
    long waited = 0;
    while (count == 0) {{
      if (waited >= timeoutMillis) {{ return null; }}
      sleep(1);
      waited = waited + 1;
    }}
    return deleteAt(0);
  }}
  int drainTo(Any c) {{ return drainToMax(c, 2147483647); }}
  int drainToMax(Any c, int maxElements) {{
    int n = 0;
    while (count > 0 && n < maxElements) {{
      Any e = deleteAt(0);
      if (c instanceof ArrayList) {{ ArrayList a = (ArrayList) c; a.add(e); }}
      if (c instanceof LinkedList) {{ LinkedList b = (LinkedList) c; b.add(e); }}
      if (c instanceof ArrayDeque) {{ ArrayDeque d = (ArrayDeque) c; d.add(e); }}
      if (c instanceof HashSet) {{ HashSet h = (HashSet) c; h.add(e); }}
      n = n + 1;
    }}
    return n;
  }}
'''


def queue(name, extends="AbstractQueue", bounded=False):
    java = f"java.util.concurrent.{name}"
    extra = gc.DEQUE + (blocking(bounded) if "Blocking" in name else "")
    body = gc.list_class(name, java, extra=extra, extends=extends, add_null_check=True)
    if bounded:
        body = body.replace(f"  {name}() {{ seqInit(10); }}",
                            f"  int capacityLimit;\n  {name}(int capacity) {{\n"
                            f"    if (capacity <= 0) {{ throw new IllegalArgumentException(null); }}\n"
                            f"    seqInit(capacity);\n    this.capacityLimit = capacity;\n  }}")
        body = body.replace("    if (e == null) { throw new NullPointerException(null); }\n    insertAt(count, e);\n    return true;\n  }\n  boolean addAll",
                            "    if (e == null) { throw new NullPointerException(null); }\n"
                            "    if (count >= this.capacityLimit) { throw new IllegalStateException(\"Queue full\"); }\n"
                            "    insertAt(count, e);\n    return true;\n  }\n  boolean addAll")
        body = body.replace("  boolean offer(Any e) { addLast(e); return true; }",
                            "  boolean offer(Any e) {\n    if (e == null) { throw new NullPointerException(null); }\n"
                            "    if (count >= this.capacityLimit) { return false; }\n    insertAt(count, e);\n    return true;\n  }")
        body = body.replace(f"    {name} out = new {name}();", f"    {name} out = new {name}(2147483647);")
    return locked(noted(body, f"Thread-safe: structural changes run under VM lock {CLOCK}; null elements are rejected."))


ENUMERATION = '''  < ---- java.util.Enumeration members (procedural implementation in SLeeLa) ---- >
  << A concrete enumeration over a snapshot of a collection, as returned by
     Hashtable.keys / elements and ConcurrentHashMap.keys / elements. >>
  Any[] items;
  int cursor;
  Enumeration(Any[] elements) { this.items = elements; this.cursor = 0; }
  boolean hasMoreElements() { return cursor < arrayLength(this.items); }
  Any nextElement() {
    if (cursor >= arrayLength(this.items)) { throw new NoSuchElementException(null); }
    Any e = this.items[cursor];
    cursor = cursor + 1;
    return e;
  }
  Iterator asIterator() {
    Any[] rest = new Any[arrayLength(this.items) - cursor];
    int i = 0;
    while (i < arrayLength(rest)) { rest[i] = this.items[cursor + i]; i = i + 1; }
    return new Iterator(rest);
  }
'''


def legacy_map(name, java, extends, note):
    body = gc.hashmap(name, java, extends=extends)
    for line in ("  Any put(Any key, Any value) { return putEntry(key, value); }\n",
                 "  Any get(Any key) { return valueFor(key); }\n",
                 "  boolean containsKey(Any key) { return hasKey(key); }\n"):
        assert line in body, line
        body = body.replace(line, "")
    body = body + NONNULL_MAP + LEGACY_MAP + (CHM_EXTRA if name == "ConcurrentHashMap" else "")
    return locked(noted(body, note))


def skiplist_map():
    t = gc.TREEMAP.replace("TreeMap", "ConcurrentSkipListMap").replace("java.util.ConcurrentSkipListMap", "java.util.concurrent.ConcurrentSkipListMap")
    api = gc.map_api("ConcurrentSkipListMap", "java.util.concurrent.ConcurrentSkipListMap", "ConcurrentSkipListSet")
    body = t + api + gc.SORTED + gc.VALUE
    # putEntry / removeEntry of the sorted map call insertSorted / deleteSorted
    return locked(noted(body, f"Thread-safe sorted map: structural changes run under VM lock {CLOCK}."),
                  skip=("Any putEntry", "Any removeEntry"))


def skiplist_set():
    t = gc.TREESET.replace("TreeSet", "ConcurrentSkipListSet").replace("java.util.ConcurrentSkipListSet", "java.util.concurrent.ConcurrentSkipListSet")
    return locked(noted(t + gc.SORTED + gc.VALUE, f"Thread-safe sorted set: structural changes run under VM lock {CLOCK}."))


files = {
    "java.util.Enumeration": ENUMERATION,
    "java.util.Hashtable": legacy_map("Hashtable", "java.util.Hashtable", "Dictionary",
                                      f"Synchronized (VM lock {CLOCK}) and null-hostile, as the legacy Hashtable."),
    "java.util.concurrent.ConcurrentHashMap": legacy_map("ConcurrentHashMap", "java.util.concurrent.ConcurrentHashMap", "AbstractMap",
                                                         f"Thread-safe: structural changes run under VM lock {CLOCK}; null keys and values are rejected."),
    "java.util.concurrent.CopyOnWriteArrayList": locked(noted(gc.list_class("CopyOnWriteArrayList", "java.util.concurrent.CopyOnWriteArrayList", extra=COW_EXTRA),
                                                             f"Thread-safe: every structural change runs under VM lock {CLOCK} and iterators are snapshots.")),
    "java.util.concurrent.CopyOnWriteArraySet": locked(noted(gc.hashset("CopyOnWriteArraySet", "java.util.concurrent.CopyOnWriteArraySet", extends="AbstractSet"),
                                                            f"Thread-safe (VM lock {CLOCK}), insertion ordered, snapshot iterators.")),
    "java.util.concurrent.ConcurrentLinkedQueue": queue("ConcurrentLinkedQueue"),
    "java.util.concurrent.ConcurrentLinkedDeque": queue("ConcurrentLinkedDeque", extends="AbstractCollection"),
    "java.util.concurrent.LinkedBlockingQueue": queue("LinkedBlockingQueue"),
    "java.util.concurrent.LinkedBlockingDeque": queue("LinkedBlockingDeque"),
    "java.util.concurrent.ArrayBlockingQueue": queue("ArrayBlockingQueue", bounded=True),
    "java.util.concurrent.ConcurrentSkipListMap": skiplist_map(),
    "java.util.concurrent.ConcurrentSkipListSet": skiplist_set(),
}
for k, v in files.items():
    (OUT / f"{k}.body").write_text("@import java.java.util\n" + v)
implement.apply_dir(OUT)
