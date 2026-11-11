# Unit 5 (Adults): Applied Decisions with the Onion

**Outcomes:** model a decision as onion layers; register the evidence first;
compare your result with an independent one; handle gaps honestly.
**Companions:** [lesson 6](../series/README.md) and the
[Advantage library](../../../README.md).

## Part 1: The method

1. Declare the field (for example "purchase of one item").
2. Register carriers and values as nouns. Money is integer cents.
3. Run the layers: dividend (starting value), review (re-read each entry),
   concerns (what is outside the field), additions, subtraction, regrouping
   (group by carrier), iteration (repeat if a second opinion differs).
4. Compare with an independent result. Agree, or re-observe the entry that
   differs.

## Part 2: The worked case (lesson 6)

| supplier | quote | fee | discount | total |
|----------|-------|-----|----------|-------|
| A | 12000 | 450 | -800 | 11650 |
| B | 11500 | 900 | 0 | 12400 |

Total = quote + fee + discount. A is cheaper by 750 cents.

The student first misses A's discount and gets A = 12450. A then looks dearer
than B (12400), which reverses the choice. The comparison shows the
difference is 800 cents on supplier A, so only that entry is re-observed.

## Part 3: Exercises

1. Supplier B adds a discount of -700. Totals? Which is cheaper?
2. Supplier B's discount is -750. Totals? Which supplier does lesson 6 pick on
   a tie, and why should a tie rule be written down?
3. A third quote (supplier C) is mentioned on a call but not in hand. How
   should it be treated in the analysis?
4. Where do the layers go? Place quote, fee and discount on the onion layers
   used by `DiPurchaseOnion`.
5. The student and the Purchase Master agree on the supplier but not on the
   total. Is the work accepted? Why?

## Answer key

1. B = 11500 + 900 − 700 = 11700. A (11650) is still cheaper, by 50 cents.
2. B = 11650, a tie with A. Lesson 6 follows the tie norm and picks supplier A
   (`a <= b` in `pick`). The norm is saved in
   `.kiro/steering/advantage-tie-norm.md`. A tie rule should be chosen on
   purpose, written down, and used by both the student and the Purchase Master.
3. As "not observed": outside the field, not compared. The lesson prints
   "supplier C is not observed, so it is not compared".
4. Quote is the dividend; the fee is the additions layer; the discount is the
   subtraction layer (as a negative number). The other layers are 0 in lesson 6.
5. No. Lesson 6 accepts only when the supplier and both totals match. A
   difference in total means an entry or a sum differs; re-observe it.
