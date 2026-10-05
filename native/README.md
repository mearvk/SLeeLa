<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Native Audio

C++17 native audio foundation behind the JavaFX Audio GUI.

## Implemented

- Stable C ABI in `include/sleela_audio.h`.
- PCM16 mono/stereo WAV input.
- Explicit sample-rate validation.
- Per-input synchronized start offset.
- Per-input gain.
- Master gain and pan.
- Stereo PCM16 WAV output.
- Bounded input count and fail-closed validation.

The GUI invokes the native command adapter rather than pretending that JavaFX itself is the mixer.

## Deliberate boundaries

Live-device capture, non-WAV codecs, DSP EQ, audio analysis, video frames, and platform-specific capture remain explicit adapters. Unsupported capabilities are rejected or remain unavailable rather than being simulated as completed native functionality.

# SLeeLa Native Cryptography Bridge

C++17 value primitives behind the `lib/crypto` general-cryptography sophistication
classes (`SLRadix`, `SLCryptoBlock`, `SLIntermix*`, `SLCryptoSeries`,
`SLCryptoComparator`, `SLNationalRegister`, ...).

- Stable C ABI in `include/sleela_crypto.h`.
- Mixed-radix converter across bases **1 to 2055** (unary, positional, delimited).
- Ordered block advance and the two internal intermix stages.
- Non-short-circuiting comparison gate.
- National-register call, fail-closed on empty submissions.

> **Security note.** These transforms are deterministic, reversible obfuscation —
> **not cryptography.** Mirrors the deprecated reference AES2 module. Use a vetted
> AEAD cipher (AES-256-GCM, ChaCha20-Poly1305) for real protection.

## Build

```sh
make
make test
```

`make test` builds and self-tests both the audio and crypto bridges.