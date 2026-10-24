# The Philosophers Constructors

**Package:** `lib/philosophers`
**Spec surface:** `#sleela 2.1`
**Register:** IQ 201+
**Companion:** [`PHILOSOPHERS.CATEGORICAL.md`](PHILOSOPHERS.CATEGORICAL.md) — the
categorical definition of the 21 Guides and the orthogonal Ordain axis.

This document is about **the Constructors**: the classes that *are* a perfect
Constructor, how you declare and call one, the lifecycle of a construction, and
the full method surface of each class. Where the Categorical document defines
the *vocabulary*, this one defines the *machinery*.

---

## 1. What a Constructor is here

In ordinary procedural design a constructor is a single, static act: call it,
get an object. In the philosophers package a Constructor is **of nature and
time** — a bounded *event* with a lifecycle. It seats a finite set of **Guides**
(declared with the `::` growth separator), opens a **time-bounded contract**,
lets a **Favor Mechanism** decide whether it may run, and — only if favor is
granted — runs, disposes its Guides, and liberates the object so it **lives free**
of the instantiative moment.

```
Constructor() :: Patch :: Seed :: Aim :: Guide
       ^            \______________________/
   the root           the Guides (growths), each colored Gold or Green
   (real call)
```

The six classes that make this work:

| Class | One-line role |
|-------|---------------|
| `SLPhilosopherConstructor` | the perfect Constructor — the orchestrator you call |
| `SLPhilosopherChain` | owns the ordered Guides declared with `::` |
| `SLPhilosopherGuide` | one `::` growth; a Wisdom with a Gold/Green impart |
| `SLPhilosopherSeating` | the finite register (21 base + band `[-2,+6]`, 7 categories) |
| `SLFavorMechanism` | catches Notice, grants/withholds runnability |
| `SLConstructorContract` | the ≤ ~2-minute moment; the whistle ticks ahead; then free |
| `SLOrdain` | *(orthogonal)* the ordinality — rank/category/crown across all 21 |

---

## 2. Declaring and calling a Constructor

A Constructor is built in-language from names; the `::` chain is produced for
display and tooling (char-level string parsing is a bridged capability, so the
chain is assembled, not parsed). The canonical lifecycle call is:

```sleela
< the perfect Constructor >
SLPhilosopherConstructor ctor = new SLPhilosopherConstructor();
ctor.create();

< admit a signed seating band in [-2,+6] (here +3 -> capacity 24) >
ctor.admit(3);

< seat Guides: (name, seating ordinal, carriesFavor) >
ctor.guide("Patch", 3, true);    < Inception, Gold  >
ctor.guide("Seed",  4, false);   < Inception, Green >
ctor.guide("Aim",   6, true);    < Intent,    Gold  >
ctor.guide("Guide", 12, true);   < Governance, Gold >

< require at least N caught Notices before favor is granted >
ctor.requireNotice(2);

< the facts/sayings of Approach, broadcast to the Guides as imparts >
String[] imparts = new String[4];
imparts[0] = "Patch";
imparts[1] = "Aim";
imparts[2] = "Guide";
imparts[3] = "Approach";

< run the instantiative moment for 90s (within the ~2-minute bound) >
boolean alive = ctor.run(imparts, 90);

print(ctor.report());
```

`ctor.chainText()` then renders `Constructor() :: Patch :: Seed :: Aim :: Guide`,
and `ctor.run(...)` returns `true` when favor was granted and the object now
lives free.

---

## 3. The construction lifecycle

```
            SLPhilosopherConstructor.run(imparts, seconds)
                            |
   create() --------------->|  chain, favor, contract are fresh
   admit(band) ------------>|  seating capacity fixed in [19,27]
   guide(name,seat,favor) ->|  Guides seated in order (<= capacity)
                            |
            +===============v=================================+
            | open the contract (SLConstructorContract.open)  |  moment begins
            |         |                                       |
            | broadcast each impart to the Guides             |  Guides listen()
            |         |                                       |
            | catch matched Notice (SLFavorMechanism)         |  Notice tallied
            |         |                                       |
            | weigh favor (quorum)                            |  verdict rendered
            |         |                                       |
            |   favor withheld -> sealMoment(), return false  |  honest refusal
            |         |                                       |
            |   favor granted:                                |
            |     tickAhead(clampSeconds(seconds))  <= 120s   |  the whistle, ahead
            |     sealMoment()                                |  moment closes
            |     disposeAll() Guides (final effect)          |
            |     liberate() -> object lives free             |  returns true
            +=================================================+
```

