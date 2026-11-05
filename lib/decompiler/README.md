<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-decompiler-logo-001.jpeg" alt="SLeeLa Decompiler" width="100%"></p>


# SLeeLa Decompiler

Max Rupplin - MEARVK LLC - 2026

The /lib/decompiler package is the SLeeLa-sourced and SLeeLa-driven decompiler design and implementation layer. It converts executable artifacts, object files, byte streams, and other supported binary inputs into evidence-preserving SLeeLa-oriented intermediate representations and VM-ready reconstruction artifacts.

## Authority

The authoritative model is:

input artifact -> acquisition -> format/OS evidence -> decoding -> CFG -> SLIR -> semantic analysis -> reconstruction -> SLeeLa/target output -> VM validation

The decompiler does not pretend that every binary can be perfectly reconstructed. Evidence, inference, reconstruction, and unresolved/fractional input are represented separately.

## Selection model

A decompilation request can declare source language family (C, C++, SLeeLa, or a loaded language module), source language version, expected input format and architecture, desired output, treatment of fractional/partial input, operating-system/ABI discernment policy, analysis depth, and additional language/format modules.

## Fractional input

Fractional input is never silently discarded. The request chooses PRESERVE, PARTIAL, REPORT_ONLY, or STRICT.

## OS discernment

OS identification is evidence-weighted, not assumed from a single signature. Format headers, ABI markers, imports/exports, relocation data, calling conventions, section names, runtime metadata, and executable evidence may contribute. Conflicting evidence remains visible in the report.

## VM readiness

The resulting SLeeLa model is validated by the same VM completeness/capability boundaries used by the compiler. Native C/C++ code provides implementation services; .sleela files define the source-level decompiler contract.

## Build

make -C lib/decompiler

or make decompiler

**SLeeLa — MEARVK LLC — 2026**

## Usage

This package has two cooperating halves, and it helps to keep them distinct:

- **The SLeeLa contract (`lib/decompiler/*.sleela`)** defines *what* a decompilation request is — its input, selection options, fractional-input policy, OS/ABI policy, pipeline stages, and output target. The `.sleela` classes are the source-of-truth contract; their method bodies are intentionally declarative.
- **The native subsystem (`decompiler/`, built as `sleela-decompiler`)** is the executing implementation — it actually reads a binary, decodes it, builds a CFG, lifts SLIR, and writes reconstructed source. When you want to *run* a decompilation and *save output*, you use this.

The sections below cover accepted inputs, the request/selection surface, worked examples, and how to direct output to a file or directory.

### Input types and mediums

Inputs are treated as untrusted evidence and are never executed. The catalogs in `LANGUAGE.FORMAT.REFERENCE.md` and `EXECUTABLE.LIBRARY.SOURCE.REFERENCE.md` are authoritative; the commonly supported mediums are:

| Medium | Examples | Evidence yielded |
|---|---|---|
| Native executable | ELF `ET_EXEC`/PIE, PE `.exe`, Mach-O | format, architecture, OS/ABI, entry point, sections, symbols |
| Shared/dynamic library | ELF `.so`/`.so.*`, PE `.dll`, Mach-O `.dylib` | SONAME/exports, imports/needed libraries, relocations |
| Static library / object | AR `.a`, ELF `.o` | member enumeration, per-member format, symbols |
| Kernel module | ELF `.ko` (incl. compressed, when safely identifiable) | module name, vermagic, license/author, dependencies |
| Managed / bytecode | JVM class/JAR, .NET assembly, BEAM, Lua bytecode | runtime metadata, managed symbol/debug attributes |
| Portable / IR | WebAssembly module, LLVM bitcode | producer metadata, name/debug/custom sections |
| Raw / firmware | raw images, blobs, dumps | byte-level evidence with SHA-256 provenance |

Native disassembly today covers x86, x86-64, AArch64 (ARM64), and 32-bit ARM (A32 + Thumb); other mediums are parsed for format/symbol/metadata evidence. A medium is *detected*, *parsed*, and/or *disassembled* — see `decompiler/README.md` for the per-format coverage matrix.

### Composing a request (the contract surface)

A request declares how the input should be interpreted and how output should be produced. The relevant contract classes and their key fields:

