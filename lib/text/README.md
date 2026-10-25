<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Text / String Library

**Status: comprehensive language-facing text surface.** This directory is the
SLeeLa `text` family: the String value type, mutable builders, a formatter, a
regular-expression contract, a charset contract, and buffered text readers and
writers. It is the text sibling of the audio (`lib/codecs/`), image
(`lib/image/`), and compression (`lib/compression/`) families.

## The honest two-layer model

SLeeLa's built-in `String` is an **opaque scalar**: source code can only
**concatenate (`+`)** and **compare (`==` / `!=`)** strings. There is no `char`
type, no indexing into a String, and no built-in `length`/`charAt`/`substring`.
So every class here is organised in two honest layers:

- **In-language operations** — real work expressible with `+`, `==`, stored
  fields, and `String[]`/`int[]` arrays with loops. Implemented here; they
  return real results.
- **Bridged operations** — char-level or runtime work (length, charAt,
  substring, case mapping, trimming, splitting, matching, ordering, encoding,
  real I/O) that needs code units the language never exposes. Each returns a
  stable, inspectable **bridge-call descriptor** that the native runtime
  fulfils — **never a faked result**, the same "never fake it" contract the
  codec and Java-envelope surfaces use.

### Bridge descriptor schema

```
text.bridge:<op>[:<receiver>][:<arg>...]
```

Field 1 is always the literal `text.bridge`; field 2 is the op name; the rest
are the receiver value then the operation arguments. `SLRegex` adds a family
tag (`text.bridge:re:<op>:...`), `SLCharset` uses `text.bridge:charset:<op>:...`,
and the readers/writers use `text.bridge:reader:...` / `text.bridge:writer:...`.
`SLString.bridgeKind(op)` reports the type the runtime yields for an op
(`int` / `boolean` / `string` / `string[]`), so a caller knows how to read a
result even though the descriptor itself is carried as a `String`.

## The classes

