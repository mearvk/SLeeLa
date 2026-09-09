<!-- @MT:SXL -->
# METATAGS — Single-Letter File Metatags for Fast Reference

## Series MT-META-0001

| Field | Value |
|-------|-------|
| Document Series | MT-META-0001 |
| Document Status | Proposal (first draft) |
| Revision | 0.1.0 |
| Date | September 2026 |
| Governing Tool | `metatag` indexer (ships with `impl/`) |
| Companion Documents | `SHEET.sheet` (system-object catalog), `SST.model` (NS-SST-0001) |
| Machine-readable registry | [`METATAGS.sheet`](METATAGS.sheet) |

> **Normative status.** Statements using **must / must not / shall / required /
> forbidden** are normative. Statements using **should / may / recommended /
> optional** are non-normative guidance. This first draft is a *proposal*; the
> normative words describe the design as proposed, pending acceptance.

---

## 1. Purpose

Every file in this repository carries meaning that is currently only discoverable
by reading it. A reader — human or machine — who wants "the files that *print*",
"the files about *rights*", or "the files that are *tasks*" has to open each one.

**Metatags** fix this. A metatag is a **single upper-case letter** stamped onto a
file that declares, at a glance, one facet the file bears. `P` means the file has
**Print** concerns; `M` means **Math / Methods**; `R` means **Reals**; `T` means
**Tasks**; and so on across a closed series of **16 tags** (within the requested
12–26 range).

The series is deliberately built to serve a reader operating at very high insight
("200+ IQ" in the request's terms): it is *dense* (one letter carries a whole
facet), *composable* (a file may bear several tags), *total* (every concern in the
request has a home), and *mechanically searchable* (the same one letter is what a
search engine, IDE, or OS file search matches on).

## 2. Design goals

1. **One glance, one letter.** A tag is exactly one `[A-Z]` character. No file is
   forced to carry more than it means; a file may carry several.
2. **Two families, one alphabet.** Tags split into a **Code family** (what the file
   *does* as software — Print, Math/Methods, Reals, Tasks, …) and a **Concern
   family** (what the file is *about* — management, material, rights, concerns,
   voters, God, humans). Both draw from the same single-letter namespace so a
   single search matches either.
3. **Grounded in the existing catalog.** Where a Code tag corresponds to a role in
   [`SHEET.sheet`](SHEET.sheet), it is mapped to that role, so metatags and the
   conducted-method catalog agree. Nothing new is invented where the sheet already
   names it.
4. **Findable everywhere.** The *same* token is emitted in three shapes — an
   in-file marker, a per-file front-matter block, and a repo-root index — so that
   `grep`, an IDE symbol search, OS search (Spotlight / Windows Search / `locate`),
   and web crawlers all resolve the same tag with no bespoke tooling.

## 3. The metatag series (16 tags)

The series carries **16** tags — the same count as `SHEET.sheet`'s 16 role
categories, and comfortably inside the requested 12–26 band. Each tag has a
single letter, a family, a name, a one-line meaning, and (for Code tags) the
`SHEET.sheet` role it maps to.

### 3.1 Code family — what the file *does*

| Tag | Name | Meaning | Maps to `SHEET.sheet` role |
|-----|------|---------|----------------------------|
| **P** | Print | Emits output / I/O; contains `print`, formatting, rendering | `piping` (Sink, Stream) |
| **M** | Math / Methods | Arithmetic, algorithms, or method/function definitions | `method` |
| **R** | Reals | Real-number / floating-point / measurement / metric content | `gain` (Metric) |
| **T** | Tasks | Threads, spawn/join, scheduling, concurrent units of work | `completitive` (Join, Future) |
| **C** | Calls | Invocation, dispatch, RPC, syscalls, the exchange API | `call` |
| **S** | System | System / core / runtime / kernel / VM structure & invariants | `root` (System) |
| **X** | Exchange & Transit | Protocols, channels, relays, gateways, transpile drivers (`.sst`) | `transitive` + `conversation` |
| **L** | Limits & Congrains | Bounds, quotas, timeouts, invariants, contracts, constraints | `limit` + `congruence` |

### 3.2 Concern family — what the file is *about*

| Tag | Name | Meaning |
|-----|------|---------|
| **G** | Governance / Management | Management, ownership, lifecycle, decision authority, process control |
| **A** | Assets / Material | Material, resources, money, holdings, physical or economic substance |
| **W** | Warrants / Rights | Rights, entitlements, permissions, licenses, legal interests |
| **N** | Notes / Concerns | Open concerns, caveats, risks, disputed or unsettled matters |
| **V** | Voters / Public | Voters, the electorate, constituencies, public record, civic process |
| **D** | Deity / God | Theological, sacred, or ultimate-authority content |
| **H** | Humans / Persons | Individual persons, biography, identity, the human party |
| **E** | Evidence / Provenance | Records, sources, provenance, evidentiary chains, citations |

