# Sleela — Architecture & the Exchange API

Sleela is a **Java-like language** that runs on top of a small, Turing-complete
**C/C++ execution core**. The core is a stack-based bytecode virtual machine that
exposes a stable **C ABI** — the *exchange* function — through which everything
above it drives execution. The Sleela front end (lexer → parser → compiler) never
executes anything itself: it lowers source code to core bytecode and then asks the
core to run it via the exchange API.

```
   +-------------------------------------------------------+
   |  Sleela front end  (C++)                              |
   |    lexer  ->  parser  ->  AST  ->  compiler/codegen   |
   +-----------------------------|-------------------------+
                                 |  emits opcodes + calls
                                 v
   +-------------------------------------------------------+
   |  Sleela Core VM  (C, stable C ABI)                    |
   |    value model | operand stack | call frames          |
   |    opcode dispatch loop                                |
   |    ---- the "exchange" executable API ----            |
   +-------------------------------------------------------+
```

The boundary is deliberate: the core is a self-contained, embeddable runtime with a
C interface. Any language (Sleela today, others later) can target it purely by
emitting bytecode and calling `slcore_exchange`.

## Directory layout

```
impl/
  core/                 # C/C++ execution core (the Turing-complete engine)
    sleela_core.h       #   public C ABI: opcodes, values, the exchange API
    sleela_core.c       #   VM implementation (stack machine + dispatch loop)
  frontend/             # Sleela language layer (C++)
    lexer.h / lexer.cpp     # Java-like tokenizer
    ast.h                   # AST node definitions
    parser.h / parser.cpp   # recursive-descent parser
    compiler.h / compiler.cpp   # AST -> core bytecode via the builder API
    driver.cpp              # CLI: sleela run file.sleela
  examples/             # sample .sleela programs
  Makefile
  DESIGN.md
```

## The Core value model

The VM is dynamically typed at the bytecode level (Sleela adds static types on top).
A core value (`SLValue`) is a tagged union:

| Tag         | Payload      |
|-------------|--------------|
| `SL_NULL`   | —            |
| `SL_INT`    | `int64_t`    |
| `SL_DOUBLE` | `double`     |
| `SL_BOOL`   | `int`        |
| `SL_STR`    | interned string id |

Strings are interned in a per-VM table and referred to by id, so values stay POD and
copyable across the C ABI.

## The Core VM model

A classic stack machine:

- **Operand stack** — opcodes push/pop `SLValue`s.
- **Constant pool** — literals referenced by index.
- **Globals** — flat slot array addressed by index.
- **Call frames** — each holds a return address, a base pointer into a **locals**
  region of the value stack, and the arg count. `CALL`/`RET` push/pop frames.
- **Code** — a flat array of `SLInstr { uint8 op; int32 a; }` (one integer operand
  covers constant indices, slot indices, jump targets, arg counts).

This is Turing-complete: unbounded conditional jumps + a growable data stack +
addressable memory (globals/locals) give it arbitrary loops and recursion.

## Opcode set

```
NOP
CONST   a   push constants[a]
POP         pop, discard
DUP         duplicate top

LOADG   a   push globals[a]
STOREG  a   globals[a] = pop
LOADL   a   push locals[a]           (a = offset from frame base)
STOREL  a   locals[a] = pop

ADD SUB MUL DIV MOD          numeric (int/double), pop 2 push 1
NEG                          arithmetic negate
EQ NE LT LE GT GE            comparisons -> bool
AND OR NOT                   logical

JMP     a   ip = a
JMPF    a   if !pop() ip = a         (jump if false)

CALL    a   call function; a = argc  (target/nargs resolved via callee const)
RET         return top of stack to caller

PRINT       pop and print + newline

SPAWN   a   start function a on a new thread; its `nargs` args are taken from
            the top of the stack and become the thread's locals; pushes a
            thread id (or -1 if at the 128-thread cap)
JOINALL     wait for every spawned thread to finish
LOCK    a   acquire lock-table slot a   (a in [0, SL_MAX_LOCKS))
UNLOCK  a   release lock-table slot a
SEND    a   send the 2-tuple (a, pop()) onto mailbox slot a (blocks while full)
RECV    a   block until a tuple is present on slot a; push its value
HALT        stop the VM
```

Function calls: a `FUNC` descriptor (entry ip, arg count, locals count) lives in a
function table. `CONST` can push a function reference; `CALL a` invokes the function
reference sitting under `a` arguments on the stack. Simpler equivalent used here:
`CALL` takes the target function index in `a` and the argc is encoded by the callee
descriptor, with args already on the stack.

## Threading model (bounded, clean)

Threading is deliberately simple and bounded — a thread is one *line of
execution*, synchronization is a small fixed set of locks, and coordination
happens over 2-tuples rather than arbitrary shared memory.

- **Per-thread state.** Execution state (operand stack, call frames, `ip`,
  `sp`, `fp`, error slot) lives in an `SLThread`, so the interpreter loop
  (`run_thread`) is fully re-entrant. `slvm_run` runs `main` on one `SLThread`
  and joins all workers before returning.
- **Shared state.** Code, constants, functions, globals and the string table
  live in the `SLVM`. Globals, interning, and `print` are each guarded by a
  mutex, so those are the only cross-thread interactions and they stay
  race-free. Output lines print atomically.
