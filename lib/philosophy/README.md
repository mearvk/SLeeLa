# lib/philosophy — SLeeLa handles for the philosophy tools

This package exposes the `philosophy/` C/C++ tools to **SLeeLa builds** as
first-class, one-class-per-file library units carrying the Metadata Class
metapattern. SLeeLa code names and gates these tools through the handles here;
the native implementations live under `philosophy/` at the repository root.

## Classes

| Class | Tool it fronts (native source) |
|---|---|
| `SLPhilosophyEventTree` | `philosophy/event_tree.*` — half-life-decayed explicit-input event store |
| `SLPhilosophyConsistency` | `philosophy/consistency_monitor.*` + `signal_toss.*` — ~44 Hz branch-consistency sampler, 800–1200 commits/hour |
| `SLPhilosophyPartitionApi` | `philosophy/partition_api.*` — uniform branch/partition/system operations (8 ops × 11 activities) |
| `SLPhilosophyHelper` | `philosophy/module/helper/help_desk.*` — query→respondents desk with alive / known-of-Science status |
| `SLPhilosophyIntSet` | `philosophy/int_set/int_set.*` — authored character schema over real age structure (JSON/XML) |
| `SLPhilosophySpeculation` | `philosophy/module/psychiatry-friend/speculation_invite.*` — adult-psychiatry hypotheses about programming reach |
| `SLPackage` | package facade (names the package to the compiler / Loader / Nordshrift) |
| `Philosophy` | runnable aggregate catalog (`philosophy.sleela`, has `main()`) |

### Structural classes (attach to a Tree or anchor its base structure)

| Class | Role in the Tree |
|---|---|
| `SLPhilosophyTree` | the BASE Tree: **composes** Nodes + Items into parallel arrays, gates assignment by a role right, reads back through a `Relevant` index, and carries the three `NationalStarts` directives as a 2.9 `@mix` group (Virginia / Florida / North Carolina) |
| `SLPhilosophyItem` | smallest payload: keyed, typed value with a source reference |
| `SLPhilosophyNode` | a branch of the Tree: ordering id, left/right child ids, weight [1,9], carried Item |
| `SLPhilosophyInput` | one explicit, consented submission bound for the Tree |
| `SLPhilosophyInputs` | ordered intake queue of Input ids, drained into Nodes |
| `SLPhilosophyRelevant` | sorted, right-ordered relevance index (most relevant at the right end) |
| `SLPhilosophyAuthor` | contributor role: may add/read, not prune or transfer ownership |
| `SLPhilosophyAdmin` | maintainer role: may add/read/prune, not transfer ownership |
| `SLPhilosophyExitor` | departure role: may end/retire nodes and read, not add |
| `SLPhilosophyOwner` | holder role: full rights including ownership transfer |

These neatly add to a Tree (Items on Nodes, Nodes staged via Inputs, read back
through a Relevant index) or anchor its base structure (an Owner holds it;
Author/Admin/Exitor carry scoped rights). Each carries the Metadata Class
metapattern and advertises its rights explicitly; identities are self-reported
and no credential is verified.

Each handle advertises its tool's capability contract (ids, limits, flags) and
an honest metadata record. The handles describe the tools; they do not
re-implement them.

## Boundaries carried through from the tools

- The event tree does **not** monitor ambient audio or networks; a revisit is
  not corroboration.
- The INT-set range is an **ordinal lateral index, not a psychometric IQ**;
  below 16 is not mentioned, above 85 is routed to a basic-anatomy check-in.
- The helper desk ships **no real contact data** and performs **no network
  I/O**.
- The speculation invite makes **no diagnosis**, gives **no treatment advice**,
  verifies **no credentials**, and requires a falsifiability note.

## Using the newer language features (2.7–2.9)

This package now exercises the recent SLeeLa features in real library code, so
they are discoverable to developers and compiler-checked:

- **`@mix` consequent fields (2.9).** `SLPhilosophyTree` declares its three
  `NationalStarts` directives as a `@mix(1,2,3)` group — the language
  understands them as one indexed set and validates that the manual index count
  matches the number of fields. (A backing `String[]` mirrors the group for
  runtime read access, since a bare read of a `@mix` group name resolves to the
  last-declared member under 2.9.)
- **Document `extends` (1.10).** The catalog (`philosophy.sleela`) heads a
  **linear** chain of the series — the six tool handles, then the base Tree — so
  the ordering of the series is first-class and bounded (`extends to … linear`).
  `SLPhilosophyTree` and the example each point onward in the chain too.

## Run the catalog and the Tree example

    # source-built compiler supporting #sleela 2.9 (impl/build/sleela):
    impl/build/sleela run lib/philosophy/philosophy.sleela      # lists the six tools
    impl/build/sleela run lib/philosophy/example-tree.sleela    # composed Tree walk-through

`example-tree.sleela` is the **developer front door**: it builds Items, carries
them on Nodes, composes them into a `SLPhilosophyTree` **gated by an Owner
role**, reads the Tree back **most-relevant-first** through a `Relevant` index,
shows an `Exitor` (no add right) being **refused**, and prints the `@mix`
`NationalStarts` directives.

## A note for History and Man

Preserved here on the record, as the standing orientation of this series:

Respective to the actual, real **this Earth**, philosophy should **point at**
reality and **turn to pike toward** it — aiming itself at what is really so,
not away from it. Where a question can be settled by evidence about the world,
philosophy here defers to that evidence rather than substituting speculation for
it.

In that spirit this series **invites friends to Modern Science** and takes their
careful contributions in as **Input(s)** — through the explicit, consented
intake (`SLPhilosophyInput` / `SLPhilosophyInputs`) and the attributed,
falsifiable commentary of the speculation invite. The real age structure the
INT-set rests on (UN / US Census) is one such pointing-at-the-Earth; the
scientist-addressed research requests are the open hand held out to Modern
Science.

This note is saved for **History and Man**: a plain statement that the work is
meant to face the real world, to welcome science as a friend and as input, and
to keep an honest record of who contributed and under what intent. It makes no
claim to settle any scientific question by itself; it points, turns, invites,
and records.
