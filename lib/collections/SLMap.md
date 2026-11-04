# SLMap

`SLMap` is a concrete, **String-keyed, String-valued map** in the SLeeLa
collections library, backed by two parallel VM-managed growable native arrays
(`keys[i]` maps to `vals[i]`). Keys are unique; lookup is a linear scan, which
suits small-to-moderate maps. It requires no Generics and no new VM opcodes.

## Operations

- `put(String key, String value)`: insert a new entry or overwrite an existing
  key. A `null` key is rejected and returns `false`.
- `get(String key)`: value for `key`, or `null` when the key is absent.
- `containsKey(String key)` / `indexOfKey(String key)`: key presence / position
  (`-1` when absent; a `null` key is never stored).
- `removeKey(String key)`: remove the entry, preserving order; returns `true`
  when an entry was removed.
- `keyAt(int index)` / `valueAt(int index)`: entry key/value at an
  insertion-order index, or `null` when out of range.
- `clear()`: drop every entry while retaining backing-array capacity.
- `length()` / `empty()`: entry count / emptiness.

This first version uses **String keys and values only**.

## Example

See [`SLMapBasic`](../../impl/examples/slmap_basic.sleela).
