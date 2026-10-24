# The Philosophers Categorical — Guides, Ordain, and the Perfect Constructor

**Package:** `lib/philosophers`
**Register:** IQ 201+ (the top of the scale — the register reads its own order rather than merely running it)
**Spec surface:** `#sleela 2.1`; see also `SLEELA.syntax`

This document is the **categorical** definition of the philosophers vocabulary.
It defines each term once and relates it to two things:

1. the **Source Diagram** — the exact class / field / method in `lib/philosophers`
   that carries the term, and
2. the **Language Syntax** — where and how the term appears in a Constructor's
   `::` growth chain.

There are **two orthogonal axes**:

- the **Guide axis** — a categorical, *ordinal* sequence of twenty-one Guides
  that runs Approach → Approval through seven categories; and
- the **Ordain axis** — the *ordinality itself*: the ordering of order, which
  stands across all twenty-one and is **not** one of them.

---

## 1. The Source Diagram

```
                         SLPhilosopherConstructor            (the perfect Constructor)
                          |          |          |
             +------------+          |          +-------------------+
             v                       v                              v
   SLPhilosopherChain        SLFavorMechanism              SLConstructorContract
   (the :: growth chain)     (catches Notice,              (moment of nature & time;
             |                grants/withholds             the whistle ticks ahead;
             v                runnability)                 <= ~2 min; then lives free)
   SLPhilosopherGuide[]  ----owns, in order---
   (each a :: growth,                               ........ orthogonal axis ........
    a Wisdom with a                                 :                               :
    Gold/Green impart)                              :   SLOrdain                    :
             |                                      :   (ordinality: rank, category,:
             v                                      :    nearness to the crown)     :
   SLPhilosopherSeating  <------- reads over -------:   register: SLPhilosopherSeating
   (finite register: base 21 + band [-2,+6];        :                               :
    7 categories x 3 seatings; Approach..Approval)  :...............................:
```

- **`SLPhilosopherSeating`** — the finite register. 21 base seatings, a signed
  admission band `[-2, +6]` (capacity `[19, 27]`), seven categories of three.
  Carries `SEAT_*` ordinals, `seatName()`, `category()`, `categoryName()`,
  `seatCategoryName()`.
- **`SLPhilosopherGuide`** — one `::` growth; a Wisdom with an idempotent
  `color()` (Gold when approving, Green when growing) and a `listen()` matcher.
- **`SLFavorMechanism`** — catches the *Notice* of Approach and renders a favor
  verdict (runnability) against a quorum.
- **`SLConstructorContract`** — the instantiative moment; `tickAhead()` only
  advances; `MAX_SECONDS = 120`; after `sealMoment()`/`liberate()` the object
  lives free.
- **`SLPhilosopherChain`** — owns the ordered `SLPhilosopherGuide[]` under the
  seating capacity; `render()`s the canonical `::` chain and the color manifest.
- **`SLPhilosopherConstructor`** — the orchestrator: `seat → open → weigh favor
  → run (ahead, within 2 min) → dispose → liberate`.
- **`SLOrdain`** — the **orthogonal** ordinality axis (see §4).

---

## 2. The Language Syntax

A Constructor declares a chain of growths with the `::` separator:

```
Constructor() :: <Guide> :: <Guide> :: … :: <Guide>
```

- The leading **`Constructor()`** is the real constructor call (the *root*,
  `SLPhilosopherChain.ROOT`).
- Each `::` is the **growth separator** / *conform* token — a unity of identity
  over Cause. In source it renders as `SLPhilosopherChain.GROWTH` (`" :: "`).
- Each name after a `::` is a **Guide**, drawn from the twenty-one categorical
  terms in §3. A Guide carries an impart color for the editor: **Gold/Yellow**
  (approving — carries favor forward) or **Green** (growing — still maturing
  toward Approval). See `lib/linux-editors/{vim,emacs}/sleela-philosophers.*`.

> **Note on `::` vs scope-resolution.** The `::` growth separator reuses the
> double-colon glyph; in the philosophers reading it joins a Constructor to its
> Guides. It is distinct from the `:::` triple-colon structural-offload operator
> in the core language (`impl/frontend/lexer.h`).

