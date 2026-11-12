# /lib/java/jdk-28: Java SE / JDK 28 as SLeeLa, one file per Java type

`/lib/java/jdk-28` holds one SLeeLa file for every public type in the
[Java SE 28 & JDK 28 API](https://download.java.net/java/early_access/jdk28/docs/api/index.html)
(early access). The rule is **1:1**: one Java type, one SLeeLa file.

Each file names its Java Source, which is the JDK 28 API page for that type.
The SLeeLa functions in the file are produced from that Source. No Java source
text is copied or expanded into the repository.

## Layout: the same folders as the online API

Every module of the online JDK 28 API has a folder directly under
`lib/java/jdk-28/`, and every package has its folders under its module:

| Online JDK 28 API (`.../jdk28/docs/api/`) | SLeeLa file |
|---|---|
| `java.base/java/util/ArrayList.html` | `lib/java/jdk-28/java.base/java/util/ArrayList.sleela` |
| `java.base/java/util/Map.Entry.html` | `lib/java/jdk-28/java.base/java/util/Map.Entry.sleela` |
| `java.sql/java/sql/Connection.html` | `lib/java/jdk-28/java.sql/java/sql/Connection.sleela` |
| `jdk.zipfs/module-summary.html` | `lib/java/jdk-28/jdk.zipfs/module-summary.md` |

- **58 module folders:**
  - 42 modules hold types: 370 folders including their package folders.
  - 16 modules export no packages, for example `java.se`, `jdk.jlink` and
    `jdk.zipfs`. Each of those folders holds a `module-summary.md` naming its
    online page and the evidence that the module is documented.
- **386 folders in all,** none empty.
- **Nested types:** a nested type keeps its documented name (`Map.Entry`).

The index files:
- [`INDEX.tsv`](INDEX.tsv) lists every Java type with its module, kind,
  SLeeLa file and Java Source URL.
- [`MODULES.tsv`](MODULES.tsv) lists the 58 modules.
- [`FOLDERS.txt`](FOLDERS.txt) lists every folder.

**Loader rule.** The `/lib` loader finds a class by file name, so
`impl/frontend/driver.cpp` treats `java/jdk-28/` as a reference tier, the same
as `docs/api/`. When a real library unit of that name exists, for example
`lib/java/java/util/ArrayList.sleela`, the real unit wins. These files never
make an existing program ambiguous. `working-binaries/sleela` was rebuilt with
this rule.

**Not created:**
- `jdk.crypto.ec`: deprecated for removal since JDK 22, and its JDK 28
  status is unconfirmed.
- `jdk.unsupported`, `jdk.unsupported.desktop` and `jdk.jsobject`: these
  have no types in the JDK 28 API index.

## What each file contains

```
<< lib/java/jdk-28/java.base/java/util/ArrayList.sleela
   SLeeLa file for the Java type java.util.ArrayList (module java.base). One Java type, one SLeeLa file.
   Java Source (JDK 28 API): https://download.java.net/java/early_access/jdk28/docs/api/java.base/java/util/ArrayList.html
   Java kind: class; extends java.util.AbstractList; implements java.util.List, ...
   Status: members from the JDK 28 API (exact signatures). >>
#sleela 3.0
material;

class ArrayList<E> {
  ... Metadata Class metapattern (11 attributes) ...
  String javaType = "java.util.ArrayList";
  String javaSource = "https://download.java.net/java/early_access/jdk28/docs/api/java.base/java/util/ArrayList.html";
  ... java declarations: constants with real values, fields, complete enums and records ...
  ... java constructors and java signatures: every public/protected member, exact Java signature ...
}
```

A member whose SLeeLa logic is not written yet throws `AbstractMethodError`
(abstract) or `UnsupportedOperationException` naming the Java signature.
Writing the logic means replacing that body with SLeeLa code that does what
the Java Source specifies.

## Inventory (generated 2026-11-12)

| Measure | Count |
|---|---:|
| JDK 28 public types | **4,721** |
| SLeeLa files in `/lib/java/jdk-28` | **4,721** (1:1) |
| Module folders | 58 (42 with types, 16 without packages) |
| Folders in all | 386 |
| Kinds | 2,856 classes, 1,590 interfaces, 157 enums, 50 annotations, 3 records, 65 not yet classified |
| Files with API members written | 4,656 |
| Files with members pending | 65 |
| Constructors with exact signatures | 4,496 |
| Methods with exact signatures | 36,536 |
| Members with working SLeeLa logic | 0 (enum and record bodies aside); this is the next step |

### Largest modules

| Module | Types | Members pending |
|---|---:|---:|
| java.base | 1,638 | 12 |
| java.desktop | 1,413 | 7 |
| java.xml | 194 | 0 |
| java.management | 190 | 0 |
| jdk.compiler | 147 | 1 |
| java.compiler | 146 | 0 |
| jdk.jdi | 131 | 0 |
| java.naming | 102 | 0 |
| jdk.xml.dom | 89 | 0 |
| java.sql | 74 | 0 |
| jdk.incubator.vector | 23 | 23 |
| jdk.incubator.json | 10 | 10 |

## Where the data comes from

- **Type list (JDK 28):** the JDK 28 API index, as mirrored in
  `lib/java/early_access/jdk28/docs/api/`. That mirror has the JDK 28 changes,
  for example `java.lang.LazyConstant` and `jdk.incubator.json`, and the
  removal of `java.applet`.
- **Members:** the reflected public API in `lib/java/api/jdk-api.jsonl` and
  `lib/java/api/jdk-signatures.jsonl`. These were reflected from OpenJDK
  25.0.2, because the sandbox that generated this tree could not download a
  JDK 28 build or reach the online pages (HTTP 403). Only types that are in
  JDK 28 are written. A member that JDK 28 added to an existing type is not
  listed yet; check against the Source page.
- **Pending (65 types):** types new in JDK 28, plus the incubator modules,
  which the reflection did not cover. Their files carry the identity and the
  Source link, and the marker `members pending`.

## Tools (run from the repository root)

```sh
python3 lib/java/jdk-28/tools/generate_jdk28.py   # (re)write every file, INDEX.tsv and MODULES.tsv
python3 lib/java/jdk-28/tools/validate_jdk28.py   # enforce the 1:1 rule and the Source links
bash    lib/java/jdk-28/tools/check_jdk28.sh 8    # compile-check every file; report in CHECK-REPORT.txt
```

`generate_jdk28.py` rewrites generated files only. A file that contains a
`< ---- BEGIN java implementation ---- >` block holds hand-written SLeeLa logic
and is never overwritten.

## Licensing

The Java API is by Oracle and the OpenJDK community, under GPLv2 with the
Classpath Exception. Each file records that in its metadata
(`metaCopyright`, `metaCorporation`). Java is a trademark of Oracle.
