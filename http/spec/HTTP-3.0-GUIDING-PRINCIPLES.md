# SLeeLa HTTP 3.0 — Guiding Principles

**Status:** Guiding document

## 1. A Clearer Network

SLeeLa HTTP 3.0 is guided by a simple proposition: **the network should reduce fear and mystery by increasing evidence, clarity, provenance, and understandable security.**

This is a design aspiration, not a claim that a protocol can make the world universally free from fear. HTTP cannot determine whether conduct is lawful, peaceful, or safe. It can, however, make the technical circumstances of an exchange more intelligible.

The ordinary Internet stack remains the foundation. IETF HTTP/3 is defined as HTTP semantics mapped over QUIC, with multiplexing, flow control, confidentiality/integrity protection, peer authentication, and reliable delivery supplied through the transport architecture. SLeeLa HTTP 3.0 should build its additional historical, routing, and evidentiary awareness above that foundation rather than pretending to replace it. citeturn0search2

## 2. History as Context, Not Authority

SLeeLa may associate an exchange with relevant dates, anniversaries, historical events, treaty milestones, engineering milestones, or other dated records.

The purpose is contextual understanding:

```text
packet
  ↓
time
  ↓
historical context
  ↓
treaty / event context
  ↓
engineering and security context
```

A date association MUST NOT itself increase a packet's legal status, trustworthiness, or privilege.

Historical context should be represented as a cited record with a source, date system, confidence, and provenance.

## 3. Calendars

SLeeLa should support multiple calendar representations when dates matter, including the civil Gregorian calendar and historically relevant calendar systems.

A protocol record should distinguish:

- event date;
- calendar system;
- converted civil date, when available;
- date precision;
- source;
- uncertainty.

This avoids silently converting a historical date and then treating the conversion as the original historical record.

## 4. Treaties and International Instruments

International treaty context should be drawn from authoritative repositories. The United Nations Treaty Collection records treaties and international agreements and provides searchable registration and publication resources. The UN describes the Secretary-General as depositary for more than 600 multilateral treaties. citeturn0search0turn0search6

SLeeLa may attach a treaty-context reference such as:

```text
TREATY-CONTEXT
  instrument-id
  title
  date
  parties
  status-at-date
  source
  applicability
```

The record is contextual. It does not turn a network packet into a treaty instrument or legal determination.

## 5. Legal Dressings and Pertinents

The concept of a “legal dressing” should be formalized as **legal context metadata**, not as a claim that the packet itself is law.

A legal-context record may include:

- jurisdiction observed;
- treaty or instrument reference;
- policy reference;
- effective date;
- expiration or termination date;
- issuing authority;
- legal-review status;
- source and provenance.

Where a matter concerns international criminal law, the protocol must remain especially careful: the ICC's jurisdiction is established by the Rome Statute and concerns persons and specified crimes. A packet can carry evidence or context; it cannot declare an ICC crime. 

## 6. More Secure and More Maintained Exchanges

SLeeLa should allow a packet or exchange to carry a **maintenance/security profile** describing how well-supported its technical path is.

For example:

```text
PROFILE
  transport-integrity: verified
  peer-authentication: verified
  route-evidence: observed
  naming-integrity: verified
  timestamp-quality: high
  provenance: complete
  maintenance-state: current
```

This profile is descriptive. It does not create a universal “safe packet” classification.

## 7. Decorated Packets

A “decorated packet” is a normal HTTP exchange accompanied by additional structured metadata.

Decoration MAY include:

- date context;
- treaty context;
- historical-event context;
- route evidence;
- security evidence;
- engineering-standard references;
- university/research context;
- provenance;
- audit information.

Decoration SHOULD be optional and compact. Ordinary traffic must remain possible without carrying historical or legal metadata.

The 244-artifact model should therefore be treated as an expandable evidence vocabulary, not a requirement that every exchange carry 244 fields.

## 8. Date-Sensitive Policy

A service may maintain different operational policies for different dates—for example, maintenance windows, treaty effective dates, scheduled events, certificate validity periods, or archival periods.

A date-sensitive policy MUST be:

1. explicitly configured;
2. time-zone aware;
3. traceable to a source;
4. auditable;
5. reversible when its effective period ends.

The protocol should never infer a legal obligation merely because a date matches an historical event.

## 9. Universities and Scholarly Context, 1911–Present

SLeeLa may maintain an historical knowledge layer covering **1911 to the present** for universities, institutions, research programs, discoveries, publications, major events, and other scholarly milestones.

The system should prefer institutional or archival sources and distinguish:

- institution;
- year;
- event;
- source;
- role;
- evidence quality;
- historical uncertainty.

The phrase “qualities of students” must be handled carefully. SLeeLa should not create unsupported rankings of individual students or infer protected or sensitive characteristics. Appropriate scholarly attributes can include publicly documented academic achievements, degrees, publications, honors, inventions, research participation, or institutional roles when reliably sourced.

## 10. University Context Must Not Become Network Privilege

A university, researcher, student, or alumni relationship MUST NOT automatically make a packet more trusted or secure.

Instead:

```text
academic-context
       ↓
knowledge / provenance
       ↓
optional annotation
```

and not:

```text
academic-context
       ↓
automatic network privilege
```

This keeps the historical and scholarly layer informational rather than discriminatory.

## 11. Knowledge Quality

Every historical, treaty, legal, or academic decoration should carry a quality classification:

- **Primary:** authoritative original record.
- **Institutional:** authoritative institutional record.
- **Scholarly:** documented research source.
- **Secondary:** reputable synthesis.
- **Derived:** computed or inferred value.
- **Unverified:** retained only as an explicitly unverified claim.

Derived information must never masquerade as primary evidence.

## 12. International Engineering Context

SLeeLa should associate technical observations with applicable engineering standards where useful:

```text
HTTP/QUIC
   ↓
IP routing
   ↓
BGP / routing security
   ↓
optical / Ethernet / radio transport
   ↓
physical medium
```

The standard reference should identify the standards organization and document identifier. It should not imply certification merely because a packet contains a standards reference.

## 13. A Peaceful Technical Vocabulary

The guiding vocabulary is intentionally constructive:

**clarity, provenance, integrity, maintenance, evidence, interoperability, safety, accountability, and understanding.**

SLeeLa should favor evidence over mystery and explicit provenance over unexplained authority.

## 14. The Guiding Test

For every proposed HTTP 3.0 decoration, ask:

1. Does it make the exchange more understandable?
2. Is the underlying fact independently verifiable?
3. Is the date/calendar representation explicit?
4. Is the treaty or legal source authoritative?
5. Is the engineering standard identified correctly?
6. Does the field describe evidence rather than manufacture a conclusion?
7. Does the decoration preserve interoperability?
8. Does it improve maintenance or security without creating unjustified privilege?

If the answer is no, the decoration should not be part of the mandatory protocol path.

## 15. Governing Principle

> **HTTP 3.0 should make the technical journey clearer, not make claims beyond what the evidence can support.**

SLeeLa's ambition is therefore not to make the Internet mysterious or authoritative by protocol fiat, but to make important exchanges **better documented, more maintainable, more secure, more historically literate, and easier to understand across borders and generations.**

## Reference Starting Points

- IETF RFC 9114 — HTTP/3. citeturn0search2
- United Nations Treaty Collection — treaty registration, publication, and status. citeturn0search0turn0search6