- **`SleelaDecompilerSourceLanguage`** — `name`, `family`, `version`, `abi`, `compiler`, `exactVersionRequired`. Selects the expected source-language family and version hypothesis.
- **`SleelaDecompilerModule` / `SleelaDecompilerModules`** — `name`, `languageFamily`, `format`, `architecture`, `operatingSystem`, `trusted`. Loads additional language/format modules; selection stays evidence-driven and trust-gated.
- **`SleelaDecompilerFractionalInput`** — `policy`, `allowGaps`, `preserveEvidence`, `emitPlaceholders`. Never silently discards partial input; choose one policy (below).
- **`SleelaDecompilerOSDetector`** — weighted evidence over headers, ABI, imports, relocations, runtime metadata, calling conventions, sections; `conflictingEvidence` stays visible.
- **`SleelaDecompilerOutput`** (the output target) — `format`, `language`, `version`, `path`, `profile`, `vmReady`, `preserveEvidence`, `includeReport`. Covered under "Directing output" below.

**Fractional-input policies** (`SleelaDecompilerFractionalInput`):

| Policy | Meaning |
|---|---|
| `PRESERVE` | keep unreadable/undecoded ranges as evidence |
| `PARTIAL` | reconstruct valid regions, insert explicit unresolved regions |
| `REPORT_ONLY` | stop before reconstruction, retain analysis evidence |
| `STRICT` | reject incomplete or contradictory input |

The native request mirror (`lib/decompiler/include/sleela_decompiler.h`) carries the same shape for the C/C++ services:

```c
sleela_decompiler_request req = {
    .source_language  = "c++",
    .source_version   = "",            /* "" = let evidence decide */
    .expected_input   = "elf",
    .desired_output   = "c++",
    .os_policy        = "weighted",
    .fractional_policy = SLEELA_FRACTIONAL_PRESERVE,
    .analysis_depth   = 2,
    .preserve_evidence = 1,
};
if (!sleela_decompiler_validate_request(&req)) { /* reject */ }
```

### Worked examples

Contract-level examples live under `tutorials/examples/`:

- `artifact-analysis.sleela` — acquire an artifact and walk it through module/output/OS-discernment selection.
- `fractional-reconstruction.sleela` — apply a fractional-input policy to incomplete input.

The lesson path in `tutorials/` (`01-artifact-to-slir`, `02-os-abi-and-format-discernment`, `03-fractional-input-and-source-reconstruction`) covers the body of the work end to end.

To actually run a decompilation, use the native CLI (built from `decompiler/`):

```sh
# Inspect: format, architecture, sections, symbols, imports/exports, metadata
sleela-decompiler inspect ./libexample.so

# Disassemble a window (ARM: add --thumb or --arm to force state)
sleela-decompiler disassemble ./program 256

# Control-flow edges / recovered function candidates
sleela-decompiler cfg       ./program
sleela-decompiler functions ./program

# Reconstruct source (lifted SLIR statements + evidence)
sleela-decompiler decompile ./program --output c++
```

### Directing output (save toward a directory; uploads)

Output is chosen on the output target. In the contract (`SleelaDecompilerOutput`):

- `format` / `language` / `version` — the reconstruction target (e.g. `c++`, `c`, `java`, `sleela`);
- `path` — the destination to write toward (a file, or a file within a directory you control);
- `includeReport` — also emit the evidence/analysis report;
- `vmReady` — validate the lifted SLIR against the VM before finalizing;
- `preserveEvidence` — retain observed-vs-inferred distinctions in the output.

With the native CLI, `--output` selects the language and `--file` saves toward a path; `--header` emits the companion header. To save into a directory, point `--file` at a path inside it (create the directory first):

```sh
mkdir -p out
# Source and companion header into ./out
sleela-decompiler decompile ./program --output c++ --file out/recovered.cpp
sleela-decompiler decompile ./program --output c++ --header --file out/recovered.hpp

# Other targets
sleela-decompiler decompile ./program --output c      --file out/recovered.c
sleela-decompiler decompile ./program --output java   --file out/Recovered.java
sleela-decompiler decompile ./program --output sleela  --file out/recovered.sleela
```

Default extensions when you let the emitter choose: `.c`, `.cpp`, `.java`, `.sleela` for source and `.h` / `.hpp` for C/C++ headers. Without `--file`, the reconstruction is written to standard output, so you can redirect or pipe it.

### Network and remote destinations (`--to`)

