# Correctness and Cost

## Loop invariants used here

| routine | invariant | why it ends |
|---------|-----------|-------------|
| `indexOf` | no earlier entry equals the noun | index rises to the fixed count |
| Euclid `gcd` | gcd(x, y) = gcd(a, b) | the remainder strictly falls and stays at least 0 |
| `isqrt` | lo x lo <= n < (hi + 1)^2 | hi - lo strictly shrinks |
| fewest shares to cover | the answer lies in [lo, hi] | hi - lo strictly shrinks |
| time-stop loop | shares held fall by the quantity sold | the old quantity strictly falls |

A loop with no stated ending argument is a loop that can hang. One deliberately
broken search (midpoint rounded up) never terminated; the test runner now has a
timeout so that a hang is a failure, not a stall.

## No cycles by construction

A noun is accepted only if its carrier is the field or an already registered noun.
So every carrier was registered earlier than the noun it carries, and following
carriers moves to strictly earlier entries. A cycle would need an entry earlier
than itself.

## Cost

| operation | cost (n entries, d chain depth) |
|-----------|---------------------------------|
| lookup by name | O(n) |
| register | O(n) |
| chain of a noun | O(d x n) |
| total under a carrier | O(n^2 x d) |
| compare two registries of n and m entries | O(n x m) |
| exact fill, closed form | O(log) |
| exact fill, plain scan | O(c / a) |

The closed form matters when the order c is large and the lot a is small: the scan
grows with the order, the closed form does not.

## Questions

1. Name the quantity that strictly decreases in Euclid's loop.
2. When is an O(n) lookup acceptable?

**Answers:** 1. The second number y (it becomes x mod y, which is below y).
2. When n is small and bounded (here 32 nouns); simple and clearly correct comes
first, and you measure before optimizing.
