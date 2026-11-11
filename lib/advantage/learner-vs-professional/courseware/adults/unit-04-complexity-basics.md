# Unit 4 (Adults): Complexity Basics

**Outcomes:** count the work of a procedure as input grows; read O-notation;
choose between a simple scan and a better structure.
**Companions:** [lessons 1, 4 and 5](../../series/README.md).

## Part 1: Counting steps

Count basic operations as a function of the input size n (here, the number of
registered nouns). O-notation keeps the dominant term and drops constants.

| procedure | what it does | cost in n (d = depth of the chain) |
|-----------|--------------|------------------------------------|
| `indexOf(noun)` | scan from the start | O(n) worst case, O(1) if first |
| `register(noun, carrier)` | one scan for the noun and one for the carrier | O(n) |
| `carries(c)` | one pass over all nouns | O(n) |
| `carrierOf(noun)` | one `indexOf` | O(n) |
| `chain(noun)` / `depthOf(noun)` | d steps, each a `carrierOf` | O(d·n) |
| `totalUnder(c)` | for each noun, walk its chain | O(n²·d) |
| lesson 4 `compare()` | for each noun in one registry, search the other | O(n·m) |

Here n and m are the sizes of the two registries. The shared registry holds at
most 32 entries, so none of these is a problem in practice. The point is to
learn how the cost grows.

## Part 2: Growth

| n | n | n² | n³ |
|---|---|----|----|
| 8 | 8 | 64 | 512 |
| 32 | 32 | 1,024 | 32,768 |
| 1,000 | 1,000 | 1,000,000 | 1,000,000,000 |

## Part 3: Exercises

1. A registry of 1,000 nouns is searched 1,000 times with `indexOf`. About how
   many comparisons in the worst case?
2. `chain` is O(d·n). If every noun is a direct child of the field, what is d
   and what does the cost become?
3. Lesson 4 compares two registries with n = m = 32. For each student noun the
   code calls `has` and then `carrierOf`, each a scan of the master. About how
   many comparisons in the worst case for the first loop?
4. Suggest a change that makes `indexOf` O(1) on average. What does it cost?
5. Is a faster `indexOf` worth it at 32 entries? When would it be?

## Answer key

1. About 1,000 × 1,000 = 1,000,000 comparisons.
2. d = 1, so the cost is O(n).
3. About 32 × (32 + 32) = 2,048 comparisons for the first loop, since each
   student noun triggers two scans. That is still O(n·m).
4. Keep a hash table from noun to index. It costs extra memory and code to keep
   the table in step with the arrays, and the language needs hash support.
5. At 32 entries no: the scan is simple and clearly correct. It matters when n
   grows to thousands or the lookup is called inside nested loops. Simple and
   correct comes first; measure before optimizing.
