# SLeeLa Decompiler Formats

Supported and planned formats are intentionally explicit.

| Format | Initial role |
|---|---|
| PE/COFF | Windows EXE, DLL, SYS/driver |
| ELF | Linux executable, shared object, kernel/module analysis |
| Mach-O | macOS executable and dynamic library analysis |
| Raw | Firmware, memory images and unknown containers |
| WebAssembly | Planned VM/IR adapter |

The parser must validate bounds before reading every header, table or section. Malformed input is reported as an analysis error rather than executed.
