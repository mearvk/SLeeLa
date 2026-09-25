# Sleelavirin™ Scanners

Scanners are external or internal engines that consume a Sleelavirin scan request and return a normalized verdict.

## Provider model

``
SLeeLa Package
      |
Sleelavirin Request
      |
+-----+----------------+
|                      |
Native SLeeLa       External AV
Scanner              Provider
|                      |
+----------+-----------+
           |
     Normalized Verdict
```

### Windows

AMSI is a primary integration reference. Microsoft documents APIs for initializing a scanning session, scanning buffers/strings, correlating requests, and interpreting malware results. https://learn.microsoft.com/en-us/windows/win32/amsi/antimalware-scan-interface-functions

### Other platforms

Linux and macOS adapters should use the locally installed security provider or an explicitly declared scanner. Sleelavirin must not assume that one vendor or engine exists on every system.