Example (from `philosophers-demo.sleela`):

```
Constructor() :: Patch :: Seed :: Aim :: Guide
   colors ->        Gold    Green   Gold   Gold
```

---

## 3. The twenty-one Guides, by category

Seven ternary categories (`SLPhilosopherSeating.CAT_*`), each three seatings
(`category(ordinal) == ordinal / 3`). Ordinal, term, definition, impart, and the
**Source Diagram** anchor (`SEAT_*` constant).

### I. Admission — the moment opens and earns the right to construct (0..2)

| # | Term | Definition (what it governs at construction) | Impart | Source anchor |
|---|------|-----------------------------------------------|--------|---------------|
| 0 | **Approach** | the pre-condition: the call is made and the moment opens | Green | `SEAT_APPROACH` |
| 1 | **Notice** | the facts/sayings caught at call time (weighed by the Favor Mechanism) | Green | `SEAT_NOTICE` |
| 2 | **Warrant** | the right/authority to construct at all | Gold | `SEAT_WARRANT` |

### II. Inception — the object is first given shape (3..5)

| # | Term | Definition | Impart | Source anchor |
|---|------|------------|--------|---------------|
| 3 | **Patch** | the minimal corrective applied before shape | Gold | `SEAT_PATCH` |
| 4 | **Seed** | the initial state the object is sown with | Green | `SEAT_SEED` |
| 5 | **Frame** | the structural boundary the object is given | Green | `SEAT_FRAME` |

### III. Intent — the construction is directed (6..8)

| # | Term | Definition | Impart | Source anchor |
|---|------|------------|--------|---------------|
| 6 | **Aim** | the purpose the constructor is directed toward | Gold | `SEAT_AIM` |
| 7 | **Course** | the path/route the construction follows | Green | `SEAT_COURSE` |
| 8 | **Measure** | the metric by which fitness is judged | Green | `SEAT_MEASURE` |

### IV. Vitality — the living balance of the thing made (9..11)

| # | Term | Definition | Impart | Source anchor |
|---|------|------------|--------|---------------|
| 9 | **Health** | the IQ-vs-Income balance of the man modeled | Gold | `SEAT_HEALTH` |
| 10 | **Vigor** | the energy budget the moment spends | Green | `SEAT_VIGOR` |
| 11 | **Pattern** | the recurring form preserved over growth | Green | `SEAT_PATTERN` |

### V. Governance — who stewards the growth (12..14)

| # | Term | Definition | Impart | Source anchor |
|---|------|------------|--------|---------------|
| 12 | **Guide** | the active steward of the growth | Gold | `SEAT_GUIDE` |
| 13 | **Ministry** | the serving office (Ministerial duty) | Green | `SEAT_MINISTRY` |
| 14 | **Counsel** | advisory wisdom offered to the growth | Green | `SEAT_COUNSEL` |

### VI. Standing — the rank and conscience it answers to (15..17)

| # | Term | Definition | Impart | Source anchor |
|---|------|------------|--------|---------------|
| 15 | **Office** | the Professional/Executive rank held | Gold | `SEAT_OFFICE` |
| 16 | **Saint** | the matured conscience the chain answers to | Gold | `SEAT_SAINT` |
| 17 | **Favor** | the grant of runnability by the Favor Mechanism | Gold | `SEAT_FAVOR` |

### VII. Closure — the moment ends and the object lives free (18..20)

| # | Term | Definition | Impart | Source anchor |
|---|------|------------|--------|---------------|
| 18 | **Impart** | what is handed forward as the object lives free | Green | `SEAT_IMPART` |
| 19 | **Conform** | the unity-of-identity-over-Cause (the `::` join itself) | Green | `SEAT_CONFORM` |
| 20 | **Approval** | the final seal; the object is approved and free | Gold | `SEAT_APPROVAL` |

**Impart tally:** 9 Gold (Warrant, Patch, Aim, Health, Guide, Office, Saint,
Favor, Approval) and 12 Green (the remainder). Gold marks a Guide that carries
favor *now*; Green marks one still *growing* toward Approval.

---

