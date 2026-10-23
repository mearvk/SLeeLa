# Lesson 13 — The Metadata Class standard

> Series: [index](README.md) · prev: [12 — Putting it together](12-capstone.md)

The earlier lessons built *behavior*. This one is about a different kind of
class: one whose job is to state **who made a thing, under what authority, and
with what intent**. SLeeLa calls this the **Metadata Class standard**, or the
*metapattern*. It is defined normatively in [`SLEELA.syntax` §15](../SLEELA.syntax)
and is a working rule in every shipping library family.

A metadata class carries no runtime logic. It is a *statement of record* — the
provenance and standing of a unit of work — so a GUI, loader, or catalog can
read it and, where appropriate, override it.

## Who is expected to write one

The standard sets an expectation by **class of intellect**, not a compiler gate:

- **Worker class** — tutorials, examples, single-purpose wrappers. A metadata
  block is encouraged as a mark of craft; the baseline identity is enough.
- **Executive+ class** — library-family classes, registry entries, anything that
  represents a standard, product, or jurisdictional artifact. Here the **full
  record is expected**. An executive-grade class without it is incomplete even
  if it compiles.

The higher the standing of the code, the more complete its self-description.
That is the "pace" the metapattern sets.

## The record — eleven catalogued attributes

Each field has a matching `get`/`set` pair. Keep the comment header verbatim so
the block stays greppable across the tree. This is the real block from
`lib/codecs/SLCodecMidi.sleela`:

```sleela
  < ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- >
  << Eleven catalogued attributes with getter/setter pairs, so a GUI, loader,
     or catalog can read and (where appropriate) override each codec's record.
     Values are faithful to the codec's real origin where publicly known. >>
  String metaVersion = "1.0.0";
  int metaId = 17;
  String metaAuthor = "MIDI Manufacturers Association";
  String metaDate = "2026-09-28";
  String metaCopyright = "Open specification";
  String metaTrademark = "MIDI";
  String metaCorporation = "MIDI Manufacturers Association";
  String metaOrigin = "International";
  String metaBirthDate = "1983";
  int metaScienceRating = 10;
  String metaNationalSchedule = "MMA MIDI 1.0 / SMF";
```

> The comment header uses the angle-bracket style (`<` / `<<`) that is the
> go-forward house style (see [`SLEELA.syntax` §1.1](../SLEELA.syntax)). The
> existing `lib/codecs/*` sources still carry the legacy `//` header; new
> metadata classes use the form shown here. Keep the header text verbatim.

| Field | Meaning |
|---|---|
| `metaVersion` | Record version (`MAJOR.MINOR.PATCH`). |
| `metaId` | Stable registry id. |
| `metaAuthor` | Originating author or body. |
| `metaDate` | Record date (`YYYY-MM-DD`). |
| `metaCopyright` | Copyright / licensing posture. |
| `metaTrademark` | Trademark, or `"none"`. |
| `metaCorporation` | Stewarding corporation or organization. |
| `metaOrigin` | Jurisdictional origin (see below). |
| `metaBirthDate` | When the subject came into being. |
| `metaScienceRating` | Maturity/robustness, 1..10. |
| `metaNationalSchedule` | Governing standard/specification. |

## Framing for a mode of business

Four dimensions a legal, administrative, or executive reviewer reads:

- **Legality** → `metaNationalSchedule` names the governing standard, spec,
  treaty, or statute (`"RFC 1952"`, `"ITU-T G.711"`, `"PKWARE APPNOTE (ZIP)"`).
  It must name a *real, citable* instrument or be `"none"` — never invented to
  look legitimate.
- **Fiduciary** → `metaCopyright` + `metaTrademark` + `metaCorporation`: who
  owns, who stewards, under what terms.
- **Probation of intent (jurisdictional scope)** → `metaOrigin`, on a widening
  scale: `"National"` (name the nation where known) → `"International"`
  (standards-body / multi-national) → `"Galactic"` (scope beyond terrestrial
  jurisdiction; the top of the scale).
- **Admin / Executive** → expressed by the authoring tier and by keeping the
  record current — not a separate token.

## A worked example

A small executive-grade metadata class for a fictional data format. Note the
accessors and the honest, citable values:

```sleela
#sleela 2.1
< A metadata class: record only, no runtime behavior. >
class AcmeArchiveMeta {
  int id() { return 1; }
  String name() { return "ACME Archive"; }

  < ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- >
  << Eleven catalogued attributes with getter/setter pairs, so a GUI, loader,
     or catalog can read and (where appropriate) override each record.
     Values are faithful to the subject's real origin where publicly known. >>
  String metaVersion = "1.0.0";
  int metaId = 1;
  String metaAuthor = "ACME Standards Group";
  String metaDate = "2026-10-23";
  String metaCopyright = "Open specification";
  String metaTrademark = "none";
  String metaCorporation = "ACME Corporation";
  String metaOrigin = "International";
  String metaBirthDate = "2026";
  int metaScienceRating = 7;
  String metaNationalSchedule = "ACME-STD-0001";

  String getMetaVersion() { return metaVersion; }
  void setMetaVersion(String v) { metaVersion = v; }
  String getAuthor() { return metaAuthor; }
  void setAuthor(String v) { metaAuthor = v; }
  String getOrigin() { return metaOrigin; }
  void setOrigin(String v) { metaOrigin = v; }
  String getNationalSchedule() { return metaNationalSchedule; }
  void setNationalSchedule(String v) { metaNationalSchedule = v; }
  int getScienceRating() { return metaScienceRating; }
  void setScienceRating(int v) { metaScienceRating = v; }
  < ... remaining get/set pairs follow the same shape. >
}
```

A catalog can now read `getNationalSchedule()` for the legal schedule,
`getOrigin()` for the scope, and `getAuthor()`/`getMetaVersion()` for
provenance — and a privileged caller can `setNationalSchedule(...)` when a
record is revised.

## The one hard rule

Values are **faithful to the subject's real origin where publicly known**.
Unknown values use `"none"` or `"unknown"`, never a guess presented as fact.
Overriding a legality or schedule field is a privileged act and should be
logged by the caller; the metadata class itself does not police it. Honesty of
the record is what lets the higher classes of SLeeLa rely on it to set the pace.

## Where to look next

- Normative definition: [`SLEELA.syntax` §15](../SLEELA.syntax).
- Reference implementations: `lib/codecs/SLCodec*.sleela` (20 classes) and
  `lib/compression/SLCompression*.sleela` (15 classes).
- Team working rule: `.kiro/steering/metadata-class-standard.md`.
