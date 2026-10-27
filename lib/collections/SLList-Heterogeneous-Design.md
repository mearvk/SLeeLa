# Heterogeneous SLList: design and verification plan

Status: **audit complete; implementation remains blocked on type-safety work**.

This document records the source/VM audit for a non-generic list that can retain values of different concrete types, including class/struct instances, and pass those values through recursive calls. It deliberately separates the current String-only `SLList` from the future heterogeneous contract.

## 1. What the current VM already supports

The core `SLValue` tagged union includes `SL_NULL`, `SL_INT`, `SL_DOUBLE`, `SL_BOOL`, `SL_STR`, `SL_STRUCT`, and `SL_ARRAY`. Structs and arrays are VM-local handles, not raw pointers. The garbage collector's value traversal marks struct/array handles stored in struct fields and array elements, and it marks globals, the last result, mailboxes, and the active stack.

The array store uses `SLValue` elements, and `arrayPush` accepts an `SLValue` at the core boundary. This makes the runtime representation a good basis for a mixed-value list.

## 2. Why the current SLList cannot yet promise this

- `lib/collections/SLList.sleela` declares `String[] values` and `add(String value)`; it is intentionally String-only.
- The semantic type system treats named types as distinct struct types. It has no source-level top type such as `Any`.
- `arrayPush` and `arraySet` lower to generic array opcodes, and the runtime array instance does not record an element-type contract. The core can therefore store a value whose runtime tag does not match the source array's declared element type if a caller bypasses static checks.
- Struct member access is compiled using the declared struct layout and a field offset. Retrieving a mixed value as the wrong struct type must be rejected before field access, not treated as a raw unchecked cast.

Until these checks exist, changing `SLList.add` to accept arbitrary objects would overstate the guarantees and could allow type confusion.

## 3. Recommended non-generic API

Keep the existing `SLList` as a concrete String list for compatibility. Introduce a separate `SLValueList` (name provisional) once the runtime checks are implemented:

- `add(Any value)` — append any supported value: null, primitive, String, array handle, or class/struct handle.
- `Any get(int index)` — return a dynamically typed value; invalid indices return null.
- `boolean set(int index, Any value)` — replace an element, false for invalid indices.
- `boolean removeAt(int index)`, `void remove()`, `void clear()`, `int length()`, `boolean empty()`.
- `String typeNameAt(int index)` and `boolean isTypeAt(int index, String typeName)` — inspect an element before using it as a concrete type.
- A checked type conversion such as `cast(TypeName, value)` (exact spelling provisional) must verify the runtime tag and struct type before the result can be used as a typed reference.

This is a dynamic top type, **not Generics**. It must preserve object identity: appending a class/struct handle stores the handle, not a serialized copy. The same handle should be safe to pass through recursive method parameters.

## 4. Required implementation safeguards

1. Add a source-level `Any` type to semantic analysis; make it accept values of all supported kinds without silently treating every named class type as interchangeable.
2. Restrict heterogeneous storage to `Any[]` / the new list. Preserve declared element-type checks for ordinary `String[]`, `Node[]`, and other typed arrays.
3. Validate `arrayPush` and `arraySet` against the array's element contract. If static information is insufficient, emit or invoke a runtime check.
4. Add a checked type-test/cast path for struct handles. Confirm both the `SL_STRUCT` tag and the exact runtime struct-type index before field access through a cast result.
5. Preserve GC reachability and alias identity across list storage, list reads, recursive calls, and collections. Do not expose raw handles as integers.

## 5. Regression test matrix

The implementation is not complete until the compiler and runtime tests cover all of the following:

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

The repository source was inspected on `main` for `SLValue`, array storage/opcodes, GC marking, semantic assignability, and compiler field-offset resolution. A local build/test run was not possible in this environment because the GitHub host could not be resolved for a repository clone. No heterogeneous implementation or successful runtime test is claimed by this document.
