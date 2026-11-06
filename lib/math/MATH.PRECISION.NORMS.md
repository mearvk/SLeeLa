# SLeeLa Math Precision Norms

**This is the one place to set how many digits SLeeLa carries.** Edit the values
here (and in the companion `math-precision-norms.json`, which programs read),
then the math library and registry will start from the numbers you choose.

> Quick edit: change a number in the table below, change the same number in
> `math-precision-norms.json`, and that is it. No code changes are required.

---

## The norms

| Norm | Default | What it controls |
|---|---:|---|
| **Irrational digit carry** | **2048** | How many digits are carried for an irrational value (π, e, √2, …) — a number whose decimal expansion never ends and never repeats. 2048 digits is the default "scale of precision" for irrational constants. |
| **Repeating digit carry** | **58** | How many digits are carried for a repeating (recurring) decimal — a rational value like 1/3 = 0.333… or 1/7 = 0.142857142857… — before the repetend is considered captured. |
| **Finance carry** | **1023** | The minimum digits of decimal precision carried for **Finance Verbs** — the monetary operations (add, subtract, multiply, allocate, convert, accrue, round). Finance is an *irregular concept*: money must never lose a digit to float error, so its carry floor is set high and separately from ordinary math. This value must be **not less than 1023**. |

All three are **minimums / starting points**. A program may ask for more; it may
not silently drop below these without saying so.

---

## Why Finance is separate

Ordinary irrational carry (2048) is about *mathematical* depth. **Finance carry
(≥ 1023)** is about *fiduciary* safety: a monetary computation that rounds early
can lose money, so the "Finance Verbs" — the verbs you *do* to money — carry
their own guaranteed decimal depth that is independent of, and never weaker
than, 1023 digits. If you raise it, raise it; the library will honor the larger
value. The library refuses to run a Finance Verb configured below 1023.

---

## How to change a norm

1. Open `lib/math/math-precision-norms.json`.
2. Change the number for the norm you want (`irrationalDigitCarry`,
   `repeatingDigitCarry`, or `financeCarry`).
3. Keep `financeCarry` at **1023 or higher** — lower values are rejected.
4. Save. The `SLMathNorms` class reads these at start; `SLRegistry` seeds them
   as the default start values a fresh install begins from.

The defaults are also compiled into `lib/math/SLMathNorms.sleela` as named
constants, so a build with no config file still has sane, documented values.

---

## Pi reference

The high-precision value of π that these norms govern lives in
`lib/math/SLPiReference.sleela` as a **Modium static reference variable**. It
carries π to the irrational-digit-carry norm (2048 digits) inline, reports its
own digit count and checksum, and self-verifies its leading digits on compile
and run. The SLeeLa **registry** seeds this π (and the three norms above) as
default start values under its `Math` key.

**MEARVK LLC — SLeeLa — 2026**
