<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">







# SLeeLa Windows 10+ Build

This platform build surface targets Windows 10 and later.

## Commands

From PowerShell:

``
powershell
powershell -ExecutionPolicy Bypass -File .\\build-windows.ps1
```

Or from a GNU Make environment:

```text
make -C build/windows
```

The Makefile dispatches to the existing Windows PowerShell build script. The native C/C++ runtime uses the Win32 backend under `impl/core`, including Winsock, Win32 threads, Win32 file/pipe APIs, LoadLibrary, ConPTY, and Windows timing.

## Native footing

Skya has a Windows-native C++ driver under `telephony-skya/drivers/platform/windows` and its native engine is compiled with the Windows build script.

## Outputs

Core output: `impl\\build\\sleela.exe` and `impl\\build\\nordshrift.exe`.
Skya focused output: `build\\skya\\windows\\skya.exe`.
Slecompiler output: `build\\slecompiler\\windows\\cmake-<architecture>`.