# POINTERS — object references in SLeeLa

> Every object has **two tiers**. **Tier 1** is the ordinary object-oriented
> public interface. **Tier 2** is the whole, complete model of the *same* object
> as the VM, the memory manager, the operating system and the separation
> boundary see it. The implicit member **`target.ref`** is the bridge between
> them.

```sleela
< tier 1: the object, as the class declares it >
Target target = new Target("suppose");
< tier 2: its VM-level reference >
int p = target.ref;
< and the typed way back >
Target again = Target.deref(p);
```

**Status: implemented (first slice).** Runnable reference:
[`examples/pointers-demo.sleela`](./examples/pointers-demo.sleela). Regression
tests: [`impl/tests/refs/`](./impl/tests/refs) (`make test-ref`). This document
describes what the current build actually does; the *Not yet* section lists what
it does not.

---

## 1. The idea in one page

C and C++ let you hold a pointer to an object: compare it, test it, store it,
follow it back. SLeeLa never shows a raw machine address — its documented rule is
that "a Sleela program never sees a raw pointer" — but it already represents an
object as a small VM-local handle. `.ref` makes that handle **a first-class,
safe, inspectable value**.

| You want (C / C++) | In SLeeLa | Notes |
|---|---|---|
| `&obj` — address of | `obj.ref` | an `int`; `0` is the null ref |
| `*p` — follow it | `Class.deref(p)` | the object, or `null` — never a crash |
| `p == q` — same object | `p == q` | refs are ints; equal refs name one incarnation |
| `p == nullptr` | `p == 0` | `null.ref` is `0` |
| `weak_ptr::expired()` | `!Ptr.valid(p)` | |
| `weak_ptr::lock()` | `Class.deref(p)` | `null` when expired |
| dangling pointer (UB) | stale ref | **detected**, reads as invalid |
| `sizeof(*p)`, `typeid` | `Ptr.bytes(p)`, `Ptr.type(p)` | tier-2 views |
| pointer arithmetic over memory | **not supported** | see §5 |
| `reinterpret_cast`, raw bytes | **not possible** | by design |

So it is closest to a **`std::weak_ptr` with a generation-checked slot**, not to
a raw `T*`: you can play with it like a pointer, but it can never corrupt memory
or alias the wrong object.

---

## 2. Tier 1 — the OOD public interface

This is the object as the *class* describes it. It is what ordinary SLeeLa code
sees, and it is unchanged by this feature.

| Aspect | What tier 1 contains |
|---|---|
| **Type** | A class name. Nominal and static: `Target`. Single inheritance (syntax ≥ 1.10), base fields placed first. |
| **State** | The declared instance fields, in declaration order (base class first). Fields start **null**; field initializers run at construction. Static fields are separate class-level globals (`Class::field`). |
| **Behaviour** | The declared methods. A call binds **statically** by the receiver's declared class name (`Target::getLabel`); the object is passed as a hidden first argument (`this`). There is no vtable and no runtime dispatch on the object's actual class. Inherited methods are aliased onto the derived class unless overridden. |
| **Construction** | `new Target("suppose")` zero-initialises the fields, runs the initializers, then calls the constructor. Constructor arguments exist since syntax 1.8, with **one** constructor per class (no overloading, no `this(...)`/`super(...)` delegation). |
| **Identity** | `==` compares identity. Assignment aliases (`Target b = a;` is the same object). |
| **Access** | `obj.field`, `obj.method(...)`; `protected` methods are checked; static methods are called through the class. |
| **Lifetime** | Reachability only. There is no destructor and no reference count; the collector reclaims what is unreachable. |
| **Visible limits** | 64 fields per class, 4,096 live objects per VM, 65,536 distinct class/struct layouts. |
| **Casts** | `OP_CHECKCAST` / `OP_INSTANCEOF` compare the *runtime* layout (with its parent chain) against a class. |

**What tier 1 hides:** where the object lives, which incarnation of its slot it
is, how big it is, how old it is to the collector, and how the system accounts
for it. That is tier 2.

---

## 3. Tier 2 — the complete model

Tier 2 is everything about the same object that is *not* in the class
declaration. It is read through `target.ref` and the `Ptr` namespace. It has four
lenses, matching the four things that "refer to" an object below the language.

Measured on this build (`impl/core/sleela_core.c`, 64-bit Linux):

| Quantity | Value |
|---|---|
| `sizeof(SLValue)` | **32 bytes** (16-byte aligned, because of its 128-bit integer member) |
| `sizeof(SLStructInstance)` — one object | **2,080 bytes** |
| &nbsp;&nbsp;header (`active`, `type`) | 16 bytes |
| &nbsp;&nbsp;`fields[64]` | 64 × 32 = 2,048 bytes, at offset 16 |
| &nbsp;&nbsp;`gc_object` pointer | offset 2,064 |
| &nbsp;&nbsp;`generation` (31-bit incarnation counter) | offset 2,072 — fits existing padding, **no size cost** |
| `sizeof(SLGCObject)` — the collector's record | 56 bytes |
| Instance store | `SL_MAX_STRUCTS` (4,096) × 2,080 = **8,519,680 bytes**, allocated lazily |

