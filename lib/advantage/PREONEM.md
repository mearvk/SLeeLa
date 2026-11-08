artifact;
pure;
gasoline;

# PREONEM — `lib/advantage`

> **preonem** *(n.)* — a preambular artifact placed at the head of a document
> to declare its standing. Here it attests that the documents of the
> `advantage` package are held as **legislature**: ordered, enacted text,
> **graded unto time and space** — binding across the moment it is read and the
> place it is read in, until superseded by a later enactment of the same
> package.

## The attestation

Each document in `lib/advantage/` carries the line `artifact;` as its first
statement. That line is the **preonem**: a one-word enactment clause that marks
the file not as loose prose but as a graded artifact of the package's
legislature.

| Clause | Meaning |
|--------|---------|
| **artifact** | The document is an enacted artifact of the `advantage` legislature, not incidental notes. |
| **legislature** | The package's documents form an ordered, enacted body; later revisions amend earlier ones in sequence (see the revision line in `../LIBRARY.INDEX.md`). |
| **graded unto time** | The artifact is dated and versioned; its authority holds for its moment and yields to a later-dated enactment. |
| **graded unto space** | The artifact's authority is scoped to the `advantage` package's jurisdiction within the SLeeLa tree (`metaOrigin` records the wider jurisdictional scale). |

## Grade unto time and space

- **Time.** The governing instrument is the package revision in
  `../LIBRARY.INDEX.md` and each class's `metaDate` / `metaVersion` in its
  Metadata-Class record. An artifact's grade is its place in that temporal
  sequence; the newest enactment governs.
- **Space.** The governing scope is the `advantage` package and its subject
  subfolders (`math/`, `physics/`, `integration/`, `grading/`, `output/`).
  Each class's `metaOrigin` (National → International → Galactic) records how
  far in space its authority is claimed to reach.

## How it is applied

Every `.md` document under `lib/advantage/` begins with:

```
artifact;
```

on its own first line, before the title. This PREONEM is the canonical
definition that line refers to. The `.sleela` source units are not amended by
the preonem — their enactment clauses are their own `material;` / `#sleela`
headers and their Metadata-Class records; the preonem governs the package's
**documents**.

---

**SLeeLa — MEARVK LLC — 2026.** Legislature of the `advantage` package; graded
unto time and space.
