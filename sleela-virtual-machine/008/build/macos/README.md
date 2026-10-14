<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLVM/8 macOS Build

Use Apple Clang with C11 support:

    cc -std=c11 -Wall -Wextra -Wpedantic -Iinclude -c src/slvm8.c -o build/slvm8.o
    ar rcs build/libslvm8.a build/slvm8.o

macOS integration remains behind the SLeeLa broker and adapter boundary.