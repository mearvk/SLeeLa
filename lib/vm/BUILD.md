# Building VM Pieces

A VM source object may produce independent C and C++ compilation units. C provides portable ABI-level construction; C++ provides optional orchestration. The final SLVM or SLJVM is assembled only after architecture/resource/security validation.

The same source model supports Linux, Windows, and macOS through platform adapters. Physical limits are measured or supplied as constraints and never grant additional capabilities.
