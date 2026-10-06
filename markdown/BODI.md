# BODI — Witness, Change, Mitigative Circumference, and Wiggle Search

Bodi is the witness layer to object change in SLeeLa. It gives a programmer a stable semantic address for a known or knowable system and records the change attempted against that address.

The preferred form is:

```java
Bodi.system("xxx").propagative("001").install("yyy");
```

Bodi is the semantic layer; Java RMI is a compatibility transport and registry mechanism underneath it.

## 1. Witness Model

A Bodi operation is understood as:

```text
system → reference → change → method → sequence → starter → man
```

- **system** — named system/object architecture being addressed.
- **reference** — registry address used to resolve it.
- **change** — intended object operation.
- **method** — operation to invoke.
- **sequence** — propagation/order identifier, such as `001`.
- **starter** — actor or subsystem initiating the operation.
- **man** — declared human/operator identity supplied by the caller.

A `BodiWitness` records this relationship with its path and creation time. The witness records an addressable event; it does not replace the target object.

## 2. RMI Equivalence

The older mental model can be expressed as:

```text
RMI.lookup("remote_name").put("datum")
```

Bodi makes the semantic idea explicit:

```text
Bodi.system("remote_name").install("datum")
```

The RMI registry remains available through `pull()` and `push()` so Java remote objects can participate without making RMI the primary Bodi vocabulary.

## 3. The Middle-Verb Set

Bodi uses a closed set of eighteen positive/negative pairs. The pair is a declared relationship, not an arbitrary English command.

| Family | Positive | Negative | Meaning |
|---|---|---|---|
| Capability | `install` | `uninstall` | place / remove capability |
| Relationship | `connect` | `disconnect` | establish / end relationship |
| Datum | `push` | `pull` | publish / retrieve datum or reference |
| Permission | `enable` | `disable` | permit / prevent operation |
| Lifecycle | `start` | `stop` | begin / end active sequence |
| Resource | `open` | `close` | open / close resource |
| Attachment | `attach` | `detach` | join / remove object from context |
| Naming | `bind` | `unbind` | associate / remove name |
| Identity | `register` | `unregister` | publish / withdraw service identity |
| Namespace | `mount` | `unmount` | place / remove resource in namespace |
| Operation | `activate` | `deactivate` | make / cease operational |
| Admission | `admit` | `reject` | accept / refuse offered change |
| Authority | `approve` | `revoke` | grant / withdraw authorization |
| Durability | `commit` | `rollback` | retain / reverse witnessed change |
| Publication | `publish` | `withdraw` | make / remove discoverability |
| Visibility | `expose` | `conceal` | reveal / hide interface |
| Observation | `observe` | `ignore` | record / decline observation |
| Boundary | `propagate` | `contain` | carry onward / stop at boundary |

The canonical vocabulary is implemented by `BodiVerb.java` and documented in `BODI_VERBS.md`.

## 4. Mitigative Circumference

The **mitigative circumference** is the bounded semantic region around a Bodi operation. An operation remains inside the circumference when its address, verb, datum, sequence, witness, and boundary are intelligible.

```text
                 MITIGATIVE CIRCUMFERENCE

        ┌──────────────────────────────────────┐
        │ address                              │
        │   ↓                                  │
        │ sequence → verb → datum → change     │
        │                 ↓                    │
        │              witness                 │
        │                 ↓                    │
        │        propagation / boundary        │
        └──────────────────────────────────────┘
```

The circumference is a guardrail, not another programming language. If a composition loses required context, it should stop at the boundary rather than invent meaning.

The minimum questions are:

```text
What is addressed?
What transition is requested?
What datum/capability is affected?
Where is this in the sequence?
What state is required before it?
What state should follow it?
Did the change remain local or propagate?
What was actually witnessed?
What inverse or containment policy exists?
```

## 5. Positive and Negative Directions

Positive and negative describe direction within a pair. They are not moral judgments.

```text
connect   ↔ disconnect
open      ↔ close
publish   ↔ withdraw
commit    ↔ rollback
propagate ↔ contain
```

A negative operation can be the correct operation during safe teardown. A positive operation can be invalid when its prerequisites are absent.

An inverse is a **logical relationship**, not a guarantee that every external side effect can be perfectly undone. If publication, transmission, time, or another external effect cannot be reversed, the Witness records that residual fact instead of asserting fictional rollback.

## 6. Forward and Reverse Closure

Represent an operation as:

```text
O = (system, sequence, verb, datum, boundary, witness)
```

For a reversible state transition:

```text
S₀ --positive--> S₁
S₁ --negative--> S₀
```

Complex sequences should preserve the same discipline:

```text
S₀ → S₁ → S₂ → ... → Sₙ
```

and, where genuinely reversible:

```text
Sₙ → ... → S₂ → S₁ → S₀
```

