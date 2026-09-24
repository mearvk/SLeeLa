# Skya Native Platform Builds

The Skya-specific build directory lives here, inside `telephony-skya/build/`.

## Linux

    ./telephony-skya/build/linux/build.sh

Output: `telephony-skya/build/linux/skya`

## Windows 10+

    powershell -ExecutionPolicy Bypass -File .\telephony-skya\build\windows\build.ps1

Output: `telephony-skya/build/windows/skya.exe`

## macOS

    ./telephony-skya/build/macos/build.sh

Output: `telephony-skya/build/macos/skya`

These are direct native builds of the current Skya engine. They complement,
rather than replace, the integrated SLeeLa `sleela skya ...` build path.
