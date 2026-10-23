<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Image Codecs

**Status: SLeeLa-side management surface + per-codec contracts.** This directory
is the language-facing image codec layer: a Loader, Verifier, Processor, and
Controller that a GUI or application drives, plus one metadata/capability class
per codec. It is the image sibling of the audio surface in `lib/codecs/` and the
video surface in `lib/video/`. SLeeLa never calls an individual codec
directly -- it asks the Controller/Loader, and the manager calls the plugin.

The actual byte-level encode/decode lives in the native plugins (native raster
codecs) and, for backend codecs, in a host-registered backend adapter. These
`.sleela` classes own lifecycle, classification, verification, and honest
capability reporting -- they never fake a successful decode. File I/O is assumed
to be provided correctly by the host.

## The four management roles

The management surface mirrors the audio/video managers' split. A GUI drives the
**Controller**; the Controller cooperates with the other three.

| Role | File | Responsibility |
| --- | --- | --- |
| **Controller** | `SLImageCodecController.sleela` | The single surface an app drives. Owns the per-codec loaded + backend tables; `load`/`unload`/`unloadAll`/`isLoaded`/`loadedCount`; `decode`/`encode`/`canProcess`; delegates resolution/text to the Verifier. |
| **Loader** | `SLImageCodecLoader.sleela` | The plugin load/unload contract and classification (`nameOf`, `isNative`) over the 20-codec registry. |
| **Verifier** | `SLImageCodecVerifier.sleela` | `resolveExtension` (filename → id), `probeMagic` (content signature → id), `headerless`, and `resultText` for the result codes. |
| **Processor** | `SLImageCodecProcessor.sleela` | Pure decode/encode dispatch over the facts `(loaded, native, backendReady)`; never a false success. |

Supporting files:

| File | Role |
| --- | --- |
| `SLPackage.sleela` | Package facade (`configure`/`name`/`available`). |
| `image-codecs.sleela` | A compact runnable Loader demo (`class ImageCodecs` with `main`). |
| `image-codec-pipeline.sleela` | The runnable **Controller/Verifier/Processor** pipeline with `main` -- the executable reference for the four roles. |
| `SLImageCodec<Name>.sleela` | One metadata/capability class per codec (20 files, see below). |

## The flow

```
file / bytes
     │
     ▼
Verifier.resolveExtension(".png")  or  Verifier.probeMagic("89504E47")  → codec id
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
Controller holds its Verifier as a field and a GUI drives **one object**:

```java
Controller ctrl = new Controller();
ctrl.create();                             // registers all codecs, none loaded

int id = ctrl.resolveExtension(".png");    // → 3 (PNG)
ctrl.load(id);                             // make it usable
if (ctrl.canProcess(id)) {                 // false until a PNG backend is wired
    int r = ctrl.decode(id);
    print(ctrl.resultText(r));             // "ok" or an honest error
}
ctrl.unloadAll();
```

Run the executable reference:

```sh
# from the repository root
./impl/build/sleela run lib/image/image-codec-pipeline.sleela
```

## Result codes

| Code | Meaning |
| --- | --- |
| `0` | ok |
| `-1` | unsupported (backend not present) |
| `-2` | invalid arguments (e.g. bad id) |
| `-3` | input is not this codec's format |
| `-4` | I/O or allocation failure |
| `-5` | codec not loaded |

A **native** codec (PPM/PNM, BMP, TGA, Radiance HDR, PCX) decodes/encodes
in-package and returns `0` once loaded. A **backend**, **container**, or
**recognized** codec returns `-1` until an approved backend adapter is
registered via `registerBackend(id)` -- never a false `0`.

## Codec registry

Handler states: `0` native · `1` backend · `2` recognized · `3` container ·
`4` event.

| id | Class | Format | State |
| --- | --- | --- | --- |
| 0 | `SLImageCodecPpm` | PPM/PNM (Netpbm) | native |
| 1 | `SLImageCodecBmp` | BMP | native |
| 2 | `SLImageCodecTga` | Truevision TGA | native |
| 3 | `SLImageCodecPng` | PNG | backend |
| 4 | `SLImageCodecJpeg` | JPEG | backend |
| 5 | `SLImageCodecGif` | GIF | backend |
| 6 | `SLImageCodecTiff` | TIFF | backend |
| 7 | `SLImageCodecWebp` | WebP | backend |
| 8 | `SLImageCodecHeif` | HEIF/HEIC | backend |
| 9 | `SLImageCodecAvif` | AVIF | backend |
| 10 | `SLImageCodecJpegXl` | JPEG XL | backend |
| 11 | `SLImageCodecJpeg2000` | JPEG 2000 | backend |
| 12 | `SLImageCodecIco` | ICO/CUR | container |
| 13 | `SLImageCodecSvg` | SVG | recognized |
| 14 | `SLImageCodecDds` | DirectDraw Surface | backend |
| 15 | `SLImageCodecOpenExr` | OpenEXR | backend |
| 16 | `SLImageCodecRadianceHdr` | Radiance HDR | native |
| 17 | `SLImageCodecPcx` | PCX | native |
| 18 | `SLImageCodecXpm` | X PixMap (XPM) | recognized |
| 19 | `SLImageCodecIcns` | Apple Icon Image | container |

## Per-codec classes

Each `SLImageCodec<Name>.sleela` carries the registry **identity + capability**
contract (`id`, `name`, `mime`, `extensions`, `state`/`stateName`,
`canDecode`/`canEncode`, `isNative`/`needsBackend`) and an **11-attribute
descriptive record** with getter/setter pairs so a GUI, loader, or catalog can
read and override each codec's metadata:

`version`, `id`, `author`, `date`, `copyright`, `trademark`, `corporation`,
`origin`, `birth date`, `science rating` (1..10 maturity), `national schedule`
(the standards-body designation).

```java
SLImageCodecPng c = new SLImageCodecPng();
print(c.getCorporation());        // W3C / PNG Development Group
print(c.getOrigin());             // United States
print(c.getNationalSchedule());   // ISO/IEC 15948 (W3C PNG)
c.setScienceRating(10);           // override where appropriate
```

Metadata values are faithful to each format's real origin where publicly known.

## Why these five are native

Native image codecs are the formats whose decode/encode is simple enough to do
in pure SLeeLa in-package, with no external library: **PPM/PNM** (plain
uncompressed raster), **BMP** and **TGA** (uncompressed or simple RLE),
**Radiance HDR** (RLE-encoded RGBE scanlines), and **PCX** (byte-oriented RLE).
Everything compressed (PNG/DEFLATE, JPEG/DCT, GIF/LZW, WebP/VP8, HEIF/HEVC,
AVIF/AV1, JPEG XL, JPEG 2000, DDS block compression, OpenEXR) or structural
(ICO/ICNS containers, SVG/XPM text formats) advertises its honest state and
defers to a registered backend.

## Relationship to the audio and video surfaces

- Audio codec surface: `lib/codecs/` (`SLCodecController` and siblings over the
  20-codec audio registry).
- Video codec surface: `lib/video/` (`VideoCodecController` and siblings over
  the 12-codec video registry).
- This image surface is the third sibling, same four roles over a 20-codec
  image registry.

## Implementation boundary

These classes manage, classify, verify, and report. Real raster decode/encode
runs in the native plugin (native codecs) or a registered backend adapter
(backend/container/recognized codecs). A codec with no adapter reports `-1`
(unsupported) rather than a false success -- the deliberate "never fake a decode"
contract carried from the audio and video designs.
