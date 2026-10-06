# FIDUCIARY

*A duty model, and therefore a Purpose.*

## Intro — the duty model, and therefore Purpose

A fiduciary model begins not with an asset but with a **duty**: a party (the
fiduciary, or steward) holds and administers an **account** — an abstract
position of value — on behalf of a beneficiary, and is bound to act in that
beneficiary's interest rather than its own. Throughout this document an
"account" is exactly that: an abstract, ledgered store of value (an asset or
position), never a person and never a claim over one. The duty attaches to the
account, and it is a duty of care, loyalty, and honest valuation. The steward
does not *own* the beneficiary; the steward owns only the obligation to keep the
account's value truthfully stated and prudently managed.

From that duty we derive what it costs to **hold ownership of an account** — the
cost of stewardship. Holding an account is not free: it carries a carrying cost
(custody, risk, and the opportunity cost of capital) that accrues continuously
against the account's wealth. We write the account's wealth as `W(t)`, a value in
a stated currency at time `t`. The steward's first quantitative duty is simply to
know `W(t)` at all times and to state it without distortion — the same honest
valuation the goods & services basket already demands elsewhere in this
repository (see `GOODS.AND.SERVICES.md`), where each item is priced explicitly
rather than asserted.

The dynamics of that wealth are the heart of the model, and they are measured in
rates. The **first rate**, `dW/dt`, is the change in the account's wealth **per
second** — the instantaneous flow of value into or out of the account (income,
yield, cost, and loss netted together). The **second rate**, `d²W/dt²`, is the
change of that flow **per second per second** — the acceleration of wealth,
telling the steward not just whether the account is gaining or losing but whether
that gain or loss is itself speeding up or slowing down. A prudent steward reads
both: the velocity of wealth and its acceleration. The numbers for *holding an
account* are these rates and the carrying cost that offsets them — and they are
numbers about the account, not about any person.

Wealth in this model is **bounded**. An account is not permitted to grow or
shrink without limit; the steward operates it within an explicit envelope,
`W_min ≤ W(t) ≤ W_max`, and within a bounded rate `|dW/dt| ≤ R_max` and bounded
acceleration `|d²W/dt²| ≤ A_max`. In early accounting the bounds are kept
deliberately modest — magnitudes on the order of **5 to 8 significant digits**
rather than arbitrarily large figures — so that `W(t)` and its rates stay legible
and auditable before any later scaling is admitted. Bounded wealth is what makes the duty
enforceable: it converts "act in the beneficiary's interest" into checkable
constraints — the account may not be drained faster than `R_max`, may not be
inflated past `W_max`, and its trajectory may not accelerate past `A_max`. When a
bound would be crossed, the duty is to halt and report, exactly as the HTTP 3.0
pipeline halts on a failed integrity check rather than proceeding.

**Therefore, Purpose.** The purpose of the fiduciary model is to make a duty
*computable*: to reduce "care faithfully for what is held in trust" to a small
set of quantities — the wealth `W(t)`, its per-second rate `dW/dt`, its
per-second-per-second acceleration `d²W/dt²`, the carrying cost of stewardship,
and the bounds that fence them all in — that can be measured, audited, and
enforced continuously. A duty that cannot be measured cannot be kept; a duty
expressed in these bounded rates can be. That is the Purpose: a stewardship of
abstract accounts that is honest by construction, bounded by design, and
accountable second by second.


## Lifetime Bound & Bounding Insignia

A natural ceiling on any account held for an individual is a **single
lifetime**: an account-holder should not spend more, across their life, than the
wealth available to them within that one lifetime. This is a generic
personal-finance constraint — it applies to any holder, and says nothing about
who a holder is. Let `L` be the lifespan and `W_life` the total wealth available
over `[t_0, t_0 + L]`. The lifetime bound is simply that cumulative outflow stays
within that lifetime wealth:

```text
    ∫[t_0 .. t_0 + L]  outflow(t) dt   ≤   W_life
```

Because spending is a component of `dW/dt`, the lifetime bound caps the
*sustainable* per-second spend and its acceleration. Holding the average spend
rate within `W_life / L` keeps the holder inside the bound for the whole life;
exceeding it borrows against time the holder does not have. The second rate
`d²W/dt²` matters too: a spend rate that is itself accelerating will breach the
lifetime ceiling early even if today's rate looks affordable.

The **bounding insignia** are the named markers that flag a holder's standing
against these limits — an approximate, promised ceiling ("as of that
approximate promise"), not a hard oracle. Each insignia is a badge the steward
raises as the holder approaches or crosses a bound:

| Insignia | Symbol | Bound it marks | Raised when |
|---|:--:|---|---|
| **Lifetime** | `W_life` | total spend over one lifetime | cumulative outflow nears `W_life` |
| **Velocity** | `R_max` | per-second spend rate | `|dW/dt|` nears `R_max` |
| **Acceleration** | `A_max` | per-second-per-second change | `|d²W/dt²|` nears `A_max` |
| **Floor** | `W_min` | minimum retained wealth | `W(t)` nears `W_min` |
| **Ceiling** | `W_max` | maximum admitted wealth | `W(t)` nears `W_max` |

The duty is the same as elsewhere in this model: when an insignia would be
crossed, the steward halts and reports rather than proceeding. The lifetime
insignia is the outermost of these — a promise, kept approximately and honestly,
that an account is not spent beyond the one lifetime it belongs to.
