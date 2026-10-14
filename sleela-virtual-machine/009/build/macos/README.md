<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLVM/9 macOS adapter build foundation

Use Apple Clang. Native Darwin/APFS behavior belongs in the adapter and is reported through the common filesystem capability contract.

Example contract compilation:

    cc -std=c11 -Wall -Wextra -Wpedantic -Iinclude -c src/slvm9.c -o build/slvm9.o