| File | Role |
| --- | --- |
| `SLString.sleela` | The comprehensive String surface: in-language concat/equals/empty/append/prepend/repeat/join/quote/copy and value helpers (`concatAll`, `wrapIfNotEmpty`, `prefixIf`/`suffixIf`); a real **char-cell view** over an explicit `String[]` (charAt, substring/slice, indexOf, pad left/right/**center**, **truncate**, **insert/delete**, multi-cell **subsequence** search/count/replace, **split**, explicit-table **case mapping**, **trim**); and the full bridged char-level API (length, charAt, substring, indexOf, contains, startsWith/endsWith, toUpperCase/toLowerCase, trim, strip/**stripLeading**/**stripTrailing**, **normalize**, replace/replaceAll/**replaceFirst**, split, compareTo/**compareToIgnoreCase**, equalsIgnoreCase, hashCode, **codePoints**, format/**formatWith**). |
| `SLStringCells.sleela` | A pure, **stateless companion** to `SLString`: every method takes its char-cell `String[]` as an argument and returns a real result (no seeded `value`, no bridge). Join/rejoin, slice/substring, reverse, single- and multi-cell search (`indexOf`/`indexOfSeq`/`lastIndexOfSeq`/`contains`/`count`), subsequence `replaceSeq`, `insert`/`deleteRange`, `splitOn`/`fieldCount`, explicit-table case `map`/`mapToString`, `trim`/`isBlank` over a caller-supplied blank set, `padLeft`/`padRight`/`padCenter`/`truncate`, `equals`/`commonPrefixLength`, and in-language **ASCII cell helpers** (`asciiCells`, `asciiCode`, `asciiUpperSet`/`asciiLowerSet`, `toUpperAscii`/`toLowerAscii`) that produce a dependable char-cell view for ASCII literals with no bridge. For lexers, column/table formatters, and template engines that think in arrays. |
| `SLStringBuilder.sleela` | A genuinely functional mutable builder: fluent append (String/int/boolean), appendLine/newline/prepend, repeat, join/appendAll over `String[]`, tracked length + edit count, `toString`; char-level ops bridged. |
| `SLStringBuffer.sleela` | The synchronized sibling (the Java StringBuilder/StringBuffer split); same builder surface, each mutation guarded by `beginSync()`/`endSync()` with honest lock-depth tracking. |
| `SLFormatter.sleela` | In-language template assembly (`kv`, `wrap`, `list`/`bullets`, `pad`, `render1..3`); printf-style `format`/`padTo`/`alignRight` bridged. |
| `SLRegex.sleela` | Pattern/flags management + in-language pattern assembly (`anchored`, `grouped`, `optional`, `alternate`, `withFlags`); all matching (`compile`, `matches`, `find`, `findAll`, `group`, `replaceAll`/`replaceFirst`, `split`) bridged. |
| `SLCharset.sleela` | Canonical-name + alias resolution and classification (`isUnicode`/`isSingleByte`/`isAsciiCompat`) in-language over well-known charset constants; `encode`/`decode`/`newEncoder`/`newDecoder` bridged. |
| `SLTextReader.sleela` | Buffered text input: in-language line/offset bookkeeping and an in-memory lookahead buffer the caller can prime; `open`/`read*`/`ready`/`closeStream` bridged. |
| `SLTextWriter.sleela` | Buffered text output: a real in-language accumulation buffer (`write`/`writeLine`/`indent`/`writeAll`); `open`/`flush`/`drain`/`closeStream` bridged. |
| `text-string-demo.sleela` | A self-contained, runnable reference (`main`) that drives the surface the way an app would. |

The core/base String value contract lives at `lib/core/SLString.sleela` (per
`lib/CORE.BASE.md`); the Java conformance envelope is
`lib/java/java/lang/String.sleela` (with its `StringBuilder`/`StringBuffer`/
`StringLatin1`/`StringUTF16`/… siblings).

## Run the reference

```sh
# from the repository root
./impl/build/sleela run lib/text/text-string-demo.sleela
./impl/build/sleela run lib/text/string-cells-demo.sleela
```

Expected from `text-string-demo`: the in-language ops print real results
(`concat`, `equals` -> true, `repeat`, `join` -> `one, two, three`, independent
`copy`), the new char-cell helpers print real results (`padCenter 9` ->
`..hello..`, `truncate 4` -> `hel~`, `replaceSeq ll` -> `he-o`, `mapCells` ->
`HEllo`, `splitOn ,` -> `x|y|z`), and the char-level ops print honest
`text.bridge:...` descriptors with their yielded kind.

Expected from `string-cells-demo` (the stateless `SLStringCells` toolkit, all
real): over `mississippi`, `indexOf ss` -> `2`, `count ss` -> `2`,
`replace iss` -> `m------ippi`; `split` on `,` yields three fields; explicit
case map -> `ABC`; `trim` -> `hi`; `padCenter 6` -> `..hi..`; `truncate 4` ->
`mis~`.

## Reserved-name notes (SLeeLa gotchas)

- `close()`, `lock()`, and `join()` are **reserved built-ins** (the thread /
  monitor / reach-handle surface). An *unqualified* in-class call to `join(...)`
  is captured by the concurrency built-in before method resolution, so the
  char-cell rejoin in `SLStringCells` is named `joinCells()` (with `joinWith()`
  for the separator form). The stream teardown is `closeStream()`; the buffer
  monitor is `beginSync()` / `endSync()`.
- A char literal `'A'` is **not** valid SLeeLa — there is no `char` type, and a
  bare `'` is a lexical error even inside an angle-bracket comment. Use the
  double-quoted single-glyph cell (`"A"`) and keep apostrophes out of comments
  on code-adjacent lines.
- Binary operators need surrounding spaces (`n - 1`, not `n-1`) or the lexer
  reads them as one identifier.
- Comments are `//` only — the lexer has no `/* */` block comments.

## Metadata standard

Each class carries the eleven-attribute **Metadata Class** record (per
`.kiro/steering/metadata-class-standard.md` and `SLEELA.syntax` §15) with
`get`/`set` pairs and honest, citable `metaNationalSchedule` values (The Java
Language Specification; Unicode / ISO/IEC 10646; the IANA Charset Registry).

## Implementation boundary

These classes model, assemble, classify, and report. Real char-level transforms,
regex matching, charset transcoding, and I/O run in the native runtime, reached
through the bridge descriptors — a bridged op with no runtime handler surfaces
honestly rather than returning a fabricated result.
