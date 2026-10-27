# Where Virus Definitions Come From — and How Sleelavirin™ Could Reasonably Build Its Own

**Author:** Max Rupplin — MEARVK LLC — 2026
**Companion to:** `CLAMAV-REFERENCE-ANALYSIS.md`, `SCANNING-CONCEPTS-GLOSSARY.md`
**Purpose:** A careful, complete account of (1) where ClamAV's virus list and
signature definitions actually come from, how they are packaged, delivered, and
trusted; and (2) the realistic, lawful options for Sleelavirin to obtain or
produce its own definitions.

> **Reference-only + legal note.** This describes ClamAV's *mechanism* from its
> source and official docs; it vendors no ClamAV code and copies no signature
> data. ClamAV's engine is GPLv2 and its official signature set is maintained by
> Cisco Talos under its own terms — **always confirm redistribution rights before
> bundling anyone else's definitions.** Sources are linked inline; content was
> rephrased for compliance with licensing restrictions.

---

## Table of contents

- [Part 1 — The short answer](#short)
- [Part 2 — Where ClamAV's definitions come from](#clamav)
  - [2.1 Who produces them](#who)
  - [2.2 The three core databases](#three)
  - [2.3 The CVD container format](#cvd)
  - [2.4 How trust is established (signing)](#trust)
  - [2.5 How updates are delivered (freshclam + CDIFF)](#delivery)
  - [2.6 The signature *formats* inside a CVD](#formats)
  - [2.7 Other public/free signature sources](#third-party)
- [Part 3 — What Sleelavirin can and cannot reuse today](#reuse)
- [Part 4 — How we might reasonably build our own definitions](#own)
  - [4.1 Guiding principles](#principles)
  - [4.2 Tier 1 — First-party hash definitions (recommended first)](#tier1)
  - [4.3 Tier 2 — Curated import from permitted feeds](#tier2)
  - [4.4 Tier 3 — Native content signatures (if we add AC/BM)](#tier3)
  - [4.5 Packaging: a Sleelavirin definition container](#packaging)
  - [4.6 Signing & trust for our container](#ownsign)
  - [4.7 Distribution & update model](#owndist)
  - [4.8 Provenance, legality, and false-positive discipline](#governance)
- [Part 5 — A concrete, staged roadmap](#roadmap)
- [Part 6 — Sources](#sources)

---

<a name="short"></a>
## Part 1 — The short answer

- **ClamAV's definitions** are produced by **Cisco Talos**, packaged into
  **digitally-signed `.cvd` containers** (`main`, `daily`, `bytecode`), served
  from `database.clamav.net`, and pulled by the **`freshclam`** updater, which
  applies small incremental **`.cdiff`** patches when possible and verifies a
  signature before trusting the data. (See `freshclam/`, `libfreshclam/`,
  `libclamav/cvd.c`, `libclamav/dsig.c` in the reference repo.)
- **Sleelavirin** already has the *right posture*: it keeps its own tiny native
  index and treats ClamAV as an **external provider**, importing only the narrow
  subset that is format-compatible (whole-file SHA-256) via
  `tools/sleelavirin_import_clamav.py`.
- **To build our own** definitions reasonably: start by **generating first-party
  SHA-256 hash definitions** from samples we are entitled to use, package them in
  a small signed Sleelavirin container with strict **provenance**, and only later
  (if we add a native content matcher) grow into literal byte-pattern signatures.
  Do **not** re-host or relabel ClamAV's signed set.

---

<a name="clamav"></a>
## Part 2 — Where ClamAV's definitions come from

<a name="who"></a>
### 2.1 Who produces them

The official ClamAV signature set is **actively maintained by Cisco Talos** and
downloaded with the `freshclam` tool shipped with ClamAV
([ClamAV Signatures manual](https://docs.clamav.net/manual/Signatures.html)).
Definitions originate from malware research, telemetry, partner feeds, and
community submissions; the engine is open source (GPLv2) but the curated
*signature data* is Talos's product and carries its own distribution terms.

<a name="three"></a>
### 2.2 The three core databases

ClamAV ships three primary databases (visible throughout the reference code as
`main.cvd`, `daily.cvd`, `bytecode.cvd`):

| DB | Role | Cadence |
|---|---|---|
| **main** | Large, stable base signature set | Infrequent |
| **daily** | Fast-moving recent threats + file-type magic (`daily.ftm`) | Multiple times/day |
| **bytecode** | Signed, sandboxed bytecode programs for complex/unpacker logic | As needed |

At load time the engine reads whichever of `*.cvd` (signed) or `*.cld` (unpacked/
updated) is newer per database (reference: the daily `.cvd` vs `.cld` version
comparison in `readdb.c`).

<a name="cvd"></a>
### 2.3 The CVD container format

A **CVD** (*ClamAV Virus Database*) is a single file that is **a 512-byte text
header followed by a gzip-compressed tar archive** of the actual signature files.
The header begins with the ASCII tag `ClamAV-VDB:` and is a colon-delimited record
(reference: `cl_cvdparse` in `libclamav/cvd.c`; struct `cl_cvd` in `clamav.h`):

| Field | Meaning |
|---|---|
| build time | human-readable timestamp |
| **version** | monotonically increasing DB version |
| **sigs** | number of signatures contained |
| fl | functionality / format level required |
| **md5** | MD5 of the archive body |
| **dsig** | digital signature over the header/body |
| builder | who built it |
| stime | build time as epoch seconds |

Related extensions: **`.cld`** = an *unsigned* archive produced locally after an
incremental update of a CVD; **`.cud`** = an *unsigned* custom archive
(reference: `cvd_type` enum in `cvd.h`). This is why a `.cvd`/`.cld` is **not**
"a text list of hashes" — it is a signed, compressed tarball of many
heterogeneous signature files.

<a name="trust"></a>
### 2.4 How trust is established (signing)

The CVD format is a **digitally-signed container** so third parties cannot tamper
with the signatures ([Signatures manual](https://docs.clamav.net/manual/Signatures.html)).
The engine verifies before trusting:

- **Legacy path:** an **MD5-based RSA** digital signature over the database
  (reference: `libclamav/dsig.c`, the `dsig` header field, and
  `cli_cvdunpack_and_verify`'s `disable_legacy_dsig` flag).
- **Modern path (ClamAV 1.5+):** a **FIPS-compliant** `.cvd.sign` detached
  signature verified against a shipped certificate chain
  (reference: `crtmgr.c`; and the
  [ClamAV 1.5.0 release notes](https://blog.clamav.net/2025/10/clamav-150-released.html)).
  When `.cvd.sign` files are absent, ClamAV falls back to the legacy MD5-RSA check.

The lesson: **definitions are only as trustworthy as the signature that binds
them to a known publisher.** Any home-grown system needs the same property.

<a name="delivery"></a>
### 2.5 How updates are delivered (freshclam + CDIFF)

- `freshclam` (code in `freshclam/` + `libfreshclam/`) contacts the configured
  **`DatabaseMirror`**, default **`https://database.clamav.net`** (reference:
  `freshclam/freshclam.c`), optionally resolving the latest version via a DNS TXT
  record (`libfreshclam/dns.c`).
- If a local DB already exists, freshclam fetches small **`.cdiff`** incremental
  patches and applies them on top of the current version, rather than
  re-downloading the whole multi-hundred-MB set; a corrupted DB triggers a full
  fresh download instead
  ([Updating Signature Databases](https://docs.clamav.net/manual/Usage/SignatureManagement.html),
  [ClamAV CVDs, CDIFFs blog](https://blog.clamav.net/2021/03/clamav-cvds-cdiffs-and-magic-behind.html)).
- Applying diffs produces an updated local archive (`.cld`); the result is
  re-verified before use.

So the delivery model is: **signed base container + signed incremental diffs +
verify-then-load**, all behind one updater daemon/timer.

<a name="formats"></a>
### 2.6 The signature *formats* inside a CVD

Inside the tarball are many file types, each a detection method (reference:
`cli_load*` family in `readdb.c`; [Signatures manual](https://docs.clamav.net/manual/Signatures.html)):

| Ext | Method | Maps to matcher |
|---|---|---|
| `.hdb`/`.hsb` | MD5 / SHA whole-file hash | hash matcher |
| `.mdb`/`.msb` | PE **section** hash | hash matcher |
| `.ndb` | hex **content** signatures (with wildcards, offsets, target type) | AC/BM |
| `.ldb` | **logical** signatures (boolean over subsigs) | AC + logic |
| `.cbc` | compiled **bytecode** | bytecode VM |
| `.cdb` | **container** metadata (archive name/size constraints) | container checks |
| `.fp`/`.ign2` | allow-list / false-positive suppression | allow-list |
| `.ftm` | file-type **magic** | type identifier |
| `.yar`/`.yara` | YARA rules | YARA engine |

Only the first row (plain whole-file hashes) is trivially portable to a simple
hash-based engine. Everything else assumes ClamAV's own matchers.

<a name="third-party"></a>
### 2.7 Other public/free signature sources

Beyond Talos's official set, ClamAV is designed to load **custom/third-party**
databases dropped into its database directory
([Signatures manual](https://docs.clamav.net/manual/Signatures.html)). Well-known
*free community* feeds exist (for example, long-standing third-party ClamAV
signature projects, and open rule sets such as public **YARA** collections). Each
has its **own license and quality bar** — some are permissive, some restrict
redistribution, some are research-only. Treat each feed's terms individually.

---

<a name="reuse"></a>
## Part 3 — What Sleelavirin can and cannot reuse today

From the existing design (`SIGNATURES.md`, `SCANNERS.md`,
`tools/sleelavirin_import_clamav.py`):

**Can reuse now (format-compatible):**
- Plain-text, already-extracted **whole-file SHA-256** records (`.hsb`-style:
  `"<64-hex-sha256> <size> <name>"`). The importer accepts *only* these, validates
  each line, writes an adjacent **provenance manifest** (`*.manifest.json`) with
  source version/URL/time/counts and the index digest, and stages+rolls back
  safely. This is a **supplemental subset**, never a replacement.

**Cannot reuse directly:**
- Signed `.cvd`/`.cld` containers (need the GPLv2 decoder + signature verify).
- MD5 `.hdb`, PE-section `.mdb/.msb`, `.ndb` content sigs, `.ldb` logical sigs,
  `.cbc` bytecode, `.cdb` container rules, YARA — all assume ClamAV's matchers.
- **Re-hosting the official Talos set** — that's a licensing/trust decision, not a
  format one. Don't relabel it as ours.

---

<a name="own"></a>
## Part 4 — How we might reasonably build our own definitions

<a name="principles"></a>
### 4.1 Guiding principles

1. **Own what we ship.** Prefer definitions we can *generate and attest to*
   ourselves over re-packaging someone else's set.
2. **Provenance is mandatory.** Every definition records where it came from,
   when, who built it, and under what rights (extends our existing
   `*.manifest.json` discipline and `PROVENANCE.md`).
3. **Sign it.** A definition container must be verifiable against a Sleelavirin
   publisher key — same property that makes ClamAV's CVD trustworthy.
4. **Absence is never proof of safety** (already encoded as
   `SLEELAVIRIN_UNKNOWN`).
5. **Legality first.** Confirm redistribution terms of any imported feed; never
   bundle samples or signatures we lack rights to; follow applicable malware-
   handling law and the repo's standards metapattern.
6. **Match our engine's capability.** Only ship signature *types* our matcher can
   actually evaluate (today: whole-file SHA-256 + feature-mask fuzzy).

<a name="tier1"></a>
### 4.2 Tier 1 — First-party hash definitions (recommended first)

This is the cleanest path and needs **no third-party data**:

1. **Collect samples we are entitled to use** — e.g. malware obtained under a
   research agreement, honeypot captures we own, vendor-shared samples whose terms
   permit derived hashes, or our own EICAR-style test artifacts for pipeline
   validation.
2. **Compute SHA-256** (exact) and our **64-bit feature fingerprint + mask**
   (fuzzy) per sample, using the same functions the scanner uses, so definitions
   and runtime agree by construction.
3. **Emit records** in the native index format already consumed by
   `signatures/native-sha256.txt` (one `<sha256> <size> <name/family>` per line),
   plus a parallel fuzzy record set keyed by the feature fingerprint.
4. **Write the provenance manifest** (source, rights basis, build time, counts,
   index digest) — reuse `tools/sleelavirin_manifest.py` /
   `sleelavirin_import_clamav.py` machinery.

A tiny **`sleelavirin_make_defs.py`** (new tool) would wrap steps 2–4: point it at
a directory of authorized samples → out comes a signed, manifested definition
file. *Derived hashes of samples we are licensed to analyze are first-party data.*

<a name="tier2"></a>
### 4.3 Tier 2 — Curated import from permitted feeds

For breadth, import only from feeds whose license permits redistribution of the
derived form we ship:

- Keep the current **strict whole-file-hash importer** as the only auto path.
- Add per-feed **adapter** scripts that normalize a permitted feed's records into
  our native line format **and stamp the feed's license + URL + version** into the
  manifest. Reject anything that isn't a clean hash record.
- Never silently merge feeds of differing licenses into one file; keep one index
  file (and manifest) **per source**, combined only at load time.

<a name="tier3"></a>
### 4.4 Tier 3 — Native content signatures (only if we add AC/BM)

If/when a native **Aho–Corasick / Boyer–Moore** matcher is added (see the gap
noted in `CLAMAV-REFERENCE-ANALYSIS.md` §5), we can author **literal byte-pattern**
definitions ourselves:

- Define a small, documented pattern DSL (hex bytes + a few wildcards + an
  optional target file type and offset) — a deliberate, minimal subset of what
  `.ndb` expresses, **specified by us**, not copied from ClamAV.
- Compile patterns into the native trie at load (compile-once/scan-many).
- Author patterns from samples we own; store the *pattern*, not the sample.

This is the only tier that lets us catch *mutated* variants by content rather than
exact hash — but it is also the most labor-intensive and false-positive-prone, so
it comes last and must ship with test corpora.

<a name="packaging"></a>
### 4.5 Packaging: a Sleelavirin definition container

Mirror the *good ideas* of CVD without copying it. A minimal, honest container:

```
sleelavirin-defs-<version>.svd      (our container; "SVD" = Sleelavirin Virus Defs)
  ├── HEADER              # text: magic "Sleelavirin-DEFS:", version, built, counts,
  │                       #       builder, body-sha256, signature, license tag
  ├── sha256.index        # whole-file SHA-256 records
  ├── fuzzy.index         # feature-fingerprint records
  ├── patterns.index      # (Tier 3, optional) native content patterns
  └── MANIFEST.json       # full provenance, per-source license, rights basis
```

Use an ordinary gzip+tar body (portable, no custom parser needed) with a plain
text header line first — the same two-part shape as CVD, but with our magic and
our fields. Keep it greppable and auditable.

<a name="ownsign"></a>
### 4.6 Signing & trust for our container

- Generate a **Sleelavirin publisher keypair**; ship the **public cert** with the
  scanner, keep the private key offline.
- Sign the container body; store a **detached signature** + the **body SHA-256** in
  the header (the properties CVD gets from `dsig`/`md5`).
- The loader **verifies signature + digest before loading** and refuses unsigned or
  mismatched containers (fail-closed), exactly as `cli_cvdunpack_and_verify` does.
- Prefer a modern algorithm (e.g. Ed25519 or RSA-SHA256), not MD5-RSA; ClamAV
  itself is moving to a FIPS-compliant scheme and keeping MD5 only as legacy
  fallback.

<a name="owndist"></a>
### 4.7 Distribution & update model

- Host containers as **immutable, versioned artifacts** (monotonic version like
  CVD) behind HTTPS, with a small **version/manifest endpoint** (or signed JSON) the
  updater reads first — analogous to freshclam's version check.
- Support **full download** now; add **incremental diffs** later only if size
  warrants it (CDIFF-style) — premature for a hash list of modest size.
- Reuse the existing operational scaffolding: the `systemd` unit/env in
  `antivirus/systemd/` and `tools/sleela-antivirus-ctl.py` can drive a periodic
  "fetch → verify → stage → atomically swap → keep manifest" update, the same safe
  replace pattern the importer already implements.

<a name="governance"></a>
### 4.8 Provenance, legality, and false-positive discipline

- **Provenance:** extend `PROVENANCE.md` + the per-index `MANIFEST.json` to record
  source, rights basis, builder, build time, counts, and container digest for
  every shipped definition.
- **Legality:** verify redistribution terms of any external feed; handle live
  malware under appropriate controls and law; keep the metadata-class standard and
  `POLICY.md`/`SECURITY.md` in force.
- **False positives:** keep an allow-list (analogous to ClamAV `.fp`), test every
  release against a known-clean corpus before publishing (ClamAV explicitly tests
  CVDs for false positives before release —
  [CVD FAQ](https://docs.clamav.net/faq/faq-cvd.html)), and record expiry/
  replacement per `SIGNATURES.md`.

---

<a name="roadmap"></a>
## Part 5 — A concrete, staged roadmap

1. **Document (done-ish):** this file + `SIGNATURES.md` describe the target model.
2. **Tier 1 tool:** add `tools/sleelavirin_make_defs.py` to generate signed,
   manifested first-party SHA-256 + fuzzy definitions from authorized samples.
3. **Container + signing:** define the `.svd` header/layout and a verify-on-load
   path (fail-closed); generate the publisher keypair; ship the public cert.
4. **Update path:** wire `sleela-antivirus-ctl.py` + the `systemd` timer to
   fetch → verify → atomically swap, reusing the importer's safe-replace logic.
5. **Tier 2 adapters:** add per-feed importers that stamp license/provenance, one
   index+manifest per source.
6. **Tier 3 (later):** only after a native AC/BM matcher exists, add a minimal
   self-specified content-pattern format and authoring tools + test corpora.

Each step is independently shippable and keeps ClamAV available as the external
provider for full coverage in the meantime.

---

<a name="sources"></a>
## Part 6 — Sources

- ClamAV Signatures manual — formats, CVD as signed container, custom DBs:
  https://docs.clamav.net/manual/Signatures.html
- Updating Signature Databases (freshclam, cdiffs, corruption handling):
  https://docs.clamav.net/manual/Usage/SignatureManagement.html
- "ClamAV, CVDs, CDIFFs and the magic behind the curtain":
  https://blog.clamav.net/2021/03/clamav-cvds-cdiffs-and-magic-behind.html
- ClamAV 1.5.0 release notes (FIPS-compliant CVD signing, `.cvd.sign`, legacy
  fallback): https://blog.clamav.net/2025/10/clamav-150-released.html
- Signature Database (CVD) FAQ (false-positive testing before release):
  https://docs.clamav.net/faq/faq-cvd.html
- Reference source in this repo: `libclamav/cvd.c`, `libclamav/cvd.h`,
  `libclamav/dsig.c`, `libclamav/crtmgr.c`, `libclamav/readdb.c`,
  `freshclam/`, `libfreshclam/`, `sigtool/`.

*Content from external sources was rephrased/summarized for compliance with
licensing restrictions; see links above for the originals.*
