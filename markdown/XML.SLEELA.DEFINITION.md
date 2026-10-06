# XML Definition for SLeeLa Classes and SST / Nordshrift Sheets

This document defines an XML interchange form that is **information-equivalent**
to the two authored SLeeLa artifact types:

1. a **SLeeLa source unit** — a `.sleela` ("Wrapper") file containing one class;
2. a **Nordshrift Scripting Sheet** — a `.sst` file, in either the 1.0
   transpilation form or the 2.0 semantic form.

The XML form exists so that a `.sleela` class or a `.sst` sheet can be
serialized, exchanged, diffed, and validated as structured XML, and then
re-emitted to its native text form with the same meaning.

All artifacts live in the repository `/xml-moment` folder:

| File | Purpose |
|---|---|
| `xml-moment/sleela-class.dtd` | DTD for a SLeeLa source unit (class). |
| `xml-moment/sleela-class.example.xml` | Worked example (`lib/autocad/SLCadModel.sleela`). |
| `xml-moment/sst-nordshrift.dtd` | DTD for a Nordshrift `.sst` sheet (1.0 + 2.0). |
| `xml-moment/sst-nordshrift-1.0.example.xml` | Worked 1.0 transpilation sheet. |
| `xml-moment/sst-nordshrift-2.0.example.xml` | Worked 2.0 semantic sheet. |

Each example carries a `<!DOCTYPE ... SYSTEM "...dtd">` declaration and is
validated with `xmllint --valid`.

---

## 1. Equivalence contract

A conforming XML document and its native `.sleela` / `.sst` text are mutually
derivable. Concretely:

- **Names mirror keywords.** XML element and attribute names are the directive
  and section keywords of the native format (`sheet`, `source`, `pipeline`,
  `subject`, `quantity`, `relation`, `class`, `field`, `method`, …).
- **Order is preserved where it is significant.** SLeeLa class members keep
  their declared order; `.sst` sections keep their authored order.
- **Bodies are verbatim.** A SLeeLa method body is carried as text inside a
  `<body>` element (CDATA), so emission reproduces the original statements
  byte-for-byte. The XML form does **not** re-parse statement syntax; it is a
  structural envelope around the authored body.
- **Comments round-trip.** Line (`//`), block (`/* */`), and doc (`///`)
  comments are preserved as `<comment kind="...">` elements.

The XML form is an **envelope**, not a second compiler front-end. The
authoritative grammar remains `SST.model` (1.0), `SST-2.0.model` (2.0), and the
SLeeLa language metadocument.

---

## 2. SLeeLa source unit — `sleela-class.dtd`

A `.sleela` file is: a `#sleela <version>` pragma, zero or more `import`
statements, interleaved comments, and exactly one top-level `class` with
fields and methods.

### 2.1 Element map

| XML element | Native construct |
|---|---|
| `<sleela-unit>` | the whole `.sleela` file (root) |
| `<pragma version="1.3"/>` | `#sleela 1.3` |
| `<import package="math"/>` | `import math;` |
| `<comment kind="line\|block\|doc">` | `//`, `/* */`, `///` |
| `<class name="..." kind="class\|facade">` | `class Name { ... }` |
| `<field name="..." type="..." initial="..."/>` | `Type name;` (optional initializer) |
| `<method name="..." returns="...">` | `RetType name(params) { ... }` |
| `<param name="..." type="..."/>` | one method parameter |
| `<body><![CDATA[ ... ]]></body>` | the verbatim method body |

### 2.2 Notes

- `kind="facade"` marks a `SLPackage.sleela`-style library front-end class.
- The surface type vocabulary is scalar (`String`, `double`, `int`, `boolean`,
  `long`, `char`, `void`); the DTD keeps `type` as free text (`CDATA`) so the
  full SLeeLa type surface and library types are expressible.
- Members and comments may interleave in any order inside `<class>`, matching
  how class bodies are authored.

### 2.3 Example (abridged)

```xml
<sleela-unit path="lib/autocad/SLCadModel.sleela" package="autocad">
  <pragma version="1.3"/>
  <import package="math"/>
  <class name="SLCadModel" kind="class">
    <field name="title" type="String"/>
    <method name="distance" returns="double">
      <param name="x1" type="double"/>
      <param name="y1" type="double"/>
      <param name="x2" type="double"/>
      <param name="y2" type="double"/>
      <body><![CDATA[ return math.hypot(x2 - x1, y2 - y1); ]]></body>
    </method>
  </class>
</sleela-unit>
```

