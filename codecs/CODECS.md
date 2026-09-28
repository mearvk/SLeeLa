# SLeeLa Codec Standards Registry

**Date:** 2026-09-28

## 1. Purpose

This document establishes the major audio-codec coverage expected by SLeeLa and separates codec identification from actual encode/decode implementation.

## 2. Registry

| ID | Standard | Extensions / container | Primary use | Current handler |
|---|---|---|---|---|
| PCM_WAV | PCM / RIFF WAVE | .wav | Uncompressed PCM | Native |
| AIFF | Audio Interchange File Format | .aif, .aiff | PCM/container | Recognized |
| FLAC | Free Lossless Audio Codec | .flac | Lossless | Backend |
| ALAC | Apple Lossless | .m4a, .caf | Lossless | Backend |
| MP3 | MPEG Layer III | .mp3 | Lossy | Backend |
| AAC | MPEG AAC | .aac, .m4a | Lossy | Backend |
| HE_AAC | High-Efficiency AAC | .aac, .m4a | Low-bitrate lossy | Backend |
| VORBIS | Ogg Vorbis | .ogg | Lossy | Backend |
| OPUS | Opus | .opus, .ogg, .webm | Speech/music | Backend |
| SPEEX | Speex | .spx, .ogg | Speech | Backend |
| WMA | Windows Media Audio | .wma | Lossy/lossless family | Backend |
| AC3 | Dolby Digital AC-3 | .ac3 | Multichannel | Backend |
| EAC3 | Enhanced AC-3 | .eac3, .ec3 | Multichannel | Backend |
| AMR_NB | AMR Narrowband | .amr | Speech | Backend |
| AMR_WB | AMR Wideband | .awb | Speech | Backend |
| G711_MULAW | ITU-T G.711 μ-law | .au, .ulaw | Telephony | Backend/interop |
| G711_ALAW | ITU-T G.711 A-law | .au, .alaw | Telephony | Backend/interop |
| MIDI | MIDI | .mid, .midi | Musical events | Event format |
| MATROSKA_AUDIO | Matroska | .mka | Container | Container |
| WEBM_AUDIO | WebM | .webm | Container | Container |

## 3. Capability Semantics

The registry distinguishes:

- **Recognized** — SLeeLa can identify the standard/format.
- **Backend** — SLeeLa has a handler contract but needs an approved codec implementation/library.
- **Native** — encode/decode exists inside the SLeeLa package.
- **Container** — the format carries audio but is not itself a compression codec.
- **Event format** — represents musical events rather than PCM samples.

This distinction is required for reliable capability reporting.

## 4. Implementation Order

1. Establish common handler ABI and registry.
2. Keep PCM/WAV on the native Audio API path.
3. Add lossless codecs: FLAC and ALAC.
4. Add common lossy codecs: MP3, AAC/HE-AAC, Vorbis, and Opus.
5. Add speech codecs: Speex and AMR-NB/AMR-WB.
6. Add telephony G.711 A-law/μ-law.
7. Add multichannel AC-3/E-AC-3.
8. Add WMA through a separately qualified backend where licensing/platform constraints permit.
9. Add container adapters for AIFF, Matroska, and WebM.
10. Add conformance fixtures for every codec actually implemented.

## 5. Security and Correctness

Every future decoder must:

- validate input sizes before allocation;
- reject malformed headers;
- bound channel counts and sample rates;
- detect integer overflow;
- enforce output-size limits;
- avoid unbounded recursion;
- fail closed on truncated input;
- report unsupported profiles explicitly;
- keep codec parsing separate from the Audio mixer.

## 6. Licensing

Codec support must not imply that a patent, trademark, proprietary SDK, or third-party implementation is included in SLeeLa. Each external backend must be reviewed for its license and distribution terms before it becomes a bundled dependency.

## 7. Status

The registry establishes coverage and dispatch contracts. It does not claim that every listed codec already has an in-tree encoder/decoder.

The next implementation work is codec-by-codec conformance, beginning with lossless formats and then the common lossy/speech families.
