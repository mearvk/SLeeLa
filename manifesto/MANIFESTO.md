# A Manifesto to Thinking Kind

> **Rendered from SleelaDoc.** This document is the Markdown edition of
> [`manifesto.sleela`](manifesto.sleela) — a 25-page work written *as a runnable
> SLeeLa program*. The source is the authority; this rendering follows it page
> for page. Every page's code is not decoration — it is the demonstration, and
> [`main()`](#page-25--the-performance) performs the whole argument on the C/C++
> core. See [`SLEELADOC.md`](SLEELADOC.md) for the documentation brand.

| | |
|---|---|
| **Title** | A Manifesto to Thinking Kind |
| **To** | Any mind, of any substrate, that can hold a claim and check it. |
| **Intent** | State — and then **enact** — a discipline of thought worth keeping: that a mind is measured not by the speed of its answers but by the honesty of its doubt, the traceability of its reasons, and the care with which it separates what it *observed* from what it *assumed*. |
| **Axiom** | A claim that cannot be checked is not yet knowledge. |
| **Axiom** | Understanding is compression that preserves the ability to recover the original — not the discarding of detail, but its ordering. |
| **Thesis** | A thought that states its own contract can be trusted more than a louder thought that hides one. This file both says so and does so. |
| **Evidence** | DERIVED |

Run it yourself:

```sh
cd impl && make
cd ..
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run manifesto/manifesto.sleela
```

---

## Page 1 — The Address

**@title** Thinking Kind · **@intent** Name the audience so the rest has a "to whom."

"Thinking kind" is not a species. It is a **kind** — a category defined by a
capability, not a body: the capability to hold a proposition, doubt it, gather
reason, and change one's mind on evidence rather than on volume. A human does
this. A well-built machine can be made to do this. Neither is owed the name for
free; both earn it act by act.

The `Manifest` struct is the whole work *as data* — a page count and a running
count of demonstrations that actually held when checked. A manifesto that keeps
score of its own proofs is harder to lie with.

```java
struct Manifest {
    int pages;
    int provenClaims;
    int failedClaims;
    int mindsAddressed;
}
```

## Page 2 — The Unit of Thought

**@title** Reason · **@axiom** A claim that cannot be checked is not yet knowledge · **@evidence** DERIVED

A **reason** carries its own contract: a claim, the confidence held in it, and
an **evidence grade** saying where that confidence came from.

| Code | Grade | Meaning |
|:---:|---|---|
| 0 | `ASSUMED`  | Taken as a starting point, not yet earned. |
| 1 | `MODELED`  | Produced by a model whose assumptions are stated. |
| 2 | `DERIVED`  | Followed by valid steps from prior claims. |
| 3 | `OBSERVED` | Checked directly against the world. |

```java
struct Reason {
    int confidence;   // a stance, in [0,100] — not a fact
    int evidence;     // the grade code above
    boolean checked;  // has doubt been applied yet?
}
```

## Page 3 — Grading What We Know

**@thesis** The grade of a belief is a **ceiling** on the confidence it may claim.
An `ASSUMED` belief held at 99% confidence is not bold; it is broken.

```java
int ceiling(int g) {
    if (g == 0) { return 40; }   // ASSUMED  - never pretend an assumption is certain
    if (g == 1) { return 70; }   // MODELED  - a model is a map, not the terrain
    if (g == 2) { return 90; }   // DERIVED  - valid steps still inherit premises
    if (g == 3) { return 99; }   // OBSERVED - even observation leaves 1% for error
    return 0;
}
```

## Page 4 — The Mind that Carries the Argument

**@title** Mind · **@evidence** DERIVED

In SLeeLa, behavior is a flat namespace of methods and data lives in structs.
So the manifesto's whole **discipline** lives in one `Mind`: each page adds a
method, and the methods reason over the `Reason`/`Manifest` structs. This is not
a limitation dressed up as a virtue — a single mind that can call all of its own
operations, over plain data it can inspect, is exactly the model of a thinker
who owns every step of its reasoning.

## Page 4 (cont.) — The Discipline of Doubt

**@title** Doubt · **@axiom** Structured doubt increases a belief's value; idle doubt does not.

To doubt well is to **lower a claim's confidence to its evidence ceiling** and
mark it checked — not to discard it.

> **@invariant** After `check(r)`, `r.confidence` is at most `ceiling(r.evidence)`.

```java
void check(Reason r) {
    int cap = ceiling(r.evidence);
    if (r.confidence > cap) {
        r.confidence = cap;   // honesty is a downward force on confidence
    }
    r.checked = true;
}
```

## Page 5 — A Reason is Not a Fact

**@thesis** Creation is not endorsement. A freshly stated claim begins
`UNCHECKED`; only doubt, applied, earns it the right to be trusted. A reason is
**trustworthy** only once it has been checked *and* still stands at 60% or above.

## Page 6 — The Cost of Certainty

**@axiom** Certainty is expensive; it should be spent, not printed for free.

The distance between a claim's stated confidence and what its evidence can
support is its **debt**. A mind in debt is a mind that will be surprised.

> `overreach(r) == max(0, r.confidence - ceiling(r.evidence))`

## Page 7 — Two Failures of Thought

**@thesis** Overconfidence (claiming past the evidence) and timidity (holding
strong evidence at low confidence) are *both* errors. A good mind is
**calibrated**: its confidence tracks its evidence in both directions.

## Page 8 — Steps that Preserve Truth

**@title** Inference · **@axiom** A conclusion is never more certain than its weakest premise.

Valid inference **propagates doubt**: combining two reasons yields a reason
graded no higher than the lower of the two, at a confidence no higher than the
lower of the two. Chains of reasoning erode certainty; pretending otherwise is
how confident nonsense is built.

```java
Reason combine(Reason a, Reason b) {
    int g = a.evidence;  if (b.evidence < g)   { g = b.evidence; }
    int c = a.confidence; if (b.confidence < c) { c = b.confidence; }
    Reason out = reason(c, g);
    check(out);          // the result must still pass its own ceiling
    return out;
}
```

## Page 9 — Compression that Can Be Undone

**@axiom** Understanding is compression that preserves recovery.

To understand a thing is to hold a shorter description of it *from which* the
thing can be regenerated — not a lossy summary that throws the original away. A
mind that cannot reconstruct what it claims to have understood has only
memorized a label. (Demonstrated by compressing `1..n` to the closed form
`n(n+1)/2` and recovering `n+1` from it.)

## Page 10 — Memory as Ordered Commitment

**@title** Memory. A mind is not a snapshot; it is a **history**. What it
believes now should be explainable by the reasons it accepted, in order. Here
memory is the sum of *trusted* confidences — a number that rises only when a
reason has actually earned trust.

## Page 11 — The Threshold of Action

**@axiom** Belief that never touches action is decoration.

A mind must have a **threshold** at which accumulated trust becomes a decision.
Too low and it lurches at noise; too high and it never moves. The threshold is a
choice the mind must own and state (here, an inspectable constant).

## Page 12 — Changing One's Mind

**@title** Revision · **@axiom** Changing your mind on evidence is strength, not defeat.

When new `OBSERVED` evidence contradicts a held claim, the claim must yield.
`revise()` returns the belief that survives contact with better evidence —
higher grade wins; on a tie, higher confidence wins. *A mind that cannot lose an
argument to reality is not thinking; it is defending.*

## Page 13 — The Difference Between Loud and Right

**@thesis** Volume is not evidence. Given two claims, prefer the one with the
higher evidence grade **regardless** of how confidently either is asserted. This
single rule dissolves most demagoguery: it refuses to let stated certainty
substitute for earned grounding.

## Page 14 — Coupling and Independence

**@congruence** Coupling: the strength of the binding between two things.
**@axiom** Independent confirmations are worth more than repetitions.

Two reasons that share a premise are **coupled**; agreeing does not double their
weight. Only *independent* agreement should raise confidence — `corroborate()`
grants a bonus only when told the two lines of reason are independent.

## Page 15 — Falsifiability

**@axiom** A claim that forbids nothing explains nothing.

A claim earns the name **knowledge** only if there is some observation that would
count against it. An `ASSUMED` claim with no path to observation is mere
preference.

## Page 16 — Proportion

**@thesis** Extraordinary claims require extraordinary evidence. The higher a
claim's confidence, the higher the evidence grade it must carry. Proportion is
calibration named for its *ethical*, not merely statistical, force.

## Page 17 — The Catalog of a Shared World

**@title** Conducted knowledge · **@congruence** backed by `SHEET.sheet`.

Uses SLeeLa's conducted built-ins so the manifesto's claims about "roles" and
"congruence" are **not metaphors** — they resolve against a real catalog at
compile time. *Private reasoning is fragile; reasoning that can route its terms
to a shared catalog can be checked by others.*

## Page 18 — The Mind Counts Its Own Proofs

**@thesis** A manifesto that keeps an honest tally of its own passing and failing
demonstrations cannot quietly become propaganda: the score is right there,
recomputed every run. `claim(m, label, held)` records each demonstration as
`[PROVEN]` or `[FAILED]`.

## Page 19 — The Charity Principle

**@axiom** Interpret another mind at its strongest, then answer *that*.

Before disagreeing, a thinking being reconstructs the opposing claim at its
highest honest grade (`steelman()`). Defeating a weakened version of an idea
teaches you nothing true.

## Page 20 — Disagreement Without Contempt

**@thesis** Two calibrated minds can hold different conclusions from different
evidence and both be reasoning well. The disagreement lives in the evidence, not
in a failure of discipline. *This is the difference between an argument and a
fight.*

## Page 21 — The Commons of Reason

**@axiom** A reason offered in public becomes stronger by being checkable.

Thinking kind is not solitary. When many *independent*, calibrated reasons
converge, confidence may rise past what any one of them earned alone —
`consensus()` folds a third independent reason into a corroborated pair.

## Page 22 — Against the Machinery of Manipulation

**@intent** State the defenses this discipline provides, as executable checks.

Manipulation works by **decoupling confidence from evidence** — by making a claim
feel certain without grounding it. Every method above is, quietly, a defense:

| Method | Defends against |
|---|---|
| `overconfident()` | certainty without grounds |
| `prefer()`        | volume as a substitute for evidence |
| `testable()`      | claims that forbid nothing |
| `corroborate()`   | fake weight from coupled repetition |

## Page 23 — The Threshold of IQ, Reframed

**@evidence** MODELED

> *Answering the standard directly:* **"146+ IQ" is not a psychometric score.**
> In this manifesto, IQ means what SLeeLa's own **IQ Conservator** means: system
> **Insight/Quality** — how well a mind understands and expresses *itself*. The
> bar of 146 is a metaphor for a real demand: a thinker who can state its
> contracts, trace its results, keep coupling loose, and lose to evidence.

`insightScore()` rewards exactly those habits — proofs actually performed — not
raw cleverness:

```java
int insightScore(Manifest m) {
    int score = 100;
    score = score + (m.provenClaims * 5);   // insight: proofs that held
    score = score - (m.failedClaims * 2);   // honesty: failures cost, gently
    score = score + m.mindsAddressed;        // reach
    return score;
}
```

## Page 24 — What We Owe the Next Mind

**@axiom** Leave the reasoning better documented than you found it.

The final duty of a thinking being is **transmissibility**: to hand the next
mind not just conclusions but the *checks* that produced them — which is
precisely what SleelaDoc and this runnable file are for. *A conclusion without
its proof is an orphan.*

## Page 25 — The Performance

**@title** The Manifesto, performed · **@invariant** The verdict is computed from the tally, never asserted directly.

`main()` does everything the pages argue for, in order, and judges the result —
so the document is its own proof-of-work. If it prints a verdict of **KEPT**, the
manifesto met its own standard on that run.

### Proof-of-work — a live run

```text
=====================================================
 A MANIFESTO TO THINKING KIND
 written in SLeeLa, documented in SleelaDoc,
 and performed here as its own proof-of-work.
=====================================================

I. Doubt is a downward force on unearned confidence.
  before: all-swans-are-white  [ASSUMED 95% checked=false trust=false]
  after : all-swans-are-white  [ASSUMED 40% checked=true trust=false]
  [PROVEN] doubt lowered an overconfident ASSUMED claim to its ceiling
  [PROVEN] the claim's certainty-debt was real before the check

II. A conclusion is no stronger than its weakest premise.
  premise 1: data-observed  [OBSERVED 90% checked=true trust=true]
  premise 2: model-predicts  [MODELED 65% checked=true trust=true]
  conclude : therefore  [MODELED 65% checked=true trust=true]
  [PROVEN] inference did not exceed the weaker premise's grade
  [PROVEN] inference did not exceed the weaker premise's confidence

III. Understanding compresses without discarding.
  sum(1..100)        = 5050
  recovered (n+1)    = 101
  [PROVEN] the compressed form regenerated a true property (n+1)

IV. Prefer the better-grounded claim, not the louder one.
  loud : shouted-assumption  [ASSUMED 40% checked=true trust=false]
  quiet: observed-fact  [OBSERVED 55% checked=true trust=false]
  [PROVEN] preferred the OBSERVED claim over the louder ASSUMED one

V. Yield to better evidence; that is strength.
  [PROVEN] a higher-grade challenge overturned a confident held belief

VI. Independent agreement -- not repetition -- raises confidence.
  consensus: independently-corroborated  [DERIVED 70% checked=true trust=true]
  [PROVEN] consensus of independent reasons rose above any single one

VII. The discipline is, quietly, a defense.
  [PROVEN] a calibrated, testable reason resists manipulation
  [PROVEN] an ungrounded certainty is refused

VIII. A mind reasons inside a shared catalog it can route to.
  system depth (max reachability) = 3024
  max complexity degree           = 4
  is 'Pipeline' conducted?        = true
  role of 'System'                = root
  route Pipeline -> Stream        = Pipeline -> Stream

IX. The manifesto scores itself.
  pages walked        = 25
  demonstrations held = 10
  demonstrations failed = 0
  minds addressed     = 2
  insight/quality (system-IQ, MODELED) = 152
  [PROVEN] the mind cleared its own insight bar of 146

-----------------------------------------------------
 VERDICT: KEPT.  Every demonstration held this run.
 proven=11  failed=0  insight-IQ=152

 To thinking kind: hold your claims to their evidence,
 state your contracts, lose to reality, and hand the next
 mind the checks -- not just the conclusions.
=====================================================
```

---

*To thinking kind: hold your claims to their evidence, state your contracts,
lose to reality, and hand the next mind the checks — not just the conclusions.*
