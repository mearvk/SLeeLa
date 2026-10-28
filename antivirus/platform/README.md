<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# Sleelavirin™ Platform Adapters

Platform adapters isolate operating-system security APIs from the portable SLeeLa antivirus model.

Initial targets:

- Linux
- Windows XP+, Windows 7+, Windows 10+
- macOS (Intel & Apple Silicon)

The portable native C/C++ engine already **compiles and runs on all of these**
from one source tree — see **`../PLATFORMS.md`** for the full build matrix
(CMake, the Windows PowerShell/`nmake` paths, and the macOS script) and the
Windows API-floor / XP notes.

These platform *adapters* are a separate concern: they bind OS **security APIs**
to the portable scanner-provider model. Windows integration may use AMSI where
available; Linux/macOS implementations should expose equivalent scanner-provider
abstractions without pretending that the operating systems provide identical
APIs. The adapters are optional — the native engine needs none of them to build
or run.