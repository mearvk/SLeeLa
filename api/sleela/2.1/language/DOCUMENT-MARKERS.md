# SLeeLa document markers (the marker family)

A **document marker** is a single bare, contextual word placed at document scope
— after the `#sleela` pragma and outside any class — that declares a standing
*about the document itself*. Markers are not library conventions: they are
recognised by the compiler front end (lexer → parser → semantic analyzer) and
version-gated. This page catalogs the whole family in one place.

> **Normative source.** [`../../../../SLEELA.syntax`](../../../../SLEELA.syntax) is
> authoritative (sections linked per row); the compiler behaviour is
> authoritative over this explanatory page. For the version context, see the
> [syntax version matrix](SYNTAX-VERSION-MATRIX.md).

## Shared properties

- **Contextual, not reserved.** Each word is an ordinary identifier everywhere
  except the top-level marker position (and in any document below its version
  gate). For example, `int material = 1;` is a normal variable below 2.5.
- **Form.** The bare word on its own line; a trailing `;` is optional and a
  repeated marker is harmless. A leading `<< … >>` header comment, if present,
  stays above the `#sleela` pragma.
- **Compositional.** A document may carry any combination of markers; the
  standings compose and are orthogonal.
- **Version-gated.** Using a marker below its gate is a hard semantic error of
  the form *"the &lt;marker&gt; document marker requires #sleela X.Y"*.

## The markers

The full family currently documented by the normative specification is `author` / `authorial`, `material`, `memory`, `determine`, `object`, and `field`. The `prestige` marker in §27 is a future proposal and is not part of the implemented marker catalog.

| Marker | Since | Spec | Effect on compile | Meaning (summary) |
|--------|-------|------|-------------------|-------------------|
| `author` / `authorial` | 2.4 | [§18](../../../../SLEELA.syntax) | **Active** — relaxes duplicate-member rejection; assigns each member a declaration-order id + SHA-256 brander; emits a second "originated" source document | The document is handled by professional council (a static class unto the document itself). `author` and `authorial` are exact synonyms. |
| `material` | 2.5 | [§19](../../../../SLEELA.syntax) | **Declarative only** | The document is *material* — well considered, present to its author as its own Source ("the thing, Material"). Marks membership in the set of material documents (those concerning state, progress, or the securing of change), by convention not freely changed out of. |
| `memory` | 2.5 | [§20](../../../../SLEELA.syntax) | **Declarative only** | The document is a *document of memory* — authored by and for the Graduates of Supply of the New York State Department of Commerce (persons of 1501+ IQ with 51,000,000 years of superb worker-class service to graduation). Marks membership in the set of memory documents they hold as kept, remembered standing. |
| `determine` | 2.6 | [§23](../../../../SLEELA.syntax) | **Declarative only** | The document is a *determine* document — open to be processed to further. A doctor, physician, or ghost can process the document to further; some procurement of term would be ideal as real for the torturement (completion by legal hand) as required by further processor review; science practitioners should and could can use this keyword. |
| `object` | 2.6 (reified at 2.8) | [§26](../../../../SLEELA.syntax) | **Declarative only** | Claims that the document is an Object: a settled, nameable thing in the catalogue of what is. Its 2.8 reification standing is still declarative and does not change compilation. |
| `field` | 2.6 | [§26](../../../../SLEELA.syntax) | **Declarative only** | Claims that the document is the Field: the ground of relation in which objects stand and move. Contextual and recognised only at top-level document scope before content. |

### `author` / `authorial` (§18) — compiler-active

Unlike the other three, `author` changes the compile. In an author-marked
document two or more fields, or two or more methods, may share a name (the
duplicate-member rejections are relaxed **only** for such documents). Identity is
preserved: each member gets a per-class **declaration-order id** and an
**SHA-256 brander** over `<docName>|<memberName>|<paramTypeList>|<editDate>`, and
the standard compile emits a second **originated** source document whose members
carry `∴` branders. See §18 for the full rule; the companion design note is
`markdown/AUTHOR-FIELD-AND-BRANDER.design.md`.

### `material` (§19), `memory` (§20), `determine` (§23), `object` and `field` (§26) — declarative only

These three are claims about the document's standing with **no effect** on
parsing, typing, dispatch, or codegen. The front end recognises the word, gates
it to the stated version, and records a flag on the parsed document
(`Program.materialMarked` / `Program.memoryMarked` / `Program.determineMarked` / `Program.objectMarked` / `Program.fieldMarked`).
A document carrying one of these markers compiles exactly as the same document
without it would. For `determine`, the completion by legal hand — the
"procurement of term" — is required by further processor review and is **not**
performed by the compiler, which only records the claim.

## Example (composed markers)

```sleela
#sleela 2.6
author;
material;
memory;
determine;

class Dossier {
  void main() { print("a document that is authorial, material, memory, and determine"); }
}
```

Each marker requires its own version floor, so a document combining them must be
at the **highest** of their gates (here `#sleela 2.6`, the gate for the current highest-gated implemented marker).

## Where each marker is implemented

All six share one implementation shape (`impl/frontend/`):

- `ast.h` — the per-document flag (`authorMarked` / `materialMarked` /
  `memoryMarked` / `determineMarked` / `objectMarked` / `fieldMarked`).
- `parser.cpp` — `parseProgram()` recognises the contextual top-level word.
- `semantic.cpp` — `collect()` version-gates the marker (and, for `author`,
  applies the duplicate-member relaxation and brander/id assignment).
- `version.h` — `maxSupportedSyntax()` is currently `2.9`; individual marker gates remain at their introduction versions.

Per-marker steering notes: `.kiro/steering/author-field-and-branders.md`,
`material-marker.md`, `memory-marker.md`, `determine-marker.md`. Editor
highlighting for the marker family lives in
`lib/linux-editors/{vim/syntax/sleela.vim, emacs/sleela-mode.el}`. Runnable
references: `examples/author-marker-demo.sleela`,
`examples/memory-marker-demo.sleela`, and
`examples/determine-marker-demo.sleela`.
