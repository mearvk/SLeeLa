# CDMA examples — what to learn here

These examples teach the **arithmetic of a radio link**, ending with the ideas
that are special to CDMA (Code Division Multiple Access): spreading/processing
gain and cell capacity. Everything is a numerical illustration — **no modem or
radio is accessed** — so the programs are deterministic and run anywhere.

Read them in order; each adds one idea.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-link-budget.sleela`](BASIC-link-budget.sleela) | The one-line budget: received power = transmit power − path loss (in dB). |
| Moderate | [`MODERATE-link-margin.sleela`](MODERATE-link-margin.sleela) | A full budget with antenna gains, cable losses, EIRP, and a margin check that decides if the link closes. |
| Advanced | [`ADVANCED-processing-gain-capacity.sleela`](ADVANCED-processing-gain-capacity.sleela) | What makes CDMA CDMA: spreading/processing gain and an ideal pole-capacity estimate. |

The original [`CLASSIC-link-budget.sleela`](CLASSIC-link-budget.sleela) is the
shortest starter.

## What we want you to take away

1. **Radio engineering is addition and subtraction in decibels.** Because dB is a
   logarithmic unit, multiplying powers becomes *adding* dB values. The whole
   link budget is a running sum of gains (positive) and losses (negative).
2. **A link "closes" when the margin is non-negative.** The moderate example
   computes received signal minus receiver sensitivity. A positive margin means
   headroom; a negative margin means you must add power or antenna gain.
3. **CDMA trades bandwidth for sharing.** The advanced example shows the
   defining move: spread a 9.6 kbps voice signal across a 1.2288 Mcps chip
   stream. The ratio (128, i.e. ~21 dB of processing gain) is what lets many
   coded users share one frequency, and it directly bounds how many users a cell
   can hold.

## Practical notes for SLeeLa 1.7

- **No math library is linked in these examples.** There is no `log10` intrinsic,
  so the advanced example uses a tiny lookup table for the exact ratios it needs
  (128 → 21.07 dB) and documents that choice. For production code you would call
  the real math classes under `/lib/math` via the native bridge; these teaching
  files stay self-contained on purpose.
- **Negative numbers** are written as `0.0 - 110.0` where a literal `-110.0`
  would read awkwardly next to other terms; both forms are valid. dBm values are
  routinely negative (a −85 dBm signal is normal).
- **Mind your units.** The advanced example multiplies Mcps and kbps up to a
  common base (×1 000 000 and ×1 000) before dividing — a classic source of
  real-world link-budget errors.

## Run them

```sh
sleela check lib/examples/cdma/MODERATE-link-margin.sleela
sleela run   lib/examples/cdma/MODERATE-link-margin.sleela
```

Output is deterministic: the basic budget yields −85 dBm, the moderate budget a
+25 dB margin ("link closes"), and the advanced example a processing gain of
21.07 dB with an ideal pole capacity near 25.6 users per cell.
