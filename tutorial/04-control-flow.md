# Lesson 04 — Control Flow & Expressions

> Series: [index](README.md) · prev: [03](03-types-structs-classes.md) · next: [05 — The `.sst` sheet & Nordshrift](05-sst-and-nordshrift.md)

The statements inside a method are Java-like: conditionals, loops, assignment,
and expressions.

## Conditionals

```sleela
int classify(int n) {
    if (n < 0) {
        return -1;
    } else {
        if (n == 0) {
            return 0;
        }
        return 1;
    }
}
```

`if (cond) { ... } else { ... }`. Conditions are `boolean` expressions.

## Loops

`while` and `for` both exist. A real `while` (from `Demo.sleela`):

```sleela
int i = 1;
while (i <= 5) {
    print("square(" + i + ") = " + square(i));
    i = i + 1;
}
```

A `for` counts explicitly:

```sleela
for (int i = 0; i < 3; i = i + 1) {
    print("i = " + i);
}
```

## Operators

| Kind | Examples |
|---|---|
| Arithmetic | `+  -  *  /  %` |
| Comparison | `==  !=  <  <=  >  >=` |
| Logical | `&&  ||  !` |
| String | `+` (concatenation) |

A worked example that mixes them — align a length up to a 4-byte boundary
(`Message.sleela`):

```sleela
int align4(int n) {
    int r = n % 4;
    if (r == 0) {
        return n;
    }
    return n + (4 - r);
}
```

## Printing

`print(x)` writes one line. Build strings with `+`:

```sleela
print("body=" + body + " aligned=" + align4(body));
print("symmetric = " + (senderSize == receiverSize));   // prints true/false
```

Note that a `boolean` expression can be concatenated into a string directly —
handy for quick assertions in a `main()`.

## A complete runnable

```sleela
#sleela 1.2
class Counter {
    int sumTo(int n) {
        int total = 0;
        for (int i = 1; i <= n; i = i + 1) {
            total = total + i;
        }
        return total;
    }
    void main() {
        print("sum 1..10 = " + sumTo(10));   // 55
    }
}
```

You now have enough language to write real Wrappers. Next, learn how the build
selects and drives them.

**Next:** [Lesson 05 — The `.sst` sheet & Nordshrift](05-sst-and-nordshrift.md)
