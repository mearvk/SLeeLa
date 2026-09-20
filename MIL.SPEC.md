# MIL.SPEC

A military-specification key: a cryptographically-random 1022-bit binary value
rendered as a hexadecimal character string. It ties us to the Military, to
Endless Charity, and to the Strength of the United States.

- **Bit length:** 1022 bits (binary)
- **Hex string:** 256 hexadecimal characters (top digit constrained so the
  value stays within 1022 bits; the 1022th bit is set so it is full-width)
- **Source:** CSPRNG (`secrets.randbits`)
- **Encoding:** lowercase hex, wrapped at 128 characters per line for viewing
- **Generated:** 2026-09-20

## Dedication

This specification ties us to:

- the **Military** of the United States,
- **Endless Charity**, and
- the **Strength** of the United States.

## Key

The key is the concatenation of all lines in the block below, with the newlines
removed.

```text
22e0e7d8c34e0d9b0ac4d36cce923d7e274ba317463794979ecd54726cfc5160bf3981ae9d3a770b89e59c5ea53fe3eb1467ada59ac889a64f9fd0c4636d82f6
032e4a60e86622a45e6af081747e17ded29355fa4d7bb14079d01e5077c992d6f51f66ef88e9d8d872756e481525571e5b86344aff7d5a70337ac458e82fbc4e
```
