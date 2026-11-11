# Unit 2 (Adults): Predicates, Quantifiers and Inference

**Outcomes:** write registry rules with predicates and quantifiers; negate them
correctly; tell valid from invalid inference; state what "not observed" licenses.
**Companions:** [lessons 1, 4 and 5](../../series/README.md). Prerequisite: Unit 1.

## Part 1: Predicates over the registry

A **predicate** is a property of an object. Let the domain be the nouns in one
registry. Write:

- `Reg(x)`: x is registered
- `Carries(c, x)`: c is the immediate carrier of x
- `Root(c)`: c is the declared field

## Part 2: Quantifiers

- `∀x P(x)`: P holds for every x in the domain
- `∃x P(x)`: P holds for at least one x
- `∃!x P(x)`: exactly one x

The registry rules (lessons 1 and 5), stated formally:

1. One place only: `∀x ( Reg(x) → ∃!c Carries(c, x) )`
2. Carrier validity: `∀x ∀c ( Carries(c, x) → ( Root(c) ∨ Reg(c) ) )`
3. Chains reach the field: for every registered x, repeatedly following the
   carrier ends at the field.

**Negation.** `¬∀x P(x) ≡ ∃x ¬P(x)` and `¬∃x P(x) ≡ ∀x ¬P(x)`.

## Part 3: Inference

An argument is **valid** if the conclusion is true in every case where the
premises are true.

- Modus ponens: P → Q, P ⊢ Q (valid)
- Modus tollens: P → Q, ¬Q ⊢ ¬P (valid)
- Affirming the consequent: P → Q, Q ⊢ P (**invalid**)
- Denying the antecedent: P → Q, ¬P ⊢ ¬Q (**invalid**)

## Part 4: Exercises

1. Negate rule 1 in words and in symbols. Which lesson-1 call is a witness to
   the negation being attempted, and what does the registry do?
2. Is this valid? Premises: every registered noun has a carrier; `kite` has no
   carrier. Conclusion: `kite` is not registered.
3. Is this valid? Premises: if a submission is good, its evidence is
   registered; this submission's evidence is registered. Conclusion: it is good.
4. In lesson 5 the chain for `order 9` prints "not observed". Which
   conclusion is licensed: `¬Reg(order 9)` in this registry, or `order 9` does
   not exist in the world?
5. Give a domain of three nouns where `∀x ∃c Carries(c, x)` is true but
   `∃c ∀x Carries(c, x)` is false.

## Answer key

1. "Some registered noun does not have exactly one carrier":
   `∃x ( Reg(x) ∧ ¬∃!c Carries(c, x) )`. Lesson 1's second `ball` call tries to
   give ball a second carrier. The registry refuses it with "noun already has
   a carrier (one place only)", so the negation never becomes true.
2. Valid, by modus tollens: Reg → HasCarrier, ¬HasCarrier ⊢ ¬Reg.
3. Invalid: affirming the consequent. Registered evidence is necessary, not
   sufficient: the claim and the step order also matter (Unit 1, Part 4).
4. `¬Reg(order 9)` in this registry only. The world is outside the field.
5. Nouns a, b, c with carriers a←field, b←a, c←b: each has a carrier, but no
   single carrier holds all three.
