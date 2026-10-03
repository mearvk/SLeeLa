# SLVM/8 Windows Build

Use MSVC or LLVM/Clang with C11 support.

MSVC entry point:

    cl /std:c11 /W4 /Iinclude /c src\slvm8.c /Fobuild\slvm8.obj
    lib /OUT:build\slvm8.lib build\slvm8.obj

The execution contract is platform-neutral; Windows OS integration remains behind the SLeeLa broker and adapter boundary.