- **Bounds.** `SL_MAX_THREADS = 128` concurrent threads; `SL_MAX_LOCKS = 32`
  lock-table slots and 32 mailbox slots (comfortably covers "~24 concurrent
  locks on a 2-tuple line of execution").
- **Locks.** `LOCK n` / `UNLOCK n` acquire/release a fixed mutex from the table
  — a simple, bounded critical-section mechanism.
- **The burble line (2-tuple mailbox).** Each mailbox slot carries one pending
  `(tag, value)` tuple. `SEND a` puts a tuple on slot `a` (blocking while a
  tuple is still in flight); `RECV a` blocks until one is present and yields its
  value. Senders and the receiver share a condition variable, so both `SEND`
  and `RECV` use `pthread_cond_broadcast` and re-check their predicate to avoid
  lost wakeups.

Because coordination is confined to guarded globals, the bounded lock table,
and the tuple mailbox, the model is easy to reason about while remaining
Turing-complete and genuinely concurrent.

## The Exchange API (the C ABI)

Everything above the core talks to it through these functions, declared in
`sleela_core.h`. `slcore_exchange` is the single dispatch entry point ("exchange
function of the underwriting C/C++"); the typed helpers are thin wrappers over it for
ergonomics.

```c
/* lifecycle */
SLVM*   slvm_new(void);
void    slvm_free(SLVM*);

/* the one exchange/dispatch entry point */
SLResult slcore_exchange(SLVM* vm, SLExchangeOp op, SLExchangeArg* arg);

/* builder helpers (wrap exchange) used by the compiler to assemble a program */
int     slvm_add_const_int(SLVM*, int64_t);
int     slvm_add_const_double(SLVM*, double);
int     slvm_add_const_str(SLVM*, const char*);
int     slvm_declare_global(SLVM*, const char* name);
int     slvm_begin_func(SLVM*, const char* name, int nargs, int nlocals);
int     slvm_emit(SLVM*, SLOp op, int32_t a);   /* returns instr index */
void    slvm_patch(SLVM*, int at, int32_t a);   /* backpatch a jump target */
int     slvm_here(SLVM*);                        /* current code length */
void    slvm_set_entry(SLVM*, int func_index);

/* execution */
SLResult slvm_run(SLVM*);                        /* runs entry function */
```

`slcore_exchange` multiplexes these operations so an embedder can drive the whole
core through one symbol:

```c
typedef enum {
    SLX_RESET, SLX_ADD_CONST, SLX_DECLARE_GLOBAL,
    SLX_BEGIN_FUNC, SLX_END_FUNC, SLX_EMIT, SLX_PATCH,
    SLX_SET_ENTRY, SLX_RUN, SLX_GET_RESULT
} SLExchangeOp;

typedef struct {           /* tagged argument/return packet */
    SLValue   value;       /* const value / run result */
    const char* name;      /* symbol name */
    int32_t   op;          /* opcode for EMIT */
    int32_t   a;           /* operand / index */
    int32_t   i0, i1, i2;  /* nargs, nlocals, patch target, ... */
    int32_t   out;         /* returned index / instr slot */
} SLExchangeArg;
```

## Sleela → Core lowering (Java-like subset, first pass)

Supported Sleela surface:

- `class Name { ... }` container (methods hoisted to core functions; `main` is entry)
- Types: `int`, `double`, `boolean`, `String`, `void` (checked at parse; core is dynamic)
- Methods: `static <type> name(params) { ... }`
- Statements: local `var decl`, assignment, `if/else`, `while`, `for`, `return`,
  `print(expr)` (built-in), expression statements, method calls
- Expressions: `+ - * / %`, comparisons, `&& || !`, unary `-`, literals, identifiers,
  calls, parentheses, string concatenation with `+`

Lowering rules:

| Sleela                | Core                                                      |
|-----------------------|-----------------------------------------------------------|
| method                | `begin_func(name, nargs, nlocals)` + body opcodes + `RET` |
| local variable        | a locals slot; `STOREL/LOADL`                             |
| `x = e`               | compile `e`, `STOREL/STOREG`                              |
| `if (c) a else b`     | `c; JMPF L1; a; JMP L2; L1: b; L2:`                       |
| `while (c) b`         | `L0: c; JMPF L1; b; JMP L0; L1:`                          |
| `for(i;c;u) b`        | desugars to init + while                                  |
| `return e`            | `e; RET`  (void: push null, RET)                          |
| `print(e)`            | `e; PRINT`                                                |
| `f(a,b)`              | push args, `CALL fidx`                                    |
| binary/unary/compare  | operand opcodes above                                     |

`main()` is registered as the entry via `slvm_set_entry`, then `slvm_run` executes
it through the core dispatch loop.


## Conducted methods (SHEET.sheet catalog)

Sleela's system-object vocabulary lives in the repo-root `SHEET.sheet`, parsed by
the shared `catalog/` module (`catalog::Catalog`). The compiler is handed the
catalog (`compile(prog, vm, &catalog)`) and resolves a family of *conducted
method* built-ins entirely at compile time — no runtime library, no core
opcodes. Each lowers to a plain constant:

| Sleela built-in       | Lowering                                                      |
|-----------------------|---------------------------------------------------------------|
| `conduct("Name")`     | `bool` — is `Name` a catalogued object?                       |
| `role("Name")`        | `String` — the object's conduct role                          |
| `insight("Name")`     | `String` — the object's note (insight)                        |
| `congruent("A","B")`  | `bool` — `catalog::congruent(A,B)`                            |
| `route("A","B")`      | `String` — `"A -> B"` if congruent, else `""`                 |
| `sysdepth()`          | `int` — `catalog.depth` (3024)                                |
| `degreemax()`         | `int` — `catalog.complexityDegreeMax` (4)                     |

Congruence (the routing relation) holds when two objects share a conduct role or
list each other as siblings — the sheet's own child/sibling graph. The driver
finds `SHEET.sheet` via `$SLEELA_SHEET` or nearby paths; if absent, the built-ins
still compile but resolve against an empty catalog. The same catalog module backs
Nordshrift's object-compatibility list, so both tools agree object-for-object.
