# SLVM/9 macOS adapter build foundation

Use Apple Clang. Native Darwin/APFS behavior belongs in the adapter and is reported through the common filesystem capability contract.

Example contract compilation:

    cc -std=c11 -Wall -Wextra -Wpedantic -Iinclude -c src/slvm9.c -o build/slvm9.o