The actual target state machine remains authoritative.

## 7. Building Up and Tearing Down

Complex systems should be built incrementally and dismantled incrementally. A useful construction pattern is:

```text
register
  ↓
bind
  ↓
attach
  ↓
connect
  ↓
open
  ↓
enable
  ↓
activate
  ↓
start
  ↓
observe
  ↓
propagate
```

A corresponding dependency-oriented teardown is:

```text
contain
  ↓
stop
  ↓
deactivate
  ↓
disable
  ↓
close
  ↓
disconnect
  ↓
detach
  ↓
unbind
  ↓
unregister
```

This is a heuristic, not a universal fixed order. The governing rule is:

> **Build prerequisites before dependents; remove dependents before prerequisites.**

Thus a sequence may use a different order when the target contract requires it, but it must not silently destroy a prerequisite needed by a later operation.

## 8. 2n, 5n, and Greater Composition

A Bodi sequence can be treated as an ordered composition:

```text
Vⁿ = V₁ · V₂ · ... · Vₙ
```

The notation `2n`, `5n`, or a larger composition means repeated or grouped operations, not permission to repeat a verb blindly.

Repetition is meaningful when at least one of these holds:

1. **Multiplicity** — there are genuinely multiple objects or instances.
2. **Sequencing** — each repetition advances a defined state machine.
3. **Idempotence** — repetition leaves a valid state without unintended duplication.

For example:

```text
register(A)
→ bind(A)
→ attach(A)
→ enable(A)
→ start(A)
```

is a meaningful five-step construction when each step's prerequisite is satisfied.

By contrast:

```text
install(A)
→ install(A)
```

must be interpreted according to the target contract. It may be idempotent, duplicate-producing, conditional, or invalid.

Bodi should never invent a state solely to make a repeated sequence appear valid.

## 9. Wiggle Verb Set — XML-Directed Search and Harmonization

The **wiggle layer** is a controlled exploratory vocabulary that operates *around and through the XML structure* without pretending that an exploratory observation is itself a system change. It is intended for best-guess linking to normally well-defined SLeeLa servers or other SLeeLa-aware endpoints.

A wiggle operation asks the XML structure a bounded question, follows an informed direction, records what was found, and converts a useful finding into a new witnessed input/output relationship.

The canonical wiggle pairs are:

| Family | Positive | Negative | Purpose |
|---|---|---|---|
| Structure | `trace` | `untrace` | follow an XML path / cease path following |
| Direction | `seek` | `avoid` | search an intended structural direction / exclude it |
| Harmonic | `tune` | `detune` | compare compatible system characteristics |
| Causation | `cause` | `decause` | test a candidate cause / remove it from the active hypothesis |
| Linkage | `link` | `unlink` | form or remove a *candidate* structural association |
| Correlation | `correlate` | `decouple` | compare related observations / separate them |
| Traversal | `ascend` | `descend` | move toward parent or child XML structure |
| Ordering | `precede` | `follow` | inspect earlier or later structural context |
| Query | `inspect` | `pass` | examine a node / deliberately skip it |
| Resonance | `harmonize` | `dissonate` | test whether structures support a common interpretation |
| Evidence | `confirm` | `question` | strengthen or reopen an interpretation |
| Search | `scan` | `bound` | perform a bounded search / constrain its frontier |

These verbs are deliberately different from the 18 middle verbs. A middle verb changes an addressed system. A wiggle verb primarily **investigates, compares, traverses, or constrains information in the XML/system description**.

### Wiggle safety rule

A wiggle may discover a candidate relationship but may not silently promote that candidate into an executed change.

```text
XML structure
   ↓
wiggle search
   ↓
observation
   ↓
count / satisfaction
   ↓
science note
   ↓
validated candidate
   ↓
(optional) Bodi middle verb
   ↓
actual change
```

Thus `link` in the wiggle layer means **candidate structural linkage** until the Bodi change layer explicitly accepts and applies the relationship.

## 10. Four-to-Five Search Envelope

For each stated functional concern, the wiggle layer may conduct approximately **four to five bounded searches** through relevant XML structure before producing a satisfaction record. This is a search budget, not a requirement to fabricate five findings.

A typical envelope is:

```text
1. trace / inspect   → locate the relevant node
2. seek / scan       → search the principal direction
3. ascend / descend  → inspect structural context
4. correlate / tune  → compare compatible evidence
5. confirm / question → decide whether the result is sufficiently supported
```

If a concern is simple, a **1-to-1** path is valid:

```text
one input → one inspection → one output
```

A more involved concern may use:

```text
4–5 searches → 1 satisfaction result
```

and a larger composition may use:

```text
4–5 → 1 → 1 → 4–5 → satisfied output
```

All of these are valid forms when their boundaries and counts are recorded.