### 3.1 The VM lens

How the virtual machine represents the object.

| Item | Detail |
|---|---|
| **Value** | A tagged value `{ type = SL_STRUCT, h = <int32 slot> }`. The slot is an index into the VM's instance store, not an address. |
| **Instance record** | `SLStructInstance { active, type, fields[64] of SLValue, gc_object, generation }`. Field storage is dense; a field's offset is its declaration index. |
| **Layout** | One entry per class/struct in the layout table: `SLStructType { name, nfields, parent_type, field names }`. Registered by `slvm_declare_struct`; the single-inheritance link is `parent_type`. An instance's `type` is its layout index. |
| **Opcodes** | `OP_NEWSTRUCT` (allocate), `OP_GETFIELD` / `OP_SETFIELD` (checked, with a GC write barrier on set), `OP_CHECKCAST`, `OP_INSTANCEOF`, `OP_CALL` (the receiver is argument 0), and — new — **`OP_REF`**. |
| **Slot validity** | A slot is valid when it is in range and `active`. |
| **Concurrency** | The store is guarded by `struct_mtx`. |
| **Read with** | `Ptr.id(p)` (slot), `Ptr.type(p)` (layout index), `Ptr.fields(p)` (field slots), `Ptr.gen(p)` (incarnation), `Ptr.valid(p)`. |

### 3.2 The memory-manager lens

How the collector and the accounting treat the object. There are **two**
separate systems, and it matters which one an object belongs to.

| System | Does an object belong to it? | Detail |
|---|---|---|
| **The garbage collector** (`runtime/garbage_collector.*`) | **Yes** | A tracing, generational, incremental mark-sweep collector. Every object is an `SLGCObject` with `payload`, `bytes`, `marked`, `queued`, `remembered`, `age`, `external`, `generation` (young/old), `mark_children`, `destroy`. Objects are reachability-managed; there is no reference count. A write barrier and a remembered set handle old→young stores. Collection runs at VM safepoints, and only while no worker threads are active. |
| **The accounting allocator** (`impl/core/sleela_memmgr.*`, the `[memory-manager]` report) | **No** | A process-wide counter with a hard limit that backs the `[memory-manager] live=… live-allocs=…` lines. **Language objects do not pass through it**, so those counters do not move when you create objects. Per-object accounting would be new work. |

| Item | Detail |
|---|---|
| **Size** | `Ptr.bytes(p)` — the payload registered with the collector (2,080 for any class instance). |
| **Age / generation** | `Ptr.age(p)` counts collections survived; `Ptr.old(p)` is true once promoted to the old generation. |
| **VM-wide** | `Ptr.live()` = active instances; `Ptr.limit()` = the 4,096 ceiling. |
| **Lifecycle** | Allocated white; marked from roots (globals, constants, the operand stack, thread stacks); unmarked objects are swept, which clears the slot and **bumps its generation**. |

### 3.3 The operating-system lens

What the host operating system sees of the object: **very little, deliberately.**

- There is **no OS-visible address, handle or descriptor** for an individual
  object. To the OS the whole VM is one process.
- Objects live inside **one contiguous instance store** — a single
  zero-initialised allocation of 8,519,680 bytes made on the first object
  allocation. The OS sees that one block, not 4,096 objects.
- Objects are therefore **not counted** by the process-level accounting
  allocator (see §3.2). There is no per-object mapping, pinning, or sharing with
  other processes; the store as a whole is an ordinary heap allocation that the
  C allocator and the OS may back however they like.
- Anything that *does* have an OS identity — sockets, files, spawned processes,
  threads — is a separate VM-local bounded handle table with its own opcodes, not
  an object.

`Ptr` exposes nothing here because there is nothing here to expose. This is a
safety property, not a gap: it is why a ref cannot be used to read or write
memory.

### 3.4 The separation lens

What keeps the two tiers, and different VMs, apart.

