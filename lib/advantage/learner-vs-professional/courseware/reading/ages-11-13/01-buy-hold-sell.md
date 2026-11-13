# Buy, Hold, Sell

Imagine a card game where cards can be bought and sold between players.
Three verbs describe what you do: **buy**, **hold** and **sell**.

Owning something you expect to sell later is called being **long**. You can only
sell what you actually have. A rule of a long-only book: **you cannot sell more
than you hold.** If you try, the answer is "refused", and nothing changes.

## Lots

Each buy is its own **lot**: how many, at what price, on what day.

| day | action | cards | price each |
|-----|--------|-------|-----------|
| 1 | buy | 10 | 5 coins |
| 3 | buy | 10 | 8 coins |

## Selling oldest first

On day 5 you sell **12** cards at **9 coins** each. Sell the **oldest lot first**
(first in, first out):

- 10 cards from the day-1 lot cost 10 x 5 = 50
- 2 cards from the day-3 lot cost 2 x 8 = 16
- total cost of the 12 cards sold: **66**
- money from the sale: 12 x 9 = **108**
- profit: 108 - 66 = **42 coins**

What is left? 8 cards from the day-3 lot, bought at 8 coins each.

## Hold

**Hold** means let time pass and do nothing. Days only go forward.

## Questions

1. You hold 8 cards. Can you sell 9? Why or why not?
2. Sell 5 of the remaining cards at 7 coins. What is the profit on those 5?

**Answers:** 1. No: you cannot sell more than you hold. The answer is "refused".
2. Cost 5 x 8 = 40, sale 5 x 7 = 35, profit 35 - 40 = **-5** (a loss of 5).
