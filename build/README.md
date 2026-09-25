# Skya Platform Builds

Direct native build entry points for Skya on Linux, Windows 10+, and macOS.

## Linux

Requirements: GCC or Clang with C++17 support.

    chmod +x build/skya-linux.sh
    ./build/skya-linux.sh

Output: `build/skya/linux/skya`

## Windows 10+

Requirements: MinGW-w64 GCC/G++ and PowerShell.

    powershell -ExecutionPolicy Bypass -File .\\build\\skya-windows.ps1

Output: `build\\skya\\windows\\skya.exe`

## macOS

Requirements: Xcode Command Line Tools and clang++.

    chmod +x build/skya-macos.sh
    ./build/skya-macos.sh

Output: `build/skya/macos/skya`

## Runtime roles

    skya --server --room lobby
    skya --client --room lobby
    skya --both --room lobby
    skya --both --http2 --room lobby
    skya --both --http3 --room lobby

These scripts build the current native Skya engine directly. The integrated
SLeeLa runtime remains the authoritative `sleela skya ...` path. The current
engine is a lifecycle/room foundation; production transport, media, NAT,
certificate, file-transfer, and firewall integrations remain documented in
`SKYA.md`.


## Slecompiler™ Product Build

Slecompiler is built from the authoritative `decompiler/` source tree.

Linux:
```
chmod +x build/slecompiler-linux.sh
./build/slecompiler-linux.sh
```

macOS:
```
chmod +x build/slecompiler-macos.sh
./build/slecompiler-macos.sh
```

Windows 10+:
```
powershell -ExecutionPolicy Bypass -File .\\build\\slecompiler-windows.ps1
```

Product outputs are isolated under `build/slecompiler/<platform>/`. The
build scripts configure the existing CMake project and do not replace
Slecompiler source.

See `build/PRODUCTS.md` for the product-build boundary and planned expansion.

Slecompiler's CMake project is explicitly C++20. The API examples, CLI, tests, and static library are part of the same configured product build; there is no separate example-only toolchain.

### Slecompiler verification path

After configuration, the native test suite can be invoked from the generated build directory with CTest. The repository also contains five API exemplars, which provide compile-time coverage of the public usage path. Cross-platform CI and packaged release verification remain future build layers.


### Windows 10+ toolchain notes

The Slecompiler Windows entry point uses CMake and PowerShell and does not require a POSIX shell. Visual Studio 2022 can be selected explicitly:

```powershell
powershell -ExecutionPolicy Bypass -File .\\build\\slecompiler-windows.ps1 -Generator "Visual Studio 17 2022" -Architecture x64
```

The script also accepts Win32 or ARM64 for Visual Studio generators. CMake-supported MinGW-w64 toolchains remain possible when selected through the CMake environment/generator.

### macOS toolchain notes

The Slecompiler macOS entry point requires Xcode Command Line Tools and CMake 3.20+. It defaults to the host architecture and a macOS 11.0 deployment target. Both are configurable:

```sh
CMAKE_OSX_ARCHITECTURES="x86_64;arm64" MACOSX_DEPLOYMENT_TARGET=11.0 ./build/slecompiler-macos.sh
```

This permits a universal macOS build when the installed SDK/toolchain supports both architectures.
