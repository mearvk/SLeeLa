<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa

## Editor's Note

**Source is now carefully Open.**

SLeeLa is developed as an inspectable software project. Source, interfaces, implementation boundaries, build methods, tests, and engineering documentation are maintained openly so that the work can be examined, built, tested, and improved with care.



## SLeeLa Standard Library

The canonical SLeeLa-facing source collection is maintained under `/lib`. The library is the source-level package surface used by the SLeeLa compiler and loader rather than a documentation-only catalog.

The current development inventory records:

- **74 packages**
- **953 SLeeLa source units**
- **1,041 total symbol records**
- **88 module-facade symbols**

The library includes the current language/runtime foundations together with package families such as regex, video, VM, reflection, audio, networking, synchronization, media, and other repository subsystems. Package-specific C/C++/Java implementations remain native/backend layers where appropriate; the `/lib` sources provide the corresponding SLeeLa language objects and package-facing contracts.

### Compiler and Loader

The compiler and loader treat `/lib` as an explicit library-resolution surface. New SLeeLa library sources are expected to participate in:

`source → package/library resolution → semantic analysis → compilation → artifact/loader resolution`

Library inventory and symbol-resolution information is kept synchronized with the compiler compatibility gate so that newly added SLeeLa classes and package facades are visible to tooling rather than remaining isolated source files.

### SST and Nordshrift

SST and Nordshrift use the repository's library inventory as part of their compiler-facing symbol and package surface. The canonical collection is intended to provide:

- package-to-source resolution;
- symbol-to-source resolution;
- module-facade discovery;
- compiler compatibility checks;
- loader visibility checks; and
- a reproducible inventory of the SLeeLa standard-library surface.

This keeps the SLeeLa source layer, compiler, loader, SST, and Nordshrift representations aligned as the library grows.

## Audio and Codec Architecture

The Audio work is organized by implementation language and responsibility:

- `audio/c/` — C11 implementation and library interface.
- `audio/cpp/` — C++17 implementation and typed API.
- `audio/java/` — Java 21 orchestration and native-process boundary.
- `audio/native/` — native media-processing implementation.
- `audio/gui/` — JavaFX presentation and integration.
- `codecs/` — codec standards registry, handler API, capability metadata, and codec conformance work.

### Major Sound Standards

The new `/codecs` registry provides explicit coverage for major audio standards and formats:

- PCM/WAV
- AIFF
- FLAC
- ALAC
- MP3
- AAC
- HE-AAC
- Vorbis
- Opus
- Speex
- WMA
- AC-3
- E-AC-3
- AMR-NB
- AMR-WB
- G.711 μ-law
- G.711 A-law
- MIDI
- Matroska Audio
- WebM Audio

The codec registry distinguishes **Native**, **Backend**, **Recognized**, **Container**, and **Event** capabilities. Listing a codec does not by itself claim that an encoder or decoder is already implemented.

The intended audio path is:

`codec/container → handler → PCM boundary → Audio API`

and, for encoding:

`Audio PCM → handler → codec/container output`

The codec layer remains separate from the Audio mixer so that validation, decoding, encoding, and media processing have clear boundaries.

See `codecs/README.md` and `codecs/CODECS.md` for the detailed registry, capability definitions, implementation order, security requirements, and licensing considerations.

The phrase **carefully Open** is intentional: openness includes clear interfaces, explicit implementation boundaries, reproducible builds, validation, tests, and documentation of unfinished areas. It does not imply that an implementation is complete merely because its source is visible.

— Editor's Note, SLeeLa