<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">





# SLeeLa Linux Build

This platform build surface targets Linux with GCC/G++ or Clang/Clang++.

## Commands

From the repository root:

```sh
make -C build/linux
```

The Makefile dispatches to the existing `scripts/build-linux.sh` core build and does not duplicate the native source list.

## Native footing

The common C/C++ runtime uses the Linux/POSIX backend under `impl/core`. Skya also has a Linux-native C++ driver/engine under `telephony-skya/drivers/platform/linux` and `telephony-skya/native`.

## Outputs

Core output: `impl/build/sleela` and `impl/build/nordshrift`.
Skya focused output: `build/skya/linux/skya`.