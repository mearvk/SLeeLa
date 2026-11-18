# SLeeLa 3.2 language API

Normative text: [`SLEELA.syntax`](../../../SLEELA.syntax) sections 44 (literal, initializer, labels, `finally`),
45 (the char specification) and 46 (the array specification). This page is the quick reference; where it and the
specification differ, the specification wins. Section 44 forms require `#sleela 3.1`; the typed-char
and array forms in sections 45 and 46 require `#sleela 3.2`. Earlier source declarations retain their
historical feature gates.

## Character (Syntax 3.2)

| Form | Meaning |
|---|---|
| `'a'`, `'\n'`, `'\u0041'`, `'\''`, `'é'` | a character literal; its value is the UTF-16 code unit and its static type is `char` |
| `char c = 65;` | a `char` holds 0 to 65535; a store keeps the low 16 bits (`char c = 65601;` is `65`) |
| `c + 1`, `-c`, `~c`, `c * 2` | arithmetic on a char gives an `int` |
| `c++`, `c += 2` | stay `char` and wrap at 16 bits |
| `(char) n` | narrows to 16 bits; `(char) -1` is `65535` |
| `"x" + c`, `s += c`, `print(c)` | a value of static type `char` is shown as the character, not its number |
| `SLChar.toString(c)` | the one-character text, usable where the static type is lost (`Any`, an `int` parameter) |

A character outside the basic multilingual plane is a lexical error (it is two code units).
Library: `lib/char/SLChar.sleela` (classification, case, digits, surrogates, `toCodePoint`, `compare`).

## Arrays (Syntax 3.2)

| Form | Meaning |
|---|---|
| `int[] a = {3, 1, 2};` `static int[] p = {2, 3};` | single-rank array initializer (local or field), available in 3.1; `{}` and a trailing comma are allowed |
| `new int[]{4, 5, 6}` | an array expression |
| `new int[3][4]`, `new int[2][3][4]` | rectangular creation, Syntax 3.2; any rank up to 8 sized dimensions |
| `new int[3][]` | Syntax 3.2 jagged array: three rows, each `null` until assigned |
| `new int[][]{{1, 2}, {3}}` | Syntax 3.2 nested initializer |
| `a.length` | Syntax 3.2 array length; read-only (`a.length = 5` and `a.length++` are errors); `arrayLength(a)` remains available below 3.2 |
| `a[i]` | the index is checked on every access; out of range is a runtime error (catchable inside a `try`, `SLEELA.syntax` 37.5) |

`new T[n]` fills with `0`, `0.0`, `false` or `null` by element type. An array is a reference: assignment and argument
passing share it. Limits: 4096 live arrays (each row of a grid is one), 2^25 elements per array, 32 bytes per element.
Library: `lib/array/` — `SLIntArray`, `SLDoubleArray`, `SLRefArray`, `SLArray2D` (copy, fill, sort, search, sum, transpose, text).

## Labeled statements and abrupt completion (Syntax 3.1)

| Form | Meaning |
|---|---|
| `outer: for (...) { ... }` | label a loop, a `switch` or a block |
| `break outer;` / `continue outer;` | leave the labeled statement / start the next iteration of the labeled loop |
| `return`, `break`, `continue` inside `try`/`catch` with a `finally` | the `finally` runs before the jump (a `return` inside the `finally` replaces the value) |
| `continue` inside a `switch` in a loop | continues the loop |

An undefined label is `break to undefined label 'x'`.

## Version

`#sleela 3.01` is the numeric spelling of Syntax 3.1; `#sleela 3.2` is the current edition. The supported range is 1.0 to 3.2, and unversioned source defaults to 3.2.
