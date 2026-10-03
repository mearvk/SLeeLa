# SLVM/9 Build

Root build:

    make
    make test
    make check

The contract library is C11 and portable.

Native adapter directories are reserved under:

    build/linux/
    build/windows/
    build/macos/

The native adapter code must remain below the common SLVM/9 API and must not change SLeeLa semantics.
