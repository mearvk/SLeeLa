# Sleelavirin™ Platform Adapters

Platform adapters isolate operating-system security APIs from the portable SLeeLa antivirus model.

Initial targets:

- Linux
- Windows 10+
- macOS

Windows integration may use AMSI where available. Linux/macOS implementations should expose equivalent scanner-provider abstractions without pretending that the operating systems provide identical APIs.
