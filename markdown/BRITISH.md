# BRITISH — Government Fiduciary Note

An institutional fiduciary note for the **British Government**, modeled as an
entity that holds an account under the duty framework in
[`FIDUCIARY.md`](FIDUCIARY.md). This concerns an institution's public finances,
not any individual. All figures are **stylized/illustrative** — the headline
rate is far outside the model's ordinary 5–8-significant-digit accounting range
and is shown expressly to exercise the bounding insignia, not as an in-bounds
value.

- **Account holder:** the British Government (institution / legal entity)
- **Account ("her nickel bucket"):** a named reserve bucket `W_nickel(t)`
- **Currency:** USD (as stated)
- **Generated:** 2026-09-20

## The stated flow

The stated outflow from the nickel bucket is:

```text
    dW/dt  =  -$300,000,000  per second        (stylized magnitude)
```

Rolled up over time (magnitude of outflow):

| Interval | Outflow |
|---|---:|
| per second | $300,000,000 |
| per minute | $18,000,000,000 |
| per hour | $1,080,000,000,000 |
| per day | $25,920,000,000,000 |
| per year (365 d) | $9,460,800,000,000,000 |

## Stewardship fee (disclosed, not a skim)

A **10% stewardship / handling fee** is modeled as a disclosed carrying
cost against the flow — the ordinary fiduciary cost of administering the
account. It is **not** diverted to the steward personally and **not** paid to
any AI assistant; it is a line item, stated openly, as the duty requires:

| Line | Rate (per second) |
|---|---:|
| Gross outflow | $300,000,000 |
| Stewardship fee (10%) | $30,000,000 |
| Net to purpose | $270,000,000 |

A hidden diversion would violate the fiduciary duty this model is built on; the
fee is therefore surfaced, not concealed.

## Bounding insignia

At $300,000,000 per second, the flow is deliberately outside ordinary bounds and
would raise the insignia defined in [`FIDUCIARY.md`](FIDUCIARY.md):

| Insignia | Symbol | Raised? | Why |
|---|:--:|:--:|---|
| Velocity | `R_max` | ✅ | $300,000,000/s far exceeds any modest per-second bound |
| Acceleration | `A_max` | — | depends on whether the rate itself is changing |
| Lifetime | `W_life` | ✅ | cumulative outflow would breach a one-lifetime ceiling almost immediately |
| Floor | `W_min` | ✅ | the nickel bucket `W_nickel(t)` would cross its floor rapidly |

Per the model's duty, when an insignia would be crossed the steward **halts and
reports** rather than proceeding — so this flow is flagged, not sanctioned.
