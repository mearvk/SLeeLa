# Lesson 4 — Brightness, Warmth, and Gamma

The Ballum facade documents these ranges:

| Control | Input range | Meaning |
|---|---:|---|
| Brightness | 0–100 | Percent |
| Warmth | 1000–10000 | Color temperature in kelvin; 6500 K is approximately neutral daylight |
| Gamma | 10–1000 | Encoded as gamma × 100; 220 means 2.20 |

Capability bits are `4` for brightness, `8` for warmth, and `16` for gamma. Check the relevant bit before calling a control. The native backend may reject a control even when the general API exists, so always inspect the returned status.

Run [03-display-controls.sleela](../examples/03-display-controls.sleela) only when you are comfortable changing visible display settings. Restore your preferred settings afterward if necessary.
