# Slecompiler™ Product Tutorial

**Max Rupplin - MEARVK LLC - 2026**

## 1. What Slecompiler is

Slecompiler™ is SLeeLa's native-artifact inspection and static-analysis product. It accepts native artifacts as data and produces structured evidence: container type, architecture, sections, symbols, imports, exports, relocations, library metadata, kernel-module metadata, decoded instructions, control-flow graphs, function candidates, and SLIR.

The default analysis model is read-only and non-executing. An analyzed executable, shared library, driver, or kernel module is not loaded or run by Slecompiler.

The product is intended for software that you are authorized to inspect.

## 2. Product layers

The normal path is:

```
Artifact
  -> container metadata
  -> native interfaces
  -> Decoder
  -> ControlFlowGraph
  -> Analyzer
  -> SLIR
  -> validation / reporting / refactoring
```

For library families:

```
Artifact A + Artifact B + ...
  -> LibraryGraph
  -> provider / dependency evidence
  -> family-level analysis
```

## 3. Build the product

From the repository root:

### Linux

```sh
./build/slecompiler-linux.sh
```

### macOS

Install Apple's Xcode Command Line Tools if they are not already installed. Apple documents the package and `xcode-select --install` workflow. urlApple Command Line Tools documentationhttps://developer.apple.com/documentation/xcode/installing-the-command-line-tools

Then:

```sh
./build/slecompiler-macos.sh
```

The build defaults to the host architecture. A universal build can be requested when the local SDK/toolchain supports both architectures:

```sh
CMAKE_OSX_ARCHITECTURES="x86_64;arm64" ./build/slecompiler-macos.sh
```

### Windows 10+

Use a CMake-supported native compiler/toolchain. For Visual Studio 2022:

```powershell
powershell -ExecutionPolicy Bypass -File .\\build\\slecompiler-windows.ps1 -Generator "Visual Studio 17 2022" -Architecture x64
```

The same entry point accepts `Win32` or `ARM64` for Visual Studio generators.

CMake itself provides the cross-platform configure/build interface used by these entry points. urlCMake command-line documentationhttps://cmake.org/cmake/help/latest/manual/cmake.1.html

## 4. First API program: inspect an artifact

The smallest useful API program is:

```cpp
#include "sleela/decompiler/decompiler.hpp"

int main(int argc, char** argv) {
    auto artifact = sleela::decompiler::Artifact::open(argv[1]);

    std::cout << artifact.name() << "\\n";
    std::cout << artifact.sha256() << "\\n";
    std::cout << artifact.interfaces().symbols.size() << "\\n";
}
```

The complete repository exemplar is:

```
decompiler/examples/api_inspect.cpp
```

It demonstrates identity, format, size, and native-interface counts.

## 5. Decode instructions and construct a CFG

Use `Decoder` for bounded instruction recovery:

```cpp
auto artifact = sleela::decompiler::Artifact::open(path);
auto instructions = sleela::decompiler::Decoder{}.decode(artifact, 0, 128);
auto cfg = sleela::decompiler::Analyzer{}.build_cfg(instructions);
```

Each `Instruction` carries bytes, mnemonic, operands, provenance, control-flow flags, branch targets, and relocation evidence when available.

The corresponding exemplar is:

```
decompiler/examples/api_decode_cfg.cpp
```

### Important limitation

Decoder coverage is deliberately conservative. Unsupported byte sequences are represented as `db` rather than guessed instructions. Current native recovery is centered on the implemented x86/x86-64 subset; ARM and ARM64 artifacts can still be inspected at the container/interface level, but this does not imply complete ARM instruction decoding.

## 6. Inspect shared libraries and kernel modules

For ELF shared libraries, `Artifact::library_metadata()` exposes evidence such as:

- SONAME;
- DT_NEEDED dependencies;
- symbol-version evidence;
- build ID;
- PIE/position-independent indicators;
- TLS;
- RELRO;
- BIND_NOW;
- NX-stack evidence;
- initialization/finalization metadata;
- PLT/GOT addresses.

For Linux kernel modules, `Artifact::kernel_module_metadata()` exposes available `.modinfo` evidence such as module name, vermagic, license, author, description, aliases, and dependencies.

The exemplar is:

```
decompiler/examples/api_library_metadata.cpp
```

Kernel-module inspection remains static. Slecompiler does not insert a module into a running kernel.

## 7. Analyze a library family

When several artifacts belong to the same software family, use `LibraryGraph`:

```cpp
sleela::decompiler::LibraryGraph graph;
graph.add(sleela::decompiler::Artifact::open("app"));
graph.add(sleela::decompiler::Artifact::open("libexample.so"));
graph.add(sleela::decompiler::Artifact::open("libsupport.so"));
```

The graph retains artifact nodes and dependency/provider relationships. The current implementation is intentionally conservative; it should not be treated as a complete dynamic-loader simulation or ABI resolver.

The exemplar is:

```
decompiler/examples/api_library_graph.cpp
```

## 8. Work with SLIR

Slecompiler's SLeeLa Intermediate Representation provides a product-neutral analysis representation.

Core objects are:

- `Module`
- `Function`
- `SlirBlock`
- `Operation`
- `Value`
- `Provenance`

A small SLIR function can be assembled directly:

```cpp
sleela::decompiler::Operation constant;
constant.opcode = sleela::decompiler::OpCode::Const;
constant.immediate = 42;
constant.outputs.push_back({0, "u64"});
```

The SLIR layer is where later architecture lifters, type recovery, refactoring, and source-generation products can converge without changing the original artifact.

