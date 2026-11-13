# Unit 3 (Adults): Processes and Algorithms

**Outcomes:** read an algorithm as sequence, selection and iteration; state a
loop invariant; argue termination and correctness.
**Companions:** [lessons 1, 2 and 5](../../series/README.md).

## Part 1: The three structures

- **Sequence:** steps run one after another.
- **Selection:** choose a path by a condition (`if`).
- **Iteration:** repeat while a condition holds (`while`).

Lesson 1's lookup (`indexOf` in `SLNounRegistry`) uses all three:

```
i = 0
while i < count:
    if nouns[i] == noun: return i
    i = i + 1
return -1
```

## Part 2: Correctness

**Specification.** `indexOf(noun)` returns the index of the noun if it is
registered, otherwise -1.

**Loop invariant.** At the top of each iteration, none of `nouns[0..i-1]` equals
`noun`.

- Initially (i = 0): the range is empty, so the invariant holds.
- Maintenance: if `nouns[i] != noun`, then after `i = i + 1` the longer range
  still has no match.
- On `return i`: `nouns[i] == noun`, so the answer is right.
- On exit with `i == count`: the invariant says no registered noun matches, so
  -1 is right.

**Termination.** `i` increases by 1 each time and is bounded by `count`, which
does not change inside the loop. So the loop ends.

## Part 3: A process with a guard

`chain(noun)` walks from a noun up to the field:

```
s = noun; cur = noun
while cur != field and guard <= count:
    cur = carrierOf(cur)
    s = s + " -> " + cur
    guard = guard + 1
```

The registry only accepts a carrier that is the field or **already registered**,
so every carrier was registered earlier than the noun it carries. Following
carriers therefore moves to strictly earlier entries and must reach the field.
The `guard` is a second line of defence, not the reason the loop ends.

## Part 4: Exercises

1. State a loop invariant for `carries(carrier)` (count nouns whose carrier
   matches).
2. Why can the registry never contain a cycle (a → b → a)? Use the order of
   registration.
3. Lesson 2's gate tests the claim, then the evidence, then the step order.
   Would swapping the order of the tests change which submissions are accepted?
   Would it change the reported reason?
4. Write the steps of `totalUnder(carrier)` in plain words. What does it return
   for a carrier that carries nothing?
5. `valueOf` returns -1 for a noun that is not observed. When is that a design
   risk, and how does the code reduce it?

## Answer key

1. After processing the first `i` entries, `n` equals the number of those
   entries whose carrier matches.
2. A cycle needs a noun whose carrier is registered after it. The registry
   accepts a carrier only if it was registered before the noun, so every
   carrier index is smaller than the noun index. A cycle would need an index
   smaller than itself.
3. No change to which are accepted (the verdict is C ∧ E ∧ S). It would change
   the reported reason when more than one test fails.
4. Start the sum at 0. For each registered noun, if the carrier appears
   anywhere on its chain, add its value. Return the sum. A carrier that carries
   nothing gives 0.
5. A real stored value of -1 (for example a one-cent discount) could not be told
   apart from "not observed". The code comment says to call `has()` first, and
   lesson 6 avoids the problem by summing with `totalUnder`, which reads the
   stored values directly and never treats -1 as a sentinel.