See `xml-moment/sleela-class.example.xml` for the full document.

---

## 3. Nordshrift sheet — `sst-nordshrift.dtd`

One DTD covers both sheet dialects, selected by the `#nordshrift` pragma value.

### 3.1 Common prologue

| XML | Native |
|---|---|
| `<pragma name="nordshrift" value="1.0\|2.0"/>` | `#nordshrift 1.0` / `#nordshrift 2.0` |
| `<pragma name="sleela" value="..."/>` | `#sleela ...` |
| `<sheet name="..." version="..." author="...">` | `sheet <name>:` block |
| `<description>` / `<tag>` | sheet `description` / `tags` |
| `<import path="..." alias="...">` with `<only>` / `<except>` | `import "..." as x [only\|except [...]]` |

### 3.2 1.0 transpilation sections

| XML element | `.sst` section |
|---|---|
| `<source>` with `<glob>` / `<exclude>` | `source:` |
| `<target .../>` | `target:` |
| `<pipeline>` with `<phases>` / `<skip>` | `pipeline:` |
| `<rules>` with `<activate>` / `<deactivate>` / `<severity>` | `rules:` |
| `<effects>` with `<declare>` / `<aliases>` | `effects:` |
| `<derive .../>` | `derive:` |
| `<guards .../>` | `guards:` |
| `<interop>` with `<type-mapping>` / `<package-allow>` / `<package-deny>` | `interop:` |
| `<profile name="..." inherits="...">` | `profile <name>:` |

Enumerated attribute values are constrained by the DTD to the vocabularies in
`SST.model` (e.g. `layout` ∈ {`mirror-source`, `flat`, `package-mapped`,
`custom`}; phase `name` ∈ the eleven pipeline phases; severity `level` ∈
{`error`, `warning`, `notice`}).

### 3.3 2.0 semantic sections

| XML element | `.sst` 2.0 block |
|---|---|
| `<subject name="..." domain="...">` with `<depends>` | `subject <name>:` |
| `<quantity .../>` | `quantity <name>:` |
| `<assumption>` with `<statement>` | `assumption <name>:` |
| `<relation>` with `<formula>` / `<inputs>` / `<outputs>` | `relation <name>:` |
| `<transformation .../>` | `transformations:` entry |
| `<comparison .../>` | `comparison:` |
| `<evidence>` with `<note>` | `evidence:` |
| `<workplan>` with `<todo>` → `<action>` / `<validation>` | `workplan:` |

The `status` vocabulary follows the 2.0 Evidence model (`observed`,
`specified`, `derived`, `modeled`, `inferred`, `assumed`); TODO `status`
follows the WorkPlan model (`planned`, `ready`, `active`, `blocked`,
`validating`, `complete`, `deferred`). Semantic children of a `<subject>` may
appear in any order, as in the native block.

### 3.4 Examples

- `xml-moment/sst-nordshrift-1.0.example.xml` — the commerce-engine
  transpilation sheet (from `SST.model` Part XVI).
- `xml-moment/sst-nordshrift-2.0.example.xml` — the `subject-libraries` sheet
  (from `examples/nordshrift/subject-libraries.sst`).

---

## 4. Validation

```sh
xmllint --noout --valid xml-moment/sleela-class.example.xml
xmllint --noout --valid xml-moment/sst-nordshrift-1.0.example.xml
xmllint --noout --valid xml-moment/sst-nordshrift-2.0.example.xml
```

All three example documents validate against their DTDs.

---

## 5. Relationship to existing specifications

- `SST.model` — Nordshrift `.sst` 1.0 format reference (grammar, sections,
  diagnostics). The 1.0 XML elements mirror its sections one-to-one.
- `SST-2.0.model` — the 2.0 semantic coordination layer. The 2.0 XML elements
  mirror its subject/quantity/relation/evidence/workplan models.
- SLeeLa standard library under `/lib` — the SLeeLa class XML form mirrors a
  `.sleela` source unit as discovered by the compiler and Nordshrift loader.

The XML form adds no new semantics; it only provides a validated, structured
serialization of what these specifications already define.
