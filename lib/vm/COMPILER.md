# SLeeLa VM Compiler Construction

The compiler treats VM construction as a compilation target. It analyzes SLeeLa source requirements, target architecture, OS, ABI, physical limits, capabilities, and module dependencies before selecting SLVM or SLJVM pieces.

The generated native foundation uses the stable C ABI. C++ may provide orchestration and richer builders, but cannot bypass the C security/capability boundary.

Piecewise output can include headers, C modules, C++ modules, linker inputs, resource manifests, and verification metadata. The compiler must reject an impossible resource plan rather than silently changing program semantics.