> **Why these letters for the concerns.** Where a natural first letter is already
> claimed by a Code tag (Management→`M`, Rights→`R`, Concerns→`C`), the Concern
> tag takes an unambiguous alternate that is still mnemonic: **G**overnance,
> **W**arrants, **N**otes. `Material` becomes **A**ssets to avoid clashing with
> **M**ath/Methods. This keeps every one of the 16 letters distinct across both
> families — a single letter never means two things.

### 3.3 The 16 letters at a glance

```
Code:    P M R T C S X L
Concern: G A W N V D H E
```

Sixteen distinct letters; ten of the twenty-six remain **reserved** for future
tags (`B F I J K O Q U Y Z`) so the series can grow toward 26 without renaming
anything.

## 4. Selection rules (normative)

1. A file **must** carry a metatag for every facet that is *load-bearing* in it —
   a facet a reader would reasonably search for to find this file.
2. A file **must not** carry a tag for a facet that merely appears in passing.
   Metatags mark what a file *is for*, not every word it contains.
3. A file **may** carry multiple tags. When it does, tags **must** be written in
   **canonical order**: the Code family in the order `P M R T C S X L`, then the
   Concern family in the order `G A W N V D H E`. Canonical order makes a
   multi-tag string itself searchable (e.g. `PM` always means Print+Math, never
   `MP`).
4. The **first** tag in canonical order is the file's **primary** tag; tools that
   can show only one letter (a badge, a column) show the primary.
5. Tags are **case-fixed**: always upper-case. Lower-case is reserved and **must
   not** be used, so searches are unambiguous.

## 5. How a tag is stamped onto a file

To be findable by *every* class of search, the same tag is emitted in up to three
shapes. Shape (a) is **required**; (b) is **required for prose/spec docs**; (c) is
**generated, not hand-authored**.

### 5.1 (a) In-file marker — required

A single comment line, near the top of the file, using the token `@MT:` followed by
the canonical tag string. The token is chosen to be greppable and to never collide
with ordinary prose.

Markdown / prose:
```markdown
<!-- @MT:PMRT -->
```

C / C++ / Sleela source:
```c
/* @MT:MSC */
```

Sheet / `.sst` / config (hash-comment files, matching `SHEET.sheet` style):
```
# @MT:LX
```

**Rule:** the marker line **must** match the regular expression
`@MT:([A-Z]+)` and the captured letters **must** be in canonical order (§4.3).

### 5.2 (b) Front-matter block — required for prose & spec documents

For `.md` and spec files, a small header block gives search engines and readers a
labeled, human-visible expansion. It uses the repo's `#key value` directive style:

```markdown
<!--
#metatags  P M R T
#primary   P
#series    MT-META-0001
-->
```

This is what a crawler indexes and what schema.org `keywords` (see §6.4) is derived
from.

### 5.3 (c) Repo-root index — generated

[`METATAGS.index`](METATAGS.index) is a generated manifest mapping every tagged
file to its tags (and the reverse: every tag to its files). It is produced by the
`metatag` tool (§7) and is the artifact an IDE, an OS-search integration, or a CI
check reads. It is **never** hand-edited.

## 6. Making documents fast to find

The whole point is speed of reference. The single token `@MT:` plus one letter is
engineered so that four different search surfaces resolve it with no custom parser.

### 6.1 Command line / `ripgrep` / `grep`

Because every tag is the literal string `@MT:` + letters, the primitives are trivial:

```sh
# every file tagged Print (primary or secondary)
rg -l '@MT:[A-Z]*P'

# every file whose PRIMARY tag is Tasks (T is first letter after @MT:)
rg -l '@MT:T'

# files about rights (W) AND humans (H)
rg -l '@MT:[A-Z]*W' | xargs rg -l '@MT:[A-Z]*H'
```

The `metatag` tool (§7) wraps these as `metatag find P`, `metatag find --primary T`,
`metatag find W H`.

### 6.2 IDE / editor search

- The `@MT:` token is a **plain literal**, so any editor's "find in files" locates
  tagged files instantly with the query `@MT:` (list all) or `@MT:...W` (by tag).
