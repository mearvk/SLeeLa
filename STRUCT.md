# SLeeLa Structs

Structs are SLeeLa's C/C++-style **named aggregates of typed fields**. They add
data modelling and instantiation to the Java-like surface without disturbing the
existing class/field/method model: a `struct` describes *data*, a `class`
describes *code*. Structs are available in **syntax 1.2 and later** (declare
`#sleela 1.2` at the top of a `.sleela` Wrapper™).

The design goal is "the best of object-orientedness, instantiation, network, and
control" on the existing Turing-complete C/C++ core:

- **Object-orientedness / instantiation** — `new Type()` builds a fresh instance;
  `.` reads and writes fields; structs nest like C aggregates.
- **Control** — struct values are *reference types* with well-defined identity
  and sharing, so they behave predictably when passed between methods and threads.
- **Network** — `structPack` / `structUnpack` give an instance a portable JSON
  wire form so it can travel over sockets or the mailbox line.

---

## 1. Declaring a struct

```sleela
struct Point {
    int x;
    int y;
}

struct Rect {
    Point origin;     // structs nest
    int   width;
    int   height;
}
```

- A struct is a top-level declaration, like `class`. It may appear before or
  after the code that uses it — declaration order does not matter.
- Fields use the same types as everywhere else: `int`, `double`, `boolean`,
  `String`, or another struct type.
- Limits: **256** struct types per program, **64** fields per struct.

## 2. Creating and using instances

```sleela
Point p = new Point();   // fresh instance; fields start null/zero
p.x = 3;
p.y = 4;
print("p = (" + p.x + ", " + p.y + ")");   // p = (3, 4)
```

`new Type()` returns a **handle** to a new instance. A struct-typed variable,
parameter, or field must be declared with its struct type so the compiler can
resolve `.field` offsets.

Nested access chains naturally:

```sleela
Rect box = new Rect();
box.origin = new Point();
box.origin.x = 1;
print(box.origin.x);     // 1
```

## 3. Reference semantics (control)

A struct value is a VM-local instance handle — the same bounded-handle
discipline the core uses for sockets, files, and threads. A SLeeLa program never
sees a raw pointer, but the semantics are C-pointer-like:

```sleela
Point p = new Point();
p.x = 3;
Point q = p;             // q and p name the SAME instance
q.x = 99;
print(p.x);              // 99  -- writes through an alias are shared
print(p == q);           // true  -- == compares identity
print(p == new Point()); // false -- distinct instances
```

- **Assignment and argument passing share** the instance (no implicit copy).
- **`==` / `!=` compare identity**, not field contents.
- Fields are **null/zero-initialised** by `new`.

This makes structs the right tool for shared mutable state handed to spawned
threads, while the existing `lock` / `unlock` built-ins still guard concurrent
field updates.

## 4. Network transport (pack / unpack)

Because an instance is a VM-local handle, it cannot cross a socket as a handle.
`structPack` serialises an instance to a self-describing JSON String, and
`structUnpack` rebuilds one on the other side:

```sleela
Order o = new Order();
o.symbol = "AAPL";
o.shares = 100;
o.limit  = 332.27;

String wire = structPack(o);
// wire == {"__type":"Order","symbol":"AAPL","shares":100,"limit":332.27}

// ... send `wire` over a socket with sockwrite(), or between threads via send() ...

Order back = structUnpack(Order, wire);   // first arg is the TYPE NAME
print(back.symbol + " x" + back.shares);   // AAPL x100
```

- `structPack(instance) -> String` — compact JSON of the instance's fields.
- `structUnpack(TypeName, json) -> instance` — a new instance of `TypeName` with
  fields populated from the JSON. `TypeName` is a bare identifier (resolved at
  compile time), **not** a string.
- The JSON is intentionally flat and forgiving. Scalar fields (`int`, `double`,
  `boolean`, `String`) round-trip; a nested struct field packs as its handle
  integer, so deep graphs should be packed/sent field by field or level by level.

See the runnable examples:

- [`impl/examples/struct_basic.sleela`](impl/examples/struct_basic.sleela) —
  declaration, `new`, member get/set, nesting, aliasing, and identity `==`.
- [`impl/examples/struct_network.sleela`](impl/examples/struct_network.sleela) —
  a client packs an `Order`, sends it over TCP, and the server unpacks and
  acknowledges it.

---

## 5. How it lowers (implementation notes)

Structs are implemented end to end across the pipeline, gated on `#sleela 1.2`:

| Layer | What was added |
|---|---|
| **Lexer** (`impl/frontend/lexer.*`) | `struct` and `new` keywords. |
| **AST** (`impl/frontend/ast.h`) | `StructDecl`, `NewExpr`, `MemberAccess`, `FieldAssign`; `Program.structs`. |
| **Parser** (`impl/frontend/parser.*`) | `parseStruct()`; struct names recognised as types (order-independent pre-pass); `new Type()`; member-access chains; `x.field = ...`. |
| **Compiler** (`impl/frontend/compiler.cpp`) | Struct layout registry (type index + field offsets), variable-type tracking to resolve `.field`, and lowering to the opcodes below; `structPack`/`structUnpack` built-ins. |
| **Core VM** (`impl/core/sleela_core.*`) | `SL_STRUCT` value tag (a bounded instance handle); `slvm_declare_struct`; a mutex-guarded instance store; opcodes `OP_NEWSTRUCT`, `OP_GETFIELD`, `OP_SETFIELD`, `OP_STRUCTPACK`, `OP_STRUCTUNPACK`. |
| **Artifact** (`impl/core/sleela_artifact.c`) | Format version 2 persists struct-type layouts (v1 artifacts still load); struct-typed globals reload as `null`. |

The value model stays a fixed-size scalar tagged union — a struct value carries
only its instance handle, exactly like the socket/file/thread handles the core
already uses. This keeps the exchange ABI and the stack VM unchanged in shape
while giving SLeeLa real aggregates.

---

*Structs are additive: every pre-1.2 program continues to compile and run
unchanged, and the compiler rejects `struct` / `new` / `structPack` /
`structUnpack` when the source declares a syntax below 1.2.*
