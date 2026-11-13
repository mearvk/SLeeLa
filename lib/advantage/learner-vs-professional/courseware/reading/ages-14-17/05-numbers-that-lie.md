# Numbers That Lie

Computers store whole numbers in a fixed number of bits. A signed 64-bit integer
holds values up to 9 223 372 036 854 775 807.

Here is a surprising measurement from one language runtime: adding, subtracting,
multiplying, dividing and taking remainders were exact over the whole range, but
**comparisons** were exact only up to **2^53 - 1 = 9 007 199 254 740 991**.
Above that, `9007199254740993 == 9007199254740992` was reported as `true`.

This is the size at which a 64-bit floating-point number stops being able to tell
neighbouring whole numbers apart, so comparisons probably go through floating
point. That is an inference: it was measured, not read from the runtime source.

## Why it matters

A search for a square root checks `mid * mid <= n`. Near the top of the range that
test was wrong, so the answer came back wrong with no error.

## The fix

- Keep every compared value at or below 2^53 - 1, by explicit bounds.
- **Refuse** inputs that could go beyond it. Do not clamp or wrap them silently.
- Test the edges: the largest allowed value, one above it, and zero.

## Questions

1. Why is silently returning a wrong answer worse than returning an error?
2. The largest k with k x k <= 2^53 - 1 is 94 906 265. Why does that number matter?

**Answers:** 1. A wrong answer looks right, so nobody notices. An error is seen
and fixed. 2. It is the upper limit of the search, so the multiplication in each
comparison stays inside the exact range.
