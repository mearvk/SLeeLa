# C++ Compile Commands Contract

Version: 0.2.0-dev

The C++ integration consumes `compile_commands.json` and preserves the exact compilation environment needed for semantic analysis.

Required information:
1. compiler executable;
2. working directory;
3. source path;
4. include paths;
5. macro definitions;
6. C++ language standard;
7. target/sysroot;
8. relevant warning and ABI flags.

A module may contain both C and C++ compilation entries. The IDE must select the correct entry by source path and must not silently apply C++ arguments to a C translation unit or vice versa.