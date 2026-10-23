# Lesson 14 — The text & String family

> Series: [index](README.md) · prev: [13 — The Metadata Class standard](13-metadata-class.md)

SLeeLa's built-in `String` scalar is deliberately small: source code can
**concatenate** with `+` and **compare** with `==` / `!=`, and that is all.
There is no `char` type and no char-level indexing. The `lib/text` family builds
a comprehensive, honest String surface on top of that primitive, in three
layers you can see directly in the runnable reference
[`lib/text/text-string-demo.sleela`](../lib/text/text-string-demo.sleela).

The classes: `SLString` (the value surface), `SLStringBuilder` (mutable),
`SLStringBuffer` (synchronized mutable), `SLFormatter` (assembly/templating),
plus `SLRegex`, `SLCharset`, `SLTextReader`, `SLTextWriter`.

## Layer 1 — In-language operations (real)

Anything expressible with `+`, `==`, stored fields, loops, and `String[]`
arrays is implemented for real and returns a real result:

```sleela
SLString s = new SLString();
s.set("SLeeLa");
s.concat(" text");          < "SLeeLa text" >
s.append("!").prepend(">> ");< ">> SLeeLa!" (fluent) >

SLString g = new SLString();
g.set("ab");
g.repeat(3);                 < "ababab" >
g.repeatWithSep(3, "-");     < "ab-ab-ab" >

String[] parts = new String[3];
parts[0] = "one"; parts[1] = "two"; parts[2] = "three";
s.join(parts, ", ");         < "one, two, three" >
s.joinNonEmpty(parts, "/");  < skips null / "" elements >

String[] opts = new String[2]; opts[0] = "red"; opts[1] = "blue";
g.equalsAnyOf(opts);         < membership test, real == in a loop >
```

`SLFormatter` adds real positional templating — the caller supplies the literal
segments already split, so no char search is needed:

```sleela
String[] seg = new String[3];
seg[0] = "Hello, "; seg[1] = "! You have "; seg[2] = " messages.";
String[] arg = new String[2]; arg[0] = "Max"; arg[1] = "3";
f.template(seg, arg);        < "Hello, Max! You have 3 messages." >
```

## Layer 2 — The char-cell view (also real)

The opaque `String` cannot be indexed, but a **char-cell view** can: represent
the text as a `String[]` whose cells are the individual glyphs. Because the
caller supplies that decomposition explicitly, char-level operations become
genuinely in-language — **no bridge, no faking**:

```sleela
String[] hello = new String[5];
hello[0] = "h"; hello[1] = "e"; hello[2] = "l"; hello[3] = "l"; hello[4] = "o";

s.lengthCells(hello);              < 5 >
s.charAtCells(hello, 1);           < "e" >
s.substringCells(hello, 1, 4);     < "ell" >
s.indexOfCell(hello, "l");         < 2 >
s.reverseCells(hello);             < "olleh" >
s.columnRight(hello, 8, ".");      < "...hello" (absolute-column pad) >
```

Produce cells at the boundary — e.g. from the bridged `chars()`/`split()` the
runtime fulfils, or from a known literal set — then do dependable char-level
work entirely in the language.

## Layer 3 — Bridged operations (honest descriptors)

Char-level work on a *raw, opaque* `String` (not decomposed into cells) still
requires the runtime. SLeeLa never fakes a result: each such method returns a
stable, inspectable **bridge descriptor** that the native runtime fulfils.

```sleela
s.length();        < "text.bridge:length:>> SLeeLa!"      (runtime yields int) >
s.toUpperCase();   < "text.bridge:toUpperCase:>> SLeeLa!"                        >
s.indexOf("L");    < "text.bridge:indexOf:>> SLeeLa!:L"    (runtime yields int) >
s.split(" ");      < "text.bridge:split:>> SLeeLa!: "      (yields string[])    >
```

`bridgeKind(op)` tells a caller the type the runtime will yield (`int`,
`boolean`, `string`, `string[]`), and `bridgeOp(fields)` reads the op name back
out of a split descriptor — so a descriptor can be reasoned about in-language.

## Run the reference demo

The demo is a single self-contained Wrapper that drives all three layers:

```sh
impl/build/sleela run lib/text/text-string-demo.sleela
```

It prints the in-language results, the real char-cell results, and the honest
bridge descriptors side by side.

## The contract

A text method is **never a silent lie**. If an operation can be done with the
language's real primitives (Layers 1–2), it is, and returns a real value. If it
needs raw code units from an opaque String (Layer 3), it says so by returning a
`text.bridge:` descriptor. That is the same "never fake it" discipline the
codec and Java-envelope surfaces follow.

## Where to look next

- Runnable reference: [`lib/text/text-string-demo.sleela`](../lib/text/text-string-demo.sleela).
- The split classes: `lib/text/SLString.sleela`, `SLStringBuilder.sleela`,
  `SLStringBuffer.sleela`, `SLFormatter.sleela`.
- The core/base String contract: `lib/core/SLString.sleela` (see `lib/CORE.BASE.md`).
