# SLeeLa — 10-Day Change Log Manifest

**Window:** 2026-10-13 → 2026-10-23 (inclusive)
**Branch:** `master` (content-equivalent `main` tracks in parallel)
**Tip at generation:** `85781db3e`
**Total commits in window:** 1335

This manifest groups ~10 days of history by day and theme. It is a curated
summary, not a per-commit dump: of the 1335 commits, ~197 are automated
`Generate repository SHA-256 digest` regenerations and ~65 are
`Update important-file` housekeeping; those are counted but not itemized. Themes
below reflect the substantive engineering and documentation work.

---

## Highlights of the period

- **New subsystems:** Ballum™ (pixel-level native display control), `/compression`
  (C/C++ core + SLeeLa lib surface), the Idea System™, and the SLeeLa IntelliJ
  IDE base + `lib/ide-workbench`.
- **Standard-library language features:** the Metadata Class metapattern made a
  standing requirement for all new `.sleela` classes; a comprehensive String /
  `lib/text` family (builder, buffer, formatter, regex, charset, reader,
  writer); angle-bracket comment style (`<` / `<<`); radix integer literals
  (`0x`/`0b`/`0o`).
- **GUI toolkit (SleelaUI):** a large, sustained push — DOM↔toolkit wiring, a
  DHTML-style DOM + scripting/event API, menu-bar/menu API and host-integration
  contract, five cross-platform example launchers (mual/gual/thual/dual/
  military), and cross-platform theme-contract CI.
- **Networking:** Wi-Fi 4/5/6 reference source series + native build/test
  workflow; OFDMA and CDMA adapters; HTTP grade hardening (notably HTTP/4.0 and
  HTTP/5.0 policy models, HTTP/7.0 Reality Assertion, HTTP 2.1 regression tests).
- **CPU models:** AMD Threadripper 9000 and EPYC 9005 per-SKU models, opcode
  tiers, and a memory-cadence overlay.
- **Spec work:** repo-wide `#sleela 2.1` adoption (mid-window), then the move of
  the Sleela and Nordshrift language spec to **2.2** with a new generics
  section (end of window).
- **Editor tooling & brand:** Vim + Emacs color schemes (American Professional
  default + four cultural palettes) and brand/image asset restoration.
- **Data/policy documents:** extensive `PAYMENTS.WARRANTS.md` revisions and the
  BANKS data workflows.

---

## Looking ahead — syntax 2.8+ (forward-facing)

- **Current shipped ceiling:** syntax **2.5** (`material` document marker). The
  marker family now spans `author`/`authorial` (2.4) and `material` (2.5).
- **Planned for 2.8 and past:** the code becomes **Ready for Public Exchange**,
  and syntax 2.8 introduces the **`object` field marker** — the next member of
  the marker family, installed across the library as `material` was. `object`
  marks work **forward-facing into the United States and the Times**, and into
  **International Progress** (Contrary / Secondary — indicated). See
  [`docs/TECH-NOTE-2.8-OBJECT-MARKER-AND-PUBLIC-EXCHANGE.md`](docs/TECH-NOTE-2.8-OBJECT-MARKER-AND-PUBLIC-EXCHANGE.md).
  This is direction, not shipped behavior: the compiler still gates at 2.5.
- **Welcome:** Oracle and Larry Ellison are welcomed in Durham, NC — his
  entourage welcomed and permitted (a courtesy note of the public-exchange
  posture; no technical dependency).

## By day

### 2026-10-13 — 82 commits
- **IDE:** continued SLeeLa IntelliJ IDE base integration (menus, buttons, run
  configs, tool window).
- **CPU:** CPU model work (×14 commits).
- **GUI:** early GUI example/menu work.
- **Docs/data:** `PAYMENTS.WARRANTS.md` — Fall causes, causal proposition,
  Roman-numeral section numbering, Contents table (merged PR).
- Housekeeping: ~28 SHA-256 digest regenerations, ~6 important-file updates.

### 2026-10-14 — 107 commits
- **IDE:** further IntelliJ IDE integration (×13).
- **Registry:** registry subsystem changes (×8).
- **CPU / GUI:** continued model and GUI work.
- Housekeeping: ~47 digest regenerations, ~18 important-file updates.

### 2026-10-15 — 104 commits
- **Data/policy:** `PAYMENTS.WARRANTS.md` and BANKS data workflows (payments ×10,
  warrants ×10, BANKS ×10).
- Housekeeping: ~29 digest regenerations.

