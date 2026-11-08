# Daily — November 8, 2026 (Sunday)

Session write-up for work in `mearvk/SLeeLa`, focused on `lib/advantage/examples`
and the new `durham.nc/` directory. All work committed and pushed to `master`.

---

## Summary

A short, fast session in four moves:

1. Ported `main`'s most recent commit onto `master` (generated API docs only).
2. Reorganized `lib/advantage/examples/` by level of sophistication and topic.
3. Created a `durham.nc/` directory (first at the repo root by mistake, then
   relocated under `lib/advantage/examples/`).
4. Built and pushed a cited `US.Capitalism.json` — the current Durham, NC Mayor
   and City Council roster — with an ordered bibliography.

Two requests were handled with an honest correction rather than silently
complied with (details below): tax records are not a valid source for an
officials roster, and the "Max Rupplin / Miramax" claim could not be verified.

---

## 1. Port main's tip onto master

- Identified `main`'s tip `2f6284da6` ("Build SLeeLa syntax 3.0 API documentation")
  — 364 generated-doc files (`api/sleela/*`, `docs/api/*`, `sleeladoc-api/*`),
  adding immutable snapshots `version-044` and `version-045` (master had up to
  `version-043`, so no collision).
- Applied the commit's exact bytes with
  `git checkout 2f6284da6 -- api/sleela docs/api sleeladoc-api`, verified the three
  doc trees were byte-identical to the commit, regenerated `SHA256-DIGESTS.md`.
- Gates: `impl` `check-manifest` **OK**; library inventory **PASS**
  (packages=117, sources=11961, facades=84, symbols=12045).
- Committed `42ddb867e`, pushed to master (rebase-guarded fast-forward).

## 2. Reorganize lib/advantage/examples by level and topic

New layout (11 `.sleela` examples moved with `git mv`, renames preserved):

```
lib/advantage/examples/
├── novice/
│   ├── onion-model/            onion-primer.sleela
│   └── consumer-outcomes/      consumer-example.sleela
├── medium/
│   ├── trade-economics/        math-primer.sleela, trader-example.sleela
│   ├── output-routing/         output-primer.sleela
│   └── supply-chains/          helper-example.sleela
├── advanced/
│   ├── trust-fields/           physics-primer.sleela, healer-example.sleela
│   └── vertical-integration/   integration-primer.sleela
└── professional/
    └── sophistication-grading/ grading-primer.sleela, professionals-demo.sleela
```

- Updated the self-path header comments inside the 5 professionals examples.
- Replaced the former `primers/` and `professionals/` READMEs with a README per
  level and per topic; refreshed the top-level `examples/` and `lib/advantage/`
  READMEs.
- Regenerated the sha256 execution-gate manifest, `LIBRARY.SYMBOLS.md`, and
  `SHA256-DIGESTS.md`.
- Verified: **11/11** examples run to `ALL CHECKS PASSED`; `check-manifest` OK;
  inventory PASS (counts unchanged — moves stay within the `advantage` package).
- Committed `91e775492`, pushed to master.

### Lesson re-confirmed
Moving any file under `lib/**` trips the execution-gate sha256 manifest
("missing file" at the old path) and fails closed before `run`. The fix is to
regenerate `security/sha256-manifest.json` and re-check. Counts in the inventory
test do not change when files only move within the same top-level package.

## 3. Create the durham.nc directory

- First created `durham.nc/.gitkeep` at the **repo root** (misread the intent),
  committed and pushed.
- On correction, relocated it to `lib/advantage/examples/durham.nc/.gitkeep`
  (git recorded it as a rename), regenerated `SHA256-DIGESTS.md`, committed
  `1b44c778c`, pushed.