The number of searches is not evidence of correctness by itself. The evidence must come from the XML structure, server response, schema relationship, or other explicitly recorded observation.

## 11. Satisfaction Count and Science Gain

A wiggle search produces a **satisfaction observation** only when the search returned a structurally intelligible result or a meaningful bounded negative result.

A simple accounting model is:

```text
C₀ = 0
Cₙ₊₁ = Cₙ + 1    when one search concern is satisfactorily resolved
```

The count is a bookkeeping measure, not a physical quantity.

A completed concern may produce a **science result**, such as:

```text
XML node → formula candidate → formula review → content gain
```

or:

```text
XML relation → comparison → validated linkage → content gain
```

The term **content gain** means that the system has acquired a new usable, documented, or better-supported piece of information. It does not imply a physical energy gain.

A useful conceptual chain is:

```text
input
  ↓
XML-directed search
  ↓
observation
  ↓
satisfaction count +1
  ↓
science note / formula review
  ↓
new content
  ↓
new input/output relation
  ↓
additional linkage
  ↓
containment index / boundary record
```

## 12. Electron / Transferon Language as Software Metaphor

The requested terms **electron gain** and **transferon gain** may be used as bounded software metaphors for a successful transfer of information or linkage state.

They must not be presented as claims about actual electrons, electrical charge, or conservation laws unless an independent physical measurement establishes such a claim.

For Bodi:

```text
transferon_gain = validated_information_transfer
 electron_gain  = validated_local_increment
```

These are conceptual counters or labels, not physical units.

A safe implementation can therefore record:

```text
transferon +1
content +1
linkage +1
containment_index +1
```

only when the corresponding observation is actually established.

## 13. Linear Search Directions

The XML structure may be searched in several explicit directions:

```text
left → right       sibling/order traversal
right → left       reverse sibling/order traversal
parent → child     descent
child → parent     ascent
attribute → node   semantic context
node → reference   external linkage
reference → node   returned linkage
```

A **linear search** means that the search frontier advances according to a declared order. It does not mean that XML is literally one-dimensional.

Each search should record:

```text
origin
frontier
Direction
target
match condition
result
count contribution
```

This prevents an exploratory traversal from becoming an unbounded scan.

## 14. XML Harmonics and Causation

**System harmonics** are structural correspondences that recur across compatible XML documents or server descriptors: matching namespaces, compatible element names, repeated sequence identifiers, consistent capability descriptions, or other explicitly defined structural patterns.

A harmonic match is evidence of correspondence, not proof of causation.

Causation must therefore be treated as a hypothesis:

```text
candidate cause
   ↓
controlled search
   ↓
observed consequence
   ↓
comparison / counter-check
   ↓
causal confidence
```

The wiggle layer should prefer `correlate`, `confirm`, and `question` before promoting a relationship to a causal assertion.

## 15. Science-Result Formation

A successful search can produce a science result without changing the remote system:

```text
XML input
  ↓
search set
  ↓
observations
  ↓
count
  ↓
formula/content review
  ↓
result document
```

A result document should preserve:

```text
source XML reference
search direction
search count
matched structure
interpretation
confidence
unresolved questions
next input/output
```

The next result can then become the next input, allowing iterative growth without losing provenance.

## 16. Mixed 1-to-1 and 4-to-5 Composition

Bodi explicitly permits mixed search topology:

```text
A → B
```

```text
A → {B, C, D, E} → F
```

```text
A → B → {C, D, E, F} → G → H
```

and:

```text
input
 ↓
4–5 XML searches
 ↓
1 satisfaction
 ↓
1 science result
 ↓
4–5 searches on the new result
 ↓
1 satisfied output
```

The topology is valid when each fan-out has a stated concern and each fan-in has a stated synthesis rule.

A fan-out without a synthesis rule is exploratory noise. A fan-in without recorded evidence is an unsupported conclusion.

## 17. Containment Count and Index Spark

Every exploratory chain should maintain a **containment count** and **index spark**.

The containment count records how many search/result boundaries were deliberately retained rather than allowed to propagate further.

The index spark is a compact marker that a new linkage, result, or search frontier became eligible for indexing.

Conceptually:

```text
search
 ↓
result
 ├── propagate → next boundary
 └── contain   → retained boundary
                 ↓
             index spark
```

A useful record is:

```text
search_count = 5
satisfied = 1
content_gain = 1
linkage_gain = 1
containment_count = 1
index_spark = 1
```

These are bookkeeping quantities. They should never be interpreted as proof merely because their counters increased.

## 18. Wiggle Circumference

The wiggle layer has its own circumference inside the larger mitigative circumference:

