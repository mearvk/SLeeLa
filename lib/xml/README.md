<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa XML (DOM)

**Status: SLeeLa-side XML Document Object Model.** This directory is the
language-facing XML layer: a DOM tree (`SLXmlDocument` → `SLXmlNode` →
`SLXmlAttribute`) with the familiar Node/Element/Document operations, a
text-to-DOM reader (`SLXmlParser`), a DOM-to-text writer (`SLXmlSerializer`),
and a conformance register (`SLXmlAspect`) that rates the **quality of each XML
aspect** this library addresses.

It is governed by the real instruments it names: XML 1.0
([W3C REC-xml, Fifth Edition](https://www.w3.org/TR/xml/)), the DOM Node/Element
interface, and Namespaces in XML 1.0. Values in the metadata records are
faithful to those origins. Content was rephrased for compliance with licensing
restrictions.

## The DOM shape

```
SLXmlDocument            the document: declaration + the single root element,
   │                     the node factory, and document-level queries
   ├─ root : SLXmlNode   the document element
   │     ├─ children : SLXmlNode[]      ordered child nodes (DOM Node)
   │     └─ attrs    : SLXmlAttribute[] named attributes (DOM Element)
   └─ writer : SLXmlSerializer          DOM → well-formed XML text
```

| File | Role |
| --- | --- |
| `SLPackage.sleela` | Package facade (`configure`/`name`/`available`). |
| `SLXmlNodeType.sleela` | The DOM `nodeType` vocabulary (element/text/cdata/PI/comment/document) and the predicates a walker branches on. |
| `SLXmlAttribute.sleela` | One `name="value"` attribute, with namespace prefix/local resolution over a char-cell view. |
| `SLXmlNode.sleela` | The central DOM node: children, attributes, `textContent`, `getElementsByTagName`, depth/counts. |
| `SLXmlDocument.sleela` | The DOM `Document`: node factory (`createElement`/`createTextNode`/…), root management, `getElementById`, `getElementsByTagName`, serialization. |
| `SLXmlSerializer.sleela` | DOM → text, compact or pretty, with XML 1.0 §2.4 escaping. |
| `SLXmlParser.sleela` | Text (as char cells) → DOM for the common subset, plus a structural well-formedness check. |
| `SLXmlAspect.sleela` | The aspect/quality register: each aspect, its governing item, and an honest 0..10 quality. |
| `xml-dom.sleela` | The runnable reference (`class XmlDom` with `main`). |

## Using it (syntax 2.5)

A caller drives **one document object** end to end:

```java
SLXmlDocument doc = new SLXmlDocument();
doc.create();

SLXmlNode catalog = doc.newRoot("catalog");           // the document element
SLXmlNode item = doc.createElement("item");
item.setAttribute("id", "a1");
item.setAttribute("origin", "United States");         // an Item's origin
item.addElement("name").addText("Liberty Bell");
catalog.appendChild(item);

print(doc.toPrettyXml());                              // declaration + tree

SLXmlNode items = doc.getElementsByTagName("item");    // a NodeList-like node
print(items.childCount());                             // 1
SLXmlNode byId = doc.getElementById("a1");             // DOM-style lookup
print(byId.textContent());                             // "Liberty Bell"
```

Run the executable reference:

```sh
# from the repository root, with the SHA-256 manifest selected
SLEELA_SHA256_MANIFEST="$(pwd)/security/sha256-manifest.json" \
  ./impl/build/sleela run lib/xml/xml-dom.sleela
```

## Character data, escaping, and the opaque String

SLeeLa's built-in `String` is **opaque**: it supports `+` and `==` but no
in-language slicing (this is the same contract `lib/text/SLString` documents).
Real character-level work is therefore done over an explicit **char-cell
`String[]`** — one glyph per cell — the sanctioned mechanism across the standard
library.

Consequences for XML, handled honestly rather than faked:

- **Serialization** of content containing `&`, `<`, `>`, or `"` needs a char-cell
  view so the writer can escape it (`&amp;`, `&lt;`, `&gt;`, `&quot;`). Attach it
  with `node.addTextCells(value, cells)` / `node.setAttributeCells(name, value,
  cells)`. Content known free of those characters uses plain `addText` /
  `setAttribute` and serializes verbatim.
- **Parsing** reads a char-cell view: `parser.parseCells(cells)` and
  `parser.isWellFormedCells(cells)`. A caller supplies the cells at the boundary
  (from a known decomposition or the runtime's bridged `chars()`).

The `xml-dom.sleela` demo shows both: it escapes `"Jack & Jill <xml>"` on output
and parses `<doc><p id="x1">hi</p></doc>` back into a DOM from cells.

## Node kinds (DOM `nodeType`)

Mirror the W3C DOM constants (`SLXmlNodeType`):

| Code | Kind | Carries |
| --- | --- | --- |
| `1` | element | name, attributes, children |
| `3` | text | character data |
| `4` | cdata | a literal `<![CDATA[…]]>` section |
| `7` | processing-instruction | `<?target data?>` |
| `8` | comment | `<!-- … -->` |
| `9` | document | the single root element |

## The aspect / quality register

`SLXmlAspect` catalogues the aspects an XML processor is judged by and reports an
**honest** 0..10 quality for how fully *this* library addresses each — an
unimplemented aspect scores low, never an inflated value.

| Aspect | Governing item | Quality (this library) |
| --- | --- | --- |
| well-formedness | W3C REC-xml §2 | strong (8/10) |
| DOM tree model | W3C DOM Level 1 | excellent (9/10) |
| namespace-awareness | Namespaces in XML 1.0 | strong (7/10) |
| serialization | W3C REC-xml §2.4 | excellent (9/10) |
| validity (DTD/XSD) | W3C REC-xml §2 / XSD | minimal (1/10) — not yet implemented |

```java
SLXmlAspect a = new SLXmlAspect();
print(a.nameOf(1) + " " + a.qualityOf(1) + "/10 " + a.bandOf(a.qualityOf(1)));
print("overall quality x10: " + a.overallQualityX10());   // 68 = 6.8 / 10
```

## Implementation boundary

This layer models, builds, queries, serializes, and reparses XML in the common
subset. It does **not** implement DTD/XSD validity, general (non-predefined)
entity expansion, or namespace binding resolution beyond prefix/local splitting;
those aspects report their honest standing through `SLXmlAspect` rather than
pretending to a capability the code does not have — the same "never fake a
result" contract the `lib/codecs` and `lib/compression` families carry.

## Metadata Class metapattern

Every class here carries the descriptive-metadata block required across SLeeLa
(see `.kiro/steering/metadata-class-standard.md`): eleven catalogued attributes
with get/set pairs, under the verbatim greppable header. `SLXmlAspect` is an
executive-grade class and carries the full record (identity + legality via
`metaNationalSchedule` + fiduciary + jurisdictional `metaOrigin`). The XML
standards are stewarded by the W3C, an international consortium (host
institutions include MIT in the United States, ERCIM in Europe, Keio in Japan,
and Beihang in China), so `metaOrigin` is `International`.