Key guarantees:

- **Time-bounded.** The contract caps the moment at `MAX_SECONDS = 120`. The
  clock only ever advances (`tickAhead` ignores backward/zero ticks — the
  perpetual whistle of time, *ahead*).
- **Honest runnability.** Favor is granted only when caught Notice meets the
  quorum. A withheld verdict stops construction with a precise state, never a
  silent fake success.
- **Lives free.** After the moment closes, `liberate()` releases the object; it
  outlives the instantiative moment that made it.
- **Idempotent disposal.** Each Guide disposes once; its matched tally is its
  output of record.

---

## 4. Class reference

### 4.1 `SLPhilosopherConstructor` — the perfect Constructor

The orchestrator you call. Composes a chain, a favor mechanism, and a contract.

| Method | Returns | Purpose |
|--------|---------|---------|
| `create()` | `void` | prepare an empty Constructor (fresh chain / favor / sealed contract) |
| `admit(band)` | `int` | admit a signed seating band in `[-2,+6]`; returns the band admitted |
| `guide(name, seatOrdinal, carriesFavor)` | `int` | seat one Guide; returns its index, or `-1` at capacity |
| `requireNotice(quorum)` | `void` | set how much Notice favor requires (min 1) |
| `run(imparts, seconds)` | `boolean` | run the moment; `true` iff favor granted and object lives free |
| `constructed()` | `boolean` | did it run to completion and now live free |
| `chainText()` | `String` | the canonical `::` chain text |
| `colorManifest()` | `String` | the Gold/Green impart of each Guide, `::`-joined |
| `verdict()` | `String` | the favor verdict text |
| `phase()` | `String` | the contract phase (`sealed`/`open`/`closed`/`free`) |
| `capacity()` / `guides()` | `int` | admitted capacity / number of seated Guides |
| `report()` | `String` | one-line quality/effect/output summary |

### 4.2 `SLPhilosopherChain` — the `::` growth chain

| Member | Returns | Purpose |
|--------|---------|---------|
| `ROOT` | `String` | the chain head, `"Constructor()"` |
| `GROWTH` | `String` | the growth separator, `" :: "` |
| `create()` | `void` | prepare an empty chain at default (band 0 → 21) capacity |
| `admit(band)` | `int` | admit the seating band (delegates to the register) |
| `seat(name, seatOrdinal, carriesFavor)` | `int` | seat one Guide; `-1` at capacity (sets `refused`) |
| `at(i)` | `SLPhilosopherGuide` | the Guide at index `i`, or `null` |
| `broadcast(impart)` | `int` | send an impart to every Guide; returns how many matched |
| `disposeAll()` | `void` | dispose every Guide (idempotent) |
| `render()` | `String` | the canonical `::` chain text |
| `colorManifest()` | `String` | the color imparts, `::`-joined |
| `size()` / `capacity()` / `refused()` | `int`/`int`/`boolean` | state |

### 4.3 `SLPhilosopherGuide` — one growth / Wisdom

| Member | Returns | Purpose |
|--------|---------|---------|
| `COLOR_NONE` / `COLOR_GOLD` / `COLOR_GREEN` | `int` | the impart colors |
| `create()` | `void` | prepare an empty, uncolored Guide |
| `sit(name, seatOrdinal, carriesFavor)` | `void` | name the Guide onto a seating with its favor |
| `color()` | `int` | the impart color — Gold if approving, else Green (idempotent) |
| `colorName()` | `String` | `"Gold/Yellow"` / `"Green"` / `"none"` |
| `listen(impart)` | `boolean` | match an impart at call time; counts the match |
| `dispose()` | `void` | stop listening; keep the matched tally (idempotent) |
| `isApproving()` / `isDisposed()` / `getMatched()` | `boolean`/`boolean`/`int` | state |

### 4.4 `SLPhilosopherSeating` — the finite register

