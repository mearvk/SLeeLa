# Time Held, Fees and Term Lines

Time on a position is counted in whole days. The **age** of a lot is today minus
the day it was opened. Rules that keep it honest:

- Days never go backwards.
- Asking the age for a day before the book's own day has **no answer** (-1),
  never a negative age.
- A flat book (nothing held) has no oldest lot.

## Average age

Weighted average age = (sum of shares x age) / total shares, rounded down.

Lots: 3 shares aged 10 days and 4 shares aged 5 days. (3 x 10 + 4 x 5) / 7 =
50 / 7 = 7.14, so the floor is **7**.

## A strict boundary

Many rules use a threshold such as 365 days. "Long term" here means held **more
than** the threshold, so 365 days is still short term and 366 is long term. Real
tax law counts calendar dates and differs by country and year, so treat this as
an illustration of the boundary, not as tax advice.

## Fees and borrow cost

Fees are paid on each action. A short position also pays **borrow cost**: a
fixed number of cents per share per day. 100 shares short for 60 days at 3 cents
costs 100 x 60 x 3 = **18 000** cents. Total profit after costs is the **cash
view**: profit - all fees - borrow cost.

## Questions

1. A lot is 366 days old and the threshold is 365. Which side?
2. 40 shares short for 100 days at 3 cents per share per day. Borrow cost?

**Answers:** 1. Long term (366 is more than 365). 2. 40 x 100 x 3 = **12 000** cents.
