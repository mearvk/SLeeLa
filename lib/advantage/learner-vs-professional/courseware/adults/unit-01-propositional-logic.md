# Unit 1 (Adults): Propositional Logic and the Input Gate

**Outcomes:** evaluate compound statements with truth tables; recognize
standard equivalences; read the Device Instructor's input gate as a logical
formula; and say precisely what a refusal does and does not prove.

**Companions:** [lessons 1, 2 and 4](../series/README.md). Run lesson 2 before
Part 4.

## Part 1: Statements and connectives

A **proposition** is a statement that is true or false (not both). In this
course a proposition is checked against a declared field of observation.

| symbol | name | true when |
|--------|------|-----------|
| ¬P | not | P is false |
| P ∧ Q | and | both are true |
| P ∨ Q | or | at least one is true (inclusive) |
| P → Q | implies | not (P true and Q false) |
| P ↔ Q | iff | P and Q have the same truth value |

Truth table for two propositions:

| P | Q | P ∧ Q | P ∨ Q | P → Q | P ↔ Q |
|---|---|:---:|:---:|:---:|:---:|
| T | T | T | T | T | T |
| T | F | F | T | F | F |
| F | T | F | T | T | F |
| F | F | F | F | T | T |

Note that `P → Q` is false in exactly one row: P true and Q false.

## Part 2: Equivalences

Two formulas are **equivalent** when they have the same truth value in every row.

- De Morgan: ¬(P ∧ Q) ≡ ¬P ∨ ¬Q and ¬(P ∨ Q) ≡ ¬P ∧ ¬Q
- Implication: P → Q ≡ ¬P ∨ Q
- Contrapositive: P → Q ≡ ¬Q → ¬P
- Double negation: ¬¬P ≡ P

Not equivalent: the converse. P → Q is not the same as Q → P.

## Part 3: Exercises

1. Build the truth table for `¬(P ∧ Q)` and for `¬P ∨ ¬Q`. Are they equivalent?
2. Show that `P → Q` and `¬Q → ¬P` are equivalent.
3. Give values of P and Q where `P → Q` is true but `Q → P` is false.
4. Let A = "the registry accepts register(noun, carrier)" and Q = "the carrier
   is the field or an already registered noun". The registry guarantees
   `A → Q`. Is `Q → A` also guaranteed? (Hint: lesson 1 registers `ball` in the
   red box, then tries `ball` in the blue box, which is registered.)

## Part 4: The input gate as a formula

Lesson 2 (`DiStudentInput`) accepts a submission when all three hold:

- C: the claim is not empty
- E: the evidence is an entry in the registry
- S: the step number is exactly one more than the last accepted step

So **good = C ∧ E ∧ S**, and **refused = ¬(C ∧ E ∧ S) ≡ ¬C ∨ ¬E ∨ ¬S**.

5. For the six submissions in lesson 2, give the values of C, E, S for each and
   check that the Instructor's verdict equals C ∧ E ∧ S.
6. A refused submission may fail more than one condition (for example an empty
   claim and unregistered evidence together). The Instructor reports only one reason.
   Which one, and in what order does it test them?
7. If the gate reported every failed condition, would the accept/refuse
   verdict change? Why or why not?

## Part 5: What a refusal proves

A refusal says a condition failed in the declared field. It does not say the
claim is false in the world. A missing registry entry is "not observed", which
is not the same as "does not exist". Treat an unobserved noun as unknown (a
third state), not as false.

8. In lesson 5 the chain for `order 9` prints "not observed". Which is the
   right reading: "order 9 does not exist" or "order 9 is not in this field"?

## Answer key

1. Both columns are: (T,T) F; (T,F) T; (F,T) T; (F,F) T. They are equivalent
   (De Morgan).
2. P→Q is false only when P=T, Q=F. ¬Q→¬P is false only when ¬Q=T and ¬P=F,
   that is Q=F and P=T. Same single row, so equivalent.
3. P = F, Q = T: `P → Q` is true (false antecedent) and `Q → P` is false.
4. No. `A → Q` holds because a carrier that fails Q is always refused. But the
   second `ball -> blue box` has Q true (blue box is registered) and is still
   refused, because ball already has a place. So Q is necessary, not
   sufficient: `Q → A` is the converse and does not hold.
5. The values (C, E, S) with last accepted step before each submission:
   1. step 1, last 0: (T,T,T) good
   2. step 2, empty claim, last 1: (F,T,T) refused
   3. step 2, evidence "green box", last 1: (T,F,T) refused
   4. step 3, last 1: (T,T,F) refused
   5. step 2, last 1: (T,T,T) good
   6. step 3, last 2: (T,T,T) good
   In every row the verdict equals C ∧ E ∧ S.
6. It tests in order: empty claim, then evidence not registered, then step
   order. It reports the first failure only.
7. No. The verdict is the same, because refused ≡ ¬C ∨ ¬E ∨ ¬S is true when
   any one condition fails. Only the explanation would be richer.
8. "order 9 is not in this field." The Instructor can say nothing more.
