# Lesson 02 — Your First Wrapper™

> Series: [index](README.md) · prev: [01](01-what-is-sleela.md) · next: [03 — Types, structs & classes](03-types-structs-classes.md)

A **Wrapper™** is a `.sleela` file: one program unit. Let's write one and read
its shape line by line.

## The whole file

```sleela
#sleela 1.2
// greeter.sleela — a first Wrapper.
class Greeter {

    // A pure helper: build a greeting string.
    String greet(String who) {
        return "hello, " + who;
    }

    // Entry point. Every runnable Wrapper has a main().
    void main() {
        print(greet("SLeeLa"));
        print(greet("world"));
    }
}
```

## Reading it

- **`#sleela 1.2`** — the version pragma. Always the first line.
- **`// ...`** — line comments, C/Java style.
- **`class Greeter { ... }`** — a class is the top-level container, just like Java.
- **`String greet(String who)`** — a method: return type, name, typed parameters.
- **`return "hello, " + who;`** — `+` concatenates strings.
- **`void main()`** — the entry point of a runnable program.
- **`print(...)`** — writes a line to standard output.

## The rules that matter early

1. **One version pragma**, first line, `#sleela <n.n>`.
2. **A runnable Wrapper needs `main()`** (a library-only Wrapper need not).
3. **Statements end in `;`**; blocks use `{ }`.
4. **Types are explicit** on declarations and signatures (Lesson 03).

## Compare to a real one

Open the demo Wrapper and note the identical shape — a class, typed methods,
`while`, and `print`:

```sh
sed -n '1,25p' impl/nordshrift/examples/demo/src/Demo.sleela
```

```sleela
#sleela 1.1
class Demo {
    int square(int n) {
        return n * n;
    }
    void main() {
        int i = 1;
        while (i <= 5) {
            print("square(" + i + ") = " + square(i));
            i = i + 1;
        }
        print("done");
    }
}
```

That's a complete, buildable Wrapper. In Lesson 05 you'll drive it through
Nordshrift; first, Lessons 03–04 cover the language pieces inside the braces.

**Next:** [Lesson 03 — Types, structs & classes](03-types-structs-classes.md)
