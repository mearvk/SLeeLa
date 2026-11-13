# Lowering Java constructs to linear procedural SLeeLa

> **Container.** This is Java container content (`/lib/java/jdk-28`), not part of
> the SLeeLa specification. Nothing here adds syntax to SLeeLa
> (`SLEELA.syntax` section 38.4).

Status: **DRAFT for review.** Section 2 (lambdas and method references) is
**implemented** by `lib/java/converter` 0.6.0 (`JConvLambda`, 2026-11-13): the
body is a method `slLambdaN(slEnv, slA, slB)` of the class holding the lambda
(instance or static as the context), the call site is
`new SLJavaLambda(N, new Any[]{this, captures})`, and each document gets one
`SLJavaLambdaTable`. `samples/lambda/LambdaDemo.java` runs identical to a real
JDK. Variables of a functional interface type are `Any`, so library code calls
them by run-time dispatch (SLEELA.syntax 43.7), which makes rule 3 of section 1
(no dependence on virtual dispatch) unnecessary for lambdas. The first
prototype is `examples/java/jdk-28/LambdaLoweringProto.sleela`. Everything else
is a design sketch and is marked *unverified*.

## 1. Principle

We do not run Java; we translate Java **procedural source** into SLeeLa that
behaves the same. So a Java construct that SLeeLa has no form for is not given a
new form. The translator **lowers** it to a short, linear sequence of things
SLeeLa already has:

- declarations and assignments (temporaries named `t$n`, renamed `t_n` on emit),
- calls on named classes,
- `if` / `while` / `for`, and `switch` on an `int`,
- arrays (`Any[]`), `new` of a named class.

Rules every lowering obeys:

1. **No new syntax, no new keyword.** If it cannot be said with the list above,
   it is rejected with a clear error (PURPOSE.md, status (c)).
2. **Order of evaluation is preserved.** Java evaluates operands left to right;
   where lowering splits an expression, each sub-result goes to a temporary in
   that order.
3. **No dependence on open Gap rows where a lowering can avoid them.** In
   particular, lowerings must not need virtual dispatch (row 8) or `finally`
   (row 14) from the VM; they carry their own dispatch and their own cleanup
   path.
4. **Linear.** One statement in, a straight run of statements out, so a lowered
   program can be read top to bottom against the Java.

## 2. Lambdas and method references (rows 28, 29, 33)

### 2.1 Representation: a lambda is `(id, env)`

A lambda value is not code, it is data: an integer `id` naming the body, and an
array `env` holding the captured values. This is defunctionalization.

```
Java:   int k = 3;
        Function<Integer,Integer> times = x -> x * k;
        times.apply(14);

SLeeLa: int k = 3;
        Any[] env = new Any[1];
        env[0] = k;                                  < capture, by value >
        SLJavaLambda times = new SLJavaLambda(1, env);
        times.apply(14);

        LambdaTable.call(1, env, a, b):   if (id == 1) { return (int) a * (int) env[0]; }
```

- **Capture by value is exact.** Java only lets a lambda capture *effectively
  final* locals, so copying the value into `env` at creation can never differ
  from Java. No boxes or cells are needed.
- **`this`.** A lambda that uses `this` (or an instance member) stores `this` in
  `env[0]`; the body reads `((Owner) env[0]).member`.
- **`id`** is assigned by the translator, one per lambda site, unique across the
  whole translated program.
- **Body** is one `if (id == N) { ... }` arm in a single generated class
  `LambdaTable`, with the parameters bound to the leading call arguments.
  A body longer than one expression is simply a longer arm (or a call to a
  generated static method `lambda$N(env, a, b)` that the arm forwards to).
- **Dispatch is a chain of integer compares**, so it works today and does not
  wait for virtual dispatch (row 8).

### 2.2 Library side: one value class answers every functional interface

`SLJavaLambda` carries the single-abstract-method names the JDK library calls,
each forwarding to `LambdaTable.call(id, env, ...)`:

| Interface (SAM) | Method on `SLJavaLambda` | Arity |
|---|---|---|
| `Function.apply`, `UnaryOperator.apply` | `apply(a)` | 1 |
| `BiFunction.apply`, `BinaryOperator.apply` | `applyBi(a, b)` | 2 |
| `Comparator.compare` | `compare(a, b)` | 2 |
| `Consumer.accept`, `Predicate.test`, `Supplier.get`, `Runnable.run`, ... | same name | 0..2 |

So library code such as `list.sort(cmp)` calls `cmp.compare(a, b)` and cannot
tell a translated lambda from a hand-written comparator class. This unblocks
`Comparator` sorting, `compute`/`merge`, `forEach`, and the Stream lowering in
section 3. A user class that implements `Comparator` keeps working because it has
the same method name.

Open point: one fat class with all SAM names (above) versus one small class per
functional interface. The fat class is simplest and is what the prototype uses.

### 2.3 Method references are lambdas

A method reference is lowered to a lambda *before* the step above, using the
arity of the target functional interface:

| Java | Lowered lambda |
|---|---|
| `Type::staticM` | `(a..) -> Type.staticM(a..)` |
| `expr::instM` | `expr` evaluated **once**, stored in `env[0]`; `(a..) -> env[0].instM(a..)` |
| `Type::instM` | `(r, a..) -> r.instM(a..)` |
| `Type::new` | `(a..) -> new Type(a..)` |
| `int[]::new` | `(n) -> new int[n]` |

