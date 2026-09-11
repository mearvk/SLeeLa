# BODI — Witness, Change, and Mitigative Circumference

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

## 9. Circuit and State Coherence

The circuit analogy is intentionally bounded: it is a model for coherent state transitions, not a claim that software obeys a physical conservation law.

A valid transition has:

```text
state before
   ↓
transition
   ↓
state after
```

A sequence has:

```text
S₀ → S₁ → S₂ → ... → Sₙ
```

A later operation must not assume a prerequisite that an earlier operation has already removed.

For example, if `write(resource)` requires an open resource:

```text
open(resource)
→ start(resource)
→ write(resource)
→ stop(resource)
→ close(resource)
```

is coherent, while:

```text
open(resource)
→ close(resource)
→ write(resource)
```

is not, unless the target explicitly defines `write` as valid after `close`.

The principle is:

> **A closed circuit or closed resource must not be rhetorically treated as open merely because a later verb says so.**

## 10. Rhetorical Safety

A Bodi expression is **rhetorically safe** when its wording claims no more than the system can establish.

The Witness should distinguish states such as:

```text
REQUESTED
ACCEPTED
REJECTED
APPLIED
REVERSED
CONTAINED
PROPAGATED
OBSERVED
FAILED
```

Therefore:

```text
install(network_witness)
```

means that installation was requested through the Bodi semantic layer. It does not alone prove successful installation.

A witnessed path may instead be:

```text
REQUESTED → ACCEPTED → APPLIED → OBSERVED
```

or:

```text
REQUESTED → REJECTED
```

or:

```text
REQUESTED → ACCEPTED → FAILED
```

This distinction keeps the language precise: **request is not fact, and fact is not inferred from syntax alone.**

## 11. The Circumferent Rule

Every operation should be readable from both its interior and its surrounding context.

```text
             circumference
          ↙                 ↘
      context             consequence
          ↘                 ↙
             Bodi change
```

The interior identifies the object, verb, datum, and sequence. The circumference identifies prerequisites, authority, boundary, inverse, consequences, and Witness.

A sequence is complete when these views agree rather than contradict one another.

## 12. Propagation and Containment

`propagate` and `contain` define the principal boundary relationship.

Propagation is not unrestricted broadcast. It means a witnessed change is permitted to move to a defined recipient or boundary.

Containment means that the change stops at the current boundary.

A propagation chain is therefore:

```text
A
 ↓ witness
B
 ↓ witness
C
```

Each boundary should have an identifiable recipient and independently interpretable Witness.

## 13. XML Duality

Bodi carries its semantic structure across a network using the agreed XML representation:

```xml
<bodi xmlns="urn:sleela:bodi:1"
      system="xxx"
      sequence="001"
      method="install"
      starter="network"
      man="operator">
    <datum>network_witness</datum>
</bodi>
```

The schema is `src/implementations/_001_/bodi/bodi.xsd`.

The relationship is:

```text
XML document
    ↓
BodiChange
    ↓
addressed system
    ↓
middle verb
    ↓
target operation
    ↓
Witness
```

XML is a transport representation, not proof that a remote operation occurred. The parser is intentionally small and deterministic, with external DTDs and external entities disabled.

## 14. Middle Verbs and Target Methods

The Bodi vocabulary should not collapse into arbitrary Java reflection:

```text
method = arbitrary user supplied string
```

Instead:

```text
Bodi middle verb
        ↓
semantic adapter / contract
        ↓
target object method
```

The middle verb expresses the intended semantic transition; the target object determines how that transition is implemented and whether it is admissible.

## 15. Transactional Groups

For greater complexity, operations may be grouped:

```text
ChangeSet
 ├── operation 1
 ├── operation 2
 ├── operation 3
 └── operation n
```

A group should retain:

```text
identity
sequence
preconditions
operations
observations
result
reversal policy
boundary policy
```

A failed operation must remain visible. A group may finish as:

```text
COMPLETE
PARTIAL
ROLLED_BACK
CONTAINED
FAILED
```

A later success must not erase an earlier failure.

## 16. Full-Sequence Completeness

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

The source expression can remain concise:

```java
Bodi.system("xxx").install("yyy");
```

Defaults may supply contextual fields, but the runtime record should preserve enough information to reconstruct the meaningful event.

## 17. Recommended Invariants

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

## 18. Backend and Network Map

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

## 19. Final Principle

Bodi should be **m-eloquent**: concise at the programming surface while retaining a disciplined, reconstructable meaning.

Bodi should be **circumferent**: every operation should be interpretable together with prerequisites, consequences, inverse, boundary, and Witness.

Bodi should be **FS-complete**: a complex sequence should expand into a complete account of address, transition, datum, order, boundary, and result.

The governing rule is:

> **Build carefully, witness continuously, propagate deliberately, contain explicitly, and tear down in dependency order.**

The middle verbs provide the vocabulary. The mitigative circumference provides the discipline. The Witness provides the record. The target system provides the actual state transition.

This permits Bodi sequences to grow from `2n` to `5n` and beyond while preserving logical coherence, explicit boundaries, and rhetorically safe statements about what the system actually did.
