# Euclid, Bezout and Exact Fills

**Bezout's identity:** for whole numbers a and b with gcd g there are whole
numbers x and y with **a x + b y = g**. Euclid's loop, run with two extra
columns, finds them.

Example: a = 240, b = 46. The loop gives gcd = **2**, with **x = -9** and
**y = 47**:

240 x (-9) + 46 x 47 = -2160 + 2162 = **2**. Check it with one multiplication.

## Exact fills

An order of c shares can be filled with whole lots of size a and size b if
a x + b y = c has non-negative whole solutions.

- **Necessary:** gcd(a, b) must divide c. With lots 20 and 30, gcd = 10, so
  c = 75 is impossible.
- **Not sufficient:** c = 10 passes the gcd test but is smaller than both lots.
- c = 70 works: 2 x 20 + 1 x 30.

A slow way is to try 0, 1, 2, ... lots of size a. A fast way uses the modular
inverse from Bezout and answers in a few steps even for an order of a trillion.
Good programs keep the slow way too, as a **reference**, and test that the two
agree.

## Questions

1. With lots 20 and 30, can c = 110 be filled? Give a fill.
2. Check Bezout for a = 20, b = 30, x = -1, y = 1.

**Answers:** 1. Yes: 1 x 20 + 3 x 30 = 110. 2. 20 x (-1) + 30 x 1 = 10 = gcd. Yes.