## 9. Validate SLIR with the bounded VM

The VM is a deterministic validation environment for the currently supported SLIR operations. It is not an emulator for an arbitrary operating system or a replacement for the original executable environment.

Example:

```cpp
sleela::decompiler::Vm vm({
    .memory_size = 4096,
    .max_steps = 32,
    .deterministic = true
});
vm.load(module);

if (!vm.run()) {
    std::cerr << vm.trap().message << "\\n";
}
```

The complete exemplar is:

```
decompiler/examples/api_slir_vm.cpp
```

Use the step and memory limits as explicit analysis boundaries.

## 10. Function recovery

The analyzer can turn CFG evidence into function candidates:

```cpp
auto candidates =
    sleela::decompiler::Analyzer{}.recover_function_candidates(
        artifact, cfg);
```

A function candidate is an evidence-backed hypothesis, not necessarily a recovered source-level function. Preserve its provenance and confidence when presenting results.

## 11. Artifact identity

Every opened artifact receives a SHA-256 digest. This identifies the exact byte sequence analyzed.

The digest is an identity/integrity value. It is not a digital signature and does not prove who produced or authorized an artifact.

For reproducible reports, store:

```
artifact path
artifact size
artifact SHA-256
Slecompiler version
analysis configuration
platform/toolchain information
```

## 12. CLI product

The current CLI provides four basic operations:

```sh
sleela-decompiler inspect <file>
sleela-decompiler disassemble <file> [length]
sleela-decompiler cfg <file> [length]
sleela-decompiler functions <file> [length]
```

The CLI is intentionally smaller than the C++ API. Applications that need structured metadata should use the library API rather than scraping terminal output.

## 13. API exemplar map

| Exemplar | Demonstrates |
|---|---|
| `api_inspect.cpp` | Artifact identity and native interfaces |
| `api_decode_cfg.cpp` | Decoder, instructions, CFG |
| `api_library_metadata.cpp` | DSO and kernel-module metadata |
| `api_library_graph.cpp` | Library-family relationships |
| `api_slir_vm.cpp` | SLIR construction and bounded VM validation |

All are under:

```
decompiler/examples/
```

They are enabled by default through the CMake option:

```
SLEE_LA_BUILD_API_EXAMPLES
```

Disable them for a library-only product build:

```sh
cmake -S decompiler -B build/slecompiler/custom -DSLEE_LA_BUILD_API_EXAMPLES=OFF
```

## 14. Recommended application architecture

For a larger product, keep the following separation:

```
input acquisition
    |
    v
Artifact
    |
    +--> metadata/reporting
    |
    +--> Decoder --> CFG --> Analyzer
    |                         |
    |                         v
    |                        SLIR
    |                         |
    |                 +-------+-------+
    |                 |               |
    v                 v               v
library graph      validation      refactoring
```

Do not make the UI, report generator, or source generator responsible for parsing raw ELF/PE/Mach-O structures directly. Keep those concerns behind the Slecompiler API so evidence and provenance remain consistent.

## 15. What the product does not yet promise

The current product foundation should not be described as a complete universal decompiler. In particular:

- instruction decoding is not complete for every architecture;
- function recovery remains conservative;
- library resolution is not a full dynamic-loader implementation;
- compressed kernel modules are not universally decompressed and interpreted;
- source-level type recovery is incomplete;
- generated C/C++ refactoring is a later layer;
- dynamic execution analysis is outside the default artifact-analysis pipeline;
- cryptographic identity is provided as SHA-256, not signing or attestation.

These distinctions belong in product documentation so API consumers can separate recovered evidence from inference.

## 16. Next development layer

The natural next API expansion is:

1. complete per-format native interfaces;
2. architecture-specific lifters;
3. richer relocation and PLT/GOT resolution;
4. function/call-graph recovery;
5. type and data-flow analysis;
6. structured JSON/report output;
7. library-family version/ABI resolution;
8. refactoring and source-emission APIs;
9. reproducible analysis manifests;
10. signed release and provenance manifests.

The API exemplars are intended to grow alongside those capabilities rather than becoming disconnected sample code.

## 17. Development workflow for API consumers

A practical integration sequence is:

1. Open the artifact and record its SHA-256 identity.
2. Inspect container metadata and native interfaces before decoding.
3. Select executable regions and decode only bounded ranges.
4. Build CFG/function candidates and retain provenance.
5. Promote useful evidence into SLIR.
6. Use the bounded VM only for supported SLIR validation.
7. Emit reports or refactoring products as separate artifacts.

Keep the original bytes immutable throughout this process. Treat inferred functions, provider matches, types, and reconstructed source as analytical products rather than replacements for the source artifact.

## 18. Documentation maintenance rule

When a public API changes, update the corresponding exemplar and these documentation surfaces in the same development pass:

- `decompiler/API.md`;
- `decompiler/docs/TUTORIAL.md`;
- `decompiler/README.md`;
- `api/decompiler/README.md`;
- `api/decompiler/API.html` when the browser-facing API summary changes;
- `decompiler/TERMINOLOGY.md` when a new product term is introduced.

When a build interface changes, update the relevant platform script and `build/README.md` / `build/PRODUCTS.md` together.

## 19. Current implementation boundary

The current foundation is intentionally incremental. ELF and GNU archive analysis is substantially developed; PE support and raw-artifact handling are part of the common model; native decoding remains conservative; and SLIR/VM support provides an architecture-neutral analysis boundary. The next layers should extend evidence and recovery without weakening the read-only, provenance-preserving model.
