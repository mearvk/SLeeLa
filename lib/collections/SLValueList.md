# SLValueList

`SLValueList` is the first non-generic heterogeneous collection API. It stores VM tagged values through `Any[]`, including primitives, strings, null, arrays, and class/struct references. Stored references are handles, so the intended behavior is identity-preserving rather than object copying.

## API

- `add(Any value)`
- `Any get(int index)`
- `boolean set(int index, Any value)`
- `boolean removeAt(int index)`
- `void remove()` removes the final logical item
- `void clear()`
- `int length()`
- `boolean empty()`

Indices are zero-based. Invalid reads return null; invalid writes/removals return false. Removed entries are nulled so the list does not keep otherwise-unused object references alive; allocated array capacity is retained for reuse.

## Current boundary

The compiler's `Any` type permits mixed values to be stored and passed through methods whose parameters are also `Any`. It does not make arbitrary concrete class members accessible through an `Any` reference. Use `instanceof` to test a runtime tag/class hierarchy and a checked cast such as `(Customer) item` before accessing concrete fields. A mismatched non-null cast raises a controlled VM runtime error; casting null to a reference type preserves null.

Ordinary typed arrays retain semantic element checks for direct indexing and for `arrayPush`/`arraySet` calls where the array's declared type is known. Runtime array instances still do not carry an element-type contract, so native/unknown-typed boundaries need further runtime enforcement.

The implementation has not yet been compiled or executed in this environment. See `SLList-Heterogeneous-Design.md` for the full regression matrix.
