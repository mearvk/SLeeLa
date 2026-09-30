<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa macOS Build

This platform build surface targets supported macOS releases with Apple Clang.

## Commands

From the repository root:

```sh
make -C build/macos
```

The Makefile dispatches to the existing `scripts/build-macos.sh` core build and does not duplicate the native source list.

## Native footing

The common C/C++ runtime uses the Darwin/POSIX backend under `impl/core`. Skya has a macOS-native C++ driver/engine under `telephony-skya/drivers/platform/macos` and `telephony-skya/native`.

## Outputs

Core output: `impl/build/sleela` and `impl/build/nordshrift`.
Skya focused output: `build/skya/macos/skya`.
Slecompiler output: `build/slecompiler/macos/cmake`.