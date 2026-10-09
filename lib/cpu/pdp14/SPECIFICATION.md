# PDP-14 Specification

| Property | SLeeLa profile |
|---|---|
| System type | Industrial control processor |
| Primary purpose | Boolean logic, sequencing, and machine control |
| I/O | Configured discrete input/output points |
| Program model | Control program / instruction sequence defined by selected profile |
| Internal state | Profile-defined logic state and control flags |
| Timing | Unspecified unless supported by a verified hardware profile |
| General-purpose CPU assumptions | Not applicable by default |

The PDP-14 should be modeled as a control system, not as a conventional register-rich workstation CPU. Documented details, derived implementation behavior, estimates, and unknowns must remain clearly distinguished.