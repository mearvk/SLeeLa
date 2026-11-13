# Lesson 04 — The high speed trader (advanced-medium)

> Series: [index](README.md) · prev: [03](03-counsel-choosing-an-approach.md) · next: [05 — Pragmatics and evolution](05-pragmatics-and-evolution.md)

Model: [models/high-speed-trader](models/high-speed-trader/README.md). Goal: at
least **5,000 evaluations per second** and about **2,000 trades per minute**
(roughly 0.7% of candidates), on synthetic data.

## What you do

1. Run it: `sleela run high-speed-trader.sleela` from the model folder.
2. Read section 2 of the output: evals/s, trades per simulated minute, vetoes by phase.
3. Find where 99% of candidates die. Most are score rejects and logic vetoes.
   Why is that the right place for them to die?

## Why the numbers fit together

5,000 evaluations per second at 2,000 trades per minute means about 33 trades per
second, so the trader must say **no** about 150 times for every yes. Speed is
mostly cheap rejection: hard gates first, scoring only for survivors, and a token
bucket so a burst of good candidates cannot exceed the cap.

## Exercises

- Change the target to 600 trades per minute. What happens to the threshold?
- Raise `evalsPerSimSecond` to 20,000. Does the model keep up in wall time?
- Why does P = 24 cost more per evaluation than P = 8?

## Advanced

Make the veto path faster: reorder so the most frequent veto comes first. Measure.
