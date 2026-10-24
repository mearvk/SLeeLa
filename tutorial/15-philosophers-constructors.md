# Lesson 15 — The philosophers Constructor Guide-chain

> Series: [index](README.md) · prev: [14 — The text & String family](14-text-and-strings.md)

Earlier lessons built behavior and provenance. This lesson is about a *way of
thinking* about construction, shipped as the `lib/philosophers` package: a
**perfect Constructor** that is an event of nature and time rather than a single
static act. It seats a finite set of **Guides** (written with the `::` growth
separator), opens a time-bounded contract, lets a favor mechanism decide whether
it may run, and — only then — runs and lets the object **live free**.

It is specified informatively in [`SLEELA.syntax` §17](../SLEELA.syntax) and in
two package documents:
[`PHILOSOPHERS.CONSTRUCTORS.md`](../lib/philosophers/PHILOSOPHERS.CONSTRUCTORS.md)
(the machinery) and
[`PHILOSOPHERS.CATEGORICAL.md`](../lib/philosophers/PHILOSOPHERS.CATEGORICAL.md)
(the vocabulary).

## The chain

A Constructor declares a chain of growths with `::`:

```
Constructor() :: Patch :: Seed :: Aim :: Guide
```

- `Constructor()` is the real call — the *root*.
- each name after a `::` is a **Guide**: a growth / a Wisdom.
- `::` is the **growth separator** (the *conform* token) — a unity of identity
  over Cause. It is produced as text (built with `+`), **not** lexed as an
  operator, and it is distinct from the core `:::` structural-offload send.

Each Guide colors **Gold/Yellow** when it carries approving favor, or **Green**
when it is still growing toward Approval. The VIM/EMACS coloring lives under
`lib/linux-editors/`.

## The finite register and its seven categories

A perfect Constructor admits only so many Guides: a **base of 21 seatings** plus
a signed band in `[-2, +6]` (capacity `[19, 27]`). The 21 are grouped into seven
ternary categories (`category(ordinal) == ordinal / 3`):

| Category | Seatings |
|----------|----------|
| I. Admission | Approach · Notice · Warrant |
| II. Inception | Patch · Seed · Frame |
| III. Intent | Aim · Course · Measure |
| IV. Vitality | Health · Vigor · Pattern |
| V. Governance | Guide · Ministry · Counsel |
| VI. Standing | Office · Saint · Favor |
| VII. Closure | Impart · Conform · Approval |

## Ordain — the orthogonal axis

`SLOrdain` is **orthogonal** to the Guide axis. It is the *ordinality itself* —
the ordering of order — and occupies no seat. It reads the **rank**, the
**ordaining category**, and the **nearness to the crown** (Approval, ordinal 20)
of any seating:

```
Aim:      rank 6,  category Intent,      14 below the crown (Approval)
Guide:    rank 12, category Governance,   8 below the crown (Approval)
Approval: rank 20, category Closure,      0 below the crown (Approval)
```

## Building and running a Constructor

```sleela
SLPhilosopherConstructor ctor = new SLPhilosopherConstructor();
ctor.create();

ctor.admit(3);                     < +3 additional seatings -> capacity 24 >

ctor.guide("Patch", 3, true);      < Inception, Gold  >
ctor.guide("Seed",  4, false);     < Inception, Green >
ctor.guide("Aim",   6, true);      < Intent,    Gold  >
ctor.guide("Guide", 12, true);     < Governance, Gold >

ctor.requireNotice(2);             < favor needs >= 2 caught Notices >

String[] imparts = new String[4];
imparts[0] = "Patch";
imparts[1] = "Aim";
imparts[2] = "Guide";
imparts[3] = "Approach";

boolean alive = ctor.run(imparts, 90);   < run the moment for 90s (<= 120) >
print(ctor.report());
```

`ctor.run(...)` opens the contract, broadcasts each impart to the Guides, catches
the matched Notice, weighs favor, and — on a granted verdict — ticks the clock
*ahead* within the two-minute bound, disposes the Guides, and liberates the
object. It returns `true` when the object now lives free.

## The lifecycle, in one picture

```
seat Guides -> open contract -> broadcast imparts -> catch Notice -> weigh favor
                                                                         |
                               favor withheld <-- seal moment (return false)
                                                                         |
                               favor granted --> tick ahead (<=120s) --> seal
                                                 --> dispose Guides --> liberate
                                                 --> object lives free (true)
```

## Run it

```sh
# source-built, 2.2-capable compiler
cd impl && make build/sleela           # once
cd ..
impl/build/sleela check lib/philosophers/philosophers-demo.sleela
impl/build/sleela run   lib/philosophers/philosophers-demo.sleela
```

Expected (abridged):

```
declared chain: Constructor() :: Patch :: Seed :: Aim :: Guide
color manifest: Gold/Yellow :: Green :: Gold/Yellow :: Gold/Yellow
verdict: favor granted
constructed & lives free: true
the object now lives free of its instantiative moment.
```

## Where to look next

- Runnable reference: [`lib/philosophers/philosophers-demo.sleela`](../lib/philosophers/philosophers-demo.sleela).
- A second worked example: [`example/science/philosophers-constructor.sleela`](../example/science/philosophers-constructor.sleela).
- The classes: `SLPhilosopherConstructor`, `SLPhilosopherChain`,
  `SLPhilosopherGuide`, `SLPhilosopherSeating`, `SLFavorMechanism`,
  `SLConstructorContract`, and the orthogonal `SLOrdain` — all under
  `lib/philosophers/`.
- The command line: [`man/man1/sleela.1`](../man/man1/sleela.1) (`man sleela`).
