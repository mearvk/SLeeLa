# PDP-14 State Model

The PDP-14 profile uses control-oriented state rather than assuming a conventional general-purpose register file.

- **Program position:** current control instruction or step, represented according to the verified program model.
- **Input image:** snapshot of configured external input points.
- **Output image:** pending values for configured output points.
- **Internal logic state:** relays, flags, timers, counters, or other elements only when supported by the selected profile.
- **Execution status:** scan state, fault state, and diagnostic status.

Names above are SLeeLa modeling concepts, not claims that each was a physically visible CPU register. Exact widths, reset values, and persistence rules are profile-defined.