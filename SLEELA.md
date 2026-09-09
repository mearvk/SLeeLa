# SLEELA.md — The `.sleela` Filetype (Wrapper™), the Sigil QR Code, and the Steganographic Frame

This document is the reference for the **`.sleela` filetype** and for the two
identity artifacts that mark a Sleela distribution: a configurable **QR code**
and a deterministic **steganographic Sigil frame**. Everything here is produced
by the dependency-free tool in [`tools/sigil/`](tools/sigil/).

---

## Part 1 — The `.sleela` filetype: **Wrapper™**

**Wrapper™** is the name of the `.sleela` file type. A **Wrapper™** is a Sleela
source file — the program unit that carries the *metadocument addend*: it is
governed by, and must be consistent with, the **Sleela Language Metadocument**
(SL-META-0001, `src/Sleela.manifest`). Throughout this repository the terms
**`.sleela` file**, **Sleela source file**, and **Wrapper™** all name the same
file type.

### 1.1 Why "Wrapper"

A Wrapper™ *wraps* intent. The Sleela design mandate is **"express intent, not
mechanism"** (see `impl/DESIGN.md` and the metadocument's Primary Design
Principle): a Sleela program states *what* a computation means and the compiler
determines *how* it is realized. The `.sleela` file is therefore the wrapper
around that intent — the human-authored surface that the toolchain lowers into
executable form while remaining answerable to the metadocument.

### 1.2 Identity at a glance

