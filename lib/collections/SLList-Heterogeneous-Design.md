# Heterogeneous SLList: design and verification plan

Status: **partial implementation committed; checked concrete-object retrieval and runtime enforcement remain outstanding**.

This document records the source/VM audit for a non-generic list that can retain values of different concrete types, including class/struct instances, and pass those values through recursive calls. It deliberately separates the current String-only `SLList` from the future heterogeneous contract.

## 1. What the current VM already supports

The core `SLValue` tagged union includes `SL_NULL`, `SL_INT`, `SL_DOUBLE`, `SL_BOOL`, `SL_STR`, `SL_STRUCT`, and `SL_ARRAY`. Structs and arrays are VM-local handles, not raw pointers. The garbage collector's value traversal marks struct/array handles stored in struct fields and array elements, and it marks globals, the last result, mailboxes, and the active stack.

The array store uses `SLValue` elements, and `arrayPush` accepts an `SLValue` at the core boundary. This makes the runtime representation a good basis for a mixed-value list.

## 2. Why the current SLList cannot yet promise this

- `lib/collections/SLList.sleela` declares `String[] values` and `add(String value)`; it is intentionally String-only.
- The semantic type system now has a source-level `Any` top type. `Any` accepts mixed values, but a value typed as `Any` is not implicitly assignable to a concrete type.
- `arrayPush` and `arraySet` lower to generic array opcodes, and the runtime array instance does not record an element-type contract. The core can therefore store a value whose runtime tag does not match the source array's declared element type if a caller bypasses static checks.
- Struct member access is compiled using the declared struct layout and a field offset. Retrieving a mixed value as the wrong struct type must be rejected before field access, not treated as a raw unchecked cast.

Until these checks exist, changing `SLList.add` to accept arbitrary objects would overstate the guarantees and could allow type confusion.

## 3. Recommended non-generic API

Keep the existing `SLList` as a concrete String list for compatibility. `SLValueList` is now added as a first implementation of mixed storage; its safe public contract is intentionally limited until runtime type inspection/casting is implemented:

- `add(Any value)` — append any supported value: null, primitive, String, array handle, or class/struct handle.
- `Any get(int index)` — return a dynamically typed value; invalid indices return null.
- `boolean set(int index, Any value)` — replace an element, false for invalid indices.
- `boolean removeAt(int index)`, `void remove()`, `void clear()`, `int length()`, `boolean empty()`.
- `String typeNameAt(int index)` and `boolean isTypeAt(int index, String typeName)` — inspect an element before using it as a concrete type.
- A checked type conversion such as `cast(TypeName, value)` (exact spelling provisional) must verify the runtime tag and struct type before the result can be used as a typed reference.

This is a dynamic top type, **not Generics**. It must preserve object identity: appending a class/struct handle stores the handle, not a serialized copy. The same handle should be safe to pass through recursive method parameters.

## 4. Required implementation safeguards

1. **Implemented:** add a source-level `Any` type to semantic analysis. Values of `Any` can be passed through `Any` parameters; `Any` does not silently convert to concrete class types.
2. **Partially implemented:** heterogeneous storage uses `Any[]`; semantic checks cover ordinary typed-array writes where the declared array type is known.
3. Semantic analysis now checks `arrayPush` and `arraySet` when the first argument has a statically known array type. Runtime array instances still do not record element contracts, so native/unknown-typed boundaries remain a gap.
4. Add a checked type-test/cast path for struct handles. Confirm both the `SL_STRUCT` tag and the exact runtime struct-type index before field access through a cast result.
5. Preserve GC reachability and alias identity across list storage, list reads, recursive calls, and collections. Do not expose raw handles as integers.

## 5. Regression test matrix

The implementation is not complete until the compiler and runtime tests cover all of the following. The current mixed-list example is illustrative and has not been compiled or executed here:

- **Mixed scalar values:** String, int, double, boolean, and null can coexist in one heterogeneous list and round-trip with the right runtime tags.
- **Mixed references:** instances of two different struct/class types can coexist; retrieving each with the correct checked type preserves identity.
- **Wrong-type rejection:** a failed cast/type check reports a controlled runtime error and never reads a field using another type's offset.
- **Typed-array integrity:** inserting a struct into a `String[]` through `arrayPush`/`arraySet` is rejected; valid typed arrays remain usable.
- **Bounds and mutation:** negative and too-large indices, append after remove, remove from empty, clear/reuse, and stable order.
- **Recursive pass-through:** a stored object is retrieved and passed down multiple recursive frames; the leaf mutates a field and the caller observes that same instance's mutation.
- **GC pressure:** retain an object only through the list, trigger a collection, then retrieve and use it; also cover arrays nested inside objects and objects nested inside arrays.
- **Capacity/exhaustion:** array growth limits and allocation failure return controlled errors without corrupting the list.
- **Compatibility:** the current String-only `SLList` and existing typed-array examples continue to compile and run.

## 6. Verification status

The repository now contains `Any`, semantic checks for typed `arrayPush`/`arraySet` calls with known array types, `SLValueList`, its API documentation, and an illustrative example. The semantic analyzer explicitly rejects casts from `Any` to a concrete struct/class and `instanceof` on `Any` until checked runtime support exists. Branches `main` and `master` contain matching file content. A local build/test run was not possible in this environment because a repository clone could not be obtained; no successful compilation or runtime test is claimed.