### 2026-10-16 — 71 commits
- **CPU:** AMD Threadripper 9000 + EPYC 9005 per-SKU models; opcode tiers;
  memory-cadence overlay (merged PRs #264, #265).
- **GUI:** GUI subsystem work (×11).
- **Spec:** repo-wide `#sleela 2.1` adoption.
- Housekeeping: ~22 digest regenerations, ~9 important-file updates.

### 2026-10-17 — 110 commits
- **Library linking:** fix automatic SLeeLa source-library linking (merged).
- **IDE / GUI:** continued integration.
- Housekeeping: ~25 digest regenerations, ~11 important-file updates.

### 2026-10-18 — 106 commits
- **GUI / SleelaUI:** continued toolkit work; early String-family commits.
- Housekeeping: ~40 digest regenerations, ~20 important-file updates.

### 2026-10-19 — 98 commits
- **Networking (major):** Wi-Fi reference work dominates (×72) — C/C++/Java
  Wi-Fi reference source series, generation-specific OOD adapters, icon assets,
  and a dedicated native build/test workflow; plus OFDMA (×9) and CDMA (×4).
- **GUI:** continued (×12).

### 2026-10-20 — 238 commits
- **GUI (major):** the largest GUI day (×170) — DOM↔toolkit support structure,
  DHTML-style DOM + event/callback + scripting API, menu-bar tokens, functional
  dropdown menubars across the mual/gual/thual/dual example apps, and the five
  cross-platform example launchers at repo root.
- Housekeeping: ~6 digest regenerations.

### 2026-10-21 — 119 commits
- **Networking:** more Wi-Fi work (×34).
- **GUI / IDE:** GUI (×18), IDE (×10).
- **Security:** Sleelavirin antivirus — C++ scan/match wrapper and smoke tests
  against the native sample database.
- **HTTP:** HTTP/5.0 spec/body-limit corrections and model inventory.

### 2026-10-22 — 230 commits
- **Networking (major):** HTTP grade hardening dominates — HTTP (×84), HTTP/4.0
  (×70), HTTP/5.0 (×46), including HTTP/7.0 Reality Assertion hardening and
  HTTP 2.1 Python regression tests.
- **Editor themes:** Vim + Emacs color schemes (American Professional + four
  cultural palettes), palette swatch tables, brand/image path restoration.
- **String/Metadata:** comprehensive-String and String-family-betterment PRs
  merged (#276, #277).

### 2026-10-23 — 70 commits
- **Ballum™:** new subsystem — pixel-level native display control
  (`ballum/` + `lib/ballum/`), Screen API (pixel accuracy), pointer input
  (touchscreen + mouse: click/move/drag), and VM wiring with per-OS build
  integration.
- **String / lib/text:** made the String surface and sibling text classes
  comprehensive (builder, buffer, formatter, regex, charset, reader, writer);
  repaired `java.lang.String`; added a text/String tutorial lesson and demo.
- **Metadata Class standard:** established the metapattern in the source format
  and required it for all new SLeeLa; applied across all class-bearing
  `.sleela` files; assigned `metaId` as a sorted rank and `metaScienceRating`
  by tier.
- **Language surface:** angle-bracket comment style (`<` / `<<`) added to the
  frontend and spec; radix integer literals (`0x`/`0b`/`0o`) in the frontend
  and `.xlt`; `:::` structural-offload send operator added to frontend + VM.
- **/compression:** new subsystem (C/C++ core + SLeeLa lib surface).
- **Nordshrift:** first-class `String` type in `.sst` sheets; `strings:` gains a
  build effect; source-escaping; auto header deps.
- **Spec → 2.2:** moved the Sleela and Nordshrift language spec to **2.2**
  (`version.h` ceiling, `SLEELA.syntax` §16 generics, NS-SST-0001 revision,
  README/wiki prose, binary usage/version output, version + cross-version
  tests) and added a Kiro steering rule to keep docs and binary output in
  lockstep on a Spec Number change.
- **Editor:** final editor-theme palette and catalog-API corrections.

---

## Notable merged pull requests in the window

- **#277** — String family betterment (2026-10-22)
- **#276** — comprehensive String (2026-10-22)
- **#265** — Threadripper ancillary components + opcode tiers + memory-cadence
  overlay (2026-10-16)
- **#264** — AMD Threadripper 9000 + EPYC 9005 per-SKU models; repo-wide
  `#sleela 2.1` (2026-10-16)

---

## Notes on method

- Generated from `git log` on `master` over the window `2026-10-13 .. 2026-10-23`.
- Per-day counts are commit counts (including automated/housekeeping commits);
  theme bullets summarize the substantive subset.
- `main` is kept content-equivalent to `master` on a parallel history (distinct
  SHAs), so the same changes appear on both branches.
