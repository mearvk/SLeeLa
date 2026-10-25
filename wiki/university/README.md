# University corpus (sourced wikitext)

A small, sourced, neutral MediaWiki corpus about the **university** as an
institution — definitional concept pages, not a list of institutions. Built to
be imported into a **self-hosted** wiki and to grow over time.

## Pages

| File | Concept |
|------|---------|
| [`Index.wiki`](Index.wiki) | Corpus index / table of contents |
| [`University.wiki`](University.wiki) | Definition, etymology, mission |
| [`History-of-the-university.wiki`](History-of-the-university.wiki) | Medieval origins, the *studium generale* |
| [`University-governance.wiki`](University-governance.wiki) | Autonomy, academic freedom, shared governance |
| [`Academic-degree.wiki`](Academic-degree.wiki) | Bachelor's, master's, doctoral levels |

## Conventions

- **Definitional and neutral.** Pages assert only what cited sources establish
  about the concept. No specific institutions are ranked or enumerated.
- **Cited.** Every substantive claim carries a `<ref>` to an authoritative
  source (UNESCO, Encyclopædia Britannica, AAUP/AAU, Cambridge, and
  encyclopaedic references). Source text is **paraphrased, not copied**, and
  each reference notes "Rephrased for compliance."
- **Self-hosted only.** This is MediaWiki markup for a private/self-hosted wiki.
  It is **not** a submission to any public encyclopedia (e.g. Wikipedia), whose
  policies on notability, independent reliable sourcing, original research, and
  mass auto-generated pages govern what may be published there.

## Relationship to the compiler's wiki emanation (syntax 2.4)

The SLeeLa compiler can emanate *structural* wikitext about a compiled document
(classes, members, branders, timestamps — see `SLEELA.syntax` §18.5). That
output is generated from the AST and is about the program's structure. This
corpus is different: it is **subject-matter content** whose facts come from
external cited sources, which a compiler cannot invent. The two are
complementary — structure from the compiler, sourced substance here.

## Extending the corpus

Add a new `.wiki` page in this directory, cite authoritative sources in a
`== Sources ==` section, tag it `[[Category:University corpus]]`, and link it
from `Index.wiki`.