| Member | Returns | Purpose |
|--------|---------|---------|
| `BASE_SEATINGS` = 21 | `int` | the base number of seatings |
| `ADDITIONAL_MIN` = -2, `ADDITIONAL_MAX` = 6 | `int` | the signed admission band |
| `MIN_CAPACITY` = 19, `MAX_CAPACITY` = 27 | `int` | the admitted-capacity bounds |
| `SEAT_*` (0..20) | `int` | the 21 seating ordinals (Approach..Approval) |
| `CAT_*` (0..6) | `int` | the 7 category ordinals |
| `admit(band)` | `int` | clamp + admit a band; returns the band admitted |
| `seats(ordinal)` | `boolean` | is the ordinal a real seat in current capacity |
| `seatName(ordinal)` | `String` | the Guide name of a seating |
| `category(ordinal)` | `int` | the category (0..6) of a seating (`ordinal / 3`) |
| `categoryName(cat)` / `seatCategoryName(ordinal)` | `String` | category names |

### 4.5 `SLFavorMechanism` — runnability

| Member | Returns | Purpose |
|--------|---------|---------|
| `FAVOR_WITHHELD` / `FAVOR_GRANTED` | `int` | the verdict codes |
| `create()` | `void` | prepare with quorum 1, verdict withheld |
| `setQuorum(q)` | `void` | the least Notice favor requires (min 1) |
| `catchNotice(saying)` | `void` | catch one fact/saying of Approach |
| `weigh()` | `int` | render the verdict against the quorum |
| `granted()` | `boolean` | was favor granted |
| `verdictText()` | `String` | `"favor granted"` / `"favor withheld"` |
| `reset()` | `void` | reset Notice/verdict, keep the quorum |

### 4.6 `SLConstructorContract` — the moment of nature and time

| Member | Returns | Purpose |
|--------|---------|---------|
| `MAX_SECONDS` = 120 | `int` | the two-minute boundary |
| `PHASE_SEALED/OPEN/CLOSED/FREE` | `int` | the phases of the moment |
| `open()` | `void` | open the moment; clock to zero |
| `tickAhead(seconds)` | `int` | advance the clock forward only; auto-close at the bound |
| `clampSeconds(seconds)` | `int` | clamp a duration into `[1, MAX_SECONDS]` |
| `withinContract()` | `boolean` | still open and under the bound |
| `sealMoment()` | `void` | close the moment deliberately (not `close()` — reserved) |
| `liberate()` | `boolean` | free the object (only from a closed moment) |
| `livesFree()` / `getClock()` / `getPhase()` / `phaseText()` | — | state |

### 4.7 `SLOrdain` — the orthogonal ordinality (summary)

Not a Constructor and not a seat. It reads the *order* of the register: `rank`,
`ordainingCategory`, `toCrown` / `CROWN = 20`, and `reading(ordinal)`. Full
treatment in [`PHILOSOPHERS.CATEGORICAL.md`](PHILOSOPHERS.CATEGORICAL.md) §4.

---

## 5. Worked example output

Running `philosophers-demo.sleela` with the Constructor from §2:

```
=== Philosophers: the perfect Constructor and its Guides ===
admitted seating band: 3   capacity: 24
declared chain: Constructor() :: Patch :: Seed :: Aim :: Guide
color manifest: Gold/Yellow :: Green :: Gold/Yellow :: Gold/Yellow
ordain(Aim):   Aim: rank 6, category Intent, 14 below the crown (Approval)
ordain(Guide): Guide: rank 12, category Governance, 8 below the crown (Approval)
ordain(crown): Approval: rank 20, category Closure, 0 below the crown (Approval)
verdict: favor granted
constructed & lives free: true
report: chain=[Constructor() :: Patch :: Seed :: Aim :: Guide]  guides=4/24  favor granted  phase=free  t=90s  colors=[Gold/Yellow :: Green :: Gold/Yellow :: Gold/Yellow]
the object now lives free of its instantiative moment.
done
```

---

## 6. Authoring notes

- **One class per file**, each carrying the Metadata Class metapattern and
  angle-bracket (`<` / `<<`) comments — the go-forward house style.
- **`sealMoment()` not `close()`**: a bare `close()` collides with the reserved
  Munction reach-handle built-in.
- **Typed local for array-element methods**: call a Guide method via a local
  (`SLPhilosopherGuide g = guides[i]; g.listen(...)`), not inline on the
  subscript, so the element type resolves at runtime.
- **Validate** with the source-built compiler (2.2-capable):
  `sleela check lib/philosophers/philosophers-demo.sleela` and
  `sleela run lib/philosophers/philosophers-demo.sleela`; keep the SHA-256
  manifest regenerated after edits (`make check-manifest` must print OK).

---

*Kept in lockstep with `lib/philosophers/*.sleela`. The source is the authority;
where this document and the source disagree, the source wins.*