A governed destination adapter delivers an already-produced reconstruction to a local or network target. It is **output only** — it transmits bytes the pipeline has already generated and never reads or executes an input artifact. On the contract side this is `SleelaDecompilerDestination`, bound to the output via `SleelaDecompilerOutput.destination`; on the native side it is `decompiler/src/destination.cpp`, driven by `--to <uri>`.

Transports and URI forms:

| Transport | URI form |
|---|---|
| Local file | `file:/abs/path` or a bare `/local/path` |
| FTP (RFC 959 `STOR`, passive) | `ftp://[user:pass@]host[:port]/remote/path` |
| HTTP POST | `http://host[:port]/path` or `http+post://...` |
| HTTP PUT | `http+put://host[:port]/path` |
| SFTP (governed `sftp` helper) | `sftp://[user@]host[:port]/remote/path` |

Addressing is explicit and uniform across IP families: `host` may be a DNS name, an IPv4 literal, or an IPv6 literal written in brackets (`[2001:db8::1]`). Name resolution uses `getaddrinfo`, so A and AAAA records are handled transparently; set the IP preference (`ANY`/`IPV4`/`IPV6`) when a host offers both. A `port` of `0` selects the transport default (FTP 21, HTTP 80, SFTP/SSH 22). The IP actually connected to is reported back for audit.

```sh
# HTTP POST / PUT (DNS host, explicit path and content type)
sleela-decompiler decompile ./program --output c++ \
    --to http://uploads.example.org/recovered.cpp --content-type text/x-c++src
sleela-decompiler decompile ./program --output c \
    --to "http+put://198.51.100.20:8080/artifacts/recovered.c"

# FTP STOR with credentials and an explicit port
sleela-decompiler decompile ./program --output java \
    --to "ftp://deploy:secret@ftp.example.net:2121/incoming/Recovered.java"

# IPv6 literal (brackets), and SFTP via a governed client on PATH
sleela-decompiler decompile ./program --output c++ --to "http://[2001:db8::1]:8443/r.cpp"
sleela-decompiler decompile ./program --output c++ --to "sftp://build@host.example/out/r.cpp"
```

`--to` takes precedence over `--file`, which takes precedence over standard output.

**Security posture.** Network delivery is a governed capability and nothing is sent unless you supply `--to`. The adapter embeds no SSH stack: SFTP is delegated to a system `sftp` client (key/agent auth; host-key checking is left to your SSH configuration and is not weakened). Plain HTTP and FTP are unencrypted — send only output you are authorized to distribute, and prefer SFTP or an HTTPS-terminating endpoint for sensitive material. Credentials passed in a URI are visible to the local process environment; prefer key-based auth where possible. TLS termination for HTTPS is a follow-on adapter.

### Boundary note

Because `.sleela` is the contract and the native subsystem is the implementation, do not expect a contract method such as `emit()` to write a file by itself — it declares the behavior that the native `sleela-decompiler decompile ... --file ...` path performs. Everything here stays static and read-only: no input artifact is loaded or executed merely to identify or reconstruct it.

## Language and format reference

The package uses LANGUAGE.FORMAT.REFERENCE.md as the decompiler-side native reference for language names, producer programs, binary/object/executable formats, architecture and OS/ABI evidence, and safety considerations. The mapping is evidence, not proof, and does not authorize execution of an input artifact.

## Reverse executable/library to source references

The decompiler also maintains `EXECUTABLE.LIBRARY.SOURCE.REFERENCE.md`, which maps executables, shared/dynamic/static libraries, object files, managed artifacts, WebAssembly, LLVM bitcode, and raw images backward through format, architecture, OS/ABI, producer, language, version, and source references.

The mapping is evidence-based rather than a claim of exact original source. Debug information, symbols, provenance, compiler fingerprints, runtime metadata, and object-member analysis are used to strengthen the reference.

## Language edition 3.0

The SLeeLa decompiler contract is aligned with the normative SLeeLa syntax
**3.0** edition. Its SLeeLa source reconstruction target names syntax 3.0 as the
current edition while preserving source-version evidence and older syntax
compatibility. A reconstructed file must not be labelled as syntax 3.0 unless
its emitted constructs satisfy that edition; the decompiler preserves uncertainty
rather than inventing unsupported source semantics. See the root
[`SLEELA.syntax`](../../SLEELA.syntax) and [API guide](../../decompiler/API.md).
