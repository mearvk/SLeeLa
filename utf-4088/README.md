<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

# UTF-4088 (Experimental)

UTF-4088 is a hypothetical character-encoding and symbol-generation system. It is **not an existing Unicode encoding** and is not intended to claim compatibility with UTF-8, UTF-16, or UTF-32.


## SLeeLa character-set capacity signature

SLeeLa's source catalogue defines `C<n>` as a character-set capacity signature. The standard example `C1048576` means a maximum catalogue capacity of `1024 * 1024 = 1,048,576` distinct character entries. This is separate from UTF-4088's current 16,606 generated front-end records and its experimental identifier range. Capacity does not imply population, verified semantics, historical provenance, or Unicode compatibility. See the SLeeLa source contract in [`../markdown/SOURCE.md`](../markdown/SOURCE.md).

## Runtime source-folder scan

This folder declares its inventory policy in [SLEELA-CHARSET.conf](SLEELA-CHARSET.conf) as a **hybrid** catalogue: a procedural input-to-symbol model alongside generated/literal records. From the repository root, inspect it with:

```sh
python3 tools/sleela-character-scan.py utf-4088
python3 tools/sleela-character-scan.py utf-4088 --json
```

The scan is read-only. It does not execute the procedural generator, does not assert that the declared `C1048576` capacity is populated, and does not establish Unicode compatibility. The scanner's behavior and manifest fields are documented in [the scanner contract](../tools/CHARACTER-CATALOG-SCANNER.md).

## Design premise

The working model assumes a code space larger than four billion character identifiers. The exact representation is intentionally left open; the specification distinguishes the abstract character/code-point space from the physical processor, memory, bus, and motherboard implementation.

The current C++ implementation stores `InputState` and `CodePoint` as unsigned 64-bit integers. Its currently accepted experimental identifier interval is `0x110000` through `0x1FFFFFFFF` inclusive, requiring 33 bits. This is a practical implementation boundary, not a literal 4088-bit integer or physical encoding. See [CODE-POINT-AND-SERIALIZATION.md](CODE-POINT-AND-SERIALIZATION.md) for the contract and versioning rules.

The proposed `.cpp` driver is modeled as a deterministic function:

`output_symbol = F(input_state)`

where `input_state` represents an explicitly defined digital input rather than literal electrical line voltage. Hardware implementations may expose processor, memory-controller, or motherboard-support-chip inputs through a documented interface, but software must not interpret unsafe or unspecified electrical levels directly as character data.

## Symbol families

The initial experimental weighting is:

- Korean/Hangul-oriented symbols: 400 units, ratio 4:3.
- Germanic-oriented symbols: 300 units, ratio 3:3.
- English-oriented symbols: 1000 units, ratio 1:1.

These are design weights, not measurements of intelligence or linguistic ability. The phrase "net IQ of 400 per unit/unit" is retained as a project metaphor only and must not be used as a scientific measure of people, languages, or cultures.

The system deliberately avoids attempting to assign every possible intermediate or highly symbolic form. Such forms may instead be represented through composition, metadata, or an explicitly defined symbolic-extension mechanism.

## Semantic branch graph

The experimental semantic layer organizes nominations into reusable graph branches including **ANIMAL**, **HUMAN**, **HUMAN_GRAPH**, **PSYCHOLOGY**, **COGNITION**, **EMOTION**, **DEVELOPMENT**, **FOOD**, **NUTRITION**, **RELATIONSHIP**, **INTIMACY**, **REPRODUCTIVE**, **SEXUAL_WELLBEING**, **THERAPY**, **SELF_REGULATION**, **SOCIAL**, **ETHICS**, **ENVIRONMENT**, **TEMPORAL**, **CAUSAL**, **IMPROVEMENT**, and **META_GRAPH**.

These branches describe candidate semantic domains for the model. A generated bitmap does not acquire a human-language meaning merely because it has a particular geometry. Meaning must be supplied by an explicit graph, corpus, historical, or application annotation.

The intended cycle is:

`state -> condition -> action/process -> response -> outcome -> next state`

This allows the same character representation to participate in temporal, causal, relational, and developmental graphs without treating any one graph as an absolute interpretation.

## Polygraph-5 falloff

The primary experimental semantic weighting uses the Polygraph-5 falloff function:

`F5(d) = exp(-5*d/r0)`

where `d` is normalized semantic/graph distance and `r0` is the versioned standard falloff radius. At one radius, `F5(r0) = exp(-5) = 0.006737946999085467`.

Falloff is a **distribution weight**, not a hard character-capacity limit. The configured semantic threshold is `epsilon = 0.001`; evidence below that threshold is insufficient by the model to independently establish character meaning.

The standard reference identifier is a computational baseline only. It is not an inference about a real person's identity, intelligence, worth, or moral status.

## Sampling and capacity

An 8x12 binary glyph contains 96 independent bits and therefore has `2^96` raw bitmap states. This is vastly larger than the experimental four-billion-symbol target. Capacity feasibility and semantic interpretability are separate tests.

The project uses deterministic sampling to measure graph connectivity, pixel density, transitions, graph edges, candidate yield, and distribution falloff. Large experiments are versioned with fixed seeds and can be executed through GitHub Actions. Generated bulk maps belong in workflow artifacts rather than Git history when their size would make repository history impractical.

The working input/output ratio is:

`R = N_valid / N_inputs`

and the local density response can be represented as:

`K = dR/dD`