| Property             | Value                                                             |
|----------------------|-------------------------------------------------------------------|
| Filetype name        | **Wrapper™**                                                      |
| Extension            | `.sleela`                                                         |
| Role                 | Program source (the "program"; a `.sst` sheet is only the control surface) |
| Governing document   | SL-META-0001 — the Sleela Language Metadocument (the metadoc addend) |
| Encoding             | UTF-8 (per the metadocument's lexical conventions)               |
| Surface language     | Java-like: classes, methods, `main()` entry point                |
| Execution            | Lexed → parsed → compiled to core bytecode → run on the C/C++ core via `slcore_exchange` |

### 1.3 What a Wrapper™ contains

A Wrapper™ is one or more classes; execution starts at `main()`. Supported
surface features today (see `impl/README.md` for the full table): classes,
typed locals (`int`, `double`, `boolean`, `String`, `void`), methods with
recursion, `if/else`/`while`/`for`, the full operator set, Java-style string
concatenation, `print(...)`, class fields (shared state), and a bounded
threading model (`spawn`/`join`/`lock`/`unlock`/`send`/`recv`).

```java
// hello.sleela — a minimal Wrapper™
class Hello {
    void main() {
        print("hello from a Wrapper\u2122");
    }
}
```

### 1.4 How a Wrapper™ flows through the toolchain

```
Wrapper™ (.sleela source)
        │
        ▼
  [ Sleela front end — C++ ]   lexer → parser → AST → compiler
        │   emits opcodes + drives the core via the exchange API
        ▼
  [ Sleela Core — C, stable ABI ]   stack VM · slcore_exchange()
```

- **Directly:** `sleela run file.sleela` compiles and runs one Wrapper™ on the C core.
- **Via Nordshrift:** a `.sst` control sheet (spec NS-SST-0001, `SST.model`)
  names a set of Wrapper™ files with a `source:` glob (e.g. `**/*.sleela`) and
  transpiles them to the **triplet** target — **Java**, **Sleela**, or **C**.

### 1.5 Relationship to neighboring filetypes

| Extension    | What it is                                                        | Program? |
|--------------|-------------------------------------------------------------------|----------|
| `.sleela`    | **Wrapper™** — Sleela program source (metadoc addend)             | **Yes**  |
| `.sst`       | Nordshrift control sheet (NS-SST-0001); names Wrapper™ inputs      | No       |
| `.sheet`     | `SHEET.sheet` object catalog (conducted methods / compat list)    | No       |
| `.manifest`  | SL-META-0001 metadocument text                                    | No       |
| `.xclass`    | SecureJDK 28 ingest input the CLI can turn into a Sleela program  | Indirect |

---

## Part 2 — The Sigil QR code (configurable)

The **Sigil QR code** encodes a single URL. By default it points at the GitHub
repository the source was developed on; it is fully configurable so any
user/developer can repoint it.

### 2.1 Default target

```
https://github.com/mearvk/Sleela
```

At the default URL the encoder selects **QR version 3**, error-correction level
**M**, mask **0** — a 29×29-module symbol. Rendered (quiet zone trimmed for
space):

```
  ███████   ██ █  █  █  ███████
  █     █ ████  ███  ██ █     █
  █ ███ █  █   ██ █ █ █ █ ███ █
  █ ███ █  █ █ ███  ██  █ ███ █
  █ ███ █ █ █ █ █    █  █ ███ █
  █     █  ██  █    ███ █     █
  ███████ █ █ █ █ █ █ █ ███████
           █  ███ ██ ██
  █ █ █ █  ███  ██ █  █   █  █
  ██ ███ █ ██ █ ██    █ █  █  █
     █ ██ ███ ██   ██  ██ █ ███
  █ ███    ███   █ ████  ██  █
  ███ ████ ███    ███████  █ ██
  ██  █  █ █ █ █ ██   ███  █  █
  █ █ █ ███   █ ████    █ ██ ██
  ███ █      ████ ███ ███  █ █
    ██ ██████   ███  ██ ██ █ ██
   ██ ██  █   █ ██ ██  ██  ██ █
  █ █   ██ ██  █   ██  █ ██  ██
   █ █   ███  █  █ █████████ █
  █  █  ██  █ █   ██ ██████
          ██ ███  █  ██   █ ███
  ███████    ██ █ █  ██ █ ██ ██
  █     █    ███████  █   ██ █
  █ ███ █ █ ██  ████  █████   █
  █ ███ █  ███ ██ ██  █  ██ ███
  █ ███ █ ████       █   ███  █
  █     █  █ █ █ ███████  █  █
  ███████ █████   ██ ███ ██  ██
```

The full-resolution PNG and an ASCII rendering are written to
`tools/sigil/out/sleela-qr.png` and `tools/sigil/out/sleela-qr.txt`.

### 2.2 Configuring the target URL

Resolution precedence (highest wins):

1. `--url` command-line argument
2. `SLEELA_QR_URL` environment variable
3. `url` field in a config file (`--config`, default `tools/sigil/sigil.config.json`)
4. Built-in default (`https://github.com/mearvk/Sleela`)

```sh
cd tools/sigil

python3 sigil.py                                   # default (or config file) URL
python3 sigil.py --url https://your.site/repo      # explicit override
SLEELA_QR_URL=https://your.site python3 sigil.py   # environment override
python3 sigil.py --print-url                        # show the resolved URL only
```

To change the committed default, edit `tools/sigil/sigil.config.json`:

```json
{ "url": "https://github.com/mearvk/Sleela" }
```

### 2.3 Encoder properties

- **Pure standard library.** No third-party dependencies; the QR encoder,
  Reed–Solomon ECC over GF(256), masking/penalty scoring, and the PNG writer are
  all implemented in-tree (`qr.py`, `png.py`).
- **Byte mode, UTF-8.** Auto-selects the smallest fitting version (1–10) at the
  chosen EC level (`L`/`M`/`Q`/`H`, default `M`).
- **Verified round-trip.** `qr_decode_check.py` re-decodes the generated matrix
  (un-mask → read format → de-interleave → RS syndrome check → byte-mode parse)
  and confirms it reproduces the input URL, with **zero Reed–Solomon syndromes**,
  across all EC levels.

---

## Part 3 — The steganographic Sigil frame (248 × 48, deterministic)

The **Sigil frame** is a fixed **248-bit-wide × 48-bit-tall** bitmap
(11,904 bits = 1,488 bytes). It looks like stochastic noise, yet it is a *pure
deterministic function of its input*: the same URL always produces the
byte-for-byte identical frame (**idempotent**), and the frame is **unique to the
input → output mapping**. The mapping is *preserved*: a recoverable digest of the
input is woven into the frame and can be verified later.

### 3.1 Excerpt

First rows of the default frame (`#` = 1, `.` = 0; truncated to 92 of 248 cols):

```
#.###.#....##..##....#.#.#...#.#..#..###.##..#.#.......####.#...#....#...#.####....#.#.#.##...##
.#..###.#......###.#..#..###.#.######..#..###...###.###.#.#.#.#.....###.##.###...#....#.#...###.
.####..##....#.##.##.#...##...#..##.#.#...#.#.#...###...#...###.#.###.##.##.##....#.##.#####...#
.#.#.##..###.##..#.#.#...#####...##.#..######.####.##..#..#.#.####..##.#..#.#....#.........#..#.
#..#....#..#.##..#.#.#..#######.....#.#....#..##.#..###.#.....#..#...##.##..#.....######.#.#..#.
###...#..####.#...###.#.##.##...#.##.#...####...#.########.##.###...###..#.##..###...###.#...#..
```

Full frame: `tools/sigil/out/sleela-sigil-frame.png` and
`tools/sigil/out/sleela-sigil-frame.txt`.

### 3.2 How it is built (semi-random, stochastic, yet reproducible)

1. **Seed.** `seed = SHA-256("SLEELA-SIGIL-v1" ‖ 0x00 ‖ input)`.
2. **Keyed noise (the "stochastic" field).** An **HMAC-DRBG** (HMAC-SHA-256, NIST
   SP 800-90A shape) is instantiated from the seed and emits the entire
   11,904-bit field. The apparent randomness is a *keyed, reproducible*
   keystream — not OS entropy — which is exactly what makes the frame idempotent.
3. **Steganographic header (the "preserve" step).** A small header is XOR-woven
   into carrier bits at DRBG-selected positions (a keyed permutation), so it is
   indistinguishable from the surrounding noise without the input, yet fully
   recoverable with it:

   | Field    | Bits | Meaning                                   |
   |----------|------|-------------------------------------------|
   | `magic`  | 32   | ASCII `SLSG`                              |
   | `version`| 8    | frame format version (currently `1`)      |
   | `width`  | 16   | frame width in bits (`248`)               |
   | `height` | 16   | frame height in bits (`48`)               |
   | `digest` | 128  | first 128 bits of `SHA-256(input)`        |

Because the noise field and the embedding positions both derive from the same
keyed DRBG, `verify(input, frame)` can regenerate the reference frame, confirm an
exact match (idempotency), and independently recover the embedded digest.

### 3.3 Idempotency & uniqueness — verification

```sh
cd tools/sigil
python3 sigil.py --verify
```

Sample output for the default URL:

```
url:        https://github.com/mearvk/Sleela (config:.../sigil.config.json)
idempotent: True
verified:   True
recovered:  magic=SLSG version=1 size=248x48 digest=e6678c0c2a9ab881f5414c5e64ecc7c5
```

- **Idempotent:** two independent generations of the same URL produce identical
  PNG and text bytes (confirmed by SHA-256 of the artifacts).
- **Unique per input:** a different URL yields a different digest and a different
  frame; the same URL always yields the same digest.
- **Verifiable mapping:** the embedded 128-bit digest equals `SHA-256(url)[:16]`,
  binding the frame to its input.

### 3.4 Configuring the frame

The frame is keyed by the same resolved URL as the QR code, so all the
configuration in §2.2 applies. Dimensions and rendering scale are adjustable:

```sh
python3 sigil.py --frame-width 248 --frame-height 48 --frame-scale 4
```

The **248 × 48** default is the canonical Sleela Sigil geometry; changing it
produces a different (still deterministic) frame and is recorded in the embedded
`width`/`height` header fields.

---

## Part 4 — Tooling reference

Everything lives in [`tools/sigil/`](tools/sigil/) and requires only Python 3
(standard library — no `pip install`, works offline):

| File                   | Purpose                                                        |
|------------------------|----------------------------------------------------------------|
| `sigil.py`             | CLI entry point (resolves URL, emits QR + frame)               |
| `qr.py`                | Pure-Python QR encoder (byte mode, RS ECC, masking, v1–10)     |
| `png.py`               | Minimal grayscale PNG writer (stdlib `zlib`)                   |
| `stego.py`             | Deterministic 248×48 frame: `generate` / `recover` / `verify`  |
| `qr_decode_check.py`   | Self-decoder proving the QR round-trips to the input URL       |
| `sigil.config.json`    | Configurable default URL                                       |
| `out/`                 | Generated artifacts (QR + frame, PNG + ASCII)                  |

```sh
cd tools/sigil
python3 sigil.py                 # generate QR + frame for the resolved URL
python3 sigil.py --verify        # prove idempotency + recover the embedded digest
python3 qr_decode_check.py       # prove the QR decodes back to its URL
```

---

*The `.sleela` filetype is **Wrapper™**. The Sigil QR code and the 248 × 48
steganographic frame are its distribution marks: one points outward (to the
repository), the other inward (a deterministic, verifiable fingerprint of that
same target).*
