# SLVM/8 Build

From sleela-virtual-machine/8:

    make
    make test
    make check

The build uses C11 and does not require a platform-specific runtime library.

Platform entry points:
- build/linux/Makefile
- build/windows/README.md
- build/macos/README.md

The root Makefile is authoritative for the contract library.