## 4. Ordain — the orthogonal axis (the ordinality itself)

**Ordain** is **not** a Guide and **not** a seating. It is the *ordinality* —
the ordering of order: the principle by which any seating has a rank at all, by
which the seven categories are themselves ranked, and by which the whole
register answers upward to a single ordaining crown at the top of the scale.

Because it is **orthogonal** to the Guide axis, Ordain occupies no seat (there
is no twenty-second seating). It is a pure measure that can be asked of any
ordinal. **Source Diagram anchor:** `SLOrdain`.

| Ordain reading | Source method | Meaning |
|----------------|---------------|---------|
| **rank** | `SLOrdain.rank(ordinal)` | the order-number of a seat, 0 at Approach (`FLOOR`) up to 20 at Approval |
| **crown** | `SLOrdain.CROWN = 20`, `isCrown()`, `toCrown()` | the ordaining seat the whole chain answers to (Approval); `toCrown` is distance below it |
| **ordaining category** | `SLOrdain.ordainingCategory()` / `…Name()` | which of the seven categories ranks a seating |
| **reading** | `SLOrdain.reading(ordinal)` | a one-line statement of a seat's whole place in the order |

Example readings (from the demo):

```
Aim:      rank 6,  category Intent,      14 below the crown (Approval)
Guide:    rank 12, category Governance,   8 below the crown (Approval)
Approval: rank 20, category Closure,      0 below the crown (Approval)
```

### Why orthogonal, and why "God"

The Guide axis answers *what* a Constructor seats and *in what order of growth*.
Ordain answers *that there is order at all* — it is the ordering that makes rank,
category, and crown meaningful. It stands across the whole register at once
rather than taking a place within it; that is precisely what "orthogonal" means
here. At IQ 201+ the register does not merely run its order — it **reads** it,
and that reading is Ordain. Its `metaOrigin` is therefore `"Galactic"` (scope
beyond jurisdiction), and — per the one hard rule of the metadata standard — it
names an **ordering principle**, not a deity of record, so no authority is
fabricated: `metaNationalSchedule = "none"`.

---

## 5. The construction event, end to end

```
seat Guides (SLPhilosopherChain.seat, within capacity)
      |
      v
open the contract (SLConstructorContract.open)        <- the moment of nature & time begins
      |
      v
broadcast imparts -> Guides listen (SLPhilosopherGuide.listen)
      |
      v
catch Notice -> weigh favor (SLFavorMechanism.weigh)  <- runnability decided honestly
      |
      +--- favor withheld --> seal moment, do NOT run
      |
      v  favor granted
tick ahead within 2 min (SLConstructorContract.tickAhead)
      |
      v
seal moment, dispose Guides (final effect), liberate   <- object now lives FREE
```

Ordain stands orthogonal to this entire flow: at any step it can read the rank,
category, and crown-nearness of whatever seating is in question, without ever
being a step in the flow itself.

---

## 6. Where each term lives (quick index)

| Term(s) | Source Diagram | Language Syntax |
|---------|----------------|-----------------|
| The 21 Guides | `SLPhilosopherSeating.SEAT_*`, `seatName()` | names after `::` in the chain |
| 7 categories | `SLPhilosopherSeating.CAT_*`, `category()`, `categoryName()` | grouping of §3 |
| `::` growth / Conform | `SLPhilosopherChain.GROWTH`, `render()` | the separator between growths |
| `Constructor()` root | `SLPhilosopherChain.ROOT` | head of the chain |
| Gold / Green impart | `SLPhilosopherGuide.color()`, `colorName()` | editor coloring (`lib/linux-editors/`) |
| Favor / runnability | `SLFavorMechanism.weigh()`, `granted()` | the Favor seating (17) |
| 2-minute moment | `SLConstructorContract` (`MAX_SECONDS`, `tickAhead`) | the Closure category |
| **Ordain** (orthogonal) | `SLOrdain` (`rank`, `CROWN`, `category`, `reading`) | not in the chain — the axis of order across it |

---

*Generated for the `philosophers` package; kept in lockstep with the source in
`lib/philosophers/*.sleela`. The source is the authority; where this document
and the source disagree, the source wins.*