The `expr::instM` row matters: `expr` is evaluated at the reference, not at each
call, and a `null` receiver must throw `NullPointerException` at the reference.

**Target typing is required.** The arity (and which overload) comes from the
functional interface the lambda is assigned or passed to. The Java front end
already treats lambdas and inexact method references as "not pertinent until
target typing" (`impl/frontend/java_equivalence.cpp`); the translator needs that
target type, so lowering runs after overload resolution.

### 2.4 Inner, local and anonymous classes (row 33)

*Inner classes implemented* by `lib/java/converter` 0.7.0 (`JConvOuter`,
2026-11-13): the enclosing instance is the field `slOuter`, set from the first
constructor parameter `slOuterArg`; `Outer.this` is `this.slOuter`. Local and
anonymous classes are still stubs there (`sleelactl java-convert` lowers them).

Same idea, no `id`: the class becomes a **named top-level class** with the
captured locals and the outer `this` as explicit fields, set by its constructor.
`new Outer().new Inner()` passes the outer reference as the first constructor
argument. Because these are ordinary classes, they do not need `SLJavaLambda`.

Anonymous class implementing a functional interface and a lambda are therefore
handled by two different, both linear, mechanisms; neither uses new syntax.

## 3. Other constructs, same approach (all *unverified* sketches)

| Java construct | Lowered form |
|---|---|
| **Streams** (row 29) | One fused loop per terminal operation. Each element is pushed through the stage lambdas in pipeline order, so side-effect interleaving and short-circuiting (`limit`, `anyMatch`, `findFirst`) match Java's laziness. Sources are lowered to an index loop. |
| **Enhanced `for`** (row 35) | Array: index loop. `Iterable`: `Iterator it = x.iterator(); while (it.hasNext()) { T v = (T) it.next(); ... }`. |
| **Labeled `break` / `continue`** | A `boolean` flag per label tested in each enclosing loop condition and after each inner loop. |
| **`switch` expression / `yield`** | A temporary assigned in each arm, followed by an `if`/`switch` statement; the temporary replaces the expression. |
| **`switch` on `String`, enum** | `String`: if-chain on value equality (hash switch is not needed). Enum: `switch` on `ordinal()`. |
| **Pattern `instanceof`, type patterns, record patterns** | `if (x instanceof T) { T v = (T) x; ... }`; record deconstruction calls the accessors into temporaries. |
| **`finally` / try-with-resources** (row 14) | Body in `try`; `catch (Any e)` records the exception in a temporary; the `finally` block is emitted on the normal path and on the catch path, then rethrows. `return` / `break` / `continue` inside the body set a completion-kind variable and jump to the single exit where `finally` runs once. Resources close in reverse order; `close` is renamed (reserved word, row 27) and a close failure is added as suppressed. |
| **`synchronized`** (row 32) | VM lock enter, body in the `try` form above, lock exit on every path. |
| **Enum constant bodies** (row 25) | One named class per constant with a body; the shared method is an `ordinal()` switch in the enum class (no virtual dispatch needed). |
| **Default interface methods** (row 12) | Copied into each implementing class until real dispatch lands (row 8). |
| **`static {}` and instance initializers** (row 34) | Static: a `clinit()` method with a done-flag, called before first static use. Instance: inlined at the top of each constructor, after the root `super("constructor")`. |
| **Varargs** | The call site packs the trailing arguments into an array. |
| **Autoboxing / unboxing** | Explicit conversion calls inserted at each boundary; `Integer` identity beyond the -128..127 cache is not reproduced. |
| **Compound assignment with narrowing** (`b += 1` on `byte`) | Expanded: `b = wrap8(b + 1)`. |
| **Ternary, `&&`, `||`** | Native; kept as is. |
| **`assert`** | Dropped (disabled by default in Java), or `if (!c) throw` when asserts are enabled for the run. |
| **`var`, text blocks, sealed** (row 36) | Resolved at translation time; no runtime form. |

## 4. Order of work

1. Settle the open points below.
2. Add `SLJavaLambda` and the generated `LambdaTable` shape to
   `lib/java/support/`, with a differential pair in `examples/java/differential/`
   (`Lambdas.java` / `Lambdas.sleela`).
3. Teach the generator that creates the stubs to emit `SLJavaLambda`-typed
   parameters for functional-interface members; implement `Comparator` sort,
   `forEach`, `compute`, `merge`, `removeIf`, `replaceAll` first.
4. Method-reference desugaring (section 2.3), then inner/anonymous classes.
5. Streams as fused loops.
6. Update PURPOSE.md rows 28, 29, 33 from Gap to Rule in the same commit as each
   piece lands (PURPOSE.md section 3).

## 5. Open points

1. Fat `SLJavaLambda` (all SAM names) or one class per functional interface?
2. One global `LambdaTable` for the whole program, or one per translated unit
   with the unit number folded into the `id`? Global is simpler; per-unit allows
   separate translation of library code.
3. Arguments and results are `Any` (erased), so every lambda body casts on entry
   and the call site casts the result. Should primitive lambdas
   (`IntBinaryOperator`, `ToIntFunction`) get their own entry points to avoid
   the casts?
4. Exceptions thrown inside a lambda body propagate through `LambdaTable.call`
   unchanged; confirm that stack/`getStackTrace` content does not matter for the
   differential tests.