```text
              BODI CIRCUMFERENCE
        ┌────────────────────────────┐
        │ semantic change            │
        │                            │
        │   WIGGLE CIRCUMFERENCE     │
        │   ┌────────────────────┐   │
        │   │ XML search         │   │
        │   │ direction          │   │
        │   │ harmonic test      │   │
        │   │ causation test     │   │
        │   │ count               │   │
        │   │ science result     │   │
        │   └────────────────────┘   │
        │                            │
        │ witnessed change           │
        └────────────────────────────┘
```

The wiggle circumference is exploratory and evidentiary. The outer Bodi circumference remains authoritative for actual object change.

## 19. Rhetorical Safety of Wiggle Results

The following distinctions are mandatory:

```text
FOUND        ≠ VALIDATED
VALIDATED    ≠ CAUSAL
CAUSAL       ≠ EXECUTED
EXECUTED     ≠ OBSERVED SUCCESS
COUNTED      ≠ PROVEN
GAIN         ≠ PHYSICAL ENERGY
```

A safe statement therefore looks like:

```text
Five bounded XML searches produced one structurally satisfactory
candidate linkage, which generated one content-review result.
```

It should not silently become:

```text
Five searches proved the system caused the result.
```

The second statement exceeds the evidence unless a separate causal procedure establishes it.

## 20. Relationship to Middle Verbs

The two vocabularies form a disciplined stack:

```text
WIGGLE VERB
   ↓
inspect / seek / trace / correlate / confirm
   ↓
XML structural finding
   ↓
validated candidate
   ↓
BODI MIDDLE VERB
   ↓
install / connect / enable / start / propagate ...
   ↓
actual object/system transition
   ↓
BodiWitness
```

A wiggle verb therefore prepares knowledge for a possible Bodi change. It does not bypass the change layer.

## 21. Full-Sequence Completeness

A sequence is **FS-complete** for Bodi purposes when its Witness can reconstruct:

```text
address
transition
subject datum
order
precondition
boundary
result
inverse/containment policy
```

For wiggle work, FS-completeness additionally requires:

```text
XML source
search direction
search envelope
observations
satisfaction count
science/content result
linkage decision
containment count
index spark
```

The source expression may remain concise; the Witness and science record preserve the surrounding context.

## 22. Recommended Invariants

1. Unknown verbs are rejected.
2. Declared inverses are explicit.
3. An inverse never promises impossible physical reversal.
4. The target state machine controls admissibility.
5. Repetition cannot silently create contradictory state.
6. Propagation boundaries are explicit.
7. A request is not represented as completed fact without a Witness.
8. Build order respects prerequisites.
9. Teardown order respects dependents.
10. Partial failure remains visible.
11. Transport and semantic meaning remain distinct.
12. Reflection remains subordinate to a Bodi semantic contract.
13. Complex compositions remain decomposable into witnessed transitions.
14. No rhetorical statement exceeds the evidence recorded by the Witness.
15. Wiggle searches remain bounded by an explicit concern.
16. A search count is not itself evidence of truth.
17. Correlation is not silently promoted to causation.
18. Candidate linkage is not silently promoted to executed linkage.
19. Physical metaphors remain explicitly metaphorical unless measured.
20. Every science result retains enough provenance to reconstruct its input.

## 23. Backend and Network Map

```text
Bodi.java
  |
  +-- system(name)
  +-- pull/push (RMI compatibility)
  +-- network bootstrap
  |
  v
BodiExtender.java
  |
  +-- registry lookup/bind
  +-- reflection metadata
  +-- invoke(BodiChange)
  +-- witness collection
  |
  +--> BodiObjectReference
  +--> BodiChange
  +--> BodiWitness
  +--> BodiVerb

BodiXmlDocument.java <--> BodiNetworkServer.java
        ^                       ^
        |                       |
    bodi.xsd             BodiNetworkClient.java

BodiNetworkConfig.java <--> BodiNetworkBootstrap.java
        ^
        |
 config/bodi.properties.example
        ^
 tools/bodi/install-bodi.sh
```

The network listener remains an explicit opt-in through configuration. The default loopback binding is a boundary safeguard; exposed deployments should apply appropriate authentication and host/network policy.

## 24. Final Principle

Bodi should be **m-eloquent**: concise at the programming surface while retaining a disciplined, reconstructable meaning.

Bodi should be **circumferent**: every operation should be interpretable together with prerequisites, consequences, inverse, boundary, and Witness.

Bodi should be **FS-complete**: a complex sequence should expand into a complete account of address, transition, datum, order, boundary, and result.

The wiggle layer extends this discipline into XML exploration: search narrowly, follow declared directions, count what was actually resolved, preserve provenance, synthesize only from recorded evidence, and hand validated candidates back to the Bodi change layer.

The governing rule is:

> **Wiggle to discover; witness to establish; change to act; contain to preserve; propagate only when the boundary permits; and never let a counter claim more than its evidence.**
