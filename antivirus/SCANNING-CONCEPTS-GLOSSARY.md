# Scanning Concepts — A Glossary for Novices and Professionals

**Author:** Max Rupplin — MEARVK LLC — 2026
**Companion to:** `CLAMAV-REFERENCE-ANALYSIS.md`
**Purpose:** Define every algorithm, data structure, and structural term used in
the ClamAV reference analysis and in the Sleelavirin™ native matcher. Each entry
gives (a) a plain-language definition, (b) *how* it works, (c) *why* it works,
and (d) its structural dependencies and trade-offs. Read top to bottom for a
course, or jump to any term.

> No ClamAV source is reproduced here. These are general computer-science
> concepts explained in our own words.

---

## Table of contents

- [Part A — The core idea: signatures and matching](#part-a)
- [Part B — String-matching algorithms](#part-b)
  - [Naive / brute-force matching](#naive)
  - [Boyer–Moore (BM)](#boyer-moore)
  - [Aho–Corasick (AC)](#aho-corasick)
  - [The trie](#trie)
  - [Finite automaton & failure links](#automaton)
- [Part C — Hashing](#part-c)
  - [Hash function](#hashfn)
  - [Cryptographic hash (MD5 / SHA-256)](#crypto-hash)
  - [Hash table / hash map](#hashtable)
  - [Open addressing & linear probing](#open-addressing)
  - [FNV-1a](#fnv)
  - [Fuzzy / similarity hashing & Hamming distance](#fuzzy)
- [Part D — Pattern description](#part-d)
  - [Regular expressions & PCRE](#pcre)
  - [Byte comparison](#byte-compare)
  - [Logical signatures & subsignatures](#logical-sigs)
  - [Prefilter / Bloom-style filter](#prefilter)
- [Part E — Structural concepts](#part-e)
  - [Compile-once / scan-many](#compile-once)
  - [The matcher "root" (in-memory database)](#root)
  - [Scan context (threaded state)](#context)
  - [fmap (byte-source abstraction)](#fmap)
  - [Bounded recursion stack](#recursion)
  - [Memory pool (arena allocation)](#mpool)
  - [Error-code control flow](#errcodes)
- [Part F — How the pieces fit: worked mental model](#part-f)

---

<a name="part-a"></a>
## Part A — The core idea: signatures and matching

**Novice framing.** A virus scanner is mostly a very fast *"find this needle in
this haystack"* machine. The **haystack** is the file's bytes. The **needles**
are **signatures**: short fingerprints of known-bad content. "Scanning" means
checking whether any needle appears in the haystack. A **match** means "these
bytes contain a known signature → report the threat by name."

**Why it is harder than it sounds.** There are *millions* of signatures and files
can be *large*. You cannot search for each needle one at a time — that would be
millions of passes over every file. The whole art of an antivirus engine is
answering *"do ANY of these millions of patterns appear?"* in roughly **one pass**
over the data. Every algorithm below is a way to make that one-pass promise true
for a different *kind* of needle (literal bytes, hashes, regexes, fuzzy matches).

**Two broad families of signature:**
1. **Content signatures** — "these specific bytes appear somewhere in the file."
   Handled by string-matching algorithms (BM, AC) and regex (PCRE).
2. **Hash signatures** — "the whole file (or a region) has exactly this
   fingerprint." Handled by hashing + hash tables.

Sleelavirin's native core currently implements family #2 (hashing) plus a fuzzy
variant; ClamAV implements both families. Everything else is machinery to do
these fast, safely, and at scale.

---

<a name="part-b"></a>
## Part B — String-matching algorithms

<a name="naive"></a>
### Naive / brute-force matching

- **What.** The obvious method: to find pattern `P` (length *m*) in text `T`
  (length *n*), try `P` at position 0, then 1, then 2 … comparing character by
  character.
- **How.** At each start position, compare up to *m* characters; on the first
  mismatch, shift the pattern one position right and start over.
- **Why it works.** It literally checks every possible alignment, so it cannot
  miss a match.
- **Cost / why we need better.** Worst case **O(n·m)** per pattern, and with *k*
  patterns it is **O(n·m·k)** — hopeless for millions of signatures. It is the
  baseline the smarter algorithms beat.

<a name="boyer-moore"></a>
### Boyer–Moore (BM)

- **What.** A single-pattern search that is fast in practice because it is
  allowed to **skip** large chunks of the text instead of moving one byte at a
  time.
- **The key insight.** Compare the pattern to the text **right-to-left** (from the
  pattern's last character backward). When a mismatch happens, you often learn
  that the pattern cannot possibly start for several more positions, so you jump
  ahead by more than one.
- **How (the "bad character" rule, the part ClamAV leans on).** Precompute, for
  every possible byte value, how far that byte is from the end of the pattern.
  On a mismatch at some text byte `c`, shift the pattern so its *last* occurrence
  of `c` lines up with that text position. If `c` does not occur in the pattern at
  all, skip the *entire* pattern length. (A second "good suffix" rule can skip
  even more; the bad-character rule alone is already very effective.)
- **Why it works / why it's fast.** Correctness: the shift tables are derived so
  that no alignment containing a match is ever skipped. Speed: on typical binary
  data most bytes mismatch immediately and trigger a big jump, so BM often
  examines **far fewer than *n*** bytes — sublinear in the happy case. That is why
  ClamAV uses BM as the quick first pass for plain literal signatures.
- **Structural dependency.** Needs a **precomputed shift table** per pattern
  (indexed by byte value). This is the "compile once" cost. In the reference
  engine this table lives in the matcher root as `bm_shift` / `bm_suffix`.
- **Limitation.** BM is *per pattern*. Running it for a million signatures means a
  million scans — so it is only used where the pattern set is small or as a fast
  complement. For "all patterns at once" you need Aho–Corasick.

<a name="aho-corasick"></a>
### Aho–Corasick (AC) — *the main engine*

- **What.** A **multi-pattern** search: find *all* occurrences of *any* of
  thousands/millions of patterns in **one single pass** over the text.
- **The key insight.** Build one combined search structure from the entire
  dictionary of patterns ahead of time (a **trie** plus **failure links**, see
  below). Then read the text byte by byte, moving through that structure. The
  work per text byte does **not** grow with the number of patterns.
- **How.**
  1. Insert every pattern into a **trie** (a tree where each edge is one byte and
     each root-to-node path spells a prefix of some pattern).
  2. Add a **failure link** from each node to the longest proper suffix that is
     also a node in the trie (classic KMP idea, generalized to many patterns).
  3. Mark nodes that complete a pattern as **final** (they carry the matched
     pattern's identity / virus name).
  4. To scan: start at the root; for each input byte, follow the matching child
     edge if it exists, otherwise follow failure links until an edge matches (or
     you fall back to root). Whenever you land on (or pass through) a final node,
     you have matched that pattern.
- **Why it works.** The trie encodes all patterns' prefixes compactly; the failure
  links guarantee that after a partial match breaks, you resume at the longest
  still-valid partial match instead of restarting — so no possible match is lost
  and no position is re-read.
- **Why it's fast.** Total time is **O(n + z)** where *n* is text length and *z*
  is the number of matches — **independent of the number of patterns**. One pass,
  constant work per byte (amortized). This single property is what makes
  million-signature scanning feasible, which is why AC is ClamAV's primary matcher
  (`matcher-ac.c`, by far the largest matcher file).
- **Structural dependency.** A **built** automaton: trie nodes, transition
  arrays, and failure links, all finalized at load time (`cli_ac_buildtrie` in the
  reference). Building is comparatively expensive; scanning is cheap. ClamAV also
  layers **wildcards**, byte ranges (`[a-b]`), case-insensitivity, and "wide"
  (UTF-16) options on top of the base AC (`cli_ac_patt`, `cli_ac_special`).

<a name="trie"></a>
### The trie (prefix tree)

- **What.** A tree where each **edge** is labeled with one symbol (here, one byte,
  so up to 256 children per node). The string for any node is the sequence of edge
  labels from the root to that node.
- **How / why.** Shared prefixes share nodes, so `evil`, `evilware`, and `evict`
  overlap on `ev…`. Lookup of a string of length *m* is **O(m)**, independent of
  how many strings are stored. That is the foundation AC builds on.
- **Trade-off.** Fast but memory-hungry if stored as a full 256-way transition
  array per node; engines mitigate this with list-based nodes for sparse levels
  and dense arrays only near the top. The reference engine exposes exactly this
  duality: `cli_ac_node` has both a `trans` array and a `list`
  (`IS_LEAF`/`IS_FINAL` macros distinguish them).

<a name="automaton"></a>
### Finite automaton & failure links

- **Finite automaton.** A machine with a finite set of **states** that moves
  between them as it reads input symbols; some states are "accepting." A trie with
  transitions *is* such a machine; AC turns it into a complete one.
- **Failure link.** The edge a state follows when the current input byte has no
  normal transition — it jumps to the state representing the longest suffix of
  what you have read so far that is still a valid prefix of some pattern. This is
  the generalization of the Knuth–Morris–Pratt (KMP) "don't re-read" trick to
  many patterns at once, and it is *why* AC never backtracks over the text.

---

<a name="part-c"></a>
## Part C — Hashing

<a name="hashfn"></a>
### Hash function

- **What.** A function that maps data of any size to a fixed-size number (the
  *hash* / *digest*).
- **Why.** It turns "compare these big blobs" into "compare these small fixed-size
  numbers," and it lets you *index* data by content.
- **Two very different jobs** (do not confuse them):
  - **Table hashing** — spread keys evenly across buckets; speed matters, security
    does not (e.g. FNV-1a).
  - **Cryptographic hashing** — produce a fingerprint that is practically unique
    and infeasible to forge (e.g. SHA-256).

<a name="crypto-hash"></a>
### Cryptographic hash (MD5 / SHA-256)

- **What.** A hash designed so that (a) the same input always gives the same
  digest, (b) it is infeasible to find two inputs with the same digest
  (*collision resistance*), and (c) a one-bit change in the input scrambles the
  whole output (*avalanche*). SHA-256 produces 256 bits (32 bytes).
- **How antivirus uses it.** Compute the digest of a whole file (or a region). If
  it equals a known-bad digest, it is *that exact* malware — a **whole-file hash
  signature**. This is the simplest, most precise signature type: zero false
  positives for that exact file.
- **Why it works.** Collision resistance means a clean file essentially never
  collides with a malware digest by accident.
- **Why it is not enough by itself.** It only catches files that are **byte-for-
  byte identical** to a known sample. Flip one byte and the digest changes
  completely — so hashing alone is defeated by trivial mutation. That is exactly
  why content matching (BM/AC) and fuzzy hashing exist alongside it.
- **In Sleelavirin.** The native core (`sleelavirin_scan.c`) stores known-bad
  **SHA-256** fingerprints and reports `SLEELAVIRIN_EXACT_MATCH` (confidence 100)
  on a hit.

<a name="hashtable"></a>
### Hash table / hash map

- **What.** A data structure giving **average O(1)** insert and lookup by key. An
  array of "slots"; the key's hash picks the slot index.
- **How.** `index = hash(key) mod capacity`. Store the entry there; look it up by
  recomputing the hash and going straight to that slot.
- **Why it works / why it's fast.** A good hash scatters keys uniformly, so most
  lookups touch one slot. This is how "is this file's digest in our million-entry
  bad list?" becomes a single array probe instead of a scan.
- **Collision.** Two keys can map to the same slot; the table needs a resolution
  strategy (below).

<a name="open-addressing"></a>
### Open addressing & linear probing

- **What.** A collision strategy that keeps everything *inside the array* (no
  linked lists). If the target slot is taken, **probe** the next slot, and the
  next, until an empty one is found (*linear probing*).
- **How lookup ends.** Searching stops at the first **empty** slot — if you reach
  an empty slot without finding your key, it is not present.
- **Why it works well.** Cache-friendly (contiguous memory), no per-node
  allocation. Requires keeping the table below a **load-factor** threshold (how
  full it is) so probe chains stay short.
- **In Sleelavirin.** `sleelavirin_hashmap_t` uses a power-of-two capacity, a
  `hash & (capacity-1)` index (fast modulo), linear probing, and refuses inserts
  past ~70% load (`count*10 >= capacity*7`) to keep probes short.

<a name="fnv"></a>
### FNV-1a (a fast non-cryptographic hash)

- **What.** Fowler–Noll–Vo, a tiny, fast hash used for **table indexing** (not
  security). Sleelavirin uses it to turn a 32-byte SHA-256 digest into a 64-bit
  table key.
- **How.** Start from a fixed 64-bit "offset basis," then for each input byte:
  XOR the byte into the accumulator, then multiply by a fixed 64-bit prime. Repeat.
- **Why it works for its job.** The alternating XOR+multiply spreads bits well
  enough to distribute keys evenly across buckets, and it is only a few
  instructions per byte — ideal for a table hash. (It is *not* collision-resistant
  and must never be used as a security fingerprint — that is SHA-256's job.)

<a name="fuzzy"></a>
### Fuzzy / similarity hashing & Hamming distance

- **The problem it solves.** Cryptographic hashes are *fragile by design*: change
  one byte and the digest is unrelated. Malware authors exploit this by making
  tiny variants. **Fuzzy hashing** produces fingerprints where *similar inputs
  give similar fingerprints*, so near-matches can be detected.
- **How (feature-vector style, as in Sleelavirin).** Summarize the file as a set
  of **features** packed into bits: a `feature_mask` marks which features are
  meaningful, and a `feature_fp` holds their values. To compare two files, XOR
  their fingerprints (restricted to features both have) and **count the differing
  bits** — the **Hamming distance**. Few differing bits → very similar.
- **Hamming distance.** The number of bit positions at which two equal-length bit
  strings differ. Computed with **popcount** (count-set-bits); the classic
  `v &= v-1` loop clears one set bit per iteration, so it loops once per set bit.
- **Why it works.** Small edits to the input change only a few feature bits, so a
  variant stays within a small Hamming distance of the original signature.
- **Trade-off.** Approximate → can have false positives, so it yields a
  **confidence score** rather than certainty. In Sleelavirin, a within-threshold
  match becomes `SLEELAVIRIN_SEMI_MATCH` with confidence derived from the distance
  (`(64 − distance)/64`), capped below 100 so it never masquerades as an exact
  hit. ClamAV's image **fuzzy hash** (for GIF/PNG/JPEG/TIFF) is the same idea
  applied to picture content.

---

<a name="part-d"></a>
## Part D — Pattern description

<a name="pcre"></a>
### Regular expressions & PCRE

- **What.** A **regular expression** (regex) is a compact language for describing
  a *set* of strings — e.g. "the bytes `MZ` followed by any 10 bytes then
  `This program`." **PCRE** = *Perl Compatible Regular Expressions*, the de-facto
  standard regex library/syntax that ClamAV embeds (`matcher-pcre.c`).
- **How.** The regex is **compiled** into a small automaton/bytecode; the engine
  then runs input through it to test for a match. It expresses things literal
  patterns cannot: alternation, optional parts, repetition, back-references.
- **Why it runs *last* in the cascade.** Regexes are the most expressive but most
  expensive matcher, and in ClamAV they typically feed **logical signatures** that
  can only be decided after the cheaper literal subsignatures are known. They may
  also require the whole buffer in memory at once. So the engine does cheap/fast
  matchers first and reserves PCRE for the end.
- **Trade-off.** Powerful but potentially slow (pathological regexes can blow up),
  hence size limits (`PCRE_MAX_FILESIZE`) and careful ordering.

<a name="byte-compare"></a>
### Byte comparison

- **What.** A subsignature type that reads a value *at an offset discovered by
  another match* and numerically compares it (e.g. "the 4 little-endian bytes
  after match X are greater than N"). ClamAV: `matcher-byte-comp.c`.
- **Why.** Lets a signature reason about **structure** (sizes, counts, offsets in a
  file format), not just the presence of bytes — far more precise than a literal
  pattern alone.

<a name="logical-sigs"></a>
### Logical signatures & subsignatures

- **What.** A **logical signature** fires only when a **boolean expression** over
  several **subsignatures** is satisfied — e.g. "subsig 0 AND (subsig 1 OR subsig
  2), and the file type is PE, and size < 2 MB." Each subsignature is itself a
  simple pattern (literal, regex, byte-compare, hash…).
- **How.** The cheap literal subsigs are matched by AC/BM in the normal pass, and
  each match **increments a counter** for its subsig (reference: `lsigcnt` in
  `cli_ac_data`). After the pass, the engine evaluates the boolean expression over
  those counters, plus metadata constraints held in `cli_lsig_tdb` (target type,
  file size, entry point, container, etc.).
- **Why it works / why it matters.** It lets analysts **compose** precise, low-
  false-positive detections out of simple reusable parts, instead of writing
  bespoke detection code per malware family. This is the single most important
  "expressiveness" feature of a mature engine.

<a name="prefilter"></a>
### Prefilter / Bloom-style filter

- **What.** A cheap, approximate pre-check that quickly rules out regions of the
  text that *cannot* contain any pattern, so the expensive matcher only runs where
  it might pay off. ClamAV: the root's `filter` and `filter_search_ext`.
- **How.** A compact bitset is precomputed from short fragments (e.g. byte
  n-grams) of all patterns. Slide it over the text; where no fragment ever
  matches, skip ahead. A **safety tail** of `maxpatlen` bytes is always scanned so
  a pattern straddling a skip boundary is never missed.
- **Why it works.** Like a **Bloom filter**, it can say "definitely absent here"
  cheaply and only occasionally says "maybe present" (a false positive just means
  you run the real matcher there — never a missed detection). It trims the raw
  bytes the AC/BM engines have to touch, which is where real scans spend time.

---

<a name="part-e"></a>
## Part E — Structural concepts

<a name="compile-once"></a>
### Compile-once / scan-many

- **What.** Parse the human-readable signature database **once** at load time and
  **compile** it into fast in-memory structures (AC trie, BM shift tables, hash
  tables, compiled regexes). Then each file scan only *walks* those structures.
- **Why it's the most important design choice.** Parsing text per file would dwarf
  the actual matching cost. By paying the build cost once, the per-file hot path
  stays tight. (Reference: the `cli_load*` family in `readdb.c` builds; the
  `*_scanbuff` family scans.)

<a name="root"></a>
### The matcher "root" (in-memory database)

- **What.** One aggregate object holding *all* compiled matcher state for one
  **target file type** — BM tables + AC trie + hash tables + prefilter + PCRE
  table + byte-compare table + fuzzy map + tuning flags. Reference:
  `struct cli_matcher`.
- **Why by file type.** The engine keeps several roots (`engine->root[i]`) and, at
  scan time, consults the root for the detected type **plus** a generic root. This
  means a PDF is only tested against PDF-relevant and generic signatures, not
  against (say) ELF signatures — a scalability win.

<a name="context"></a>
### Scan context (threaded state)

- **What.** A single structure passed through every function on the scan path that
  carries *all* per-scan state: the engine, current byte source, options, counters,
  limits, evidence, cancellation flag. Reference: `cli_ctx` in `others.h`.
- **Why.** **No globals on the hot path.** The engine itself is read-only and
  shared; all mutable state lives in the context. That is what makes the scanner
  **thread-safe** (many scans at once over one engine) and makes limits and
  "abort now" behave uniformly everywhere.

<a name="fmap"></a>
### fmap (byte-source abstraction)

- **What.** A uniform handle for "the bytes being scanned," whether they are a
  file on disk (memory-mapped), an in-RAM buffer, or a **window into a parent**
  byte source. Reference: `fmap_t`.
- **Why.** Matchers ask the fmap for bytes rather than touching file descriptors.
  The identical matcher code therefore scans a disk file, a buffer, or a nested
  region inside an archive — and the OS can lazily page in only the bytes actually
  examined.

<a name="recursion"></a>
### Bounded recursion stack

- **The problem.** Containers nest: a ZIP holds a TAR holds an EXE holds an
  embedded object… A malicious file can nest *very* deeply on purpose (a "zip
  bomb"/"recursion bomb") to crash or hang the scanner.
- **The strategy.** Model nesting with an **explicit stack** on the context
  (`recursion_stack[]`, `recursion_level`, bounded by
  `engine->max_recursion_level`), not with C function recursion. Each time the
  scanner opens a child object it **pushes** a new layer (a fresh fmap over the
  extracted bytes) and **pops** on return.
- **Why it works / why it's safer.** Depth is limited by data-driven policy rather
  than the C call stack, so deeply nested input cannot overflow the stack, and the
  engine can enforce a hard depth/size/time budget. This is a core *hardening*
  pattern, not just an implementation detail.

<a name="mpool"></a>
### Memory pool (arena allocation)

- **What.** Allocate many small long-lived objects (the compiled signatures) from
  one big pre-reserved region (a **pool/arena**) instead of thousands of
  individual `malloc`s. Reference: `mpool_t`, enabled under `USE_MPOOL`.
- **Why.** The signature set is huge and lives for the whole engine lifetime.
  Pooling cuts allocation overhead, reduces fragmentation, and lets the engine
  free everything at once on teardown — faster load and shutdown.

<a name="errcodes"></a>
### Error-code control flow

- **What.** Every function returns a status code (`cl_error_t`), and control flow
  is driven by it: `CL_CLEAN`/`CL_SUCCESS`, `CL_VIRUS` (matched — may keep scanning
  to gather *all* names), `CL_BREAK`, and resource/limit errors (`CL_EMEM`,
  `CL_EMAXSIZE`, `CL_ETIMEOUT`).
- **Why.** In a C engine with no exceptions, a disciplined return-code convention
  is how layers communicate "matched," "stop," "out of memory," or "over the
  limit" cleanly. A "saved result" idiom lets a stage remember a pending
  virus/type verdict while a later stage still runs. Sleelavirin mirrors the idea
  with `sleelavirin_match_t.kind` + a confidence score.

---

<a name="part-f"></a>
## Part F — How the pieces fit: a worked mental model

Imagine scanning one file, end to end, using the terms above:

1. **Identify the type** (`filetypes.c`) → choose the matching **root** (§root)
   plus the generic root.
2. Wrap the bytes in an **fmap** (§fmap) and push a layer on the **recursion
   stack** (§recursion).
3. Run the **prefilter** (§prefilter) to find where any pattern could even start;
   skip the dead zones (keep a `maxpatlen` safety tail).
4. Over the surviving bytes, run **Boyer–Moore** (§BM) for quick literal hits,
   then **Aho–Corasick** (§AC) to test *all* literal patterns in one pass,
   incrementing **subsignature** counters (§logical-sigs).
5. Run **byte-compare** (§byte-compare) and **fuzzy hash** (§fuzzy) checks; finally
   run **PCRE** (§PCRE) for the expensive regex subsigs.
6. Evaluate each **logical signature's** boolean expression over the collected
   counters and metadata (§logical-sigs). Separately, compute the file's
   **SHA-256** and probe the **hash table** (§hashtable, §open-addressing) for an
   exact known-bad fingerprint.
7. Any hit → record the threat name via the engine's "append virus" step, carried
   on the **scan context** (§context) and reported through an **error code**
   (§errcodes). Pop the recursion layer and continue with any sibling/child
   objects.

**The throughline:** compile everything once (§compile-once) into type-specific
roots, then make *one* cheap pass per byte using automata (AC/BM) and O(1) hash
probes, guarded by a prefilter, bounded by an explicit recursion stack, with all
state on a single thread-safe context. That combination is what lets a scanner
check millions of signatures against large files quickly **and** safely — and it
is the design Sleelavirin follows at small scale, deferring full signature
coverage to ClamAV as an external provider.
