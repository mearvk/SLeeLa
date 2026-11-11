# Euclid and the Biggest Shared Factor

You have **24** rare cards, **36** common cards and **60** special cards. You
want to make identical packs with nothing left over. How many packs can you make
at most?

The answer is the **greatest common divisor** (gcd) of 24, 36 and 60.

## Euclid's loop (about 300 BC)

To find gcd(a, b): while b is not 0, replace (a, b) by (b, a mod b). When b hits
0, a is the answer. ("mod" means the remainder after dividing.)

gcd(24, 36):

| a | b | remainder of a / b |
|---|---|--------------------|
| 24 | 36 | 24 |
| 36 | 24 | 12 |
| 24 | 12 | 0 |
| 12 | 0 | stop |

gcd = **12**. Now gcd(12, 60): 12 divides 60 exactly, so the answer stays **12**.

Each pack gets 24/12 = **2** rare, 36/12 = **3** common, 60/12 = **5** special.
The ratio is **2 : 3 : 5**, and 12 packs use everything.

## Why the loop must stop

The remainder is always **smaller** than b, and it never goes below 0. A number
that gets strictly smaller and cannot go below 0 must reach 0. So the loop ends.

## Questions

1. gcd(18, 48)?  2. Reduce the ratio 18 : 48.  3. gcd(7, 13)?

**Answers:** 1. (18,48) -> (48,18) -> (18,12) -> (12,6) -> (6,0): **6**.
2. 3 : 8.  3. 1 (they share no factor except 1), so 7 : 13 cannot be reduced.
