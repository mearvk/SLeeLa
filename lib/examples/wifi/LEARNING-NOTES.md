# Wi-Fi examples — what to learn here

These examples teach the **decisions and arithmetic** behind Wi-Fi: turning a
channel number into a frequency, applying a security policy, and planning
channels so access points do not interfere. They do **not** access a physical
radio — reaching real hardware requires the native SLVM bridge the Wi-Fi
subsystem README documents. The three new examples are self-contained and
deterministic.

Read them in order.

## The cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-channel-frequency.sleela`](BASIC-channel-frequency.sleela) | Channel → center frequency for the 2.4 GHz band: `2407 + 5 * channel`. |
| Moderate | [`MODERATE-security-policy.sleela`](MODERATE-security-policy.sleela) | A safe-default policy that denies open/WEP and anything below WPA2. |
| Advanced | [`ADVANCED-nonoverlap-plan.sleela`](ADVANCED-nonoverlap-plan.sleela) | Detect co-channel/adjacent interference and explain why {1, 6, 11} is the clean set. |
| Smoke | [`USED-wifi-policy-smoke.sleela`](USED-wifi-policy-smoke.sleela) | Exercises the real `SLWifi*` library classes. It is a *library* example — validate it alongside its subsystem, not with `check` on its own. |

## What we want you to take away

1. **A channel is a frequency in disguise.** The basic example shows the exact
   linear rule for the 2.4 GHz band (channel 1 = 2412 MHz, 6 = 2437, 11 = 2462).
   Knowing the rule makes the interference math obvious.
2. **Security is policy, not vibes.** The moderate example encodes a real-world
   safe default: open networks and legacy WEP are rejected outright, and a
   minimum strength (WPA2) is required. Each denial gives a specific reason —
   good security tooling always tells you *why*.
3. **Why {1, 6, 11}?** In the 2.4 GHz band channels are 20 MHz wide but spaced
   only 5 MHz apart, so two APs interfere unless their channel numbers differ by
   at least 5. The advanced example checks every pair and flags overlaps; 1, 6,
   and 11 are the three channels that are mutually ≥5 apart, which is why they
   are the standard non-overlapping plan.

## Practical notes for SLeeLa 1.7

- **Object arrays** use the standard idiom (static-field array, elements built in
  typed locals and read back through typed locals) — see the advanced plan.
- **Ranks and reason codes** are small integers compared with `==`/`<`, a tidy
  way to express a policy without strings.
- **The smoke example is different.** `USED-wifi-policy-smoke.sleela` calls the
  real library classes and has no `main`, so running `sleela check` on it alone
  reports "no main method" — that is expected for a library-style example.

## Run them

```sh
sleela run lib/examples/wifi/MODERATE-security-policy.sleela
sleela run lib/examples/wifi/ADVANCED-nonoverlap-plan.sleela
```

Output is deterministic: channel 1 = 2412 MHz; the policy allows only WPA2/WPA3;
and the advanced plan finds 2 interfering pairs caused by the AP on channel 4.