| Boundary | How it is kept |
|---|---|
| **Tier 1 ↔ tier 2** | Tier 1 code cannot see a slot or a generation. The only door is `.ref` (one opcode), and it hands out an *opaque, validated* int. |
| **Ref ↔ memory** | A ref is never an address. It selects a slot in a bounded table and is re-validated on every use: slot in range, slot active, generation equal. |
| **Object ↔ object** | `Class.deref` also checks the target is an instance of `Class`; a ref that lands on a different class reads as `null`. |
| **VM ↔ VM / run ↔ run** | A ref is meaningful only inside the VM that made it. It is **not** persisted: the artifact format does not store live objects (a saved struct-typed global reloads as `null`), and generations restart at 0 in a new run, so a ref written to disk or sent elsewhere must be treated as garbage. |
| **Thread ↔ thread** | The store is shared and mutex-guarded; collection is deferred while workers run. |
| **ISA** | Tier 2 is one append-only opcode (`OP_REF`, 182) with one packed operand, so it cannot grow a back door without an ISA change that the CI gates. |

---

## 4. The reference: `target.ref`

### 4.1 Format

A ref is an ordinary **`int`**:

```
 ref = (generation << 32) | (slot + 1)        0 is the null ref
```

For the first object in a fresh VM (slot 0, generation 0) the ref is `1`.
`generation` is a 31-bit counter on the slot, **incremented every time the
collector reclaims that slot**. That is what stops a reused slot from aliasing:

```
 an old ref      (slot 1, generation 0)  ->  (0 << 32) | 2  =          2
 the new object  (slot 1, generation 1)  ->  (1 << 32) | 2  = 4294967298     different value
```

### 4.2 Semantics

- **Weak.** A ref does not keep its object alive. It is an `int`, not an object
  value, so the collector does not trace it.
- **Valid ≠ reachable.** `Ptr.valid(p)` means "this slot is still occupied by
  the *same incarnation*." An unreachable object that has not yet been swept is
  still valid until the next collection.
- **`deref` makes it strong again.** `Class.deref(p)` returns an ordinary object
  value. If a collection is in its marking phase, the object is shaded first, so
  a value now on the stack cannot be freed underneath you.
- **Fails closed.** A null, stale, forged, out-of-range or wrong-class ref never
  traps and never reaches memory:

| Operation on a bad ref | Result |
|---|---|
| `Class.deref(p)` | `null` |
| `Ptr.valid(p)`, `Ptr.old(p)` | `false` |
| `Ptr.id/gen/type/fields/bytes/age(p)` | `-1` |
| `Ptr.live()`, `Ptr.limit()` | unaffected (VM-wide) |

- **Only misuse of the *type* is an error.** `.ref` applied to a non-object, or a
  `Ptr.*` call given a non-`int`, is a compile error (or a runtime error if the
  type is only known late).

---

## 5. Language surface

| Form | Type | Meaning |
|---|---|---|
| `x.ref` | `int` | the ref of object `x`; `0` when `x` is `null` |
| `Class.deref(p)` | `Class` | the object, or `null` if `p` is invalid or not a `Class` |
| `Ptr.valid(p)` | `boolean` | slot occupied by the same incarnation |
| `Ptr.old(p)` | `boolean` | promoted to the old generation |
| `Ptr.id(p)` | `int` | slot index (`-1` if invalid) |
| `Ptr.gen(p)` | `int` | slot incarnation |
| `Ptr.type(p)` | `int` | layout (class) index |
| `Ptr.fields(p)` | `int` | number of field slots in use |
| `Ptr.bytes(p)` | `int` | instance size as the collector counts it |
| `Ptr.age(p)` | `int` | collections survived |
| `Ptr.live()` | `int` | active objects in the VM |
| `Ptr.limit()` | `int` | object-store ceiling (4,096) |

Rules:

- **`x.ref` is read-only.** `x.ref = 5;` is rejected: *"'ref' is the implicit
  reference of an object and is read-only."*
- **A declared field named `ref` wins.** `ref` is not a keyword, and 155 existing
  `.sleela` lines already use it as an identifier. If a class declares a field
  `ref`, `x.ref` is that field.
- **A user class, variable or struct named `Ptr` wins.** The builtin namespace
  applies only when no such name is in scope (the same rule as `Munction`).
- **Not version-gated.** `x.ref` on a class without a `ref` field was already a
  compile error in every edition, so no previously valid program changes
  meaning.
- **Arithmetic on refs is not a supported operation.** A ref is an `int`, so
  `p + 1` compiles, but it only changes a slot number; the result is validated
  like any other ref (`Ptr.valid(p + 1000000)` is `false`) and may name some
  *other* live object. Use `Ptr.id` to read the slot; do not compute with refs.

### Worked example

[`examples/pointers-demo.sleela`](./examples/pointers-demo.sleela) prints, for
`Target target = new Target("suppose")` (output from this build):

```
== Tier 1: the public interface ==
label = suppose

== Tier 2: the VM view, through target.ref ==
ref (packed handle) : 1
valid               : true
slot id             : 0
slot generation     : 0
layout (type) index : 0
field slots         : 1
instance bytes (GC) : 2080
collector age       : 0
old generation      : false
live objects in VM  : 1
object store limit  : 4096

== Weak, and never aliased ==
ref to a short-lived object, before collection: slot 1 generation 0
after a full collection, still valid?          : false
a new object reuses slot 1 at generation 1
old ref equals new ref?                        : false
old ref still reaches an object?               : false
the surviving object is now old                : true
```

