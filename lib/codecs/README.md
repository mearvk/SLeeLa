<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Audio Codecs

**Status: SLeeLa-side management surface + per-codec contracts.** This directory
is the language-facing codec layer: a Loader, Verifier, Processor, and
Controller that a GUI or application drives, plus one metadata/capability class
per codec, mirroring the native manager in `codecs/` (the C/C++ core at
`codecs/include/sleela_codec_manager.h`). SLeeLa never calls an individual codec
directly — it asks the Controller/Loader, and the manager calls the plugin.

The actual byte-level encode/decode lives in the native plugins (and, for
backend codecs, in a host-registered backend adapter). These `.sleela` classes
own lifecycle, classification, verification, and honest capability reporting —
they never fake a successful decode.

## The four management roles

The management surface mirrors the native manager's split. A GUI drives the
**Controller**; the Controller cooperates with the other three.

| Role | File | Responsibility |
| --- | --- | --- |
| **Controller** | `SLCodecController.sleela` | The single surface an app drives. Owns the per-codec loaded + backend tables; `load`/`unload`/`unloadAll`/`isLoaded`/`loadedCount`; `decode`/`encode`/`canProcess`; delegates resolution/text to the Verifier. |
| **Loader** | `SLCodecLoader.sleela` | The plugin load/unload contract and classification (`nameOf`, `isNative`) over the 20-codec registry. |
| **Verifier** | `SLCodecVerifier.sleela` | `resolveExtension` (filename → id), `probeMagic` (content signature → id, mirroring each plugin's `probe()`), `headerless`, and `resultText` for the result codes. |
| **Processor** | `SLCodecProcessor.sleela` | Pure decode/encode dispatch over the facts `(loaded, native, backendReady)`; never a false success. |

Supporting files:

| File | Role |
| --- | --- |
| `SLPackage.sleela` | Package facade (`configure`/`name`/`available`). |
| `codecs.sleela` | A compact runnable Loader demo (`class Codecs` with `main`). |
| `codec-pipeline.sleela` | The runnable **Controller/Verifier/Processor** pipeline with `main` — the executable reference for the four roles. |
| `SLCodec<Name>.sleela` | One metadata/capability class per codec (20 files, see below). |

## The flow

```
file / bytes
     │
     ▼
Verifier.resolveExtension(".flac")  or  Verifier.probeMagic("fLaC")   → codec id
     │
     ▼
Controller.load(id)                 → mark the codec usable (lifecycle)
     │
     ▼
Controller.canProcess(id)           → true iff loaded AND (native OR backend wired)
     │
     ▼
Controller.decode(id) / encode(id)  → Processor dispatches; honest result code
     │
     ▼
Controller.resultText(code)         → human-readable status for the GUI
     │
     ▼
Controller.unload(id) / unloadAll()
```

## Using it from a GUI (syntax 1.11)

As of `#sleela 1.11`, method calls resolve on class-typed fields, so the
Controller holds its Verifier/Processor as fields and a GUI drives **one
object**:

```java
Controller ctrl = new Controller();
ctrl.create();                             // registers all codecs, none loaded

int id = ctrl.resolveExtension(".flac");   // → 2 (FLAC)
ctrl.load(id);                             // make it usable
if (ctrl.canProcess(id)) {                 // false until a FLAC backend is wired
    int r = ctrl.decode(id);
    print(ctrl.resultText(r));             // "ok" or an honest error
}
ctrl.unloadAll();
```

Run the executable reference:

```sh
# from the repository root
./impl/build/sleela run lib/codecs/codec-pipeline.sleela
```

## Result codes

Mirror `sleela_codec_result` from the native plugin interface:

| Code | Meaning |
| --- | --- |
| `0` | ok |
| `-1` | unsupported (backend not present) |
| `-2` | invalid arguments (e.g. bad id) |
| `-3` | input is not this codec's format |
| `-4` | I/O or allocation failure |
| `-5` | codec not loaded |

A **native** codec (PCM/WAV, AIFF, G.711 μ-law/A-law) decodes/encodes in-package
and returns `0` once loaded. A **backend** codec returns `-1` until an approved
backend adapter is registered via `registerBackend(id)` — never a false `0`.

## Codec registry

Ids and handler states mirror `codecs/include/sleela_codecs.h`
(0 native · 1 backend · 2 recognized · 3 container · 4 event).

| id | Class | Codec | State |
| --- | --- | --- | --- |
| 0 | `SLCodecPcmWav` | PCM/WAV | native |
| 1 | `SLCodecAiff` | AIFF | native |
| 2 | `SLCodecFlac` | FLAC | backend |
| 3 | `SLCodecAlac` | ALAC | backend |
| 4 | `SLCodecMp3` | MPEG Layer III | backend |
| 5 | `SLCodecAac` | AAC | backend |
| 6 | `SLCodecHeAac` | HE-AAC | backend |
| 7 | `SLCodecVorbis` | Ogg Vorbis | backend |
| 8 | `SLCodecOpus` | Opus | backend |
| 9 | `SLCodecSpeex` | Speex | backend |
| 10 | `SLCodecWma` | Windows Media Audio | backend |
| 11 | `SLCodecAc3` | Dolby Digital AC-3 | backend |
| 12 | `SLCodecEac3` | Dolby Digital Plus | backend |
| 13 | `SLCodecAmrNb` | AMR-NB | backend |
| 14 | `SLCodecAmrWb` | AMR-WB | backend |
| 15 | `SLCodecG711Mulaw` | G.711 μ-law | native |
| 16 | `SLCodecG711Alaw` | G.711 A-law | native |
| 17 | `SLCodecMidi` | MIDI | event |
| 18 | `SLCodecMatroskaAudio` | Matroska Audio | container |
| 19 | `SLCodecWebmAudio` | WebM Audio | container |

## Per-codec classes

Each `SLCodec<Name>.sleela` carries the registry **identity + capability**
contract (`id`, `name`, `mime`, `extensions`, `state`/`stateName`,
`canDecode`/`canEncode`, `isNative`/`needsBackend`) and an **11-attribute
descriptive record** with getter/setter pairs so a GUI, loader, or catalog can
read and override each codec's metadata:

`version`, `id`, `author`, `date`, `copyright`, `trademark`, `corporation`,
`origin`, `birth date`, `science rating` (1..10 maturity), `national schedule`
(the standards-body designation).

```java
SLCodecMp3 c = new SLCodecMp3();
print(c.getCorporation());        // Fraunhofer-Gesellschaft
print(c.getOrigin());             // Germany
print(c.getNationalSchedule());   // ISO/IEC 11172-3 (MPEG-1 Layer III)
c.setScienceRating(9);            // override where appropriate
```

Metadata values are faithful to each codec's real origin where publicly known.

## Relationship to the native tree and to video

- Native audio manager/plugins: `codecs/` (`sleela_codec_manager.h`,
  `sleela_codec_plugin.h`, `sleela_codecs.h`).
- The parallel **video** codec surface — the same four roles over the 12-codec
  video registry — lives in `lib/video/` (`VideoCodecLoader`,
  `VideoCodecVerifier`, `VideoCodecProcessor`, `VideoCodecController`, and
  `video-codec-pipeline.sleela`), mirroring
  `video/codecs/include/sleela_video_codecs.h`.

## Implementation boundary

These classes manage, classify, verify, and report. Real transcoding requires
the native plugin (native codecs) or a registered backend adapter (backend
codecs). A backend codec with no adapter reports `-1` (unsupported) rather than
a false success — the deliberate "never fake a decode" contract carried from the
native design.