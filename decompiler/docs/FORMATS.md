# Slecompiler™ Formats and Native Artifact Classes

**Max Rupplin - MEARVK LLC - 2026**

Slecompiler™ treats container format and software role as separate concepts. An ELF file can be an executable, shared library, relocatable object, or kernel module; the parser therefore records both the container and detected artifact class.

| Container | Native classes | Typical Linux examples |
|---|---|---|
| ELF32/ELF64 | Executable, DynamicLibrary, RelocatableObject, KernelModule | /bin/*, /lib*/lib*.so*, .o, .ko |
| GNU ar | StaticArchive | libSomething.a |
| PE/COFF | Executable, DynamicLibrary, Driver, RelocatableObject | .exe, .dll, .sys, .obj |
| Mach-O | Executable, DynamicLibrary | macOS binaries and dylibs |
| Raw | RawArtifact | firmware, dumps, unknown blobs |
| WebAssembly | Planned VM/IR adapter | .wasm |

## ELF shared libraries

Slecompiler™ should preserve SONAME and filename; DT_NEEDED dependencies; dynamic and regular symbol tables; symbol version definitions and requirements; PLT/GOT-related relocation evidence; TLS sections and relocations; init/fini arrays; GNU build ID and note records; GNU hash and SysV hash tables; and section/program-header permissions and load ranges.

## Linux .ko kernel modules

A .ko is normally an ELF relocatable kernel module. Slecompiler™ should inspect, when present:

- ELF headers, section headers and relocations;
- .modinfo metadata;
- vermagic;
- module license, author, description and alias records;
- depends information;
- exported symbol records;
- __ksymtab and related symbol evidence;
- module initialization and cleanup entry-point evidence;
- architecture and ABI indicators;
- compiler/build fingerprints;
- strings and references to devices, buses, protocols, firmware, sysfs/procfs interfaces, and subsystem names.

Analysis remains static. A .ko is never inserted into a running kernel by Slecompiler™.

## Archives and object files

For .a archives, the archive is modeled as a container of members. Each member can be analyzed as an ELF/COFF relocatable object while retaining archive/member provenance. .o files are analyzed directly as relocatable objects, including symbols and relocation sites.

All parsers must validate bounds before reading a header, table, string, section, segment, relocation, or symbol. Malformed input is reported as an analysis error rather than executed.