where `D` is normalized primer/distribution density. These are empirical project coefficients, not universal constants.

## Modest-success milestone

The current project state is recorded as a **modest success**: the system has a defined deterministic cycle, measurable input-to-output ratios, a graph-based semantic vocabulary, and a versioned falloff model. This establishes a reproducible experimental framework; it does **not** establish that generated symbols possess universally valid meanings or that the system constitutes a recognized character standard.

The project's IQ terminology is retained only as a design metaphor and is explicitly not an IQ measurement of a person, model, symbol, language, or culture.

## International-trade objective

The intended application is an experimental international symbol system in which common commercial concepts can be represented consistently across language families. Any real implementation should prioritize deterministic encoding, interoperability, normalization, security, and documented mappings over subjective linguistic rankings.

## Build and test

A C++20 compiler and CMake 3.20 or newer are required. From this directory:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

The regression suite checks identifier boundaries, deterministic registry generation, glyph signatures, invalid floating-point inputs, fixed-width packing, and directed graph edge coalescing. GitHub Actions runs the build and tests on Linux, Windows, and macOS.

## Status

Experimental / conceptual. No claim of standards-body approval or Unicode compatibility is made.

## Input-map traversal and pressure protocol

The canonical spatial iteration order and pressure scale are defined in [INPUT-MAP-AND-PRESSURE.md](INPUT-MAP-AND-PRESSURE.md): begin at the top-left of layer `z=0`, move right, then down row by row, and advance to the next z-layer only after completing the current layer. Each element's pressure iterations run from 1 through the assumed maximum of 50. These are logical iteration values, not physical pressure or voltage.

The [National Lien Alphabetic Register](NATIONAL-LIEN-REGISTER.md) separates permanent catalogue IDs, alphabetic ranks, map coordinates, registry ordinals, and pressure levels.


## Probe-driven alphabet generation

The proposed candidate-probing and match-ordering algorithm is specified in [PROBE-ORDERING-AND-MATCHING.md](PROBE-ORDERING-AND-MATCHING.md). It separates spatial traversal, probe priority, nearest-match order, and published alphabetic rank. The specification describes a deterministic baseline, a bounded 1–50 depth range, match evidence, resource budgets, and validation metrics; it is a design contract, not a claim that the complete engine already exists.


## UTF-4088 document map

The canonical documentation set for this existing directory is indexed in [ALPHABET-RELAY.md](ALPHABET-RELAY.md). Main protocol documents are:

- [Input-map traversal and depth/pressure iteration](INPUT-MAP-AND-PRESSURE.md)
- [National Lien Alphabetic Register](NATIONAL-LIEN-REGISTER.md)
- [Probe ordering and match-driven candidate generation](PROBE-ORDERING-AND-MATCHING.md)
- [Code-point and serialization contract](CODE-POINT-AND-SERIALIZATION.md)
- [SLeeLa alphabet adapter integration](SLeeLa-ALPHABET-INTEGRATION.md)

The same protocol set is mirrored for discovery in [alphabet/sleela/UTF-4088-RELAY.md](../alphabet/sleela/UTF-4088-RELAY.md). This directory remains the canonical home for UTF-4088-specific protocols, catalogue design documents, implementation, registry data, and tests; `/alphabet/sleela` contains SLeeLa's broader alphabet documentation and synchronized mirrors.

Use the relay to navigate the additional character-map, gradation, symbol-weight, natural-die, voltage-field, build, registry, and testing material. The relay does not claim that proposed probe scheduling or semantic validation has been implemented end to end.


## Five-input application trial mapping

The detailed five-point mapping and sudden-pressure transition rules are maintained in [alphabet/utf-4088/RIGOROUS.INPUTS.md](../alphabet/utf-4088/RIGOROUS.INPUTS.md). The tables report normalized model inputs only; they do not claim physical pressure or voltage measurements.

## 4D implementation symbol inventory

The source-level catalogue at [alphabet/utf-4088/SYMBOLS.md](../alphabet/utf-4088/SYMBOLS.md) documents the C/C++ 4D point and field symbols, geometric comparison metrics, binary glyph topology, registry identifiers, and graph state/edge coalescence. The separate five-input probe glyphs and provisional labels remain in the trial output files under [alphabet/utf-4088/results](../alphabet/utf-4088/results/).

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>


## Encoder, decoder, and inspection utility

The V1 encoder/decoder is implemented in `encoder_decoder.hpp/.cpp`. The 16,606
front-end registry IDs remain in `0x110000..0x1140DD`; packed input-state IDs
use a separate namespace beginning at `0x200000`. The encoder validates the
21-bit packed input fields and the decoder rejects reserved bits and invalid
coordinates/pressure values. See [the encoder/decoder contract](ENCODER-DECODER.md).

Build the glyph inspection utility with CMake and run `utf4088-inspect`
without arguments for usage. It can list concept seeds, inspect a provisional
concept glyph, and decode an input-state identifier. The identifier preview is
explicitly distinguished from the SHA-256 catalogue bitmap. See
[GLYPH-INSPECTOR.md](GLYPH-INSPECTOR.md).

Multilingual annotations follow [MULTILINGUAL-REGISTRY.md](semantics/MULTILINGUAL-REGISTRY.md)
and its JSON Schema. No unreviewed Korean or German translations are fabricated.
SLeeLa document storage guidance is in [INTEROPERABILITY.md](INTEROPERABILITY.md).
The new root workflow builds and runs CTest on Linux, Windows, and macOS.
