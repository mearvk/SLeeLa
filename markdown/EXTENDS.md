# `extends` — class inheritance and document grouping (syntax 1.10)

    Language element: the `extends` keyword, in two meanings
    Syntax version:   1.10  (the .sleela grammar version that introduces both)
    Surface:          Java-like
    Companions:       BODI.md (witness/organization vocabulary), SHEET.sheet
                      (section inheritances; the grouper terms), MUNCTION.md
    Status:           Normative for the two `extends` meanings

---

## 0. Two meanings, one keyword

As of syntax **1.10**, `extends` carries two related meanings, disambiguated by
*where* it appears:

1. **The OOD meaning — a class extends a base class.** Inside a type
   declaration, `class D extends B` makes `D` inherit `B`'s instance fields and
   methods. This is the original object-oriented meaning, now actually resolved
   by the compiler (before 1.10 it was retained authorship metadata only).

2. **The document meaning — a document extends *to* other documents.** At the
   top level of a `.sleela` document, `extends to <targets> <grouper>;` declares
   that this document naturally points to one or more other documents, organized
   by a **grouper**. One document sheet pointing to another is a first-class,
   checked relationship — linear, grouped, or grouped behind a server of
   services.

Both meanings are **version-gated at 1.10**. A document that declares `#sleela
1.9` (or lower) keeps the historical behavior exactly: `extends` on a class is
metadata only, and `extends to ...` is not accepted. This preserves every
existing program unchanged.

---

## 1. The OOD meaning — class inheritance

```sleela
#sleela 1.10
class Animal { String name; void setName(String n){ name=n; } String describe(){ return "animal "+name; } }
class Pet extends Animal { String owner; void setOwner(String o){ owner=o; } String describe(){ return "pet "+name+" of "+owner; } }
class Dog extends Pet { String fetch(){ return name+" fetches for "+owner; } }
```

Rules:

- **Fields are inherited.** A derived class's instance layout is the base's
  inherited fields first, then its own. A field with the same name as a base
  field is not duplicated.
- **Methods are inherited.** A base method is callable on a derived instance.
- **Same-name method overrides.** A derived method with a base's name replaces
  it for that class.
- **Multi-level chains inherit the whole way down.** `Dog` above inherits
  `setName` from `Animal` (its grandparent) and `setOwner`/`describe` from `Pet`.
- **Single inheritance.** One base per class (as the surface grammar already
  allowed). `implements` lists remain authorship metadata.
- **Errors are reported, not papered over.** An unknown base type and cyclic
  inheritance are compile-time semantic errors.

This maps onto the catalog's existing inheritance vocabulary (`SHEET.sheet`
`section inheritances`): a **BaseClass** is a parent in an inheritance relation;
a **DerivedClass** specializes/extends a base with `Override`/`Extension`.

## 2. The document meaning — `extends to` with a grouper

```sleela
#sleela 1.10
extends to NextChapter linear;                        // linear reals
extends to { Intro, Body, Appendix } group;           // grouped set
extends to { Orders, Billing } services ClearingHub;  // group + server of services
```

Grammar:

```text
extends to <Target> <grouper> ;
extends to { <Target> (, <Target>)* } <grouper> [<Server>] ;

<grouper> := linear | group | services
```

`to` and the grouper words (`linear`, `group`, `services`) are **ordinary
identifiers, not keywords**, so they remain free to use elsewhere; the statement
is recognized by the leading `extends` token (which is otherwise only valid
inside a type). This mirrors how the Munction™ verbs and the `ran::` namespace
keep ordinary words free.

### 2.1 The groupers (organization terms)

| Grouper | Structure | Bound | Meaning |
|---|---|---|---|
| `linear` | ordered chain — **linear reals** | congruent-linear max (3024) | this document points to the next in an ordered chain; each target is the next real in the line |
| `group` | unordered set | complexity-degree max (4 members) | the targets are gathered together under one organizer, no order implied |
| `services` | group + a **Server of Services** | complexity-degree max (4 members) | a group whose members register under one front document (the server of services), named after the grouper |

The bounds come from the system invariants in `SHEET.sheet`
(`congruent-linear-systems-max = 3024`, `complexity-degree-max = 4`), the same
limits the conducted `congruent()`/`degreemax()` built-ins use. A linear chain
is a line of reals up to 3024 long; a group (or services group) organizes up to
4 members in one degree. Exceeding a bound is a compile-time error.

### 2.2 Why a grouper

A bare "document A points to document B" loses the *shape* of the relationship.
The grouper is the organization term that records whether the pointed-to
documents are a **line** (ordered reals), a **set** (grouped), or a **set behind
a server** (grouped, then a server of services) — so the relationship stays
intelligible and bounded, in the spirit of the BODI™ mitigative circumference
(a composition should not lose required context).

---

## 3. Compiler integration (syntax 1.10)

- `#sleela 1.10` is the minimum version for either meaning; `maxSupportedSyntax`
  is raised to `1.10` (`impl/frontend/version.h`).
- No new lexer/parser **keywords**: class `extends` already tokenized; the
  document statement reuses the `extends` token and reads `to`/the grouper as
  identifiers. Recognition is anchored at the top-level parser dispatch.
- Class inheritance is resolved in `impl/frontend/semantic.cpp` (merge base
  fields/methods base-first, validate base, cycle-check) and lowered in
  `impl/frontend/compiler.cpp` (base-first instance layout; alias inherited
  methods). Both are guarded by `syntax >= 1.10`.
- Document extensions are parsed into `Program.documentExtensions`
  (`impl/frontend/ast.h`: `DocumentExtension` + `Grouper`) and validated in the
  semantic pass against the catalog bounds.

See `examples/extends/inheritance.sleela` and
`examples/extends/document_grouping.sleela` for runnable programs.

---

`extends` (syntax 1.10) — the original OOD base-class meaning, now resolved, plus
a document that extends *to* other documents organized by a grouper (linear
reals, a group, or a group behind a server of services).
