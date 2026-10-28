# SleelaDoc — the SLeeLa documentation brand

**SleelaDoc** is SLeeLa's in-source documentation convention: structured
doc-comments that make a `.sleela` **Wrapper™** self-describing. It is to a
Sleela source file what Javadoc is to Java — but it documents *intent* (the
Sleela mandate is "express intent, not mechanism"), not just signatures.

SleelaDoc is a **comment convention**, so it is fully compatible with the
compiler as it exists today: every SleelaDoc block is an ordinary angle-bracket
comment and is ignored by the lexer. The `sleeladoc` extractor
(`lib/sleeladoc/`) parses these blocks out of any Wrapper™ without running it,
and emits a Markdown or HTML API page.

## Block forms (go-forward, angle-bracket style)

The go-forward house comment style is the angle-bracket form (`<` / `<<`; see
[`comment-style.md`](../.kiro/steering/comment-style.md) and `SLEELA.syntax`
§1.1). SleelaDoc is layered on top of it: a doc comment is an ordinary angle
comment whose **first non-whitespace token is `@doc`** (a block) or whose line
begins with `< @` (a line). The `@doc` sentinel is what distinguishes a *doc*
comment from an ordinary implementation note, and it is what the extractor
greps for.

| Form | Meaning |
|------|---------|
| `<< @doc ... >>` | A **SleelaDoc block** — attaches to the class, struct, field, or method that immediately follows it. |
| `< @doc ... >`   | A **SleelaDoc line** — a single-line doc comment attached to the next declaration. |
| `<< @page N: Title ... >>` | A **page banner** — marks a numbered page of a long-form document (a manifesto, spec, or essay carried in source). |

Ordinary `<` / `<<` (and legacy `//`, `/* */`) comments remain non-doc
implementation notes — only the `@doc` / `@page` sentinel promotes a comment to
SleelaDoc.

> **Migration note.** Earlier SleelaDoc used the Java-style `/** ... */`, `///`,
> and `/*! page N */` forms. Those still lex as legacy comments **only where the
> legacy `/* */` and `//` forms are still accepted**; new and edited files use
> the angle-bracket `<< @doc >>` / `< @doc >` / `<< @page >>` forms above, which
> are the go-forward house style and the forms the `sleeladoc` extractor
> recognizes. Avoid apostrophes inside an angle comment (a lexical error); phrase
> doc prose without contractions or possessives.

## Tags

A SleelaDoc block is free prose plus optional `@tags`. The brand's tag set:

| Tag | Purpose |
|-----|---------|
| `@title`     | The name of the work or unit. |
| `@intent`    | *What* this expresses (never *how*). |
| `@param`     | A named parameter of a method: `@param name description`. |
| `@returns`   | What a method yields. |
| `@axiom`     | A stated first principle taken as given. |
| `@thesis`    | A claim the unit argues or demonstrates. |
| `@invariant` | A condition that must always hold. |
| `@proof`     | The reasoning or runnable demonstration that backs a thesis. |
| `@congruence`| A structural sameness linking this unit to another (see `SHEET.sheet`). |
| `@evidence`  | Status of a statement: OBSERVED / DERIVED / MODELED / ASSUMED. |
| `@to`        | The intended reader. |
| `@see`       | A cross-reference. |

## Example

```sleela
#sleela 2.5
material;
<< @doc
   @title    Reason
   @intent   Model the smallest unit of thought that can be checked.
   @axiom    A claim that cannot be checked is not yet knowledge.
   @thesis   Structured doubt increases the value of a belief.
   @proof    check() runs the demonstration and prints the result.
   @evidence DERIVED >>
class Reason {
  < @doc The confidence, in [0,100], after checking. >
  int confidence;

  << @doc
     @intent  Check the belief and report confidence.
     @returns the confidence in [0,100]. >>
  int check() { return confidence; }
}
```

## Extracting docs — `sleeladoc` (the Javadoc-tool analogue)

The `lib/sleeladoc/` package is the SleelaDoc **extractor**: the SLeeLa-side
tool that reads SleelaDoc blocks out of a Wrapper™ and renders an API page, the
way `javadoc` renders Java. It is one-class-per-file in the house style:

| Class | Role |
|---|---|
| `SLDocTag` | one parsed `@tag` (name + value). |
| `SLDocComment` | one SleelaDoc block: its prose plus ordered tags, and the member it attaches to. |
| `SLDocMember` | a documented declaration (class / field / method) with its doc comment. |
| `SLDocModel` | the parsed model of a whole unit: its members in declaration order. |
| `SLDocScanner` | the char-cell reader that lifts `@doc` / `@page` blocks and `@tag`s from source lines. |
| `SLDocRenderer` | renders an `SLDocModel` as Markdown or minimal HTML. |
| `SLSleelaDoc` | the façade a caller drives: scan a unit, then render. |

A runnable reference lives at `lib/sleeladoc/sleeladoc-demo.sleela`:

```sh
# from the repository root, with the SHA-256 manifest selected
SLEELA_SHA256_MANIFEST="$(pwd)/security/sha256-manifest.json" \
  ./impl/build/sleela run lib/sleeladoc/sleeladoc-demo.sleela
```

Because SLeeLa's built-in `String` is opaque (no in-language slicing), the
scanner reads an explicit **char-cell `String[]`** view of each source line —
the sanctioned real char-level mechanism (see `lib/text/SLString` and
`lib/xml`). The demo supplies the cells of a small documented unit and prints
the rendered Markdown.

## The compiler's built-in doc outputs — `--wiki` and the originated twin

Separate from the `sleeladoc` extractor, the **compiler itself** emits
documentation for an **author-marked** document (the `author;` / `authorial`
marker, syntax 2.4+) as a *consequence of compilation* — see
[`SLEELADOC.WIKI.md`](SLEELADOC.WIKI.md) for the full guide:

- **Wikitext emanation** (`--wiki` / `--wiki-dir=<dir>`): a structured MediaWiki
  page — an infobox, a sortable wikitable of every class and member with its
  declaration-order id and SHA-256 brander, and a declaration-order "Movement"
  section. Implemented in `impl/frontend/wikigen.{h,cpp}`.
- **Originated source document** (`*.originated.sleela`): the auditable twin of
  the artifact in which every member is located, originated, and carries a `∴`
  brander. Implemented in `impl/frontend/originate.{h,cpp}`.

```sh
./impl/build/sleela compile <file.sleela> --wiki --wiki-dir=out -o out/<file>.sleela
# -> out/<file>.wiki  (MediaWiki)  and  <file>.originated.sleela  (the twin)
```

These are **factual, structured** outputs over the *declared surface* (classes
and members), while the `sleeladoc` extractor renders the *authored intent* (the
`@doc` prose and tags). The two are complementary: use `sleeladoc` for
intent-level API pages, and `--wiki` for a branded, member-by-member catalog.

## The manifesto

[`manifesto.sleela`](manifesto.sleela) is the reference long-form SleelaDoc
document: a runnable ~25-page **"Manifesto to Thinking Kind."** It predates the
angle-bracket migration and is pinned at `#sleela 2.1` with the legacy `/** */`
and `/*! page N */` forms; it is the historical reference for the *brand* and
its tag vocabulary. New SleelaDoc is authored in the angle-bracket forms above.