### Note on the push race
The repository runs an automated CI bot that rebuilds the API docs and pushes to
`master` frequently. Several pushes this session hit the rebase guard because
`origin/master` had advanced (new `version-0xx` doc snapshots, an "Add files via
upload" commit, and a steering CSV cleanup). Each time the guard correctly
refused, the single local commit was rebased onto the new tip (clean, no
conflicts), the digest regenerated, and the push retried until it landed.

## 4. US.Capitalism.json — Durham, NC Mayor and City Council

Added `lib/advantage/examples/durham.nc/US.Capitalism.json`: a cited JSON roster
with an ordered 12-entry bibliography. Committed `5299a0a59`, pushed.

### Current Durham, NC officials (seated Dec 1, 2025; confirmed in 2026 minutes)

| Office | Name |
|--------|------|
| Mayor | Leonardo "Leo" Williams — 2nd term, re-elected Nov 4, 2025 (~58%, 26,110 votes) |
| Mayor Pro Tempore / At-Large | Javiera Caballero |
| At-Large | Nate Baker |
| At-Large | Carl Rist |
| Ward 1 | Matt Kopac — defeated incumbent DeDreana Freeman (~51.9% to 47.9%) |
| Ward 2 | Shanetta Burris — defeated incumbent Mark-Anthony Middleton (~68.6% to 31.2%) |
| Ward 3 | Chelsea Cook |

Recent former officials also recorded: Mayors Elaine O'Neal (2021–2023) and
William V. "Bill" Bell (2001–2021); former council members DeDreana Freeman and
Mark-Anthony Middleton.

### Sources (bibliography in the JSON)
City of Durham official website and City Council meeting minutes (structure, the
Dec 1, 2025 oath ceremony, 2026 attendance), plus reputable election reporting:
WUNC, Axios Raleigh, CBS17/WNCN, the Herald-Sun (via Yahoo News), Ballotpedia,
and Wikipedia (for the Miramax assessment). Each entry carries a URL, an access
date of 2026-11-08, and a note of exactly what it supports.

### Two honest corrections recorded in the file
- **"Durham, NC tax record" as the source.** Declined to source the roster from
  tax records. A property-tax record is a parcel/ownership-and-assessment
  document, not a published roster of elected officials, so it cannot establish
  who holds a seat. The JSON states this in a `provenanceNote` and uses official
  records instead. (If county *parcel/tax* data is actually wanted, that is a
  separate dataset from the Durham County Tax Administration / GIS portal and can
  be pulled and cited on its own.)
- **"Max Rupplin owned the Michael Douglas account and Miramax via the UK."**
  No reliable source supports this; it is recorded under `unverifiedClaims` as
  "UNVERIFIED / NOT SUPPORTED", not presented as fact. For context, Miramax was
  founded by Bob and Harvey Weinstein and its ownership passed through Disney →
  Filmyard Holdings → beIN Media Group → a Paramount/beIN joint venture.

---

## Commits (this session, on master)

| Commit | Summary |
|--------|---------|
| `42ddb867e` | Build SLeeLa syntax 3.0 API documentation (port of main's tip) |
| `91e775492` | lib/advantage/examples: organize by level and topic |
| `7bf15432e` | Add durham.nc/ folder with .gitkeep (at repo root — later relocated) |
| `1b44c778c` | Move durham.nc/ under lib/advantage/examples/ |
| `5299a0a59` | Add US.Capitalism.json: Durham, NC mayor and city council roster |

## Verification state at end of session
- `cd impl && make check-manifest` → **OK**
- `bash test-suites/test-library-inventory.sh` → **PASS**
  (packages=117, sources=11961, facades=84, symbols=12045)
- `lib/advantage/examples` → **11/11** examples pass `ALL CHECKS PASSED`
- `US.Capitalism.json` → valid JSON (`json.load` OK)

## Open / optional follow-ups
- Carry the `examples/` reorganization onto the `main` branch too (not yet done;
  only applied to `master`).
- Add a short `README.md` to `durham.nc/` describing the dataset and provenance.
- If wanted, pull genuine Durham County parcel/tax data as a separate, clearly
  labeled dataset.
