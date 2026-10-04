<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Codec Handlers

**Package:** `codecs/`  
**Date:** 2026-09-28

This package defines the SLeeLa audio-codec handler registry. It provides one place to identify major audio coding standards, describe their containers/signatures, and expose whether a handler is natively implemented, requires an external codec backend, or is only recognized.

The codec registry is deliberately separate from `audio/`: the Audio API performs PCM/WAV mixing, while `codecs/` defines compressed-audio and container integration.

## Handler Classes

- **Native** — SLeeLa contains the codec implementation.
- **Backend** — the handler contract exists, but an approved codec library/backend is required for actual encode/decode.
- **Recognized** — the format can be identified and described, but encode/decode is not claimed.
- **Container** — identifies a container/transport rather than a compression algorithm.

No handler is advertised as fully implemented merely because its name appears in the registry.

## Major Standards Covered

| Handler | Standard / Format | Typical role | State |
|---|---|---|---|
| PCM/WAV | PCM in RIFF/WAVE | Uncompressed audio | Native audio path |
| AIFF | AIFF/AIFF-C | Uncompressed/container audio | Recognized |
| FLAC | Free Lossless Audio Codec | Lossless | Backend |
| ALAC | Apple Lossless Audio Codec | Lossless | Backend |
| MP3 | MPEG-1/2 Layer III | Lossy | Backend |
| AAC | MPEG-2/4 AAC | Lossy | Backend |
| HE-AAC | MPEG-4 HE-AAC | Lossy / low bitrate | Backend |
| Vorbis | Xiph.Org Vorbis | Lossy | Backend |
| Opus | IETF Opus / RFC 6716 | Speech/music | Backend |
| Speex | Xiph.Org Speex | Speech | Backend |
| WMA | Windows Media Audio | Lossy/lossless family | Backend |
| AC-3 | Dolby Digital | Multichannel | Backend |
| E-AC-3 | Dolby Digital Plus | Multichannel | Backend |
| AMR-NB | Adaptive Multi-Rate Narrowband | Speech | Backend |
| AMR-WB | Adaptive Multi-Rate Wideband | Speech | Backend |
| μ-law | G.711 μ-law | Telephony PCM companding | Backend/interop |
| A-law | G.711 A-law | Telephony PCM companding | Backend/interop |
| MIDI | MIDI Standard | Note/event data, not sampled audio | Event format |
| WebM/Matroska audio | Matroska/WebM containers | Container | Container |

## Architecture: individual codecs + a Loader/Manager

Each codec is now a **self-contained plugin** in its own folder under
`codecs/codecs/<name>/`, implementing a small common vtable
(`codecs/include/sleela_codec_plugin.h`): `describe`, `load`, `unload`, `probe`,
`decode`, `encode`. Every codec ships C **and** C++ (the vtable/glue in `.c`, the
byte-level/format detail in `.cpp`) plus its own `.md`.

SLeeLa never links or calls an individual codec directly. It calls the **Codec
Loader / Unloader / Management Controller** (`codecs/include/sleela_codec_manager.h`),
which owns every plugin's lifecycle:

```
   SLeeLa  ->  Codec Manager (loader/unloader/controller)  ->  individual codec plugin
                 load / unload / resolve / decode / encode
```

- `sleela_codec_manager_create()` registers every known codec (none loaded).
- `sleela_codec_manager_load(mgr, id)` / `..._unload(mgr, id)` manage lifecycle.
- `..._resolve_extension` / `..._resolve_probe` pick a codec from a filename or
  content bytes.
- `..._decode` / `..._encode` dispatch to the loaded plugin.
- A backend-less codec reports its state and returns a clean
  `SLEELA_CODEC_ERR_UNSUPPORTED` instead of faking success.

The SLeeLa-facing handle to the Loader is `lib/codecs/SLCodecLoader.sleela`
(runnable demo: `lib/codecs/codecs.sleela`).

### Native codecs (fully implemented in-package, no external library)

- **PCM/WAV** (`codecs/codecs/pcm_wav/`) — RIFF/WAVE 16-bit PCM read/write; the
  PCM boundary every other codec targets.
- **AIFF** (`codecs/codecs/aiff/`) — `FORM`/`AIFF` big-endian 16-bit PCM
  read/write, including the 80-bit extended sample-rate field.
- **G.711 μ-law / A-law** (`codecs/codecs/g711_mulaw/`, `.../g711_alaw/`) —
  telephony companding, one byte ↔ one 16-bit PCM sample.

### Backend codecs (contract + probe, pluggable implementation)

FLAC, ALAC, MP3, AAC, HE-AAC, Vorbis, Opus, Speex, WMA, AC-3, E-AC-3, AMR-NB,
AMR-WB are **Backend** handlers: they identify the format (a C++ signature
probe) and delegate encode/decode to a **runtime-registered backend adapter**.

```c
/* A host wires an approved library (libFLAC, libopus, ...) at startup: */
static const sleela_codec_backend flac_adapter = { "libFLAC", flac_dec, flac_enc };
sleela_codec_backend_register(SLEELA_CODEC_FLAC, &flac_adapter);
```

With a backend registered, the plugin's decode/encode dispatch to it; with none
registered, they return `SLEELA_CODEC_ERR_UNSUPPORTED` — never a false success.
(No third-party libraries are bundled in-tree.) MIDI is an **Event** format and
Matroska/WebM are **Container** handlers (probe + identification).

## C Handler Registry (metadata)

The static registry is in:

- `codecs/include/sleela_codecs.h`
- `codecs/src/sleela_codecs.c`

It provides:

- stable codec identifiers;
- human-readable names;
- MIME/type and extension metadata;
- handler state;
- encode/decode capability flags;
- lookup by identifier;
- lookup by common file extension;
- enumeration of the complete registry.

The registry is metadata and dispatch infrastructure. It does not silently substitute a system utility or third-party library for a codec implementation.

## Integration Rule

The Audio API in `audio/` should consume decoded PCM through an explicit boundary. Codec handlers should not bypass Audio validation or directly mutate mixer state.

The intended path is:

`compressed/container input → codec handler → PCM boundary → Audio API → output`

For encoding:

`Audio PCM → codec handler → compressed/container output`

## Completion Policy

A handler becomes **Native** only when the codec implementation, validation, tests, and build integration are present.

A **Backend** handler may be wired to a supported codec library later. Until then, callers can inspect its state and fail cleanly instead of receiving a false success.

See `CODECS.md` for the detailed registry and implementation order.