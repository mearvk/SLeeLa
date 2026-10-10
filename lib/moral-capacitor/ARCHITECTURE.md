# Moral Capacitor™ — Architecture

The **Moral Capacitor™** is a small, uniform ethical gate that every `/lib`
library can place in front of a consequential operation. Like an electrical
capacitor, it **accumulates charge and only discharges when enough has built
up** — here the charge is *moral justification* and the discharge is
*permission to proceed*. An operation runs only when the capacitor has stored
enough moral charge from recognised ethical frameworks **and** no hard moral
veto (a red line) is tripped.

It is designed for all `/lib` families but is placed first and foremost in front
of the three surfaces that actually *do things*: the **VM**, the **OS**
(including OS Creator™), and the **Machine**.

> **Name & mark.** This component is the **Moral Capacitor™**. The mark denotes
> the gate surface (`SLMoralCapacitor`) and the charge → threshold → discharge
> verdict contract described here.

## 1. The capacitor metaphor, made precise

```text
   operation (actor, action, target, scope)
        |
        v
   charge up  <-- ethical frameworks each contribute moral charge
        |          (deontology, utilitarianism, virtue, beneficence, consent)
        v
   [ Moral Capacitor ]  stored charge C, threshold T, veto set V
        |
        +-- any veto in V tripped?  -> DENY (hard red line; no discharge)
        +-- C >= T ?                -> DISCHARGE: PERMIT the operation
        +-- 0 < C < T ?             -> DEFER: hold; ask for more justification
        +-- C <= 0 ?                -> DENY: no moral basis
```

- **Charge (`SLMoralCharge`)** is the sum of per-framework contributions, each a
  small signed integer: a framework that endorses the operation adds charge, one
  that objects subtracts it. Charge is bounded (a capacitor has a maximum).
- **Threshold (`T`)** is the minimum charge required to permit, chosen by the
  consulting library according to how consequential the operation is (reading a
  clock needs little; wiping a disk needs a lot).
- **Veto (`SLMoralVeto`)** is a set of absolute red lines (child safety, CBRN /
  weapons, non-consensual surveillance, deception/impersonation, scaled abuse,
  irreversible destruction without consent). A tripped veto denies regardless of
  charge — a capacitor cannot discharge through an open circuit.

## 2. Verdicts

`SLMoralVerdict` is one of:

| Verdict | Meaning |
|---|---|
| `PERMIT` | charge ≥ threshold and no veto: the operation may proceed. |
| `DEFER` | partial charge: hold the operation and request more justification/consent. |
| `DENY` | no charge, or a veto tripped: the operation must not proceed. |

Every verdict carries the stored charge, the threshold, the deciding framework
or veto, and a human-readable reason, so a caller can log *why*.

## 3. Classes

| Class | Role |
|---|---|
| `SLMoralCapacitor` | The gate. `charge(framework, delta)` adds moral charge; `veto(redline, tripped)` arms a red line; `evaluate(threshold)` returns an `SLMoralVerdict`. Reusable: `discharge()` resets stored charge after a decision. |
| `SLMoralCharge` | The accumulated charge with per-framework attribution and a bounded maximum. |
| `SLMoralFramework` | The ethical lenses (deontology, utilitarianism, virtue ethics, beneficence, consent/autonomy), bridging the `coorenagraph` moral vocabulary into evaluable weights. |
| `SLMoralVeto` | The red-line catalogue and whether each is tripped for the operation. |
| `SLMoralOperation` | What is being judged: actor, action verb, target, scope, and reversibility. |
| `SLMoralVerdict` | The decision (PERMIT / DEFER / DENY) with charge, threshold, and reason. |
| `SLMoralPolicy` | A named bundle of a threshold + an armed veto set, so a library applies a consistent stance (e.g. "VM default", "OS destructive", "Machine power"). |
| `SLMoralLedger` | An append-only record of verdicts for audit (what was asked, what was decided, why). |

## 4. The universal consult pattern

Any `/lib` class gates an operation the same way:

```sleela
SLMoralCapacitor cap = new SLMoralCapacitor(); cap.configure();
SLMoralOperation op = new SLMoralOperation();
op.configure("vm", "allocate", "guest-memory", "process");   // actor, action, target, scope

// Charge from the frameworks that apply; arm any red lines.
cap.appraise(op);                      // frameworks score the operation
SLMoralVerdict v = cap.evaluate(cap.policy().threshold());
if (v.permitted()) { /* proceed */ } else { /* deny or defer */ }
```

Libraries that want a one-call gate use an adapter (§5) that bundles the
appraisal, the policy, and the ledger entry.

## 5. VM / OS / Machine adapters (the priority surfaces)

| Adapter | Guards |
|---|---|
| `SLVMMoralGuard` | VM operations: memory growth, spawning threads/processes, native/OS bridge calls, running a program on the substrate. |
| `SLOSMoralGate` | OS operations: process execution (`osRun`/`osSpawn`), filesystem destruction (`osRemove`/format), installer disk writes, and OS Creator™ writing a tree. |
| `SLMachineMoralGate` | Machine operations: powering on, booting a guest, and executing programs on the composed machine. |

Each adapter wraps an `SLMoralCapacitor` with a surface-appropriate
`SLMoralPolicy` and exposes intention-named checks (e.g. `permitErase(target)`),
returning an `SLMoralVerdict` and appending to an `SLMoralLedger`.

## 6. Relationship to existing governance

- The Moral Capacitor™ **refines** the governance `ETHICS_NORMS` axis
  (`lib/opcodes/governance/SLGovConcept`): where the EventObserver reports
  whether an opcode run honoured ethics, the capacitor is the *gate consulted
  before* a consequential operation runs.
- It draws its framework vocabulary from `coorenagraph` (Deontology,
  Utilitarianism, VirtueEthics, Beneficence, consent/autonomy) rather than
  inventing a second ethics vocabulary.
- It honours the Constitution: a verdict is a **Precondition** (README Article II)
  on consequential operations; a tripped veto is an **Invariant** violation.

## 7. Honesty note

The Moral Capacitor™ is a **decision gate**, not a moral oracle. It makes an
operation's ethical justification explicit, bounded, auditable, and refusable;
it does not claim to compute objective morality. A library is responsible for
charging it honestly (not padding charge to force a discharge) and for honouring
a `DENY`/`DEFER`. The red-line veto set is deliberately absolute and mirrors the
platform's content-safety boundaries.