- [`METATAGS.index`](METATAGS.index) is emitted in **ctags-compatible** form as
  [`.tags.mt`](#) so editors that read a tags file expose each tag letter as a
  jumpable symbol (e.g. jump to symbol `MT:W` → the rights files).
- A `.editorconfig`-adjacent hint file lists the token so language servers can
  surface tags as document symbols.

### 6.3 OS file search

- **macOS Spotlight:** the `metatag` tool writes the tags into the extended
  attribute `com.apple.metadata:kMDItemKeywords`, so `mdfind "kMDItemKeywords == 'MT-P'"`
  and Finder keyword search return tagged files. It also drops a sidecar
  `.spotlight` comment.
- **Windows Search:** tags are written to the NTFS `System.Keywords` property
  (via an alternate data stream sidecar), matched by `tag:MT-P` in Explorer.
- **Linux:** tags are written to the `user.xdg.tags` extended attribute
  (`setfattr -n user.xdg.tags -v "MT-P"`), which `baloo`/`tracker` and
  `locate`-style indexers pick up.

Each OS keyword uses the form `MT-<letter>` (e.g. `MT-P`, `MT-W`) so a single OS
search term maps one-to-one to a metatag.

### 6.4 Web / search engines

For any file published to the web (GitHub Pages, a docs site), the front-matter
(§5.2) is rendered into:

- an HTML `<meta name="keywords" content="MT-P, MT-M, ...">` tag, and
- a JSON-LD `schema.org/TechArticle` block whose `keywords` array carries the
  `MT-<letter>` tokens and whose `about` carries the tag *names*.

A generated `sitemap.mt.xml` lists every tagged document with its tags as
`<keywords>`, and a `robots`-friendly `metatags.txt` at the site root advertises
the scheme, so crawlers index the vocabulary itself. This lets an external search
("SLeeLa rights documents") resolve to the `W`-tagged files.

## 7. The `metatag` tool

A companion utility. A **working POSIX-shell reference implementation** ships now
at [`impl/metatag/metatag.sh`](impl/metatag/metatag.sh) (no build step); a compiled
version sharing the `catalog/` sheet parser is proposed to follow. It provides:

- `metatag scan` — walks the repo, reads every `@MT:` marker, validates canonical
  order and letter legality against [`METATAGS.sheet`](METATAGS.sheet), and reports
  violations (exit non-zero for CI).
- `metatag index` — (re)generates [`METATAGS.index`](METATAGS.index), the
  ctags-style `.tags.mt`, and the web artifacts (§6.4).
- `metatag stamp <file> <TAGS>` — writes/updates the in-file marker, front-matter,
  and OS extended attributes in one step.
- `metatag find <TAGS...>` — the search front end (§6.1), with `--primary`.

Because it reuses the existing `catalog/` module, the Code-tag→role mappings in
§3.1 are validated against `SHEET.sheet` at scan time: a Code tag whose mapped role
disappears from the sheet is a scan error.

## 8. Worked example — the concern documents in this repo

Applying the series to the existing files:

| File | Tags (canonical) | Reading |
|------|------------------|---------|
| `README.md` | `S X G` | System overview, transpile/exchange, project governance |
| `impl/README.md` | `P M T C S X L` | prints, methods, tasks, calls, system, exchange, limits |
| `SHEET.sheet` | `S L` | System root + limits/congrains catalog |
| `SST.model` | `X L` | transpile-transit spec, normative limits |
| `1982.md` | `N V H E` | concerns, public/voters, a person, evidentiary method |
| `SAKES.md` | `N V H E` | concerns, public record, persons, evidence |
| `SAKES_SIZE.md` | `R N H E` | measurement ("size"), concerns, a person, provenance |
| `SAKES_BINARY_TIMELINE.md` | `R V H E` | binary measure, voters/public, persons, evidence |

Note how `God` (`D`) and `Assets/Material` (`A`), `Warrants/Rights` (`W`), and
`Governance` (`G`) are all in the alphabet and ready the moment a file bears them —
the series is **total over the requested concerns** (management→`G`, material→`A`,
rights→`W`, concerns→`N`, voters→`V`, God→`D`, humans→`H`) even where no current
file yet uses a given letter.

## 9. Rollout plan

1. **Accept the series** (this doc + `METATAGS.sheet`).
2. **Stamp** existing files per §8 using `metatag stamp` (in-file markers first).
3. **Generate** `METATAGS.index` + web artifacts via `metatag index`.
4. **Wire CI:** `metatag scan` runs on every push; canonical-order or illegal-letter
   violations fail the build.
5. **Grow toward 26** only by claiming reserved letters (`B F I J K O Q U Y Z`);
   existing letters are never repurposed.

## 10. Open questions (for reviewers)

- Should `X` (Exchange & Transit) be split into two tags (protocol vs. transpile)
  now, or held as one until a file needs the distinction?
- Do we want a **negation/severity** convention for `N` (Notes/Concerns), e.g.
  `N!` for a blocking concern? (Would break the "one letter" rule; proposed as a
  separate follow-up, not part of this series.)
- Confirm the OS-keyword prefix `MT-` (vs. `mt:` or `#MT`) for cross-platform search.
