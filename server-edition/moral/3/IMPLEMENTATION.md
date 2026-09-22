# Service 3 Implementation Style

Service 3 introduces a remedial lifecycle without making the server
self-modifying.

```
Configuration
     |
     v
Preflight
     |
     +---- failure ----> recovery.log + stop
     |
     v
Single-instance lock
     |
     v
Server.sleela
     |
     +---- failure ----> recovery.log + nonzero exit
     |
     v
Completion evidence
```

The implementation deliberately avoids automatic source rewriting, silent
binary substitution, arbitrary package downloads, automatic public network
exposure, and treating a log message as proof of integrity.

The recovery record is an operational artifact. It does not replace
cryptographic verification or independent verification.

NIST recommends threat modeling, automated testing, static analysis, black-box
and structural tests, historical tests, and fuzzing as broadly applicable
software-verification techniques. Service 3 is complementary to repository
testing, not a replacement for it. citeturn0search4
