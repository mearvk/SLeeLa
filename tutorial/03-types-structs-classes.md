# Lesson 03 — Types, Structs & Classes

> Series: [index](README.md) · prev: [02](02-first-wrapper.md) · next: [04 — Control flow & expressions](04-control-flow.md)

SLeeLa's type surface is small and Java-like. This lesson covers the primitives,
`struct` (plain data), and `class` (data + behavior).

## Primitive types you'll use

| Type | Meaning |
|---|---|
| `int` | integer |
| `double` | floating point |
| `String` | text |
| `boolean` | `true` / `false` |
| `void` | no return value (methods only) |

Declarations state the type first: `int n = 3;`, `double x = 1.5;`,
`String s = "hi";`.

## `struct` — plain data

A `struct` groups fields. It's the shape of a record, with no methods. This is
straight from a real Wrapper (`churn/1/assumptions/assumptions.sleela`):

```sleela
struct Assumption {
    String name;
    double length;       // units that go into one whole
    double speed;        // units per year (speed of the process)
    int    lengthening;  // 1 = lengthens base reality, 0 = substitutes
    double moralWeight;  // 0..1 importance to base reality
}
```

Fields are typed, one per line, ending in `;`.

## `class` — data and behavior

A `class` adds methods. Fields hold state; methods act on it:

```sleela
#sleela 1.2
class Rect {
    int w;
    int h;

    int area() {
        return w * h;
    }

    void main() {
        Rect r = new Rect();
        r.w = 4;
        r.h = 5;
        print("area = " + r.area());   // area = 20
    }
}
```

- **`new Rect()`** constructs an instance.
- **`r.w`, `r.area()`** — field and method access with `.`.

## When to use which

- **`struct`** when you only need a bundle of values (a row, a message shape, a
  parameter set).
- **`class`** when values come with operations (compute, validate, print).

Look at `examples/symmetry/src/core/Message.sleela` for a class that computes a
frame size from a body length — data plus the operations that give it meaning.

**Next:** [Lesson 04 — Control flow & expressions](04-control-flow.md)
