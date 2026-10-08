# Nintendo Switch 2 — CPU Reference

## Hardware Profile

| Field | Reference |
|---|---|
| System | Nintendo Switch 2 |
| CPU/GPU | Custom NVIDIA processor |
| CPU architecture | Custom NVIDIA ARM-based design |
| CPU clock | Nintendo does not publish a public CPU clock specification |
| System memory | 12 GB LPDDR5X-class system memory; implementation-level details are not all publicly specified |
| Internal storage | 256 GB UFS |
| Launch | June 5, 2025 |
| Launch MSRP | $449.99 US at launch |

## SLeeLa Representation

The Switch 2 profile is deliberately conservative where Nintendo has not published low-level CPU clock details. The model should treat clock frequency as configurable/unknown rather than inventing a fixed frequency.

Nintendo officially specifies a custom NVIDIA processor and 256 GB UFS storage. Nintendo launched the system in the US at $449.99; Nintendo later revised the US MSRP to $499.99 effective September 1, 2026. citeturn0search2turn0search5turn0search1

## Compatibility Intent

The profile is intended for modern ARM-console architecture research, memory modeling, GPU/CPU scheduling studies, and SLeeLa guest-machine abstractions. It does not contain proprietary firmware.

## Cost Note

The documented launch price is $449.99 US. The current US MSRP after Nintendo's September 2026 price revision is $499.99.
