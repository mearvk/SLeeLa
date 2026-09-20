# Lesson 07 — Conducted Methods

> Series: [index](README.md) · prev: [06](06-the-triplet.md) · next: [08 — Subjects & the sheet model](08-subjects.md)

SLeeLa programs can ask about their own structure. The **conducted methods** are
built-in queries resolved against a shared model sheet (`SHEET.sheet`). They let
a program reason about *itself* — the "self-describing system" idea from Lesson 01.

## The vocabulary

| Method | Answers |
|---|---|
| `role(name)` | the role an object plays in the system |
| `insight(name)` | a known gloss/insight the object resolves |
| `route(a, b)` | whether related objects route to a known congruence |
| `congruent(a, b)` | whether two objects are congruent |
| `sysdepth()` | the system's declared depth |
| `degreemax()` | the complexity-degree ceiling |

## Seeing them in a real Wrapper

`examples/symmetry/src/core/Message.sleela` prints the roles of the objects it
declares:

```sleela
void main() {
    print("== Message (Request/Response framing) ==");
    print("Message role  = " + role("Message"));
    print("Request role  = " + role("Request"));
    print("Response role = " + role("Response"));
    // ...
}
```

When run, each `role("...")` resolves against the catalogued objects the sheet
declares for that build (the sheet lists them; the runtime answers).

## Querying from the driver, too

Nordshrift exposes the same model from the command line:

```sh
export SLEELA_SHEET="$PWD/SHEET.sheet"
impl/build/nordshrift objects            # list catalogued objects
impl/build/nordshrift relevance --target=c Thread
```

## Why this matters

Conducted methods are how SLeeLa keeps *meaning* next to *code*: an object isn't
just a class name, it has a declared role, insight, and congruence relationships
the program (and the tooling) can consult. It's the difference between a program
that merely runs and one that can **explain its own shape**.

A caution consistent with the rest of the repo: these are **structural** queries
over declared model objects — roles and congruences of *software* objects — not
judgments about people.

**Next:** [Lesson 08 — Subjects & the sheet model](08-subjects.md)
