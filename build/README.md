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