The layout index (`0`) depends on class order in the program; the rest is
structural.

---

## 6. Safety properties

1. **No raw addresses.** A ref indexes a bounded table; there is no way to turn
   it into a machine address or to read or write outside the table.
2. **Every use is validated** (range, active, generation, and — for `deref` —
   class).
3. **No use-after-free by aliasing.** A reclaimed slot's generation moves on, so
   old refs cannot name the object that reuses it.
4. **No resurrection hazard.** `deref` shades the object during incremental
   marking.
5. **Closed under bad input.** NaN, negatives, huge values and `null` all fail
   closed (§4.2).
6. **Bounded.** 4,096 objects, 64 fields, and a 31-bit generation (a slot would
   have to be reclaimed 2,147,483,648 times to wrap).
7. **Artifacts are checked.** The artifact validator rejects an `OP_REF` with an
   out-of-range selector or class operand.

---

## 7. Implementation map

| Concern | File |
|---|---|
| Opcode `OP_REF` (182), selectors, packing macros, ABI minor 6 → 7 | `impl/core/sleela_core.h` |
| Generation counter, destroy hook, `ref_pack` / `ref_resolve_locked`, the `OP_REF` case | `impl/core/sleela_core.c` |
| Artifact operand validation | `impl/core/sleela_artifact.c` |
| `x.ref`, `Ptr.*`, `Class.deref`: type checking, read-only rule | `impl/frontend/semantic.cpp` |
| Lowering and static typing of `deref` | `impl/frontend/compiler.cpp` |
| ISA registry and opcode map (the CI gates compare these with the enum) | `lib/vm/InstructionSet.sleela`, `lib/vm/OPCODE-MAP.md` |
| Normative language text | `SLEELA.syntax` §39, `src/Sleela.formal-core` §15.6 |
| Tests | `impl/tests/refs/` (`make test-ref`) |
| Runnable reference | `examples/pointers-demo.sleela` |

`OP_REF` is **one** opcode. Its operand packs the selector in the low 4 bits and,
for `deref`, the class layout index above them: `a = (typeIndex << 4) | selector`.

---

## 8. Not yet

Honest boundaries of this first slice:

- **No ref type in the type system.** A ref is a plain `int`, so the compiler
  cannot stop you passing the wrong int, and `deref` needs the class named at the
  call (`Target.deref(p)`). A typed ref (a `Target&` or `Ptr<Target>` form) is the
  natural next step.
- **No pointers to fields, array elements, or arrays.** Only class/struct
  instances have refs.
- **No persistence.** Refs are valid only within one run of one VM (§3.4).
- **Introspection is numeric.** `Ptr.type` gives a layout index, not a class
  *name*, because strings are produced by a separate path.
- **No per-object accounting** in the `[memory-manager]` report; objects are
  invisible to it (§3.2).
- **No strong pin** to keep an object alive through a ref.
- **No virtual dispatch.** Methods still bind statically by declared class
  (§2), so `deref` returns the class you ask for, not the object's most-derived
  type.
- **The prebuilt `working-binaries/sleela` does not include this yet.** Build the
  compiler from source (`cd impl && make -j8 build/sleela`) to use it. Refreshing
  the committed binaries is a separate, documented procedure.
- **One naming pitfall, unrelated to refs:** a variable named `next` is not an
  ordinary variable (`next` is the language's degree word), so `next.ref` is
  rejected at runtime. Pick another name.

---

## 9. Tests

`make test-ref` (or `bash impl/tests/refs/run_ref_tests.sh`) checks:

- identity, the round trip through `deref`, and two objects having different refs;
- every tier-2 view;
- null, forged and out-of-range refs failing closed;
- wrong-class `deref` returning `null`;
- a declared field named `ref` winning;
- a ref going stale after a full collection, and a reused slot **never aliasing**;
- `x.ref = …`, a non-int argument, and an unknown `Ptr` function all rejected
  with the right message;
- a user class named `Ptr` taking over the name;
- the compiled **artifact** validating and running identically to the source.

Also kept green: the ISA verifiers (`tools/verify-osls-isa-extension.py`,
`tools/verify-vm-source-coverage.sh`, 183 opcodes) and the existing class-dispatch,
artifact-ABI, version, threads, Munction, OSLS, memory-manager and
unsigned-integer test targets. Two pre-existing failures are unrelated and
reproduce without this change: `test-syntax-1.8` (a stale expected message for
constructor overloading) and the opt-in `test-exceptions` (its link recipe omits
`-fopenmp`).
