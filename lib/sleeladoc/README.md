<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa SleelaDoc — the extractor

**Status: the SLeeLa-side SleelaDoc extractor — the Javadoc-tool analogue.**
This directory reads **SleelaDoc** doc-comments out of a `.sleela` Wrapper™ and
renders a Markdown or HTML API page, the way `javadoc` renders Java. The
convention it consumes is defined in
[`manifesto/SLEELADOC.md`](../../manifesto/SLEELADOC.md); this package is the
tool that puts it to work.

SleelaDoc documents **intent**, not just signatures (the SLeeLa mandate is
"express intent, not mechanism"). A doc comment is an ordinary angle-bracket
comment whose sentinel token is `@doc` (or `@page`), so it lexes as a comment
today and needs no compiler change.

## The classes

One SLeeLa source file per front-end object, in the house style; every class
carries the Metadata Class metapattern.

| File | Role |
| --- | --- |
| `SLDocTag.sleela` | one parsed `@tag` (name without the `@`, plus its value). |
| `SLDocComment.sleela` | one SleelaDoc block: free-prose summary + ordered tags. |
| `SLDocMember.sleela` | a documented declaration (class / field / method) + its doc comment and signature. |
| `SLDocModel.sleela` | the parsed model of a whole unit: members in declaration order. |
| `SLDocScanner.sleela` | the token-stream reader that lifts doc blocks and tags and attaches them to the following declaration. |
| `SLDocRenderer.sleela` | renders an `SLDocModel` as Markdown (`toMarkdown`) or minimal HTML (`toHtml`). |
| `SLSleelaDoc.sleela` | the one-object façade a caller drives (`begin` → `feed` → `toMarkdown`/`toHtml`). |
| `SLPackage.sleela` | package facade. |
| `sleeladoc-demo.sleela` | the runnable reference (`class SleeladocDemo` with `main`). |

## The doc-comment forms the scanner recognizes

The go-forward comment style is the angle-bracket form (`<` / `<<`; see
[`comment-style.md`](../../.kiro/steering/comment-style.md)). SleelaDoc layers a
`@doc` / `@page` sentinel on top:

| Form | Meaning |
| --- | --- |
| a block comment whose first content token is `@doc` | a **SleelaDoc block**, attached to the next declaration |
| a line comment whose first content token is `@doc` | a **one-line SleelaDoc comment** |
| a block comment whose first content token is `@page` | a **page banner** for a long-form document |

Inside a block, a line whose first token is an `@tag` starts a tag (`@name`
then its value); any other run is summary prose.

> Because an angle comment cannot contain the angle delimiters themselves, the
> literal opener/closer glyphs are described here rather than quoted: a block
> opens with the two-glyph opener followed by `@doc`, runs over following lines,
> and closes with the two-glyph closer. A one-line doc opens with the one-glyph
> opener followed by `@doc` and closes with the one-glyph closer on the same
> line. See `manifesto/SLEELADOC.md` for worked, copy-pasteable examples.

## Tags

`@title`, `@intent`, `@param`, `@returns`, `@axiom`, `@thesis`, `@invariant`,
`@proof`, `@congruence`, `@evidence`, `@to`, `@see`, `@page`.

## Driving it (syntax 2.5)

SLeeLa's built-in `String` is **opaque** (no in-language slicing), so — exactly
as in `lib/text/SLString` and `lib/xml` — the scanner reads each source line as
its **whitespace-separated tokens** (a `String[]`), which a caller produces at
the char-cell boundary. A caller drives one façade object:

```java
SLSleelaDoc doc = new SLSleelaDoc();
doc.begin("Reason");

// feed each tokenized source line in declaration order;
// the second argument is true when the line's first token begins uppercase
// (a class-type head), which the caller computes at the char-cell boundary.
doc.feed(opener2("@doc"), false);          // a doc block opens
doc.feed(tokens("@title", "Reason"), false);
doc.feed(tokens("class", "Reason", "{"), true);
// ...

String md   = doc.toMarkdown();  // a Markdown API page
String html = doc.toHtml();      // a minimal HTML fragment
print(doc.documentedCount() + " / " + doc.memberCount());
```

Run the executable reference:

```sh
# from the repository root, with the SHA-256 manifest selected
SLEELA_SHA256_MANIFEST="$(pwd)/security/sha256-manifest.json" \
  ./impl/build/sleela run lib/sleeladoc/sleeladoc-demo.sleela
```

It feeds the tokenized lines of a small documented class `Reason` (a class-level
doc block, a one-line field doc, and a method doc block) and prints the rendered
Markdown and HTML.

## Declaration recognition

From tokens alone, the scanner records three member kinds:

| First tokens | Member |
| --- | --- |
| `class <Name>` | a class (kind `CLASS`) |
| a type head, a name, then a `(` token | a method (kind `METHOD`) |
| a type head then a name | a field (kind `FIELD`) |

A *type head* is a known scalar (`void`, `int`, `long`, `boolean`, `double`,
`float`, `char`, `String`, `byte`) or a token the caller flags as beginning with
an uppercase letter (the SLeeLa class-name convention). A completed doc comment
is held pending and attached to the next declaration recorded.

## Relationship to the compiler's built-in doc outputs

The extractor renders the **authored intent** (the `@doc` prose and tags). It is
complementary to the compiler's own author-marked outputs, which render the
**declared surface** as a consequence of compilation:

- **Wikitext emanation** — `sleela compile <file> --wiki` → a MediaWiki page
  (`impl/frontend/wikigen`); see
  [`manifesto/SLEELADOC.WIKI.md`](../../manifesto/SLEELADOC.WIKI.md).
- **Originated source twin** — `*.originated.sleela` (`impl/frontend/originate`).

Use `sleeladoc` for intent-level API pages; use `--wiki` for a branded,
member-by-member catalog.

## Implementation boundary

The scanner classifies lines from tokens; it is a pragmatic reader for the
documented surface, not a second parser (it does not type-check). Per-character
HTML escaping of values is a char-cell concern handled by the `lib/xml`
serializer; the identifier/short-text surface a doc page renders is passed
through faithfully. These are the same honest boundaries the `lib/codecs`,
`lib/compression`, and `lib/xml` families carry.
