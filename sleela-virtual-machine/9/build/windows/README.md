# SLVM/9 Windows adapter build foundation

Use MSVC or LLVM/Clang. The adapter must translate SLeeLa filesystem capabilities to native Windows volume/file-handle APIs without changing the common SLVM/9 contract.

Example contract compilation:

    cl /std:c11 /W4 /Iinclude /c src\\slvm9.c /Fobuild\\slvm9.obj
