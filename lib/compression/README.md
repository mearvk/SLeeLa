<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Compression

**Status: SLeeLa-side management surface + per-algorithm contracts.** This
directory is the language-facing compression layer: a Loader, Verifier,
Processor, and Controller that a GUI or application drives, plus one
metadata/capability class per algorithm, mirroring the native manager in
`compression/` (the C/C++ core at
`compression/include/sleela_compression_manager.h`). SLeeLa never calls an
individual algorithm directly — it asks the Controller/Loader, and the manager
calls the plugin.

The actual byte-level compress/decompress lives in the native plugins (Store,
RLE, Huffman, LZW) and, for backend algorithms, in a host-registered backend
adapter. These `.sleela` classes own lifecycle, classification, verification,
and honest capability reporting — they never fake a successful decompress.

This is the compression counterpart to `lib/codecs/` (audio): the same four
roles over the 15-algorithm compression registry.

## The four management roles

A GUI drives the **Controller**; the Controller cooperates with the other three.

| Role | File | Responsibility |
| --- | --- | --- |
| **Controller** | `SLCompressionController.sleela` | The single surface an app drives. Owns the per-algorithm loaded + backend tables; `load`/`unload`/`unloadAll`/`isLoaded`/`loadedCount`; `compress`/`decompress`/`canProcess`; delegates resolution/text to the Verifier. |
| **Loader** | `SLCompressionLoader.sleela` | The plugin load/unload contract and classification (`nameOf`, `isNative`) over the 15-algorithm registry. |
| **Verifier** | `SLCompressionVerifier.sleela` | `resolveExtension` (filename → id), `probeMagic` (content signature → id, mirroring each plugin's `probe()`), `headerless`, and `resultText` for the result codes. |
| **Processor** | `SLCompressionProcessor.sleela` | Pure compress/decompress dispatch over the facts `(loaded, native, backendReady)`; never a false success. |

Supporting files:

| File | Role |
| --- | --- |
| `SLPackage.sleela` | Package facade (`configure`/`name`/`available`). |
| `compression.sleela` | A compact runnable Loader demo (`class Compression` with `main`). |
| `compression-pipeline.sleela` | The runnable **Controller/Verifier/Processor** pipeline with `main` — the executable reference for the four roles. |
| `SLCompression<Name>.sleela` | One metadata/capability class per algorithm (15 files, see below). |

## The flow

```
file / bytes
     │
     ▼
Verifier.resolveExtension(".gz")  or  Verifier.probeMagic("1F8B08")  → algorithm id
     │
     ▼
Controller.load(id)                  → mark the algorithm usable (lifecycle)
     │
     ▼
Controller.canProcess(id)            → true iff loaded AND (native OR backend wired)
     │
     ▼
Controller.compress(id)/decompress(id) → Processor dispatches; honest result code
     │
     ▼
Controller.resultText(code)          → human-readable status for the GUI
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
ctrl.create();                             // registers all algorithms, none loaded

int id = ctrl.resolveExtension(".gz");     // → 6 (gzip)
ctrl.load(id);                             // make it usable
if (ctrl.canProcess(id)) {                 // false until a gzip backend is wired
    int r = ctrl.decompress(id);
    print(ctrl.resultText(r));             // "ok" or an honest error
}
ctrl.unloadAll();
```

Run the executable reference:

```sh
# from the repository root
./impl/build/sleela run lib/compression/compression-pipeline.sleela
./impl/build/sleela run lib/compression/compression.sleela
```

## Result codes

Mirror `sleela_compression_result` from the native plugin interface:

| Code | Meaning |
| --- | --- |
| `0` | ok |
| `-1` | unsupported (backend not present) |
| `-2` | invalid arguments (e.g. bad id) |
| `-3` | input is not this algorithm's format |
| `-4` | I/O or allocation failure |
| `-5` | algorithm not loaded |
| `-6` | output buffer too small |

A **native** algorithm (Store, RLE, Huffman, LZW) compresses/decompresses
in-package and returns `0` once loaded. A **backend** algorithm returns `-1`
until an approved backend adapter is registered via `registerBackend(id)` —
never a false `0`.

## Algorithm registry

Ids and handler states mirror `compression/include/sleela_compression.h`
(0 native · 1 backend · 2 recognized · 3 container).

| id | Class | Algorithm | State |
| --- | --- | --- | --- |
| 0 | `SLCompressionStore` | Store (raw) | native |
| 1 | `SLCompressionRle` | Run-Length Encoding | native |
| 2 | `SLCompressionHuffman` | Canonical Huffman | native |
| 3 | `SLCompressionLzw` | Lempel-Ziv-Welch | native |
| 4 | `SLCompressionDeflate` | DEFLATE (RFC 1951) | backend |
| 5 | `SLCompressionZlib` | zlib (RFC 1950) | backend |
| 6 | `SLCompressionGzip` | gzip (RFC 1952) | backend |
| 7 | `SLCompressionBzip2` | bzip2 | backend |
| 8 | `SLCompressionLzma` | LZMA | backend |
| 9 | `SLCompressionXz` | xz (LZMA2) | backend |
| 10 | `SLCompressionZstd` | Zstandard | backend |
| 11 | `SLCompressionLz4` | LZ4 | backend |
| 12 | `SLCompressionBrotli` | Brotli | backend |
| 13 | `SLCompressionSnappy` | Snappy | backend |
| 14 | `SLCompressionZip` | ZIP archive | container |

## Per-algorithm classes

Each `SLCompression<Name>.sleela` carries the registry **identity + capability**
contract (`id`, `name`, `mime`, `extensions`, `state`/`stateName`,
`canCompress`/`canDecompress`, `isNative`/`needsBackend`) and an **11-attribute
descriptive record** with getter/setter pairs so a GUI, loader, or catalog can
read and override each algorithm's metadata:

`version`, `id`, `author`, `date`, `copyright`, `trademark`, `corporation`,
`origin`, `birth date`, `science rating` (1..10 maturity), `national schedule`
(the standards-body / specification designation).

```java
SLCompressionZstd c = new SLCompressionZstd();
print(c.getCorporation());        // Facebook / Meta Platforms
print(c.getOrigin());             // United States
print(c.getNationalSchedule());   // RFC 8878 (Zstandard)
c.setScienceRating(10);           // override where appropriate
```

Metadata values are faithful to each algorithm's real origin where publicly
known.

## Relationship to the native tree and to codecs

- Native compression manager/plugins: `compression/`
  (`sleela_compression_manager.h`, `sleela_compressor_plugin.h`,
  `sleela_compression.h`).
- The parallel **audio codec** surface — the same four roles over the 20-codec
  audio registry — lives in `lib/codecs/` (`SLCodecLoader`, `SLCodecVerifier`,
  `SLCodecProcessor`, `SLCodecController`, and `codec-pipeline.sleela`),
  mirroring `codecs/include/sleela_codec_manager.h`.

## Implementation boundary

These classes manage, classify, verify, and report. Real (de)compression
requires the native plugin (native algorithms) or a registered backend adapter
(backend algorithms). A backend algorithm with no adapter reports `-1`
(unsupported) rather than a false success — the deliberate "never fake a
decompress" contract carried from the native design.
