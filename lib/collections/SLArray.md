# SLArray

`SLArray` is a concrete, **fixed-capacity, bounds-safe String array** in the
SLeeLa collections library. It wraps the VM-managed native array primitive
(`arrayNew`/`arrayGet`/`arraySet`/`arrayLength`) behind a forgiving, bounds-
checked API. It requires no Generics and no new VM opcodes.

Unlike `SLList` (a growable list whose logical length tracks appends),
`SLArray` has a **fixed logical length equal to its capacity**: once
`allocate(n)` is called, every index `0 .. n-1` is addressable and starts as
`null`.

> Naming note: the symbol `SLArray` now has a single definition, in the
> `collections` family. The former `core.SLArray` (which held no array storage)
> was renamed to [`SLCoreCell`](../core/SLCoreCell.sleela) so that `SLArray`
> resolves unambiguously.

## Operations

- `allocate(int n)`: (re-)allocate `n` null slots; a negative `n` is clamped to `0`.
- `length()` / `empty()`: number of slots / whether there are none.
- `inBounds(int index)`: whether `index` addresses a real slot.
- `get(int index)`: slot value, or `null` for an out-of-range index (no trap).
- `set(int index, String value)`: write a slot; returns `false` when out of range.
- `fill(String value)`: set every slot to `value`.
- `clear()`: set every slot back to `null`.
- `indexOf(String value)` / `contains(String value)`: linear search (matches `null`).
- `reverse()`: reverse the contents in place.
- `copyTo(SLArray dest, int srcPos, int destPos, int length)`: copy a range into
  another `SLArray`, clamped so the call is always safe; returns the number of
  slots actually copied.
- `toList()`: produce an independent [`SLList`](SLList.sleela) with the same values.

Indices are zero-based. Out-of-range reads return `null` and out-of-range
writes return `false`, rather than trapping, matching the forgiving contract
used across the collections family. This first version accepts **Strings only**.

## Example

See [`SLArrayBasic`](../../impl/examples/slarray_basic.sleela) for an exercise of
allocation, bounds behavior, fill/clear, search, reverse, range copy, and
conversion to `SLList`.
